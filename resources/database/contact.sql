CREATE TABLE contacts (
	"id" SERIAL,
	"software" VARCHAR(255),
	"version" VARCHAR(16),
	"kind" VARCHAR(255),
	"yourName" VARCHAR(255),
	"email" VARCHAR(255),
	"subject" VARCHAR(255),
	"message" TEXT,
	PRIMARY KEY ("id")
);

CREATE TABLE bugs (
	"id" SERIAL,
	"software" VARCHAR(255),
	"version" VARCHAR(16),
	"kind" VARCHAR(255),
	"yourName" VARCHAR(255),
	"email" VARCHAR(255),
	"bug" VARCHAR(255),
	"module" VARCHAR(255),
	"priority" INTEGER,
	"description" TEXT,
	PRIMARY KEY ("id")
);
