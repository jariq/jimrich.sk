---
title: "Ktorý modul je ten správny?"
date: 2006-12-30T00:25:52+00:00
draft: false
ji_lang_name: Slovak
ji_lang_code: sk
ji_rss_desc: Ak neviete, ktoré moduly (ovládače) by ste mali mať zavedené v jadre, mohol by vám pomôcť web predstavený v tomto článku.
---

Ak neviete, ktoré moduly (ovládače) by ste mali mať zavedené v jadre, mohol by vám pomôcť web [Debian GNU/Linux device driver check page][1]. 
Stačí vložiť do formulára výstup z príkazu `lspci -n` a stránka vám zobrazí zoznam odporúčaných modulov pre jednotlivé časti vášho počítača. 
Pre úplnosť len pripomínam, že zoznam zavedených modulov sa dá zobraziť príkazom `lsmod`.

[1]: https://kmuto.jp/debian/hcl/