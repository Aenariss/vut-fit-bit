/*
IDS project 2
@author: xbubel08, xfiala61
*/

/*
Tabulky:  Author
         Genre
         Character(_table)
         Manga
         Person
         User(_table)
         Episode
         Magazine
         Volume
         Order(_table)
Pomocne tabulky:
		Ordered_Volumes
		Ordered_Magazines
		Episode_in_Magazine
		Episode_in_Volume
		Manga_Genre
		Author_Writes
		Letter_From_Fans
Dalsi entity:
        Misa_order - MATERIALIZED VIEW
        primary_key_generators - SEQUENCE
*/

/* Nejprve smaz mozne predchozi tabulky
*/

DROP TABLE Author CASCADE CONSTRAINTS;
DROP TABLE Genre CASCADE CONSTRAINTS;
DROP TABLE Character_table CASCADE CONSTRAINTS;
DROP TABLE Manga CASCADE CONSTRAINTS;
DROP TABLE Person CASCADE CONSTRAINTS;
DROP TABLE User_table CASCADE CONSTRAINTS;
DROP TABLE Episode CASCADE CONSTRAINTS;
DROP TABLE Magazine CASCADE CONSTRAINTS;
DROP TABLE Volume CASCADE CONSTRAINTS;
DROP TABLE Order_table CASCADE CONSTRAINTS;
DROP TABLE Ordered_Volumes CASCADE CONSTRAINTS;
DROP TABLE Ordered_Magazines CASCADE CONSTRAINTS;
DROP TABLE Episode_in_Magazine CASCADE CONSTRAINTS;
DROP TABLE Episode_in_Volume CASCADE CONSTRAINTS;
DROP TABLE Manga_Genre CASCADE CONSTRAINTS;
DROP TABLE Author_Writes CASCADE CONSTRAINTS;
DROP TABLE Letter_From_Fans CASCADE CONSTRAINTS;

DROP SEQUENCE primary_key_generators;
DROP MATERIALIZED VIEW Misa_orders;

/* Vytvor tabulky */

CREATE TABLE Genre (
	id INT GENERATED ALWAYS AS IDENTITY NOT NULL PRIMARY KEY,
	name VARCHAR(100) NOT NULL
);

CREATE TABLE Author (
    id INT GENERATED ALWAYS AS IDENTITY NOT NULL PRIMARY KEY,
    age int NOT NULL,
    writes_genre int NOT NULL,
    CONSTRAINT author_writes_genre_foreign_key
		FOREIGN KEY (writes_genre) REFERENCES Genre (id)
);

CREATE TABLE User_table (
    id INT GENERATED ALWAYS AS IDENTITY NOT NULL PRIMARY KEY,
    email VARCHAR(100) NOT NULL CHECK(REGEXP_LIKE
    	(email, '^[a-zA-Z0-9_]+@[a-zA-Z0-9_]+\.[a-zA-Z]{2,}$'))
);

CREATE TABLE Person (
    id INT DEFAULT NULL PRIMARY KEY,
    first_name VARCHAR(100) NOT NULL,
    last_name VARCHAR(100) NOT NULL,
    city VARCHAR(100) NOT NULL,
    street VARCHAR(100) NOT NULL,
    house_number VARCHAR(100) NOT NULL,
    author_id int DEFAULT NULL,
    user_id int DEFAULT NULL,
    CONSTRAINT author_id_const
		FOREIGN KEY (author_id) REFERENCES Author (id) ON DELETE CASCADE,
	CONSTRAINT user_id_const
		FOREIGN KEY (user_id) REFERENCES User_table (id) ON DELETE CASCADE
);

CREATE TABLE Manga (
	id INT GENERATED ALWAYS AS IDENTITY NOT NULL PRIMARY KEY,
	name VARCHAR(100) NOT NULL,
	status VARCHAR(100) NOT NULL,
	date_released DATE NOT NULL,
	last_update DATE NOT NULL,
	publisher VARCHAR(100) NOT NULL
);

