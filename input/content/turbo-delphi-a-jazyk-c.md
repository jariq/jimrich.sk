---
title: "Turbo Delphi a jazyk C"
date: 2007-04-06T20:31:48+00:00
draft: false
ji_lang_name: Slovak
ji_lang_code: sk
ji_rss_desc: Článok predstavuje spôsob, ako napísať časť programu v jazyku C a použiť ju v Delphi.
---

Vyvíjate aplikácie v [Turbo Delphi][1]? 
Stalo sa vám už, že ste chceli v niektorej z nich použiť knižnicu, ktorá poskytovala rozhranie iba pre jazyk C? 
Hľadali ste na internete prepis hlavičkových súborov tejto knižnice použiteľný v Delphi, no vaša snaha bola márna? 
Ak ste aspoň na dve z predchádzajúcich otázok odpovedali kladne, je tento článok určený práve pre vás. 
Predstavuje spôsob, ako napísať časť programu v jazyku C a použiť ju v Delphi.

# Úvod

Jednou z hlavných úloh tohto článku je názorne predviesť, ako v prostredí [Microsoft Visual Studio 2005 Professional][2] vytvoriť dynamicky linkovanú knižnicu (DLL súbor), ktorá exportuje funkcie obsahujúce potrebný kód napísaný v jazyku C a ako ju následne pripojiť k projektu vyvíjanému v Turbo Delphi.

# Vytvorenie DLL knižnice vo Visual C++

Vo Visual Studiu zvoľte možnosť založiť nový projekt pomocou menu "File > New > Project".

V okne "Project Types" vyberte voľbu "Visual C++ > Win32 > Win32 Console Application" a zvoľte názov projektu `DLLtest`.

![Screenshot](dll1.jpg)

V nasledujúcom okne s názvom "Win32 Application Wizard" zvoľte "Application Type: DLL" a "Additional options: Export symbols".

![Screenshot](dll2.jpg)

Visual Studio vygeneruje súbor `DLLtest.cpp`, v ktorom sa nachádza ukážková funkcia `fnDLLtest`:

```
DLLTEST_API int fnDLLtest(void)
{
  return 42;
}
```

Dopíšte pod ňu vlastnú funkciu `fnDLLtext` pracujúcu s reťazcami:

```
DLLTEST_API char * fnDLLtext(char * retazec)
{
  return retazec;
}
```

![Screenshot](dll3.jpg)

V paneli "Solution Explorer" kliknite pravým tlačidlom myši na váš projekt a z kontextového menu vyberte voľbu "Add > New item".

![Screenshot](dll4.jpg)

V okne "Add New Item - DLLtest" vyberte voľbu "Visual C++ > Code > Module-Definition File (.def)" a ako názov nového súboru zadajte `DLLtest`.

![Screenshot](dll5.jpg)

Upravte čerstvo pridaný súbor `DLLtest.def` tak, aby obsahoval mená exportovaných funkcií:

```
LIBRARY "DLLtest"
EXPORTS
fnDLLtest
fnDLLtext
```

![Screenshot](dll6.jpg)

Výberom z rolovacieho menu "Solution Configurations" na paneli "Standard toolbar" zmeňte profil z "Debug" na "Release" a skompilujte knižnicu pomocou menu "BUILD > Build solution".

![Screenshot](dll7.jpg)

V adresári projektu by sa mal nachádzať podadresár `release` obsahujúci výslednú knižnicu `DLLtest.dll`.

# Volanie funkcií z DLL v Borland Delphi

Prekopírujte knižnicu `DLLtest.dll` priamo do adresára vášho delphi projektu.

Za deklaráciou funkcií z knižnice použite klauzuly [`cdecl`][3] a `external`.

Nasledujúci jednoduchý program demonštruje použitie funkcií exportovaných DLL knižnicou vytvorenou podľa tohto článku:

```
program Project1;

{$APPTYPE CONSOLE}

uses
SysUtils;

function fnDLLtest() : integer; cdecl; external 'DLLtest.dll';
function fnDLLtext(retazec : pchar) : pchar; cdecl; external 'DLLtest.dll';

begin

writeln('Vystup funkcie vracajucej integer: ' + IntToStr( fnDLLtest() ));
writeln('Vystup funkcie vracajucej "string": ' + fnDLLtext( Pchar('ahoj') ));
readln;

end.
```

# Záver

Za nevýhodu tohto riešenia sa dá označiť fakt, že spolu s finálnou verziou vašej aplikácie musíte distribuovať aj balík [Microsoft Visual C++ 2005 Redistributable Package][4], bez ktorého bude vytvorená DLL knižnica nepoužiteľná. 
Ak teda máte dostatok času a skúseností, môže byť vhodnejším riešením prepísať hlavičkové súbory spomínanej knižnice z jazyka C do jazyka Object Pascal, alebo celú aplikáciu vyvíjať v [Turbo C++][5].


[1]: https://en.wikipedia.org/wiki/Turbo_Delphi
[2]: https://www.visualstudio.com/
[3]: https://en.wikipedia.org/wiki/Calling_convention
[4]: https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist
[5]: https://en.wikipedia.org/wiki/Turbo_C%2B%2B