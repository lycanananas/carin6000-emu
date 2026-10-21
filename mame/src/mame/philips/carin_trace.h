// license:BSD-3-Clause
// Compact per-channel text tracing for the CARiN driver: small files meant to be read by tools, not people.
#ifndef MAME_PHILIPS_CARIN_TRACE_H
#define MAME_PHILIPS_CARIN_TRACE_H

#pragma once

#include <algorithm>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

// Writes trace/<channel>.log (truncated every run) plus trace/summary.txt.
// CARIN_TRACE=off disables everything, CARIN_TRACE=a,b enables only the listed channels,
// CARIN_TRACE_CAP=<bytes> changes the per-channel size cap.
class carin_trace
{
public:
	enum channel { EVENTS, IPC, MAILBOX, MCASIC, ARCNET, DUART, NV, IRQ, CDROM, VIDEO, CHANNEL_COUNT };

	carin_trace() = default;
	carin_trace(const carin_trace &) = delete;
	carin_trace &operator=(const carin_trace &) = delete;
	~carin_trace() { finish(); }

	void open(const char *dir = "trace")
	{
		m_dir = dir;
		std::error_code ec;
		std::filesystem::create_directories(m_dir, ec);

		if (const char *cap = std::getenv("CARIN_TRACE_CAP"))
			m_cap = std::strtoull(cap, nullptr, 0);

		const char *mask = std::getenv("CARIN_TRACE");
		for (int ch = 0; ch < CHANNEL_COUNT; ch++)
		{
			m_enabled[ch] = !mask || (std::strcmp(mask, "off") != 0 && std::strstr(mask, s_names[ch]) != nullptr);
			if (mask && std::strcmp(mask, "all") == 0)
				m_enabled[ch] = true;
			if (m_enabled[ch])
				m_chan[ch].out.open(m_dir / (std::string(s_names[ch]) + ".log"), std::ios::trunc);
		}
		m_opened = true;
	}

	// One text line; identical consecutive lines are folded into "t=a..b <text> xN".
	void line(channel ch, double t, const char *fmt, ...)
	{
		if (!m_opened || !m_enabled[ch])
			return;
		char body[256];
		va_list ap;
		va_start(ap, fmt);
		std::vsnprintf(body, sizeof(body), fmt, ap);
		va_end(ap);

		m_last_t = t;
		chan &c = m_chan[ch];
		if (c.count && c.body == body)
		{
			c.count++;
			c.last_t = t;
			return;
		}
		flush_pending(ch);
		c.body = body;
		c.first_t = c.last_t = t;
		c.count = 1;
	}

	// Cheap counter for things that are only worth a number in the summary.
	void count(const char *name, uint64_t n = 1) { m_counters[name] += n; }

	void pc_sample(uint32_t pc) { m_pc_hist[pc]++; }

	// Shared RAM access: logged only when the (kind, word, pc) key is new or its value changed; sequential new words merge into ranges.
	void ipc(double t, char kind, uint32_t addr, uint32_t value, uint32_t pc)
	{
		if (!m_opened)
			return;
		m_last_t = t;
		const uint64_t key = (uint64_t(pc) << 17) | (uint64_t(addr & 0xfffc) << 1) | (kind == 'W' ? 1 : 0);
		ipc_key &k = m_ipc_keys[key];
		k.count++;
		const bool first = k.count == 1;
		const bool changed = !first && k.value != value;
		k.value = value;
		if (first || (changed && k.changes < 8))
		{
			if (changed)
				k.changes++;
			range_feed(IPC, kind, addr, 4, value, kind == 'R', pc, t);
		}
	}

	// NV access: reads merge into ranges regardless of value, writes only when the value repeats.
	void nv(double t, char kind, uint32_t offset, uint8_t value, uint32_t pc)
	{
		if (!m_opened)
			return;
		m_last_t = t;
		range_feed(NV, kind, offset, 1, value, kind == 'W', pc, t);
	}

