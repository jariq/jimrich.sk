---
title: "VPN siete s OpenVPN (4)"
date: 2008-12-01T22:30:50+00:00
draft: false
ji_lang_name: Slovak
ji_lang_code: sk
ji_rss_desc: Vo štvrtej časti seriálu sa zoznámime s významom pojmu PKI a pozrieme sa bližšie na konfiguráciu OpenVPN s certifikátmi.
---

Všetky diely seriálu: 
[1](/vpn-siete-s-openvpn-1/) 
[2](/vpn-siete-s-openvpn-2/) 
[3](/vpn-siete-s-openvpn-3/) 
4 
[5](/vpn-siete-s-openvpn-5/)

Vo štvrtej časti seriálu sa zoznámime s významom pojmu PKI a pozrieme sa bližšie na konfiguráciu OpenVPN s certifikátmi.

# 1. Asymetrické šifrovanie a elektronický podpis

V jednej z predchádzajúcich častí seriálu som spomínal, že asymetrické šifry používajú kľúčový pár (napr. RSA) pozostávajúci z privátnej a verejnej časti a že dáta zašifrované verejným kľúčom je možné dešifrovať len privátnym kľúčom a naopak. 
Pre ľahšie pochopenie významu certifikátov sa najskôr zameriame na pojem elektronický podpis. 
Veľmi zjednodušene je možné povedať, že elektronický podpis je dátová štruktúra alebo súbor, ktorý obsahuje hash dokumentu zašifrovaný pomocou privátneho kľúča podpisujúcej osoby.

Predpokladajme, že Alice vlastní kľúčový pár a chce elektronicky podpísať dokument. 
Musí teda vypočítať hash (napr. SHA1) tohto dokumentu a výsledok zašifrovať pomocou svojho privátneho kľúča. 
Pôvodný dokument pošle Alice spolu s podpisom Bobovi e-mailom.

Ak chce mať Bob istotu, že dokument, ktorý dostal e-mailom naozaj pochádza od Alice, musí overiť jej elektronický podpis. 
To znamená, že vypočíta hash prijatého dokumentu pomocou rovnakého algoritmu aký použila Alice, čo je v tomto prípade SHA1. 
Následne použije Bob verejný kľúč Alice na dešifrovanie hashu dokumentu z elektronického podpisu, ktorý vytvorila Alice svojím privátnym kľúčom. 
Ak je Bobom vypočítaný hash dokumentu zhodný s hashom dešifrovaným z elektronického podpisu, Bob môže zodpovedne prehlásiť, že dokument pochádza od Alice a nebol cestou k nemu nikým pozmenený.

Z predchádzajúceho textu je možné získať základnú predstavu o elektronickom podpise, no nezodpovedanou otázkou ostáva, ako sa dostal verejný kľúč Alice k Bobovi. 
Ak by ho Alice poslala Bobovi e-mailom, mohol by ho cestou niekto vymeniť za iný verejný kľúč a Bob by dokumenty podpísané útočníkom považoval za dokumenty pochádzajúce od Alice. 
Je teda veľmi dôležité sprostredkovať medzi zúčastnenými stranami výmenu verejného kľúča dôveryhodným spôsobom. 
V tomto jednoduchom príklade by dôveryhodný spôsob mohol vypadať tak, že Alice donesie Bobovi svoj verejný kľúč osobne na USB kľúči.

Predstavme si ale, že chceme používať elektronický podpis (asymetrické šifrovanie) v oveľa väčšom merítku. 
Napríklad v organizácii, ktorá má stovky zamestnancov. 
Ak by sme použili spôsob výmeny verejných kľúčov osobným prenosom na USB kľúči, musel by každý zamestnanec odniesť svoj kľúč všetkým svojim kolegom. 
To by znamenalo, že zamestnanci by nerobili nič iné, iba by si medzi sebou vymieňali verejné kľúče. 
A to nepočítame s tým, že pri prezradení alebo strate privátneho kľúča by si zamestnanec musel vygenerovať nový kľúčový pár a opäť rozdistribuovať nový verejný kľúč.

Riešení tohto problému je viacero. 
[GnuPG][1] sa ho snaží riešiť princípom tzv. [pavučiny dôvery][2] (z angl. web of trust), no v praxi oveľa viac bežný a rozšírený je princíp [PKI][3] (Public Key Infrastructure) u nás často nazývaný infraštruktúrou verejného kľúča.

# 2. PKI

Aby sme zabránili chaosu pri výmene verejných kľúčov modelom "každý s každým", poveríme jedného zamestnanca evidenciou verejných kľúčov všetkých ostatných - spravíme z neho certifikačnú autoritu (ďalej len CA). 
Úlohou tejto CA bude dôveryhodným spôsobom zozbierať verejné kľúče všetkých zamestnancov a podpísať ich svojim privátnym kľúčom - vydať certifikáty.

