# navdb

Dekoder bazy map CARiN / Carminat (`carindb` z płyty `NAV_DB`) do GeoPackage. Wynik ma być pierwszym krokiem do konwertera OSM → Carminat. Format jest rozpracowywany statycznie: z pliku i z disassembly modułów OS-9, które go czytają (`pbp`, `dbc`, `dbq`, `dbpa`…), bez emulatora.

Stan:
- znany jest nagłówek (blok 0), tablica 93 typów rekordów z rozmiarami i katalog 31 tablic z położeniem w pliku;
- tablica 1 to parcele mapy: 49 031 prostokątów, w GeoPackage jako warstwa `parcels`;
- parcele są skompresowane (bitowo plus kod tekstu nazw), a rozpakowywanie jest przeniesione 1:1 z modułu `pbp` (`tools/parcel.py`);
- drogi: odcinki z każdej parceli, w GeoPackage jako warstwa `roads`: geometria, nazwa ulicy, dzielnica, miasto, kraj, klasa drogi (FRC 0-6), kierunkowość, długość w metrach. Podgląd Warszawy: `temp/navdb/warszawa.png`, według klas: `temp/navdb/warszawa_klasy.png`;
- poziomy routingu 1–3 (tablice 1e, 3, 2): ta sama budowa co parcele, coraz ważniejsze drogi (`roads.level`);
- tło mapy (tablice 6, 16, 1c): obszary (woda, lasy, zabudowa, cmentarze, wyspy) z nazwą i powierzchnią oraz uproszczone drogi z numerami, w 3 poziomach szczegółowości (warstwy `areas`, `overview_roads`);
- POI (tablica 7, nazwy z tablicy 11): 698 tys. punktów z nazwą, marką i kategorią (warstwa `poi`);
- GeoPackage ma indeks przestrzenny (RTree), więc QGIS otwiera też całą bazę;
- podgląd całej mapy Warszawy: `temp/navdb/warszawa_mapa.png`;
- tablice `d`/`e` to nazwy miejscowości i POI z indeksem alfabetycznym.

Szczegóły w [docs/format.md](docs/format.md): specyfikacja formatu z podziałem na pola potwierdzone w kodzie, sprawdzone na danych i tylko widziane, z adresami funkcji OS-9, z notatkami pod enkoder (sekcja 8) i z listą otwartych spraw.

Liczby dla całej bazy: 8,9 mln odcinków dróg (4 poziomy routingu), 302 tys. obszarów, 215 tys. dróg przeglądowych, 698 tys. POI. Zrozumiane jest ok. 50% objętości pliku. Brakuje jeszcze indeksów wyszukiwania (tablica f to 21%), części indeksowych tablic i kilku pól.

## Następne kroki (pod enkoder)

1. Indeksy wyszukiwania: f (ulice), d/e (miejscowości), 10, 12, 18; części indeksowe (`a`) tablic.
2. Połączenia między parcelami (sloty 9 i 10, punkty brzegowe), pozostałe bajty odcinka, sloty 3, 7, 11 i 12.
3. Drobne tablice 15, 14, 1d, 0, 4, a, b, c, 13, 17.
4. Enkoder: OSM (na początek Mazowsze) → te same struktury; sprawdzanie dekoderem i w emulatorze.

```bash
python3 navdb/tools/carindb2gpkg.py        # cała baza -> temp/navdb/carindb.gpkg (kilka minut)
python3 navdb/tools/carindb2gpkg.py temp/navdb/carindb temp/navdb/warszawa.gpkg --bbox 20.85,52.10,21.27,52.37
```

## Oglądanie w QGIS

Otwórz `temp/navdb/carindb.gpkg` (cała baza) albo wycinek z `--bbox`. Styl wczytuje się sam z tabeli `layer_styles` w pliku (`tools/qgis_style.py`):
- drogi według klasy (FRC 0 czerwone … 6 szare); drobne klasy dopiero po przybliżeniu (klasa 4 od 1:150 000, 6 od 1:50 000), żeby widok kraju nie rysował 8,6 mln odcinków;
- nazwy ulic od 1:25 000;
- drogi nie do nawigacji przerywaną szarą linią od 1:30 000.

Dla porównania można dodać podkład OSM (Przeglądarka → XYZ Tiles → OpenStreetMap).

## Dane

Baza nie jest w repo. Wypakowanie z obrazu płyty do `temp/navdb/` (ignorowane przez gita):

```bash
python3 navdb/tools/extract_navdb.py        # cd/carin_maps_pl_2012_2013.img -> temp/navdb/
```

## Narzędzia

| Skrypt | Co robi |
|---|---|
| `tools/extract_navdb.py [obraz] [katalog]` | wypakowuje pliki płyty NAV_DB (ISO9660 w sektorach MODE2/2352) |
| `tools/carindb_header.py [carindb]` | wypisuje nagłówek, tablicę typów rekordów i katalog tablic |
| `tools/carindb2gpkg.py [carindb] [out.gpkg]` | dekoder do GeoPackage (na razie warstwa `parcels`) |
| `tools/parcel.py` | rozpakowanie parceli (port z `pbp`: czytnik bitów, sloty, kod tekstu) |
| `tools/qgis_style.py` | domyślne style QGIS zapisywane w GeoPackage |
| `tools/gpkg.py` | minimalny zapis GeoPackage przez sqlite3, bez bibliotek GDAL dla Pythona |
| `tools/annotate_dbq.py` | podpisuje eksport `dbq` z Ghidry (`RECSIZE_xx`) i robi indeks: które funkcje używają których typów rekordów |

Disassembly modułów leży w `temp/navdb/dis/` (unidasm z `re-safrane/extracted/flash_0209/*.mod`).

## Ghidra (dekompilacja modułów bazy)

```bash
/opt/ghidra/support/analyzeHeadless temp/navdb/ghidra navdb_db -import temp/navdb/mods/*.mod \
    -processor 68000:BE:32:MC68030 -postScript ExportOS9Modules.java $PWD/temp/navdb/ghidra_out \
    -scriptPath $PWD/tools/ghidra -overwrite
python3 tools/ghidra/resolve_a6.py temp/navdb/mods/064_dbq.mod temp/navdb/ghidra_out/064_dbq.mod_analysis.md \
    temp/navdb/ghidra_out/064_dbq.resolved.md
```

`ExportOS9Modules.java` (skrypt Ghidry w Javie):
- tworzy funkcje od prologów `link a5`;
- ustawia konwencję Microware C (argumenty w D0/D1, potem stos);
- eksportuje pseudo-C i assembler do Markdown.

`resolve_a6.py` zamienia wywołania przez tablicę skoków w danych procesu (`unaff_A6 + -N`) na nazwy funkcji, a zmienne globalne na `g_XXXX`.

Projekt jest osobny (`temp/navdb/ghidra/`), żeby nie ruszać `temp/ghidra/`.
