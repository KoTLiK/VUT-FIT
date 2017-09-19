--------------------------------------------------------------------------------
--------------------- SQL Skript na projekt z predmetu IDS --------------------- 
--------------------------------------------------------------------------------
-------------------------- Andrej Hučko (xhucko01) -----------------------------
-------------------------- Milan Augustín (xaugus09) ---------------------------
--------------------------------------------------------------------------------

SET SERVEROUTPUT ON

DROP TRIGGER generovanie_idz;
DROP TRIGGER overenie_ico;

DROP PROCEDURE hodnotenie_produktu;
DROP PROCEDURE vypis_kosika;

DROP TABLE zakaznik CASCADE CONSTRAINTS;
DROP TABLE recenzia CASCADE CONSTRAINTS;
DROP TABLE objednavka CASCADE CONSTRAINTS;
DROP TABLE produkt CASCADE CONSTRAINTS;
DROP TABLE dodavatel CASCADE CONSTRAINTS;
DROP TABLE pastelky CASCADE CONSTRAINTS;
DROP TABLE skicare CASCADE CONSTRAINTS;

DROP TABLE je_dodany CASCADE CONSTRAINTS;
DROP TABLE sa_nachadzal CASCADE CONSTRAINTS;
DROP TABLE vlozenie_do_kosika CASCADE CONSTRAINTS;

DROP SEQUENCE sq_zakaznik; -- used in trigger
DROP SEQUENCE sq_recenzia;
DROP SEQUENCE sq_objednavka;
DROP SEQUENCE sq_produkt_s;
DROP SEQUENCE sq_produkt_p;

--------------------------------------------------------------------------------
----------------------------- Vytvorenie tabuliek ------------------------------
--------------------------------------------------------------------------------

CREATE TABLE zakaznik(
  ID_Zakaznik INTEGER PRIMARY KEY NULL,
  meno VARCHAR(42) NOT NULL
);

CREATE TABLE recenzia (
  ID_Recenzia INTEGER PRIMARY KEY,
  datum DATE NOT NULL,
  sprava VARCHAR(512),
  hodnotenie INTEGER NOT NULL,
  
  ID_Produkt INTEGER NOT NULL, -- cudzi kluc
  ID_Zakaznik INTEGER NOT NULL, -- cudzi kluc
  
  CONSTRAINT ch_hodnotenie CHECK (hodnotenie >= 0 AND hodnotenie <= 100)
);

CREATE TABLE objednavka (
  ID_Objednavka INTEGER PRIMARY KEY,
  ulica VARCHAR(42) NULL,
  mesto VARCHAR(42) NULL,
  psc NUMBER(5,0) NULL,
  sposob_dopravy VARCHAR(12) NOT NULL,
  celkova_cena NUMBER NOT NULL,
  
  ID_Zakaznik INTEGER NOT NULL, -- cudzi kluc
  
  CONSTRAINT ch_psc CHECK (psc >= 00001 AND psc <= 99999),
  CONSTRAINT ch_sposob_dopravy CHECK (sposob_dopravy IN ('na dobierku', 'kurierom', 'na predajnu')),
  CONSTRAINT ch_celkova_cena CHECK (celkova_cena > 0)
);

CREATE TABLE dodavatel (
  ico VARCHAR(8) PRIMARY KEY,
  nazov VARCHAR(42) NOT NULL,
  ulica VARCHAR(42) NOT NULL,
  mesto VARCHAR(42) NOT NULL
);

CREATE TABLE produkt (
  ID_Produkt INTEGER PRIMARY KEY,
  cena NUMBER NOT NULL,
  skladom INTEGER NOT NULL,
  oblubenost INTEGER,
  predajnost INTEGER NOT NULL, -- za posledny mesiac
  pocet_v_baleni INTEGER NOT NULL,
  nazov VARCHAR(20),
  
  CONSTRAINT ch_cena CHECK (cena > 0),
  CONSTRAINT ch_skladom CHECK (skladom >= 0),
  CONSTRAINT ch_oblubenost CHECK (oblubenost >= 0 AND oblubenost <= 100),
  CONSTRAINT ch_predajnost CHECK (predajnost >= 0),
  CONSTRAINT ch_pocet_v_baleni CHECK (pocet_v_baleni > 0),
  CONSTRAINT ch_meno_produktu CHECK (LENGTH(nazov) > 0)
);

