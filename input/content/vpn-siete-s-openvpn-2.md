---
title: "VPN siete s OpenVPN (2)"
date: 2008-10-20T07:15:20+00:00
draft: false
ji_lang_name: Slovak
ji_lang_code: sk
ji_rss_desc: Teoretické poznatky o VPN sieťach z prvého dielu dnes rozšírime o prvú praktickú skúsenosť. Názorne si predvedieme nasadenie OpenVPN v najjednoduchšej forme t.j. vo forme VPN serveru pre jediného klienta s využitím statického šifrovacieho kľúča.
---

Všetky diely seriálu: 
[1](/vpn-siete-s-openvpn-1/) 
2 
[3](/vpn-siete-s-openvpn-3/) 
[4](/vpn-siete-s-openvpn-4/) 
[5](/vpn-siete-s-openvpn-5/)

Teoretické poznatky o VPN sieťach z prvého dielu dnes rozšírime o prvú praktickú skúsenosť. 
Názorne si predvedieme nasadenie OpenVPN v najjednoduchšej forme t.j. vo forme VPN serveru pre jediného klienta s využitím statického šifrovacieho kľúča.

## 1. Statický kľúč vs. certifikáty

Jednou z kľúčových vlastností každého VPN riešenia je šifrovanie, ktoré zabraňuje nielen odpočúvaniu prenosov ale aj neoprávnenému pozmeneniu prenášaných údajov. 
Šifry môžeme vo všeobecnosti rozdeliť na symetrické a asymetrické.

**Symetrické šifry** zväčša využívajú na šifrovanie údajov i na ich dešifrovanie jedno tajomstvo - v prevažnej väčšine implementácií je to heslo. 
Dáta zašifrované nejakým heslom je možné dešifrovať len tým istým heslom. 
Hlavným problémom tohto spôsobu šifrovania sa teda logicky stáva bezpečná výmena hesla medzi zúčastnenými stranami.

**Asymetrické šifry** narozdiel od symetrických, používajú kľúčový pár (napr. RSA) pozostávajúci z privátnej a verejnej časti. 
Dáta zašifrované verejným kľúčom je možné dešifrovať len privátnym kľúčom a naopak. 
Preto verejná časť kľúčového páru môže byť distribuovaná aj po nezabezpečenom kanáli. 
Viac o používaní asymetrických šifier sa môžete dozvedieť z článku [Sila GnuPG][1].

OpenVPN podporuje obidva uvedené spôsoby šifrovania. 
Symetrické šifrovanie sa využíva v móde s tzv. statickým kľúčom a asymetrické šifrovanie v móde využívajúcom certifikáty. 
Základný zoznam vlastností oboch riešení som zámerne nerozdelil na výhody a nevýhody, pretože vždy závisí od konkrétneho prípadu a našich požiadaviek, či je daná vlastnosť výhodná alebo nevýhodná.

Riešenie so statickým kľúčom:

- využíva symetrické šifrovanie
- jednoduchá konfigurácia
- nemusíte prevádzkovať certifikačnú autoritu
- na server sa môže pripojiť len jeden klient
- kľúč musí byť uložený v textovej podobe na oboch systémoch (klient a server), kde je vystavený riziku, že bude odcudzený

Riešenie s certifikátmi:

- využíva asymetrické šifrovanie
- zložitejšia konfigurácia
- musíte prevádzkovať certifikačnú autoritu
- na server sa môže pripojiť viac ako jeden klient
- kľúč môže byť bezpečne uložený na čipovej karte chránený PIN kódom

Ako už bolo spomenuté v úvode, tento diel seriálu sa zaoberá nasadením OpenVPN so statickým kľúčom. 
Využitie certifikátov bude témou niektorého z ďalších dielov.

## 2. Modelová situácia

Predpokladajme, že máme k dispozícii dva počítače A a B, medzi ktorými chceme vytvoriť VPN sieť. 
Prostredie oddeľujúce tieto dva systémy nie je vôbec podstatné, môže to byť napríklad krížený sieťový kábel alebo aj internet. 
Jediná podmienka je, že systém B musí mať možnosť pripojiť sa na port 1194/UDP systému A.

V ďalšom texte predpokladajme počítače prepojené kríženým káblom. 
Oba počítače majú k dispozícii jediné sieťové rozhranie a je na nich nainštalovaný operačný systém [Ubuntu 6.06 Server][2]. 
Systém A bude na reálnom sieťovom rozhraní používať IP adresu 192.168.1.1 a systém B IP adresu 192.168.1.2.

