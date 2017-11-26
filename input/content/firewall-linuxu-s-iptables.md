---
title: "Firewall linuxu s iptables"
date: 2005-07-19T15:06:21+00:00
draft: false
ji_lang_name: Slovak
ji_lang_code: sk
ji_rss_desc: Článok predstavuje základy práce s obslužným programom iptables, ktorý slúži na konfiguráciu firewallu v systémoch s jadrom Linux.
---

Systémy pracujúce s jadrom Linux využívajú na filtrovanie paketov jeho časť zvanú [netfilter][1]. 
Konfigurácia netfiltru sa vykonáva pomocou obslužných programov `ipchains` alebo `iptables`. 
Ipchains sú k dispozícii pre jadrá 2.2 a pre jadrá 2.4 a vyššie je možné použiť aj iptables. 
Nie je ich však možné kombinovať, čo znamená, že buď máte firewall vytvorený iba s iptables alebo iba s ipchains. 
Syntax oboch programov je veľmi podobná, avšak významný rozdiel medzi nimi je v tom, že iptables poskytujú možnosti stavového firewallu, čo veľmi uľahčuje vytváranie pravidiel najmä pre gateway, ale i bežnú pracovnú stanicu či server. 
Preto sa budeme zaoberať práve tvorbou firewallu pomocou iptables.

# 1. Úvod

Pravidlá pre filtrovanie paketov sa pomocou iptables zoraďujú do troch hlavných reťazí - **INPUT**, **OUTPUT** a **FORWARD**. 
Reťaz INPUT, ako už jej názov napovedá, bude obsahovať pravidlá pre pakety vstupujúce do systému zo siete. 
Reťaz OUTPUT zas obsahuje pravidlá pre pakety odchádzajúce z nášho systému do siete a reťaz FORWARD sa využíva pri budovaní routerov a firewallov oddeľujúcich siete. 
Pri bežnom zabezpečení pracovnej stanice alebo servera sa reťaz FORWARD nevyužíva.

Paket, ktorý príde do systému je porovnávaný postupne so všetkými pravidlami odpovedajúcej reťaze (pre prichádzajúce pakety je to INPUT) a pri nájdení prvého pravidla, ktorému vyhovuje sa s ním vykoná akcia, ktorú toto pravidlo definuje. 
Najbežnejšie akcie sú **ACCEPT**, **REJECT** a **DROP**. 
ACCEPT povolí prechod paketu, DROP paket zahodí a REJECT paket odmietne, pričom o tom pomocou ICMP správy informuje jeho zdroj.

Na výpis všetkých pravidiel definovaných pomocou iptables použijeme príkaz `iptables -L`. 
Samozrejme musíme byť prihlásený ako používateľ root. 
Ak počítač nemá definované pravidlá, teda je úplne otvorený, bude výpis vyzerať nasledovne:

```
[root@linux root]# iptables -L
Chain INPUT (policy ACCEPT)
target prot opt source destination

Chain FORWARD (policy ACCEPT)
target prot opt source destination

Chain OUTPUT (policy ACCEPT)
target prot opt source destination
```

Na výpise môžeme vidieť už spomínané tri hlavné reťaze a za každou z nich v zátvorke uvedenú politiku reťaze. 
Politika reťaze je základným pravidlom, ktoré platí pre paket, ktorý sa nezhoduje so žiadnym pravidlom z danej reťaze. 
Z príkladu je zrejmé, že pre pakety prichádzajúce do systému zo siete (INPUT) je politika ACCEPT a pretože reťaz neobsahuje žiadne pravidlá, budú všetky pakety prijaté. 
Obdobná situácia je aj v reťaziach FORWARD a OUTPUT.

# 2. Syntax

Po nutnom, avšak veľmi odľahčenom teoretickom úvode nasleduje názorná ukážka syntaxe iptables. 
Tieto príklady obsahujú popis len základných parametrov. 
Pre kompletný opis možností odporúčam preštudovať manuálové stránky programu iptables `man iptables`.

```
# iptables -F
```

Vymaže všetky definované pravidlá v reťaziach INPUT OUTPUT a FORWARD.

```
# iptables -P INPUT DROP
```

Nastaví politiku (-P) pre reťaz INPUT na DROP. 
Na vstupe zo siete sú teda odhadzované všetky pakety, pre ktoré neexistuje iné pravidlo.

```
# iptables -A INPUT -p tcp --dport 80 -m state --state NEW -j ACCEPT
```

Do reťaze INPUT pridá (-A) stavové pravidlo (-m state), ktoré povolí (-j ACCEPT) zahájiť nové (--state NEW) TCP spojenie (-p tcp) smerované na cieľový port 80 (--dport 80).