CREATE TABLE Episode (
	id INT GENERATED ALWAYS AS IDENTITY NOT NULL PRIMARY KEY,
	name VARCHAR(100) NOT NULL,
	episode_number int NOT NULL,
	wrapping VARCHAR(100) NOT NULL,
	pages int NOT NULL,
	in_manga int NOT NULL,
	CONSTRAINT in_manga_foreign_key
		FOREIGN KEY (in_manga) REFERENCES Manga (id)
);


CREATE TABLE Character_table (
	id INT GENERATED ALWAYS AS IDENTITY NOT NULL PRIMARY KEY,
	name VARCHAR(100) NOT NULL,
	age VARCHAR(100) NOT NULL,
	hair_color VARCHAR(100) NOT NULL,
	power_level int DEFAULT NULL,
	first_appearence int NOT NULL,
	last_appearence int NOT NULL,
	CONSTRAINT first_ap_foreign_key
		FOREIGN KEY (first_appearence) REFERENCES Episode (id) ON DELETE SET NULL,
	CONSTRAINT last_ap_foreign_key
		FOREIGN KEY (last_appearence) REFERENCES Episode (id) ON DELETE SET NULL
);

CREATE TABLE Magazine (
	id INT GENERATED ALWAYS AS IDENTITY NOT NULL PRIMARY KEY,
	name VARCHAR(100) NOT NULL,
	release_number int NOT NULL
);

CREATE TABLE Volume (
	id INT GENERATED ALWAYS AS IDENTITY NOT NULL PRIMARY KEY,
	author_note VARCHAR(1000) DEFAULT NULL,
	release_number int NOT NULL
);

CREATE TABLE Order_table (
	id INT GENERATED ALWAYS AS IDENTITY NOT NULL PRIMARY KEY,
	made_by int NOT NULL,
	date_of_order DATE NOT NULL,
	status VARCHAR(100) NOT NULL,
	CONSTRAINT order_const	-- uzivatel vytvoril
		FOREIGN KEY (made_by) REFERENCES User_table (id)
);

CREATE TABLE Ordered_Volumes (	-- svazku objednano
	order_table_id INT NOT NULL,
	volume_id INT NOT NULL,
	CONSTRAINT Ordered_volumes_primary_key
		PRIMARY KEY (order_table_id, volume_id),
	CONSTRAINT order_table_volume_foreign_key
		FOREIGN KEY (order_table_id) REFERENCES Order_table (id),
	CONSTRAINT volume_id_order_foreign_key
		FOREIGN KEY (volume_id) REFERENCES Volume (id)
);

CREATE TABLE Ordered_Magazines (	-- Magazinu objednano
	order_table_id INT NOT NULL,
	magazine_id INT NOT NULL,
	CONSTRAINT Ordered_Magazines_primary_key
		PRIMARY KEY (order_table_id, magazine_id),
	CONSTRAINT order_table_mag_foreign_key
		FOREIGN KEY (order_table_id) REFERENCES Order_table (id),
	CONSTRAINT magazine_id_ordered_foreign_key
		FOREIGN KEY (magazine_id) REFERENCES Magazine (id)
);

CREATE TABLE Episode_in_Magazine (	-- Episoda vychazi v Magazinu
	episode_id INT NOT NULL,
	magazine_id INT NOT NULL,
	CONSTRAINT Episode_in_Magazine_primary_key
		PRIMARY KEY (episode_id, magazine_id),
	CONSTRAINT ep_in_mag_episde_foregin_key
		FOREIGN KEY (episode_id) REFERENCES Episode (id),
	CONSTRAINT ep_in_mag_magazine_id_foreign_key
		FOREIGN KEY (magazine_id) REFERENCES Magazine (id)
);

