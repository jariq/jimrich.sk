---
title: "VPN siete s OpenVPN (1)"
date: 2008-10-14T07:15:07+00:00
draft: false
ji_lang_name: Slovak
ji_lang_code: sk
ji_rss_desc: Prvý diel série o OpenVPN je zameraný skôr teoreticky a jeho úlohou je nenáročne objasniť základné princípy virtuálnych privátnych sietí.
---

Všetky diely seriálu: 
1 
[2](/vpn-siete-s-openvpn-2/) 
[3](/vpn-siete-s-openvpn-3/) 
[4](/vpn-siete-s-openvpn-4/) 
[5](/vpn-siete-s-openvpn-5/)

Prvý diel série o OpenVPN je zameraný skôr teoreticky a jeho úlohou je nenáročne objasniť základné princípy virtuálnych privátnych sietí.

## 1. Načo je VPN

Ak pracujete v IT oblasti, je celkom bežné, že počas práce nevyužívate len váš osobný počítač, ale pristupujete aj na viacero iných systémov vo firemnej sieti. 
Pokiaľ sedíte v kancelárii nie je to najmenší problém. 
Situácia sa však mierne komplikuje ak nestíhate a potrebujete si prácu vziať so sebou domov alebo na služobnú cestu. 
Firemný notebook máte síce so sebou, ale k plnohodnotnej práci vám chýbajú systémy, ktoré sa nachádzajú v lokálnej sieti vašej firmy.

Tento problém sa dá riešiť viacerými spôsobmi v závislosti od toho, aké prostriedky na firemných systémoch využívate. 
Ak sa jedná iba o zdieľané súbory, môžete si ich napríklad prekopírovať na lokálny disk. 
Ak sa jedná o prácu na inom operačnom systéme než je nainštalovaný na vašom počítači, môžete ho virtualizovať pomocou nástrojov ako [VMware][1] alebo [Xen][2]. 
Po čase používania týchto riešení však pravdepodobne zistíte, že sú v porovnaní s priamym prístupom vo firme časovo oveľa náročnejšie a menej pohodlné. 
Navyše vzniká množstvo rôznych kópií a verzií rovnakých súborov a budete musieť stráviť ďalší čas riešením problémov s ich synchronizáciou.

Ideálne riešenie by v tomto prípade malo zamestnancom umožniť služby poskytované firemnými systémami využívať zo sietí vonkajších. 
Ak by sme ale jednotlivé služby publikovali napr. forwardovaním portov na firewalle, bezpečnosť celého riešenia (a zároveň celej firemnej siete) by klesla na mieru bezpečnosti najmenej bezpečnej publikovanej služby. 
Preto je lepšie sprístupniť všetky služby jednotným spôsobom využívajúcim šifrovanie prenosov prípadne i viac-faktorovú formu autentifikácie používateľov. 
Takýmto riešením môže byť napríklad umožnenie vzdialeného prístupu do firemnej siete pomocou [VPN][3] - virtuálnej privátnej siete (z angl. virtual private network).

## 2. Ako VPN funguje

VPN siete sú často prevádzkované ako klient-server aplikácie, čoho príkladom môže byť [OpenVPN][4] alebo implementácia [PPTP][5] v MS Windows. 
VPN server (ďalej len server) vtedy beží priamo na firewallovom systéme a pri svojom spustení vytvorí virtuálne sieťové rozhranie (virtuálnu sieťovú kartu) a tým aj ďalší subnet firemnej siete. 
Server očakáva spojenia na vonkajšom rozhraní firewallového systému a vykonáva autentifikáciu VPN klienta (ďalej len klient), ktorý sa pripája z vonkajšej siete pomocou klientskej aplikácie. 
Po úspešnom overení je klientskému systému pridelená IP adresa z virtuálneho subnetu. 
Medzi serverom a klientom je následne vytvorený šifrovaný tunel, ktorým sú bezpečne prenášané všetky pakety smerované do a z lokálnej siete. 
Samozrejme spojenia medzi VPN a lokálnou sieťou je možné obmedzovať pomocou pravidiel firewallu a tým jednoducho určiť, ku ktorým systémom sa klienti môžu dostať.

Ak sa vám predchádzajúci odstavec nezdal zrozumiteľný, nič si z toho nerobte. 
Po vysvetlení princípu šifrovaného tunelu si ukážeme využitie VPN siete na konkrétnom príklade.

## 3. Princíp šifrovaného tunelu

Tunely sú vo všeobecnosti považované za niečo záhadné a každý, kto ich spomenie, je "cool" :). 
Netreba sa ich však zbytočne báť, pretože ich princíp je veľmi jednoduchý.

