CREATE TABLE enterprises (
	"id" SERIAL,
	"company" VARCHAR(255),
	"hostname" VARCHAR(255),
	"database" VARCHAR(255),
	"username" VARCHAR(255),
	"password" VARCHAR(255),
	"port" INTEGER,
	PRIMARY KEY ("id")
);