CREATE TABLE Episode_in_Volume (	-- Episoda je soucasti svazku
	episode_id INT NOT NULL,
	volume_id INT NOT NULL,
	CONSTRAINT Episode_in_Volume_primary_key
		PRIMARY KEY (episode_id, volume_id),
	CONSTRAINT ep_in_vol_episde_foregin_key
		FOREIGN KEY (episode_id) REFERENCES Episode (id),
	CONSTRAINT ep_in_vol_volume_foregin_key
		FOREIGN KEY (volume_id) REFERENCES Volume (id)
);

CREATE TABLE Manga_Genre (	-- Manga je zanru
	genre_id INT NOT NULL,
	manga_id INT NOT NULL,
	CONSTRAINT Manga_Genre_primary_key
		PRIMARY KEY (genre_id, manga_id),
	CONSTRAINT genre_in_manga_foreign_key
		FOREIGN KEY (genre_id) REFERENCES Genre (id),
	CONSTRAINT manga_in_genre_foreign_key
		FOREIGN KEY (manga_id) REFERENCES Manga (id)
);

CREATE TABLE Author_Writes (    -- Autor pise/kresli
	author_id INT NOT NULL,
	manga_id INT NOT NULL,
	draws NUMBER(1) NOT NULL,   -- Zaznac, ze autor muze kreslit
	writes NUMBER(1) NOT NULL,  -- Zaznac, ze autor muze psat
	CONSTRAINT Manga_Author_primary_key
		PRIMARY KEY (author_id, manga_id),
	CONSTRAINT author_writes_manga_foreign_key
		FOREIGN KEY (author_id) REFERENCES Author (id),
	CONSTRAINT manga_written_by_Author_foreign_key
		FOREIGN KEY (manga_id) REFERENCES Manga (id)
);

CREATE TABLE Letter_From_Fans (     -- Specialni tabulka na dopisy od fanousku ve svazku
	id INT GENERATED ALWAYS AS IDENTITY NOT NULL PRIMARY KEY,
	volume_id INT NOT NULL,
	letter VARCHAR(280) NOT NULL,
	CONSTRAINT volume_id_letter
		FOREIGN KEY (volume_id) REFERENCES Volume (id)
);

/* Vytvoreni sekvence pro generovani klicu */
CREATE SEQUENCE primary_key_generators
START WITH 1
INCREMENT BY 1;

/* Trigger pro nastaveni primarniho klice ze sekvence */
CREATE OR REPLACE TRIGGER insert_primary_key
BEFORE INSERT ON Person FOR EACH ROW
BEGIN
	IF :NEW.id IS NULL THEN
	    :NEW.id := primary_key_generators.nextval;
   	END IF;
END;

/* Trigger pro kontrolu, jestli je pri vlozeni nove mangy do databaze jeji status validni */
CREATE OR REPLACE TRIGGER manga_status
BEFORE INSERT ON Manga FOR EACH ROW
BEGIN
	IF :NEW.status NOT IN ('FINISHED', 'HIATUS', 'ONGOING')	THEN -- zkontroluj
		RAISE_APPLICATION_ERROR(-20500, 'Nevalidni status mangy!');
	END IF;
END;

/* Vloz ukazkova data */

/* Vloz zanry */

INSERT INTO Genre (name)
VALUES('Shonen');

INSERT INTO Genre (name)
VALUES('Horror');

INSERT INTO Genre (name)
VALUES('Dark Fantasy');

INSERT INTO Genre (name)
VALUES('Fantasy');

INSERT INTO Genre (name)
VALUES('Thriller');

/* Vloz autory */
INSERT INTO Author (age, writes_genre)
VALUES(50, 1);

INSERT INTO Person (first_name, last_name, city, street, house_number, author_id)
VALUES('Masashi', 'Kishimoto', 'Tokyo', 'Sakura Street', '245', 1);

INSERT INTO Author (age, writes_genre)
VALUES(50, 2);

INSERT INTO Person (first_name, last_name, city, street, house_number, author_id)
VALUES('Junji', 'Ito', 'Kyoto', 'Uzumaki', '564', 2);

INSERT INTO Author (age, writes_genre)
VALUES(54, 3);

