// license:BSD-3-Clause
// copyright-holders:Ryan Holtz

#ifndef MAME_PHILIPS_CARIN_H
#define MAME_PHILIPS_CARIN_H

#include "machine/mc68681.h"
#include "machine/intelfsh.h"
#include "cpu/m68000/m68030.h"
#include "cdislavehle.h"
#include "cdicdic.h"
#include "cdi220_lcd.h"
#include "sound/dmadac.h"
#include "mcd212.h"
#include "bus/rs232/rs232.h"
#include "screen.h"

#include "carin_trace.h"

#include <fstream>

class carin_state : public driver_device
{
public:
	carin_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this, "maincpu")
		, m_lcd(*this, "lcd")
		, m_slave_hle(*this, "slave_hle")
		, m_plane_a(*this, "plane0", 0x80000, ENDIANNESS_BIG)
		, m_plane_b(*this, "plane1", 0x80000, ENDIANNESS_BIG)
		, m_cdic(*this, "cdic")
		, m_cdrom(*this, "cdrom")
		, m_mcd212(*this, "mcd212")
		, m_duart(*this, "duart")
		, m_dmadac(*this, "dac%u", 1U)
		, m_flash(*this, "flash%u", 0U)
		, m_bootflash(*this, "bootflash")
	{ }

	void carin(machine_config &config);

private:
	virtual void machine_start() override ATTR_COLD;

	required_device<m68ec030_device> m_maincpu;
	required_device<cdi220_lcd> m_lcd;
	required_device<cdislave_hle_device> m_slave_hle;
	memory_share_creator<uint16_t> m_plane_a;
	memory_share_creator<uint16_t> m_plane_b;
	// Backed by the "nv" nvram device, which takes its first-boot contents from the "nv" region (nv.img).
	uint8_t m_nv[0x2000] = {};
	required_device<cdicdic_device> m_cdic;
	required_device<cdrom_image_device> m_cdrom;
	required_device<mcd212_device> m_mcd212;
	required_device<mc68681_device> m_duart;
	required_device_array<dmadac_sound_device, 2> m_dmadac;
	// Main flash (flsh, nav software): four 29F040 on the byte lanes of the 32-bit bus.
	required_device_array<amd_29f040_device, 4> m_flash;
	// Boot flash (bootrom, flsh_boot, flsh_voice, flsh_slave1): one byte-wide chip at 0x000000.
	required_device<amd_29f080_device> m_bootflash;
	uint32_t m_mcasic_regs[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00000040 };
	uint8_t m_mcasic_reg12 = 0x10;
	bool m_mcasic_mock_pending = false;
	bool m_mcasic_mock_armed = false;
	uint16_t m_mcasic_mock_status = 0x0040;
	uint8_t m_mcasic_mock_phase = 0;
	uint8_t m_mcasic_boot_status = 0;
	bool m_mcasic_boot_irq_pending = false;
	emu_timer *m_i2c_timer = nullptr;
	uint16_t m_i2c_result = 0;
	bool m_i2c_pending = false;
	uint8_t m_mbox_irq_channel = 0;
	uint32_t m_slave_shared_ram[0x10000 / 4] = {};
	std::ofstream m_uart_out[2];
	carin_trace m_trace;
	double trace_time() { return machine().time().as_double(); }
	uint8_t m_arc_ram[0x800] = {};
	uint8_t m_arc_status = 0x91;
	uint8_t m_arc_imr = 0;
	uint8_t m_arc_config = 0;
	uint8_t m_arc_subadr = 0;
	uint8_t m_arc_nodeid = 0;
	uint8_t m_arc_ptr_hi = 0;
	uint16_t m_arc_address = 0;
	uint8_t m_arc_rx_page = 2;
	emu_timer *m_pc_watch_timer = nullptr;
	emu_timer *m_tick_timer = nullptr;
	bool m_tick_enabled = false;

	template<int Channel> uint16_t plane_r(offs_t offset, uint16_t mem_mask = ~0);
	template<int Channel> void plane_w(offs_t offset, uint16_t data, uint16_t mem_mask = ~0);
	TIMER_CALLBACK_MEMBER(pc_watch_tick);
	TIMER_CALLBACK_MEMBER(system_tick);
	uint32_t mcasic_r(offs_t offset);
	void mcasic_w(offs_t offset, uint32_t data, uint32_t mem_mask = ~0);
	void i2c_control_w(uint16_t data);
	void update_level3_irq();
	TIMER_CALLBACK_MEMBER(i2c_done);
	uint32_t carin_slave_r(offs_t offset, uint32_t mem_mask);
	void carin_slave_w(offs_t offset, uint32_t data, uint32_t mem_mask);
	uint8_t carin_arcnet_r(offs_t offset);
	void carin_arcnet_w(offs_t offset, uint8_t data);
	void carin_arcnet_command(uint8_t data);
	void carin_arcnet_transmit(unsigned page);
	void carin_mailbox_kick(uint8_t channel);
	uint8_t shared_byte(uint32_t offset) const;
	void set_shared_byte(uint32_t offset, uint8_t value);
	void cdic_irq_w(int state);
	uint8_t duart_r(offs_t offset);
	void duart_w(offs_t offset, uint8_t data);
	void duart_irq_w(int state);
	void mcd_irq_w(int state);
	void log_irq(const char *source, unsigned level, int state);
	void carin_mem(address_map &map) ATTR_COLD;
	void carin_cpu_space(address_map &map) ATTR_COLD;
};

#endif // MAME_PHILIPS_CARIN_H