```
# iptables -A INPUT -p tcp --dport 22 -s 10.1.1.15 -m state --state NEW -j ACCEPT
```

Do reťaze INPUT pridá (-A) stavové pravidlo (-m state), ktoré povolí (-j ACCEPT) zahájiť nové (--state NEW) TCP spojenie (-p tcp) smerované na cieľový port 22 (--dport 22). 
Toto spojenie však musí pochádzať z IP adresy 10.1.1.15 (-s 10.1.1.15).

```
# iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT
```

Do reťaze INPUT pridá (-A) stavové pravidlo (-m state), ktoré povolí (-j ACCEPT) prechod pre pakety, ktoré sú príbuzné alebo patria do už nadviazaného spojenia (--state ESTABLISHED,RELATED).

# 3. Pracovná stanica

Toto boli príklady pravidiel, ktorých znalosť je postačujúca pre vytvorenie firewallu určeného pre pracovnú stanicu či server. 
Ukážme si ďalej príklad kompletného firewallového skriptu pre pracovnú stanicu, na ktorej nebežia žiadne sieťové služby (ftp, www, samba..), ku ktorým by pristupovali užívatelia z iných systémov. 
(Všetky tu uvedené ukážkové skripty sú obyčajné shell skripty. Každý z nich môžeme uložiť, napríklad ako `/etc/rc.d/rc.firewall` a jeho spúšťanie pri štarte počítača zabezpečíme pridaním záznamu do `/etc/rc.d/rc.local`)

```
#!/bin/sh
iptables -P INPUT DROP
iptables -P FORWARD DROP
iptables -P OUTPUT ACCEPT
iptables -F
iptables -A INPUT -s 127.0.0.1 -i lo -m state --state NEW -j ACCEPT
iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT
echo "Pravidla firewallu zavedene.."
```

Tento skript je naozaj veľmi krátky, no aj účinný. 
Na začiatku skriptu sa nastavuje politika hlavných reťazí a vymazujú sa všetky existujúce pravidlá. 
Reťaz INPUT má politiku DROP, aby sa zamedzilo pokusom o pripojenie sa k nejakej na počítači bežiacej sieťovej službe. 
Reťaz FORWARD má politiku DROP, pretože sa jedná o jednoduchý systém, ktorý nepreposiela pakety medzi dvoma sieťami. 
Politika ACCEPT reťaze OUTPUT znamená, že akákoľvek sieťová komunikácia, ktorú zahájime z našej stanice je povolená. 
Nasledujúce pravidlo povoľuje spojenia z localhostu. 
Localhost je fiktívne sieťové rozhranie reprezentujúce náš systém a ten sám so sebou komunikovať môže. 
Ak by toto pravidlo chýbalo, nemusel by vôbec na pracovnej stanici fungovať grafický X-server. 
Posledné pravidlo skriptu garantuje vstup do systému paketom, ktoré pochádzajú z niektorého spojenia, ktoré sme nadviazali z našej stanice. 
Teda z vonkajšej siete nemôže byť zahájená komunikácia s naším systémom, ale náš systém môže zahájit komunikáciu s vonkajšími systémami a tejto komunikácii bude garantovaný hladký priebeh.

# 4. Webový server

Ďalší príklad bude firewallový skript pre webový server. 
Webový server zvyčajne obsluhuje pripojenia na port 80/TCP. 
Predpokladajme, že náš fiktívny server má byť dostupný pre celý svet, ale nesmie byť vôbec dostupný pre IP adresu 10.1.1.145, z ktorej sme zaznamenali pokusy o útok. 
Skript bude opäť veľmi jednoduchý a bude to vlastne len doplnený predchádzajúci skript.

```
#!/bin/sh
iptables -P INPUT DROP
iptables -P FORWARD DROP
iptables -P OUTPUT ACCEPT
iptables -F
iptables -A INPUT -s 127.0.0.1 -i lo -m state --state NEW -j ACCEPT
iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT
# DOPLNENE PRAVIDLA
iptables -A INPUT -s 10.1.1.145 -j DROP
iptables -A INPUT -p tcp --dport 80 -m state --state NEW -j ACCEPT
echo "Pravidla firewallu zavedene.."
```

Vidíme, že kostra skriptu ostala úplne rovnaká a rovnaký je aj jej význam. 
Doplnené pravidlá odzrkadľujú presne naše požiadavky. 
IP adresu 10.1.1.145 sme úplne zbavili prístupu a povolili sme prichádzajúcu komunikáciu len na port 80/TCP.

# 5. Pokročilý skript

