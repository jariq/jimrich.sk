---
title: "Windows ako router"
date: 2007-04-12T19:56:56+00:00
draft: false
ji_lang_name: Slovak
ji_lang_code: sk
ji_rss_desc: Včera som v registroch objavil voľbu IPEnableRouter.
---

Môj názor, že Windows je v sieti takmer nepoužiteľný som dlho obhajoval tvrdením, že neexistuje alternatíva k linuxovému príkazu:

```
# echo "1" > /proc/sys/net/ipv4/ip_forward
```

Včera som však v registroch objavil voľbu `IPEnableRouter` a zostrojil som aj trošku krkolomný príkaz:

```
C:\> reg add HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters /v IPEnableRouter /t REG_DWORD /d 1 /f
```

Značnou nevýhodou oproti linuxu však naďalej ostáva nutnosť reštartovať systém a absencia použiteľného IP filtra.