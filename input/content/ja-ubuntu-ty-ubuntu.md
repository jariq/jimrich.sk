---
title: "Ja Ubuntu, ty Ubuntu"
date: 2006-08-06T07:41:50+00:00
draft: false
ji_lang_name: Slovak
ji_lang_code: sk
ji_rss_desc: Cieľom článku je previesť mojím "postinštalačným rituálom" pre Ubuntu 6.06 (Dapper Drake) používateľa ľubovoľnej inej distribúcie.
---

Bola doba, keď som mal z inštalácie softvéru zo zdrojových kódov strach, pretože môj [Mandrake][1] pri nej stále vyhadzoval nejaké chyby. 
Niekde som sa dočítal, že existuje distribúcia [Debian][2] a že sú pre ňu dostupné tisíce binárnych balíkov. 
S vidinou lepšej budúcnosti som sa teda pretrápil inštaláciou stable vetvy a zistil som, že balíkov síce sú tisíce, no nie práve najnovších verzií. 
Tak som Debian nechal Debianom a predstavoval si, aký by to bol systém, keby sa dal ľahšie nainštalovať a mal novšie verzie balíkov. 
Dnes už si taký systém predstavovať nemusím. 
Je tu a volá sa [Ubuntu][3].

Hneď v úvode musím upozorniť, že článok nie je určený pre začínajúcich používateľov. 
Jeho cieľom je stručne previesť mojím "postinštalačným rituálom" používateľa ľubovoľnej inej distribúcie a upozorniť ho na niektoré vlastnosti, ktorými sa tento systém vo verzii 6.06 (Dapper Drake) líši od ostatných distribúcií.

# 1. Kde je root?

Pri inštalácii operačného systému GNU/Linux sa zvyčajne zadáva heslo pre používateľa root a vytvára sa aspoň jedno neprivilegované používateľské konto, ktoré je určené pre správcu systému na vykonávanie bežných činností. 
Distribúcia Ubuntu však volí iný prístup. 
Do systému sa nemôžete prihlásiť ako používateľ root, ale používatelia, ktorým je to dovolené (viď `/etc/sudoers`), môžu spúšťať príkazy s privilégiami roota pomocou programu [`sudo`][4]. 
O tom, ktorý z týchto dvoch prístupov je lepší sa vedú nekonečné diskusie. 
Prívrženci klasického spôsobu tvrdia, že k získaniu konta roota stačí pri "sudo prístupe" uhádnuť len jedno heslo, a preto by nemal byť používaný. 
Druhá strana ale oponuje s faktom, že pri bežnom používaní konta roota útočník pozná login a musí uhádnuť už len heslo.

Ak sa rozhodnete používať sudo a budete spúšťať za sebou viacero príkazov, na ktoré sú potrebné práva roota, zvážte použitie príkazu:

```
$ sudo su
```

Po zadaní vášho používateľského hesla budete mať prístupný plnohodnotný rootovský shell. 
Ak však preferujete klasický prístup, stačí ak pre používateľa root vytvoríte heslo

```
$ sudo passwd root
```

a upravíte súbor `/etc/sudoers`, kde odoberiete právo bežným používateľom využívať delegovanie privilégií programom sudo (`man sudoers`).

# 2. Verzie, zdroje balíkov a nástroje APT

Jednotlivé verzie Ubuntu majú samozrejme číselné značenie, no zároveň má každá verzia svoj vlastný názov, ako napríklad "Hoary Hedgehog", "Breezy Badger", či "Dapper Drake". 
Používatelia vo fórach často používajú radšej tieto mená než numerické verzie, preto by ste si mali čím skôr na ne zvyknúť a overiť si, ako sa volá verzia, ktorú používate.

