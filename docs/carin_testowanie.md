# CARiN: testowanie i uruchamianie

Wszystkie komendy z katalogu głównego repo. Wymagane w `emu/mame/roms/carin/`: `bootrom_0209.bin` i `nv.img`; płyta: `images/52.chd`.

## 1. Budowanie i test bez okna

```bash
tools/carin_run.sh -b 30   # -b = przebuduj, 30 = sekundy emulacji
tools/carin_run.sh 30      # bez budowania
```

Skrypt uruchamia emulator bez grafiki i drukuje raport (~80 linii, też w `emu/mame/report.txt`). Konsole OS-9 lądują w `emu/mame/uart_a.txt` i `uart_b.txt`.

## 2. Pierwsze uruchomienie = instalacja z CD

Przy pustym flashu bootrom ładuje instalator z CD (`loader.ini`), a `il` programuje flash (ok. 150 s emulacji). Zapisz wynik i od tej pory startuj z flasha:

```bash
CARIN_STATE_SAVE=nvram/carin_installed tools/carin_run.sh 200   # instalacja, zapis stanu
CARIN_STATE=nvram/carin_installed tools/carin_run.sh 60         # start z zainstalowanego softu
```

Gotowa kopia po instalacji jest w `nvram/carin_installed/`.

## 3. Stan maszyny (NV i flash)

Emulator trzyma w `emu/mame/nvram/carin/`: `nv` (8 KB NV), `flash0..3` (główny flash 0x1200000) i `bootflash` (bootrom + flash slave'a). `carin_run.sh` domyślnie startuje od zera (nv.img, pusty flash), `carin_gui.sh` kontynuuje poprzedni stan. Opcje obu skryptów:

| Zmienna | Działanie |
|---|---|
| `CARIN_STATE=<katalog>` | start z zapisanego stanu (pliki `nv`, `flash0..3`, `bootflash`) |
| `CARIN_STATE_SAVE=<katalog>` | zapis stanu po przebiegu |
| `CARIN_NV=<plik>` / `CARIN_NV_SAVE=<plik>` | to samo tylko dla NV |
| `CARIN_FRESH=1` | start od zera (nv.img, pusty flash) |
| `CARIN_KEEP_NV=1`, `CARIN_KEEP_FLASH=1` | `carin_run.sh`: zostaw NV / flash z poprzedniego przebiegu |

## 4. Uruchomienie z oknem (GUI)

```bash
CARIN_STATE=nvram/carin_installed tools/carin_gui.sh   # start z zainstalowanego softu
tools/carin_gui.sh                                     # kontynuacja ostatniej sesji
CARIN_FRESH=1 tools/carin_gui.sh                       # od zera (instalacja z CD)
CARIN_DEBUG=1 tools/carin_gui.sh                       # z debuggerem MAME
tools/carin_gui.sh -b                                  # najpierw przebuduj
```

Otwierają się dwa okna: wideo nawigacji i konsola OS-9 (terminal A). Emulacja idzie w czasie rzeczywistym. Stan maszyny zostaje między sesjami GUI (w przeciwieństwie do `carin_run.sh`). Opcje stanu są te same co w punkcie 3. Dodatkowe opcje MAME można dopisać na końcu.

Po zamknięciu MAME logi sesji (`uart_a.txt`, `uart_b.txt`, `report.txt`, `trace/`) są kopiowane do `logs/gui/<data_czas>/`. Ten katalog możesz podesłać jako feedback. Jeśli klawisze nie trafiają do konsoli, przełącz klawiaturę Scroll Lockiem.

Nie odpalaj jednocześnie GUI i `carin_run.sh`, bo oba korzystają z `emu/mame/nvram/carin/` i `emu/mame/trace/`.

## 5. Logi

Pliki w `emu/mame/trace/` (czyszczone przy każdym uruchomieniu skryptu):

- `summary.txt`: liczniki, próbki PC, ogony kanałów
- `events.log`: kamienie milowe (czytaj pierwszy)
- `ipc`, `mailbox`, `mcasic` (też I2C), `arcnet`, `duart`, `nv`, `irq`, `cdrom`, `video`: po jednym kanale

Zmienne: `CARIN_TRACE=off` (wyłącza), `CARIN_TRACE=ipc,nv` (tylko wybrane kanały), `CARIN_TRACE_CAP=<bajty>` (limit pliku).

## 6. Sonda (zrzut stanu, podgląd magistrali)

```bash
PROBE_AT=12.3 PROBE_MEM=9F8800:64 PROBE_WTAP=9F8800-9F88FF tools/carin_run.sh 13
```

Wynik: `emu/mame/trace/probe.log`. `PROBE_RTAP` to odczyty, `PROBE_EXIT=1` kończy po zrzucie, `PROBE_BP="adres:wyrażenia;..."` ustawia breakpointy.

## 7. Moduły OS-9 z flasha

```bash
python3 tools/carin_flash_modules.py nvram/carin_installed extracted/flash_0209
```

Wypisuje adresy modułów w głównym flashu i wyciąga je do katalogu.

## 8. Gdzie jesteśmy

Instalacja z CD przechodzi, start z flasha uruchamia aplikację MMI. Po ok. 60 s `mmi_root` robi reset (power down), bo `recv_ibus` nie zgłasza gotowości (sygnał 0x6E30), więc procesy MMI nie dostają startu, a `build_screen` nie odpowiada na watchdog (0x6E06). `recv_ibus` czeka na coś z `/pack/rc`, `/rc5`, `/c0/_129` (ARCNET) albo `/ibus`. To następny krok.