Informácie sú v [IP sieťach][6] prenášané vo forme paketov, ktoré okrem prenášaných dát (payload) obsahujú aj viacero rôznych hlavičiek (headers), v ktorých je možné nájsť informácie o zdroji paketu i o cieli, pre ktorý je určený. 
Predstavme si sieťový paket ako kamión, ktorý ma na dverách číslo cieľového portu, na ktorý smeruje (cieľovú IP adresu pre zjednodušenie zanedbajme). 
SSH packet by teda vyzeral nejako takto:

![Nákladné auto prezentujúce SSH paket](paket.png)

Podstata tunelovania vo VPN sieťach spočíva v tom, že pakety pre vzdialenú privátnu sieť sú zašifrované a následne vložené do dátovej časti verejných paketov. 
Vzdialený VPN server prijme verejný paket a dešifrovaním jeho obsahu získa paket pre privátnu sieť. 
Predstaviť si to môžeme tak, že kamión vezie v nákladnej časti druhý kamión a až ten obsahuje reálne dáta. 
V prípade, že VPN server počúva na porte 443 a komunikujeme s SSH serverom vo firemnej sieti, bude paket VPN spojenia vyzerať nejako takto:

![Nákladné auto prezentujúce VPN paket](vpn_paket.png)

Pri pohľade na obrázok vám pravdepodobne napadne, že takýto prenos je "mrhanie kapacitou", pretože sivý kamión má menší úložný priestor využiteľný na prepravu tovaru. 
Analogicky to platí aj pri prenose vo VPN sieti, kde sa v jednom pakete nachádzajú viaceré hlavičky dvakrát a to znamená zníženie využiteľnej prenosovej rýchlosti medzi VPN klientom a systémami z firemnej siete.

## 4. Reálny príklad

Predstavme si malú firemnú sieť pozostávajúcu z jediného privátneho subnetu, ktorej klienti využívajú na prístup k internetu jednu bránu plniacu zároveň úlohu firewallového systému. 
Typ pripojenia k internetu nie je v tomto prípade dôležitý, stačí keď povieme, že vonkajšie rozhranie firewallu využíva verejnú IP adresu. 
Zjednodušenú schému siete znázorňuje nasledujúci obrázok:

![Schéma siete](network1.png)

V prípade, že by sa zamestnanec chcel pripojiť k niektorému z firemných systémov z vonkajšej siete, jeho pokus by bol zastavený firewallovým systémom. 
(Skutočnosť, že firemná sieť využíva IP adresy z privátneho rozsahu a teda nie je z verejných sietí smerovateľná pre zjednodušenie zanedbajme.)

Ak na firewallovom systéme tejto siete spustíme VPN server s virtuálnym sieťovým rozhraním v subnete 192.168.2.0/24, budú sa k nemu môcť pripájať zamestnanci z externých sietí. 
Po overení identity bude vzdialenému systému pridelená na virtuálnom rozhraní IP adresa práve zo subnetu 192.168.2.0/24. 
Pre systémy zo subnetu 192.168.1.0/24 sa bude tento systém javiť akoby bol umiestnený v druhom lokálnom subnete, no v skutočnosti bude komunikácia z a do firemnej siete smerovaná cez internet šifrovaným tunelom.

Nasledujúci animovaný obrázok znázorňuje 14 krokov komunikácie VPN klienta so systémom v lokálnej sieti. 
Jednotlivé kroky sú zobrazované automaticky po 10 sekundách.

![Princíp VPN komunikácie](network2.gif)

## 5. Záver

Dúfam, že tento článok splnil svoj cieľ a nenáročnou formou vám priblížil základné princípy VPN sietí. 
V ďalších dieloch si názorne ukážeme ako vytvoriť VPN sieť s aplikáciou OpenVPN i ako jednoducho vytvoriť vlastnú certifikačnú autoritu, ktorá bude vydávať certifikáty VPN klientom. 
Nevyhneme sa ani pokročilejším témam ako je viac-faktorová autentifikácia a s ňou spojené prihlasovanie do VPN siete pomocou certifikátu uloženého na čipovej karte.

[Článok bol publikovaný aj na portáli linuxos.sk][7]

Všetky diely seriálu: 
1 
[2](/vpn-siete-s-openvpn-2/) 
[3](/vpn-siete-s-openvpn-3/) 
[4](/vpn-siete-s-openvpn-4/) 
[5](/vpn-siete-s-openvpn-5/)


[1]: https://www.vmware.com/
[2]: https://en.wikipedia.org/wiki/Xen
[3]: https://en.wikipedia.org/wiki/Virtual_private_network
[4]: https://openvpn.net/
[5]: https://en.wikipedia.org/wiki/Point-to-Point_Tunneling_Protocol
[6]: https://en.wikipedia.org/wiki/Internet_Protocol
[7]: https://linuxos.sk/clanok/vpn-siete-s-openvpn-1/