CREATE TABLE pastelky (
  ID_Produkt INTEGER NOT NULL,
  typ VARCHAR(20) NOT NULL,
  dlzka INTEGER NOT NULL, -- [mm]
  
  UNIQUE (ID_Produkt),
  CONSTRAINT ch_dlzka CHECK (dlzka > 0)
);

CREATE TABLE skicare (
  ID_Produkt INTEGER NOT NULL,
  gramaz INTEGER NOT NULL,
  format_skicaru VARCHAR(4) NOT NULL,
  
  UNIQUE (ID_Produkt),
  CONSTRAINT ch_gramaz CHECK (gramaz > 0)
); 

CREATE TABLE sa_nachadzal(
  ID_Produkt INTEGER NOT NULL,
  ID_Objednavka INTEGER NOT NULL
); 

CREATE TABLE vlozenie_do_kosika(
  ID_Produkt INTEGER NOT NULL,
  ID_Zakaznik INTEGER NOT NULL
); 

CREATE TABLE je_dodany(
  ID_Produkt INTEGER NOT NULL,
  ico VARCHAR(8) NOT NULL
); 

-------------------------------------------------------------------------------- Kompozitne kluce
ALTER TABLE sa_nachadzal ADD (FOREIGN KEY (ID_Produkt) REFERENCES produkt(ID_Produkt));
ALTER TABLE sa_nachadzal ADD (FOREIGN KEY (ID_Objednavka) REFERENCES objednavka(ID_Objednavka));
ALTER TABLE vlozenie_do_kosika ADD (FOREIGN KEY (ID_Produkt) REFERENCES produkt(ID_Produkt));
ALTER TABLE vlozenie_do_kosika ADD (FOREIGN KEY (ID_Zakaznik) REFERENCES zakaznik(ID_Zakaznik));
ALTER TABLE je_dodany ADD (FOREIGN KEY (ID_Produkt) REFERENCES produkt(ID_Produkt));
ALTER TABLE je_dodany ADD (FOREIGN KEY (ico) REFERENCES dodavatel(ico));

-------------------------------------------------------------------------------- Cudzie kluce
ALTER TABLE objednavka ADD (FOREIGN KEY (ID_Zakaznik) REFERENCES zakaznik(ID_Zakaznik));
ALTER TABLE recenzia ADD (FOREIGN KEY (ID_Zakaznik) REFERENCES zakaznik(ID_Zakaznik));
ALTER TABLE recenzia ADD (FOREIGN KEY (ID_Produkt) REFERENCES produkt(ID_Produkt));
ALTER TABLE pastelky ADD (FOREIGN KEY (ID_Produkt) REFERENCES produkt(ID_Produkt));
ALTER TABLE skicare ADD (FOREIGN KEY (ID_Produkt) REFERENCES produkt(ID_Produkt));

-------------------------------------------------------------------------------- Sekvencie

CREATE SEQUENCE sq_zakaznik INCREMENT BY 1 START WITH 1 NOMAXVALUE MINVALUE 1 NOCYCLE; -- used for trigger
CREATE SEQUENCE sq_recenzia INCREMENT BY 1 START WITH 1 NOMAXVALUE MINVALUE 1 NOCYCLE;
CREATE SEQUENCE sq_objednavka INCREMENT BY 1 START WITH 1 NOMAXVALUE MINVALUE 1 NOCYCLE;
CREATE SEQUENCE sq_produkt_s INCREMENT BY 2 START WITH 1 NOMAXVALUE MINVALUE 1 NOCYCLE; -- vsetky neparne
CREATE SEQUENCE sq_produkt_p INCREMENT BY 2 START WITH 2 NOMAXVALUE MINVALUE 2 NOCYCLE; -- vsetky parne


--------------------------------------------------------------------------------
----------------------------------- Triggers -----------------------------------
--------------------------------------------------------------------------------

CREATE OR REPLACE TRIGGER generovanie_idz
BEFORE INSERT ON Zakaznik FOR EACH ROW
BEGIN
  SELECT sq_zakaznik.nextval into :new.ID_ZAKAZNIK FROM dual;
