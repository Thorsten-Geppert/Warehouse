CREATE TABLE version (
	version VARCHAR(255)
);

INSERT INTO version VALUES ('4.0.0');

CREATE TABLE users (
	"username" VARCHAR(64),
	"password" VARCHAR(64),
	"realName" VARCHAR(128),
	"email" VARCHAR(255),
	"description" TEXT,
	"status" INTEGER DEFAULT 0,
	"openToRemind" BOOLEAN DEFAULT FALSE,
	PRIMARY KEY ("username")
);

INSERT INTO users VALUES ('admin', MD5('admin'), 'Administrator', 'info@bolso.de', 'Administrator account.Please change the default password!', 4);

CREATE TABLE enterprise (
	"id" SERIAL,
	"company" VARCHAR(255),
	"executive" VARCHAR(255),
	"address1" VARCHAR(255),
	"address2" VARCHAR(255),
	"zipcode" VARCHAR(255),
	"city" VARCHAR(255),
	"salesTaxIdentificationNumber" VARCHAR(255),
	"taxNumber" VARCHAR(255),
	"salesTaxIndicator" VARCHAR(255),
	"telephone" VARCHAR(255),
	"fax" VARCHAR(255),
	"email" VARCHAR(255),
	"web" VARCHAR(255),
	"bank" VARCHAR(255),
	"bankCode" VARCHAR(255),
	"holder" VARCHAR(255),
	"accountNumber" VARCHAR(255),
	"bic" VARCHAR(255),
	"iban" VARCHAR(255),
	"mandatoryProductsArticleNumber" BOOLEAN DEFAULT FALSE,
	"mandatoryClientsOwnClientId" BOOLEAN DEFAULT FALSE,
	PRIMARY KEY ("id")
);

CREATE TABLE common (
	"id" SERIAL,
	"minorValueAddedTax" FLOAT8,
	"majorValueAddedTax" FLOAT8,
	"monitionStates" INTEGER DEFAULT 3,
	"monitionPrice" FLOAT8 DEFAULT 0,
	"reportUse" BOOLEAN DEFAULT FALSE,
	"reportPathWindows" TEXT,
	"reportPathUnix" TEXT,
	"reportPathMacOS" TEXT,
	"jreHomeWindows" TEXT,
	"jreHomeUnix" TEXT,
	"jreHomeMacOS" TEXT,
	PRIMARY KEY ("id")
);

CREATE TABLE log (
	"id" SERIAL,
	"stamp" TIMESTAMP,
	"username" VARCHAR(64),
	"number" INTEGER,
	"module" VARCHAR(255),
	"description" TEXT,
	"message" TEXT,
	PRIMARY KEY (id)
);

CREATE TABLE clients (
	"clientId" SERIAL,
	"ownClientId" VARCHAR(255),
	"kind" VARCHAR(255),
	"formOfOrganization" INTEGER DEFAULT 0,
	"circularLetter" VARCHAR(255),
	"salutation" VARCHAR(64),
	"title" VARCHAR(64),
	"firstname" VARCHAR(255),
	"name1" VARCHAR(255),
	"name2" VARCHAR(255),
	"name3" VARCHAR(255),
	"name4" VARCHAR(255),
	"shortName" VARCHAR(255),
	"address" VARCHAR(255),
	"number" VARCHAR(16),
	"zipcode" VARCHAR(16),
	"city" VARCHAR(255),
	"federalState" VARCHAR(255),
	"land" VARCHAR(255),
	"telephone1Country" VARCHAR(8),
	"telephone1Prefix" VARCHAR(16),
	"telephone1Number" VARCHAR(32),
	"telephone2Country" VARCHAR(8),
	"telephone2Prefix" VARCHAR(16),
	"telephone2Number" VARCHAR(32),
	"telephone3Country" VARCHAR(8),
	"telephone3Prefix" VARCHAR(16),
	"telephone3Number" VARCHAR(32),
	"fax1Country" VARCHAR(8),
	"fax1Prefix" VARCHAR(16),
	"fax1Number" VARCHAR(32),
	"fax2Country" VARCHAR(8),
	"fax2Prefix" VARCHAR(16),
	"fax2Number" VARCHAR(32),
	"fax3Country" VARCHAR(8),
	"fax3Prefix" VARCHAR(16),
	"fax3Number" VARCHAR(32),
	"mobile1Country" VARCHAR(8),
	"mobile1Prefix" VARCHAR(16),
	"mobile1Number" VARCHAR(32),
	"mobile2Country" VARCHAR(8),
	"mobile2Prefix" VARCHAR(16),
	"mobile2Number" VARCHAR(32),
	"email1" VARCHAR(255),
	"email2" VARCHAR(255),
	"internet1" VARCHAR(255),
	"internet2" VARCHAR(255),
	"informations" TEXT,
	"taxNumber" VARCHAR(255),
	"salesTaxIdentificationNumber" VARCHAR(255),
	"discount" FLOAT8,
	"liableToTax" BOOLEAN DEFAULT TRUE,
	"group" VARCHAR(255),
	"sector" VARCHAR(255),
	"createdBy" VARCHAR(64),
	"createdAt" TIMESTAMP DEFAULT NOW(),
	"changedBy" VARCHAR(64),
	"changedAt" TIMESTAMP DEFAULT NOW(),
	"hiddenInformation" TEXT,
	"birthdayDay" INTEGER,
	"birthdayMonth" INTEGER,
	"birthdayYear" INTEGER,
	"active" BOOLEAN DEFAULT FALSE,
	"condition" VARCHAR(255),
	"monitionable" BOOLEAN DEFAULT TRUE,
	PRIMARY KEY ("clientId"),
	UNIQUE ("ownClientId")
);

CREATE TABLE salutations (
	"salutation" VARCHAR(255) PRIMARY KEY
);

INSERT INTO salutations VALUES ('Herr');
INSERT INTO salutations VALUES ('Frau');
INSERT INTO salutations VALUES ('Firma');

CREATE TABLE titles (
	"title" VARCHAR(255) PRIMARY KEY
);

INSERT INTO titles VALUES ('BA');
INSERT INTO titles VALUES ('BBA');
INSERT INTO titles VALUES ('BSc');
INSERT INTO titles VALUES ('Bakk.');
INSERT INTO titles VALUES ('Dipl.-Bw.');
INSERT INTO titles VALUES ('Dipl.-Bw. Dr.');
INSERT INTO titles VALUES ('Dipl.-Ing.');
INSERT INTO titles VALUES ('Dipl.-Ing. Dr.');
INSERT INTO titles VALUES ('Dipl.-Ing. DDr.');
INSERT INTO titles VALUES ('Dipl.-Ing. Dr.h.c.');
INSERT INTO titles VALUES ('Dipl.-Ing. Mag.');
INSERT INTO titles VALUES ('Dipl.-Ing. Mag. Dr.');
INSERT INTO titles VALUES ('Dipl.-HTL-Ing.');
INSERT INTO titles VALUES ('Dipl.-HLFL-Ing.');
INSERT INTO titles VALUES ('DDipl.-Ing.');
INSERT INTO titles VALUES ('DDipl.-Ing. Dr.');
INSERT INTO titles VALUES ('DDipl.-Ing. Mag. Dr.');
INSERT INTO titles VALUES ('Dkfm.');
INSERT INTO titles VALUES ('Dkfm. Dr.');
INSERT INTO titles VALUES ('Dkfm. DDr.');
INSERT INTO titles VALUES ('Dkfm. Dipl.-Ing.');
INSERT INTO titles VALUES ('Dkfm. Dipl.-Ing. Dr.');
INSERT INTO titles VALUES ('Dkfm. Mag.');
INSERT INTO titles VALUES ('Dkfm. Mag. Dr.');
INSERT INTO titles VALUES ('Dr.');
INSERT INTO titles VALUES ('DDr.');
INSERT INTO titles VALUES ('DDDr.');
INSERT INTO titles VALUES ('Dr.h.c.');
INSERT INTO titles VALUES ('Dipl.-Tierarzt');
INSERT INTO titles VALUES ('Dipl.-Tierarzt Dr.');
INSERT INTO titles VALUES ('Dipl.-Vw.');
INSERT INTO titles VALUES ('Dipl.-Vw. Dr.');
INSERT INTO titles VALUES ('Dipl.-Vw. Mag.');
INSERT INTO titles VALUES ('Ing.');
INSERT INTO titles VALUES ('Ing. Dr.');
INSERT INTO titles VALUES ('Ing. Dr.h.c.');
INSERT INTO titles VALUES ('Ing. Dipl.-Ing.');
INSERT INTO titles VALUES ('Ing. Dipl.-Ing. Dr.');
INSERT INTO titles VALUES ('Ing. Dipl.-Ing. Dr.h.c.');
INSERT INTO titles VALUES ('Ing. Mag.');
INSERT INTO titles VALUES ('Ing. Mag. Dr.');
INSERT INTO titles VALUES ('Ing. Dkfm.');
INSERT INTO titles VALUES ('Ing. Dkfm. Dr.');
INSERT INTO titles VALUES ('Lic');
INSERT INTO titles VALUES ('Mag.');
INSERT INTO titles VALUES ('Mag. Dr.');
INSERT INTO titles VALUES ('Mag. DDr.');
INSERT INTO titles VALUES ('Mag. DDDr.');
INSERT INTO titles VALUES ('Mag. Dr.h.c.');
INSERT INTO titles VALUES ('Mag. Dipl.-Ing.');
INSERT INTO titles VALUES ('Mag. Dipl.-Ing. Dr.');
INSERT INTO titles VALUES ('Mag. FH');
INSERT INTO titles VALUES ('MMag.');
INSERT INTO titles VALUES ('MMag. Dr.');
INSERT INTO titles VALUES ('MMag. DDr.');
INSERT INTO titles VALUES ('MMMag.');
INSERT INTO titles VALUES ('MMMag. Dr.');
INSERT INTO titles VALUES ('MA');
INSERT INTO titles VALUES ('MAS');
INSERT INTO titles VALUES ('MBA');
INSERT INTO titles VALUES ('MIB');
INSERT INTO titles VALUES ('Int. MLA');
INSERT INTO titles VALUES ('Prof.');

CREATE TABLE lands (
	"land" VARCHAR(255) PRIMARY KEY
);

