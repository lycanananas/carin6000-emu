# carin6000-emu

Emulator nawigacji Philips CARiN (Renault Carminat, Mk2) oparty na MAME. 68EC030 z OS-9, wideo i napęd CD z rodziny CD-i (MCD212, CDIC).

Stan: instalacja oprogramowania z CD do flasha przechodzi, start z flasha uruchamia całą aplikację MMI bez power down (zapłon i wyświetlacz na magistrali nav są emulowane na sztywno). Ekran wideo działa (plansza startowa z ostrzeżeniem i przyciskiem OK); brakuje jeszcze wejścia z pilota/auta (szczegóły w [docs/carin_testowanie.md](docs/carin_testowanie.md), sekcja „Gdzie jesteśmy”).

## Zawartość repo

| Katalog | Co jest w środku |
|---|---|
| `mame/` | Drzewo źródeł MAME przycięte do jednego sterownika: `src/mame/philips/carin.cpp` (+ `carin.h`, `carin_trace.h`) i urządzeń, których używa. Bazą jest upstream MAME, commit `b1f71f8f`. |
| `roms/carin/` | `bootrom_0209.bin` (bootrom 2.09) i `nv.img` (seed pamięci NV). |
| `cd/` | Płyta z oprogramowaniem nawigacji, nie jest w repo (`.gitignore`): `carin_nav_sw.chd` albo `carin_nav_sw.cue` + `.img` (+ opcjonalnie `.ccd`/`.sub`). |
| `tools/` | `carin.py` (budowanie i uruchamianie) oraz narzędzia analityczne. |
| `docs/` | Notatki z reverse engineeringu. |
| `state/` | Stany maszyny (NV + flash). Ignorowany przez gita. |
| `logs/` | Log builda i logi kolejnych uruchomień. Ignorowany przez gita. |
| `temp/` | Brudnopis: wyciągnięte pliki, projekty Ghidry itp. Ignorowany przez gita. |

## Wymagania

- kompilator C++ (GCC lub Clang), `make`, Python 3.9+
- biblioteki deweloperskie MAME dla Linuksa: SDL2, SDL2_ttf, fontconfig, ALSA/PulseAudio, Qt 6 (debugger) i X11
  - Arch: `pacman -S base-devel sdl2 sdl2_ttf fontconfig libpulse alsa-lib qt6-base libxinerama`
  - Debian/Ubuntu: `apt install build-essential libsdl2-dev libsdl2-ttf-dev libfontconfig-dev libpulse-dev libasound2-dev qt6-base-dev libxinerama-dev`
- `chdman` (pakiet `mame-tools` lub `mame`), żeby zrobić obraz CHD

## Szybki start

```bash
# 1. Budowanie (pierwszy raz kilka–kilkanaście minut, zależnie od liczby rdzeni)
tools/carin.py build

# 2. Płyta: wrzuć carin_nav_sw.chd do cd/, albo carin_nav_sw.cue + .img i zrób z nich CHD:
tools/carin.py chd

# 3. Pierwsze uruchomienie: instalacja z CD do flasha (ok. 150 s czasu emulacji, przy
#    domyślnym -nothrottle kilka sekund; konsola pokaże "il: Release loaded correctly",
#    potem zamknij MAME)
tools/carin.py gui --seed --state installed

# 4. Kolejne uruchomienia: start z zainstalowanego oprogramowania
tools/carin.py gui --state installed
```

Instalację można też zrobić bez okna, w kilka sekund: `tools/carin.py run 200 --seed --state installed`.

Wszystkie opcje: `tools/carin.py --help` oraz `tools/carin.py run --help` / `gui --help`. Skrypt nie czyta zmiennych środowiskowych.

## Stan maszyny: seed albo własny seed