Jednoduché skripty máme za sebou. 
Ukážeme si aj jeden mierne pokročilý. 
Pokročilý však iba v tom, že je trošku programátorsky "vyfintený". 
Avšak už základné ukážkové skripty poskytujú postačujúcu ochranu pre pracovnú stanicu, poprípade server v sieti, ktorá je chránená firewallom. 
Predpokladajme teda, že tvoríme skript pre počítač, na ktorom má bežat ftp server a webový server aj s podporou šifrovaného spojenia https. 
Ďalej je len pre administrátora prístupný SSH server. 
O všetkých prípadných pokusoch pripojenia na SSH server inou osobou než administrátor chceme mať záznam v logoch. 
Tiež odpovedať na ping má server iba administrátorovi. 
Skript môže byť nasledujúci:

```
#!/bin/sh

# Definicia premennych
IPT=/sbin/iptables
ADMIN=10.1.1.123
SLUZBY="21 80 443"

# Defaultna politika
$IPT -P INPUT DROP
$IPT -P OUTPUT ACCEPT
$IPT -P FORWARD DROP
$IPT -F

# SSHDROP retaz na logovanie
$IPT -N SSHDROP 2> /dev/null
$IPT -A SSHDROP -j LOG --log-prefix 'SSH-pokus-DROP: '
$IPT -A SSHDROP -j DROP

# Ping zo stanice administratora
$IPT -A INPUT -p icmp --icmp-type echo-request -s $ADMIN -m state --state NEW -j ACCEPT

# SSH zo stanice administratora
$IPT -A INPUT -p tcp --dport 22 -s $ADMIN -m state --state NEW -j ACCEPT

# Iny pokus o pripojenie k secure shell zaloguj a odopri
$IPT -A INPUT -p tcp --dport 22 -s ! $ADMIN -j SSHDROP

# Povolene sluzby
for i in $SLUZBY;
do
  $IPT -A INPUT -p tcp --dport $i -m state --state NEW -j ACCEPT
done

# Spojenia z localhostu
iptables -A INPUT -s 127.0.0.1 -i lo -m state --state NEW -j ACCEPT

# Grantovany navrat nadviazanym spojeniam
$IPT -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT

# Zavedenie modulu pre sledovanie FTP spojeni
modprobe ip_conntrack_ftp

echo "Pravidla firewallu zavedene.."
```

Premenné a cyklus sú v tomto skripte použité kvôli uľahčeniu písania skriptu a ten sa zároveň aj nepatrne skracuje. 
Trochu neznáme sa môže zdať vytvorenie vlastnej reťaze SSHDROP. 
To má na starosti riadok `$IPT -N SSHDROP 2> /dev/null`, ktorý zároveň presmeruje chybovú hlášku v prípade, že reťaz s týmto menom už existuje. 
Táto reťaz len zoskupuje viaceré pravidlá, s ktorými sa bude porovnávať na ňu odoslaný paket. 
V našom prípade, ak príde požiadavka na SSH server z inej IP adresy než je administrátorova, budú sa tieto pakety spracovávať reťazou SSHDROP. 
To znamená, že budú zalogované do kernelových logov a následne zahodené. 
Na povolenie vybraných TCP portov je použitý cyklus, ktorý sa vykoná pre všetky hodnoty v reťazci SLUZBY. 
No a posledný riadok skriptu zavedie do jadra modul ip_conntrack_ftp, ktorý pomáha netfiltru sledovať stav FTP spojení a umožnuje využívať službu FTP v aktívnom i v pasívnom móde. 
Nezabúdajte, že výpis pravidiel pomocou už notoricky známeho `iptables -L` je skvelý na kontrolu správnosti skriptu.

# 6. Záver

Filtrovanie paketov pomocou iptables sa dá využiť nielen na ochranu počítača pred sieťou, ale aj na ochranu siete pred používateľmi počítača. 
Pridaním pravidiel povoľujúcich iba služby DNS a WEB do reťaze OUTPUT môžeme docieliť, že bežný používateľ systému bude môcť využívať iba programy pracujúce s týmto portom, čiže webové prehliadače. 
Tu však možnosti iptables ani zďaleka nekončia. 
Dajú sa pomocou nich vytvárať dedikované firewallové systémy brániace celé siete. 
Konfigurácia takýchto zariadení je však omnoho zložitejšia, lebo neraz obsahujú aj tri, či štyri sieťové adaptéry a človek vykonávajúci túto prácu sa musí dobre orientovať aj v otázkach bezpečnosti linuxového jadra. 
Je jasné, že tento článok iba načrtol ako sa dá vytvárať firewall. 
Viac informácií môžete nájsť v manuálových stránkach alebo na adrese spomínanej v úvode.

[Článok bol publikovaný aj na portáli linuxos.sk][2]


[1]: https://www.netfilter.org
[2]: https://linuxos.sk/clanok/firewall-linuxu-s-iptables/