![Schéma siete](static1.png)

Našou úlohou je medzi týmito počítačmi vytvoriť VPN sieť [10.1.1.0/255.255.255.252][3]. 
Virtuálne sieťové rozhranie systému A v nej bude používať IP adresu 10.1.1.1 a virtuálne sieťové rozhranie systému B IP adresu 10.1.1.2.

![Schéma siete](static2.png)

Systém A bude plniť úlohu VPN servera, čo znamená, že bude na porte 1194/UDP počúvať na prichádzajúce spojenie od systému B, ktorý bude v úlohe VPN klienta.

## 3. Inštalácia OpenVPN

Aplikácia OpenVPN pozostáva z jediného rovnomenného spustiteľného binárneho súboru. 
Tento súbor sa používa na spustenie serveru i klienta, a preto je nutné rovnakú aplikáciu nainštalovať na oba systémy. 
Rozdiel medzi klientom a serverom je len v konfigurácii.

Ak ste zvyknutí pracovať s viacerými operačnými systémami určite oceníte multiplatformovosť, pretože OpenVPN je podľa oficiálnej dokumentácie možné prevádzkovať na platformách Linux 2.2+, Solaris, OpenBSD 3.0+, Mac OS X, FreeBSD, NetBSD a v neposlednej rade aj Microsoft Windows. 
Osobne mám skúsenosti s prevádzkovaním na Linuxe, OpenBSD a MS Windows a môžem potvrdiť, že OpenVPN na týchto platformách funguje naozaj bez najmenších problémov.

OpenVPN odporúčam inštalovať z oficiálnych repozitárov vami používanej distribúcie. 
Ak používate distribúciu [Ubuntu][4], môžete balík "openvpn" zo skupiny "universe" nainštalovať príkazom:

```
$ sudo apt-get install openvpn
```

Pokiaľ používate inú distribúciu a balík OpenVPN sa v nej nenachádza, môžete ho zostaviť zo zdrojových kódov, ktoré sú k dispozícii na [domovskej stránke projektu][5]. 
Budete na to potrebovať v systéme nainštalované knižnice [LZO][6] a [OpenSSL][7]. 
Inštaláciu zo zdrojových kódov môžete vykonať napríklad spustením nasledovných príkazov:

```
# wget http://openvpn.net/release/openvpn-2.0.9.tar.gz
# tar xfz openvpn-2.0.9.tar.gz
# cd openvpn-2.0.9
# ./configure --prefix=/usr/local
# make
# make install
```

V tomto prípade bude výsledný binárny súbor `openvpn` umiestnený do adresára `/usr/local/sbin`.

## 4. Virtuálne sieťové rozhrania

OpenVPN pracuje s virtuálnymi sieťovými rozhraniami [TUN/TAP][8], ktoré softvérovo emulujú sieťové zariadenia a musia mať podporu v jadre operačného systému.

Väčšina distribučných jadier TUN/TAP ovládač obsahuje, no pokiaľ si zostavujete jadro sami, mali by ste sa uistiť, že ste tento ovládač pridali. 
Nachádza sa v sekcii ovládačov sieťových rozhraní pod názvom *"Universal TUN/TAP device driver support"* a jeho prítomnosť vo vašom jadre môžete overiť prítomnosťou riadku

```
CONFIG_TUN=m
```

alebo

```
CONFIG_TUN=y
```

v `config` súbore vytvorenom pri zostavovaní jadra.

Kým bežné sieťové rozhrania ako `eth0` priamo zastupujú hardvér, čo môže byť napríklad sieťová karta v PCI slote, tak pakety prechádzajúce rozhraniami TUN/TAP sú preposielané používateľskému programu. 
Táto vlastnosť sa v prípade OpenVPN využíva o.i. na šifrovanie a dešifrovanie paketov pred ich odoslaním cez reálne sieťové rozhranie.

Rozhranie TAP simuluje klasické zariadenie typu Ethernet pracujúce s rámcami na druhej (linkovej) vrstve [OSI modelu][9] a rozhranie TUN simuluje point-to-point zariadenie pracujúce s paketmi na tretej (sieťovej) vrstve rovnakého modelu. 
Zjednodušene sa dá povedať, že TAP sa primárne používa na vytvorenie bridge-ovanej VPN siete a TUN na vytvorenie route-ovanej VPN siete.

