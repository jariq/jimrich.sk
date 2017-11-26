---
title: "Ako na vsftpd"
date: 2005-11-08T11:16:46+00:00
draft: false
ji_lang_name: Slovak
ji_lang_code: sk
ji_rss_desc: Tento návod vám pomôže nainštalovať a nakonfigurovať známy vsftpd server.
---

Potrebujete po sieti prijímať väčšie množstvo súborov a už vás a ani vašich kolegov nebaví posielať ich cez icq? 
Riešenie je jednoduché - pustite na vašej pracovnej stanici ftp server. 
Neviete ako na to? 
Čítajte ďalej a tento návod vám pomôže nainštalovať a nakonfigurovať známy [vsftpd][1] server. 
Návod je "šitý na mieru" pre distribúciu Slackware, ale s výnimkou umiestnenia štartovacieho skriptu, by mal byť použiteľný aj na iných distribúciách.

# 1. Inštalácia a príprava systému

Keďže je vsftpd súčasťou spomínanej distribúcie, nemusíme ho kompilovať zo zdrojových kódov, ale stačí napr. pomocou utility `slackpkg` nainštalovať distribučný balík. 
Ako root teda zadáme príkaz:

```
# slackpkg install vsftpd
```

Z bezpečnostných dôvodov by žiadna sieťová služba nemala bežať pod používateľom root. 
Preto aj vsftpd poskytuje možnosť behu pod neprivilegovaným používateľom. 
Na tento účel si vytvoríme fiktívneho používateľa a skupinu s menom vsftpd:

```
# groupadd vsftpd
# useradd -g vsftpd vsftpd
# passwd vsftpd
```

V ďalšom kroku vytvoríme používateľa, ktorý bude mať právo pripájať sa prostredníctvom ftp klienta na náš server. 
Tento používateľ by nemal mať právo prihlásiť sa do systému lokálne. 
Ako predvolený shell mu preto pridelíme `/bin/false`. 
Toto obmedzenie zavádzame kvôli zvýšeniu bezpečnosti systému, pretože heslá sú v komunikácii s protokolom ftp prenášané nekryptované. 
Potenciálny útočník by tak ľahko mohol získať platný login do systému.

Skontrolujeme súbor `/etc/shells` a overíme, či sa tam nachádza záznam pre `/bin/false`. 
Ak nie, pridáme ho tam a následne vytvoríme novú skupinu a už spomínaného používateľa.

```
# groupadd ftpusers
# useradd -g ftpusers -d /var/ftp -s /bin/false ftpuser
# passwd ftpuser
```

Meno tohto používateľa musíme zapísať do zoznamu používateľov, ktorí majú právo sa pripojiť na náš ftp server:

```
# echo "ftpuser" > /etc/vsftpd.users
```

Posledný krok v príprave systému je vytvorenie adresára, v ktorom budú súbory dostupné cez ftp a nastavenie prístupových práv.

```
# mkdir /var/ftp
# chown ftpuser:ftpusers /var/ftp
```

# 2. Nastavenia v súbore vsftpd.conf

Vsftpd server má konfiguráciu uloženú v konfiguračnom súbore `/etc/vsftpd.conf`, ktorý je síce dobre komentovaný, no ako býva zvykom, neobsahuje všetky možné direktívy. 
Tie sa môžeme dozvedieť jedine čítaním manuálu (`man vsftpd.conf`). 
Konfiguračný súbor pre náš jednoduchý server môže vyzerať napríklad takto (po každej časti uvádzam komentár, ktorý do súboru samozrejme nepatrí):

```
#####################################
#   ZAKLADNA KONFIGURACIA SERVERA   #
#####################################
ftpd_banner=FTP-SERVER
listen=YES
background=YES
nopriv_user=vsftpd
```

Tieto direktívy určujú, že démon nemá používať superserver inetd, ale má na porte počúvať sám, má bežať na pozadí a má na svoj beh využívať nami vytvoreného neprivilegovaného používateľa vsftpd.

```
#####################################
#     KONFIGURACIA POUZIVATELOV     #
#####################################
anonymous_enable=NO
local_enable=YES
chroot_local_user=YES
userlist_enable=YES
userlist_deny=NO
userlist_file=/etc/vsftpd.users
```

V tejto časti sa nachádzajú nastavenia vzťahujúce sa na používateľov servera. 
Je tu napríklad zakázané prihlásenie anonymného používateľa, uzamykanie používateľov do ich domovských adresárov (tzv. chrootovanie) a v neposlednom rade je tu aj zmienka o súbore `/etc/vsftpd.users`. 
Ak sa k serveru bude snažiť prihlásiť používateľ, ktorého login nebude uvedený v tomto súbore, bude odpojený ešte pred výzvou na zadanie hesla. 
Takáto konfigurácia sa pričiňuje o zvýšenie bezpečnosti.

```
#####################################
#  POKROCILA KONFIGURACIA SERVERA   #
#####################################
write_enable=YES
ascii_upload_enable=YES
ascii_download_enable=YES
local_umask=022
connect_from_port_20=YES
idle_session_timeout=600
data_connection_timeout=120
ls_recurse_enable=YES
xferlog_enable=YES
xferlog_std_format=YES
xferlog_file=/var/log/vsftpd.log
```

V poslednej časti ukážkového konfiguračného súboru je napríklad povolený upload súborov. 
Tiež sú tu nastavené sieťové parametre a zapnuté logovanie udalostí. 
Napriek tomu, že takto zostavený [konfiguračný súbor][2] by mal byť pre implementáciu slabo zaťaženého servera postačujúci, ešte raz vám odporúčam si aspoň zbežne prezrieť konfiguračné direktívy popísané v manuálových stránkach (`man vstpd.conf`).

# 3. Štartovací skript

Pre uľahčenie spúšťania ftp servera je dobré vytvoriť si štartovací skript, ktorý môže mať napríklad [takúto podobu][3]. 
Uložíme ho do súboru `/etc/rc.d/rc.vsftpd`, a ak chceme server spúšťať priamo po štarte operačného systému, dopíšeme na koniec súboru `/etc/rc.d/rc.local` nasledovný text:

```
if [ -x /etc/rc.d/rc.vsfpd ]; then
  /etc/rc.d/rc.vsftpd start
fi
```

# 4. Pravidlá firewallu

Pre ftp prenosy v aktívnom móde je potrebné na vstupe povoliť nadviazanie spojenia na port 21/tcp. 
Ak chceme používať ftp v pasívnom móde a filtrujeme pomocou stavového firewallu sprostredkovaného netfiltrom, treba do jadra nahrať modul `ip_conntrack_ftp`, ktorý dokáže rozoznávať spojenia vytvorené pasívnym prenosom ftp.

```
# modprobe ip_conntrack_ftp
```

Podrobnejšie sa o problematike filtrovania týchto spojení môžete dočítať v anglickom článku [Linux FTP Server Setup][4], ktorý je vynikajúco spracovaným zdrojom informácií o protokole ftp.

[1]: http://vsftpd.beasts.org/
[2]: vsftpd.conf
[3]: rc.vsftpd
[4]: http://www.linuxhomenetworking.com/wiki/index.php/Quick_HOWTO_:_Ch15_:_Linux_FTP_Server_Setup