INSERT INTO Person (first_name, last_name, city, street, house_number, author_id)
VALUES('Kentaro', 'Miura', 'Kyoto', 'Schrieke', '666', 3);

INSERT INTO Author (age, writes_genre)
VALUES(40, 4);

INSERT INTO Person (first_name, last_name, city, street, house_number, author_id)
VALUES('Eiichiro', 'Oda', 'Tokyo', 'Sanji Street', '69', 4);

INSERT INTO Author (age, writes_genre)
VALUES(34, 5);

INSERT INTO Person (first_name, last_name, city, street, house_number, author_id)
VALUES('Hajime', 'Isayama', 'Tokyo', 'Ackerman Street', '420', 5);

/* Vloz uzivatele */

INSERT INTO User_table (email)
VALUES('joseph@joestar.cz');

INSERT INTO Person (first_name, last_name, city, street, house_number, user_id)
VALUES('Joseph', 'Joestar', 'New York', 'Muscle Street', '2589', 1);

INSERT INTO User_table (email)
VALUES('Misa@Misa.cz');

INSERT INTO Person (first_name, last_name, city, street, house_number, user_id)
VALUES('Misa', 'Misa', 'New York', 'Muscle Street', '2589', 1);

/* Vloz mangy */
/* Ukazka triggeru, kdy bude status validni */
INSERT INTO Manga (name, status, date_released, last_update, publisher)
VALUES('Naruto', 'FINISHED', TO_DATE('1999-09-21', 'yyyy/mm/dd'), TO_DATE('2014-11-10', 'yyyy/mm/dd'), 'Shueisha');

INSERT INTO Manga (name, status, date_released, last_update, publisher)
VALUES('Berserk', 'ONGOING', TO_DATE('1990-11-26', 'yyyy/mm/dd'), TO_DATE('2021-1-22', 'yyyy/mm/dd'), 'Hakusensha');

INSERT INTO Manga (name, status, date_released, last_update, publisher)
VALUES('One piece', 'ONGOING', TO_DATE('1997-07-19', 'yyyy/mm/dd'), TO_DATE('2021-4-21', 'yyyy/mm/dd'), 'Shueisha');

INSERT INTO Manga (name, status, date_released, last_update, publisher)
VALUES('Attack on titan', 'ONGOING', TO_DATE('2009-09-09', 'yyyy/mm/dd'), TO_DATE('2021-4-09', 'yyyy/mm/dd'), 'Kodansha');


/* Vloz episody */

INSERT INTO Episode (name, episode_number, wrapping, pages, in_manga)
VALUES('Naruto Uzumaki', 1, 'BLACK', 42, 1);

INSERT INTO Episode (name, episode_number, wrapping, pages, in_manga)
VALUES('The worst client Saiaku no irainin', 2, 'BLACK', 43, 1);

INSERT INTO Episode (name, episode_number, wrapping, pages, in_manga)
VALUES('Black Swordsman', 3, 'BLACK', 100, 2);

/* Vloz charaktery */

INSERT INTO Character_table (name, age, hair_color, power_level, first_appearence, last_appearence)
VALUES('Naruto', 13, 'BLOND', 30, 1, 2);

INSERT INTO Character_table (name, age, hair_color, power_level, first_appearence, last_appearence)
VALUES('Guts', 15, 'BLACK', 50, 3, 3);

INSERT INTO Character_table (name, age, hair_color, power_level, first_appearence, last_appearence)
VALUES('Tsunade', 33, 'BLOND', 50, 1, 2);

/* Vloz magaziny */

INSERT INTO Magazine (name, release_number)
VALUES('Shonen Jump', 59);

INSERT INTO Magazine (name, release_number)
VALUES('Young Animal', 40);

/* Vloz svazky */

INSERT INTO Volume (author_note, release_number)
VALUES('Very good volume please read', 49);

INSERT INTO Volume (author_note, release_number)
VALUES('Haha i created masterpiece', 30);

