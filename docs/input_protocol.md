# Wejście: moduł pośredni na `nav_bus` (stan RE)

Notatki z inżynierii wstecznej, stan na 2026-10-21. Przyciski z deski/kierownicy i dane z radia zbiera moduł pośredni (to nie jest slave C166). Do nawigacji wysyła je po I2C, bez linii MRQ.

## Warstwa fizyczna (potwierdzone)

- Urządzenie OS-9 `/nav_bus` (sterownik `vbnavi2c`, menedżer `vbf`) na kontrolerze I2C w MCASIC (`vbi2csp2`, 0xC00020).
- Moduł ma adres I2C 0x50 (zapis), 0x51 (odczyt).
- Linia „uwagi” modułu to IRQ poziomu 4 (autowektor 28), bramkowana bitem 2 w 0xC0001C. Po jej zgłoszeniu `vbnavi2c` czyta 1 bajt statusu z 0x51.
- Status: połówka bajtu na slot (slot 0 w górnej). Bit 0 = „ramkę odebrałem”, bit 1 = „mam ramkę dla nawigacji”.
- Pobranie ramki ze slotu 1: zapis `01 01` do 0x50, potem odczyt 0x50 bajtów z 0x51 (potwierdzone eksperymentem: status 0x13 wywołał dokładnie to).
- Sloty z deskryptora `nav_bus`: slot 0 (flagi 0, adres 0x00), slot 1 (flagi 0x02, adres 0x50/0x01, prefiks 2 bajty), oba po 0x50 bajtów.

## Ramki nawigacja → moduł (widziane na magistrali)

| Ramka | Znaczenie (hipoteza) |
|---|---|
| `01 08 00` | inicjalizacja `vbnavi2c` |
| `00 1C 07 8F 04 03 01 00 89` | slot 0, ID 0x1C, długość 7 (stan wejść od `recv_ibus`) |
| `00 3B 06 3A 00 01 01 01` | slot 0, adres modułu 0x3B, długość 6, funkcja 0x3A (NAOK) |

## Słownik komend (`send_ibus`)

Procesy MMI wysyłają tekstowe komendy (np. `GIKS\r`, `SAUD,0,0,0`, `NAOK,1,2,1`, `SDIS,1,%d`). `send_ibus` tłumaczy je na binarne ramki według tablicy: adres docelowy + dwa bajty funkcji.

| Komenda | Adres | Funkcja |
|---|---|---|
| CSIC, CCNF | 3B | F0 00 |
| SDST | 3B | 3A 23 |
| NAOK | 3B | 3A 00 |
| SAUD | 3B | 68 4E (4 bajty danych) |
| SDIS, CDSP | 3B | F0 3F |
| CCTL, SMOS | 3B | F0 4F |
| STIM / SDAT | 3B | 80 40 (4) |
| GTIM / GDAT | 3B | 80 41 (4) |
| **GIKS** | 3B | **80 10**: zapytanie o stan przycisków (hipoteza: „Get Input Keys State”) |
| SGTS | 3B | 67 02 |
| SNAS | 7F | 67 02 |
| STVS | 3B | BB 4F |
| DACK, DACB, DACR, DACE, DACN | 3B | 3F A0, A1, A2, B0, FF |
| DREQ, DIRE, DRTM | 3B | 3F A0 |
| NACK…NRCP | 7F | 3F A0… (odpowiedniki z adresem 0x7F) |

## Przepływ w OS-9

- `recv_ibus` odbiera ramki z `/nav_bus` (`$7b0a`, do 0x25 bajtów), patrz „Którędy przychodzą do nawigacji”.
- Zmiany stanu z modułu trafiają do tablicy zmiennych stanu (`$a2e2`) i generują 2-bajtowe zdarzenia `[kod][arg]` (0xB3–0xBD) do `/pipe/i2c_trigger`.
- `device_dialog` czyta `i2c_trigger`, `bsi_nav_in` i `bsi_ctrl_c`, a odpowiada komendami tekstowymi przez `uni_requests` → `send_ibus` → I2C.
- Pilot IR (RC5) to osobna droga: slave C166 → `/rc5` (h_ibus_dr) → `recv_ibus`.

## Adresacja komunikatów (potwierdzone)

Komunikat wewnętrzny ma postać `[źródło][cel][funkcja][dane]`. Na magistrali dochodzi jeszcze bajt długości: `[slot][źródło][długość od siebie do końca][cel][funkcja][dane]`.

- 0x3B to nawigacja. Cele 0xBF, 0xE7 i 0xFF `recv_ibus` traktuje jak 0x3B.
- 0x80 to serwer stanu auta (kluczyk). `GIKS` = `3B→80 fn 0x10` („Get Ignition Key State”), odpowiedź `80→3B fn 0x11`, wartość 0/1/3/7 = położenie kluczyka. To nie są przyciski.
- 0x3A to panel. `3A→3B fn 0x48` to przycisk, a `3A→3B fn 0x49` to pokrętło.
- 0xF0 to źródło zastępcze dla ramek przepisywanych przez `recv_ibus`.

Tablica routingu w `recv_ibus` (dane, offset 0x1096; `[źródło][cel][funkcja] → handler`): 67 3B 01, 67 7F 01, 80 3B 11, F0 3B 11, 80 3B 23, 80 3B 24, 3A 3B 48/49/00/19/21/23/5B, F0 3B 43, 3F 3B 04/14/05/30/00/01/0E/0F/9F/9E, 3F 7F 04/14/05/30/00/01/0E/0F/0B/9F/9E/40.

## Przyciski: `3A 3B 48 <bajt>` (handler `recv_ibus+0x64C`)

