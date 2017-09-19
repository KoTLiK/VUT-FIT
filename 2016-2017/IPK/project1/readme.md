Dokumentácia projektu 1 do IPK 2016/2017
======================================

### NÁZOV
&nbsp; &nbsp; **ftrestd** - serverová aplikácia pre prenos súborov

&nbsp; &nbsp; **ftrest** - klientska aplikácia pre prenos súborov

### POUŽITIE
&nbsp; &nbsp; **ftrestd** [-r ROOT-FOLDER] [-p PORT]

&nbsp; &nbsp; **ftrest** COMMAND REMOTE-PATH [LOCAL-PATH]

---
### OPIS
Klient/server aplikácie pre prenos súborov, ktoré navzájom komunikujú pomocou HTTP1.1 protokolu, využívajúce RESTful API vybraných operácií _PUT_, _GET_, _DELETE_.
```
PUT /USER-ACCOUNT/REMOTE-PATH?type=[file|folder] HTTP/1.1
```

### PREPÍNAČE
**Server:**
* **-r ROOT-FOLDER** - nastaví koreňový adresár, kde budú ukladané súbory pre jednotlivých užívateľov _(implicitne je nastavený aktuálny adresár)_
* **-p PORT** - určuje port, na ktorom bude server naslúchať _(implicitný port je 6677)_

**Klient:**
* **COMMAND** - je príkaz, ktorý vykoná server
  * **put** - skopíruje súbor z _LOCAL-PATH_ do adresára _REMOTE-PATH_
  * **get** - skopíruje súbor z _REMOTE-PATH_ do aktuálneho lokálneho adresára alebo na miesto určené voliteľným prepínačom _LOCAL-PATH_
  * **del** - vymaže súbor na serveri určeným pomocou _REMOTE-PATH_
  * **lst** - vypíše obsah vzdialeného adresára určeným pomocou _REMOTE-PATH_
  * **mkd** - vytvorí adresár na serveri určeným pomocou _REMOTE-PATH_
  * **rmd** - odstráni adresár na serveri určeným pomocou _REMOTE-PATH_
* **REMOTE-PATH** - udáva vzdialenú cestu adresára alebo súboru na servery
* **LOCAL-PATH** - udáva cestu v lokálnom súborovom systéme, je povinný pre príkaz **put**

---
### IMPLEMENTÁCIA
Serverová aplikácia je implementovaná v jazyku C++ ako iteratívny TCP server využívajúci IPv4 protokol. Server čaká na klienta a po nadviazaní spojenia, očakáva a prijíma HTTP požiadavku, pokiaľ klient nepošle signál, že viac dát už nepríde. Následne je požiadavka spracovávaná, kde sa z hlavičky extrahujú informácie o príkaze a vzdialenej ceste, a ďalej sa získavajú potrebné údaje z hlavičky o obsahu správy, či sa má extrahovať a koľko bajtov obsahuje. Po extrakcií údajov sa overuje správnosť požiadavky, konkrétne sa vyhodnotí vzdialená cesta, či ide o platnú cestu a či ukazuje na súbor, adresár alebo neukazuje na nič. Následne sa podľa príkazu, na základe sady podmienok, vyhodnotí správa. Môžu nastať tri situácie:
* požiadavka nemôže byť obslúžená a príslušná chyba sa odošle naspäť klientovi;
  * _400 Bad Request, &nbsp;403 Forbidden, &nbsp;404 Not Found, &nbsp;409 Conflict_
* požiadavka bude obslúžená, ale nastala vnútorná chyba servera a odošle sa správa o chybe;
  * _500 Internal Server Error_
* požiadavka bude obslúžená úspešne a odošle sa správa o úspechu s požadovaným obsahom.
  * _200 OK_

Klientska aplikácia je rovnako implementovaná v jazyku C++ a využívajúca TCP/IPv4 protokol. Klient na začiatku skontroluje správnosť parametrov. Ak sú parametre správne, vytvorí sa príslušná HTTP hlavička. Pokiaľ bol zadaný príkaz put, je parameter _LOCAL-PATH_ skontrolovaný a v prípade úspešnosti je pribalený binárny obsah súboru do správy. Potom sa nadviaže spojenie so serverom a správa sa odošle. Po odoslaní správy sa uzavrie soket na zapisovanie ďalších dát. Následne klient očakáva odpoveď od servera. Po prijatí správy klient extrahuje z hlavičky správy stav požiadavky. Pokiaľ požiadavka skončila neúspechom, je vypísaný status kód s frázou a príslušnou chybou na štandardný chybový výstup a aplikácia končí neúspechom s návratovou hodnotou 1. Inak aplikácia končí úspešne. Pokiaľ išlo o jeden z príkazov _get_ alebo _lst_, je tento príkaz príslušne obslúžený.

### CHYBY
Pre interné chyby (fatálne), chybové hlásenie predchádza **ERROR:** a klient/server aplikácia je ukončená neúspechom s návratovou hodnotou 1. Všetky ostatné chyby sú spôsobené počas obsluhy požiadavky a sú vypísané na strane klienta vo formáte **STATUS-CODE PHRASE: ERROR-MESSAGE** a rovnako je klient ukončený neúspechom s návratovou hodnotou 1.

---
### PRÍKLADY
**Server:**
```
 $ ./ftrestd
 $ ./ftrestd -r ./
 $ ./ftrestd -p 12345
 $ ./ftrestd -r ./ -p 12345
 $ ./ftrestd -p 12345 -r ./
```
**Klient:**
```
 $ ./ftrest mkd http://localhost:6677/user/folder
 $ ./ftrest get http://localhost:6677/user/folder 
 400 Bad Request: Not a File.
 $ ./ftrest put http://localhost:6677/user/folder/doc.pdf ./doc.pdf
 $ ./ftrest get http://localhost:6677/user/folder/doc.pdf ./
 $ ./ftrest del http://localhost:6677/user/folder/doc.pdf
```

---
### AUTOR
**&nbsp; &nbsp; Milan Augustín &nbsp; &nbsp; (xaugus09)**