END;
/

CREATE OR REPLACE TRIGGER overenie_ico
BEFORE INSERT ON Dodavatel FOR EACH ROW
DECLARE
  icochar VARCHAR(8);
  loopindex NUMBER;
  ico_chunk NUMBER;
  calculation NUMBER := 0;
  forbidden VARCHAR(8) := '';
BEGIN
  -- dbms_output.put_line('Running ICO Trigger.');
  -- icochar := :new.ico;  
  forbidden := REGEXP_SUBSTR(:new.ico, '[^0-9]');
  icochar := REGEXP_SUBSTR(:new.ico, '[0-9]+');
  -- dbms_output.put_line(icochar);
  -- dbms_output.put_line('ICO to be checked: ' || icochar || ' with length ' || LENGTH(icochar));
  IF ( LENGTH(forbidden) != 0 ) THEN
    -- dbms_output.put_line('nejaky nepovoleny znak, ne');
    RAISE_APPLICATION_ERROR(-20000, 'IČ obsahuje nepřípustný znak');
  END IF;  
  
  IF (LENGTH(icochar) != 8) THEN
    RAISE_APPLICATION_ERROR(-20000, 'IČ nemá správnou délku');
  END IF;

  FOR loopindex in 1..7
  LOOP
    ico_chunk := TO_NUMBER(SUBSTR(icochar,loopindex,1));
    --dbms_output.put_line('Chunk ' || loopindex || ': ' || ico_chunk);
    calculation := calculation + ico_chunk*(9-loopindex);
    --dbms_output.put_line('Counted: ' || ico_chunk || '*' || (9-loopindex));
  END LOOP;
  --dbms_output.put_line('Calculation: ' || calculation );
  ico_chunk := mod (calculation, 11);
  --dbms_output.put_line('Calculation: ' || ico_chunk );
  calculation := mod ( (11-ico_chunk), 10 );
  ico_chunk := TO_NUMBER(SUBSTR(icochar,8,1));
  --dbms_output.put_line('This: ' || calculation || ' should be equal to: ' || ico_chunk);
  
  IF ( ico_chunk != calculation ) THEN
    RAISE_APPLICATION_ERROR(-20001, 'IČ nesouhlasí modulo 11');
  END IF;
  
END;
/

--------------------------------------------------------------------------------
---------------------------- Vkladanie do tabuliek -----------------------------
--------------------------------------------------------------------------------

-- Tabulka: zakaznik -> ID_Zakaznik (INTEGER), meno (VARCHAR(64))
INSERT INTO zakaznik (meno) VALUES ('Janko Hrasko');  -- 1
INSERT INTO zakaznik (meno) VALUES ('Anna Slana');  -- 2
INSERT INTO zakaznik (meno) VALUES ('Peter Vesely');  -- 3
INSERT INTO zakaznik (meno) VALUES ('Michal Dobrota');  -- 4
INSERT INTO zakaznik (meno) VALUES ('Martin Klingacik');  -- 5
INSERT INTO zakaznik (meno) VALUES ('Do Long Thanh');  -- 6
INSERT INTO zakaznik (meno) VALUES ('Andrej Hucko');  -- 7
INSERT INTO zakaznik (meno) VALUES ('Milan Augustin');  -- 8
INSERT INTO zakaznik (meno) VALUES ('Martina Grzybowska');  -- 9
INSERT INTO zakaznik (meno) VALUES ('Jakub Handzus');  -- 10
INSERT INTO zakaznik (meno) VALUES ('Marek Schauer');  -- 11
INSERT INTO zakaznik (meno) VALUES ('Daniel Florek');  -- 12
INSERT INTO zakaznik (meno) VALUES ('Martin Grnac');  -- 13