Bity 0–5 to kod, bit 6 to flaga (dla kodów 2 i 3 zdarzenie idzie tylko przy wyzerowanym bicie 6, czyli przy wciśnięciu), bit 7 to flaga.

| Kod | Zdarzenie | Klawisz (test „check KEY” w diagnozie) |
|---|---|---|
| 0 | 0x9B → `i2c_trigger` | Mute NAV |
| 1 | 0x94 → `i2c_trigger` | Last message |
| 2 | 0xB3 (arg 1) → `i2c_trigger` | VOL+ |
| 3 | 0xB4 → `i2c_trigger` | VOL− |
| 4 | 0x83 → `i2c_trigger` | wciśnięcie pokrętła (otwiera menu) |
| 8 | 0x93 → callback MMI | Stand by (gasi ekran) |
| 9 | 0xC0 → callback MMI | Address book |
| 7, 12 | 0xC1 → callback MMI | autotest diagnostyczny (czerwony ekran „auto test not ok”); brak przycisku na panelu, pewnie wejście serwisowe |
| 5, 6, 10, 11 | brak obsługi | — |

Panel: 6 przycisków i pokrętło (A/B i wciśnięcie), 9 przewodów. Moduł pośredni ma 11 wejść.

## Pokrętło: `3A 3B 49 <bajt>` (handler `recv_ibus+0x736`)

Bity 0–5 to liczba kroków, bit 6 to kierunek (ujemny). Wynik to zdarzenie 0x80 z wartością ze znakiem do `i2c_trigger`.

## Pilot RC5: `/pack/rc5keys`

`recv_ibus` czyta po 1 bajcie i zamienia kod na gotowy komunikat. Są trzy tablice (wybierane kodem 0x57 albo 0x62, czyli dwa typy pilota):

| RC5 | Komunikat | RC5 | Komunikat |
|---|---|---|---|
| 57 / 62 | `F0 3B 48 05` | 52 | `F0 3B 48 0D` |
| 55 / 60 | `F0 3B 49 21` | D5 / E0 | `F0 3B 49 23` |
| 56 / 61 | `F0 3B 49 A1` | D6 / E1 | `F0 3B 49 A3` |
| 50 / 5E | `F0 3B 49 E1` | D0 / DE | `F0 3B 49 E3` |
| 51 / 5F | `F0 3B 49 61` | D1 / DF | `F0 3B 49 63` |
| 1E | `F0 3B 48 0E` | 1C | `F0 3B 48 0F` |
| 10 / 66 | `F0 3B 48 1A` | 11 / 67 | `F0 3B 48 1B` |
| 3E / 77 | `F0 3B 48 1C` | 2E | `F0 3B 48 1D` |
| 47 | `F0 3B 48 1E` | 7E | `67 3B 00 01` |

Kody ruchu (fn 0x49) to cztery kierunki (bity 7/6) razy 1 albo 3 kroki (bit 7 kodu RC5 = szybciej).

## Którędy przychodzą do nawigacji (potwierdzone)

Przez `/nav_bus`. `recv_ibus` otwiera go jako ścieżkę 8 i rejestruje sygnał 0x2829 (SS_SSig). `vbnavi2c` zwraca dla SS_SSig status 0x6000 (asynchronicznie), a `vbf` wysyła sygnał dopiero po odebraniu ramki. Czytnik `recv_ibus+$7b0a` czyta do 0x25 bajtów i ma trzy tryby, wybierane konfiguracją:

- `(-$6d46)` = 1 (tak jest w 0209): tryb raw, komunikat to bajty ramki bez zmian, więc źródło 0x3A zostaje;
- `(-$6d4a)` = 1: `F0 3B` + bajty od 3. (źródło zastąpione);
- domyślnie: `F0 3B` + bajty od 5.; fn 0xA8 (RDS) idzie osobno, fn 0/1/3 to uzgadnianie.

Sekwencja na I2C (emulowana w `carin.cpp`, `car_frame`):

1. Moduł podnosi linię uwagi (IRQ4).
2. `vbnavi2c` czyta status z 0x51 (1 bajt): 0x13 = slot 1 ma ramkę.
3. Zapis `01 01` do 0x50, potem odczyt 0x50 bajtów z 0x51: ramka `[src][len][dst][fn][dane]`, len liczy siebie i bajty po sobie (dla przycisku `3A 04 3B 48 kod`).
4. `recv_ibus` dostaje `3A 3B 48 kod` i przekazuje do handlera `+0x64C`.

Pozostałe źródła w `recv_ibus` (sygnały 0x2821–0x2829 po kolei): 0x2821 `/pack/rc5keys`, 0x2822 `/rc5` (h_ibus_dr, kanał skrzynki 8; kody RC5 zamieniane na `F0 3B 48/49`, routing ich nie ma, więc pilot nie steruje MMI w tej konfiguracji), 0x2824 moduł danych `rds_test_if_conf`, 0x2825 `rds_time`, 0x2826 `/c0/_129` (ARCNET, czytnik `$7a88`: `[src][len]` i len bajtów), 0x2829 `/nav_bus`.

### Kanał 8 (h_ibus_dr) w emulatorze

Powiadomienie bez żądania: typ 6, payload `01` (są dane). Odczyt: żądanie typu 2, odpowiedź typu 0 z danymi. Każdy odczyt zużywa jedno powiadomienie. Przerwanie od odpowiedzi musi przyjść z opóźnieniem (sterownik ustawia flagę oczekiwania `$74` dopiero po wysłaniu żądania). Funkcja `car_rc5` w `carin.cpp`.

## Do ustalenia

1. Dlaczego autotest (kody 7/12) kończy się „not ok”: którego testu emulator nie spełnia.
2. Czy kody 5, 6, 10, 11 coś znaczą w teście „check KEY”.
