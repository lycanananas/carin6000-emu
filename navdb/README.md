# navdb

Dekoder bazy map CARiN / Carminat (`carindb` z płyty `NAV_DB`) do GeoPackage. Wynik ma być pierwszym krokiem do konwertera OSM → Carminat. Format jest rozpracowywany statycznie: z pliku i z disassembly modułów OS-9, które go czytają (`pbp`, `dbc`, `dbq`, `dbpa`…), bez emulatora.

Stan:
- znany jest nagłówek (blok 0), tablica 93 typów rekordów z rozmiarami i katalog 31 tablic z położeniem w pliku;
- tablica 1 to parcele mapy: 49 031 prostokątów, w GeoPackage jako warstwa `parcels`;
- parcele są skompresowane (bitowo plus kod tekstu nazw), a rozpakowywanie jest przeniesione 1:1 z modułu `pbp` (`tools/parcel.py`);
- drogi: odcinki z każdej parceli, w GeoPackage jako warstwa `roads` (geometria tak, atrybuty i nazwy jeszcze nie). Podgląd Warszawy: `temp/navdb/warszawa.png`;
- tablice `d`/`e` to nazwy miejscowości i POI z indeksem alfabetycznym.

Szczegóły w [docs/format.md](docs/format.md).

```bash
python3 navdb/tools/carindb2gpkg.py        # cała baza -> temp/navdb/carindb.gpkg (kilka minut)
python3 navdb/tools/carindb2gpkg.py temp/navdb/carindb temp/navdb/warszawa.gpkg --bbox 20.85,52.10,21.27,52.37
```

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
