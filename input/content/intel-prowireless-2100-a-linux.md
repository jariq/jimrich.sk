---
title: "Intel PRO/Wireless 2100 a Linux"
date: 2005-10-19T12:14:21+00:00
draft: false
ji_lang_name: Slovak
ji_lang_code: sk
ji_rss_desc: Tiež patríte medzi ľudí, ktorí si kúpili notebook pýšiaci sa technológiou intel centrino a po inštalácii obľúbenej linuxovej distribúcie ste zistili, že "nefunguje" wifi karta? Tento problém sa samozrejme dá riešiť a s týmto návodom by to mal zvládnuť takmer každý.
---

Tiež patríte medzi ľudí, ktorí si kúpili notebook pýšiaci sa technológiou intel centrino a po inštalácii obľúbenej linuxovej distribúcie ste zistili, že "nefunguje" wifi karta? 
Nie je to tým, že by bola vaša distribúcia zlá, ale tým, že ovládače pre karty Intel PRO/Wireless 2100 zatiaľ neboli oficiálne začlenené do linuxového jadra a kvôli reštriktívnej licencii na firmvér asi ani nebudú. 
Tento problém sa samozrejme dá riešiť a s týmto návodom by to mal zvládnuť takmer každý.

[![ipw2100 MiniPCI](ipw2100.jpg)][1]

Zo stránky [ipw2100.sourceforge.net][2] si stiahnite [ovládače][3] a [firmvér][4]. 
Ak máte kernel radu 2.4, budete potrebovať aj [backpatch][5]. 
Inštaláciu vykonávajte ako superužívateľ root. 
Priamo v koreňovom adresári si vytvorte osobitný adresár určený len pre ňu a nakopírujte tam stiahnuté súbory.

```
# mkdir /build
# cp ipw* /build
```

Rozbaľte zdrojové kódy ovládačov.

```
# cd /build
# gunzip ipw2100-1.1.0.tgz
# tar xvf ipw2100-1.1.0.tar
# cd /build/ipw2100-1.1.0
```

Ak máte kernel radu 2.4, je teraz tá správna chvíľa pre aplikovanie backpatchu. 
Ak máte jadro radu 2.6 túto časť samozrejme vynecháte.

```
# patch -p1 < ../ipw2100-1.1.0-1.patch
```

Preveďte kompiláciu a inštaláciu ovládačov notoricky známym

```
# make
# make install
```

Ak vás neprekvapil žiadny error, tak to znamená, že ovládače sú uspešne nainštalované a treba už len na správne miesto prekopírovať firmvér. 
Ak ste boli menej šťastní a nejaká tá chybička sa priplietla, radím vám celé chybové hlásenie skopírovať a dať ho vyhľadávať do googla. 
Je to väčšinou najrýchlejšia cesta k riešeniu problému.

Rozbaľte balík s firmvérom karty:

```
# cd /build
# gunzip ipw2100-fw-1.3.tgz
# tar xvf ipw2100-fw-1.3.tar
```

Slovo firmvér môže znieť strašidelne, no nemusíte sa ho báť. 
Firmvér je do tejto karty nahrávaný vždy, keď je inicializovaná a to v akomkoľvek systéme, nielen v linuxe. 
Nehrozí teda, že by ju to nejako poškodilo. 
Podľa dokumentácie by mali byť súbory s ním nakopírované do adresára `/usr/lib/hotplug/firmware/`, no je lepšie, ak si to overíte v súbore `/etc/hotplug/firmware.agent` napríklad príkazom:

```
# cat /etc/hotplug/firmware.agent | grep FIRMWARE_DIR=
FIRMWARE_DIR=/lib/firmware
```

Ak u vás tento súbor neexistuje, musíte si nainštalovať [hotplug skripty][6]. 
Z výpisu je vidieť, že na Slackware 10.2 je potrebné súbory s firmvérom umiestniť inde, než popisuje dokumentácia ovládačov. 
Prekopírujte teda spomínané súbory do zobrazeného adresára.

```
# cd /build/ipw2100-fw-1.3
# cp ipw2100-1.3* /lib/firmware
```

Nakoniec môžete celý adresár `/build` odstrániť a reštartovať systém.

```
# rm -Rf /build
# reboot
```

Po reštarte si príkazom `lsmod` overte, či kernel používa modul `ipw2100`. Ak nie, nahrajte ho (samozrejme ako root) ručne

```
# modprobe ipw2100
```

Na záver stačí už len nastaviť parametre bezdrôtovej siete. 
Nasledovné príklady by vám v tom mohli pomôcť.

**Sieť typu "ad-hoc" so statickou IP adresou:**

```
# iwconfig eth1 mode ad-hoc channel 5 essid linux
# ifconfig eth1 10.1.1.123 netmask 255.255.255.0 up
# route add default gw 10.1.1.1
```

**Sieť typu "infrastructure" s dynamickou IP adresou:**

```
# iwconfig eth1 mode managed essid provider
# dhcpcd eth1
```

Ďalšie parametre príkazu `iwconfig` z balíka [wireless-tools][7] nájdete v manuáli (`man iwconfig`).

[1]: ipw2100big.jpg
[2]: http://ipw2100.sourceforge.net/
[3]: http://prdownloads.sourceforge.net/ipw2100/ipw2100-1.1.0.tgz?download
[4]: http://ipw2100.sourceforge.net/firmware.php?fid=4
[5]: http://dellaric.home.cern.ch/dellaric/ipw2100/ipw2100-1.1.0-1.patch
[6]: https://sourceforge.net/projects/linux-hotplug/
[7]: http://www.hpl.hp.com/personal/Jean_Tourrilhes/Linux/Tools.html