Stan to pliki w `state/<nazwa>/`: `nv` (8 KB NV), `flash0..3` (główny flash), `bootflash` (bootrom + flash slave'a) i `mk48t08`. Domyślna nazwa to `default`. Zamiast nazwy można podać ścieżkę. MAME pracuje bezpośrednio na tym katalogu, więc każde uruchomienie kontynuuje poprzednie.

**Z seeda** (`--seed`): flash jest pusty, NV pochodzi z `roms/carin/nv.img`. Bootrom nie znajdzie oprogramowania we flashu, więc załaduje instalator z CD i zaprogramuje flash (ok. 150 s czasu emulacji, a bez okna to kilka sekund).

```bash
tools/carin.py run 200 --seed                    # instalacja do state/default
tools/carin.py run 200 --seed --state installed  # instalacja do state/installed
```

**Z własnego seeda** (`--load KATALOG`): stan jest zastępowany kopią plików z podanego katalogu, np. zachowanego snapshotu. Oryginał zostaje nienaruszony.

```bash
tools/carin.py run 60 --save-to ~/carin_snapshots/po_instalacji --state installed   # zrób snapshot
tools/carin.py gui --load ~/carin_snapshots/po_instalacji --state proba             # start z niego
```

**Bez flag** stan jest kontynuowany, a jeśli katalog jest pusty, maszyna startuje jak z seeda.

| Opcja | Działanie |
|---|---|
| `--state NAZWA\|ŚCIEŻKA` | który stan (domyślnie `state/default`) |
| `--seed` | wyczyść stan: pusty flash, NV z `nv.img` |
| `--load KATALOG` | wczytaj stan z katalogu (własny seed) |
| `--save-to KATALOG` | po zakończeniu skopiuj stan do katalogu |

## Bez okna (headless)

```bash
tools/carin.py run            # 30 s emulacji
tools/carin.py run 90         # 90 s
tools/carin.py run 90 -b      # najpierw przebuduj
```

Emulacja idzie najszybciej, jak się da (`--realtime` zwalnia do czasu rzeczywistego), bez obrazu i dźwięku. Na końcu skrypt drukuje raport (~80 linii): zdarzenia, liczniki, gdzie stoi CPU, końcówkę konsoli OS-9 i kanałów śledzenia.

## Z oknem (GUI)

```bash
tools/carin.py gui --state installed
tools/carin.py gui --state installed --debug     # z debuggerem MAME
tools/carin.py gui --state installed --realtime  # w tempie prawdziwego urządzenia
tools/carin.py gui -- -window -nomax             # dodatkowe opcje MAME po "--"
```

Otwierają się dwa okna: wideo nawigacji i konsola OS-9 (terminal A). Emulacja domyślnie idzie najszybciej, jak się da (`-nothrottle`); `--realtime` zwalnia ją do prędkości prawdziwego urządzenia. Jeśli klawisze nie trafiają do konsoli, przełącz klawiaturę Scroll Lockiem.

## Logi

Każde uruchomienie dostaje własny katalog `logs/<data>_<czas>_<run|gui>_<stan>/`, a `logs/latest` wskazuje na ostatni:

- `uart_a.txt`, `uart_b.txt`: konsole OS-9 (DUART kanał A/B)
- `report.txt`: raport jak po `run`
- `mame.log`: wyjście MAME
- `trace/`: `events.log` (czytaj pierwszy), `summary.txt` i kanały `ipc`, `mailbox`, `mcasic`, `arcnet`, `duart`, `nv`, `irq`, `cdrom`, `video`

Opcje: `--trace off`, `--trace ipc,nv` (tylko wybrane kanały), `--trace-cap BAJTY`. Raport dla starego katalogu: `tools/carin.py report logs/<katalog>`. Log builda: `logs/build.log`.

Katalog logów z sesji GUI nadaje się do wysłania jako feedback.

## Sonda (zrzut stanu, podgląd magistrali)

```bash
tools/carin.py run 13 --state installed --probe-at 12.3 --probe-mem 9F8800:64 --probe-wtap 9F8800-9F88FF
```

Wynik trafia do `trace/probe.log` w katalogu logów. Pozostałe opcje to `--probe-rtap` (odczyty), `--probe-bp "adres:wyrażenia;..."`, `--probe-exit`, `--probe-regs` i `--probe-stack`.

## Narzędzia

| Skrypt | Do czego |
|---|---|
| `tools/carin.py` | build, run, gui, chd, report |
| `tools/extract_cd.py [obraz] [katalog]` | wyciąga pliki z płyty (domyślnie do `temp/extracted_cd/`); raport używa stamtąd `loader.ini` do nazywania modułów |
| `tools/carin_flash_modules.py state/installed [katalog]` | lista modułów OS-9 we flashu, opcjonalnie wyciąga je do katalogu |
| `tools/unpack_os9.py plik katalog` | wyciąga moduły OS-9 z dowolnego pliku (np. `loader.ini`) |
| `tools/ghidra/export_os9_modules.py [moduły] [wyjście]` | analiza modułów w headless Ghidrze i eksport do Markdown |
| `tools/carin_probe.lua` | skrypt Lua sondy, uruchamiany przez `carin.py run --probe-*` |

## Praca na sterowniku

Kod sterownika jest w `mame/src/mame/philips/carin.cpp`. Po zmianach wystarczy `tools/carin.py build` albo `run -b`. Drzewo MAME zawiera tylko pliki potrzebne do zbudowania tego sterownika, więc dodanie urządzenia spoza obecnego zestawu wymaga dogrania jego źródeł z upstreamu (ten sam commit `b1f71f8f`).

## Uwagi prawne

Bootrom, obraz NV i oprogramowanie z płyty pochodzą z urządzenia z lat 90., którego producent od dawna go nie wspiera. Traktujemy je jako abandonware o znaczeniu wyłącznie hobbystycznym. MAME jest na licencji GPL-2.0+ (pliki w `mame/` zachowują swoje nagłówki licencyjne, patrz `mame/COPYING`).
