---
title: "VPN siete s OpenVPN (3)"
date: 2008-11-11T22:54:11+00:00
draft: false
ji_lang_name: Slovak
ji_lang_code: sk
ji_rss_desc: V tretej časti seriálu sa zameriame na vylepšenie konfigurácie VPN siete vytvorenej v predchádzajúcej časti a podrobnejšie sa pozrieme aj na využitie OpenVPN pri tunelovaní proxy serverov.
---

Všetky diely seriálu: 
[1](/vpn-siete-s-openvpn-1/) 
[2](/vpn-siete-s-openvpn-2/) 
3 
[4](/vpn-siete-s-openvpn-4/) 
[5](/vpn-siete-s-openvpn-5/)

V tretej časti seriálu sa zameriame na vylepšenie konfigurácie VPN siete vytvorenej v predchádzajúcej časti a podrobnejšie sa pozrieme aj na využitie OpenVPN pri tunelovaní proxy serverov.

## 1. Konfiguračné súbory

Vráťme sa k VPN sieti opisovanej v predchádzajúcej časti.

![Sieťový diagram](static2.png)

Medzi počítačmi A a B sme vytvorili VPN sieť 10.1.1.0/255.255.255.252. 
Systém A v tomto prípade plnil úlohu VPN servera a systém B bol v úlohe VPN klienta. 
Aplikáciu OpenVPN sme na týchto systémoch spúšťali nasledovnými príkazmi:

```
root@A:~# openvpn --dev tun --ifconfig 10.1.1.1 10.1.1.2 --secret static.key
root@B:~# openvpn –remote 192.168.1.1 --dev tun --ifconfig 10.1.1.2 10.1.1.1 --secret static.key
```

Všetky konfiguračné parametre uvedené v týchto príkazoch je možné zapísať do konfiguračného súboru. 
Vytvorme teda na počítači A konfiguračný súbor `/root/mojasiet-server.conf` s nasledovným obsahom:

```
# Konfiguracny subor VPN servera
dev tun
ifconfig 10.1.1.1 10.1.1.2
secret /root/static.key
comp-lzo
keepalive 10 60
ping-timer-rem
persist-tun
persist-key
user openvpn
group openvpn
daemon
```

Význam prvých troch parametrov je známy z predchádzajúcej časti. 
Parameter `comp-lzo` zabezpečí, že dáta budú pred prenosom VPN sieťou komprimované. 
Použitím tohto parametra by sa mala zvýšiť prenosová rýchlosť, no zároveň bude viac zaťažený procesor. 
Parametre `keepalive 10 60` a `ping-timer-rem` nepriamo zabezpečujú, že časový limit spojenia medzi VPN serverom a VPN klientom na sieťových prvkoch nachádzajúcich sa medzi nimi nevyprší, pretože VPN server bude v pravidelných intervaloch "pingovať" klienta. 
Parametre `persist-tun` a `persist-key` zabezpečia, že v prípade automatického reštartu tunelu sa nebude znova konfigurovať virtuálne sieťové rozhranie a čítať šifrovací kľúč. 
To má význam hlavne pri použití direktív `user openvpn` a `group openvpn`, ktoré zabezpečia, že po štarte proces uvoľní privilégia roota a bude ďalej bežať pod bežným používateľom, ktorý nemá na tieto operácie dostatočné oprávnenia. 
Vďaka direktíve `daemon` bude proces bežať na pozadí a nebude vypisovať hlásenia na terminál, ale ich zaznamená prostredníctvom syslog daemona.

Pred samotným spustením OpenVPN je ešte potrebné vytvoriť neprivilegovaného používateľa openvpn patriaceho do rovnomennej skupiny. 
Môžeme to urobiť napríklad pomocou príkazov:

```
root@A:~# groupadd openvpn
root@A:~# useradd -g openvpn openvpn
root@A:~# passwd openvpn
```

Keď je používateľ vytvorený, môžeme spustiť OpenVPN a odovzdať mu cestu ku konfiguračnému súboru pomocou parametra `--config`.

```
root@A:~# openvpn --config /root/mojasiet-server.conf
```

Následne odporúčam overiť, či proces beží príkazom:

