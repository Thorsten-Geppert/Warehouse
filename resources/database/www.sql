CREATE TABLE online_contact_messages (
	id SERIAL,
	salutation VARCHAR(255),
	title VARCHAR(255),
	firstname VARCHAR(255),
	lastname VARCHAR(255),
	company VARCHAR(255),
	street VARCHAR(255),
	zipcode VARCHAR(16),
	city VARCHAR(255),
	telephone VARCHAR(255),
	fax VARCHAR(255),
	email VARCHAR(255),
	internet VARCHAR(255),
	subject VARCHAR(255),
	message TEXT,
	PRIMARY KEY (id)
);