INSERT INTO lands VALUES ('Afghanistan');
INSERT INTO lands VALUES ('Ägypten');
INSERT INTO lands VALUES ('Aland');
INSERT INTO lands VALUES ('Albanien');
INSERT INTO lands VALUES ('Algerien');
INSERT INTO lands VALUES ('Amerikanisch-Samoa');
INSERT INTO lands VALUES ('Amerikanische Jungferninseln');
INSERT INTO lands VALUES ('Andorra');
INSERT INTO lands VALUES ('Angola');
INSERT INTO lands VALUES ('Anguilla');
INSERT INTO lands VALUES ('Antarktis');
INSERT INTO lands VALUES ('Antigua und Barbuda');
INSERT INTO lands VALUES ('Äquatorialguinea, Republik');
INSERT INTO lands VALUES ('Argentinien');
INSERT INTO lands VALUES ('Armenien');
INSERT INTO lands VALUES ('Aruba');
INSERT INTO lands VALUES ('Ascension');
INSERT INTO lands VALUES ('Aserbaidschan');
INSERT INTO lands VALUES ('Äthiopien');
INSERT INTO lands VALUES ('Australien');
INSERT INTO lands VALUES ('Bahamas');
INSERT INTO lands VALUES ('Bahrain');
INSERT INTO lands VALUES ('Bangladesch');
INSERT INTO lands VALUES ('Barbados');
INSERT INTO lands VALUES ('Belgien');
INSERT INTO lands VALUES ('Belize');
INSERT INTO lands VALUES ('Benin');
INSERT INTO lands VALUES ('Bermuda');
INSERT INTO lands VALUES ('Bhutan');
INSERT INTO lands VALUES ('Bolivien');
INSERT INTO lands VALUES ('Bosnien und Herzegowina');
INSERT INTO lands VALUES ('Botswana');
INSERT INTO lands VALUES ('Bouvetinsel');
INSERT INTO lands VALUES ('Brasilien');
INSERT INTO lands VALUES ('Britische Jungferninseln');
INSERT INTO lands VALUES ('Britisches Territorium im Indischen Ozean');
INSERT INTO lands VALUES ('Brunei');
INSERT INTO lands VALUES ('Bulgarien');
INSERT INTO lands VALUES ('Burkina Faso');
INSERT INTO lands VALUES ('Burundi');
INSERT INTO lands VALUES ('Chile');
INSERT INTO lands VALUES ('China, Volksrepublik');
INSERT INTO lands VALUES ('Cookinseln');
INSERT INTO lands VALUES ('Costa Rica');
INSERT INTO lands VALUES ('Cote d''Ivoire');
INSERT INTO lands VALUES ('Dänemark');
INSERT INTO lands VALUES ('Deutschland');
INSERT INTO lands VALUES ('Die Kronkolonie St. Helena und Nebengebiete');
INSERT INTO lands VALUES ('Diego Garcia');
INSERT INTO lands VALUES ('Dominica');
INSERT INTO lands VALUES ('Dominikanische Republik');
INSERT INTO lands VALUES ('Dschibuti');
INSERT INTO lands VALUES ('Ecuador');
INSERT INTO lands VALUES ('El Salvador');
INSERT INTO lands VALUES ('Eritrea');
INSERT INTO lands VALUES ('Estland');
INSERT INTO lands VALUES ('Europäische Union');
INSERT INTO lands VALUES ('Falklandinseln');
INSERT INTO lands VALUES ('Färöer');
INSERT INTO lands VALUES ('Fidschi');
INSERT INTO lands VALUES ('Finnland');
INSERT INTO lands VALUES ('Frankreich');
INSERT INTO lands VALUES ('Französisch-Guayana');
INSERT INTO lands VALUES ('Französisch-Polynesien');
INSERT INTO lands VALUES ('Französische Süd- und Antarktisgebiete');
INSERT INTO lands VALUES ('Gabun');
INSERT INTO lands VALUES ('Gambia');
INSERT INTO lands VALUES ('Georgien');
INSERT INTO lands VALUES ('Ghana, Republik');
INSERT INTO lands VALUES ('Gibraltar');
INSERT INTO lands VALUES ('Grenada');
INSERT INTO lands VALUES ('Griechenland');
INSERT INTO lands VALUES ('Grönland');
INSERT INTO lands VALUES ('Guadeloupe');
INSERT INTO lands VALUES ('Guam');
INSERT INTO lands VALUES ('Guatemala');
INSERT INTO lands VALUES ('Guernsey, Vogtei');
INSERT INTO lands VALUES ('Guinea, Republik');
INSERT INTO lands VALUES ('Guinea-Bissau, Republik');
INSERT INTO lands VALUES ('Guyana');
INSERT INTO lands VALUES ('Haiti');
INSERT INTO lands VALUES ('Heard und McDonaldinseln');
INSERT INTO lands VALUES ('Honduras');
INSERT INTO lands VALUES ('Hongkong');
INSERT INTO lands VALUES ('Indien');
INSERT INTO lands VALUES ('Indonesien');
INSERT INTO lands VALUES ('Insel Man');
INSERT INTO lands VALUES ('Irak');
INSERT INTO lands VALUES ('Iran');
INSERT INTO lands VALUES ('Irland, Republik');
INSERT INTO lands VALUES ('Island');
INSERT INTO lands VALUES ('Israel');
INSERT INTO lands VALUES ('Italien');
INSERT INTO lands VALUES ('Jamaika');
INSERT INTO lands VALUES ('Japan');
INSERT INTO lands VALUES ('Jemen');
INSERT INTO lands VALUES ('Jersey');
INSERT INTO lands VALUES ('Jordanien');
INSERT INTO lands VALUES ('Kaimaninseln');
INSERT INTO lands VALUES ('Kambodscha');
INSERT INTO lands VALUES ('Kamerun');
INSERT INTO lands VALUES ('Kanada');
INSERT INTO lands VALUES ('Kanarische Inseln');
INSERT INTO lands VALUES ('Kap Verde, Republik');
INSERT INTO lands VALUES ('Kasachstan');
INSERT INTO lands VALUES ('Katar');
INSERT INTO lands VALUES ('Kenia');
INSERT INTO lands VALUES ('Kirgisistan');
INSERT INTO lands VALUES ('Kiribati');
INSERT INTO lands VALUES ('Kokosinseln');
INSERT INTO lands VALUES ('Kolumbien');
INSERT INTO lands VALUES ('Komoren');
INSERT INTO lands VALUES ('Kongo, Demokratische Republik');
INSERT INTO lands VALUES ('Kongo, Republik');
INSERT INTO lands VALUES ('Korea, Demokratische Volkrepublik');
INSERT INTO lands VALUES ('Korea, Republik');
INSERT INTO lands VALUES ('Kroatien');
INSERT INTO lands VALUES ('Kuba');
INSERT INTO lands VALUES ('Kuwait');
INSERT INTO lands VALUES ('Laos');
INSERT INTO lands VALUES ('Lesotho');
INSERT INTO lands VALUES ('Lettland');
INSERT INTO lands VALUES ('Libanon');
INSERT INTO lands VALUES ('Liberia, Republik');
INSERT INTO lands VALUES ('Libyen');
INSERT INTO lands VALUES ('Liechtenstein, Fürstentum');
INSERT INTO lands VALUES ('Litauen');
INSERT INTO lands VALUES ('Luxemburg');
INSERT INTO lands VALUES ('Macao');
INSERT INTO lands VALUES ('Madagaskar, Republik');
INSERT INTO lands VALUES ('Malawi, Republik');
INSERT INTO lands VALUES ('Malaysia');
INSERT INTO lands VALUES ('Malediven');
INSERT INTO lands VALUES ('Mali, Republik');
INSERT INTO lands VALUES ('Malta');
INSERT INTO lands VALUES ('Marokko');
INSERT INTO lands VALUES ('Marshallinseln');
INSERT INTO lands VALUES ('Martinique');
INSERT INTO lands VALUES ('Mauretanien');
INSERT INTO lands VALUES ('Mauritius, Republik');
INSERT INTO lands VALUES ('Mayotte');
INSERT INTO lands VALUES ('Mazedonien');
INSERT INTO lands VALUES ('Mexiko');
INSERT INTO lands VALUES ('Mikronesien, Föderierte Staaten von');
INSERT INTO lands VALUES ('Moldawien');
INSERT INTO lands VALUES ('Monaco');
INSERT INTO lands VALUES ('Mongolei');
INSERT INTO lands VALUES ('Montserrat');
INSERT INTO lands VALUES ('Mosambik');
INSERT INTO lands VALUES ('Myanmar');
INSERT INTO lands VALUES ('Namibia, Republik');
INSERT INTO lands VALUES ('Nauru');
INSERT INTO lands VALUES ('Nepal');
INSERT INTO lands VALUES ('Neukaledonien');
INSERT INTO lands VALUES ('Neuseeland');
INSERT INTO lands VALUES ('Neutrale Zone');
INSERT INTO lands VALUES ('Nicaragua');
INSERT INTO lands VALUES ('Niederlande');
INSERT INTO lands VALUES ('Niederländische Antillen');
INSERT INTO lands VALUES ('Niger');
INSERT INTO lands VALUES ('Nigeria');
INSERT INTO lands VALUES ('Niue');
INSERT INTO lands VALUES ('Nördliche Marianen');
INSERT INTO lands VALUES ('Norfolkinsel');
INSERT INTO lands VALUES ('Norwegen');
INSERT INTO lands VALUES ('Oman');
INSERT INTO lands VALUES ('Österreich');
INSERT INTO lands VALUES ('Pakistan');
INSERT INTO lands VALUES ('Palästinensische Autonomiegebiete');
INSERT INTO lands VALUES ('Palau');
INSERT INTO lands VALUES ('Panama');
INSERT INTO lands VALUES ('Papua-Neuguinea');
INSERT INTO lands VALUES ('Paraguay');
INSERT INTO lands VALUES ('Peru');
INSERT INTO lands VALUES ('Philippinen');
INSERT INTO lands VALUES ('Pitcairninseln');
INSERT INTO lands VALUES ('Polen');
INSERT INTO lands VALUES ('Portugal');
INSERT INTO lands VALUES ('Puerto Rico');
INSERT INTO lands VALUES ('Réunion');
INSERT INTO lands VALUES ('Ruanda, Republik');
INSERT INTO lands VALUES ('Rumänien');
INSERT INTO lands VALUES ('Russische Föderation');
INSERT INTO lands VALUES ('Salomonen');
INSERT INTO lands VALUES ('Sambia, Republik');
INSERT INTO lands VALUES ('Samoa');
INSERT INTO lands VALUES ('San Marino');
INSERT INTO lands VALUES ('São Tomé und Príncipe');
INSERT INTO lands VALUES ('Saudi-Arabien, Königreich');
INSERT INTO lands VALUES ('Schweden');
INSERT INTO lands VALUES ('Schweiz');
INSERT INTO lands VALUES ('Senegal');
INSERT INTO lands VALUES ('Serbien und Montenegro');
INSERT INTO lands VALUES ('Seychellen, Republik der');
INSERT INTO lands VALUES ('Sierra Leone, Republik');
INSERT INTO lands VALUES ('Simbabwe, Republik');
INSERT INTO lands VALUES ('Singapur');
INSERT INTO lands VALUES ('Slowakei');
INSERT INTO lands VALUES ('Slowenien');
INSERT INTO lands VALUES ('Somalia, Demokratische Republik');
INSERT INTO lands VALUES ('Spanien');
INSERT INTO lands VALUES ('Sri Lanka');
INSERT INTO lands VALUES ('St. Kitts und Nevis');
INSERT INTO lands VALUES ('St. Lucia');
INSERT INTO lands VALUES ('St. Pierre und Miquelon');
INSERT INTO lands VALUES ('St. Vincent und die Grenadinen (GB)');
INSERT INTO lands VALUES ('Südafrika, Republik');
INSERT INTO lands VALUES ('Sudan');
INSERT INTO lands VALUES ('Südgeorgien und die Südlichen Sandwichinseln');
INSERT INTO lands VALUES ('Suriname');
INSERT INTO lands VALUES ('Svalbard und Jan Mayen');
INSERT INTO lands VALUES ('Swasiland');
INSERT INTO lands VALUES ('Syrien');
INSERT INTO lands VALUES ('Tadschikistan');
INSERT INTO lands VALUES ('Taiwan');
INSERT INTO lands VALUES ('Tansania, Vereinigte Republik');
INSERT INTO lands VALUES ('Thailand');
INSERT INTO lands VALUES ('Timor-Leste, Demokratische Republik');
INSERT INTO lands VALUES ('Togo, Republik');
INSERT INTO lands VALUES ('Tokelau');
INSERT INTO lands VALUES ('Tonga');
INSERT INTO lands VALUES ('Trinidad und Tobago');
INSERT INTO lands VALUES ('Tristan da Cunha');
INSERT INTO lands VALUES ('Tschad, Republik');
INSERT INTO lands VALUES ('Tschechische Republik');
INSERT INTO lands VALUES ('Tunesien');
INSERT INTO lands VALUES ('Türkei');
INSERT INTO lands VALUES ('Turkmenistan');
INSERT INTO lands VALUES ('Turks- und Caicosinseln');
INSERT INTO lands VALUES ('Tuvalu');
INSERT INTO lands VALUES ('Uganda, Republik');
INSERT INTO lands VALUES ('Ukraine');
INSERT INTO lands VALUES ('Union der Sozialistischen Sowjetrepubliken');
INSERT INTO lands VALUES ('Uruguay');
INSERT INTO lands VALUES ('Usbekistan');
INSERT INTO lands VALUES ('Vanuatu');
INSERT INTO lands VALUES ('Vatikanstadt');
INSERT INTO lands VALUES ('Venezuela');
INSERT INTO lands VALUES ('Vereinigte Arabische Emirate');
INSERT INTO lands VALUES ('Vereinigte Staaten von Amerika');
INSERT INTO lands VALUES ('Vereinigtes Königreich von Großbritannien und Nordirland');
INSERT INTO lands VALUES ('Vietnam');
INSERT INTO lands VALUES ('Wallis und Futuna');
INSERT INTO lands VALUES ('Weihnachtsinsel');
INSERT INTO lands VALUES ('Weißrussland');
INSERT INTO lands VALUES ('Westsahara');
INSERT INTO lands VALUES ('Zentralafrikanische Republik');
INSERT INTO lands VALUES ('Zypern, Republik');
INSERT INTO lands VALUES ('Ungarn');
INSERT INTO lands VALUES ('Montenegro');

