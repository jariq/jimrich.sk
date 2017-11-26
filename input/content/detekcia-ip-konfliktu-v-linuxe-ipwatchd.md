---
title: "Detekcia IP konfliktu v Linuxe - IPwatchD"
date: 2006-12-01T07:00:31+00:00
draft: false
ji_lang_name: Slovak
ji_lang_code: sk
ji_rss_desc: Článok popisuje príčiny vzniku IP konfliktov a predstavuje možnosti ich detekcie na operačných systémoch s jadrom Linux.
---

IP konflikt nastáva v prípade, že dve rôzne zariadenia v lokálnej sieti používajú rovnakú IP adresu. 
Operačný systém MS Windows na túto nežiadúcu udalosť upozorňuje varovným hlásením a v logoch je možné nájsť aj [MAC adresu][1] konfliktného systému. 
Ak však používate operačný systém GNU/Linux a zúčastnili ste sa IP konfliktu, či už ako strana, ktorá ho spôsobila, alebo ako strana ním postihnutá, pravdepodobne ste si všimli iba opakujúce sa výpadky konektivity. 
Ich skutočnú príčinu by ste však v systémových logoch hľadali márne, pretože linuxové jadro na túto udalosť vôbec nereaguje.

# Detekcia IP konfliktu

IP konflikt je možné odhaliť len v prípadoch, keď váš systém zo siete príjme paket s rovnakou zdrojovou IP adresou ako je ním používaná adresa. 
Paket však musí pochádzať z iného systému a túto skutočnosť je možné určiť podľa zdrojovej MAC adresy, ktorá je jedinečnou hardvérovou adresou sieťového rozhrania - v prípade počítača sieťovej karty. 
Ak sa teda k vášmu počítaču dostane zo siete takýto paket, je jasné, že niekde na lokálnej sieti je počítač, ktorý používa rovnakú IP adresu ako váš systém.

V prípade, že sú na lokálnej sieti použité ako rozbočovače switch-e (cca 90% sietí), k vášmu počítaču sa dostanú len pakety jemu priamo adresované. 
Konfliktný systém by teda musel vygenerovať paket s rovnakou cieľovou adresou ako je jeho vlastná (zdrojová) IP adresa. 
Takýto paket by však žiadny systém nikdy neodoslal do siete, ale spracoval by ho sám, lebo je v skutočnosti adresovaný jemu samému. 
Preto je jedinou možnosťou detekovať IP konflikt z tzv. [broadcast paketov][2] (ďalej len obežníky), ktoré sú doručené všetkým zariadeniam na lokálnej sieti. 
Obežníky vysiela napríklad aj Samba, no tá málokedy beží na všetkých počítačoch v sieti, a preto väčšina operačných systémov detekuje IP konflikt z ARP obežníkov.

# Výpadky konektivity

S [protokolom ARP][3], ktorý slúži o.i. na zisťovanie MAC adries zariadení v lokálnej sieti, priamo súvisia aj výpadky konektivity sprevádzajúce IP konflikt. 
Ak chce počítač A s IP adresou 10.1.1.2 komunikovať s počítačom B s IP adresou 10.1.1.3, vyšle najskôr ARP obežník adresovaný všetkým systémom na lokálnej sieti, v ktorom sa pýta:

> "Kto má IP adresu 10.1.1.3? Odpoveď žiada počítač s IP adresou 10.1.1.2 a MAC adresou AA:AA:BB:BB:CC:CC."

Tento obežník dostanú všetky systémy na lokálnej sieti, no odpovie naň iba počítač B ako vlastník príslušnej IP adresy: 

> "IP adresu 10.1.1.3 mám ja a moja MAC adresa je 11:11:22:22:33:33."

Počítač A si odpoveď dočasne uloží do pamäte (ďalej len "ARP cache") a je schopný začať komunikáciu s počítačom B. 
Životnosť údajov v "ARP cache" je však za normálnych podmienok obmedzená a spravidla sa jednotlivé záznamy odstránia, ak nie sú používané dlhšie než jednu minútu. 
Veľmi dôležité je vedieť, že keď má počítač v "ARP cache" príslušné údaje, nevysiela obežník, ale používa záznam dostupný v pamäti.

Nedostatky tohto spôsobu komunikácie sa prejavujú najmä v prípade, keď sa na lokálnej sieti nachádza systém s duplicitnou IP adresou. 
Ten totiž odpovedá na ARP obežníky ostatných systémov a zanáša tak do ich "ARP cache" nesprávne údaje. 
Následkom toho nie sú tieto systémy schopné s vami komunikovať, čo sa prejavuje výpadkom konektivity. 
V skutočnosti je ale jediným problémom nesprávna MAC adresa v "ARP cache" ostatných systémov. 
Tomuto javu sa dá predísť používaním tzv. statických ARP záznamov, čo je však najmä vo väčších sieťach náročné na údržbu. 
Dovolím si pripomenúť, že záznamy v "ARP cache" je možné zobraziť a spravovať pomocou utility "arp", ktorá je štandardnou súčasťou väčšiny operačných systémov.

# Zabránenie IP konfliktu