## 5. Spustenie VPN servera a klienta

Takmer všetky konfiguračné nastavenia OpenVPN je možné uviesť priamo cez parametre pri spustení, alebo ich aplikácia môže čítať z konfiguračného súboru. 
Osobne uprednostňujem druhú možnosť kvôli jednoduchšej údržbe, no v nasledujúcom príklade bude kvôli väčšej názornosti konfigurácia vykonaná prostredníctvom parametrov z príkazového riadka. 
Použitie konfiguračných súborov bude popísané v nasledujúcom článku.

Pred prvým spustením OpenVPN v móde so statickým kľúčom je nutné tento kľúč vygenerovať. 
Používa sa na to príkaz:

```
root@A:~# openvpn --genkey --secret static.key
```

V aktuálnom adresári bude po spustení tohto príkazu vytvorený súbor `static.key` s obsahom podobným tomuto:

```
#
# 2048 bit OpenVPN static key
#
-----BEGIN OpenVPN Static key V1-----
05bf51aad90ff49c8807ca92bdc8c350
20cc270e6b46e040864dd5b07fd10a73
8c5452590947cd2a37bc558cd58aa8fd
d78321cc02bb2e7c8db20a677bbab592
00fdedce367674c47d595eb40a410593
568854d77e3128154dbca4f993e5c9da
d16716246fb7be6f9affb3598978afa8
83ce6b6e88ae02767e8257f80ffcca5a
3dc2aae37475fb379514866f83a25fba
8f2fae44583074646b7a5144f129ed28
a46a0569bf9e50f46d1f300fcc8d15d3
9ccce1ec108ad53f79d2dfce55dd4733
7372702f96afa683558d804304723126
18aaeb039085c8f1fb693dd670b2d2ca
915966e7bbc174ebacbec4c718a169d2
2d0a6ebf481a8bae37437c0625784ba0
-----END OpenVPN Static key V1-----
```

OpenVPN server na systéme A môžeme následne spustiť príkazom:

```
root@A:~# openvpn --dev tun --ifconfig 10.1.1.1 10.1.1.2 --secret static.key
```

Parameter `--dev tun` určuje, že má byť použité virtuálne sieťové rozhranie typu TUN. 
Parameter `--ifconfig 10.1.1.1 10.1.1.2` hovorí, že bude vytvorený tunel medzi IP adresou 10.1.1.1 a 10.1.1.2. 
OpenVPN vďaka tomuto parametru automaticky nastaví na vytvorené virtuálne sieťové rozhranie adresu 10.1.1.1 a bude očakávať, že sa pripojí klient s IP adresou 10.1.1.2. 
Parameter `--secret static.key` určuje názov súboru, ktorý obsahuje statický kľúč.

Výstupom tohto príkazu by mal byť text podobný tomuto:

```
root@A:~# openvpn --dev tun --ifconfig 10.1.1.1 10.1.1.2 --secret static.key
Sat Jan 5 17:16:18 2008 OpenVPN 2.0.6 i486-pc-linux-gnu [SSL] [LZO] [EPOLL] built on Apr 10 2006
Sat Jan 5 17:16:18 2008 IMPORTANT: OpenVPN's default port number is now 1194, based on an official port number assignment by IANA. OpenVPN 2.0-beta16 and earlier used 5000 as the default port.
Sat Jan 5 17:16:18 2008 TUN/TAP device tun0 opened
Sat Jan 5 17:16:18 2008 ifconfig tun0 10.1.1.1 pointopoint 10.1.1.2 mtu 1500
Sat Jan 5 17:16:18 2008 UDPv4 link local (bound): [undef]:1194
Sat Jan 5 17:16:18 2008 UDPv4 link remote: [undef]
```

Z výpisu je pre nás podstatná iba informácia o názve vytvoreného virtuálneho sieťového rozhrania, čo je v tomto prípade `tun0`.

Príkaz po spustení ostane blokovať konzolu a ďalší výpis textu uvidíme až pri pripájaní klienta. 
Samozrejme OpenVPN sa dá používať aj ako daemon (systémová služba), ktorý neblokuje terminál. 
Postup priameho spúšťania z konzoly sa však používa najmä pri prvotnom testovaní spojenia, pretože prípadnú chybu v konfigurácii okamžite vidíme na obrazovke.

