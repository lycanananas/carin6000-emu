# PoC: własny program z płyty (flash nietknięty)

Kopia płyty NAV_SW, w której moduł `il` (instalator, jedyny, który programuje flash) jest podmieniony na własny program. Bootrom startuje z płyty jak przy aktualizacji, `sysgo` z płyty uruchamia „IL”, czyli nasz program. Program:

- rysuje HELLO WORLD i licznik w framebufferze ustawionym przez `videodr`;
- czyta przyciski z `/nav_bus` (ramki `3A 3B 48 kod` i `3A 3B 49 kroki`): VOL+ +1, VOL− −1, pokrętło ±kroki, wciśnięcie pokrętła zeruje, Last message liczy 700 cyfr π (spigot Rabinowitza–Wagona, cyfry pojawiają się na bieżąco; w tym czasie przyciski czekają w kolejce);
- loguje wszystko na UART A (konsola OS-9);
- na starcie zabija `ui_handler` (UI instalatora), który inaczej co chwilę sygnalizuje `sysgo` („unexpected signal 0xccc6”).

## Budowanie

Wymaga `m68k-elf-gcc` oraz `cd/carin_nav_sw.img` i oryginalnego `il` w `../re-safrane/extracted/loader.ini/65_il.mod`.

```bash
make -C poc          # poc/build/il.mod, cd/carin_poc.img + .cue
make -C poc chd      # dodatkowo cd/carin_poc.chd
```

Jak to działa:

1. `src/start.S`: wejście procesu i opakowania wywołań systemowych (`trap #0`).
2. `src/main.c`: program. Kod jest tylko PC-relative i nie ma zmiennych globalnych (moduł OS-9 jest tylko do odczytu), cały stan trzyma na stosie. Makefile odrzuca `.data`/`.bss` i relokacje absolutne.
3. `tools/mkmod.py`: binarka → moduł OS-9 (nagłówek, parzystość, CRC-24, pusty blok idata/irefs, bo F$Fork zawsze go przetwarza). Moduł ma dokładnie długość oryginalnego `il`, więc `loader.ini` zachowuje rozmiar i offsety.
4. `tools/mkdisc.py`: podmienia moduł w obu kopiach `loader.ini` na płycie (`RENAULT/` i `RENAULT/app_sw/`) i przelicza EDC/ECC zmienionych sektorów (`tools/cdsector.py`, sprawdzone na oryginalnych sektorach).

## Test w emulatorze

```bash
tools/carin.py gui --state poc --load state/installed --cd carin_poc
```

`--load` kopiuje stan, więc `state/installed` zostaje nietknięty. Płyta PoC to kopia NAV_SW, więc działa jak aktualizacja: włóż ją do działającej nawigacji, zamknij „Attention!”, soft ustawi flagę sw-load i się zrestartuje, a bootrom załaduje PoC.

## Na prawdziwym sprzęcie

Nagrywanie (obraz MODE2/2352, raw):

```bash
cdrdao write --device /dev/sr0 --driver generic-mmc-raw cd/carin_poc.cue
```

Ryzyka:

1. Nie wiemy, co w aucie każe bootromowi startować z płyty przy ważnym flashu. W emulatorze to linia z ekspandera I/O (zgadnięta). Najpierw sprawdź kopią oryginalnej NAV_SW, czy urządzenie w ogóle startuje z CD-R.
2. `sysgo` z płyty startuje `il_slave` na slavie (tak jak przy normalnej aktualizacji). Bez poleceń od `il` nie powinien niczego programować, ale tego jeszcze nie sprawdziliśmy w RE.
3. Paleta jest nieznana, więc kolory mogą wyjść inne niż w emulatorze. Użyte są indeksy, którymi rysuje oryginalny soft (0x49 tło, 0xFF tekst, 0x92 akcent, 0x00).
4. Czy moduł pośredni wysyła przyciski bez uzgadniania z MMI (które tu nie działa), wyjdzie dopiero w aucie.

Jeśli program się nie uruchomi: wyjmij płytę i zrób reset (wyłącz zapłon), a urządzenie wystartuje z flasha.