/* Vloz objednavky */

INSERT INTO Order_Table (made_by, date_of_order, status)
VALUES(1, TO_DATE('2020-08-14', 'yyyy/mm/dd'), 'INPROCESS');

INSERT INTO Order_Table (made_by, date_of_order, status)
VALUES(1, TO_DATE('2020-08-15', 'yyyy/mm/dd'), 'INPROCESS');

INSERT INTO Order_Table (made_by, date_of_order, status)
VALUES(1, TO_DATE('2020-09-25', 'yyyy/mm/dd'), 'FINISHED');

INSERT INTO Order_Table (made_by, date_of_order, status)
VALUES(2, TO_DATE('2020-08-14', 'yyyy/mm/dd'), 'INPROCESS');

INSERT INTO Order_Table (made_by, date_of_order, status)
VALUES(2, TO_DATE('2020-08-15', 'yyyy/mm/dd'), 'INPROCESS');

INSERT INTO Order_Table (made_by, date_of_order, status)
VALUES(2, TO_DATE('2020-09-25', 'yyyy/mm/dd'), 'FINISHED');


/* Vloz objednane svazky */

INSERT INTO Ordered_Volumes (order_table_id, volume_id)
VALUES(1, 1);

INSERT INTO Ordered_Volumes (order_table_id, volume_id)
VALUES(2, 2);

/* Vloz objednane magaziny */

INSERT INTO Ordered_Magazines (order_table_id, magazine_id)
VALUES(1, 1);

INSERT INTO Ordered_Magazines (order_table_id, magazine_id)
VALUES(2, 2);

/* Vloz epizodu do magazinu */

INSERT INTO Episode_in_Magazine (episode_id, magazine_id)
VALUES(1, 1);

INSERT INTO Episode_in_Magazine (episode_id, magazine_id)
VALUES(2, 1);

INSERT INTO Episode_in_Magazine (episode_id, magazine_id)
VALUES(3, 2);

/* Vloz episodu do svazku */

INSERT INTO Episode_in_Volume (episode_id, volume_id)
VALUES(1, 1);

INSERT INTO Episode_in_Volume (episode_id, volume_id)
VALUES(3, 2);

/* Vloz zanr do mangy */

INSERT INTO Manga_Genre (genre_id, manga_id)
VALUES(1, 1);

INSERT INTO Manga_Genre (genre_id, manga_id)
VALUES(2, 2);

/* Vloz udaje jestli autor pise a/nebo kresli */

INSERT INTO Author_Writes (author_id, manga_id, draws, writes)
VALUES(1, 1, 0, 1);

INSERT INTO Author_Writes (author_id, manga_id, draws, writes)
VALUES(2, 1, 1, 1);

INSERT INTO Author_Writes (author_id, manga_id, draws, writes)
VALUES(3, 2, 1, 1);

/* Vloz dopisy od fanousku */

INSERT INTO Letter_From_Fans (volume_id, letter)
VALUES(1, 'P-Please author the limit is too short, it cant possibly fit a message this big');

INSERT INTO Letter_From_Fans (volume_id, letter)
VALUES(2, 'I love your manga senpaii');

/* Ukazka triggeru primary_key_generators generujiciho primarni klic, v nasem pripade id*/
SELECT * FROM PERSON;

/* Ukazka triggeru manga_status, kdy bude status validni c.2 */
INSERT INTO Manga (name, status, date_released, last_update, publisher)
VALUES('Test_Manga', 'FINISHED', TO_DATE('2020-04-21', 'yyyy/mm/dd'), TO_DATE('2020-04-22', 'yyyy/mm/dd'), 'Test_Publisher');

SELECT * FROM MANGA;

/* Ukazka, kdy trigger manga_status zahlasi chybu kvuli NEplatnemu statusu mangy */
INSERT INTO Manga (name, status, date_released, last_update, publisher)
VALUES('Test_Manga2', 'INVALID', TO_DATE('2021-04-22', 'yyyy/mm/dd'), TO_DATE('2021-04-23', 'yyyy/mm/dd'), 'Test_Publisher2');