V ďalšom kroku musíme súbor so statickým kľúčom zo systému A bezpečne (ideálne na USB kľúči alebo pomocou SSH spojenia) preniesť na systém B. 
Ak by niekto tento kľúč pri prenose odcudzil, mohol by dešifrovať všetky údaje prenášané VPN sieťou.

OpenVPN klienta na systéme B môžeme spustiť príkazom:

```
root@B:~# openvpn --remote 192.168.1.1 --dev tun --ifconfig 10.1.1.2 10.1.1.1 --secret static.key
```

Parameter `--remote 192.168.1.1` určuje reálnu IP adresu VPN servera, na ktorý sa má klient pripojiť. 
V tomto prípade sa teda systém B pripojí na adresu 192.168.1.1 a predvolený port 1194/UDP. 
Význam ostatných parametrov je rovnaký ako pri VPN serveri. 
Všimnite si však opačné poradie adries určených parametrom `--ifconfig 10.1.1.2 10.1.1.1`. 
OpenVPN vďaka tomuto parametru automaticky nastaví na vytvorené virtuálne sieťové rozhranie adresu 10.1.1.2 a na druhej strane tunela bude očakávať server s adresou 10.1.1.1.

Výstupom tohto príkazu by mal byť text podobný tomuto:

```
root@B:~# openvpn --remote 192.168.1.1 --dev tun --ifconfig 10.1.1.2 10.1.1.1 --secret static.key
Sat Jan 5 17:17:24 2008 OpenVPN 2.0.6 i486-pc-linux-gnu [SSL] [LZO] [EPOLL] built on Apr 10 2006
Sat Jan 5 17:17:24 2008 IMPORTANT: OpenVPN's default port number is now 1194, based on an official port number assignment by IANA. OpenVPN 2.0-beta16 and earlier used 5000 as the default port.
Sat Jan 5 17:17:25 2008 TUN/TAP device tun0 opened
Sat Jan 5 17:17:25 2008 ifconfig tun0 10.1.1.2 pointopoint 10.1.1.1 mtu 1500
Sat Jan 5 17:17:25 2008 UDPv4 link local (bound): [undef]:1194
Sat Jan 5 17:17:25 2008 UDPv4 link remote: 192.168.1.1:1194
Sat Jan 5 17:17:35 2008 Peer Connection Initiated with 192.168.1.1:1194
Sat Jan 5 17:17:36 2008 Initialization Sequence Completed
```

Na termináli systému A by mali pribudnúť dva riadky informujúce o pripojení klienta:

```
Sat Jan 5 17:17:36 2008 Peer Connection Initiated with 192.168.1.2:1194
Sat Jan 5 17:17:36 2008 Initialization Sequence Completed
```

Po úspešnom nadviazaní spojenia medzi klientom a serverom je medzi systémom A a systémom B vytvorená VPN sieť 10.1.1.0/30.

## 6. Overenie funkčnosti VPN siete

Medzi používanými sieťovými rozhraniami by na oboch systémoch malo pribudnúť jedno rozhranie typu TUN. 
Konfiguráciu sieťových rozhraní systému A môžeme zobraziť príkazom `ifconfig`:

```
root@A:~# ifconfig
eth0 Link encap:Ethernet HWaddr 00:0C:29:34:A4:D1
inet addr:192.168.1.1 Bcast:192.168.1.255 Mask:255.255.255.0
inet6 addr: fe80::20c:29ff:fe34:a4d1/64 Scope:Link
UP BROADCAST RUNNING MULTICAST MTU:1500 Metric:1
RX packets:134 errors:0 dropped:0 overruns:0 frame:0
TX packets:107 errors:0 dropped:0 overruns:0 carrier:0
collisions:0 txqueuelen:1000
RX bytes:13968 (13.6 KiB) TX bytes:15070 (14.7 KiB)
Interrupt:185 Base address:0x1400

lo Link encap:Local Loopback
inet addr:127.0.0.1 Mask:255.0.0.0
inet6 addr: ::1/128 Scope:Host
UP LOOPBACK RUNNING MTU:16436 Metric:1
RX packets:281 errors:0 dropped:0 overruns:0 frame:0
TX packets:281 errors:0 dropped:0 overruns:0 carrier:0
collisions:0 txqueuelen:0
RX bytes:27988 (27.3 KiB) TX bytes:27988 (27.3 KiB)

tun0 Link encap:UNSPEC HWaddr 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00
inet addr:10.1.1.1 P-t-P:10.1.1.2 Mask:255.255.255.255
UP POINTOPOINT RUNNING NOARP MULTICAST MTU:1500 Metric:1
RX packets:0 errors:0 dropped:0 overruns:0 frame:0
TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
collisions:0 txqueuelen:100
RX bytes:0 (0.0 b) TX bytes:0 (0.0 b)
```

