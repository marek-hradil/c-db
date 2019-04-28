# Dokumentace
- SQL-like databáze implementovaná v C
- podporuje:
  - CREATE TABLE
  - SELECT
  - INSERT
  - UPDATE
  - DELETE
- výstup v JSON formátu, jelikož se s ním dále nejlépe pracuje
- nestabilní a z části nefunkční

## Konfigurace
1. Vytvořit složku v kořenovém adresáři
  - dbs - pro databáze
  - test - pro testovací scénáře
2. Založit databázi
  - toto jde zatím jen manuálně, tudíž je potřeba založit složku v `/dbs`
3. `OPTIONAL` přiřadit víc paměti, pokud nepracuje správně

## Struktura
- projekt se skládá ze čtyř hlavních částí
  - SQL parser
  - Commands
  - DBAccessor
  - DBCreator

### SQL-Parser
- hlavní účel je naparsovat query ve formátu stringu do `CompiledQuery` struktury, která se dále předává do commands
- dost často se parsuje na základě mezer, tudíž syntaxe query je hodně strict, ale zárověn pořád podobná SQL
- reprezentováno soubory končícími na QueryParser
- bližší popis:
  - queryParser.c
    - vezme query string a pomocí mezer ho rozdělí do QueryParts array, která se dále předává do specifičtějších parserů
    - části převede na malá písmena
    - vezme první část `KEYWORD` a podle té se rozdělí do parseru
    - naalokuje potřebné části(pozn. aplikace se snaží alokování dělat na jednom místě a celkově alokovat co nejmíň, jelikož s tím byly "problémy")
    - pokud keyword nepozná, nic neprovede
  - createQueryParser.c
    - naparsuje `CREATE TABLE` query, které má zaúkol vytvořit novou tabulku
    - syntax: `CREATE TABLE ( <nazev> <typ>, <nazev> <typ>, <nazev> <typ> )`
      - nazev sloupce: libovolny, bez mezer
      - typ: VARCHAR nebo INT
  - selectQueryParser.c
    - naparsuje `SELECT` query, které vybere data z tabulky
    - syntax: `SELECT <nazev_sloupce>,<nazev_sloupce>,<nazev_sloupce>,... FROM <nazev_tabulky>`
  - insertQueryParser.c
    - naparsuje `INSERT` query, které vloží nový řádek do tabulky
    - syntax:
      - `INSERT INTO <nazev_tabulky> VALUES (<hodnota>,<hodnota>,<hodnota>)`
      - `INSERT INTO <nazev_tabulky> (<nazev_sloupce>,<nazev_sloupce>) VALUES (<hodnota>,<hodnota>)`
      - hodnoty a nazvy mohou byt ohraniceny uvozovkami
  - updateQueryParser.c
    - naparsuje `UPDATE` query, které mění hodnoty řádku
    - syntax: `UPDATE <nazev_tabulky> SET <nazev_sloupce>=<nova_hodnota>`
  - deleteQueryParser.c
    - naparsuje `DELETE` query, které smaže řádek
    - syntax: `DELETE FROM <nazev_tabulky>`

### Commands
- každý command má svůj soubor skládající se z:
  - funkce pro vykonání commandu na jednom řádku
  - funkce `executeCommand` která mu dává správné fungování a volá ho v kontextu celé tabulky

### DBAccessor
- obstarává přístupy k jednotlivým souborům
- spravuje cesty k souborům
- připojuje se k jednotlivým databázím

### DBCreator
- vytváří soubory a tabulky
- pracuje nad Accessor vrstvou

## Design decisions
- JSON
  - Select vrací JSON formát, jelikož je to dnes nejpoužívatelnejší, jeden z nejjednodušších a nejlépe dále zpracovatelných formátů
- Testy
  - testy byly psány pro potřebu vývoje
  - většinou fungují(oproti aplikaci samotné)
- binární zápis
  - šetří místo
  - kdo kdy viděl databázi, která to ukládá do texťáku
- logger
  - lépe zpracovává hlášky pro uživatele, které ale nejsou output
  - v budoucnu by měl zapisovat do souboru
