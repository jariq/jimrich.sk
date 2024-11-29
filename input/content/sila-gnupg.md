---
title: "Sila GnuPG"
date: 2005-12-28T19:17:41+00:00
draft: false
ji_lang_name: Slovak
ji_lang_code: sk
ji_rss_desc: GnuPG je veľmi silný nástroj na šifrovanie dát a vytváranie digitálnych podpisov. Dlhšiu dobu sa mi úspešne darilo práci s ním vyhýbať, no odkedy som pochopil princíp jeho činnosti, stal sa neoddeliteľnou súčasťou môjho života.
---

[GnuPG][1] (GNU Privacy Guard) je veľmi silný nástroj na šifrovanie dát a vytváranie digitálnych podpisov. 
Dlhšiu dobu sa mi úspešne darilo práci s ním vyhýbať, no odkedy som pochopil princíp jeho činnosti, stal sa neoddeliteľnou súčasťou môjho života.

## 1. Využitie GnuPG

Je všeobecne známe, že akékoľvek dáta, ktoré posielame po sieti, či už lokálnej alebo po internete, sa môžu dostať do rúk nepovolanej osoby. 
Ako jednoduchý príklad nám poslúži e-mailová správa. 
Predstavte si, že posielate kolegovi dôležité materiály o novom produkte, ktorý má vašej firme zabezpečiť dominantnú pozíciu na trhu. 
Správu odošlete pomocou zabezpečeného protokolu SMTPs a mylne sa domnievate, že e-mail sa nemôže dostať do nepovolaných rúk. 
Na poštovom serveri je však správa po prijatí uložená ako čistý text. 
Čo ak správca tohto servera podľahol lákavej ponuke konkurencie a prepošle im vašu správu? 
Ak chcete takejto situácii zabrániť, máte dve možnosti. 
Buď materiály kolegovi odovzdáte osobne, alebo ušetríte čas a odošlete mu ich zašifrované. 
A práve to vám umožní GnuPG.

## 2. Ako to funguje

GnuPG šifruje dáta pomocou kľúčových párov. 
Dáta zašifrované verejným kľúčom môžu byť dešifrované len prislúchajúcim privátnym (tajným) kľúčom. 
Privátny kľúč je navyše chránený heslom, na rozdiel od verejného. 
Ak chcete poslať kolegovi šifrovanú správu, zašifrujete ju jeho verejným kľúčom a len on bude schopný ju dešifrovať, pretože vlastní správny privátny kľúč a heslo k nemu. 
Každý používateľ GnuPG si teda musí vygenerovať kľúčový pár. 
Verejný kľúč môže zverejniť napríklad na svojej webovej stránke, no privátny kľúč si musí chrániť ako "oko v hlave".

## 3. Práca s GnuPG

Ukážme si, ako môže Alice zašifrovať pomocou GnuPG súbor pre Boba. 
Po inštalácii balíka GnuPG si Bob musí vygenerovať svoj vlastný kľúčový pár.

```
bob@host1$ gpg --gen-key
```

Následne exportuje svoj verejný kľúč do ASCII súboru a ten odovzdá Alice:

```
bob@host1$ gpg -a -o ~/bob.asc --export bob@host1.sk
```

Alice si ho pridá do svojej databázy kľúčov (angl. keyring):

```
alice@host2$ gpg --import bob.asc
```

Súbor pre Boba s názvom `udaje.tar` zašifruje Alice pomocou príkazu:

```
alice@host2$ gpg -e -o udaje.tar.gpg -r bob@host1.sk udaje.tar
```

Takto zašifrovaný súbor môže Alice bez obáv odoslať e-mailom. 
Bob ho dešifruje príkazom:

```
bob@host1$ gpg -d -o udaje.tar udaje.tar.gpg
```

## 4. Šifrovať sa dá aj bez kľúčov

GnuPG poskytuje možnosť šifrovať dáta aj bez kľúčov, len s použitím hesla. 
Dešifrovať ich samozrejme dokáže iba osoba, ktorá pozná heslo. 
Táto jednoduchšia možnosť je vhodná najmä na komunikáciu s niekým, koho poznáte a môžete sa s ním jednoducho a bezpečne dohodnúť na používanom hesle (napríklad s priateľkou). 
Okrem jednoduchosti je výhodou aj to, že môžete často meniť heslo a znížiť tak riziko, že niekto, kto náhodou získal vaše heslo, dešifruje všetky vaše staršie správy. 
Osobne si myslím, že tento spôsob šifrovania správ je náchylný na útok hrubou silou, a preto odporúčam jeho použitie dobre zvážiť.