```
root@A:~# ps aux | grep openvpn
```

Výstup by sa mal podobať tomuto:

```
openvpn 4308 0.0 0.1 4324 932 ? Ss 18:43 0:00 openvpn --config /root/mojasiet-server.conf
```

Treba pamätať na to, že sme OpenVPN spustili v daemonizovanom móde, a preto prípadnú chybu konfigurácie nevypíše na terminál, ale zaznamená prostredníctvom syslog daemona. 
Ak by teda predchádzajúci príkaz nevyprodukoval žiadny výstup, znamená to, že je v konfigurácii chyba a jej bližší popis je možné nájsť v systémových logoch.

Na počítači B je postup obdobný. 
Taktiež je nutné vytvoriť konfiguračný súbor VPN klienta s názvom `mojasiet-client.conf` a nasledovným obsahom:

```
# Konfiguracny subor VPN klienta
dev tun
remote 192.168.1.1
ifconfig 10.1.1.2 10.1.1.1
secret /root/static.key
comp-lzo
keepalive 10 60
ping-timer-rem
persist-tun
persist-key
user openvpn
group openvpn
daemon
```

Význam jednotlivých konfiguračných direktív je rovnaký ako pri VPN serveri. 
Po vytvorení neprivilegovaného používateľa openvpn patriaceho do skupiny openvpn je možné spustiť OpenVPN podobne ako na počítači A príkazom:

```
root@B:~# openvpn --config /root/mojasiet-client.conf
```

Aj v tomto prípade odporúčam overiť, či proces beží pomocou nástroja `ps` resp. kontrolou systémových logov. 
Overenie funkčnosti samotnej VPN siete je možné vykonať spôsobom popísaným v predchádzajúcom článku pomocou programu `ping`.

## 2. OpenVPN v Ubuntu

Distribúcia Ubuntu sa používateľovi snaží vytváranie OpenVPN sietí čo najviac uľahčiť a robí to najmä vhodnou kombináciou parametrov odovzdávaných z príkazového riadka (v init skripte) s parametrami načítavanými z konfiguračných súborov. 
Tvorca balíka OpenVPN pre distribúciu Ubuntu predpokladá, že používateľ bude ukladať konfiguračné súbory pre jednotlivé VPN siete do adresára `/etc/openvpn` a dá im príponu `conf`. 
Po spustení init skript vyhľadá v spomínanom adresári všetky súbory s touto príponou a spustí pre ne samostatnú inštanciu procesu `openvpn`.

Ak chcete použiť tento distribúciou Ubuntu preferovaný spôsob, musíte statický kľúč presunúť do adresára `/etc/openvpn`. 
Pre zvýšenie bezpečnosti je tiež vhodné pre súbor s kľúčom nastaviť prístupové práva tak, aby ho mohol čítať iba používateľ root. 
Môžete to vykonať napríklad príkazmi:

```
root@A:~# mv static.key /etc/openvpn
root@A:~# chown root.root /etc/openvpn/static.key
root@A:~# chmod 600 /etc/openvpn/static.key
```

Do toho istého adresára je potrebné prekopírovať aj konfiguračný súbor `mojasiet-server.conf`, ktorý sme použili v predchádzajúcom odseku a opraviť v ňom cestu ku súboru so statickým kľúčom z absolútnej `/root/static.key` na relatívnu `static.key`. 
Na záver už ostáva len spustiť OpenVPN ako službu pomocou init skriptu:

```
root@A:~# /etc/init.d/openvpn start
* Starting virtual private network daemon.
* mojasiet-server (OK)
                  [ OK ]
```

Ak by ste chceli systém pripojiť do ďalšej VPN siete, stačí pre ňu vytvoriť konfiguračný súbor a reštartovať spojenia pomocou init skriptu.

## 3. Tunelovanie proxy servera

V predchádzajúcom dieli som spomínal, že medzi hlavné výhody použitia OpenVPN so statickým kľúčom patrí o.i. jednoduchá konfigurácia a skutočnosť, že na server sa môže pripojiť len jeden klient. 
Práve kvôli týmto dvom vlastnostiam s obľubou používam konfiguráciu so statickým kľúčom na tunelovanie proxy serverov alebo prechod cez príliš reštriktívne firewally.

