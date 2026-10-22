# PoC: własny program z płyty (flash nietknięty)

Kopia płyty NAV_SW, w której `sysgo` zamiast UI instalatora (`ui_handler`) uruchamia nasz program. Sam instalator `il`, jedyny moduł, który programuje flash, jest zastąpiony zaślepką, która tylko śpi (`src/il_stub.S`), więc flash nie może zostać ruszony, nawet jeśli urządzenie wejdzie w pętlę ładowania. Podmiana jest we wszystkich czterech `loader.ini` na płycie: oba warianty sprzętu (`Mk2IC/0109` i `Mk2IC/0209`), w każdym `RENAULT/` i `RENAULT/app_sw/`. Ta sama płyta działa więc na 0109 i 0209. Bootrom startuje z płyty jak przy aktualizacji. Program:

- rysuje HELLO WORLD i licznik w framebufferze ustawionym przez `videodr`;
- czyta przyciski z `/nav_bus` (ramki `3A 3B 48 kod` i `3A 3B 49 kroki`): VOL+ +1, VOL− −1, pokrętło ±kroki, wciśnięcie pokrętła zeruje, Last message liczy 700 cyfr π (spigot Rabinowitza–Wagona, cyfry pojawiają się na bieżąco; w tym czasie przyciski czekają w kolejce);
- loguje wszystko na UART A (konsola OS-9).

## Budowanie

Wymaga `m68k-elf-gcc` i `cd/carin_nav_sw.img`.

```bash
make -C poc          # poc/build/ui_handler.mod, poc/build/il.mod (zaślepka), cd/carin_poc.img + .cue
make -C poc chd      # dodatkowo cd/carin_poc.chd
```

Jak to działa:

1. `src/start.S`: wejście procesu i opakowania wywołań systemowych (`trap #0`).
2. `src/main.c`: program. Kod jest tylko PC-relative i nie ma zmiennych globalnych (moduł OS-9 jest tylko do odczytu), cały stan trzyma na stosie. Makefile odrzuca `.data`/`.bss` i relokacje absolutne.
3. `src/il_stub.S`: zaślepka `il`, w kółko `F$Sleep`.
4. `tools/mkmod.py`: binarka → moduł OS-9 (nagłówek, parzystość, CRC-24, pusty blok idata/irefs, bo F$Fork zawsze go przetwarza). Moduł ma dokładnie długość oryginału (`ui_handler` 31628 B, `il` 41004 B, w obu wariantach tyle samo), więc `loader.ini` zachowuje rozmiar i offsety.
5. `tools/mkdisc.py`: szuka modułów po nazwie (nagłówek OS-9) we wszystkich `loader.ini` na płycie, podmienia je i przelicza EDC/ECC zmienionych sektorów (`tools/cdsector.py`, sprawdzone na oryginalnych sektorach). Gdy nie znajdzie dokładnie czterech kopii albo rozmiar się nie zgadza, przerywa.

## Test w emulatorze

```bash
tools/carin.py gui --state poc --load state/installed --cd carin_poc          # 0209
tools/carin.py gui --state poc0109 --load state/hw0109 --cd carin_poc         # 0109
tools/carin.py gui --state poc_pusty --seed --hw 0109 --cd carin_poc          # pusty flash: od razu z płyty
```

`--load` kopiuje stan, więc oryginał zostaje nietknięty. Płyta PoC to kopia NAV_SW, więc działa jak aktualizacja: włóż ją do działającej nawigacji, zamknij „Attention!”, soft ustawi flagę sw-load i się zrestartuje, a bootrom załaduje PoC.

## Na prawdziwym sprzęcie

Nagrywanie (obraz MODE2/2352, raw):

```bash
cd cd && cdrdao write --device /dev/sr0 carin_poc.cue
```

Ryzyka:

1. Nie wiemy, co w aucie każe bootromowi startować z płyty przy ważnym flashu. W emulatorze to linia z ekspandera I/O (zgadnięta). Najpierw sprawdź kopią oryginalnej NAV_SW, czy urządzenie w ogóle startuje z CD-R.
2. `sysgo` z płyty startuje `il_slave` na slavie (tak jak przy normalnej aktualizacji). Bez poleceń od `il` (tu zaślepki) nie powinien niczego programować, ale tego jeszcze nie sprawdziliśmy w RE.
3. Paleta jest nieznana, więc kolory mogą wyjść inne niż w emulatorze. Użyte są indeksy, którymi rysuje oryginalny soft (0x49 tło, 0xFF tekst, 0x92 akcent, 0x00).
4. Czy moduł pośredni wysyła przyciski bez uzgadniania z MMI (które tu nie działa), wyjdzie dopiero w aucie.

Jeśli program się nie uruchomi: wyjmij płytę i zrób reset (wyłącz zapłon), a urządzenie wystartuje z flasha.