Ak bude niekto chcieť overiť podpis dokumentu vytvorený certifikátom ktoréhokoľvek zamestnanca, použije verejný kľúč CA na overenie pravosti verejného kľúča zamestnanca (overí podpis CA na certifikáte zamestnanca) a až potom overí podpis samotného dokumentu. 
Všetci zamestnanci teda budú potrebovať dôveryhodným spôsobom získať iba verejný kľúč (certifikát) CA.

Certifikát bežne obsahuje unikátne sériové číslo, identifikáciu vlastníka kľúčového páru tzv. Subject, verejný kľúč, špecifikáciu platnosti certifikátu (zvyčajne 1 rok) a tiež účel, na ktorý môže byť používaný. 
V prípade, že dôjde k prezradeniu privátneho kľúča zamestnanca alebo zamestnanec ukončí s firmou pracovný pomer, CA pridá sériové číslo jeho certifikátu na zoznam zrušených certifikátov - CRL (Certificate Revocation List). 
Pri overovaní podpisu je preto vždy potrebné okrem kontroly pravosti certifikátu skontrolovať aj jeho platnosť voči CRL.

Osobne za najväčší problém PKI považujem preukázateľnosť dôveryhodnosti CA, čo je vec, ktorá sa bohužiaľ nedá vyriešiť technologicky. 
Certifikačné autority síce svoje privátne kľúče uchovávajú v bezpečnom úložisku (kryptografický hardvér podobný čipovej karte), ale ak spravia napríklad pri overovaní identity žiadateľa o certifikát vedome alebo nevedome chybu či výnimku, má to negatívny dopad na všetkých používateľov PKI dôverujúcich tejto autorite.

# 3. Certifikačná autorita s gnoMint

Predpokladám, že základné pojmy ako PKI, certifikát, CA alebo CRL sú z predchádzajúceho textu jasné, a tak sa môžeme smelo pustiť do vytvorenia vlastnej PKI infraštruktúry. 
Asi nikomu netreba predstavovať aplikáciu [OpenSSL][4], ktorá je de facto štandardným riešením pokiaľ sa jedná o vydávanie certifikátov a prevádzkovanie jednoduchej CA. 
Balík OpenVPN tiež obsahuje sadu skriptov s názvom [EasyRSA][5], ktoré prácu s OpenSSL výrazne uľahčujú.

Osobne však na správu CA preferujem grafické nástroje, kde je ponuka tiež široká. 
Môžete sa napríklad rozhodnúť pre multiplatformovú aplikáciu [XCA][6] využívajúcu knižnicu OpenSSL a grafický toolkit Qt, alebo môžete siahnuť po aplikácii [gnoMint][7], ktorá je grafickou nadstavbou nad knižnicou GnuTLS a využíva grafický toolkit GTK.

Nech už sa rozhodnete pre ktorúkoľvek zo spomínaných aplikácií, pre ďalší postup podľa tohoto článku budete potrebovať vytvoriť self-signed certifikačnú autoritu, vydať certifikáty pre jeden server a dvoch klientov, zrušiť certifikát jedného klienta a vygenerovať CRL.

Pôvodne som chcel na tomto mieste popísať proces vytvorenia a prevádzky CA s aplikáciou gnoMint pomocou screenshot-ov, no kvôli značnej rozsiahlosti som od tohto zámeru upustil a pripravil som radšej [video][8]. 
K dispozícii sú tiež anglické a slovenské titulky, ktoré je nutné manuálne zapnúť v pravom dolnom rohu videa.

<iframe width="560" height="315" src="https://www.youtube.com/embed/KbInXaFbC8g?rel=0" frameborder="0" allowfullscreen></iframe>

Ak ste postupovali podľa videa a vydali ste všetky potrebné certifikáty, mali by ste mať k dispozícii nasledovné súbory:

- `openvpnca.cer`  
  Certifikát CA v PEM formáte
- `openvpnca.crl`  
  Zoznam zrušených certifikátov CA v PEM formáte
- `192.168.1.1.key`  
  Privátny kľúč servera v nešifrovanom PEM formáte
- `192.168.1.1.cer`  
  Certifikát servera v PEM formáte
- `client1.key`  
  Privátny kľúč Klienta 1 v nešifrovanom PEM formáte
- `client1.cer`  
  Certifikát Klienta 1 v PEM formáte
- `client2.key`  
  Privátny kľúč Klienta 2 v nešifrovanom PEM formáte
- `client2.cer`  
  Certifikát Klienta 2 v PEM formáte

# 4. OpenVPN a certifikáty