-- Tabulka: dodavatel -> ico(ID), nazov, ulica, mesto  
INSERT INTO dodavatel VALUES ('01526111', 'Kohinoor', 'Vlhka 7', 'Brno');
INSERT INTO dodavatel VALUES ('01525981', 'Maped', 'Kolejni 2', 'Brno');
INSERT INTO dodavatel VALUES ('01526235', 'Milanove pastelky', 'Námestie SNP 18', 'Banská Bystrica');
INSERT INTO dodavatel VALUES ('43219861', 'Andrejove skicare', 'Dukelská 9', 'Nové Mesto nad Váhom');
INSERT INTO dodavatel VALUES ('42424241', 'The Big Brother', 'Nowhere 42', 'Mystery City');
INSERT INTO dodavatel VALUES ('47539798', 'Faber-Castell', 'Hlavni 21', 'Praha');
INSERT INTO dodavatel VALUES ('77958438', 'Katkine Kresby', 'Kolejni 2', 'Brno');
INSERT INTO dodavatel VALUES ('45289824', 'Kgezby, Pastegky a Skicage', 'Kraloholska 42', 'Banska Bystrica');

-- Tabulka: produkt -> ID, cena, skladom, oblubenost, predajnost, pocet_v_baleni
    -- Tabulka: skicare -> ID_Produkt, gramaz, format_skicaru
INSERT INTO produkt VALUES (sq_produkt_s.NEXTVAL, 1.07, 400, NULL, 0, 15, 'SuperVykres');
    INSERT INTO skicare VALUES (sq_produkt_s.CURRVAL, 11, 'A4');
INSERT INTO produkt VALUES (sq_produkt_s.NEXTVAL, 0.05, 5000, 50, 200, 1, 'WindowsSkicar');
    INSERT INTO skicare VALUES (sq_produkt_s.CURRVAL, 11, 'A4');
INSERT INTO produkt VALUES (sq_produkt_s.NEXTVAL, 0.6, 500, 80, 30, 10, 'CasioSkicar');
    INSERT INTO skicare VALUES (sq_produkt_s.CURRVAL, 23, 'A3');
INSERT INTO produkt VALUES (sq_produkt_s.NEXTVAL, 0.7, 450, 45, 10, 25, 'JeToJedno');
  INSERT INTO skicare VALUES (sq_produkt_s.CURRVAL, 40, 'A2');
INSERT INTO produkt VALUES (sq_produkt_s.NEXTVAL, 1.2, 100, 40, 10, 5, 'Ehmmm');
    INSERT INTO skicare VALUES (sq_produkt_s.CURRVAL, 17, 'A5');

-- Tabulka: produkt -> ID, cena, skladom, oblubenost, predajnost, pocet_v_baleni    
    -- Tabulka: pastelky -> ID_Produkt, typ, dlzka
INSERT INTO produkt VALUES (sq_produkt_p.NEXTVAL, 0.25, 700, 100, 50, 1, 'Infinite');
    INSERT INTO pastelky VALUES (sq_produkt_p.CURRVAL, 'obycajna', 130);
INSERT INTO produkt VALUES (sq_produkt_p.NEXTVAL, 6.99, 200, NULL, 0, 6, 'Ceruzka42');
    INSERT INTO pastelky VALUES (sq_produkt_p.CURRVAL, 'progresso', 120);
INSERT INTO produkt VALUES (sq_produkt_p.NEXTVAL, 0.18, 100, 75, 20, 1, 'Finite');
    INSERT INTO pastelky VALUES (sq_produkt_p.CURRVAL, 'voskovka', 60);
INSERT INTO produkt VALUES (sq_produkt_p.NEXTVAL, 2.99, 50, 80, 60, 20, 'Clever');
    INSERT INTO pastelky VALUES (sq_produkt_p.CURRVAL, 'ceruzka', 150);
INSERT INTO produkt VALUES (sq_produkt_p.NEXTVAL, 4.49, 450, 50, 20, 15, 'Xiaomi');
    INSERT INTO pastelky VALUES (sq_produkt_p.CURRVAL, 'ceruzka', 90);    

-- Tabulka: je_dodany -> ID_Produkt, ico
INSERT INTO je_dodany VALUES (1, '01526111');
INSERT INTO je_dodany VALUES (1, '43219861');
INSERT INTO je_dodany VALUES (1, '77958438');
INSERT INTO je_dodany VALUES (2, '01525981');
INSERT INTO je_dodany VALUES (2, '77958438');
INSERT INTO je_dodany VALUES (2, '42424241');
INSERT INTO je_dodany VALUES (3, '01526235');
INSERT INTO je_dodany VALUES (3, '47539798');
INSERT INTO je_dodany VALUES (3, '01526235');
INSERT INTO je_dodany VALUES (4, '77958438');
INSERT INTO je_dodany VALUES (4, '01526235');
INSERT INTO je_dodany VALUES (5, '01526111');
INSERT INTO je_dodany VALUES (5, '43219861');
INSERT INTO je_dodany VALUES (6, '43219861');
INSERT INTO je_dodany VALUES (7, '01526111');
INSERT INTO je_dodany VALUES (8, '42424241');
INSERT INTO je_dodany VALUES (9, '01525981');
INSERT INTO je_dodany VALUES (10, '01526111');

