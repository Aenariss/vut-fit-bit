/*
IDS project 2
@author: xbubel08, xfiala61
*/

/*
Entity:  Author
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
    id INT GENERATED ALWAYS AS IDENTITY NOT NULL PRIMARY KEY,
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
	status VARCHAR(100) NOT NULL
		CHECK(status IN ('FINISHED', 'HIATUS', 'ONGOING')),
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
		CHECK(status IN ('FINISHED', 'CANCELED', 'INPROCESS')),
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


/* Vloz ukazkova data */

/* Vloz zanry */

INSERT INTO Genre (name)
VALUES('Shonen');

INSERT INTO Genre (name)
VALUES('Horror');

INSERT INTO Genre (name)
VALUES('Dark Fantasy');

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

/* Vloz uzivatele */

INSERT INTO User_table (email)
VALUES('pepa@pepikov.cz');

INSERT INTO Person (first_name, last_name, city, street, house_number, user_id)
VALUES('Joseph', 'Joestar', 'New York', 'Muscle Street', '2589', 1);

/* Vloz mangy */

INSERT INTO Manga (name, status, date_released, last_update, publisher)
VALUES('Naruto', 'FINISHED', TO_DATE('1999-09-21', 'yyyy/mm/dd'), TO_DATE('2014-11-10', 'yyyy/mm/dd'), 'Shueisha');

INSERT INTO Manga (name, status, date_released, last_update, publisher)
VALUES('Berserk', 'ONGOING', TO_DATE('1990-11-26', 'yyyy/mm/dd'), TO_DATE('2021-1-22', 'yyyy/mm/dd'), 'Hakusensha');

/* Vloz episody */

INSERT INTO Episode (name, episode_number, wrapping, pages, in_manga)
VALUES('First', 1, 'BLACK', 42, 1);

INSERT INTO Episode (name, episode_number, wrapping, pages, in_manga)
VALUES('Second', 2, 'BLACK', 43, 1);

INSERT INTO Episode (name, episode_number, wrapping, pages, in_manga)
VALUES('Black Swordsman', 3, 'BLACK', 100, 2);

/* Vloz charaktery */

INSERT INTO Character_table (name, age, hair_color, power_level, first_appearence, last_appearence)
VALUES('Naruto', 13, 'BLOND', 30, 1, 2);

INSERT INTO Character_table (name, age, hair_color, power_level, first_appearence, last_appearence)
VALUES('Guts', 15, 'BLACK', 50, 3, 3);

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
VALUES(2, 'Can you please finish berserk before you die');
