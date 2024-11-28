---
title: "Environment.SpecialFolder on Windows, Linux and OS X"
date: 2015-01-18T12:45:58+00:00
draft: false
ji_lang_name: English
ji_lang_code: en
ji_rss_desc: I have created simple test application which dumps values of System.Environment.SpecialFolder enumeration members and executed it on Windows 8.1 with .NET Framework 4.5, on Ubuntu 14.04 LTS with Mono 3.2.8 and on Mac OS X 10.10.1 Yosemite with Mono 3.10.0.
---

While developing [Pkcs11Admin][1] - multiplatform application written in C# - I was trying to figure out where to store log file so it can be easily reached by the user on all supported operating systems - Windows, Linux and Mac OS X.

To avoid hardcoding I wanted to use [`System.Environment.SpecialFolder`][2] enumeration but I was not quite sure which member (`ENM`) to choose because the documentation does not contain the actual paths. 
So I have created simple test application which dumps these paths and executed it on Windows 8.1 with .NET Framework 4.5 (`WIN`), on Ubuntu 14.04 LTS with Mono 3.2.8 (`LIN`) and on Mac OS X 10.10.1 Yosemite with Mono 3.10.0 (`OSX`).

Here are the results:

```
ENM: System.Environment.SpecialFolder.AdminTools
WIN: C:\Users\jariq\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Administrative Tools
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.ApplicationData
WIN: C:\Users\jariq\AppData\Roaming
LIN: /home/jariq/.config
OSX: /Users/jariq/.config

ENM: System.Environment.SpecialFolder.CDBurning
WIN: C:\Users\jariq\AppData\Local\Microsoft\Windows\Burn\Burn
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.CommonAdminTools
WIN: C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Administrative Tools
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.CommonApplicationData
WIN: C:\ProgramData
LIN: /usr/share
OSX: /usr/share

ENM: System.Environment.SpecialFolder.CommonDesktopDirectory
WIN: C:\Users\Public\Desktop
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.CommonDocuments
WIN: C:\Users\Public\Documents
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.CommonMusic
WIN: C:\Users\Public\Music
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.CommonOemLinks
WIN: empty
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.CommonPictures
WIN: C:\Users\Public\Pictures
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.CommonProgramFiles
WIN: C:\Program Files\Common Files
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.CommonProgramFilesX86
WIN: C:\Program Files (x86)\Common Files
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.CommonPrograms
WIN: C:\ProgramData\Microsoft\Windows\Start Menu\Programs
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.CommonStartMenu
WIN: C:\ProgramData\Microsoft\Windows\Start Menu
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.CommonStartup
WIN: C:\ProgramData\Microsoft\Windows\Start Menu\Programs\StartUp
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.CommonTemplates
WIN: C:\ProgramData\Microsoft\Windows\Templates
LIN: /usr/share/templates
OSX: /usr/share/templates

ENM: System.Environment.SpecialFolder.CommonVideos
WIN: C:\Users\Public\Videos
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.Cookies
WIN: C:\Users\jariq\AppData\Local\Microsoft\Windows\INetCookies
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.Desktop
WIN: C:\Users\jariq\Desktop
LIN: /home/jariq/Desktop
OSX: /Users/jariq/Desktop

ENM: System.Environment.SpecialFolder.DesktopDirectory
WIN: C:\Users\jariq\Desktop
LIN: /home/jariq/Desktop
OSX: /Users/jariq/Desktop

ENM: System.Environment.SpecialFolder.Favorites
WIN: C:\Users\jariq\Favorites
LIN: empty
OSX: /Users/jariq/Library/Favorites

ENM: System.Environment.SpecialFolder.Fonts
WIN: C:\WINDOWS\Fonts
LIN: /home/jariq/.fonts
OSX: /Users/jariq/Library/Fonts

ENM: System.Environment.SpecialFolder.History
WIN: C:\Users\jariq\AppData\Local\Microsoft\Windows\History
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.InternetCache
WIN: C:\Users\jariq\AppData\Local\Microsoft\Windows\INetCache
LIN: empty
OSX: /Users/jariq/Library/Caches

ENM: System.Environment.SpecialFolder.LocalApplicationData
WIN: C:\Users\jariq\AppData\Local
LIN: /home/jariq/.local/share
OSX: /Users/jariq/.local/share

ENM: System.Environment.SpecialFolder.LocalizedResources
WIN: empty
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.MyComputer
WIN: empty
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.MyDocuments
WIN: C:\Users\jariq\Documents
LIN: /home/jariq
OSX: /Users/jariq

ENM: System.Environment.SpecialFolder.MyMusic
WIN: C:\Users\jariq\Music
LIN: /home/jariq/Music
OSX: /Users/jariq/Music

ENM: System.Environment.SpecialFolder.MyPictures
WIN: C:\Users\jariq\Pictures
LIN: /home/jariq/Pictures
OSX: /Users/jariq/Pictures

ENM: System.Environment.SpecialFolder.MyVideos
WIN: C:\Users\jariq\Videos
LIN: /home/jariq/Videos
OSX: /Users/jariq/Videos

ENM: System.Environment.SpecialFolder.NetworkShortcuts
WIN: C:\Users\jariq\AppData\Roaming\Microsoft\Windows\Network Shortcuts
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.Personal
WIN: C:\Users\jariq\Documents
LIN: /home/jariq
OSX: /Users/jariq

ENM: System.Environment.SpecialFolder.PrinterShortcuts
WIN: C:\Users\jariq\AppData\Roaming\Microsoft\Windows\Printer Shortcuts
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.ProgramFiles
WIN: C:\Program Files
LIN: empty
OSX: /Applications

ENM: System.Environment.SpecialFolder.ProgramFilesX86
WIN: C:\Program Files (x86)
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.Programs
WIN: C:\Users\jariq\AppData\Roaming\Microsoft\Windows\Start Menu\Programs
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.Recent
WIN: C:\Users\jariq\AppData\Roaming\Microsoft\Windows\Recent
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.Resources
WIN: C:\WINDOWS\resources
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.SendTo
WIN: C:\Users\jariq\AppData\Roaming\Microsoft\Windows\SendTo
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.StartMenu
WIN: C:\Users\jariq\AppData\Roaming\Microsoft\Windows\Start Menu
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.Startup
WIN: C:\Users\jariq\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.System
WIN: C:\WINDOWS\system32
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.SystemX86
WIN: C:\WINDOWS\SysWOW64
LIN: empty
OSX: empty

ENM: System.Environment.SpecialFolder.Templates
WIN: C:\Users\jariq\AppData\Roaming\Microsoft\Windows\Templates
LIN: /home/jariq/Templates
OSX: /Users/jariq/Templates

ENM: System.Environment.SpecialFolder.UserProfile
WIN: C:\Users\jariq
LIN: /home/jariq
OSX: /Users/jariq

ENM: System.Environment.SpecialFolder.Windows
WIN: C:\WINDOWS
LIN: empty
OSX: empty
```

For now I have ended up using `Environment.SpecialFolder.MyDocuments` member because it points to the directories which are easily accessible even by a casual user.


[1]: https://www.pkcs11admin.net
[2]: https://msdn.microsoft.com/en-us/library/system.environment.specialfolder%28v=vs.110%29.aspx