Mnohé operačné systémy pri svojom zavádzaní alebo pri zmene sieťových nastavení vysielajú do siete špeciálny typ ARP obežníka, tzv. ["gratuitous ARP"][4]. 
Týmto spôsobom sa snažia zistiť, či náhodou túto IP adresu už nejaké zariadenie v lokálnej sieti nepoužíva. 
Ak nedostanú žiadnu odpoveď, predpokladajú, že zadaná IP adresa je voľná a začnú ju aktívne používať. 
Ak odpoveď dostanú, vo väčšine prípadov na túto skutočnosť upozornia vizuálne alebo zalogovaním udalosti a IP adresu sieťovému rozhraniu nepriradia.

Závažným problémom linuxového jadra je skutočnosť, že na "gratuitous ARP" vôbec nereaguje. 
Systém, ktorý tento obežník vyslal teda nedostane žiadnu odpoveď a spôsobí konflikt. 
Navyše všetky ostatné systémy na lokálnej sieti po prijatí "gratuitous ARP" obežníka upravia údaje vo svojej "ARP cache".

V drvivej väčšine prípadov by bolo možné IP konfliktu zabrániť okamžitým zaslaním odpovede na "gratuitous ARP" a následné vyslanie tohto obežníka by už len zabezpečilo, že všetky systémy upravia údaje vo svojej "ARP cache" na správne hodnoty.

# IP konflikt v Linuxe

V predchádzajúcom texte som už spomínal, že linuxové jadro neobsahuje kód, ktorý by zabezpečil detekciu IP konfliktu a odpovedanie na obežníky typu "gratuitous ARP". 
Na jednej strane je takéto správanie možné považovať za nesprávne, na druhej strane túto "funkcionalitu" využíva napríklad [High-Availability Linux project][5]. 
Absenciu detekcie IP konfliktu sa snažil riešiť aj [Marc Merlin][6], ktorý vytvoril patch pre linuxový kernel s názvom [ARPpatch][7]. 
Implementoval v ňom logovanie IP konfliktu prostredníctvom syslogu, no z ťažko pochopiteľných príčin bol tento patch vývojármi kernelu odmietnutý s odôvodnením, že túto funkcionalitu môže zabezpečovať jednoduchý daemon. 
Súhlasím s p. Merlinom a tiež si myslím, že logovanie tejto udalosti i reakciu na "gratuitous ARP" by mal vykonávať kernel. 
V systémoch, na ktorých je odpoveď na "gratuitous ARP" nežiadúca, by sa mohla táto funkcionalita vypnúť napríklad prostredníctvom pseudo-súborového systému `/proc`.

Myslím, že je málo pravdepodobné, že by sa mi podarilo zmeniť názor vývojárov jadra. 
Preto som založil projekt na serveri [sourceforge.net][8] a v jazyku [Perl][9] vytvoril ukážkovú implementáciu nimi navrhovaného daemona. 
Nazval som ho [IPwatchD][10] a v podstate je to len jednoduchý sniffer, ktorý na zachytávanie a analýzu ARP paketov používa knižnicu [libpcap][11]. 
IPwatchD môže bežať v aktívnom alebo pasívnom móde. 
V pasívnom móde zabezpečuje iba zaznamenanie IP konfliktu prostredníctvom syslog daemona. 
V aktívnom móde okrem toho aj reaguje na obežníky typu "gratuitous ARP" a pomocou utility "arping" na ne odpovedá.

Pre účely tohto článku som pripravil aj [špeciálnu výukovú verziu zdrojových kódov][12], ktorá obsahuje podrobnejšie komentáre. 
Zatiaľ sa jedná len o beta verziu, a preto uvítam akékoľvek návrhy na zlepšenie kódu alebo architektúry daemona.

# Poznámka na záver

Mojím hlavným cieľom bolo v krátkosti a nenáročne objasniť problematiku vzniku a detekcie IP konfliktov, ktorá je však veľmi úzko spätá s protokolom ARP a sieťovým modelom TCP/IP. 
Zistil som, že je veľmi náročné písať o téme, kde "všetko so všetkým súvisí" a udržať text prehľadný a pochopiteľný pre čo najširší okruh čitateľov. 
Akékoľvek nejasnosti sa preto rád pokúsim vysvetliť v diskusii pod článkom.

[Článok bol publikovaný aj na portáli linuxos.sk][13]


[1]: https://en.wikipedia.org/wiki/MAC_address
[2]: https://en.wikipedia.org/wiki/Broadcasting_(networking)
[3]: https://en.wikipedia.org/wiki/Address_Resolution_Protocol
[4]: https://wiki.wireshark.org/Gratuitous_ARP
[5]: http://www.linux-ha.org/
[6]: http://marc.merlins.org/
[7]: http://marc.merlins.org/linux/arppatch/
[8]: https://sourceforge.net/
[9]: http://www.linuxsoft.cz/article.php?id_article=675
[10]: http://ipwatchd.sourceforge.net
[11]: https://www.tcpdump.org/
[12]: ipwatchd.html
[13]: https://linuxos.sk/clanok/detekcia-ip-konfliktu-v-linuxe-ipwatchd/