-- Tabulka: recenzia -> ID, datum, sprava, hodnotenie, ID_produktu, ID_zakaznika
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '01.03.2012', 'ok', 75, 1, 1);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '30.09.2015', 'super', 83, 1, 2);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '19.01.2013', 'ok', 68, 1, 3);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '28.04.2010', 'nic moc', 42, 1, 5);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '14.02.2009', 'ok', 63, 1, 4);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '07.07.2015', 'nic moc', 30, 2, 7);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '23.08.2014', 'ok', 52, 2, 6);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '16.10.2010', 'da sa', 48, 2, 9);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '06.01.2011', 'cele zle', 4, 2, 11);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '07.03.2017', 'lorem ipsum', 28, 2, 8);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '10.12.2012', 'cele zle', 25, 3, 9);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '29.08.2011', 'super', 99, 3, 8);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '27.04.2015', 'da sa', 68, 3, 11);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '25.11.2009', 'v pohode', 84, 3, 7);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '11.02.2014', 'lorem ipsum', 69, 4, 10);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '12.10.2012', 'nic moc', 25, 4, 5);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '08.09.2011', 'super', 99, 4, 1);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '04.07.2015', NULL, 68, 5, 2);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '15.09.2016', 'ok', 55, 5, 6);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '23.10.2014', NULL, 35, 5, 8);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '09.12.2014', 'da sa', 45, 6, 7);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '23.08.2012', NULL, 15, 6, 10);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '02.10.2013', 'super', 61, 6, 9);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '23.02.2015', NULL, 75, 7, 12);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '23.10.2016', 'super', 69, 7, 13);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '02.10.2017', 'moze byt', 58, 7, 4);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '23.10.2016', 'super', 49, 8, 5);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '02.10.2017', 'ok', 64, 8, 1);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '15.04.2017', 'super', 76, 9, 9);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '07.05.2017', 'moze byt', 59, 9, 2);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '15.08.2016', 'ok', 85, 10, 12);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '08.09.2016', 'moze byt', 76, 10, 3);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '17.11.1989', 'Skvele pastelky!!!', 100, 2, 3);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '01.01.1970', 'Najlepsie voskovky', 75, 6, 4);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '09.04.2017', 'mysteriozne farby...', 42, 10, 5);
INSERT INTO recenzia VALUES (sq_recenzia.NEXTVAL, '12.03.2015', NULL, 20, 7, 4);

-- Tabulka: vlozenie_do_kosika -> ID_Produkt & ID_Zakaznik
INSERT INTO vlozenie_do_kosika VALUES (2, 5);
INSERT INTO vlozenie_do_kosika VALUES (6, 5);
INSERT INTO vlozenie_do_kosika VALUES (5, 5);
INSERT INTO vlozenie_do_kosika VALUES (5, 5);
INSERT INTO vlozenie_do_kosika VALUES (5, 5);
INSERT INTO vlozenie_do_kosika VALUES (7, 5);
INSERT INTO vlozenie_do_kosika VALUES (8, 5);
INSERT INTO vlozenie_do_kosika VALUES (2, 2);
INSERT INTO vlozenie_do_kosika VALUES (2, 2);
INSERT INTO vlozenie_do_kosika VALUES (2, 2);
INSERT INTO vlozenie_do_kosika VALUES (2, 2);
INSERT INTO vlozenie_do_kosika VALUES (1, 2);
INSERT INTO vlozenie_do_kosika VALUES (8, 2);
INSERT INTO vlozenie_do_kosika VALUES (8, 2);
INSERT INTO vlozenie_do_kosika VALUES (4, 1);
INSERT INTO vlozenie_do_kosika VALUES (4, 1);
INSERT INTO vlozenie_do_kosika VALUES (6, 1);
INSERT INTO vlozenie_do_kosika VALUES (10, 1);
INSERT INTO vlozenie_do_kosika VALUES (10, 1);
INSERT INTO vlozenie_do_kosika VALUES (7, 12);
INSERT INTO vlozenie_do_kosika VALUES (7, 12);
INSERT INTO vlozenie_do_kosika VALUES (2, 12);
INSERT INTO vlozenie_do_kosika VALUES (6, 12);
INSERT INTO vlozenie_do_kosika VALUES (6, 12);