To, že Ubuntu vychádza z Debianu potvrdzuje aj skutočnosť, že používa deb balíky a nástroje na ich správu súhrne známe pod názvom APT. 
V úvode spomínané tisíce balíkov sú rozdelené do [viacerých skupín][5]. 
Prvou z nich je skupina **main** obsahujúca základné komponenty systému, ktoré sú bez výhrad voľne distribuovateľné. 
V skupine **restricted** sa nachádzajú balíky, ktorých licencia nejakým spôsobom čiastočne zabraňuje ich úplne voľnému šíreniu, no sú bežne používané a pre niektoré počítačové systémy nevyhnutné. 
Na balíky z týchto dvoch skupín poskytuje Ubuntu Team plnú podporu a garantuje vydávanie bezpečnostných záplat. 
Skupina balíkov s názvom **universe** obsahuje obrovské množstvo softvéru, ktorý však nie je oficiálne udržiavaný Ubuntu Teamom a o aktualizácie a bezpečnostné záplaty sa stará používateľská komunita. 
Skupina **multiverse** obsahuje na rozdiel od predchádzajúcich troch skupín len softvér, ktorý nie je voľne šíriteľný a je na používateľovi, aby si overil v licenčných podmienkach, či ho používať môže. 
Patrí tu napríklad flash-player od firmy Macromedia. 
Pre staršie verzie systému je k dispozícii ešte skupina balíkov s názvom **backports**, v ktorej sú dostupné nové verzie niektorých často používaných programov prebrané z aktuálnej verzie systému.

To, ktoré skupiny balíkov chcete používať a z akého mirroru ich sťahovať sa nastavuje v konfiguračnom súbore `/etc/apt/sources.list`. 
Na mojom systéme vyzerá nasledovne:

```
deb http://sk.archive.ubuntu.com/ubuntu/ dapper main restricted universe multiverse
deb http://sk.archive.ubuntu.com/ubuntu/ dapper-updates main restricted universe multiverse
deb http://security.ubuntu.com/ubuntu dapper-security main restricted universe multiverse
deb http://sk.archive.ubuntu.com/ubuntu/ dapper-backports main restricted universe multiverse
```

Z predchádzajúcich stĺpcov by ste už mali vedieť, čo znamenajú jednotlivé názvy v týchto riadkoch. 
Snáď len pripomeniem, že po inštalácii sú povolené iba skupiny main a restricted. 
Ak chcete používať softvér z universe a multiverse, musíte tento súbor editovať.

V úvode som spomínal, že na správu balíkov používa Ubuntu nástroje APT. 
Uvediem tu pár príkladov ich použitia, ktoré sú podľa mňa pre správu desktopového systému postačujúce. 
Aktualizovať lokálnu databázu informácií o dostupných balíkoch môžete príkazom:

```
# apt-get update
```

Tento príkaz by ste mali spustiť vždy, keď sa chystáte inštalovať nejaký nový softvér, aby ste inštalovali len najnovšie verzie. 
Aplikovať dostupné updaty na všetky nainštalované balíky môžete následne pomocou príkazu:

```
# apt-get upgrade
```

Ak chcete inštalovať nejaký program a nepoznáte presný názov balíka, môžete sa ho pokúsiť vyhľadať príkazom:

```
# apt-cache search hladanafraza
```

Výstupom z tohto príkazu však niekedy býva nesmierne množstvo balíkov, a preto ho často používam spolu s príkazom `grep`, ktorý zabezpečí, že sa vypíšu iba riadky obsahujúce definované slovo:

```
# apt-cache search hladanafraza | grep slovo
```

Pre zobrazenie podrobnejších informácií o balíku spustite:

```
# apt-cache show nazovbalika
```

Ak už poznáte presný názov balíka, môžete ho nainštalovať pomocou príkazu:

```
# apt-get install nazovbalika
```

Balíky, ktoré už nepotrebujete odoberiete príkazom:

```
# apt-get remove nazovbalika
```

Ďalšie možnosti použitia nástrojov apt môžete nájsť v manuálových stránkach (`man apt`).

# 3. Softvér, ktorý mi po inštalácii chýba