Určite poznáte prípad, keď sa vám správcovia siete snažia za každú cenu znepríjemňovať život monitorovaním a obmedzovaním na každom kroku. 
Samozrejme ich k tomu môže viesť potreba zvýšenia bezpečnosti lokálnej siete, ale občas môžete získať pocit, že to robia skôr kvôli zvyšovaniu sebavedomia a pocitu moci. 
Ak sa rozhodnete tu popísanú techniku použiť vo vašej sieti, dovolím si vám pripomenúť, že v nej môže byť zakázaná a môžete jej použitím porušiť pravidlá používania danej siete, čo sa väčšinou neobíde bez následkov.

Na tomto mieste by som rád zdôraznil, že **každý tunel má dve strany**, a preto musíte mať v externej sieti dostupný počítač, na ktorom môžete spustiť OpenVPN server. 
Pri tunelovaní proxy servera pomocou OpenVPN sa využíva najmä skutočnosť, že SSL spojenie vytvorené aplikáciou OpenVPN je pre bežne nakonfigurovaný proxy server alebo firewall nerozoznateľné od ostatných spojení využívajúcich protokol HTTPS. 
Opisovaná technika sa samozrejme dá použiť aj v sieťach, ktoré nepoužívajú proxy server, ale sú chránené sieťovým firewallom. 
Na jej realizáciu stačí do externých sietí otvorený jediný UDP alebo TCP port.

![Sieťový diagram](proxy.png)

Predpokladajme scenár, keď chceme na systéme "PrivateSystem" získať pripojenie k internetu bez obmedzení, no jediná cesta do externých sietí vedie cez systém "ProxyServer". 
Pri realizácii tohto zámeru použijeme napríklad domáci počítač "PublicSystem", ktorý má verejnú IP adresu alebo je naň "forwardovaný" port 443/TCP zo systému, ktorý ňou disponuje.

Pre realizáciu tohto zámeru je potrebné:

- na systéme "PublicSystem" spustiť OpenVPN server počúvajúci na porte 443/TCP
- na systéme "PublicSystem" nakonfigurovať [NAT][1] pre spojenia VPN klienta "PrivateSystem" smerujúce do internetu
- nakonfigurovať OpenVPN na systéme "PrivateSystem" tak, aby sa pripájal na "PublicSystem" cez "ProxyServer"
- presmerovať sieťovú komunikáciu VPN klienta "PrivateSystem" smerujúcu do internetu cez VPN server "PublicSystem"

Spustenie OpenVPN serveru počúvajúceho na porte 443/TCP na systéme "PublicSystem" by vám už nemalo robiť problém. 
Stačí vygenerovať statický kľúč a použiť konfiguračný súbor s nasledovným obsahom:

```
# Konfiguracny subor VPN servera
dev tun
ifconfig 10.1.1.1 10.1.1.2
proto tcp-server
port 443
secret static.key
comp-lzo
keepalive 10 60
ping-timer-rem
persist-tun
persist-key
daemon
```

Pred konfiguráciou prekladu zdrojových adries (NAT) na systéme "PublicSystem" je najskôr potrebné povoliť preposielanie paketov medzi sieťami, do ktorých je tento systém pripojený. 
Môžeme to urobiť napríklad príkazom:

```
root@PublicSystem:~# echo "1" > /proc/sys/net/ipv4/ip_forward
```

Predpokladajme, že "PublicSystem" používa na pripojenie k internetu sieťové rozrhranie `eth0` a VPN klient "PrivateSystem" používa vo VPN sieti IP adresu 10.1.1.2. 
NAT-ovanie spojení tohto klienta smerujúcich cez "PublicSystem" do internetu je možné zapnúť pravidlom firewallu nastaveným pomocou nástroja iptables:

```
root@PublicSystem:~# iptables -t nat -A POSTROUTING -s 10.1.1.2 -o eth0 -j MASQUERADE
```