	// Called periodically: makes pending lines visible and refreshes the summary every few seconds.
	void tick(double t)
	{
		if (!m_opened)
			return;
		m_last_t = t;
		for (int ch = 0; ch < CHANNEL_COUNT; ch++)
			flush_all(channel(ch));
		if (t - m_last_summary >= 4.0)
		{
			m_last_summary = t;
			write_summary();
		}
	}

	void finish()
	{
		if (!m_opened || m_finished)
			return;
		m_finished = true;
		for (int ch = 0; ch < CHANNEL_COUNT; ch++)
			flush_all(channel(ch));
		write_summary();
	}

private:
	static constexpr const char *s_names[CHANNEL_COUNT] = { "events", "ipc", "mailbox", "mcasic", "arcnet", "duart", "nv", "irq", "cdrom", "video" };

	struct chan
	{
		std::ofstream out;
		std::string body;
		double first_t = 0, last_t = 0;
		unsigned count = 0;
		uint64_t lines = 0, bytes = 0;
		bool truncated = false;
		std::deque<std::string> tail;
	};

	struct range
	{
		bool valid = false, same_value = false;
		char kind = 0;
		uint32_t pc = 0, start = 0, end = 0, value = 0, step = 1;
		unsigned n = 0;
		double t = 0;
		uint32_t last = 0;
		int dir = 0;
	};

	struct ipc_key
	{
		uint32_t value = 0;
		uint32_t count = 0;
		uint32_t changes = 0;
	};

	void range_feed(channel ch, char kind, uint32_t addr, uint32_t step, uint32_t value, bool same_value, uint32_t pc, double t)
	{
		if (!m_enabled[ch])
			return;
		range &r = m_range[ch];
		if (r.valid && r.kind == kind && r.pc == pc && (!r.same_value || value == r.value))
		{
			if (r.n > 1 && addr == r.last)
			{
				return;
			}
			if (r.dir >= 0 && addr == r.last + step)
			{
				r.dir = 1;
				r.last = r.end = addr;
				r.n++;
				return;
			}
			if (r.dir <= 0 && r.last >= step && addr == r.last - step)
			{
				r.dir = -1;
				r.last = r.start = addr;
				r.n++;
				return;
			}
		}
		flush_range(ch);
		r = { true, same_value, kind, pc, addr, addr, value, step, 1, t, addr, 0 };
	}

	void flush_range(channel ch)
	{
		range &r = m_range[ch];
		if (!r.valid)
			return;
		r.valid = false;
		flush_pending(ch);
		char text[160];
		if (r.n == 1)
			std::snprintf(text, sizeof(text), "t=%.3f %c %05X val=%X pc=%06X", r.t, r.kind, r.start, r.value, r.pc);
		else if (r.same_value)
			std::snprintf(text, sizeof(text), "t=%.3f %c %05X-%05X n=%u val=%X pc=%06X", r.t, r.kind, r.start, r.end, r.n, r.value, r.pc);
		else
			std::snprintf(text, sizeof(text), "t=%.3f %c %05X-%05X n=%u pc=%06X", r.t, r.kind, r.start, r.end, r.n, r.pc);
		emit(ch, text);
	}

	void flush_pending(channel ch)
	{
		chan &c = m_chan[ch];
		if (!c.count)
			return;
		char head[64];
		if (c.count > 1)
			std::snprintf(head, sizeof(head), "t=%.3f..%.3f ", c.first_t, c.last_t);
		else
			std::snprintf(head, sizeof(head), "t=%.3f ", c.first_t);
		std::string text = head + c.body;
		if (c.count > 1)
			text += " x" + std::to_string(c.count);
		c.count = 0;
		emit(ch, text.c_str());
	}

	void flush_all(channel ch)
	{
		flush_range(ch);
		flush_pending(ch);
		if (m_enabled[ch])
			m_chan[ch].out.flush();
	}

