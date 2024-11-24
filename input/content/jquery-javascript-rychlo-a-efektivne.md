---
title: "jQuery - javascript rýchlo a efektívne"
date: 2009-07-28T07:57:08+00:00
draft: false
ji_lang_name: Slovak
ji_lang_code: sk
ji_rss_desc: Nechcelo sa mi "babrať s čistým javascript-om", a tak som si povedal, že skúsim použiť niektorý z open source frameworkov.
---

Za svoju hráčsku prax som už prešiel nejednu [point-n-click adventúru][1] a občas sa mi stalo, že som v hre "zakysol". 
Je to veľmi nepríjemný pocit a po pár hodinách trápenia v takej situácii siaham po návode. 
Mnohokrát sa však pri hľadaní riešenia nechtiac dozviem z návodu aj veci, ktoré som sa dozvedieť vôbec nechcel a tým si už dopredu tak trochu pokazím zábavu. 
Keď som pred zhruba desiatimi rokmi písal slovenské návody na sériu adventúr [Monkey Island][2], chcel som tento problém eliminovať rozčlenením návodov do sekcií, pričom každá sekcia by bola venovaná iba jedinému problému. 
Základná idea teda bola celkom jednoduchá: 

> Hráč identifikuje správnu sekciu podľa názvu, ktorý obsahuje popis problému a v texte tejto sekcie nájde riešenie len pre svoj aktuálny problém.

Nápad sa mi podarilo úspešne realizovať a s výsledkom som bol dlho spokojný.

Tento mesiac však vyšiel nový diel série s názvom [Tales of Monkey Island][3] a pár dní po ňom aj remake prvej časti s názvom [The Secret of Monkey Island: Special Edition][4]. 
Pri tejto príležitosti som sa rozhodol revidovať spomínané návody a napadlo mi, že sa posuniem ešte o krok ďalej a pomocou javascript-u ukryjem pred očami čitateľa text, ktorý nemá resp. nechce vidieť. 
Základná idea sa teda rozšírila nasledovne: 

> Hráč identifikuje správnu sekciu podľa názvu, ktorý obsahuje popis problému a **po kliknutí na názov sekcie sa mu zobrazí jej text**, kde nájde riešenie len pre svoj aktuálny problém.

Nechcelo sa mi však "babrať s čistým javascript-om", a tak som si povedal, že skúsim použiť niektorý z open source frameworkov. 
Svoju pozornosť som upriamil na frameworky [Dojo][5] a [jQuery][6]. 
Ako prvé som vyskúšal Dojo, no keď sa mi nepodarilo dosiahnuť požadovaný efekt ani po dvoch hodinách, povedal som si, že je čas ísť ďalej. 
Na rad prišlo jQuery a na moje veľké prekvapenie bol výsledok na svete v priebehu desiatich minút.

jQuery má veľmi jednoduchú syntax a s jeho pomocou dokážete manipulovať naozaj s ľubovoľným objektom (alebo skupinou objektov) nachádzajúcim sa v HTML dokumente. 
Z tutoriálov som veľmi rýchlo zistil, že pre skrytie všetkých odstavcov (HTML element p) stačí použiť kód:

```
$("p").fadeOut(1000);
```

Tento príklad som pozmenil tak, aby sa skrývali iba odstavce vnorené v elementoch používajúcich CSS triedu s názvom "section":

```
$(".section").find("p").fadeOut(1000);
```

Potom už stačilo len pridať obsluhu udalosti kliknutia ľavého tlačidla myši a sformátovať text návodu podľa nasledovného príkladu:

```
<div class="section">
  <h4>Názov kapitoly</h4>
  <p>Text kapitoly..</p>
</div>
```

Vďaka jQuery som sa tiež úplne vyhol nepríjemnému ladeniu javascript-u pre rôzne prehliadače. 
Všetky mnou použité funkcie totiž fungovali "out-of-the-box" vo Firefox-e, Internet Explorer-i, Google Chrome i v Opere.

Výsledok si môžete pozrieť v návodoch na stránke [Monkey Island Corner][7].


[1]: https://en.wikipedia.org/wiki/Point-and-click_adventure_game
[2]: https://en.wikipedia.org/wiki/Monkey_Island_(series)
[3]: http://store.steampowered.com/app/31170/Tales_of_Monkey_Island_Complete_Pack/
[4]: http://store.steampowered.com/app/32360/The_Secret_of_Monkey_Island_Special_Edition/
[5]: https://dojotoolkit.org/
[6]: https://jquery.com/
[7]: http://monkey.jimrich.sk