-- Tabulka: objednavka -> ID, ulica, mesto, psc, sposob_dopravy, celkova_cena, ID_Zakaznik
INSERT INTO objednavka VALUES (sq_objednavka.NEXTVAL, NULL, NULL, NULL, 'na predajnu', 8.17, 5);
INSERT INTO objednavka VALUES (sq_objednavka.NEXTVAL, NULL, NULL, NULL, 'na predajnu', 12.99, 1);
INSERT INTO objednavka VALUES (sq_objednavka.NEXTVAL, 'Bozetechova 2', 'Brno', 60200, 'kurierom', 15, 2);
INSERT INTO objednavka VALUES (sq_objednavka.NEXTVAL, 'Leskova 3', 'Bratislava', 81104, 'kurierom', 19.49, 3);
INSERT INTO objednavka VALUES (sq_objednavka.NEXTVAL, NULL, NULL, NULL, 'na predajnu', 25.99, 4);

-- Tabulka: sa_nachadzal -> ID_Produkt & ID_Objednavka
INSERT INTO sa_nachadzal VALUES (2, 1);
INSERT INTO sa_nachadzal VALUES (6, 1);
INSERT INTO sa_nachadzal VALUES (5, 1);
INSERT INTO sa_nachadzal VALUES (2, 2);
INSERT INTO sa_nachadzal VALUES (4, 2);
INSERT INTO sa_nachadzal VALUES (7, 3);
INSERT INTO sa_nachadzal VALUES (5, 3);
INSERT INTO sa_nachadzal VALUES (6, 4);
INSERT INTO sa_nachadzal VALUES (1, 4);
INSERT INTO sa_nachadzal VALUES (2, 5);
INSERT INTO sa_nachadzal VALUES (9, 5);
INSERT INTO sa_nachadzal VALUES (10, 5);

--------------------------------------------------------------------------------
------------------------------------ SELECT ------------------------------------
--------------------------------------------------------------------------------

-- 1.)
-- Spojenie tabuliek: skicare, produkt
-- Filtracia skicarov formatu A4 zo vsetkych produktov, vypis gramaze a ceny
SELECT gramaz, cena 
FROM skicare NATURAL JOIN produkt
WHERE format_skicaru = 'A4';

-- 2.)
-- Spojenie tabuliek: zakaznik, recenzia
-- Vypis recenzovanych produktov (uverejnena recenzia s hodnotenim viac ako 50%)
SELECT id_produkt
FROM PRODUKT NATURAL JOIN RECENZIA
WHERE hodnotenie > 50;

-- 3.)
-- Spojenie tabuliek: produkt, sa_nachadzal, objednávka
-- Vypis vsetkych produktov (+ cena) nachadzajucich sa v danej objednavke (c. 2)
SELECT ID_PRODUKT, CENA
FROM PRODUKT NATURAL JOIN SA_NACHADZAL NATURAL JOIN OBJEDNAVKA
WHERE ID_OBJEDNAVKA = 2;

-- dva dotazy s klauzulí GROUP BY a agregační funkcí
-- 4.)
-- Pocet dodavatelov z kazdeho mesta
SELECT COUNT(ICO) AS POCET, MESTO 
FROM DODAVATEL
GROUP BY MESTO
ORDER BY POCET DESC;

-- 5.)
-- Vypis priemernych cien podla typov pasteliek
SELECT AVG(CENA) AS PRIEMER, TYP
FROM PASTELKY NATURAL JOIN PRODUKT
GROUP BY TYP
ORDER BY PRIEMER;