Len pre istotu pripomínam, že ak systém "PublicSystem" nebude chránený externým firewallom, je potrebné ho zabezpečiť aj ďalšími pravidlami. 
Ak nemáte s konfiguráciou pravidiel firewallu pomocou iptables skúsenosti, môže vám pomôcť napríklad článok ["Firewall linuxu s iptables"][2].

Spustenie OpenVPN klienta na systéme "PrivateSystem" by vám taktiež nemalo robiť problém. 
Musíte samozrejme použiť rovnaký statický kľúč ako na serveri a konfiguračný súbor s nasledovným obsahom:

```
# Konfiguracny subor VPN klienta
dev tun
remote <PublicSystemIP>
port 443
proto tcp-client
http-proxy <ProxyServerIP> <ProxyServerPort>
http-proxy-retry
http-proxy-option AGENT Mozilla/5.0+(Windows;+U;+Windows+NT+5.1;+en-US;+rv:1.9.0.3)+Gecko/2008092417+Firefox/3.0.3+(.NET+CLR+3.5.30729)
ifconfig 10.1.1.2 10.1.1.1
secret static.key
redirect-gateway
comp-lzo
keepalive 10 60
ping-timer-rem
persist-tun
persist-key
daemon
```

Význam väčšiny konfiguračných direktív už bol objasnený v predchádzajúcom texte. 
Nová je napríklad direktíva `http-proxy`, ktorá definuje proxy server, cez ktorý sa nadväzuje VPN spojenie. 
Táto direktíva musí byť nasledovaná IP adresou a portom tohoto proxy servera. 
Direktíva `http-proxy-option` nasledovaná kľúčovým slovom `AGENT` a identifikačným reťazcom prehliadača zabezpečí, že proxy serveru bude spojenie prezentované ako spojenie vykonané pomocou prehliadača Mozilla Firefox 3.0.3.

Direktíva `redirect-gateway` je v celom procese kľúčová, pretože zabezpečuje úpravu smerovacej tabuľky (angl. [routing table][3]) operačného systému a presmerovanie sieťovej komunikácie VPN klienta "PrivateSystem" smerom do internetu cez VPN server "PublicSystem". 
Toto presmerovanie pozostáva z vytvorenia statickej cesty (angl. route) na "PublicSystem" cez aktuálnu predvolenú bránu (angl. default gateway) a zmeny tejto brány na adresu vzdialeného VPN servera. 
Pri rozpojení VPN siete je smerovacia tabuľka vrátená späť do pôvodného stavu.

Systémy "PublicSystem" a "PrivateSystem" sú v tomto momente pripravené poskytovať výhody neobmedzeného internetu :)

## 4. Záver

Princíp tunelovania proxy servera alebo firewallového systému prezentovaný v tomto článku nie je ničím novým a je využívaný aj mnohými trójskymi koňmi a inými typmi škodlivého kódu. 
Skúste si predstaviť, čo sa môže stať, keď do svojho systému nainštalujete aplikáciu z nedôveryhodného zdroja a tá sa pripojí do útočníkom ovládanej VPN siete. 
Útočník tak v momente získa prístup do vašej lokálnej siete a pri páchaní škôd sa takmer určite nezamerá iba na lokálne zdroje, ale bude cez váš systém pristupovať do ďalších sietí, z tých do ďalších, z tých do ďalších a nakoniec sa stane len veľmi ťažko stopovateľným.

V ďalšej časti sa bližšie pozrieme na vytvorenie a prevádzku certifikačnej autority, ktorá je schopná vydávať certifikáty pre OpenVPN servery i klientov. 
No a samozrejme sa budeme venovať aj konfigurácii OpenVPN v móde s certifikátmi.

[Článok bol publikovaný aj na portáli linuxos.sk][4]

Všetky diely seriálu: 
[1](/vpn-siete-s-openvpn-1/) 
[2](/vpn-siete-s-openvpn-2/) 
3 
[4](/vpn-siete-s-openvpn-4/) 
[5](/vpn-siete-s-openvpn-5/)


[1]: https://en.wikipedia.org/wiki/Network_address_translation
[2]: /firewall-linuxu-s-iptables/
[3]: https://en.wikipedia.org/wiki/Routing_table
[4]: https://linuxos.sk/clanok/vpn-siete-s-openvpn-3/