Ak by ste chceli v predchádzajúcom príklade použiť šifrovanie pomocou hesla, zmenil by sa príkaz, ktorým Alice zašifruje dáta pre Boba na takýto:

```
alice@host2$ gpg -c -o udaje.tar.gpg udaje.tar
```

## 5. Digitálne podpisovanie súborov

Predstavte si situáciu, že Alice posiela Bobovi e-mailom daňové priznanie, ktoré je zašifrované jeho verejným kľúčom. 
Ak by mu chcel niekto narobiť problémy, zabráni doručeniu e-mailu od Alice a podstrčí namiesto neho sfalšovanú verziu. 
Samozrejme útočník falošnú verziu tiež zašifruje Bobovým verejným kľúčom, pretože ten je k dispozícii na jeho firemnom webe. 
Bob sa tak bude mylne domnievať, že súbor bol prenášaný sieťou bezpečne, pretože ho bez problémov dešifroval. 
Aby sa zabránilo takejto forme útoku, existuje možnosť digitálneho podpisovania súborov.

Vynikajúca vlastnosť šifrovacích algoritmov použitých v GnuPG je, že súbory zašifrované privátnym kľúčom, môžu byť dešifrované aj verejným kľúčom. 
Pri podpisovaní súboru sa spraví jeho hash, a ten je potom špeciálnym spôsobom zašifrovaný pomocou privátneho kľúča odosielateľa. 
Každý, kto má verejný kľúč odosielateľa, si môže overiť, že súbor pochádza naozaj od neho, a že nebol zmenený.

V našom prípade by to teda znamenalo, že pred odoslaním daňového priznania ho Alice nielen zašifruje Bobovým verejným kľúčom, ale aj digitálne podpíše svojím privátnym. 
Ak by chcel útočník vykonať zámenu z prvého prípadu, nebol by schopný sfalšovať digitálny podpis súboru, pretože nevlastní privátny kľúč Alice.

Alice zašifruje súbor už známym postupom:

```
alice@host2$ gpg -e -o udaje.tar.gpg -r bob@host1.sk udaje.tar
```

Vytvorí osobitný súbor s digitálnym podpisom šifrovaného súboru:

```
alice@host2$ gpg -sba udaje.tar.gpg
```

Bob po obdržaní oboch súborov môže overiť pravosť šifrovaného súboru príkazom:

```
bob@host1$ gpg --verify udaje.tar.gpg.asc udaje.tar.gpg
```

## 6. Dôveryhodnosť kľúčov

Šifrovanie s GnuPG je bezpečné len v prípade, ak sme si 100% istí, že verejný kľúč, ktorým šifrujeme, naozaj pochádza od danej osoby. 
Preto sa dá nastavovať miera dôveryhodnosti kľúča a kľúče je možné podpisovať, čím sa ich dôveryhodnosť zvyšuje. 
Pre bežné použitie však stačí, ak nám verejný kľúč dodá jeho vlastník osobne, alebo napríklad telefonátom s ním overíme odtlačok (angl. fingerprint) kľúča, ktorý je možné zobraziť príkazom:

```
$ gpg --fingerprint bob@host1.sk
```

Pokiaľ ste si teda istí, že kľúč vo vašej databáze naozaj patrí danej osobe, môžete mu nastaviť mieru dôveryhodnosti na maximum. 
Spravíte to príkazom:

```
$ gpg --edit-key bob@host1.sk
```

Následne zadáte v podmenu príkaz "trust" a vyberiete zodpovedajúce parametre. 
Ak nemáte zadefinovanú mieru dôveryhodnosti kľúča, bude GnuPG pri jeho používaní zobrazovať upozornenie.

## 7. Záver

GnuPG je veľmi populárne a teší sa širokej podpore v iných softvérových produktoch. 
Vo forme pluginov je integrovateľné do e-mailových a IM klientov. 
Ako príklad veľmi dobre poslúži [Mozilla Thunderbird][2] a jej plugin [enigmail][3].

Ak by vás nebavilo spravovať kľúče z prostredia shellu, tak môžete použiť napríklad program KGpg, ktorý je súčasťou window manažéra a správcu plochy [KDE][4]. 
Na [domovskej stránke projektu GnuPG][5] môžete samozrejme nájsť aj grafických správcov kľúčov pre MS Windows a ďalšie systémy.

[Článok bol publikovaný aj na portáli linuxos.sk][6]


[1]: https://www.gnupg.org/
[2]: https://www.mozilla.com/thunderbird/
[3]: https://www.enigmail.net
[4]: https://www.kde.org
[5]: https://www.gnupg.org/
[6]: https://linuxos.sk/clanok/sila-gnupg/