-- jeden dotaz obsahující predikát EXISTS
-- 6.)
-- Vypis vsetkych dodavatelov pasteliek
SELECT NAZOV
FROM DODAVATEL
WHERE EXISTS( 
  SELECT TYP
  FROM PASTELKY NATURAL JOIN PRODUKT NATURAL JOIN JE_DODANY
  WHERE ICO = DODAVATEL.ICO
);

-- jeden dotaz obsahujuci predikat IN (SELECT ...)
-- 7.)
-- Vypis objednavok z miest, kde sa nachadzaju zaroven aj dodavatelia
SELECT *
FROM OBJEDNAVKA
WHERE MESTO IN (
  SELECT MESTO
  FROM DODAVATEL
);

--------------------------------------------------------------------------------
--------------------------------- Procedura ------------------------------------
--------------------------------------------------------------------------------

CREATE OR REPLACE PROCEDURE hodnotenie_produktu(prah IN NUMBER)
AS 
  CURSOR produktPTR IS
    SELECT id_produkt, AVG(hodnotenie) as priemer
    FROM PRODUKT NATURAL JOIN RECENZIA
    GROUP BY ID_PRODUKT
    ORDER BY priemer DESC;
  line produktPTR%ROWTYPE;
  
BEGIN
  IF (prah > 100 OR prah < 0) THEN
    RAISE_APPLICATION_ERROR(-20005, 'Vstupna hodnota procedury musi byt v intervale <0,100>');
  END IF;
  dbms_output.put_line('Procedura na zobrazenie ID Produktov, ktore maju hodnotenie aspon '||prah||'%.'); 
  OPEN produktPTR;
   LOOP
     FETCH produktPTR into line;
     EXIT WHEN produktPTR%NOTFOUND;
     IF ( line.priemer >= prah ) THEN
       dbms_output.put_line( 'ID Produktu: [' || line.ID_PRODUKT || '], Hodnotenie: ' || line.priemer||'%');
     END IF;
   END LOOP;
  CLOSE produktPTR; 
END; 
/

EXECUTE hodnotenie_produktu(50);

-- -----------------------------------------------------------------------------

CREATE OR REPLACE PROCEDURE vypis_kosika(zak_in IN NUMBER)
AS
  whatever NUMBER;
  CURSOR pointer IS
    SELECT *
    FROM zakaznik NATURAL JOIN vlozenie_do_kosika NATURAL JOIN produkt
    WHERE ID_ZAKAZNIK = zak_in
    ORDER BY ID_ZAKAZNIK, ID_PRODUKT;
    
  line pointer%ROWTYPE;
  menozakaznika zakaznik.meno%TYPE;
  
  formatskicaru SKICARE.FORMAT_SKICARU%TYPE;
  gramazskicaru SKICARE.GRAMAZ%TYPE;
  
  typpastelky PASTELKY.TYP%TYPE;
  dlzkapastelky PASTELKY.DLZKA%TYPE;
  
  cartcnt NUMBER := 0;
  price NUMBER := 0;