CREATE TABLE conditions (
	"condition" VARCHAR(255) PRIMARY KEY
);

INSERT INTO conditions VALUES ('Zahlbar sofort ohne Skonto');
INSERT INTO conditions VALUES ('Zahlbar sofort ohne Abzug');
INSERT INTO conditions VALUES ('10 Tage - 3 %, 30 Tage netto');
INSERT INTO conditions VALUES ('7 Tage - 2 %, 20 Tage netto');
INSERT INTO conditions VALUES ('2 % zum 15. des 2. Folgemonats');

CREATE TABLE delivery (
	"delivery" VARCHAR(255) PRIMARY KEY
);

INSERT INTO delivery VALUES ('Post - Brief');
INSERT INTO delivery VALUES ('Post - Brief international');
INSERT INTO delivery VALUES ('Post - Postkarte');
INSERT INTO delivery VALUES ('Post - Einschreiben');
INSERT INTO delivery VALUES ('Post - Einschreiben mit Rückschein');
INSERT INTO delivery VALUES ('Post - Büchersendung - Standard');
INSERT INTO delivery VALUES ('Post - Büchersendung - Kompakt');
INSERT INTO delivery VALUES ('Post - Büchersendung - Groß');
INSERT INTO delivery VALUES ('Post - Büchersendung - Maxi');
INSERT INTO delivery VALUES ('Post - Warensendung - Standard');
INSERT INTO delivery VALUES ('Post - Warensendung - Komptakt');
INSERT INTO delivery VALUES ('Post - Warensendung - Maxi');
INSERT INTO delivery VALUES ('Post - Warensendung - ');
INSERT INTO delivery VALUES ('DHL - Paket');
INSERT INTO delivery VALUES ('DHL - Päckchen');
INSERT INTO delivery VALUES ('DHL - Expressbrief');
INSERT INTO delivery VALUES ('DPD - Paket');
INSERT INTO delivery VALUES ('GLS - Parcel - Business');
INSERT INTO delivery VALUES ('GLS - Parcel - Business-Small');
INSERT INTO delivery VALUES ('GLS - Parcel - Euro Business');
INSERT INTO delivery VALUES ('GLS - Parcel - Euro Business-Small');
INSERT INTO delivery VALUES ('GLS - Parcel - Global Business');
INSERT INTO delivery VALUES ('GLS - Express');
INSERT INTO delivery VALUES ('GLS - Euro-Express');
INSERT INTO delivery VALUES ('Hermes - Paket');

CREATE TABLE federalstate (
	"federalstate" VARCHAR(255) PRIMARY KEY
);

INSERT INTO federalstate VALUES ('Baden-Württemberg');
INSERT INTO federalstate VALUES ('Bayern');
INSERT INTO federalstate VALUES ('Berlin');
INSERT INTO federalstate VALUES ('Brandenburg');
INSERT INTO federalstate VALUES ('Bremen');
INSERT INTO federalstate VALUES ('Hamburg');
INSERT INTO federalstate VALUES ('Hessen');
INSERT INTO federalstate VALUES ('Mecklenburg-Vorpommern');
INSERT INTO federalstate VALUES ('Niedersachsen');
INSERT INTO federalstate VALUES ('Nordrhein-Westfalen');
INSERT INTO federalstate VALUES ('Rheinland-Pfalz');
INSERT INTO federalstate VALUES ('Saarland');
INSERT INTO federalstate VALUES ('Sachsen');
INSERT INTO federalstate VALUES ('Sachsen-Anhalt');
INSERT INTO federalstate VALUES ('Schleswig-Holstein');
INSERT INTO federalstate VALUES ('Thüringen');

CREATE TABLE products (
	"productId" SERIAL,
	"articleNumber" VARCHAR(255),
	"group" VARCHAR(255),
	"package" VARCHAR(255),
	"available" BOOLEAN DEFAULT TRUE,
	"distributorId" INTEGER,
	"manufacturer" VARCHAR(255),
	"publisher" VARCHAR(255),
	"ean" VARCHAR(255),
	"shortName" VARCHAR(255),
	"name1" VARCHAR(255),
	"name2" VARCHAR(255),
	"name3" VARCHAR(255),
	"name4" VARCHAR(255),
	"description" TEXT,
	"isbn10" VARCHAR(255),
	"isbn13" VARCHAR(255),
	"note" TEXT,
	"tax" INTEGER NOT NULL DEFAULT 0, -- 0 full, 1 reduced
	"priceWithoutTax" FLOAT8,
	"priceWithTax" FLOAT8,
	"purchasePriceWithoutTax" FLOAT8,
	"purchasePriceWithTax" FLOAT8,
	"weight" FLOAT8,
	"createdBy" VARCHAR(64),
	"createdAt" TIMESTAMP DEFAULT NOW(),
	"changedBy" VARCHAR(64),
	"changedAt" TIMESTAMP DEFAULT NOW(),
	"hiddenInformation" TEXT,
	PRIMARY KEY ("productId"),
	FOREIGN KEY ("distributorId") REFERENCES clients ("clientId") ON UPDATE CASCADE ON DELETE RESTRICT
);

CREATE TABLE orders (
	"orderId" SERIAL,
	"kind" INTEGER NOT NULL DEFAULT 0, -- 0 Angebot, 1 Auftrag, 2 Gutschrift
	"clientId" INTEGER,
	"ownClientId" INTEGER,
	"basedOn" INTEGER,
	"taxRate" INTEGER NOT NULL DEFAULT 0, -- 0 full, 1 reduced
	"when" DATE,
	"billAddressSalutation" VARCHAR(255),
	"billAddressTitle" VARCHAR(64),
	"billAddressFirstname" VARCHAR(64),
	"billAddressName1" VARCHAR(255),
	"billAddressName2" VARCHAR(255),
	"billAddressName3" VARCHAR(255),
	"billAddressName4" VARCHAR(255),
	"billAddressAddress" VARCHAR(255),
	"billAddressNumber" VARCHAR(16),
	"billAddressZipcode" VARCHAR(16),
	"billAddressCity" VARCHAR(255),
	"billAddressFederalState" VARCHAR(255),
	"billAddressLand" VARCHAR(255),
	"billAddressTranslated" TEXT,
	"shippingAddressSalutation" VARCHAR(64),
	"shippingAddressTitle" VARCHAR(64),
	"shippingAddressFirstname" VARCHAR(255),
	"shippingAddressName1" VARCHAR(255),
	"shippingAddressName2" VARCHAR(255),
	"shippingAddressName3" VARCHAR(255),
	"shippingAddressName4" VARCHAR(255),
	"shippingAddressAddress" VARCHAR(255),
	"shippingAddressNumber" VARCHAR(16),
	"shippingAddressZipcode" VARCHAR(16),
	"shippingAddressCity" VARCHAR(255),
	"shippingAddressFederalState" VARCHAR(255),
	"shippingAddressLand" VARCHAR(255),
	"shippingAddressTranslated" TEXT,
	"payment" INTEGER NOT NULL DEFAULT 0,
	"bank" VARCHAR(255),
	"bankCode" VARCHAR(16),
	"accountNumber" VARCHAR(16),
	"bic" VARCHAR(255),
	"iban" VARCHAR(255),
	"bankOwner" VARCHAR(255),
	"liableToTax" BOOLEAN DEFAULT TRUE,
	"beforeTax" FLOAT8,
	"fullTax" FLOAT8,
	"reducedTax" FLOAT8,
	"afterTax" FLOAT8,
	"fullTaxPercent" FLOAT8,
	"reducedTaxPercent" FLOAT8,
	"invoiceId" INTEGER,
	"invoiceDate" DATE,
	"condition" VARCHAR(255),
	"orderNumber" VARCHAR(255),
	"delivery" VARCHAR(255),
	"prepandText" TEXT,
	"appendText" TEXT,
	"prints" INTEGER DEFAULT 0,
	"payed" TIMESTAMP,
	"linkedOrderId" INTEGER,
	"linkedOrderIndex" INTEGER,
	"taxNumber" VARCHAR(255),
	"salesTaxIdentificationNumber" VARCHAR(255),
	"monition" BOOLEAN DEFAULT TRUE,
	"monitionState" INTEGER DEFAULT 0,
	"owner" VARCHAR(255),
	"createdBy" VARCHAR(64),
	"createdAt" TIMESTAMP DEFAULT NOW(),
	"changedBy" VARCHAR(64),
	"changedAt" TIMESTAMP DEFAULT NOW(),
	PRIMARY KEY ("orderId"),
	FOREIGN KEY ("clientId") REFERENCES clients ON UPDATE CASCADE ON DELETE RESTRICT,
	FOREIGN KEY ("basedOn") REFERENCES orders ("orderId") ON UPDATE CASCADE ON DELETE RESTRICT,
	FOREIGN KEY ("linkedOrderId") REFERENCES orders ON UPDATE CASCADE ON DELETE RESTRICT
);