/* Procedura, ktera zjisti celkovy pocet epizod v zadane manze */
CREATE OR REPLACE PROCEDURE episode_in_manga_count(manga_name IN VARCHAR)
AS
    episode_total NUMBER;   -- celkovy pocet episod v manze
    searched_id Manga.id%TYPE;   -- id mangy, ktere ziskam z nazvu, pouziti %TYPE
    CURSOR ep_manga_ids IS SELECT Episode.in_manga from Episode;    -- Pouziti kurzoru
    ep_manga_id NUMBER;

BEGIN
    episode_total := 0;

    SELECT Manga.id INTO episode_in_manga_count.searched_id from Manga where name = manga_name; -- Najdi ID na hledani

    open ep_manga_ids;
    LOOP
        FETCH ep_manga_ids into ep_manga_id;
        EXIT WHEN ep_manga_ids%NOTFOUND;  -- Jakmile se prosli vsechny, konec

        IF ep_manga_id = searched_id THEN
            episode_total := episode_total + 1;
        END IF;
    END LOOP;

    CLOSE ep_manga_ids;

    IF episode_total = 1 THEN   -- Kontrola pro spravny vypis z lexikalniho hlediska
       dbms_output.put_line('Manga ' || episode_in_manga_count.manga_name || ' ma ' || episode_total || ' epizodu.');
    ELSIF episode_total > 1 AND episode_total < 5 THEN
       dbms_output.put_line('Manga ' || episode_in_manga_count.manga_name || ' ma ' || episode_total || ' epizody.');
    ELSE
        dbms_output.put_line('Manga ' || episode_in_manga_count.manga_name || ' ma ' || episode_total || ' epizod.');
    END IF;

    EXCEPTION WHEN NO_DATA_FOUND THEN   -- Osetreni vyjimek
        dbms_output.put_line('Chyba! ' || manga_name || ' neni v databazi!');

END;

/* procedura vypise, zda autor kresli a/nebo pise svoji mangu, pokud neni zadane autorovo prijmeni vypise se error */
CREATE OR REPLACE PROCEDURE author_write_draw (author_last_name varchar)
AS
    bool_writes NUMBER;
    bool_draws NUMBER;
    author_exist NUMBER;
BEGIN

    SELECT Person.author_id INTO author_exist FROM Person
    WHERE Person.last_name = author_last_name;

    IF author_exist is not NULL THEN
        SELECT Author_Writes.writes INTO bool_writes FROM Person
        INNER JOIN Author_Writes ON Author_Writes.author_id = Person.author_id
        WHERE Person.last_name = author_last_name;

        SELECT Author_Writes.draws INTO bool_draws FROM Person
        INNER JOIN Author_Writes ON Author_Writes.author_id = Person.author_id
        WHERE Person.last_name = author_last_name;

        IF bool_writes = 1 AND bool_draws = 0 THEN
            DBMS_OUTPUT.PUT_LINE('Autor ' || author_last_name || ' píše svoji mangu');
        ELSIF bool_draws = 1 AND bool_writes = 0 THEN
            DBMS_OUTPUT.PUT_LINE('Autor ' || author_last_name || ' kreslí svoji mangu');
        ELSE
            DBMS_OUTPUT.PUT_LINE('Autor ' || author_last_name || ' kreslí a píše svoji mangu');
        END IF;
    ELSE
         DBMS_OUTPUT.PUT_LINE(author_last_name || ' není autorem mangy');
    END IF;
END;

BEGIN -- Ukazka funkcionality procedury na pocitani epizod v manze (Vypis na dbms_output)
   episode_in_manga_count('Naruto');
END;

BEGIN -- Ukazka procedury na vypis zda autor kresli a/nebo pise - autor kresli i pise
    author_write_draw('Miura');
END;