Ideálna distribúcia bohužiaľ neexistuje a v každej mi po inštalácii chýbajú niketoré programy, na ktoré som zvyknutý. 
V Ubuntu pridávam nasledovné balíky:

Midnight Commander:

```
# apt-get install mc
```

Adobe Acrobat Reader:

```
# apt-get install acroread
```

Macromedia flash plugin pre Firefox:

```
# apt-get install flashplugin-nonfree
```

Dodatkové true type fonty od Microsoftu (Arial, Courier, Times New Roman a ďalšie):

```
# apt-get install msttcorefonts
```

Nástroje na kompiláciu zdrojových kódov:

```
# apt-get install build-essential
```

Gnomebaker (aplikácia na napaľovanie CD):

```
# apt-get install gnomebaker
```

Poštový klient Mozilla Thunderbird s podporou pre [GnuPG][6]:

```
# apt-get install mozilla-thunderbird
# apt-get install mozilla-thunderbird-enigmail
```

V prostredí Gnome je predvolený emailový klient Evolution, no ja preferujem Thunderbird. 
Preto ho po inštalácii ešte nastavujem ako predvolený pomocou grafickej aplikácie dostupnej prostredníctvom menu *"System - Preferences - Preferred Applications"*.

Ak vám vadí, že zástupcovia pre Firefox a Thunderbird v menu Applications nepoužívajú oficiálne ikony, môžete si ich stiahnuť z [kde-look.org][7] a pomocou *"Alacarte Menu Editor"* (*"Applications - Accessories - Alacarte Menu Editor"*) to napraviť.

# 4. Podpora slovenčiny

Základným prvkom podpory slovenčiny je možnosť používať slovenskú klávesnicu. 
V prostredí Gnome mi však prvý krát spôsobovalo jej nastavenie menšie problémy, preto popíšem postup detailnejšie. 
Spustite grafický nástroj pre konfiguráciu klávesnice pomocou menu *"System - Preferences – Keyboard"*. 
V záložke *"Layouts"* si pridajte slovenské rozloženie a v záložke *"Layout Options"*, v sekcii *"Group Shift/Lock behavior"* si zvoľte *"Alt+Shift changes group"*, čo zabezpečí, že medzi používanými rozloženiami klávesnice budete prepínať kombináciou kláves `Alt+Shift` rovnako ako v prostredí MS Windows. 
Ak ste si zvykli na zobrazovanie aktuálneho rozloženia na paneli, tak si naň cez menu, ktoré zobrazíte pravým tlačítkom myši pridajte *"Keyboard indicator"*.

Potrebné balíky pre podporu slovenčiny môžete pridať voľbou slovenského jazyka v menu *"System - Administration - Language Support"* alebo priamou inštaláciou balíkov:

```
# apt-get install language-pack-gnome-sk-base language-pack-gnome-sk language-pack-sk-base language-pack-sk aspell-sk mozilla-firefox-locale-sk openoffice.org-l10n-sk thunderbird-locale-sk myspell-sk openoffice.org-help-sk
```

Osobne preferujem anglickú verziu systému a ako predvolený jazyk si nechávam *"English (United States of America)"*. 
Preto ale musím manuálne vykonať úpravy v OpenOffice. 
Ak ste sa rozhodli rovnako ako ja, spustite napríklad Writer (*"Application - Office - OpenOffice.org Word Processor"*) a v jeho menu *"Tools - Options - Language Settings - Languages"* nastavte slovenský jazyk.

# 5. Multimédiá

Mnohé distribúcie dnes používajú na spracovávanie multimediálneho obsahu engine zvaný gstreamer. 
Tento engine pravdepodobne viac vyhovuje ich licenčným podmienkam, pretože neobsahuje kód, ktorý by bol licenčne sporný, no mne sa do neho nikdy nepodarilo pridať podporu pre prehrávanie mp3, DivX či Xvid. 
Vlastne som to ani poriadne neskúšal, pretože gstreamer nahradzujem za engine sprostredkovaný knižnicou xinelib. 
V praxi to znamená len toľko, že nahradím prehrávač totem skompilovaný s podporou gstreamer-u za verziu kompilovanú s podporou xinelib:

```
# apt-get install totem-xine
```

Pri inštalácii tohto balíka sa odoberie pôvodný `totem-gstreamer`. 
Keď som testoval Dapper-a, tak som si všimol, že ak po tomto kroku spustím totem cez menu *"Applications"*, tak okamžite spadne. 
Ak je však spustený cez Nautilus dvojklikom na nejaký video súbor, tak ho bez problému prehrá. 
Aj to je jedna z príčin, prečo ešte stále používam Breezy-ho. 
No o tom až v závere.

Podporu prehrávania videa vo formáte mpeg4 pridáte inštaláciou balíka `libxine-extracodecs`, s ktorým sa nainštaluje aj knižnica `libmad0`, ktorá umožňuje prehrávanie zvukových súborov vo formáte mp3.

```
# apt-get install libxine-extracodecs
```

Tiež je dobré nainštalovať plugin pre Firefox, ktorý umožní prehrávanie videa na webových stránkach.

```
# apt-get install totem-xine-firefox-plugin
```

No a nakoniec už len inštalujem prehrávač zvukových súborov s názvom XMMS:

```
# apt-get install xmms
```

Tento prehrávač používa staré GTK1, preto ak by sa vám jeho vzhľad nepozdával, skúste jeho nasledovníka s názvom Beep Media Player.

# Záver

Ubuntu hodnotím ako skvelý systém na desktop, pretože v oficiálnych repozitároch je dostupné obrovské množstvo balíkov v novších verziách než sú v stable vetve Debianu. 
Už si ani nespomínam, kedy som naposledy musel kompilovať nejaký program zo zdrojových kódov, lebo na `apt-get install takmerkazdybalik` sa zvyká veľmi ľahko :) 
Ubuntu dokonca poopravilo aj môj nie práve najlepší názor na prostredie Gnome. 
Táto distribúcia je určite ústretovým krokom voči začiatočníkom, no ešte stále ich môže odradiť, keď budú musieť upravovať záznamy v `/etc/fstab` kvôli nepripojeným diskovým oddielom so súborovým systémom NTFS, alebo editovať `xorg.conf`, kvôli úprave frekvencie monitora. 
Pomôcť s týmito banálnymi úlohami im však môže viacero dokumentačných projektov, z ktorých musím spomenúť aspoň [Unofficial Ubuntu Starter Guide][8].

Možno ste v texte postrehli, že aj keď Dapper bol vypustený už dávnejšie, ešte stále používam Breezy-ho. 
Je to kvôli viacerým chybám podobným tej s totemom, ktoré som objavil pri testovacej inštalácii a kvôli tomu, že sa viacero mojich známych sťažovalo na stabilitu niektorých aplikácií. 
Neberte však moje frflanie tak, že Dapper je zlý a Breeze je lepší. 
Keby som teraz preinštalovával na mojom desktope systém, určite by som použil Dapper-a, pretože Ubuntu je distribúcia pružne reagujúca na zmeny a je možné, že nedostatky, ktoré ma odradili od upgradu pred dvoma mesiacmi sú už dávno odstránené.

[Článok bol publikovaný aj na portáli linuxos.sk][9]


[1]: http://www.mandriva.com/
[2]: https://www.debian.org
[3]: https://www.ubuntu.com
[4]: https://en.wikipedia.org/wiki/Sudo
[5]: https://www.ubuntu.com/ubuntu/components
[6]: /sila-gnupg/
[7]: http://www.kde-look.org/content/show.php?content=31544
[8]: http://ubuntuguide.org/
[9]: https://linuxos.sk/clanok/ja-ubuntu-ty-ubuntu/