# ISA Projekt 2017/2018

Vytvořte komunikující aplikaci podle konkrétní vybrané specifikace pomocí síťové knihovny **BSD sockets** *(pokud není ve variantě zadání uvedeno jinak)*. Projekt bude vypracován v jazyce **C/C++**. Pokud individuální zadání nespecifikuje vlastní referenční systém, musí být projekt přeložitelný a spustitelný na serveru merlin.fit.vutbr.cz.

Vypracovaný projekt uložený v archívu .tar a se jménem ```xlogin00.tar``` odevzdejte elektronicky přes IS. Soubor nekomprimujte.

**Termín odevzdání je 20.11.2017 (hard deadline).** Odevzdání emailem po uplynutí termínu není možné.

##### Odevzdaný projekt musí obsahovat:
 - soubor se zdrojovým kódem (dodržujte jména souborů uvedená v konkrétním zadání),
 - funkční Makefile pro překlad zdrojového souboru,
 - dokumentaci (soubor ```manual.pdf```), která bude obsahovat uvedení do problematiky, návrhu aplikace, popis implementace, základní informace o programu, návod na použití. V dokumentaci se očekává následující: titulní strana, obsah, logické strukturování textu, přehled nastudovaných informací z literatury, popis zajímavějších pasáží implementace, použití vytvořených programů a literatura.
 - soubor ```README``` obsahující krátký textový popis programu s případnými rozšířeními/omezeními, příklad spuštění a seznam odevzdaných souborů,
 - další požadované soubory podle konkrétního typu zadání.
 - Pokud v projektu nestihnete implementovat všechny požadované vlastnosti, je nutné veškerá omezení jasně uvést v dokumentaci a v souboru ```README```.
 - Co není v zadání jednoznačně uvedeno, můžete implementovat podle svého vlastního výběru. Zvolené řešení popište v dokumentaci.
 - Při řešení projektu respektujte zvyklosti zavedené v OS unixového typu (jako je například formát textového souboru).
 - Vytvořené programy by měly být použitelné a smysluplné, řádně komentované a formátované a členěné do funkcí a modulů. Program by měl obsahovat nápovědu informující uživatele o činnosti programu a jeho parametrech. Případné chyby budou intuitivně popisovány uživateli.
 - Aplikace __nesmí__ v žádném případě skončit s chybou SEGMENTATION FAULT ani jiným násilným systémovým ukončením (např. dělení nulou).
 - Pokud přejímáte velmi krátké pasáže zdrojových kódů z různých tutoriálů či příkladů z Internetu (ne mezi sebou), tak je nutné vyznačit tyto sekce a jejich autory dle licenčních podmínek, kterými se distribuce daných zdrojových kódů řídí. V případě nedodržení bude na projekt nahlíženo jako na plagiát.
 - Konzultace k projektu podává vyučující, který zadání vypsal.
 - Před odevzdáním zkontrolujte, zda jste dodrželi všechna jména souborů požadovaná ve společné části zadání i v zadání pro konkrétní projekt. Zkontrolujte, zda je projekt přeložitelný.

## **IRC Bot** s logovanim SYSLOG

Pomocí BSD sockets vytvořte jednoduchého IRC bota. Při použití přepínače ```-l``` a za ním následujícím seznamem klíčových slov se provede kontrola přijatých zpráv. Pokud je taková zpráva nalezena (PRIVMSG nebo NOTICE) a obsahuje alespoň jedno klíčové slovo ze seznamu, celé znění zprávy a přezdívka odesílatele zprávy se zaloguje ve formátu: ```<nickname>:msg```. Bot tento log odešle pomocí SYSLOG protokolu (použije pro to pole CONTENT) na logovací server (port 514). SYSLOG zpráva bude obsahovat také pole PRI, TIMESTAMP, HOSTNAME a TAG (viz. [RFC 3164](https://www.ietf.org/rfc/rfc3164.txt)). Facility nechť je vždy local0 a severity Informational. Jako HOSTNAME pole stačí IP adresa. Komunikace s IRC serverem bude pomocí TCP, logování pak pomocí UDP. V případě, že nebude použita volba ```-s```, použijte jako adresu syslog serveru ip ```127.0.0.1```. Pokud není uveden seznam klíčových slov, není potřeba cokoli logovat.

V následující sekci jsou popsány dvě funkce bota. Pokud je na nějaký kanál zaslána zpráva uvozená znakem ```?``` a klíčovým slovem (```?today``` a ```?msg```), provede bot definovanou operaci.

#### Funkce:

 - ```?today``` - zašle zprávu na daný kanál s datem ve formátu ```dd.mm.yyyy```
 - ```?msg <nickname>:<msg>``` - pokud je uživatel na daném kanále, odešle mu zprávu okamžitě na tentýž kanál, pokud uživatel momentálně není na kanále, zprávu si uloží a odešle ji hned jakmile se daný uživatel připojí na daný kanál (obsah zprávy bude ```<nickname>:<msg>```)

#### Použití:
```
isabot HOST[:PORT] CHANNELS [-s SYSLOG_SERVER] [-l HIGHLIGHT] [-h|--help]
```
 - ```HOST``` je název serveru (např. irc.freenode.net)
 - ```PORT``` je číslo portu, na kterém server naslouchá (výchozí **6667**)
 - ```CHANNELS``` obsahuje název jednoho či více kanálů, na které se klient připojí (název kanálu je zadán včetně úvodního ```#``` nebo ```&```, v případě více kanálů jsou tyto __odděleny čárkou__)
 - ```-s SYSLOG_SERVER``` je ip adresa logovacího (SYSLOG) serveru
 - ```-l HIGHLIGHT``` seznam klíčových slov oddělených čárkou (např. "ip,tcp,udp,isa")

#### Příklady použití:
```
isabot irc.freenode.net:6667 "#ISAChannel,#IRC" -s 192.168.0.1 -l "ip,isa"
isabot irc.freenode.net "#ISAChannel,#IRC" -l "ip,isa" -s 127.0.0.1
isabot irc.freenode.net #ISAChannel -l "tcp,udp"
```

#### Příklad logu:

Bot přijme IRC zprávu:
```
:xlogin00!xlogin00@asdf.cz PRIVMSG #ISAChannel :isa je nej
```

a odešle SYSLOG zprávu:
```
<134>Sep 21 16:00:00 192.168.0.1 isabot <xlogin00>: isa je nej
```

#### Upřesnění zadání:

 - jako nickname, username, hostname, servername a realname použijte svůj FIT xlogin
 - uvažujte komunikaci jen s IRC servery bez hesla a bez potřeby registrace
 - slova ze seznamu porovnávejte **case-sensitive**
 - v případě jakékoli chyby vypište chybu na stderr a program ukončete
 - program se ukončí pomocí SIGINT (ctrl+c)
 - jako referenční stroj pro překlad a běh bota použijte školní server merlin

#### Dokumentace

Pro vytvoření kvalitní dokumentace se řiďte fakultními pokyny pro psaní textu a také pravidly pro bibliografické citace. Můžete vyjít z šablony pro BP/DP a upravit ji. Soupis častých chyb naleznete v zadání Ing. Holkoviče.

#### Literatura:
 - [RFC 1459](https://www.ietf.org/rfc/rfc1459.txt)
 - [RFC 3164](https://www.ietf.org/rfc/rfc3164.txt)
