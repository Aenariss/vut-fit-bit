-- Vypnuti kontroly cizich klicu pro mazani tabulky
SET FOREIGN_KEY_CHECKS=0;

DROP TABLE IF EXISTS User;
DROP TABLE IF EXISTS response;
DROP TABLE IF EXISTS role;
DROP TABLE IF EXISTS upvote;
DROP TABLE IF EXISTS userroles;
DROP TABLE IF EXISTS visited;
DROP TABLE IF EXISTS years;
DROP TABLE IF EXISTS course_registration;
DROP TABLE IF EXISTS question;
DROP TABLE IF EXISTS reply;
DROP TABLE IF EXISTS course;
DROP TABLE IF EXISTS category;

CREATE TABLE IF NOT EXISTS User (
    id INTEGER NOT NULL UNIQUE AUTO_INCREMENT,
    surname VARCHAR(255),
    name VARCHAR(255),
    password VARCHAR(255),
    username VARCHAR(255),
    email VARCHAR(255),
    roles VARCHAR(255),
    date DATETIME,
    active BOOLEAN,
    karma INT,
    PRIMARY KEY(id),
    UNIQUE(username),
    UNIQUE(email)
);

CREATE TABLE IF NOT EXISTS visited (
    page VARCHAR(255),
    user INT,
    visited DATETIME,
    FOREIGN KEY(user) REFERENCES user(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS Years (
    year VARCHAR(4) PRIMARY KEY
);

CREATE TABLE IF NOT EXISTS Course (
    shortcut VARCHAR(10) NOT NULL UNIQUE,
	name VARCHAR(100),
	grade INTEGER,
	author INTEGER,
	approved BOOLEAN,
	date DATETIME,
	FOREIGN KEY(author) REFERENCES User(id) ON DELETE SET NULL,
	PRIMARY KEY(shortcut)
);

CREATE TABLE IF NOT EXISTS Upvote (
	user	INT,
	reply	INT,
	value	INT,
	question INT,
	PRIMARY KEY(user, reply)
);

CREATE TABLE IF NOT EXISTS role (
    id INT NOT NULL UNIQUE,
    name VARCHAR(80),
    description VARCHAR(250),
    PRIMARY KEY (id),
    UNIQUE (name)
);

CREATE TABLE IF NOT EXISTS Question (
    id INT not null unique AUTO_INCREMENT,
    title VARCHAR(100),
    body VARCHAR(600),
    solved BOOLEAN,
    author INT,
    category VARCHAR(100),
    course VARCHAR(10),
    year VARCHAR(4),
    date DATETIME,
    lastUpdated DATETIME,
    foreign key(author) references User(id) ON DELETE SET NULL,
    foreign key(course) references Course(shortcut) ON DELETE CASCADE,
    foreign key(year) references Years(year) ON DELETE CASCADE,
    primary key(id)
);

CREATE TABLE IF NOT EXISTS Reply (
	id	INTEGER NOT NULL UNIQUE AUTO_INCREMENT,
	author INT,
	question INT,
	upvote INT,
	solution BOOLEAN,
	message	VARCHAR(600),
	date DATETIME,
	correct boolean,
	FOREIGN KEY(author) REFERENCES User(id) ON DELETE SET NULL,
	PRIMARY KEY(id),
	FOREIGN KEY(question) REFERENCES Question(id) ON DELETE CASCADE
);


CREATE TABLE IF NOT EXISTS Response (
    id INT NOT NULL UNIQUE AUTO_INCREMENT,
    author INT,
    message VARCHAR(300),
    reply INT,
    date DATETIME,
    question INTEGER,
    FOREIGN KEY(reply) REFERENCES Reply(id) ON DELETE CASCADE,
    FOREIGN KEY(author) REFERENCES user(id) ON DELETE SET NULL,
    PRIMARY KEY(id),
    FOREIGN KEY(question) REFERENCES Question(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS Course_registration (
    id INT NOT NULL UNIQUE AUTO_INCREMENT,
    course VARCHAR(10),
    user INT,
    approved BOOLEAN,
    FOREIGN KEY(course) REFERENCES Course(shortcut) ON DELETE CASCADE,
    FOREIGN KEY(user) REFERENCES User(id) ON DELETE CASCADE,
    PRIMARY KEY(id)
);

CREATE TABLE IF NOT EXISTS Category (
    id INT NOT NULL UNIQUE AUTO_INCREMENT,
    name VARCHAR(100),
    course VARCHAR(10),
    FOREIGN KEY(course) REFERENCES course(shortcut) ON DELETE CASCADE,
    PRIMARY KEY(id)
);

CREATE TABLE IF NOT EXISTS UserRoles (
    user_id INT,
    role_id INT,
    FOREIGN KEY(user_id) REFERENCES user(id) ON DELETE CASCADE,
    FOREIGN KEY(role_id) REFERENCES role(id) ON DELETE CASCADE
);


ALTER TABLE User CONVERT TO CHARACTER SET utf8mb4  COLLATE utf8mb4_unicode_ci;
ALTER TABLE response CONVERT TO CHARACTER SET utf8mb4  COLLATE utf8mb4_unicode_ci;
ALTER TABLE role CONVERT TO CHARACTER SET utf8mb4  COLLATE utf8mb4_unicode_ci;
ALTER TABLE upvote CONVERT TO CHARACTER SET utf8mb4  COLLATE utf8mb4_unicode_ci;
ALTER TABLE userroles CONVERT TO CHARACTER SET utf8mb4  COLLATE utf8mb4_unicode_ci;
ALTER TABLE visited CONVERT TO CHARACTER SET utf8mb4  COLLATE utf8mb4_unicode_ci;
ALTER TABLE years CONVERT TO CHARACTER SET utf8mb4  COLLATE utf8mb4_unicode_ci;
ALTER TABLE course_registration CONVERT TO CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
ALTER TABLE question CONVERT TO CHARACTER SET utf8mb4  COLLATE utf8mb4_unicode_ci;
ALTER TABLE reply CONVERT TO CHARACTER SET utf8mb4  COLLATE utf8mb4_unicode_ci;
ALTER TABLE course CONVERT TO CHARACTER SET utf8mb4  COLLATE utf8mb4_unicode_ci;
ALTER TABLE category CONVERT TO CHARACTER SET utf8mb4  COLLATE utf8mb4_unicode_ci;

SET FOREIGN_KEY_CHECKS=1;

-- Aktualni roky
INSERT INTO Years values ('2022');

-- Role
INSERT INTO Role values (1, 'admin', 'Nejvyssi role');
INSERT INTO Role values (2, 'mod', 'Spravce kurzu');

-- Uzivatele pro testovani
INSERT INTO User Values (1, 'Adminovič', 'Admin', '$2b$12$bLRnpUdX5Jx9AnAFFjDyk.7/6dI90BIya/e5m6cs7BwtG.eaV/FPG', 'AdminAccount', 'admin@admin.com', NULL, now(), TRUE, 20);
INSERT INTO User Values (2, 'Moderátorovič', 'Moderátor', '$2b$12$Q4zPF5rhPskTdxkI4QWpn.EnMDzoxB1R5NFJYVmPatS2MXSQ6Htl2', 'ModAccount', 'mod@mod.com', NULL, now(), TRUE, 10);
INSERT INTO User Values (3, 'Teacher', 'Učitel', '$2b$12$rtsFSnk299tZSlZNR2cbROqMcGEZnqouyg.5kES6BRQ50w.RtYUSK', 'Učitel', 'ucitel@teacher.com', NULL, now(), TRUE, 5);
INSERT INTO User Values (4, 'Zdepa', 'Pepa', '$2b$12$rtsFSnk299tZSlZNR2cbROqMcGEZnqouyg.5kES6BRQ50w.RtYUSK', 'Pepa321', 'pepa@zdepa.com', NULL, now(), TRUE, 0);

-- Nahodni uzivatele
INSERT INTO User Values (5, 'Novak', 'Petr', '$2b$12$rtsFSnk299tZSlZNR2cbROqMcGEZnqouyg.5kES6BRQ50w.RtYUSK', 'NovPet', 'novak55@seznam.cz', NULL, now(), TRUE, 0);
INSERT INTO User Values (6, 'Dijkstra', 'Sigismund', '$2b$12$rtsFSnk299tZSlZNR2cbROqMcGEZnqouyg.5kES6BRQ50w.RtYUSK', 'Sigi', 'sigi@reuven.cz', NULL, now(), TRUE, 1);
INSERT INTO User Values (7, 'Eilhart', 'Filippa', '$2b$12$rtsFSnk299tZSlZNR2cbROqMcGEZnqouyg.5kES6BRQ50w.RtYUSK', 'Crow', 'filipa99@seznam.cz', NULL, now(), TRUE, 3);
INSERT INTO User Values (8, 'Merigold', 'Triss', '$2b$12$rtsFSnk299tZSlZNR2cbROqMcGEZnqouyg.5kES6BRQ50w.RtYUSK', 'TheRedhead', 'bestgirl455@seznam.cz', NULL, now(), TRUE, 0);
INSERT INTO User Values (9, 'Uzumaki', 'Naruto', '$2b$12$rtsFSnk299tZSlZNR2cbROqMcGEZnqouyg.5kES6BRQ50w.RtYUSK', 'Ramen45', 'hokage66@seznam.cz', NULL, now(), TRUE, 2);
INSERT INTO User Values (10, 'Uchiha', 'Sasuke', '$2b$12$rtsFSnk299tZSlZNR2cbROqMcGEZnqouyg.5kES6BRQ50w.RtYUSK', 'Avenger', 'IhateItachi79@seznam.cz', NULL, now(), TRUE, 4);
INSERT INTO User Values (11, 'Bestgirl', 'Megumin', '$2b$12$rtsFSnk299tZSlZNR2cbROqMcGEZnqouyg.5kES6BRQ50w.RtYUSK', 'EXPLOOOSION', 'chomusuke33@axel.cz', NULL, now(), TRUE, 1);

-- Prideleni roli uzivatelum
INSERT INTO userroles values (1, 1);
INSERT INTO userroles values (2, 2);

-- Kurzy
INSERT INTO Course values ('IEL', 'Elektronika pro informační technologie', 1, 3, TRUE, now());
INSERT INTO Course values ('INK', 'Neschválený kurz', 1, 3, FALSE, now());
INSERT INTO Course values ('IDM', 'Diskrétní matematika', 1, 3, TRUE, now());

INSERT INTO Course values ('IAL', 'Algoritmy', 2, 3, TRUE, now());
INSERT INTO Course values ('ISS', 'Signály a systémy', 2, 3, TRUE, now());

INSERT INTO Course values ('IIS', 'Informační systémy', 3, 3, TRUE, now());
INSERT INTO Course values ('IMP', 'Vestavěné systémy', 3, 3, TRUE, now());

INSERT INTO Course values ('TIN', 'Teoretická informatika', 4, 3, TRUE, now());

-- Kategorie otazek
INSERT INTO Category values (1, 'Obecné', 'IEL');
INSERT INTO Category values (2, 'Cviceni 1', 'IEL');
INSERT INTO Category values (3, 'Obecné', 'INK');
INSERT INTO Category values (4, 'Obecné', 'IDM');
INSERT INTO Category values (5, 'Obecné', 'IAL');
INSERT INTO Category values (6, 'Obecné', 'ISS');
INSERT INTO Category values (7, 'Obecné', 'IIS');
INSERT INTO Category values (8, 'Projekt', 'IIS');
INSERT INTO Category values (9, 'Půlsemestrální zkouška', 'IIS');
INSERT INTO Category values (10, 'Obecné', 'IMP');
INSERT INTO Category values (11, 'Obecné', 'TIN');

INSERT INTO Question values (1, 'Jaký je rozdíl mezi stejnosměrným a střídavým proudem', 'Viz název otázky', FALSE, 3, 'Obecné', 'IEL', '2022', now(), now());
INSERT INTO Question values (2, 'Test', 'Test', FALSE, 3, 'Obecné', 'IEL', '2022', now(), now());

INSERT INTO Question values (3, 'Test', 'Test', FALSE, 4, 'Obecné', 'IDM', '2022', now(), now());
INSERT INTO Question values (4, 'Test', 'Test', FALSE, 5, 'Obecné', 'IAL', '2022', now(), now());

INSERT INTO Question values (5, 'Test', 'Test', FALSE, 3, 'Obecné', 'ISS', '2022', now(), now());

INSERT INTO Question values (6, 'Test', 'Test', FALSE, 9, 'Obecné', 'IIS', '2022', now(), now());
INSERT INTO Question values (7, 'SmUgdGVuaGxlIHByb2pla3QgZG9zdGHEjXVqw61jw60gbmEgcMSba27DqSBib2RvdsOpIG9ob2Rub2NlbsOtPw==', 'I sure do hope so', TRUE, 1, 'Projekt', 'IIS', '2022', now(), now());

INSERT INTO Question values (8, 'Test', 'Test', FALSE, 3, 'Obecné', 'IMP', '2022', now(), now());
INSERT INTO Question values (9, 'Test', 'Test', FALSE, 3, 'Obecné', 'TIN', '2022', now(), now());

-- Odpovedi
INSERT INTO Reply values (1, 9, 7, 5, TRUE, 'To se brzo dozvíme', now(), TRUE);
INSERT INTO Reply values (2, 6, 7, 2, FALSE, 'Určitě ano', now(), TRUE);

INSERT INTO Reply values (3, 5, 1, 5, FALSE, 'Test 1', now(), FALSE);
INSERT INTO Reply values (4, 4, 1, 5, FALSE, 'Test odpoved 2', now(), FALSE);

INSERT INTO Reply values (5, 5, 2, 0, FALSE, 'Test 2', now(), FALSE);
INSERT INTO Reply values (6, 7, 3, 0, FALSE, 'Test odpoved 3', now(), FALSE);

INSERT INTO Reply values (7, 5, 4, 0, FALSE, 'Test 3', now(), FALSE);
INSERT INTO Reply values (8, 7, 5, 0, FALSE, 'Test odpoved 4', now(), FALSE);

INSERT INTO Reply values (9, 5, 7, 0, FALSE, 'Test 4', now(), FALSE);
INSERT INTO Reply values (10, 7, 8, 0, FALSE, 'Test odpoved 5', now(), FALSE);

-- odpovedi na odpovedi
INSERT INTO Response values (1, 8, '+1, urcite to tak bude', 1, now(), 7);
INSERT INTO Response values (2, 7, '+2', 1, now(), 7);

INSERT INTO Response values (3, 8, '-1 Ne', 2, now(), 3);
INSERT INTO Response values (4, 8, '+1 Ano', 3, now(), 4);

INSERT INTO Response values (5, 8, '+1 Test', 7, now(), 3);
INSERT INTO Response values (6, 8, '+1 TestTest', 8, now(), 3);

-- Registrace uzivatelu na kurzi

INSERT INTO Course_registration values (1, 'IIS', 9, FALSE);
INSERT INTO Course_registration values (2, 'IIS', 1, TRUE);
INSERT INTO Course_registration values (3, 'IIS', 7, TRUE);
INSERT INTO Course_registration values (4, 'IIS', 6, TRUE);
INSERT INTO Course_registration values (5, 'IIS', 5, TRUE);

INSERT INTO Course_registration values (6, 'IEL', 9, FALSE);
INSERT INTO Course_registration values (7, 'IEL', 1, TRUE);
INSERT INTO Course_registration values (8, 'IEL', 7, FALSE);
INSERT INTO Course_registration values (9, 'IEL', 6, FALSE);
INSERT INTO Course_registration values (10, 'IEL', 5, TRUE);

INSERT INTO Course_registration values (11, 'ISS', 7, FALSE);
INSERT INTO Course_registration values (12, 'ISS', 6, FALSE);
INSERT INTO Course_registration values (13, 'ISS', 5, TRUE);

INSERT INTO Course_registration values (14, 'IAl', 7, FALSE);
INSERT INTO Course_registration values (15, 'IAL', 6, FALSE);
INSERT INTO Course_registration values (16, 'IAL', 5, TRUE);

INSERT INTO Course_registration values (17, 'IMP', 7, FALSE);
INSERT INTO Course_registration values (18, 'IMP', 6, FALSE);
INSERT INTO Course_registration values (19, 'IMP', 5, TRUE);

INSERT INTO Course_registration values (20, 'TIN', 7, FALSE);
INSERT INTO Course_registration values (21, 'TIN', 6, FALSE);