/* Neoptimalizovany plan bez pouziti indexu*/
EXPLAIN PLAN FOR
    SELECT COUNT(Manga.name), Person.last_name FROM Author
    INNER JOIN Person ON Person.author_id = Author.id
    INNER JOIN Author_Writes ON Author_Writes.author_id = Author.id
    INNER JOIN Manga ON Manga.id = Author_Writes.manga_id
    WHERE Manga.status = 'FINISHED'
    GROUP BY Person.last_name;

/* Neoptimalizovany vypis planu bez pouziti indexu*/
SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY());

/* Vytvoreni indexu nad sloupcem status tabulky manga*/
CREATE INDEX index_1 ON Manga(status);

/* Optimalizovany plan s pouzitim indexu*/
EXPLAIN PLAN FOR
    SELECT COUNT(Manga.name), Person.last_name FROM Author
    INNER JOIN Person ON Person.author_id = Author.id
    INNER JOIN Author_Writes ON Author_Writes.author_id = Author.id
    INNER JOIN Manga ON Manga.id = Author_Writes.manga_id
    WHERE Manga.status = 'FINISHED'
    GROUP BY Person.last_name;

/* Optimalizovany vypis planu s pouzitim indexu*/
SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY());

/* Prideleni prav druhemu clenu tymu XFIALA61*/
GRANT ALL ON Author TO XFIALA61;
GRANT ALL ON Genre TO XFIALA61;
GRANT ALL ON Character_table TO XFIALA61;
GRANT ALL ON Manga TO XFIALA61;
GRANT ALL ON Person TO XFIALA61;
GRANT ALL ON User_table TO XFIALA61;
GRANT ALL ON Episode TO XFIALA61;
GRANT ALL ON Magazine TO XFIALA61;
GRANT ALL ON Volume TO XFIALA61;
GRANT ALL ON Order_table TO XFIALA61;
GRANT ALL ON Ordered_Volumes TO XFIALA61;
GRANT ALL ON Ordered_Magazines TO XFIALA61;
GRANT ALL ON Episode_in_Magazine TO XFIALA61;
GRANT ALL ON Episode_in_Volume TO XFIALA61;
GRANT ALL ON Manga_Genre TO XFIALA61;
GRANT ALL ON Author_Writes TO XFIALA61;
GRANT ALL ON Letter_From_Fans TO XFIALA61;

GRANT EXECUTE ON author_write_draw TO XFIALA61;
GRANT EXECUTE ON episode_in_manga_count TO XFIALA61;


/* Materializovany pohled ukazujici pocet objednavek a jejich status od uzivatele s emailem Misa@Misa.cz*/
CREATE MATERIALIZED VIEW Misa_orders AS
SELECT XBUBEL08.Order_table.id, XBUBEL08.Order_table.status FROM XBUBEL08.User_table
INNER JOIN XBUBEL08.Order_table ON XBUBEL08.Order_table.made_by = XBUBEL08.User_table.id
WHERE XBUBEL08.User_table.email = 'Misa@Misa.cz';

GRANT ALL ON Misa_orders TO XFIALA61;

/* Vypis materializovany pohled*/
Select * from MISA_ORDERS;

/* Aktualizace materializovany pohled */
INSERT INTO XBUBEL08.Order_Table (made_by, date_of_order, status)
VALUES(2, TO_DATE('2020-09-29', 'yyyy/mm/dd'), 'INPROCESS');

/* Vypis materializovany pohled po aktualizaci originalni tabulky -- Nic se nezmeni */
Select * from MISA_ORDERS;

/* Normalni vypis dane tabulky, ktera bude, narozdil od materializovany pohled, aktualizovana */
SELECT XBUBEL08.Order_table.id, XBUBEL08.Order_table.status FROM XBUBEL08.User_table
INNER JOIN XBUBEL08.Order_table ON XBUBEL08.Order_table.made_by = XBUBEL08.User_table.id
WHERE XBUBEL08.User_table.email = 'Misa@Misa.cz';