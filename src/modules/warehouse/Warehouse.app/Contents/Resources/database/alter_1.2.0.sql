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

ALTER TABLE enterprise ADD COLUMN "mandatoryClientsOwnClientId" BOOLEAN DEFAULT FALSE;

ALTER TABLE clients ADD COLUMN "ownClientId" VARCHAR(255);
ALTER TABLE clients ADD COLUMN "kind" VARCHAR(255);
ALTER TABLE clients ADD COLUMN "group" VARCHAR(255);
ALTER TABLE clients ADD COLUMN "sector" VARCHAR(255);
ALTER TABLE clients ADD COLUMN "birthdayDay" INTEGER;
ALTER TABLE clients ADD COLUMN "birthdayMonth" INTEGER;
ALTER TABLE clients ADD COLUMN "birthdayYear" INTEGER;
ALTER TABLE clients ADD COLUMN "active" BOOLEAN DEFAULT FALSE;
ALTER TABLE clients ADD COLUMN "condition" VARCHAR(255);

UPDATE clients SET active = true;

CREATE TABLE sectors (
	"sector" VARCHAR(255) PRIMARY KEY
);

CREATE TABLE groups (
	"group" VARCHAR(255) PRIMARY KEY
);

ALTER TABLE orders ADD COLUMN "ownClientId" VARCHAR(255) UNIQUE;
ALTER TABLE orders ADD COLUMN "owner" VARCHAR(255);

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

ALTER TABLE orders_positions ADD COLUMN "stock" VARCHAR(255) REFERENCES stocks (name) ON UPDATE CASCADE ON DELETE SET NULL;
ALTER TABLE orders_positions ADD COLUMN "bookedOut" BOOLEAN DEFAULT FALSE;
