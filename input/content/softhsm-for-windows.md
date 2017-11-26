---
title: "SoftHSM for Windows"
date: 2011-08-17T21:05:56+00:00
draft: false
ji_lang_name: Slovak
ji_lang_code: sk
ji_rss_desc: Minulý týždeň bola vydaná verzia 1.3 softvérového tokenu SoftHSM, do ktorej som prispel viacerými patchmi umožňujúcimi natívny beh v prostredí Microsoft Windows. Zároveň som vytvoril inštalačný balík pre túto platformu.
---

Minulý týždeň bola vydaná verzia 1.3 softvérového tokenu [SoftHSM][1], do ktorej som prispel viacerými patchmi umožňujúcimi natívny beh v prostredí Microsoft Windows. 
Zároveň som vytvoril [inštalačný balík][2] pre túto platformu.

Projekt SoftHSM implementuje rýdzo softvérové kryptografické úložisko dostupné prostredníctvom rozhrania [PKCS#11][3], ktoré môže byť použité napríklad na testovanie aplikácií bez nutnosti vlastniť drahý HSM modul alebo čipovú kartu a príslušný middleware. 
SoftHSM je vyvíjané ako súčasť projektu [OpenDNSSEC][4] a využíva implementáciu kryptografických funkcií z knižnice [Botan][5].


[1]: https://www.opendnssec.org/softhsm/
[2]: https://sourceforge.net/projects/softhsm4windows/
[3]: https://en.wikipedia.org/wiki/PKCS_11
[4]: https://www.opendnssec.org/
[5]: https://botan.randombit.net/