	void emit(channel ch, const char *text)
	{
		chan &c = m_chan[ch];
		c.lines++;
		c.tail.emplace_back(text);
		if (c.tail.size() > 6)
			c.tail.pop_front();
		if (c.truncated)
			return;
		const size_t len = std::strlen(text) + 1;
		if (c.bytes + len > m_cap)
		{
			c.truncated = true;
			c.out << "# TRUNCATED: channel cap reached, see summary.txt\n";
			return;
		}
		c.out << text << '\n';
		c.bytes += len;
	}

	void write_summary()
	{
		std::ofstream s(m_dir / "summary.txt", std::ios::trunc);
		char buf[200];
		std::snprintf(buf, sizeof(buf), "# carin trace summary, last event t=%.3f\n", m_last_t);
		s << buf << "## channels (lines written to <channel>.log)\n";
		for (int ch = 0; ch < CHANNEL_COUNT; ch++)
		{
			if (!m_enabled[ch])
				continue;
			s << s_names[ch] << ": lines=" << m_chan[ch].lines << " bytes=" << m_chan[ch].bytes << (m_chan[ch].truncated ? " TRUNCATED" : "") << '\n';
		}

		s << "## counters\n";
		std::vector<std::pair<std::string, uint64_t>> counters;
		for (const auto &kv : m_counters)
			counters.emplace_back(kv.first, kv.second);
		std::sort(counters.begin(), counters.end());
		for (const auto &kv : counters)
			s << kv.first << "=" << kv.second << '\n';

		s << "## pc samples (top 12, symbolise with tools/carin_summary.py)\n";
		std::vector<std::pair<uint32_t, uint32_t>> pcs(m_pc_hist.begin(), m_pc_hist.end());
		std::sort(pcs.begin(), pcs.end(), [] (const auto &a, const auto &b) { return a.second > b.second; });
		for (size_t i = 0; i < pcs.size() && i < 12; i++)
		{
			std::snprintf(buf, sizeof(buf), "pc=%06X n=%u\n", pcs[i].first, pcs[i].second);
			s << buf;
		}

		s << "## shared RAM busiest (kind addr pc count last_value)\n";
		struct row { uint64_t key; ipc_key k; };
		std::vector<row> rows;
		for (const auto &kv : m_ipc_keys)
			rows.push_back({ kv.first, kv.second });
		std::sort(rows.begin(), rows.end(), [] (const row &a, const row &b) { return a.k.count > b.k.count; });
		for (size_t i = 0; i < rows.size() && i < 12; i++)
		{
			std::snprintf(buf, sizeof(buf), "%c %04X pc=%06X n=%u val=%X\n", (rows[i].key & 1) ? 'W' : 'R',
				unsigned((rows[i].key >> 1) & 0xfffc), unsigned(rows[i].key >> 17), rows[i].k.count, rows[i].k.value);
			s << buf;
		}

		s << "## last lines per channel\n";
		for (int ch = 0; ch < CHANNEL_COUNT; ch++)
		{
			if (!m_enabled[ch] || m_chan[ch].tail.empty())
				continue;
			s << "[" << s_names[ch] << "]\n";
			for (const std::string &l : m_chan[ch].tail)
				s << "  " << l << '\n';
		}
	}

	std::filesystem::path m_dir;
	bool m_opened = false, m_finished = false;
	bool m_enabled[CHANNEL_COUNT] = {};
	uint64_t m_cap = 256 * 1024;
	double m_last_t = 0, m_last_summary = 0;
	chan m_chan[CHANNEL_COUNT];
	range m_range[CHANNEL_COUNT];
	std::unordered_map<const char *, uint64_t> m_counters;
	std::unordered_map<uint32_t, uint32_t> m_pc_hist;
	std::unordered_map<uint64_t, ipc_key> m_ipc_keys;
};

#endif // MAME_PHILIPS_CARIN_TRACE_H