Konfigurácia OpenVPN s certifikátmi je podobná ako pri statickom kľúči s tým rozdielom, že každá zo zúčastnených strán používa svoj vlastný privátny kľúč a certifikát. 
Rád by som upozornil, že v nasledujúcom texte už nebudem uvádzať podrobnosti typu kam uložiť konfiguráciu, ako to celé spustiť, ako otestovať či je VPN sieť funkčná alebo kde nájsť chybové hlásenia ak niečo nefunguje. 
Odpoveď na všetky uvedené otázky je totiž možné nájsť v predchádzajúcich dieloch seriálu.

Súborom, ktoré obsahujú privátne kľúče je potrebné nastaviť prístupové práva tak, aby ich mohol čítať iba používateľ root. 
Na serveri je ešte potrebné vygenerovať súbor s parametrami pre [Diffie-Hellman Key Agreement Protocol][9] (ďalej len Diffie-Hellman). 
Aj keď je tento súbor možné vygenerovať priamo v aplikácii gnoMint, vo videu som túto aktivitu zámerne vynechal, pretože so samotným PKI priamo nesúvisí. 
Potrebný súbor je možné vygenerovať pomocou aplikácie OpenSSL príkazom:

```
root@server:~# openssl dhparam -out dh.pem 2048
```

Konfigurácia OpenVPN servera bude nasledovná:

```
# Konfiguracny subor VPN servera
local 192.168.1.1
dev tun
server 10.1.1.0 255.255.255.0
ca openvpnca.cer
cert 192.168.1.1.cer
key 192.168.1.1.key
#crl-verify openvpnca.crl
dh dh.pem
push "redirect-gateway"
comp-lzo
keepalive 10 60
ping-timer-rem
persist-tun
persist-key
user openvpn
group openvpn
daemon
```

Väčšina parametrov je známa z predchádzajúcich dielov. 
Nový je napríklad parameter `server` nasledovaný špecifikáciou siete, ktorý v tomto konkrétnom prípade zabezpečí, že server bude klientom automaticky prideľovať IP adresy z rozsahu [10.1.1.0/24][10]. 
Konfiguračná direktíva `ca` určuje cestu k certifikátu CA, ktorej klienti budú serverom akceptovaní, direktíva `cert` cestu k certifikátu servera, direktíva `key` cestu k privátnemu kľúču servera a direktíva `crl-verify` cestu k zoznamu zrušených certifikátov. 
Direktívu `crl-verify` zatiaľ necháme zakomentovanú. 
Direktíva `dh` určuje cestu k súboru s parametrami pre Diffie-Hellman. 
Parameter `push` nasledovaný konfiguračnou direktívou `"redirect-gateway"` sa postará o to, že táto direktíva bude posielaná klientom a tí ju použijú vo svojej konfigurácii. 
Takýmto spôsobom sa dá zo servera centrálne upravovať napríklad smerovacia tabuľka klientov.

Konfigurácia klientov bude nasledovná:

```
# Konfiguracny subor VPN klienta
client
dev tun
remote 192.168.1.1
tls-remote 192.168.1.1
ca openvpnca.cer
cert client1.cer
key client1.key
comp-lzo
keepalive 10 60
ping-timer-rem
persist-tun
persist-key
user openvpn
group openvpn
daemon
```

Aj v tomto prípade je väčšina direktív už známa. 
Novinkou je direktíva `client`, ktorá o.i. povoľuje prijímanie `push` parametrov od servera. 
Význam direktív `ca`, `cert` a `key` je rovnaký ako pri serveri s tým rozdielom, že na klientskej stanici určuje cesty k príslušným súborom patriacim Klientovi 1. 
Nad významom direktívy `tls-remote` budeme ešte polemizovať v ďalšej sekcii článku, no pre úplnosť uvediem, že je nasledovaná hodnotou položky commonName zo subject-u certifikátu servera a zabezpečuje, že sa klient odmietne pripojiť na server, ktorého certifikát nevyhovie tomuto kritériu.

S obdobnou konfiguráciou by sa mal úspešne pripojiť aj Klient 2. 
Ak ste ale pozorne sledovali video a postupovali podľa neho pri vydávaní certifikátov, tak ste si určite všimli, že certifikát Klienta 2 bol certifikačnou autoritou zrušený, a preto by mal byť tomuto klientovi odmietnutý prístup do VPN siete. 
Odkomentujte teda direktívu `crl-verify` v konfigurácii servera, reštartujte na ňom aplikáciu OpenVPN a pokúste sa znova pripojiť s certifikátom Klienta 2. 
Ak ste nikde neurobili chybu, Klientovi 2 bude prístup do VPN siete naozaj odmietnutý.

# 5. Nie je všetko zlato čo sa blyští