Z výpisu je vidieť, že systém A používa rozhranie `tun0` s IP adresou 10.1.1.1. 
Na systéme B je výstup príkazu ifconfig analogický, a teda je z neho vidieť, že tento systém používa rozhranie `tun0` s IP adresou 10.1.1.2:

```
root@B:~# ifconfig
eth0 Link encap:Ethernet HWaddr 00:0C:29:53:E8:F9
inet addr:192.168.1.2 Bcast:192.168.1.255 Mask:255.255.255.0
inet6 addr: fe80::20c:29ff:fe53:e8f9/64 Scope:Link
UP BROADCAST RUNNING MULTICAST MTU:1500 Metric:1
RX packets:138 errors:0 dropped:0 overruns:0 frame:0
TX packets:126 errors:0 dropped:0 overruns:0 carrier:0
collisions:0 txqueuelen:1000
RX bytes:14196 (13.8 KiB) TX bytes:20008 (19.5 KiB)
Interrupt:185 Base address:0x1400

lo Link encap:Local Loopback
inet addr:127.0.0.1 Mask:255.0.0.0
inet6 addr: ::1/128 Scope:Host
UP LOOPBACK RUNNING MTU:16436 Metric:1
RX packets:2 errors:0 dropped:0 overruns:0 frame:0
TX packets:2 errors:0 dropped:0 overruns:0 carrier:0
collisions:0 txqueuelen:0
RX bytes:100 (100.0 b) TX bytes:100 (100.0 b)

tun0 Link encap:UNSPEC HWaddr 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00
inet addr:10.1.1.2 P-t-P:10.1.1.1 Mask:255.255.255.255
UP POINTOPOINT RUNNING NOARP MULTICAST MTU:1500 Metric:1
RX packets:0 errors:0 dropped:0 overruns:0 frame:0
TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
collisions:0 txqueuelen:100
RX bytes:0 (0.0 b) TX bytes:0 (0.0 b)
```

Ak ani jeden zo systémov nepoužíva firewall, malo by byť možné príkazom ping overiť konektivitu medzi virtuálnymi rozhraniami. 
Z počítača A by sa nám malo podariť "pingnúť" virtuálnu IP adresu počítača B:

```
root@A:~# ping 10.1.1.2 -c 4
PING 10.1.1.2 (10.1.1.2) 56(84) bytes of data.
64 bytes from 10.1.1.2: icmp_seq=1 ttl=64 time=1.79 ms
64 bytes from 10.1.1.2: icmp_seq=2 ttl=64 time=0.740 ms
64 bytes from 10.1.1.2: icmp_seq=3 ttl=64 time=0.553 ms
64 bytes from 10.1.1.2: icmp_seq=4 ttl=64 time=0.782 ms

--- 10.1.1.2 ping statistics ---
4 packets transmitted, 4 received, 0% packet loss, time 3013ms
rtt min/avg/max/mdev = 0.553/0.966/1.790/0.483 ms
```

Ak by sme počas pingovania z počítača A pustili na virtuálnom rozhraní počítača B analyzátor sieťovej prevádzky tzv. sniffer, mali by sme vidieť ICMP echo pakety vysielané programom ping a zároveň ICMP reply pakety, ktoré odosiela systém B ako odpoveď:

```
root@B:~# tcpdump -i tun0
tcpdump: WARNING: arptype 65534 not supported by libpcap - falling back to cooked socket
tcpdump: verbose output suppressed, use -v or -vv for full protocol decode
listening on tun0, link-type LINUX_SLL (Linux cooked), capture size 96 bytes
17:41:02.803616 IP 10.1.1.1 > 10.1.1.2: ICMP echo request, id 62222, seq 1, length 64
17:41:02.804933 IP 10.1.1.2 > 10.1.1.1: ICMP echo reply, id 62222, seq 1, length 64
17:41:03.808143 IP 10.1.1.1 > 10.1.1.2: ICMP echo request, id 62222, seq 2, length 64
17:41:03.808165 IP 10.1.1.2 > 10.1.1.1: ICMP echo reply, id 62222, seq 2, length 64
17:41:04.793097 IP 10.1.1.1 > 10.1.1.2: ICMP echo request, id 62222, seq 3, length 64
17:41:04.793120 IP 10.1.1.2 > 10.1.1.1: ICMP echo reply, id 62222, seq 3, length 64
17:41:05.848259 IP 10.1.1.1 > 10.1.1.2: ICMP echo request, id 62222, seq 4, length 64
17:41:05.848280 IP 10.1.1.2 > 10.1.1.1: ICMP echo reply, id 62222, seq 4, length 64

8 packets captured
16 packets received by filter
0 packets dropped by kernel
```

Na reálnom sieťovom rozhraní systému B by však sniffer v tomto momente zachytil len VPN pakety prichádzajúce na port 1194/UDP, ktorý je v nasledujúcom výpise označený slovom "opevpn":

```
root@B:~# tcpdump -i eth0
tcpdump: verbose output suppressed, use -v or -vv for full protocol decode
listening on eth0, link-type EN10MB (Ethernet), capture size 96 bytes
17:41:02.803616 IP 192.168.1.1.openvpn > 192.168.1.2.openvpn: UDP, length 124
17:41:02.804933 IP 192.168.1.2.openvpn > 192.168.1.1.openvpn: UDP, length 124
17:41:03.808143 IP 192.168.1.1.openvpn > 192.168.1.2.openvpn: UDP, length 124
17:41:03.808165 IP 192.168.1.2.openvpn > 192.168.1.1.openvpn: UDP, length 124
17:41:04.793097 IP 192.168.1.1.openvpn > 192.168.1.2.openvpn: UDP, length 124
17:41:04.793120 IP 192.168.1.2.openvpn > 192.168.1.1.openvpn: UDP, length 124
17:41:05.848259 IP 192.168.1.1.openvpn > 192.168.1.2.openvpn: UDP, length 124
17:41:05.848280 IP 192.168.1.2.openvpn > 192.168.1.1.openvpn: UDP, length 124

8 packets captured
16 packets received by filter
0 packets dropped by kernel
```

Tieto výpisy názorne potvrdzujú "princíp šifrovaného tunelu" (vysvetlený v predchádzajúcom dieli tohto seriálu), ktorý spočíva v tom, že pakety pre privátnu sieť sú prenášané v zašifrovanej podobe v dátovej časti verejných paketov. 
V tomto konkrétnom prípade teda pakety vymieňané medzi verejnými rozhraniami s IP adresami 192.168.1.1 a 192.168.1.2 v sebe obsahujú ICMP komunikáciu medzi IP adresami 10.1.1.1 a 10.1.1.2.

## 7. Záver

V treťom dieli sa pokúsime vylepšiť konfiguráciu dnes vytvorenej VPN siete použitím konfiguračných súborov a daemonizáciou procesu. 
Pozrieme sa aj na prístup distribúcie Ubuntu, ktorá vhodne kombinuje konfiguračné parametre odovzdávané z príkazového riadka s parametrami načítavanými z konfiguračných súborov. 
No a aby nechýbal ani príklad z reálneho života, použijeme OpenVPN so statickým kľúčom na tunelovanie proxy servera.

[Článok bol publikovaný aj na portáli linuxos.sk][10]

Všetky diely seriálu: 
[1](/vpn-siete-s-openvpn-1/) 
2 
[3](/vpn-siete-s-openvpn-3/) 
[4](/vpn-siete-s-openvpn-4/) 
[5](/vpn-siete-s-openvpn-5/)


[1]: /sila-gnupg/
[2]: http://www.ubuntu.com/server
[3]: http://jodies.de/ipcalc?host=10.1.1.1&amp;mask1=255.255.255.252&amp;mask2=
[4]: https://www.ubuntu.com
[5]: https://www.openvpn.net
[6]: https://www.oberhumer.com/opensource/lzo/
[7]: https://www.openssl.org
[8]: https://en.wikipedia.org/wiki/TUN/TAP
[9]: https://en.wikipedia.org/wiki/OSI_model
[10]: https://linuxos.sk/clanok/vpn-siete-s-openvpn-2/