CREATE TABLE stocks (
	"name" VARCHAR(255),
	"address" VARCHAR(255),
	"number" VARCHAR(16),
	"zipcode" VARCHAR(16),
	"city" VARCHAR(255),
	"comment" TEXT,
	"active" BOOLEAN DEFAULT false,
	PRIMARY KEY ("name")
);

CREATE TABLE orders_positions (
	"orderPositionId" SERIAL,
	"orderId" INTEGER,
	"rank" INTEGER,
	"amount" FLOAT8,
	"productId" INTEGER,
	"text" TEXT,
	"price" FLOAT8,
	"fullPrice" FLOAT8,
	"reduction" FLOAT8,
	"reducted" FLOAT8,
	"tax" FLOAT8,
	"sum" FLOAT8,
	"absolutePrice" FLOAT8,
	"taxPercent" FLOAT8,
	"taxLevel" FLOAT8,
	"beforeTax" FLOAT8,
	"afterTax" FLOAT8,
	"articleNumber" VARCHAR(255),
	"ean" VARCHAR(255),
	"shortName" VARCHAR(255),
	"name1" VARCHAR(255),
	"name2" VARCHAR(255),
	"name3" VARCHAR(255),
	"name4" VARCHAR(255),
	"description" TEXT,
	"note" TEXT,
	"isbn10" VARCHAR(255),
	"isbn13" VARCHAR(255),
	"group" VARCHAR(255),
	"package" VARCHAR(255),
	"available" BOOLEAN DEFAULT TRUE,
	"distributorId" INTEGER,
	"manufacturer" VARCHAR(255),
	"publisher" VARCHAR(255),
	"weight" FLOAT8,
	"priceWithoutTax" FLOAT8,
	"priceWithTax" FLOAT8,
	"purchasePriceWithoutTax" FLOAT8,
	"purchasePriceWithTax" FLOAT8,
	"stock" VARCHAR(255),
	"bookedOut" BOOLEAN DEFAULT FALSE,
	PRIMARY KEY ("orderPositionId"),
	FOREIGN KEY ("orderId") REFERENCES orders ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY ("productId") REFERENCES products ON UPDATE CASCADE ON DELETE RESTRICT,
	FOREIGN KEY ("stock") REFERENCES stocks ("name") ON UPDATE CASCADE ON DELETE SET NULL,
	FOREIGN KEY ("distributorId") REFERENCES clients ("clientId") ON UPDATE CASCADE ON DELETE RESTRICT
);

CREATE TABLE notes (
	"noteId" SERIAL,
	"title" VARCHAR(255),
	"note" TEXT,
	"createdBy" VARCHAR(64),
	"createdAt" TIMESTAMP DEFAULT NOW(),
	"changedBy" VARCHAR(64),
	"changedAt" TIMESTAMP DEFAULT NOW(),
	PRIMARY KEY ("noteId")
);

CREATE SEQUENCE "orders_orderInvoiceId_seq" INCREMENT 1 START {s:invoiceStartSequence};
CREATE SEQUENCE "orders_creditInvoiceId_seq" INCREMENT 1 START {s:creditStartSequence};

CREATE TABLE banks (
	"bankId" SERIAL,
	"bank" VARCHAR(255),
	"bankCode" VARCHAR(255),
	"bic" VARCHAR(255),
	PRIMARY KEY ("bankId")
);

SELECT SETVAL('"clients_clientId_seq"', {s:clientStartSequence}, true);
SELECT SETVAL('"products_productId_seq"', {s:productStartSequence}, true);
SELECT SETVAL('"orders_orderId_seq"', {s:orderStartSequence}, true);