BEGIN

  OPEN pointer;
    LOOP
      FETCH pointer into line;
      EXIT WHEN pointer%NOTFOUND;
      cartcnt := cartcnt +1;
      price := price + line.CENA;
      
      IF ( mod(line.ID_PRODUKT, 2) = 0 ) THEN
        SELECT TYP INTO typpastelky FROM produkt NATURAL JOIN pastelky WHERE ID_PRODUKT = line.ID_PRODUKT;
        SELECT DLZKA INTO dlzkapastelky FROM produkt NATURAL JOIN pastelky WHERE ID_PRODUKT = line.ID_PRODUKT;
        dbms_output.put( cartcnt || '. Pastelky: "'|| line.NAZOV || '", typ ' || typpastelky || ', dlzka ' || dlzkapastelky || 'mm, ' || line.POCET_V_BALENI || 'kusov ' );
      
        IF ( line.CENA < 1 ) THEN
          dbms_output.put_line ( '0'||line.cena||'€' );
        ELSE
          dbms_output.put_line ( line.cena||'€' );
        END IF; 
        -- dbms_output.put_line('Mame tu pastelku.');
      END IF;
      
      IF ( mod(line.ID_PRODUKT, 2) = 1 ) THEN
        SELECT FORMAT_SKICARU INTO formatskicaru FROM produkt NATURAL JOIN skicare WHERE ID_PRODUKT = line.ID_PRODUKT;
        SELECT GRAMAZ INTO gramazskicaru FROM produkt NATURAL JOIN skicare WHERE ID_PRODUKT = line.ID_PRODUKT;
        dbms_output.put( cartcnt || '. Skicar "' || line.NAZOV || '" formatu '|| formatskicaru || ', gramaz ' || gramazskicaru || 'g/list, ' || line.POCET_V_BALENI || 'kusov ' );
        
        IF ( line.CENA < 1 ) THEN
          dbms_output.put_line ( '0'||line.cena||'€' );
        ELSE
          dbms_output.put_line ( line.cena||'€' );
        END IF;  
        -- dbms_output.put_line('Mame tu skicar.');
      END IF;

    END LOOP;
  CLOSE pointer; 

  IF (cartcnt = 0) THEN
    RAISE_APPLICATION_ERROR(-20100, 'Uvedeny zakaznik ma prazdny nakupny kosik.');
  END IF;
  SELECT meno INTO menozakaznika FROM zakaznik WHERE ID_ZAKAZNIK = zak_in;
  dbms_output.put_line('');
  dbms_output.put_line('Zakaznik ' || menozakaznika || ' ma v kosiku '||cartcnt||' produktov v celkovej cene '|| price || ' eur.');
END;
/

EXECUTE vypis_kosika(2);

--------------------------------------------------------------------------------
------------------------ Explain plan a pouzitie indexu ------------------------
--------------------------------------------------------------------------------

DROP INDEX planIndex;

EXPLAIN PLAN FOR
SELECT nazov, AVG(hodnotenie) as priemerne_hodnotenie
FROM PRODUKT JOIN RECENZIA USING (ID_produkt)
WHERE EXTRACT(year from datum) = 2016
GROUP BY nazov
ORDER BY nazov DESC;
SELECT * FROM TABLE(DBMS_XPLAN.display);

CREATE INDEX planIndex ON PRODUKT (ID_Produkt, nazov);

EXPLAIN PLAN FOR
SELECT  /*+ INDEX(planIndex PRODUKT)*/ nazov, AVG(hodnotenie) as priemerne_hodnotenie
FROM PRODUKT JOIN RECENZIA USING (ID_produkt)
WHERE EXTRACT(year from datum) = 2016
GROUP BY nazov
ORDER BY nazov DESC;
SELECT * FROM TABLE(DBMS_XPLAN.display);

--------------------------------------------------------------------------------
-------------------------- Pridelenie prav XAUGUS09 ----------------------------
--------------------------------------------------------------------------------

GRANT ALL ON ZAKAZNIK TO xhucko01;
GRANT ALL ON DODAVATEL TO xhucko01;
GRANT ALL ON PRODUKT TO xhucko01;
GRANT ALL ON PASTELKY TO xhucko01;
GRANT ALL ON SKICARE TO xhucko01;
GRANT ALL ON RECENZIA TO xhucko01;
GRANT ALL ON OBJEDNAVKA TO xhucko01;
GRANT ALL ON JE_DODANY TO xhucko01;
GRANT ALL ON SA_NACHADZAL TO xhucko01;
GRANT ALL ON VLOZENIE_DO_KOSIKA TO xhucko01;

GRANT EXECUTE ON hodnotenie_produktu TO xhucko01;
GRANT EXECUTE ON vypis_kosika TO xhucko01;

--------------------------------------------------------------------------------
--------------------------- Materializovany pohlad -----------------------------
--------------------------------------------------------------------------------

DROP MATERIALIZED VIEW dodavatelMat;

CREATE MATERIALIZED VIEW dodavatelMat
REFRESH ON COMMIT AS
  SELECT COUNT(ICO) AS POCET, MESTO 
  FROM DODAVATEL
  GROUP BY MESTO;
  
GRANT ALL ON dodavatelMat TO xhucko01;


SELECT * FROM dodavatelMat;

INSERT INTO dodavatel VALUES ('30776236', 'Tree Materials', 'Abstraktna 4', 'Imaginary');
COMMIT;

SELECT * FROM dodavatelMat;
