#include "DynSoftContact.h"
#include <DSCasts.h>
#include <fcgi_stdio.h>
#include "Configuration.h"
#include "../lib/ContactEntity.h"
#include "../lib/ContactController.h"
#include "../lib/BugEntity.h"
#include "../lib/BugController.h"

DynSoftContact::DynSoftContact() {
	db = NULL;
}

DynSoftContact::~DynSoftContact() {
	if(db && db->IsConnected()) {
		db->Disconnect();
		delete db;
		db = NULL;
	}
}

int DynSoftContact::Init() {
	db = new DSPostgreSQLDatabase;
	db->SetHostname(HOSTNAME);
	db->SetDatabase(DATABASE);
	db->SetUsername(USERNAME);
	db->SetPassword(PASSWORD);
	db->SetPort(PORT);

	if(!db->Connect())
		return 1;

	return 0;
}

int DynSoftContact::Run() {
	if(!db->IsConnected())
		db->Connect();

	if(!db->IsConnected()) {
		printf("Could not reconnect to database. Stopping program. Please contact us");
		return 2;
	}

	const wxString kind(cgi.rq[_N("kind")]);
	if(kind == _N("Contact")) {
		ContactEntity entity(db);

		entity.Set(_N("software"), cgi.rq[_N("software")]);
		entity.Set(_N("version"), cgi.rq[_N("version")]);
		entity.Set(_N("kind"), kind);
		entity.Set(_N("yourName"), cgi.rq[_N("yourName")]);
		entity.Set(_N("email"), cgi.rq[_N("email")]);
		entity.Set(_N("subject"), cgi.rq[_N("subject")]);
		entity.Set(_N("message"), cgi.rq[_N("message")]);

		ContactController controller(db);
		if(controller.Insert(&entity))
			printf("OK");
		else
			printf("Database (inserting) error");
	} else if(kind == _N("Bug")) {
		BugEntity entity(db);

		entity.Set(_N("software"), cgi.rq[_N("software")]);
		entity.Set(_N("version"), cgi.rq[_N("version")]);
		entity.Set(_N("kind"), kind);
		entity.Set(_N("yourName"), cgi.rq[_N("yourName")]);
		entity.Set(_N("email"), cgi.rq[_N("email")]);
		entity.Set(_N("bug"), cgi.rq[_N("bug")]);
		entity.Set(_N("module"), cgi.rq[_N("module")]);
		entity.Set(_N("priority"), DSCasts::ToUnsignedInt(cgi.rq[_N("priority")]));
		entity.Set(_N("description"), cgi.rq[_N("description")]);

		BugController controller(db);
		if(controller.Insert(&entity))
			printf("OK");
		else
			printf("Database (inserting) error");
	} else {
		printf("Unknown kind.");
	}

	return 0;
}

IMPLEMENT_WEB(DynSoftContact, contact)