CREATE TABLE file_categories (
	"categoryId" SERIAL,
	"parentId" INTEGER,
	"name" VARCHAR(255),
	PRIMARY KEY ("categoryId"),
	FOREIGN KEY ("parentId") REFERENCES file_categories ("categoryId") ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE files (
	"fileId" SERIAL,
	"categoryId" INTEGER,
	"name" VARCHAR(255),
	"description" TEXT,
	"origin" TEXT,
	"mimetype" VARCHAR(64),
	"extension" VARCHAR(255),
	"size" INTEGER,
	"humanSize" VARCHAR(255),
	"fileOid" OID,
	PRIMARY KEY ("fileId"),
	FOREIGN KEY ("categoryId") REFERENCES file_categories ("categoryId") ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE product_groups (
	"group" VARCHAR(255) PRIMARY KEY
);

CREATE TABLE product_packages (
	"package" VARCHAR(255) PRIMARY KEY
);

INSERT INTO product_packages VALUES ('Stk.');
INSERT INTO product_packages VALUES ('Std.');
INSERT INTO product_packages VALUES ('Pauschale');
INSERT INTO product_packages VALUES ('m²');
INSERT INTO product_packages VALUES ('m³');
INSERT INTO product_packages VALUES ('lfm');

CREATE TABLE distributors (
	"distributor" VARCHAR(255) PRIMARY KEY
);

CREATE TABLE manufacturers (
	"manufacturer" VARCHAR(255) PRIMARY KEY
);

CREATE TABLE publishers (
	"publisher" VARCHAR(255) PRIMARY KEY
);

CREATE TABLE addresses (
	"addressId" SERIAL,
	"clientId" INTEGER,
	"description" VARCHAR(255),
	"salutation" VARCHAR(64),
	"title" VARCHAR(64),
	"firstname" VARCHAR(255),
	"name1" VARCHAR(255),
	"name2" VARCHAR(255),
	"name3" VARCHAR(255),
	"name4" VARCHAR(255),
	"address" VARCHAR(255),
	"number" VARCHAR(16),
	"zipcode" VARCHAR(16),
	"city" VARCHAR(255),
	"federalState" VARCHAR(255),
	"land" VARCHAR(255),
	"information" TEXT,
	PRIMARY KEY ("addressId"),
	FOREIGN KEY ("clientId") REFERENCES clients ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE contactpersons (
	"contactPersonId" SERIAL,
	"clientId" INTEGER,
	"function" VARCHAR(255),
	"department" VARCHAR(255),
	"salutation" VARCHAR(64),
	"title" VARCHAR(64),
	"firstname" VARCHAR(255),
	"lastname" VARCHAR(255),
	"telephoneCountry" VARCHAR(8),
	"telephonePrefix" VARCHAR(16),
	"telephoneNumber" VARCHAR(32),
	"mobileCountry" VARCHAR(8),
	"mobilePrefix" VARCHAR(16),
	"mobileNumber" VARCHAR(32),
	"faxCountry" VARCHAR(8),
	"faxPrefix" VARCHAR(16),
	"faxNumber" VARCHAR(32),
	"email" VARCHAR(255),
	"internet" VARCHAR(255),
	"information" TEXT,
	PRIMARY KEY ("contactPersonId"),
	FOREIGN KEY ("clientId") REFERENCES clients ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE departments (
	"department" VARCHAR(255) PRIMARY KEY
);

INSERT INTO departments VALUES ('Geschäftsführung');
INSERT INTO departments VALUES ('Personalabteilung');
INSERT INTO departments VALUES ('Buchhaltung');
INSERT INTO departments VALUES ('Rechnungswesen');
INSERT INTO departments VALUES ('Finanzen');
INSERT INTO departments VALUES ('Rechtsabteilung');
INSERT INTO departments VALUES ('Forschung & Entwicklung');
INSERT INTO departments VALUES ('Fertigung');
INSERT INTO departments VALUES ('Produktion');
INSERT INTO departments VALUES ('Qualitätssicherung');
INSERT INTO departments VALUES ('Arbeitssicherheit');
INSERT INTO departments VALUES ('Öffentlichkeitsarbeit');
INSERT INTO departments VALUES ('Marketingabteilung');
INSERT INTO departments VALUES ('IT');
INSERT INTO departments VALUES ('EDV');
INSERT INTO departments VALUES ('Technik');
INSERT INTO departments VALUES ('Verkauf');
INSERT INTO departments VALUES ('Vertrieb');
INSERT INTO departments VALUES ('Logistik');
INSERT INTO departments VALUES ('Materialwirtschaft');
INSERT INTO departments VALUES ('Poststelle');
INSERT INTO departments VALUES ('Kundenbetreuung');
INSERT INTO departments VALUES ('Hausverwaltung');

CREATE TABLE functions (
	"function" VARCHAR(255) PRIMARY KEY
);

CREATE TABLE descriptions (
	"description" VARCHAR(255) PRIMARY KEY
);


INSERT INTO descriptions VALUES ('Hauptaddresse');
INSERT INTO descriptions VALUES ('Lieferaddresse');
INSERT INTO descriptions VALUES ('Auslandsadresse');
INSERT INTO descriptions VALUES ('Firmenadresse');

CREATE TABLE linked_media_descriptions (
	"linked_media_description" VARCHAR(255) PRIMARY KEY
);

INSERT INTO linked_media_descriptions VALUES ('Produktbild groß');
INSERT INTO linked_media_descriptions VALUES ('Produktbild mittel');
INSERT INTO linked_media_descriptions VALUES ('Produktbild klein');
INSERT INTO linked_media_descriptions VALUES ('Produktbild');
INSERT INTO linked_media_descriptions VALUES ('Rechnung');
INSERT INTO linked_media_descriptions VALUES ('Gutschrift');
INSERT INTO linked_media_descriptions VALUES ('Lieferschein');

CREATE TABLE linked_files_clients (
	"linkedFileId" SERIAL,
	"fileId" INTEGER,
	"description" VARCHAR(255),
	"id" INTEGER,
	PRIMARY KEY ("linkedFileId"),
	FOREIGN KEY ("fileId") REFERENCES files ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY ("id") REFERENCES clients ("clientId") ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE linked_files_products (
	"linkedFileId" SERIAL,
	"fileId" INTEGER,
	"id" INTEGER,
	"description" VARCHAR(255),
	PRIMARY KEY ("linkedFileId"),
	FOREIGN KEY ("fileId") REFERENCES files ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY ("id") REFERENCES products ("productId") ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE linked_files_orders (
	"linkedFileId" SERIAL,
	"fileId" INTEGER,
	"description" VARCHAR(255),
	"id" INTEGER,
	PRIMARY KEY ("linkedFileId"),
	FOREIGN KEY ("fileId") REFERENCES files ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY ("id") REFERENCES orders ("orderId") ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE keys_values_clients (
	"keyValueId" SERIAL,
	"id" INTEGER,
	"key" VARCHAR(255),
	"value" VARCHAR(255),
	PRIMARY KEY("keyValueId"),
	FOREIGN KEY ("id") REFERENCES clients ("clientId") ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE keys_values_products (
	"keyValueId" SERIAL,
	"id" INTEGER,
	"key" VARCHAR(255),
	"value" VARCHAR(255),
	PRIMARY KEY("keyValueId"),
	FOREIGN KEY ("id") REFERENCES products ("productId") ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE keys_values_orders (
	"keyValueId" SERIAL,
	"id" INTEGER,
	"key" VARCHAR(255),
	"value" VARCHAR(255),
	PRIMARY KEY("keyValueId"),
	FOREIGN KEY ("id") REFERENCES orders ("orderId") ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE bank_connections (
	"bankConnectionId" SERIAL,
	"clientId" INTEGER,
	"description" VARCHAR(255),
	"bank" VARCHAR(255),
	"bankCode" VARCHAR(255),
	"accountNumber" VARCHAR(255),
	"bic" VARCHAR(255),
	"iban" VARCHAR(255),
	"bankOwner" VARCHAR(255),
	"referenceNumber" VARCHAR(255),
	"referenceDate" TIMESTAMP DEFAULT NOW(),
	"referenceState" VARCHAR(255),
	"referenceAccount" VARCHAR(255),
	"notes" TEXT,
	"active" BOOLEAN DEFAULT TRUE,
	PRIMARY KEY ("bankConnectionId"),
	FOREIGN KEY ("clientId") REFERENCES clients ("clientId") ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE templates (
	"templateId" SERIAL,
	"name" VARCHAR(255),
	"content" TEXT,
	PRIMARY KEY ("templateId")
);

CREATE TABLE monition_texts (
	"number" INTEGER,
	"prepend" TEXT,
	"append" TEXT,
	PRIMARY KEY ("number")
);

CREATE TABLE monitions (
	"monitionId" SERIAL,
	"invoiceDate" DATE,
	"clientId" INTEGER,
	"orderId" INTEGER,
	"invoiceId" INTEGER,
	"price" FLOAT8,
	"created" DATE DEFAULT NOW(),
	"monitionState" INTEGER DEFAULT 0,
	PRIMARY KEY ("monitionId"),
	FOREIGN KEY ("clientId") REFERENCES clients ON UPDATE CASCADE ON DELETE RESTRICT,
	FOREIGN KEY ("orderId") REFERENCES orders ON UPDATE CASCADE ON DELETE RESTRICT
);

CREATE TABLE vouchers (
	"voucherId" SERIAL,
	"description" VARCHAR(255),
	"invoiceId" VARCHAR(255),
	"invoiceDate" DATE,
	"company" TEXT,
	"notes" TEXT,
	"priceWithoutTax" FLOAT8,
	"reducedTax" FLOAT8,
	"fullTax" FLOAT8,
	"priceWithTax" FLOAT8,
	"createdBy" VARCHAR(64),
	"createdAt" TIMESTAMP DEFAULT NOW(),
	"changedBy" VARCHAR(64),
	"changedAt" TIMESTAMP DEFAULT NOW(),
	PRIMARY KEY ("voucherId")
);

CREATE TABLE keys_values_vouchers (
	"keyValueId" SERIAL,
	"id" INTEGER,
	"key" VARCHAR(255),
	"value" VARCHAR(255),
	PRIMARY KEY("keyValueId"),
	FOREIGN KEY ("id") REFERENCES vouchers ("voucherId") ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE linked_files_vouchers (
	"linkedFileId" SERIAL,
	"fileId" INTEGER,
	"description" VARCHAR(255),
	"id" INTEGER,
	PRIMARY KEY ("linkedFileId"),
	FOREIGN KEY ("fileId") REFERENCES files ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY ("id") REFERENCES vouchers ("voucherId") ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE vouchers_positions (
	"voucherPositionId" SERIAL,
	"voucherId" INTEGER,
	"quantity" INTEGER DEFAULT 1,
	"description" TEXT,
	"articleNumber" VARCHAR(255),
	"article" VARCHAR(255),
	"tax" INTEGER,
	"taxPrice" FLOAT8,
	"price" FLOAT8,
	PRIMARY KEY ("voucherPositionId"),
	FOREIGN KEY ("voucherId") REFERENCES vouchers ("voucherId") ON UPDATE CASCADE ON DELETE CASCADE
);







CREATE TABLE purchases (
	"purchaseId" SERIAL,
	"distributorId" INTEGER,
	"distributorAddress" TEXT,
	"clientId" VARCHAR(255),
	"orderId" VARCHAR(255),
	"orderDate" DATE DEFAULT NOW(),
	"purchaser" VARCHAR(255),
	"informations" TEXT,
	"billingAddress" TEXT,
	"deliveryAddress" TEXT,
	"textHead" TEXT,
	"textFoot" TEXT,
	"sum" FLOAT8 DEFAULT 0.0,
	"done" DATE,
	"createdBy" VARCHAR(64),
	"createdAt" TIMESTAMP DEFAULT NOW(),
	"changedBy" VARCHAR(64),
	"changedAt" TIMESTAMP DEFAULT NOW(),
	PRIMARY KEY ("purchaseId"),
	FOREIGN KEY ("distributorId") REFERENCES clients ("clientId") ON UPDATE CASCADE ON DELETE RESTRICT
);

SELECT SETVAL('"purchases_purchaseId_seq"', {s:purchaseStartSequence}, true);

CREATE TABLE purchases_positions (
	"purchasePositionId" SERIAL,
	"purchaseId" INTEGER,
	"amount" FLOAT8,
	"productId" INTEGER,
	"text" TEXT,
	"price" FLOAT8,
	"fullPrice" FLOAT8,
	"articleNumber" VARCHAR(255),
	"ean" VARCHAR(255),
	"shortName" VARCHAR(255),
	"name1" VARCHAR(255),
	"name2" VARCHAR(255),
	"name3" VARCHAR(255),
	"name4" VARCHAR(255),
	"description" TEXT,
	"note" TEXT,
	"isbn10" VARCHAR(255),
	"isbn13" VARCHAR(255),
	"group" VARCHAR(255),
	"package" VARCHAR(255),
	"available" BOOLEAN DEFAULT TRUE,
	"distributorId" INTEGER,
	"manufacturer" VARCHAR(255),
	"publisher" VARCHAR(255),
	"weight" FLOAT8,
	"priceWithoutTax" FLOAT8,
	"priceWithTax" FLOAT8,
	"purchasePriceWithoutTax" FLOAT8,
	"purchasePriceWithTax" FLOAT8,
	"stock" VARCHAR(255),
	"bookedOut" BOOLEAN DEFAULT FALSE,
	PRIMARY KEY ("purchasePositionId"),
	FOREIGN KEY ("purchaseId") REFERENCES purchases ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY ("productId") REFERENCES products ON UPDATE CASCADE ON DELETE RESTRICT,
	FOREIGN KEY ("stock") REFERENCES stocks ("name") ON UPDATE CASCADE ON DELETE SET NULL,
	FOREIGN KEY ("distributorId") REFERENCES clients ("clientId") ON UPDATE CASCADE ON DELETE RESTRICT
);

CREATE TABLE linked_files_purchases (
	"linkedFileId" SERIAL,
	"fileId" INTEGER,
	"description" VARCHAR(255),
	"id" INTEGER,
	PRIMARY KEY ("linkedFileId"),
	FOREIGN KEY ("fileId") REFERENCES files ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY ("id") REFERENCES purchases ("purchaseId") ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE keys_values_purchases (
	"keyValueId" SERIAL,
	"id" INTEGER,
	"key" VARCHAR(255),
	"value" VARCHAR(255),
	PRIMARY KEY("keyValueId"),
	FOREIGN KEY ("id") REFERENCES purchases ("purchaseId") ON UPDATE CASCADE ON DELETE CASCADE
);






CREATE TABLE reminders (
	"reminderId" SERIAL,
	"username" VARCHAR(64),
	"clientId" INTEGER DEFAULT NULL,
	"productId" INTEGER DEFAULT NULL,
	"orderId" INTEGER DEFAULT NULL,
	"voucherId" INTEGER DEFAULT NULL,
	"purchaseId" INTEGER DEFAULT NULL,
	"dateTime" TIMESTAMP,
	"notes" TEXT,
	PRIMARY KEY ("reminderId"),
	FOREIGN KEY ("username") REFERENCES users ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY ("clientId") REFERENCES clients ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY ("productId") REFERENCES products ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY ("orderId") REFERENCES orders ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY ("voucherId") REFERENCES vouchers ("voucherId") ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY ("purchaseId") REFERENCES purchases ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE sectors (
	"sector" VARCHAR(255) PRIMARY KEY
);

CREATE TABLE groups (
	"group" VARCHAR(255) PRIMARY KEY
);

CREATE TABLE stock_books (
	"stockBookId" SERIAL,
	"stock" VARCHAR(255),
	"product" INTEGER,
	"quantity" FLOAT8,
	"comment" TEXT,
	"username" VARCHAR(255),
	"created" TIMESTAMP DEFAULT NOW(),
	PRIMARY KEY ("stockBookId"),
	FOREIGN KEY ("stock") REFERENCES stocks ("name") ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY ("product") REFERENCES products ("productId") ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY ("username") REFERENCES users ON UPDATE CASCADE ON DELETE SET NULL
);

CREATE TABLE advanced_preferences (
	"key" VARCHAR(255) NOT NULL PRIMARY KEY,
	"value" VARCHAR(255)
);

INSERT INTO advanced_preferences VALUES ('global.year', NULL);
INSERT INTO advanced_preferences VALUES ('clients.media.default_description', NULL);
INSERT INTO advanced_preferences VALUES ('products.media.default_description', NULL);
INSERT INTO advanced_preferences VALUES ('orders.media.default_description', NULL);
INSERT INTO advanced_preferences VALUES ('purchases.media.default_description', NULL);
INSERT INTO advanced_preferences VALUES ('vouchers.media.default_description', NULL);


CREATE TABLE jasper_report (
	name VARCHAR(255) NOT NULL PRIMARY KEY
);

INSERT INTO jasper_report VALUES ('orders');
INSERT INTO jasper_report VALUES ('clients');
INSERT INTO jasper_report VALUES ('products');
INSERT INTO jasper_report VALUES ('purchases');

CREATE TABLE jasper_reports (
	report VARCHAR(255) NOT NULL,
	name VARCHAR(255) NOT NULL,
	description TEXT,
	rank INTEGER NOT NULL DEFAULT 0,
	PRIMARY KEY (report),
	FOREIGN KEY (name) REFERENCES jasper_report (name) ON UPDATE CASCADE ON DELETE CASCADE
);

INSERT INTO jasper_reports (name, report, rank) VALUES ('orders', 'Order-DinA4', 0);
INSERT INTO jasper_reports (name, report, rank) VALUES ('orders', 'Delivery-DinA4', 1);
INSERT INTO jasper_reports (name, report, rank) VALUES ('clients', 'Client-DinA4', 0);
INSERT INTO jasper_reports (name, report, rank) VALUES ('products', 'Product-DinA4', 0);
INSERT INTO jasper_reports (name, report, rank) VALUES ('purchases', 'Purchase-DinA4', 0);

CREATE TABLE reports (
	"name" VARCHAR(255),
	"content" TEXT,
	PRIMARY KEY ("name")
);

INSERT INTO reports VALUES ('BankConnection', '<table border="1">
	<tr>
		<th>Bezeichnung</th>
		<th>Bank</th>
		<th>Bankleitzahl</th>
		<th>BIC</th>
		<th>Kontonummer</th>
		<th>IBAN</th>
		<th>Inhaber</th>
	</tr>
	{d:rows}
</table>');
INSERT INTO reports VALUES ('BankConnectionRow', '<tr>
	<td>{s:description}</td>
	<td>{s:bank}</td>
	<td>{s:bankCode}</td>
	<td>{s:bic}</td>
	<td>{s:accountNumber}</td>
	<td>{s:iban}</td>
	<td>{s:bankOwner}</td>
</tr>');
INSERT INTO reports VALUES ('Client', '<h5>Kontaktinformationen</h5>

<table width="100%">
	<tr>
		<td valign="top" align="right"><b>Kundennummer:</b></td>
		<td valign="top" width="150">{s:clientId}</td>
		<td valign="top" align="right"><b>Kurzname:</b></td>
		<td valign="top" width="150">{s:shortName}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Eigene Kundennummer:</b></td>
		<td valign="top" width="150">{s:ownClientId}</td>
		<td valign="top" align="right"><b>Art:</b></td>
		<td valign="top" width="150">{s:kind}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Gruppe:</b></td>
		<td valign="top" width="150">{s:group}</td>
		<td valign="top" align="right"><b>Branche:</b></td>
		<td valign="top" width="150">{s:sector}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Rechtsform:</b></td>
		<td valign="top">{s:formOfOrganization}</td>
		<td valign="top" align="right"><b>Steuerpflichtig:</b></td>
		<td valign="top">{s:liableToTax}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Briefanrede:</b></td>
		<td valign="top">{s:circularLetter}</td>
		<td valign="top" align="right"><b>Stra&szlig;e/Hnr.:</b></td>
		<td valign="top">{s:address} {s:number}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Anrede:</b></td>
		<td valign="top">{s:salutation}</td>
		<td valign="top" align="right"><b>Plz/Ort:</b></td>
		<td valign="top">{s:zipcode} {s:city}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Titel:</b></td>
		<td valign="top">{s:title}</td>
		<td valign="top" align="right"><b>Bundesland:</b></td>
		<td valign="top">{s:federalState}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Vorname:</b></td>
		<td valign="top">{s:firstname}</td>
		<td valign="top" align="right"><b>Land:</b></td>
		<td valign="top">{s:land}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Name 1:</b></td>
		<td valign="top">{s:name1}</td>
		<td valign="top" align="right"><b>Name 3:</b></td>
		<td valign="top">{s:name3}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Name 2:</b></td>
		<td valign="top">{s:name2}</td>
		<td valign="top" align="right"><b>Name 4:</b></td>
		<td valign="top">{s:name4}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Geburtstag:</b></td>
		<td valign="top">{s:birthdayDay} {s:birthdayMonth} {s:birthdayYear}</td>
		<td valign="top" align="right"><b>Aktiv:</b></td>
		<td valign="top">{s:active}</td>
	</tr>

</table>

<u><h5>Verbindungsinformationen</h5></u>

<table width="100%">
	<tr>
		<td valign="top" align="right"><b>Telefon 1:</b></td>
		<td valign="top" width="200">{s:telephone1}</td>
		<td valign="top" align="right"><b>Mobil 1:</b></td>
		<td valign="top" width="200">{s:mobile1}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Telefon 2:</b></td>
		<td valign="top">{s:telephone2}</td>
		<td valign="top" align="right"><b>Mobil 2:</b></td>
		<td valign="top">{s:mobile2}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Telefon 3:</b></td>
		<td valign="top">{s:telephone3}</td>
		<td valign="top" align="right"><b>E-Mail 1:</b></td>
		<td valign="top">{s:email1}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Fax 1:</b></td>
		<td valign="top">{s:fax1}</td>
		<td valign="top" align="right"><b>E-Mail 2:</b></td>
		<td valign="top">{s:email2}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Fax 2:</b></td>
		<td valign="top">{s:fax2}</td>
		<td valign="top" align="right"><b>Internet 1:</b></td>
		<td valign="top">{s:internet1}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Fax 3:</b></td>
		<td valign="top">{s:fax3}</td>
		<td valign="top" align="right"><b>Internet 2:</b></td>
		<td valign="top">{s:internet2}</td>
	</tr>
</table>

<u><h5>Liquidit&auml;t</h5></u>

<table>
	<tr>
		<td valign="top" align="right" width="85"><b>Steuernummer:</b></td>
		<td valign="top">{s:taxNumber}</td>
	</tr>
	<tr>
		<td valign="top" align="right" width="85"><b>Umsatzsteuer-Identifkationsnummer:</b></td>
		<td valign="top">{s:salesTaxIdentificationNumber}</td>
	</tr>

	<tr>
		<td valign="top" align="right" width="85"><b>Rabatt:</b></td>
		<td valign="top">{s:discount} %</td>
	</tr>

	<tr>
		<td valign="top" align="right" width="85"><b>Kondition:</b></td>
		<td valign="top">{s:condition}</td>
	</tr>
</table>

<u><h5>Bankverbindungen</h5></u>

{d:bankConnections}

<u><h5>Informationen</h5></u>

{s:informations}

<u><h5>Adressen</h5></u>

{d:addresses}

<u><h5>Ansprechpartner</h5></u>

{d:contactPersons}

<u><h5>Weitere Daten</h5></u>

{d:moreData}

<u><h5>Verknüpfte Dateien</h5></u>

{d:linkedFiles}');
INSERT INTO reports VALUES ('ClientAddress', '<table width="100%">

	<tr>
		<td valign="top" align="right"><b>Beschreibung:</b></td>
		<td valign="top">{s:description}</td>
		<td valign="top" align="right"><b>Stra&szlig;e/Hnr.:</b></td>
		<td valign="top">{s:address} {s:number}</td>
	</tr>
	<tr>
		<td valign="top" align="right"><b>Anrede:</b></td>
		<td valign="top">{s:salutation}</td>
		<td valign="top" align="right"><b>Plz/Ort:</b></td>
		<td valign="top">{s:zipcode} {s:city}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Titel:</b></td>
		<td valign="top">{s:title}</td>
		<td valign="top" align="right"><b>Bundesland:</b></td>
		<td valign="top">{s:federalState}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Vorname:</b></td>
		<td valign="top">{s:firstname}</td>
		<td valign="top" align="right"><b>Land:</b></td>
		<td valign="top">{s:land}</td>
	</tr>

	<tr>
		<td valign="top" width="25%" align="right"><b>Name 1:</b></td>
		<td valign="top" width="25%">{s:name1}</td>
		<td valign="top" width="25%" align="right"><b>Name 3:</b></td>
		<td valign="top" width="25%">{s:name3}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Name 2:</b></td>
		<td valign="top">{s:name2}</td>
		<td valign="top" align="right"><b>Name 4:</b></td>
		<td valign="top">{s:name4}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Informationen:</b></td>
		<td colspan="3" valign="top">{s:information}</td>
	</tr>
</table>

<p></p>');
INSERT INTO reports VALUES ('ClientContactPerson', '<table width="100%">
	<tr>
		<td valign="top" width="25%" align="right"><b>Abteilung:</b></td>
		<td valign="top" width="25%">{s:department}</td>
		<td valign="top" width="25%" align="right"><b>Funktion:</b></td>
		<td valign="top" width="25%">{s:function}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Anrede:</b></td>
		<td valign="top">{s:salutation}</td>
		<td valign="top" align="right"><b>Telefon:</b></td>
		<td valign="top">{s:telephone}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Titel:</b></td>
		<td valign="top">{s:title}</td>
		<td valign="top" align="right"><b>Mobile:</b></td>
		<td valign="top">{s:mobile}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Vorname:</b></td>
		<td valign="top">{s:firstname}</td>
		<td valign="top" align="right"><b>Fax:</b></td>
		<td valign="top">{s:fax}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Nachname:</b></td>
		<td valign="top">{s:lastname}</td>
		<td valign="top" align="right"><b>E-Mail:</b></td>
		<td valign="top">{s:email}</td>
	</tr>

	<tr>
		<td valign="top" align="right">&nbsp;</td>
		<td valign="top">&nbsp;</td>
		<td valign="top" align="right"><b>Internet:</b></td>
		<td valign="top">{s:internet}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Informationen:</b></td>
		<td colspan="3" valign="top">{s:information}</td>
	</tr>
</table>

<p></p>');
INSERT INTO reports VALUES ('ClientHeader', '<html>
	<head>
	</head>
	<body>
		<h3>{s:name}</h3>
	</body>
</html>');
INSERT INTO reports VALUES ('ClientFooter', '<html>
	<head>
	</head>
	<body>
		<p align="center"><font size="1">Seite @PAGENUM@ von @PAGESCNT@</font></p>
	</body>
</html>');
INSERT INTO reports VALUES ('Informationen', '<h5>Informationen</h5>

{s:text}');
INSERT INTO reports VALUES ('KeyValue', '<table border="1">
	<tr>
		<th>Beschreibung</th>
		<th>Wert</th>
	</tr>
	{d:rows}
</table>');
INSERT INTO reports VALUES ('KeyValueRow', '<tr>
	<td>{s:key}</td>
	<td>{s:value}</td>
</td>');
INSERT INTO reports VALUES ('LinkedFileRow', '<b>{s:description}:</b> {s:pathAndFile}<br>');
INSERT INTO reports VALUES ('Monition', '<html>
<head>
</head>
<body>

	<br>
	<br>
	<br>
	<br>
	<table border="0" width="100%">
		<tr>
			<td colspan="3">
				<font size="1">{s:company} - {s:executive} - {s:address1} - {s:zipcode} {s:city}</font>
				<br>
			</td>
		</tr>
		<tr>
			<td valign="top" width="33%">
				<font size="1">
					<br>&nbsp;<br>
					{s:billAddress}
				</font>
			</td>
			<td valign="top" width="33%">
				<font size="1">
					<br>&nbsp;<br>
					{s:translatedBillAddress}
				</font>
			</td>
			<td valign="top" align="right" width="34%">
				<font size="1">
					<br>&nbsp;<br>
					Tel.: {s:telephone}<br>
					Fax: {s:fax}<br>
					Mail: {s:email}<br>
					Web: {s:web}<br>
					USt-IdNr: {s:salesTaxIdentificationNumber}<br>
					Steuernummer: {s:taxNumber}<br>
				</font>
				<br>
				<font size="1">
					USt-IdNr: {s:orderSalesTaxIdentificationNumber}<br>
					Steuernummer: {s:orderTaxNumber}<br>
					Rechnungsnummer: {s:invoiceId}<br>
					Rechnungsdatum: {s:invoiceDate}<br>
					Versand: {s:delivery}<br>
					Zeichen: {s:orderNumber}<br>
				</font>
			</td>
		</tr>
		<tr>
			<td colspan="3">
				<font size="6"><b>Zahlungsaufforderung - Stufe {s:monitionState}</b></font>
			</td>
		</tr>
	</table>

	<br>
	<br>

	<p>{s:prependText}</p>

	<br>
	<br>

	<table border="1" width="100%">
		<tr>
			<td width="10%" align="right"><font size="1"><b>Stufe</b></font></td>
			<td width="20%" align="left"><font size="1"><b>Datum</b></font></td>
			<td width="25%" align="right"><font size="1"><b>Rechnungsnummer</b></font></td>
			<td width="25%" align="right"><font size="1"><b>Auftragsnummer</b></font></td>
			<td width="20%" align="right"><font size="1"><b>Betrag</b></font></td>
		</tr>
		<tr>
			<td width="10%" align="right"><font size="1">{s:monitionState}</font></td>
			<td width="20%" align="left"><font size="1">{s:invoiceDate}</font></td>
			<td width="25%" align="right"><font size="1">{s:invoiceId}</font></td>
			<td width="25%" align="right"><font size="1">{s:orderId}</font></td>
			<td width="20%" align="right"><font size="1">{s:price}</font></td>
		</tr>
		<tr>
			<td width="80%" align="right" colspan="4"><font size="1">Mahngebühren:</font></td>
			<td width="20%" align="right"><font size="1">{s:monitionPrice}</font></td>
		</tr>
		<tr>
			<td width="80%" align="right" colspan="4"><font size="1"><b>Gesamt:</b></font></td>
			<td width="20%" align="right"><font size="1"><b>{s:sum}</b></font></td>
		</tr>
	</table>

	<p>{s:appendText}</p>

	<div style="page-break-before:always">

</body>
</html>');
INSERT INTO reports VALUES ('Note', '{s:note}');
INSERT INTO reports VALUES ('NoteHeader', '<html>
	<head>
	</head>
	<body>
		<h3>{s:title}</h3>
	</body>
</html>');
INSERT INTO reports VALUES ('NoteFooter', '<html>
	<head>
	</head>
	<body>
		<p align="center"><font size="1">Seite @PAGENUM@ von @PAGESCNT@</font></p>
	</body>
</html>');
INSERT INTO reports VALUES ('Order', '<table border="0" width="100%">
	<tr>
		<td colspan="8"><font size="1">{s:prepandText}</font></td>
	</tr>
	
	{d:positions}
	{d:linked}

	<tr>
		<td colspan="7" align="right"><font size="1">Summe:</font></td>
		<td align="right"><font size="1">{s:sumBeforeTax} EUR</font></td>
	</tr>
	<tr>
		<td colspan="7" align="right"><font size="1">MwSt. {s:fullTaxPercent}%:</font></td>
		<td align="right"><font size="1">{s:sumFullTax} EUR</font></td>
	</tr>
	<tr>
		<td colspan="7" align="right"><font size="1">MwSt. {s:reducedTaxPercent}%:</font></td>
		<td align="right"><font size="1">{s:sumReducedTax} EUR</font></td>
	</tr>
	<tr>
		<td colspan="7" align="right"><font size="1"><b>Betrag:</b></font></td>
		<td align="right"><font size="1"><b><u>{s:sumAfterTax} EUR</u></b></font></td>
	</tr>

	<tr>
		<td colspan="8">
			<font size="1">
				{s:condition}<br><br>
				Bezahlung per {s:payment} {s:bankData}<br><br>
				Gesamtmenge: {s:fullAmount}<br>
				Gesamtgewicht: {s:fullWeight} kg
			</font>
		</td>
	</tr>

	<tr>
		<td colspan="8"><font size="1">{s:appendText}</font></td>
	</tr>
</table>');
INSERT INTO reports VALUES ('OrderDelivery', '<table border="0" width="100%">
	<tr>
		<td colspan="4"><font size="1">{s:prepandText}</font></td>
	</tr>
	
	{d:positions}
	{d:linked}

	<tr>
		<td colspan="4">
			<font size="1">
				Gesamtmenge: {s:fullAmount}<br>
				Gesamtgewicht: {s:fullWeight} kg
			</font>
		</td>
	</tr>

	<tr>
		<td colspan="4"><font size="1">{s:appendText}</font></td>
	</tr>
</table>');
INSERT INTO reports VALUES ('OrderDeliveryHeader', '<html>
<head>
</head>
<body>

	<br>
	<br>
	<br>
	<br>
	<table border="0" width="100%">
		<tr>
			<td colspan="3">
				<font size="1">{s:company} - {s:executive} - {s:address1} - {s:zipcode} {s:city}</font>
				<br>
			</td>
		</tr>
		<tr>
			<td valign="top" width="33%">
				<font size="1">
					<br>&nbsp;<br>
					{s:shippingAddress}
				</font>
			</td>
			<td valign="top" width="33%">
				<font size="1">
					<br>&nbsp;<br>
					{s:translatedShippingAddress}
				</font>
			</td>
			<td valign="top" align="right" width="34%">
				<font size="1">
					<br>&nbsp;<br>
					Tel.: {s:telephone}<br>
					Fax: {s:fax}<br>
					Mail: {s:email}<br>
					Web: {s:web}<br>
					USt-IdNr: {s:salesTaxIdentificationNumber}<br>
					Steuernummer: {s:taxNumber}<br>
				</font>
				<br>
				<font size="1">
					Kundennummer: {s:clientId}<br>
					USt-IdNr: {s:orderSalesTaxIdentificationNumber}<br>
					Steuernummer: {s:orderTaxNumber}<br>
					Rechnungsnummer: {s:invoiceId}<br>
					Rechnungsdatum: {s:invoiceDate}<br>
					Datum: {s:when}<br>
					Versand: {s:delivery}<br>
					Zeichen: {s:orderNumber}<br>
					Eigentümer: {s:owner}<br>
				</font>
			</td>
		</tr>
		<tr>
			<td colspan="3">
				<font size="6"><b>{s:kind}: {s:orderId} {s:copy}</b></font>
			</td>
		</tr>
	</table>
	<br>
	<br>

	<table border="1" width="100%">
		<tr>
			<td width="4%" align="right"><font size="1"><b>Nr.</b></font></td>
			<td width="10%" align="right"><font size="1"><b>Menge</b></font></td>
			<td width="71%" align="left"><font size="1"><b>Produkt</b></font></td>
			<td width="15%" align="right"><font size="1"><b>Gewicht</b></font></td>
		</tr>
	</table>

</body>
</html>');
INSERT INTO reports VALUES ('OrderDeliveryFooter', '<p align="center">Seite @PAGENUM@ von @PAGESCNT@</p>');
INSERT INTO reports VALUES ('OrderDeliveryRow', '<tr>
	<td width="4%" align="right" valign="top"><font size="1">{s:position}</font></td>
	<td width="10%" align="right" valign="top"><font size="1">{s:amount} {s:package}</font></td>
	<td width="71%" align="left" valign="top"><font size="1">{s:productId}<br>{s:name}<br>{s:description}<br>{s:stock}</font></td>
	<td width="15%" align="right" valign="top"><font size="1">{s:fullWeight} kg</font></td>
</tr>
<tr>
	<td colspan="4"><hr></td>
</tr>');
INSERT INTO reports VALUES ('OrderFooter', '<p align="center"><font size="1">Bank: {s:bank}, Bankleitzahl: {s:bankCode}, Inhaber: {s:holder}, Kontonummer: {s:accountNumber}, BIC: {s:bic}, IBAN: {s:iban}</font></p>
<p align="center">Seite @PAGENUM@ von @PAGESCNT@</p>');
INSERT INTO reports VALUES ('OrderHeader', '<html>
<head>
</head>
<body>

	<br>
	<br>
	<br>
	<br>
	<table border="0" width="100%">
		<tr>
			<td colspan="3">
				<font size="1">{s:company} - {s:executive} - {s:address1} - {s:zipcode} {s:city}</font>
				<br>
			</td>
		</tr>
		<tr>
			<td valign="top" width="33%">
				<font size="1">
					<br>&nbsp;<br>
					{s:shippingAddress}
				</font>
			</td>
			<td valign="top" width="33%">
				<font size="1">
					<br>&nbsp;<br>
					{s:translatedShippingAddress}
				</font>
			</td>
			<td valign="top" align="right" width="34%">
				<font size="1">
					<br>&nbsp;<br>
					Tel.: {s:telephone}<br>
					Fax: {s:fax}<br>
					Mail: {s:email}<br>
					Web: {s:web}<br>
					USt-IdNr: {s:salesTaxIdentificationNumber}<br>
					Steuernummer: {s:taxNumber}<br>
				</font>
				<br>
				<font size="1">
					Kundennummer: {s:clientId}<br>
					USt-IdNr: {s:orderSalesTaxIdentificationNumber}<br>
					Steuernummer: {s:orderTaxNumber}<br>
					Rechnungsnummer: {s:invoiceId}<br>
					Rechnungsdatum: {s:invoiceDate}<br>
					Datum: {s:when}<br>
					Versand: {s:delivery}<br>
					Zeichen: {s:orderNumber}<br>
					Eigentümer: {s:owner}<br>
				</font>
			</td>
		</tr>
		<tr>
			<td colspan="3">
				<font size="6"><b>{s:kind}: {s:orderId} {s:copy}</b></font>
			</td>
		</tr>
	</table>

	<br>
	<br>

	<table border="1" width="100%">
		<tr>
			<td width="4%" align="right"><font size="1"><b>Nr.</b></font></td>
			<td width="10%" align="right"><font size="1"><b>Menge</b></font></td>
			<td width="13%" align="right"><font size="1"><b>Preis</b></font></td>
			<td width="15%" align="right"><font size="1"><b>Gesamt</b></font></td>
			<td width="10%" align="right"><font size="1"><b>Rabatt</b></font></td>
			<td width="15%" align="right"><font size="1"><b>Summe</b></font></td>
			<td width="13%" align="right"><font size="1"><b>Steuer</b></font></td>
			<td width="20%" align="right"><font size="1"><b>Betrag</b></font></td>
		</tr>
	</table>

</body>
</html>');
INSERT INTO reports VALUES ('OrderLinkedDeliveryRow', '<tr>
	<td colspan="3"><b>Auftrag {s:orderId} ({s:when})</b></td>
</tr>

{d:positions}');
INSERT INTO reports VALUES ('OrderLinkedRow', '<tr>
	<td colspan="8"><b>Auftrag {s:orderId} ({s:when})</b></td>
</tr>

{d:positions}');
INSERT INTO reports VALUES ('OrderRow', '<tr>
	<td colspan="8"><font size="1">{s:productId}<br>{s:name}<br>{s:description}</font></td>
</tr>
<tr>
	<td width="4%" align="right" valign="top"><font size="1">{s:position}</font></td>
	<td width="10%" align="right" valign="top"><font size="1">{s:amount} {s:package}</font></td>
	<td width="13%" align="right" valign="top"><font size="1">{s:price} EUR</font></td>
	<td width="15%" align="right" valign="top"><font size="1">{s:fullPrice} EUR</font></td>
	<td width="10%" align="right" valign="top"><font size="1">{s:reduction} %</font></td>
	<td width="15%" align="right" valign="top"><font size="1">{s:beforeTax} EUR</font></td>
	<td width="13%" align="right" valign="top"><font size="1">{s:tax} EUR</font></td>
	<td width="20%" align="right" valign="top"><font size="1">{s:afterTax} EUR {s:taxLevelString}</font></td>
</tr>
<tr>
	<td colspan="8">{s:text}<hr></td>
</tr>');
INSERT INTO reports VALUES ('Product', '<h5>Produktinformationen</h5>

<table>
	<tr>
		<td valign="top" align="right"><b>Produktnummer:</b></td>
		<td valign="top">{s:productId}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Artikelnummer:</b></td>
		<td valign="top">{s:articleNumber}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Gruppe:</b></td>
		<td valign="top">{s:group}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Einheit:</b></td>
		<td valign="top">{s:package}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Vorhanden:</b></td>
		<td valign="top">{s:available}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Lieferant:</b></td>
		<td valign="top">{s:distributor}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Hersteller:</b></td>
		<td valign="top">{s:manufacturer}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Herausgeber:</b></td>
		<td valign="top">{s:publisher}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>EAN:</b></td>
		<td valign="top">{s:ean}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Kurzname:</b></td>
		<td valign="top">{s:shortName}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Name 1:</b></td>
		<td valign="top">{s:name1}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Name 2:</b></td>
		<td valign="top">{s:name2}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Name 3:</b></td>
		<td valign="top">{s:name3}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Name 4:</b></td>
		<td valign="top">{s:name4}</td>
	</tr>
	
	<tr>
		<td valign="top" align="right"><b>ISBN 10:</b></td>
		<td valign="top">{s:isbn10}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>ISBN 13:</b></td>
		<td valign="top">{s:isbn13}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Steuer:</b></td>
		<td valign="top">{s:tax}</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Preis ohne Steuer:</b></td>
		<td valign="top" align="left">{s:priceWithoutTax} EUR</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Preis mit Steuer:</b></td>
		<td valign="top" align="left">{s:priceWithTax} EUR</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Einkaufspreis ohne Steuer:</b></td>
		<td valign="top" align="left">{s:purchasePriceWithoutTax} EUR</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Einkaufspreis mit Steuer:</b></td>
		<td valign="top" align="left">{s:purchasePriceWithTax} EUR</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Marge ohne Steuer:</b></td>
		<td valign="top" align="left">{s:marginWithoutTax} EUR</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Marge mit Steuer:</b></td>
		<td valign="top" align="left">{s:marginWithTax} EUR</td>
	</tr>

	<tr>
		<td valign="top" align="right"><b>Gewicht:</b></td>
		<td valign="top" align="left">{s:weight} kg</td>
	</tr>

	<tr>
		<td valign="top" align="right" valign="top"><b>Beschreibung:</b></td>
		<td valign="top">{s:description}</td>
	</tr>

	<tr>
		<td valign="top" align="right" valign="top"><b>Notiz:</b></td>
		<td valign="top">{s:note}</td>
	</tr>
</table>

<u><h5>Weitere Daten</h5></u>

{d:moreData}

<u><h5>Verknüpfte Dateien</h5></u>

{d:linkedFiles}');
INSERT INTO reports VALUES ('ProductHeader', '<html>
	<head>
	</head>
	<body>
		<h3>{s:name1}</h3>
	</body>
</html>');
INSERT INTO reports VALUES ('ProductFooter', '<html>
	<head>
	</head>
	<body>
		<p align="center"><font size="1">Seite @PAGENUM@ von @PAGESCNT@</font></p>
	</body>
</html>');
INSERT INTO reports VALUES ('Statistic', '{s:table}');
INSERT INTO reports VALUES ('StatisticHeader', '<html>
	<head>
	</head>
	<body>
		<table>
			<tr>
				<td align="right"><b>Kundennummer:</b></td>
				<td>{s:clientId}</td>
				<td align="right"><b>Produktnummer:</b></td>
				<td>{s:productId}</td>
			</tr>
			<tr>
				<td align="right"><b>Von:</b></td>
				<td>{s:from}</td>
				<td align="right"><b>Bis:</b></td>
				<td>{s:to}</td>
			</tr>
		</table>
	</body>
</html>');
INSERT INTO reports VALUES ('StatisticFooter', '<html>
	<head>
	</head>
	<body>
		<p align="center">Seite @PAGENUM@ von @PAGESCNT@</p>
	</body>
</html>');
INSERT INTO reports VALUES ('StockBookings', '{s:table}');
INSERT INTO reports VALUES ('StockBookingsHeader', '<html>
	<head>
	</head>
	<body>
		<h1>Lagerbuchungen {s:date}</h1>
	</body>
</html>');
INSERT INTO reports VALUES ('StockBookingsFooter', '<html>
	<head>
	</head>
	<body>
		<p align="center">Seite @PAGENUM@ von @PAGESCNT@</p>
	</body>
</html>');
INSERT INTO reports VALUES ('Voucher', '
	<u><h5>Positionen</h5></u>

	<table border="1" width="100%">
		<tr>
			<td><b>Beschreibung</b></td>
			<td><b>Artikelnummer</b></td>
			<td><b>Artikel</b></td>
			<td><b>Steuersatz</b></td>
			<td><b>Steuer</b></td>
			<td><b>Preis</b></td>
		</tr>
		{d:positions}
	</table>

	<u><h5>Beschreibung</h5></u>

	<p>{s:description}</p>

	<u><h5>Notizen</h5></u>

	<p>{s:notes}</p>

	<u><h5>Weitere Daten</h5></u>

	{d:moreData}

	<u><h5>Verknüpfte Dateien</h5></u>

	{d:linkedFiles}
');
INSERT INTO reports VALUES ('VoucherHeader', '
<html>
<head>
</head>
<body>
	<p align="center"><font size="1">Seite @PAGENUM@ von @PAGESCNT@</font></p>
	<h1>{s:company}</h1>

	<p>
		<b>Rechnungsnummer:</b> {s:invoiceId}<br />
		<b>Rechnungsdatum:</b> {s:invoiceDate}<br />
		<b>Nettopreis:</b> {s:priceWithoutTax} Euro<br />
		<b>Reduzierte Steuer:</b> {s:reducedTax} Euro<br />
		<b>Volle Steuer:</b> {s:fullTax} Euro<br />
		<b>Bruttopreis:</b> {s:priceWithTax} Euro
	</p>
</body>
</html>
');
INSERT INTO reports VALUES ('VoucherFooter', '<html>
	<head>
	</head>
	<body>
		<p align="center"><font size="1">Seite @PAGENUM@ von @PAGESCNT@</font></p>
	</body>
</html>');
INSERT INTO reports VALUES ('VoucherPosition', '<tr>
	<td valign="top">{s:description}</td>
	<td align="right" valign="top">{s:articleNumber}</td>
	<td valign="top">{s:article}</td>
	<td valign="top" align="center">{s:tax}</td>
	<td valign="top" align="right">{s:taxPrice}</td>
	<td valign="top" align="right">{s:price}</td>
</tr>');
INSERT INTO reports VALUES ('Purchase', '<html>
<head>
</head>

<body>
	
	<p>{s:textHead}</p>

	<p>&nbsp;</p>

	{d:positions}

	<table border="0" width="100%">
		<tr>
			<td align="right"><b>Gesamt: {s:sum}</b></td>
		</tr>
	</table>

	<p>&nbsp;</p>

	<p>
		<font size="1">
			Gesamtmenge: {s:fullAmount}<br>
			Gesamtgewicht: {s:fullWeight} kg
		</font>
	</p>

	<p>&nbsp;</p>

	<p>{s:textFoot}</p>

</body>
</html>');
INSERT INTO reports VALUES ('PurchaseHeader', '<html>
<head>
</head>

<body>
	<table cellspacing="0" cellpadding="0">
		<tr>
			<td><b>Rechnungsadresse</b></td>
			<td>&nbsp;&nbsp;&nbsp;</td>
			<td><b>Lieferadresse</b></td>
		</tr>
		<tr>
			<td><font size="1">{s:billingAddress}</font></td>
			<td>&nbsp;&nbsp;&nbsp;</td>
			<td><font size="1">{s:deliveryAddress}</font></td>
		</tr>
	</table>

	<p>&nbsp;</p><p>&nbsp;</p>

	<p>
		<font size="1">{s:distributorAddress}</font>
	</p>

	<p>&nbsp;</p>

	<p>
		<font size="1">Kundennummer: {s:clientId}</font>
	</p>

	<p>&nbsp;</p>

	<table width="100%" cellspacing="0" cellpadding="0">
		<tr>
			<td><font size="4"><b>Bestellung:</b> {s:generatedId}</td>
			<td align="right">{s:orderDate}</td>
		</tr>
	</table>

	<p>&nbsp;</p><p>&nbsp;</p>

	<table border="1" width="100%">
		<tr>
			<td width="4%" align="right"><font size="1"><b>Nr.</b></font></td>
			<td width="10%" align="right"><font size="1"><b>Menge</b></font></td>
			<td width="20%"><font size="1"><b>Art.-Nr.</b></font></td>
			<td width="38%"><font size="1"><b>Artikel</b></font></td>
			<td width="13%" align="right"><font size="1"><b>Preis</b></font></td>
			<td width="15%" align="right"><font size="1"><b>Gesamt</b></font></td>
		</tr>
	</table>

</body>
</html>');
INSERT INTO reports VALUES ('PurchaseRow', '<table border="0" width="100%">
	<tr>
		<td width="4%" align="right"><font size="1">{s:position}</font></td>
		<td width="10%" align="right"><font size="1">{s:amount}</font></td>
		<td width="20%"><font size="1">{s:generatedArticleNumber}</font></td>
		<td width="38%"><font size="1">{s:name}</font></td>
		<td width="13%" align="right"><font size="1">{s:price}</font></td>
		<td width="15%" align="right"><font size="1">{s:fullPrice}</font></td>
	</tr>
	<tr>
		<td colspan="6">{s:text}<hr></td>
	</tr>
</table>');
INSERT INTO reports VALUES ('PurchaseFooter', '<p align="center">Seite @PAGENUM@ von @PAGESCNT@</p>');