Pri používaní PKI si treba v prvom rade uvedomiť, že je len nadstavbou nad asymetrickým šifrovaním. 
Kľúčové páry používané klientom, serverom či samotnou autoritou sa principiálne nijako nelíšia, a preto je napríklad možné použiť certifikát klienta v úlohe certifikačnej autority alebo servera. 
Akousi záplatou, ktorá má zabrániť takémuto "zneužívaniu" kľúčov je špecifikovanie účelu, na ktorý môže byť certifikát používaný. 
Do klientskych certifikátov sa teda zvyčajne pridáva rozšírenie "Extended key usage" s hodnotou "TLS Web client authentication" a do serverových certifikátov "TLS Web server authentication".

Ak však aplikácia pracujúca s certifikátmi tieto rozšírenia ignoruje, či už zámerne alebo nedopatrením, môže to mať negatívny vplyv na bezpečnosť. 
Môžete si to jednoducho vyskúšať tak, že spustíte OpenVPN server s privátnym kľúčom a certifikátom Klienta 1 a pokúsite sa naň pripojiť s privátnym kľúčom a certifikátom Klienta 2. 
Ak vynecháte direktívu `tls-remote`, spojenie bude bez najmenších problémov nadviazané. 
Aplikácia OpenVPN totiž bez dodatočnej konfigurácie nekontroluje účel, na ktorý bol certifikát vydaný. 
Vlastník certifikátu Klient 1 by teda mohol s vysokou pravdepodobnosťou úspešne vykonať [man-in-the-middle][11] (ďalej len MITM) útok na Klienta 2 a naopak. 
Je viacero spôsobov ako tomuto útoku zabrániť, no všetky bohužiaľ vyžadujú, aby sa používateľ orientoval v problematike PKI či sieťových útokov na vyššej než bežnej úrovni.

Prvá možnosť je pomocou direktívy `tls-remote` do konfigurácie klienta napevno zadať commonName certifikátu servera. 
Pre účely tohto článku a siete s jedným serverom je toto riešenie postačujúce, no môže sa stať zásadným problémom, ak by ste potrebovali vytvoriť cluster rozkladajúci zátaž na viacero serverov.

Druhá možnosť je definovať účel použitia certifikátu pomocou neštandardného rozšírenia "Netscape Certificate Type", ktoré OpenVPN radu 2.0 dokáže používať. 
Ako už názov tohto rozšírenia napovedá, jedná sa o proprietárne riešenie spoločnosti Netscape, ktorému by sa malo v dnešnej dobe radšej vyhýbať. 
Preto som ho nepoužil ani v tomto článku.

Tretia a pravdepodobne "najčistejšia" možnosť je vydávať certifikáty pre servery samostatnou certifikačnou autoritou. 
Toto riešenie zabraňuje spomínanému MITM útoku a neznamená problém ani v prípade, že sa rozhodnete použiť cluster. 
Ak pre vás PKI nie je cudzím pojmom, určite viete, že je dobrou praktikou nepoužívať jednu certifikačnú autoritu na všetko, ale vytvoriť sústavu autorít s odlišnými politikami, ktoré vydávajú rôzne typy certifikátov. 
Táto skutočnosť často uniká dokonca aj komerčným spoločnostiam poskytujúcim certifikačné služby napriek tomu, že sa pravdepodobne jedná o najlepšie riešenie.

# 6. Záver

Piaty diel tohto seriálu bude venovaný viacfaktorovej autentizácii a použitiu čipových kariet ako bezpečného úložiska privátnych kľúčov. 
Tento diel bude zároveň posledný, a preto sa prosím ozvite v diskusii pod článkom ak si myslíte, že som niektorej téme nevenoval dostatočný priestor. 
Pokúsim sa to napraviť :)

[Článok bol publikovaný aj na portáli linuxos.sk][12]

Všetky diely seriálu: 
[1](/vpn-siete-s-openvpn-1/) 
[2](/vpn-siete-s-openvpn-2/) 
[3](/vpn-siete-s-openvpn-3/) 
4 
[5](/vpn-siete-s-openvpn-5/)


[1]: https://www.gnupg.org/
[2]: https://en.wikipedia.org/wiki/Web_of_trust
[3]: https://en.wikipedia.org/wiki/Public_key_infrastructure
[4]: https://www.openssl.org/
[5]: https://openvpn.net/index.php/documentation/miscellaneous/rsa-key-management.html
[6]: https://sourceforge.net/projects/xca/
[7]: http://gnomint.sourceforge.net/
[8]: https://www.youtube.com/watch?v=KbInXaFbC8g
[9]: https://en.wikipedia.org/wiki/Diffie%E2%80%93Hellman_key_exchange
[10]: http://jodies.de/ipcalc?host=10.1.1.0&amp;mask1=24&amp;mask2=
[11]: https://en.wikipedia.org/wiki/Man-in-the-middle_attack
[12]: https://linuxos.sk/clanok/vpn-siete-s-openvpn-4/