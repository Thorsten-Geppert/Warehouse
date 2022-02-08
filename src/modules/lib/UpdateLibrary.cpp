#include "UpdateLibrary.h"
#include <wx/url.h>
#include <wx/sstream.h>
#include <DSLib.h>
#include <DSFile.h>
#include <DSPostgreSQLQuery.h>

using namespace DynSoft;

UpdateLibrary::UpdateLibrary(const wxString &url) {
	this->url = url;

	versions.Add(_N("0.2.0"));
	versions.Add(_N("1.0.0"));
	versions.Add(_N("1.0.1"));
	versions.Add(_N("1.0.2"));
	versions.Add(_N("1.0.3"));
	versions.Add(_N("1.2.0"));
	versions.Add(_N("1.4.0"));
	versions.Add(_N("1.6.0"));
	versions.Add(_N("1.6.1"));
	versions.Add(_N("1.6.2"));
	versions.Add(_N("1.8.0"));
	versions.Add(_N("1.8.1"));
	versions.Add(_N("1.8.2"));
	versions.Add(_N("1.8.3"));
	versions.Add(_N("1.8.4"));
	versions.Add(_N("1.8.5"));
	versions.Add(_N("1.8.6"));
	versions.Add(_N("1.8.7"));
	versions.Add(_N("1.8.8"));
	versions.Add(_N("1.8.9"));
	versions.Add(_N("2.0.0"));
	versions.Add(_N("3.0.0"));
	versions.Add(_N("4.0.0"));
}

UpdateType *UpdateLibrary::CheckForUpdates(
	const wxString &version,
	wxString *errorMessage
) {
	UpdateType *updateType = NULL;

	bool error = true;
	wxString message;
	wxURL connectionUrl(url);

	switch(connectionUrl.GetError()) {
		case wxURL_SNTXERR:
			message = _("The given Update URL is wrong.");
			break;
		case wxURL_NOPROTO:
			message = _("No Protocol specified. The given Update URL is wrong.");
			break;
		case wxURL_NOHOST:
			message = _("No hostname given. The given Update URL is wrong.");
			break;
		case wxURL_NOPATH:
			message = _("No path given. The given Update URL is wrong.");
			break;
		case wxURL_CONNERR:
			message = _("Connection error.");
			break;
		case wxURL_PROTOERR:
			message = _("Unknown protocol error.");
			break;
		case wxURL_NOERR:
		default:
			error = false;
			break;
	}

	wxString response;
	if(!error) {
		wxInputStream *inputStream = connectionUrl.GetInputStream();
		if(!inputStream) {
			message = _("Could not open Update URL.");
			error   = true;
		} else {
			wxStringOutputStream outputStream;
			inputStream->Read(outputStream);
			response = outputStream.GetString();
		}
	}

	if(!error) {
		wxArrayString versionAndInformation(DSLib::Split(response, '|'));
		if(versionAndInformation.GetCount() != 2) {
			message = _("Unknown update string");
			error   = true;
		} else {
			if(DSLib::IsGreaterVersion(versionAndInformation[0], version))
				updateType = new UpdateType(versionAndInformation[0], versionAndInformation[1]);
		}
	}

	if(errorMessage)
		*errorMessage = message;

	return updateType;
}

wxArrayString UpdateLibrary::GetVersions(const wxString &actualVersion) {
	wxArrayString nextVersions;

	const int position = versions.Index(actualVersion);
	if(position == wxNOT_FOUND)
		return nextVersions;
	
	const unsigned int count = versions.GetCount();
	if(static_cast<unsigned int>(position + 1) == count)
		return nextVersions;
	
	for(unsigned int i = position + 1; i < count; i++)
		nextVersions.Add(versions[i]);
	
	return nextVersions;
}

bool UpdateLibrary::GetVersion(EnterpriseEntity &enterpriseEntity, wxString *version) {
	if(!version)
		return false;

	DSPostgreSQLDatabase *db = GetDatabaseObject(enterpriseEntity);
	if(!db)
		return false;
	
	DSPostgreSQLQuery query(db);
	query.Query(_N("SELECT version FROM version LIMIT 1"));
	if(query.NextRecord())
		*version = query.GetField(_N("version")).ToString();
	query.FreeResult();

	db->Disconnect();
	delete db;

	return !version->IsEmpty();
}

bool UpdateLibrary::SetVersion(EnterpriseEntity &enterpriseEntity, const wxString &version) {
	DSPostgreSQLDatabase *db = GetDatabaseObject(enterpriseEntity);
	if(!db)
		return false;
	
	DSPostgreSQLQuery query(db);
	query.Begin();

	bool ok = query.Query(_N("DELETE FROM version"), false);

	if(ok) {
		query.Prepare(_N("INSERT INTO version VALUES (:version:)"));
		query.Bind(_N("version"), version);
		ok = query.Exec(false);
	}

	if(!ok || !query.Commit()) {
		query.Rollback();
		ok = false;
	}

	db->Disconnect();
	delete db;

	return ok;
}

DSPostgreSQLDatabase *UpdateLibrary::GetDatabaseObject(EnterpriseEntity &enterpriseEntity) {
	DSPostgreSQLDatabase *db = new DSPostgreSQLDatabase;
	db->SetHostname(enterpriseEntity.Get(_N("hostname"))->ToString());
	db->SetDatabase(enterpriseEntity.Get(_N("database"))->ToString());
	db->SetUsername(enterpriseEntity.Get(_N("username"))->ToString());
	db->SetPassword(enterpriseEntity.Get(_N("password"))->ToString());
	db->SetPort(enterpriseEntity.Get(_N("port"))->ToString());

	if(db->Connect())
		return db;

	delete db;

	return NULL;
}

SqlUpdateType *UpdateLibrary::ParseWarehouseUpdateFile(const wxString &updateFile) {
	SqlUpdateType *sqlUpdateType = NULL;

	DSFile file(updateFile, _N("r"));
	if(file.IsOpened()) {
		wxString content;
		file.ReadAll(&content);
		file.Close();

		unsigned int area = 0;
		wxString version;
		wxString description;
		wxString sql;

		const size_t length = content.Length();
		for(size_t i = 0; i < length; i++) {
			if(area == 0 && content[i] == '\n') {
				area++;
			} else if(area == 1 && i + 2 < length && content[i] == '\n' && content[i + 1] == '\n' && content[i + 2] == '\n') {
				area++;
				i += 2;
			} else {
				switch(area) {
					case 0: version     += content[i]; break;
					case 1: description += content[i]; break;
					case 2: sql         += content[i]; break;
				}
			}
		}

		wxArrayString versions(DSLib::Split(version, ' '));
		sqlUpdateType = new SqlUpdateType(
			versions,
			description,
			sql
		);
	}

	return sqlUpdateType;
}

bool UpdateLibrary::Update(EnterpriseEntity &enterpriseEntity, const wxString &version, wxTextCtrl *informationsTextCtrl) {
	bool ok = true;

	informationsTextCtrl->AppendText(_("Start update to version: ") + version);

	if(version == _N("1.0.0")) {
		ok = SetVersion(enterpriseEntity, version);
	} else if(version == _N("1.0.1")) {
		ok = SetVersion(enterpriseEntity, version);
	} else if(version == _N("1.0.2")) {
		ok = SetVersion(enterpriseEntity, version);
	} else if(version == _N("1.0.3")) {
		ok =
			AlterTable(enterpriseEntity, _N("orders"), _N("RENAME \"monitionStates\" TO \"monitionState\""))
			&&
			AlterTable(enterpriseEntity, _N("vouchers_positions"), _N("ALTER description TYPE text"))
			&&
			AlterTable(enterpriseEntity, _N("vouchers_positions"), _N("ADD COLUMN quantity integer DEFAULT 1"))
			&&
			AlterTable(enterpriseEntity, _N("common"), _N("ADD COLUMN \"monitionPrice\" FLOAT8 DEFAULT 0"));
		if(ok)
			ok = SetVersion(enterpriseEntity, version);
	} else if(version == _N("1.2.0")) {
		ok = Query(enterpriseEntity, _N(
			"CREATE TABLE stocks ("
			"	\"name\" VARCHAR(255),"
			"	\"address\" VARCHAR(255),"
			"	\"number\" VARCHAR(16),"
			"	\"zipcode\" VARCHAR(16),"
			"	\"city\" VARCHAR(255),"
			"	\"comment\" TEXT,"
			"	\"active\" BOOLEAN DEFAULT false,"
			"	PRIMARY KEY (\"name\")"
			");"
			""
			"ALTER TABLE enterprise ADD COLUMN \"mandatoryClientsOwnClientId\" BOOLEAN DEFAULT FALSE;"
			""
			"ALTER TABLE clients ADD COLUMN \"ownClientId\" VARCHAR(255);"
			"ALTER TABLE clients ADD COLUMN \"kind\" VARCHAR(255);"
			"ALTER TABLE clients ADD COLUMN \"group\" VARCHAR(255);"
			"ALTER TABLE clients ADD COLUMN \"sector\" VARCHAR(255);"
			"ALTER TABLE clients ADD COLUMN \"birthdayDay\" INTEGER;"
			"ALTER TABLE clients ADD COLUMN \"birthdayMonth\" INTEGER;"
			"ALTER TABLE clients ADD COLUMN \"birthdayYear\" INTEGER;"
			"ALTER TABLE clients ADD COLUMN \"active\" BOOLEAN DEFAULT FALSE;"
			"ALTER TABLE clients ADD COLUMN \"condition\" VARCHAR(255);"
			""
			"UPDATE clients SET active = true;"
			""
			"CREATE TABLE sectors ("
			"	\"sector\" VARCHAR(255) PRIMARY KEY"
			");"
			""
			"CREATE TABLE groups ("
			"	\"group\" VARCHAR(255) PRIMARY KEY"
			");"
			""
			"ALTER TABLE orders ADD COLUMN \"ownClientId\" VARCHAR(255) UNIQUE;"
			"ALTER TABLE orders ADD COLUMN \"owner\" VARCHAR(255);"
			""
			"CREATE TABLE stock_books ("
			"	\"stockBookId\" SERIAL,"
			"	\"stock\" VARCHAR(255),"
			"	\"product\" INTEGER,"
			"	\"quantity\" FLOAT8,"
			"	\"comment\" TEXT,"
			"	\"username\" VARCHAR(255),"
			"	\"created\" TIMESTAMP DEFAULT NOW(),"
			"	PRIMARY KEY (\"stockBookId\"),"
			"	FOREIGN KEY (\"stock\") REFERENCES stocks (\"name\") ON UPDATE CASCADE ON DELETE CASCADE,"
			"	FOREIGN KEY (\"product\") REFERENCES products (\"productId\") ON UPDATE CASCADE ON DELETE CASCADE,"
			"	FOREIGN KEY (\"username\") REFERENCES users ON UPDATE CASCADE ON DELETE SET NULL"
			");"
			""
			"ALTER TABLE orders_positions ADD COLUMN \"stock\" VARCHAR(255) REFERENCES stocks (name) ON UPDATE CASCADE ON DELETE SET NULL;"
			"ALTER TABLE orders_positions ADD COLUMN \"bookedOut\" BOOLEAN DEFAULT FALSE;"
		));
		if(ok)
			ok = SetVersion(enterpriseEntity, version);
	} else if(version == _N("1.4.0")) {
		ok = Query(enterpriseEntity, _N(
			"ALTER TABLE banks ADD COLUMN bic VARCHAR(255);"
			"ALTER TABLE bank_connections ADD bic VARCHAR(255);"
			"ALTER TABLE bank_connections ADD iban VARCHAR(255);"
			"ALTER TABLE orders ADD COLUMN bic VARCHAR(255);"
			"ALTER TABLE orders ADD COLUMN iban VARCHAR(255);"
			"ALTER TABLE enterprise ADD COLUMN bic VARCHAR(255);"
			"ALTER TABLE enterprise ADD COLUMN iban VARCHAR(255);"
		));
		if(ok)
			ok = SetVersion(enterpriseEntity, version);
	} else if(version == _N("1.6.0")) {
		ok = Update_1_6_0(enterpriseEntity);
		if(ok)
			ok = SetVersion(enterpriseEntity, version);
	} else if(version == _N("1.6.1")) {
		ok = SetVersion(enterpriseEntity, version);
	} else if(version == _N("1.6.2")) {
		ok = SetVersion(enterpriseEntity, version);
	} else if(version == _N("1.8.0")) {
		ok = Query(enterpriseEntity, _N(
			"CREATE TABLE purchases ("
			"	\"purchaseId\" SERIAL,"
			"	\"distributorId\" INTEGER,"
			"	\"distributorAddress\" TEXT,"
			"	\"clientId\" VARCHAR(255),"
			"	\"orderId\" VARCHAR(255),"
			"	\"orderDate\" DATE DEFAULT NOW(),"
			"	\"purchaser\" VARCHAR(255),"
			"	\"informations\" TEXT,"
			"	\"billingAddress\" TEXT,"
			"	\"deliveryAddress\" TEXT,"
			"	\"textHead\" TEXT,"
			"	\"textFoot\" TEXT,"
			"	\"sum\" FLOAT8 DEFAULT 0.0,"
			"	\"done\" DATE,"
			"	\"createdBy\" VARCHAR(64),"
			"	\"createdAt\" TIMESTAMP DEFAULT NOW(),"
			"	\"changedBy\" VARCHAR(64),"
			"	\"changedAt\" TIMESTAMP DEFAULT NOW(),"
			"	PRIMARY KEY (\"purchaseId\"),"
			"	FOREIGN KEY (\"distributorId\") REFERENCES clients (\"clientId\") ON UPDATE CASCADE ON DELETE RESTRICT"
			");"
			""
			"SELECT SETVAL('\"purchases_purchaseId_seq\"', 10000, true);"
			""
			"CREATE TABLE purchases_positions ("
			"	\"purchasePositionId\" SERIAL,"
			"	\"purchaseId\" INTEGER,"
			"	\"amount\" FLOAT8,"
			"	\"productId\" INTEGER,"
			"	\"text\" TEXT,"
			"	\"price\" FLOAT8,"
			"	\"fullPrice\" FLOAT8,"
			"	\"articleNumber\" VARCHAR(255),"
			"	\"ean\" VARCHAR(255),"
			"	\"shortName\" VARCHAR(255),"
			"	\"name1\" VARCHAR(255),"
			"	\"name2\" VARCHAR(255),"
			"	\"name3\" VARCHAR(255),"
			"	\"name4\" VARCHAR(255),"
			"	\"description\" TEXT,"
			"	\"note\" TEXT,"
			"	\"isbn10\" VARCHAR(255),"
			"	\"isbn13\" VARCHAR(255),"
			"	\"group\" VARCHAR(255),"
			"	\"package\" VARCHAR(255),"
			"	\"available\" BOOLEAN DEFAULT TRUE,"
			"	\"distributorId\" INTEGER,"
			"	\"manufacturer\" VARCHAR(255),"
			"	\"publisher\" VARCHAR(255),"
			"	\"weight\" FLOAT8,"
			"	\"priceWithoutTax\" FLOAT8,"
			"	\"priceWithTax\" FLOAT8,"
			"	\"purchasePriceWithoutTax\" FLOAT8,"
			"	\"purchasePriceWithTax\" FLOAT8,"
			"	\"stock\" VARCHAR(255),"
			"	\"bookedOut\" BOOLEAN DEFAULT FALSE,"
			"	PRIMARY KEY (\"purchasePositionId\"),"
			"	FOREIGN KEY (\"purchaseId\") REFERENCES purchases ON UPDATE CASCADE ON DELETE CASCADE,"
			"	FOREIGN KEY (\"productId\") REFERENCES products ON UPDATE CASCADE ON DELETE RESTRICT,"
			"	FOREIGN KEY (\"stock\") REFERENCES stocks (\"name\") ON UPDATE CASCADE ON DELETE SET NULL,"
			"	FOREIGN KEY (\"distributorId\") REFERENCES clients (\"clientId\") ON UPDATE CASCADE ON DELETE RESTRICT"
			");"
			""
			"CREATE TABLE linked_files_purchases ("
			"	\"linkedFileId\" SERIAL,"
			"	\"fileId\" INTEGER,"
			"	\"description\" VARCHAR(255),"
			"	\"id\" INTEGER,"
			"	PRIMARY KEY (\"linkedFileId\"),"
			"	FOREIGN KEY (\"fileId\") REFERENCES files ON UPDATE CASCADE ON DELETE CASCADE,"
			"	FOREIGN KEY (\"id\") REFERENCES purchases (\"purchaseId\") ON UPDATE CASCADE ON DELETE CASCADE"
			");"
			""
			"CREATE TABLE keys_values_purchases ("
			"	\"keyValueId\" SERIAL,"
			"	\"id\" INTEGER,"
			"	\"key\" VARCHAR(255),"
			"	\"value\" VARCHAR(255),"
			"	PRIMARY KEY(\"keyValueId\"),"
			"	FOREIGN KEY (\"id\") REFERENCES purchases (\"purchaseId\") ON UPDATE CASCADE ON DELETE CASCADE"
			");"
			""
			"ALTER TABLE reminders ADD COLUMN \"purchaseId\" INTEGER;"
			"ALTER TABLE reminders ADD CONSTRAINT \"reminders_purchaseId_fkey\" FOREIGN KEY (\"purchaseId\") REFERENCES purchases (\"purchaseId\") ON UPDATE CASCADE ON DELETE CASCADE;"
			""
			"ALTER TABLE products DROP COLUMN \"distributor\";"
			"ALTER TABLE products ADD COLUMN \"distributorId\" INTEGER;"
			"ALTER TABLE products ADD CONSTRAINT \"products_distributorId_fkey\" FOREIGN KEY (\"distributorId\") REFERENCES clients (\"clientId\") ON UPDATE CASCADE ON DELETE RESTRICT;"
			""
			"ALTER TABLE orders_positions DROP COLUMN \"distributor\";"
			"ALTER TABLE orders_positions ADD COLUMN \"distributorId\" INTEGER;"
			"ALTER TABLE orders_positions ADD CONSTRAINT \"orders_positions_distributorId_fkey\" FOREIGN KEY (\"distributorId\") REFERENCES clients (\"clientId\") ON UPDATE CASCADE ON DELETE RESTRICT;"
			""
			"INSERT INTO reports VALUES ('Purchase', '<html>\n"
			"<head>\n"
			"</head>\n"
			"\n"
			"<body>\n"
			"	\n"
			"	<p>{s:textHead}</p>\n"
			"\n"
			"	<p>&nbsp;</p>\n"
			"\n"
			"	{d:positions}\n"
			"\n"
			"	<table border=\"0\" width=\"100%\">\n"
			"		<tr>\n"
			"			<td align=\"right\"><b>Gesamt: {s:sum}</b></td>\n"
			"		</tr>\n"
			"	</table>\n"
			"\n"
			"	<p>&nbsp;</p>\n"
			"\n"
			"	<p>\n"
			"		<font size=\"1\">\n"
			"			Gesamtmenge: {s:fullAmount}<br>\n"
			"			Gesamtgewicht: {s:fullWeight} kg\n"
			"		</font>\n"
			"	</p>\n"
			"\n"
			"	<p>&nbsp;</p>\n"
			"\n"
			"	<p>{s:textFoot}</p>\n"
			"\n"
			"</body>\n"
			"</html>');"
			"INSERT INTO reports VALUES ('PurchaseHeader', '<html>\n"
			"<head>\n"
			"</head>\n"
			"\n"
			"<body>\n"
			"	<table cellspacing=\"0\" cellpadding=\"0\">\n"
			"		<tr>\n"
			"			<td><b>Rechnungsadresse</b></td>\n"
			"			<td>&nbsp;&nbsp;&nbsp;</td>\n"
			"			<td><b>Lieferadresse</b></td>\n"
			"		</tr>\n"
			"		<tr>\n"
			"			<td><font size=\"1\">{s:billingAddress}</font></td>\n"
			"			<td>&nbsp;&nbsp;&nbsp;</td>\n"
			"			<td><font size=\"1\">{s:deliveryAddress}</font></td>\n"
			"		</tr>\n"
			"	</table>\n"
			"\n"
			"	<p>&nbsp;</p><p>&nbsp;</p>\n"
			"\n"
			"	<p>\n"
			"		<font size=\"1\">{s:distributorAddress}</font>\n"
			"	</p>\n"
			"\n"
			"	<p>&nbsp;</p>\n"
			"\n"
			"	<p>\n"
			"		<font size=\"1\">Kundennummer: {s:clientId}</font>\n"
			"	</p>\n"
			"\n"
			"	<p>&nbsp;</p>\n"
			"\n"
			"	<table width=\"100%\" cellspacing=\"0\" cellpadding=\"0\">\n"
			"		<tr>\n"
			"			<td><font size=\"4\"><b>Bestellung:</b> {s:generatedId}</td>\n"
			"			<td align=\"right\">{s:orderDate}</td>\n"
			"		</tr>\n"
			"	</table>\n"
			"\n"
			"	<p>&nbsp;</p><p>&nbsp;</p>\n"
			"\n"
			"	<table border=\"1\" width=\"100%\">\n"
			"		<tr>\n"
			"			<td width=\"4%\" align=\"right\"><font size=\"1\"><b>Nr.</b></font></td>\n"
			"			<td width=\"10%\" align=\"right\"><font size=\"1\"><b>Menge</b></font></td>\n"
			"			<td width=\"20%\"><font size=\"1\"><b>Art.-Nr.</b></font></td>\n"
			"			<td width=\"38%\"><font size=\"1\"><b>Artikel</b></font></td>\n"
			"			<td width=\"13%\" align=\"right\"><font size=\"1\"><b>Preis</b></font></td>\n"
			"			<td width=\"15%\" align=\"right\"><font size=\"1\"><b>Gesamt</b></font></td>\n"
			"		</tr>\n"
			"	</table>\n"
			"\n"
			"</body>\n"
			"</html>');"
			"INSERT INTO reports VALUES ('PurchaseRow', '<table border=\"0\" width=\"100%\">\n"
			"	<tr>\n"
			"		<td width=\"4%\" align=\"right\"><font size=\"1\">{s:position}</font></td>\n"
			"		<td width=\"10%\" align=\"right\"><font size=\"1\">{s:amount}</font></td>\n"
			"		<td width=\"20%\"><font size=\"1\">{s:generatedArticleNumber}</font></td>\n"
			"		<td width=\"38%\"><font size=\"1\">{s:name}</font></td>\n"
			"		<td width=\"13%\" align=\"right\"><font size=\"1\">{s:price}</font></td>\n"
			"		<td width=\"15%\" align=\"right\"><font size=\"1\">{s:fullPrice}</font></td>\n"
			"	</tr>\n"
			"	<tr>\n"
			"		<td colspan=\"6\">{s:text}<hr></td>\n"
			"	</tr>\n"
			"</table>');"
			"INSERT INTO reports VALUES ('PurchaseFooter', '<p align=\"center\">Seite @PAGENUM@ von @PAGESCNT@</p>');"
		));
		if(ok)
			ok = SetVersion(enterpriseEntity, version);
	} else if(version == _N("1.8.1")) {
		ok = Query(enterpriseEntity, _N(
			"CREATE TABLE advanced_preferences ("
			"	\"key\" VARCHAR(255) NOT NULL PRIMARY KEY,"
			"	\"value\" VARCHAR(255)"
			");"
			"INSERT INTO advanced_preferences VALUES ('global.year', NULL);"
			"INSERT INTO advanced_preferences VALUES ('clients.media.default_description', NULL);"
			"INSERT INTO advanced_preferences VALUES ('products.media.default_description', NULL);"
			"INSERT INTO advanced_preferences VALUES ('orders.media.default_description', NULL);"
			"INSERT INTO advanced_preferences VALUES ('purchases.media.default_description', NULL);"
			"INSERT INTO advanced_preferences VALUES ('vouchers.media.default_description', NULL);"
		));
		if(ok)
			ok = SetVersion(enterpriseEntity, version);
	} else if(version == _N("1.8.2")) {
		ok = SetVersion(enterpriseEntity, version);
	} else if(version == _N("1.8.3")) {
		ok = SetVersion(enterpriseEntity, version);
	} else if(version == _N("1.8.4")) {
		ok = SetVersion(enterpriseEntity, version);
	} else if(version == _N("1.8.5")) {
		ok = SetVersion(enterpriseEntity, version);
	} else if(version == _N("1.8.6")) {
		ok = SetVersion(enterpriseEntity, version);
	} else if(version == _N("1.8.7")) {
		ok = SetVersion(enterpriseEntity, version);
	} else if(version == _N("1.8.8")) {
		ok = SetVersion(enterpriseEntity, version);
	} else if(version == _N("1.8.9")) {
		ok = Query(enterpriseEntity, _N(
			"UPDATE reports SET content = '"
			"	<u><h5>Positionen</h5></u>\n"
			"\n"
			"	<table border=\"1\" width=\"100%\">\n"
			"		<tr>\n"
			"			<td><b>Beschreibung</b></td>\n"
			"			<td><b>Artikelnummer</b></td>\n"
			"			<td><b>Artikel</b></td>\n"
			"			<td><b>Steuersatz</b></td>\n"
			"			<td><b>Steuer</b></td>\n"
			"			<td><b>Preis</b></td>\n"
			"		</tr>\n"
			"		{d:positions}\n"
			"	</table>\n"
			"\n"
			"	<u><h5>Beschreibung</h5></u>\n"
			"\n"
			"	<p>{s:description}</p>\n"
			"\n"
			"	<u><h5>Notizen</h5></u>\n"
			"\n"
			"	<p>{s:notes}</p>\n"
			"\n"
			"	<u><h5>Weitere Daten</h5></u>\n"
			"\n"
			"	{d:moreData}\n"
			"\n"
			"	<u><h5>Verknüpfte Dateien</h5></u>\n"
			"\n"
			"	{d:linkedFiles}\n"
			"' WHERE name = 'Voucher'"
		));
		
		if(ok) {
			ok = Query(enterpriseEntity, _N(
				"UPDATE reports SET content = '"
					"<html>"
					"<head>\n"
					"</head>\n"
					"<body>\n"
					"	<p align=\"center\"><font size=\"1\">Seite @PAGENUM@ von @PAGESCNT@</font></p>\n"
					"	<h1>{s:company}</h1>\n"
					"\n"
					"	<p>\n"
					"		<b>Rechnungsnummer:</b> {s:invoiceId}<br />\n"
					"		<b>Rechnungsdatum:</b> {s:invoiceDate}<br />\n"
					"		<b>Nettopreis:</b> {s:priceWithoutTax} Euro<br />\n"
					"		<b>Reduzierte Steuer:</b> {s:reducedTax} Euro<br />\n"
					"		<b>Volle Steuer:</b> {s:fullTax} Euro<br />\n"
					"		<b>Bruttopreis:</b> {s:priceWithTax} Euro\n"
					"	</p>\n"
					"</body>\n"
					"</html>"
				"' WHERE name = 'VoucherHeader'"
			));
			if(ok)
				ok = SetVersion(enterpriseEntity, version);
		}
	} else if(version == _N("2.0.0")) {
		ok = Query(enterpriseEntity, _N(
			"ALTER TABLE common ADD COLUMN \"reportUse\" BOOLEAN DEFAULT FALSE;"
			"ALTER TABLE common ADD COLUMN \"reportPathWindows\" TEXT;"
			"ALTER TABLE common ADD COLUMN \"reportPathUnix\" TEXT;"
			"ALTER TABLE common ADD COLUMN \"reportPathMacOS\" TEXT;"
			"ALTER TABLE common ADD COLUMN \"jreHomeWindows\" TEXT;"
			"ALTER TABLE common ADD COLUMN \"jreHomeUnix\" TEXT;"
			"ALTER TABLE common ADD COLUMN \"jreHomeMacOS\" TEXT;"
			""
			"CREATE TABLE jasper_report ("
			"	name VARCHAR(255) NOT NULL PRIMARY KEY"
			");"
			""
			"INSERT INTO jasper_report VALUES ('orders');"
			""
			"CREATE TABLE jasper_reports ("
			"	report VARCHAR(255) NOT NULL,"
			"	name VARCHAR(255) NOT NULL,"
			"	description TEXT,"
			"	rank INTEGER NOT NULL DEFAULT 0,"
			"	PRIMARY KEY (report),"
			"	FOREIGN KEY (name) REFERENCES jasper_report (name) ON UPDATE CASCADE ON DELETE CASCADE"
			");"
			""
			"INSERT INTO jasper_reports (name, report, rank) VALUES ('orders', 'Order-DinA4', 0);"
			"INSERT INTO jasper_reports (name, report, rank) VALUES ('orders', 'Delivery-DinA4', 1);"
		));
		if(ok)
			ok = SetVersion(enterpriseEntity, version);
	} else if(version == _N("3.0.0")) {
		ok = Query(enterpriseEntity, _N(
			"INSERT INTO jasper_report VALUES ('clients');"
			"INSERT INTO jasper_report VALUES ('products');"
			"INSERT INTO jasper_report VALUES ('purchases');"
			"INSERT INTO jasper_report VALUES ('vouchers');"
			"INSERT INTO jasper_reports (name, report, rank) VALUES ('clients', 'Client-DinA4', 0);"
			"INSERT INTO jasper_reports (name, report, rank) VALUES ('products', 'Product-DinA4', 0);"
			"INSERT INTO jasper_reports (name, report, rank) VALUES ('purchases', 'Purchase-DinA4', 0);"
			"INSERT INTO jasper_reports (name, report, rank) VALUES ('vouchers', 'Voucher-DinA4', 0);"
		));
		if(ok)
			ok = SetVersion(enterpriseEntity, version);
	} else if(version == _N("4.0.0")) {
		ok = SetVersion(enterpriseEntity, version);
	}

	informationsTextCtrl->AppendText(wxString(_(" - ")) + wxString((ok ? _("SUCCESS") : _("ERROR"))) + wxString(_N("\n")));

	return ok;
}

bool UpdateLibrary::AlterTable(EnterpriseEntity &enterpriseEntity, const wxString &table, const wxString &sql) {
	DSPostgreSQLDatabase *db = GetDatabaseObject(enterpriseEntity);
	if(!db)
		return false;

	DSPostgreSQLQuery query(db);
	query.Prepare(_N("ALTER TABLE \":table:\" :alter:"));
	query.Bind(_N("table"), table, false);
	query.Bind(_N("alter"), sql, false);
	const bool ok = query.Exec(false);

	db->Disconnect();
	delete db;

	return ok;
}

bool UpdateLibrary::Query(EnterpriseEntity &enterpriseEntity, const wxString &queryString) {
	DSPostgreSQLDatabase *db = GetDatabaseObject(enterpriseEntity);
	if(!db)
		return false;

	DSPostgreSQLQuery query(db);
	query.Begin();
	const bool ok = query.Query(queryString);
	if(!query.Commit())
		query.Rollback();

	db->Disconnect();
	delete db;

	return ok;
}

bool UpdateLibrary::Update_1_6_0(EnterpriseEntity &enterpriseEntity) {
	return Query(enterpriseEntity, _N(
		"ALTER TABLE bank_connections ADD COLUMN \"referenceNumber\" VARCHAR(255);\n"
		"ALTER TABLE bank_connections ADD COLUMN \"referenceDate\" TIMESTAMP DEFAULT NOW();\n"
		"ALTER TABLE bank_connections ADD COLUMN \"referenceState\" VARCHAR(255);\n"
		"ALTER TABLE bank_connections ADD COLUMN \"referenceAccount\" VARCHAR(255);\n"
		"ALTER TABLE bank_connections ADD COLUMN \"notes\" TEXT;\n"
		"ALTER TABLE bank_connections ADD COLUMN \"active\" BOOLEAN DEFAULT TRUE;\n"
		"ALTER TABLE clients ADD COLUMN \"monitionable\" BOOLEAN DEFAULT TRUE;\n"
		"CREATE TABLE reports (\n"
		"	\"name\" VARCHAR(255),\n"
		"	\"content\" TEXT,\n"
		"	PRIMARY KEY (\"name\")\n"
		");\n"
		"\n"
		"INSERT INTO reports VALUES ('BankConnection', '<table border=\"1\">\n"
		"	<tr>\n"
		"		<th>Bezeichnung</th>\n"
		"		<th>Bank</th>\n"
		"		<th>Bankleitzahl</th>\n"
		"		<th>BIC</th>\n"
		"		<th>Kontonummer</th>\n"
		"		<th>IBAN</th>\n"
		"		<th>Inhaber</th>\n"
		"	</tr>\n"
		"	{d:rows}\n"
		"</table>');\n"
		"INSERT INTO reports VALUES ('BankConnectionRow', '<tr>\n"
		"	<td>{s:description}</td>\n"
		"	<td>{s:bank}</td>\n"
		"	<td>{s:bankCode}</td>\n"
		"	<td>{s:bic}</td>\n"
		"	<td>{s:accountNumber}</td>\n"
		"	<td>{s:iban}</td>\n"
		"	<td>{s:bankOwner}</td>\n"
		"</tr>');\n"
		"INSERT INTO reports VALUES ('Client', '<h5>Kontaktinformationen</h5>\n"
		"\n"
		"<table width=\"100%\">\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Kundennummer:</b></td>\n"
		"		<td valign=\"top\" width=\"150\">{s:clientId}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Kurzname:</b></td>\n"
		"		<td valign=\"top\" width=\"150\">{s:shortName}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Eigene Kundennummer:</b></td>\n"
		"		<td valign=\"top\" width=\"150\">{s:ownClientId}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Art:</b></td>\n"
		"		<td valign=\"top\" width=\"150\">{s:kind}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Gruppe:</b></td>\n"
		"		<td valign=\"top\" width=\"150\">{s:group}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Branche:</b></td>\n"
		"		<td valign=\"top\" width=\"150\">{s:sector}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Rechtsform:</b></td>\n"
		"		<td valign=\"top\">{s:formOfOrganization}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Steuerpflichtig:</b></td>\n"
		"		<td valign=\"top\">{s:liableToTax}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Briefanrede:</b></td>\n"
		"		<td valign=\"top\">{s:circularLetter}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Stra&szlig;e/Hnr.:</b></td>\n"
		"		<td valign=\"top\">{s:address} {s:number}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Anrede:</b></td>\n"
		"		<td valign=\"top\">{s:salutation}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Plz/Ort:</b></td>\n"
		"		<td valign=\"top\">{s:zipcode} {s:city}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Titel:</b></td>\n"
		"		<td valign=\"top\">{s:title}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Bundesland:</b></td>\n"
		"		<td valign=\"top\">{s:federalState}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Vorname:</b></td>\n"
		"		<td valign=\"top\">{s:firstname}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Land:</b></td>\n"
		"		<td valign=\"top\">{s:land}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Name 1:</b></td>\n"
		"		<td valign=\"top\">{s:name1}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Name 3:</b></td>\n"
		"		<td valign=\"top\">{s:name3}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Name 2:</b></td>\n"
		"		<td valign=\"top\">{s:name2}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Name 4:</b></td>\n"
		"		<td valign=\"top\">{s:name4}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Geburtstag:</b></td>\n"
		"		<td valign=\"top\">{s:birthdayDay} {s:birthdayMonth} {s:birthdayYear}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Aktiv:</b></td>\n"
		"		<td valign=\"top\">{s:active}</td>\n"
		"	</tr>\n"
		"\n"
		"</table>\n"
		"\n"
		"<u><h5>Verbindungsinformationen</h5></u>\n"
		"\n"
		"<table width=\"100%\">\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Telefon 1:</b></td>\n"
		"		<td valign=\"top\" width=\"200\">{s:telephone1}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Mobil 1:</b></td>\n"
		"		<td valign=\"top\" width=\"200\">{s:mobile1}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Telefon 2:</b></td>\n"
		"		<td valign=\"top\">{s:telephone2}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Mobil 2:</b></td>\n"
		"		<td valign=\"top\">{s:mobile2}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Telefon 3:</b></td>\n"
		"		<td valign=\"top\">{s:telephone3}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>E-Mail 1:</b></td>\n"
		"		<td valign=\"top\">{s:email1}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Fax 1:</b></td>\n"
		"		<td valign=\"top\">{s:fax1}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>E-Mail 2:</b></td>\n"
		"		<td valign=\"top\">{s:email2}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Fax 2:</b></td>\n"
		"		<td valign=\"top\">{s:fax2}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Internet 1:</b></td>\n"
		"		<td valign=\"top\">{s:internet1}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Fax 3:</b></td>\n"
		"		<td valign=\"top\">{s:fax3}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Internet 2:</b></td>\n"
		"		<td valign=\"top\">{s:internet2}</td>\n"
		"	</tr>\n"
		"</table>\n"
		"\n"
		"<u><h5>Liquidit&auml;t</h5></u>\n"
		"\n"
		"<table>\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\" width=\"85\"><b>Steuernummer:</b></td>\n"
		"		<td valign=\"top\">{s:taxNumber}</td>\n"
		"	</tr>\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\" width=\"85\"><b>Umsatzsteuer-Identifkationsnummer:</b></td>\n"
		"		<td valign=\"top\">{s:salesTaxIdentificationNumber}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\" width=\"85\"><b>Rabatt:</b></td>\n"
		"		<td valign=\"top\">{s:discount} %</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\" width=\"85\"><b>Kondition:</b></td>\n"
		"		<td valign=\"top\">{s:condition}</td>\n"
		"	</tr>\n"
		"</table>\n"
		"\n"
		"<u><h5>Bankverbindungen</h5></u>\n"
		"\n"
		"{d:bankConnections}\n"
		"\n"
		"<u><h5>Informationen</h5></u>\n"
		"\n"
		"{s:informations}\n"
		"\n"
		"<u><h5>Adressen</h5></u>\n"
		"\n"
		"{d:addresses}\n"
		"\n"
		"<u><h5>Ansprechpartner</h5></u>\n"
		"\n"
		"{d:contactPersons}\n"
		"\n"
		"<u><h5>Weitere Daten</h5></u>\n"
		"\n"
		"{d:moreData}\n"
		"\n"
		"<u><h5>Verknüpfte Dateien</h5></u>\n"
		"\n"
		"{d:linkedFiles}');\n"
		"INSERT INTO reports VALUES ('ClientAddress', '<table width=\"100%\">\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Beschreibung:</b></td>\n"
		"		<td valign=\"top\">{s:description}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Stra&szlig;e/Hnr.:</b></td>\n"
		"		<td valign=\"top\">{s:address} {s:number}</td>\n"
		"	</tr>\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Anrede:</b></td>\n"
		"		<td valign=\"top\">{s:salutation}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Plz/Ort:</b></td>\n"
		"		<td valign=\"top\">{s:zipcode} {s:city}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Titel:</b></td>\n"
		"		<td valign=\"top\">{s:title}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Bundesland:</b></td>\n"
		"		<td valign=\"top\">{s:federalState}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Vorname:</b></td>\n"
		"		<td valign=\"top\">{s:firstname}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Land:</b></td>\n"
		"		<td valign=\"top\">{s:land}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" width=\"25%\" align=\"right\"><b>Name 1:</b></td>\n"
		"		<td valign=\"top\" width=\"25%\">{s:name1}</td>\n"
		"		<td valign=\"top\" width=\"25%\" align=\"right\"><b>Name 3:</b></td>\n"
		"		<td valign=\"top\" width=\"25%\">{s:name3}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Name 2:</b></td>\n"
		"		<td valign=\"top\">{s:name2}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Name 4:</b></td>\n"
		"		<td valign=\"top\">{s:name4}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Informationen:</b></td>\n"
		"		<td colspan=\"3\" valign=\"top\">{s:information}</td>\n"
		"	</tr>\n"
		"</table>\n"
		"\n"
		"<p></p>');\n"
		"INSERT INTO reports VALUES ('ClientContactPerson', '<table width=\"100%\">\n"
		"	<tr>\n"
		"		<td valign=\"top\" width=\"25%\" align=\"right\"><b>Abteilung:</b></td>\n"
		"		<td valign=\"top\" width=\"25%\">{s:department}</td>\n"
		"		<td valign=\"top\" width=\"25%\" align=\"right\"><b>Funktion:</b></td>\n"
		"		<td valign=\"top\" width=\"25%\">{s:function}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Anrede:</b></td>\n"
		"		<td valign=\"top\">{s:salutation}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Telefon:</b></td>\n"
		"		<td valign=\"top\">{s:telephone}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Titel:</b></td>\n"
		"		<td valign=\"top\">{s:title}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Mobile:</b></td>\n"
		"		<td valign=\"top\">{s:mobile}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Vorname:</b></td>\n"
		"		<td valign=\"top\">{s:firstname}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Fax:</b></td>\n"
		"		<td valign=\"top\">{s:fax}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Nachname:</b></td>\n"
		"		<td valign=\"top\">{s:lastname}</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>E-Mail:</b></td>\n"
		"		<td valign=\"top\">{s:email}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\">&nbsp;</td>\n"
		"		<td valign=\"top\">&nbsp;</td>\n"
		"		<td valign=\"top\" align=\"right\"><b>Internet:</b></td>\n"
		"		<td valign=\"top\">{s:internet}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Informationen:</b></td>\n"
		"		<td colspan=\"3\" valign=\"top\">{s:information}</td>\n"
		"	</tr>\n"
		"</table>\n"
		"\n"
		"<p></p>');\n"
		"INSERT INTO reports VALUES ('ClientHeader', '<html>\n"
		"	<head>\n"
		"	</head>\n"
		"	<body>\n"
		"		<h3>{s:name}</h3>\n"
		"	</body>\n"
		"</html>');\n"
		"INSERT INTO reports VALUES ('ClientFooter', '<html>\n"
		"	<head>\n"
		"	</head>\n"
		"	<body>\n"
		"		<p align=\"center\"><font size=\"1\">Seite @PAGENUM@ von @PAGESCNT@</font></p>\n"
		"	</body>\n"
		"</html>');\n"
		"INSERT INTO reports VALUES ('Informationen', '<h5>Informationen</h5>\n"
		"\n"
		"{s:text}');\n"
		"INSERT INTO reports VALUES ('KeyValue', '<table border=\"1\">\n"
		"	<tr>\n"
		"		<th>Beschreibung</th>\n"
		"		<th>Wert</th>\n"
		"	</tr>\n"
		"	{d:rows}\n"
		"</table>');\n"
		"INSERT INTO reports VALUES ('KeyValueRow', '<tr>\n"
		"	<td>{s:key}</td>\n"
		"	<td>{s:value}</td>\n"
		"</td>');\n"
		"INSERT INTO reports VALUES ('LinkedFileRow', '<b>{s:description}:</b> {s:pathAndFile}<br>');\n"
		"INSERT INTO reports VALUES ('Monition', '<html>\n"
		"<head>\n"
		"</head>\n"
		"<body>\n"
		"\n"
		"	<br>\n"
		"	<br>\n"
		"	<br>\n"
		"	<br>\n"
		"	<table border=\"0\" width=\"100%\">\n"
		"		<tr>\n"
		"			<td colspan=\"3\">\n"
		"				<font size=\"1\">{s:company} - {s:executive} - {s:address1} - {s:zipcode} {s:city}</font>\n"
		"				<br>\n"
		"			</td>\n"
		"		</tr>\n"
		"		<tr>\n"
		"			<td valign=\"top\" width=\"33%\">\n"
		"				<font size=\"1\">\n"
		"					<br>&nbsp;<br>\n"
		"					{s:billAddress}\n"
		"				</font>\n"
		"			</td>\n"
		"			<td valign=\"top\" width=\"33%\">\n"
		"				<font size=\"1\">\n"
		"					<br>&nbsp;<br>\n"
		"					{s:translatedBillAddress}\n"
		"				</font>\n"
		"			</td>\n"
		"			<td valign=\"top\" align=\"right\" width=\"34%\">\n"
		"				<font size=\"1\">\n"
		"					<br>&nbsp;<br>\n"
		"					Tel.: {s:telephone}<br>\n"
		"					Fax: {s:fax}<br>\n"
		"					Mail: {s:email}<br>\n"
		"					Web: {s:web}<br>\n"
		"					USt-IdNr: {s:salesTaxIdentificationNumber}<br>\n"
		"					Steuernummer: {s:taxNumber}<br>\n"
		"				</font>\n"
		"				<br>\n"
		"				<font size=\"1\">\n"
		"					USt-IdNr: {s:orderSalesTaxIdentificationNumber}<br>\n"
		"					Steuernummer: {s:orderTaxNumber}<br>\n"
		"					Rechnungsnummer: {s:invoiceId}<br>\n"
		"					Rechnungsdatum: {s:invoiceDate}<br>\n"
		"					Versand: {s:delivery}<br>\n"
		"					Zeichen: {s:orderNumber}<br>\n"
		"				</font>\n"
		"			</td>\n"
		"		</tr>\n"
		"		<tr>\n"
		"			<td colspan=\"3\">\n"
		"				<font size=\"6\"><b>Zahlungsaufforderung - Stufe {s:monitionState}</b></font>\n"
		"			</td>\n"
		"		</tr>\n"
		"	</table>\n"
		"\n"
		"	<br>\n"
		"	<br>\n"
		"\n"
		"	<p>{s:prependText}</p>\n"
		"\n"
		"	<br>\n"
		"	<br>\n"
		"\n"
		"	<table border=\"1\" width=\"100%\">\n"
		"		<tr>\n"
		"			<td width=\"10%\" align=\"right\"><font size=\"1\"><b>Stufe</b></font></td>\n"
		"			<td width=\"20%\" align=\"left\"><font size=\"1\"><b>Datum</b></font></td>\n"
		"			<td width=\"25%\" align=\"right\"><font size=\"1\"><b>Rechnungsnummer</b></font></td>\n"
		"			<td width=\"25%\" align=\"right\"><font size=\"1\"><b>Auftragsnummer</b></font></td>\n"
		"			<td width=\"20%\" align=\"right\"><font size=\"1\"><b>Betrag</b></font></td>\n"
		"		</tr>\n"
		"		<tr>\n"
		"			<td width=\"10%\" align=\"right\"><font size=\"1\">{s:monitionState}</font></td>\n"
		"			<td width=\"20%\" align=\"left\"><font size=\"1\">{s:invoiceDate}</font></td>\n"
		"			<td width=\"25%\" align=\"right\"><font size=\"1\">{s:invoiceId}</font></td>\n"
		"			<td width=\"25%\" align=\"right\"><font size=\"1\">{s:orderId}</font></td>\n"
		"			<td width=\"20%\" align=\"right\"><font size=\"1\">{s:price}</font></td>\n"
		"		</tr>\n"
		"		<tr>\n"
		"			<td width=\"80%\" align=\"right\" colspan=\"4\"><font size=\"1\">Mahngebühren:</font></td>\n"
		"			<td width=\"20%\" align=\"right\"><font size=\"1\">{s:monitionPrice}</font></td>\n"
		"		</tr>\n"
		"		<tr>\n"
		"			<td width=\"80%\" align=\"right\" colspan=\"4\"><font size=\"1\"><b>Gesamt:</b></font></td>\n"
		"			<td width=\"20%\" align=\"right\"><font size=\"1\"><b>{s:sum}</b></font></td>\n"
		"		</tr>\n"
		"	</table>\n"
		"\n"
		"	<p>{s:appendText}</p>\n"
		"\n"
		"	<div style=\"page-break-before:always\">\n"
		"\n"
		"</body>\n"
		"</html>');\n"
		"INSERT INTO reports VALUES ('Note', '{s:note}');\n"
		"INSERT INTO reports VALUES ('NoteHeader', '<html>\n"
		"	<head>\n"
		"	</head>\n"
		"	<body>\n"
		"		<h3>{s:title}</h3>\n"
		"	</body>\n"
		"</html>');\n"
		"INSERT INTO reports VALUES ('NoteFooter', '<html>\n"
		"	<head>\n"
		"	</head>\n"
		"	<body>\n"
		"		<p align=\"center\"><font size=\"1\">Seite @PAGENUM@ von @PAGESCNT@</font></p>\n"
		"	</body>\n"
		"</html>');\n"
		"INSERT INTO reports VALUES ('Order', '<table border=\"0\" width=\"100%\">\n"
		"	<tr>\n"
		"		<td colspan=\"8\"><font size=\"1\">{s:prepandText}</font></td>\n"
		"	</tr>\n"
		"	\n"
		"	{d:positions}\n"
		"	{d:linked}\n"
		"\n"
		"	<tr>\n"
		"		<td colspan=\"7\" align=\"right\"><font size=\"1\">Summe:</font></td>\n"
		"		<td align=\"right\"><font size=\"1\">{s:sumBeforeTax} EUR</font></td>\n"
		"	</tr>\n"
		"	<tr>\n"
		"		<td colspan=\"7\" align=\"right\"><font size=\"1\">MwSt. {s:fullTaxPercent}%:</font></td>\n"
		"		<td align=\"right\"><font size=\"1\">{s:sumFullTax} EUR</font></td>\n"
		"	</tr>\n"
		"	<tr>\n"
		"		<td colspan=\"7\" align=\"right\"><font size=\"1\">MwSt. {s:reducedTaxPercent}%:</font></td>\n"
		"		<td align=\"right\"><font size=\"1\">{s:sumReducedTax} EUR</font></td>\n"
		"	</tr>\n"
		"	<tr>\n"
		"		<td colspan=\"7\" align=\"right\"><font size=\"1\"><b>Betrag:</b></font></td>\n"
		"		<td align=\"right\"><font size=\"1\"><b><u>{s:sumAfterTax} EUR</u></b></font></td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td colspan=\"8\">\n"
		"			<font size=\"1\">\n"
		"				{s:condition}<br><br>\n"
		"				Bezahlung per {s:payment} {s:bankData}<br><br>\n"
		"				Gesamtmenge: {s:fullAmount}<br>\n"
		"				Gesamtgewicht: {s:fullWeight} kg\n"
		"			</font>\n"
		"		</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td colspan=\"8\"><font size=\"1\">{s:appendText}</font></td>\n"
		"	</tr>\n"
		"</table>');\n"
		"INSERT INTO reports VALUES ('OrderDelivery', '<table border=\"0\" width=\"100%\">\n"
		"	<tr>\n"
		"		<td colspan=\"4\"><font size=\"1\">{s:prepandText}</font></td>\n"
		"	</tr>\n"
		"	\n"
		"	{d:positions}\n"
		"	{d:linked}\n"
		"\n"
		"	<tr>\n"
		"		<td colspan=\"4\">\n"
		"			<font size=\"1\">\n"
		"				Gesamtmenge: {s:fullAmount}<br>\n"
		"				Gesamtgewicht: {s:fullWeight} kg\n"
		"			</font>\n"
		"		</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td colspan=\"4\"><font size=\"1\">{s:appendText}</font></td>\n"
		"	</tr>\n"
		"</table>');\n"
		"INSERT INTO reports VALUES ('OrderDeliveryHeader', '<html>\n"
		"<head>\n"
		"</head>\n"
		"<body>\n"
		"\n"
		"	<br>\n"
		"	<br>\n"
		"	<br>\n"
		"	<br>\n"
		"	<table border=\"0\" width=\"100%\">\n"
		"		<tr>\n"
		"			<td colspan=\"3\">\n"
		"				<font size=\"1\">{s:company} - {s:executive} - {s:address1} - {s:zipcode} {s:city}</font>\n"
		"				<br>\n"
		"			</td>\n"
		"		</tr>\n"
		"		<tr>\n"
		"			<td valign=\"top\" width=\"33%\">\n"
		"				<font size=\"1\">\n"
		"					<br>&nbsp;<br>\n"
		"					{s:shippingAddress}\n"
		"				</font>\n"
		"			</td>\n"
		"			<td valign=\"top\" width=\"33%\">\n"
		"				<font size=\"1\">\n"
		"					<br>&nbsp;<br>\n"
		"					{s:translatedShippingAddress}\n"
		"				</font>\n"
		"			</td>\n"
		"			<td valign=\"top\" align=\"right\" width=\"34%\">\n"
		"				<font size=\"1\">\n"
		"					<br>&nbsp;<br>\n"
		"					Tel.: {s:telephone}<br>\n"
		"					Fax: {s:fax}<br>\n"
		"					Mail: {s:email}<br>\n"
		"					Web: {s:web}<br>\n"
		"					USt-IdNr: {s:salesTaxIdentificationNumber}<br>\n"
		"					Steuernummer: {s:taxNumber}<br>\n"
		"				</font>\n"
		"				<br>\n"
		"				<font size=\"1\">\n"
		"					Kundennummer: {s:clientId}<br>\n"
		"					USt-IdNr: {s:orderSalesTaxIdentificationNumber}<br>\n"
		"					Steuernummer: {s:orderTaxNumber}<br>\n"
		"					Rechnungsnummer: {s:invoiceId}<br>\n"
		"					Rechnungsdatum: {s:invoiceDate}<br>\n"
		"					Datum: {s:when}<br>\n"
		"					Versand: {s:delivery}<br>\n"
		"					Zeichen: {s:orderNumber}<br>\n"
		"					Eigentümer: {s:owner}<br>\n"
		"				</font>\n"
		"			</td>\n"
		"		</tr>\n"
		"		<tr>\n"
		"			<td colspan=\"3\">\n"
		"				<font size=\"6\"><b>{s:kind}: {s:orderId} {s:copy}</b></font>\n"
		"			</td>\n"
		"		</tr>\n"
		"	</table>\n"
		"	<br>\n"
		"	<br>\n"
		"\n"
		"	<table border=\"1\" width=\"100%\">\n"
		"		<tr>\n"
		"			<td width=\"4%\" align=\"right\"><font size=\"1\"><b>Nr.</b></font></td>\n"
		"			<td width=\"10%\" align=\"right\"><font size=\"1\"><b>Menge</b></font></td>\n"
		"			<td width=\"71%\" align=\"left\"><font size=\"1\"><b>Produkt</b></font></td>\n"
		"			<td width=\"15%\" align=\"right\"><font size=\"1\"><b>Gewicht</b></font></td>\n"
		"		</tr>\n"
		"	</table>\n"
		"\n"
		"</body>\n"
		"</html>');\n"
		"INSERT INTO reports VALUES ('OrderDeliveryFooter', '<p align=\"center\">Seite @PAGENUM@ von @PAGESCNT@</p>');\n"
		"INSERT INTO reports VALUES ('OrderDeliveryRow', '<tr>\n"
		"	<td width=\"4%\" align=\"right\" valign=\"top\"><font size=\"1\">{s:position}</font></td>\n"
		"	<td width=\"10%\" align=\"right\" valign=\"top\"><font size=\"1\">{s:amount} {s:package}</font></td>\n"
		"	<td width=\"71%\" align=\"left\" valign=\"top\"><font size=\"1\">{s:productId}<br>{s:name}<br>{s:description}<br>{s:stock}</font></td>\n"
		"	<td width=\"15%\" align=\"right\" valign=\"top\"><font size=\"1\">{s:fullWeight} kg</font></td>\n"
		"</tr>\n"
		"<tr>\n"
		"	<td colspan=\"4\"><hr></td>\n"
		"</tr>');\n"
		"INSERT INTO reports VALUES ('OrderFooter', '<p align=\"center\"><font size=\"1\">Bank: {s:bank}, Bankleitzahl: {s:bankCode}, Inhaber: {s:holder}, Kontonummer: {s:accountNumber}, BIC: {s:bic}, IBAN: {s:iban}</font></p>\n"
		"<p align=\"center\">Seite @PAGENUM@ von @PAGESCNT@</p>');\n"
		"INSERT INTO reports VALUES ('OrderHeader', '<html>\n"
		"<head>\n"
		"</head>\n"
		"<body>\n"
		"\n"
		"	<br>\n"
		"	<br>\n"
		"	<br>\n"
		"	<br>\n"
		"	<table border=\"0\" width=\"100%\">\n"
		"		<tr>\n"
		"			<td colspan=\"3\">\n"
		"				<font size=\"1\">{s:company} - {s:executive} - {s:address1} - {s:zipcode} {s:city}</font>\n"
		"				<br>\n"
		"			</td>\n"
		"		</tr>\n"
		"		<tr>\n"
		"			<td valign=\"top\" width=\"33%\">\n"
		"				<font size=\"1\">\n"
		"					<br>&nbsp;<br>\n"
		"					{s:shippingAddress}\n"
		"				</font>\n"
		"			</td>\n"
		"			<td valign=\"top\" width=\"33%\">\n"
		"				<font size=\"1\">\n"
		"					<br>&nbsp;<br>\n"
		"					{s:translatedShippingAddress}\n"
		"				</font>\n"
		"			</td>\n"
		"			<td valign=\"top\" align=\"right\" width=\"34%\">\n"
		"				<font size=\"1\">\n"
		"					<br>&nbsp;<br>\n"
		"					Tel.: {s:telephone}<br>\n"
		"					Fax: {s:fax}<br>\n"
		"					Mail: {s:email}<br>\n"
		"					Web: {s:web}<br>\n"
		"					USt-IdNr: {s:salesTaxIdentificationNumber}<br>\n"
		"					Steuernummer: {s:taxNumber}<br>\n"
		"				</font>\n"
		"				<br>\n"
		"				<font size=\"1\">\n"
		"					Kundennummer: {s:clientId}<br>\n"
		"					USt-IdNr: {s:orderSalesTaxIdentificationNumber}<br>\n"
		"					Steuernummer: {s:orderTaxNumber}<br>\n"
		"					Rechnungsnummer: {s:invoiceId}<br>\n"
		"					Rechnungsdatum: {s:invoiceDate}<br>\n"
		"					Datum: {s:when}<br>\n"
		"					Versand: {s:delivery}<br>\n"
		"					Zeichen: {s:orderNumber}<br>\n"
		"					Eigentümer: {s:owner}<br>\n"
		"				</font>\n"
		"			</td>\n"
		"		</tr>\n"
		"		<tr>\n"
		"			<td colspan=\"3\">\n"
		"				<font size=\"6\"><b>{s:kind}: {s:orderId} {s:copy}</b></font>\n"
		"			</td>\n"
		"		</tr>\n"
		"	</table>\n"
		"\n"
		"	<br>\n"
		"	<br>\n"
		"\n"
		"	<table border=\"1\" width=\"100%\">\n"
		"		<tr>\n"
		"			<td width=\"4%\" align=\"right\"><font size=\"1\"><b>Nr.</b></font></td>\n"
		"			<td width=\"10%\" align=\"right\"><font size=\"1\"><b>Menge</b></font></td>\n"
		"			<td width=\"13%\" align=\"right\"><font size=\"1\"><b>Preis</b></font></td>\n"
		"			<td width=\"15%\" align=\"right\"><font size=\"1\"><b>Gesamt</b></font></td>\n"
		"			<td width=\"10%\" align=\"right\"><font size=\"1\"><b>Rabatt</b></font></td>\n"
		"			<td width=\"15%\" align=\"right\"><font size=\"1\"><b>Summe</b></font></td>\n"
		"			<td width=\"13%\" align=\"right\"><font size=\"1\"><b>Steuer</b></font></td>\n"
		"			<td width=\"20%\" align=\"right\"><font size=\"1\"><b>Betrag</b></font></td>\n"
		"		</tr>\n"
		"	</table>\n"
		"\n"
		"</body>\n"
		"</html>');\n"
		"INSERT INTO reports VALUES ('OrderLinkedDeliveryRow', '<tr>\n"
		"	<td colspan=\"3\"><b>Auftrag {s:orderId} ({s:when})</b></td>\n"
		"</tr>\n"
		"\n"
		"{d:positions}');\n"
		"INSERT INTO reports VALUES ('OrderLinkedRow', '<tr>\n"
		"	<td colspan=\"8\"><b>Auftrag {s:orderId} ({s:when})</b></td>\n"
		"</tr>\n"
		"\n"
		"{d:positions}');\n"
		"INSERT INTO reports VALUES ('OrderRow', '<tr>\n"
		"	<td colspan=\"8\"><font size=\"1\">{s:productId}<br>{s:name}<br>{s:description}</font></td>\n"
		"</tr>\n"
		"<tr>\n"
		"	<td width=\"4%\" align=\"right\" valign=\"top\"><font size=\"1\">{s:position}</font></td>\n"
		"	<td width=\"10%\" align=\"right\" valign=\"top\"><font size=\"1\">{s:amount} {s:package}</font></td>\n"
		"	<td width=\"13%\" align=\"right\" valign=\"top\"><font size=\"1\">{s:price} EUR</font></td>\n"
		"	<td width=\"15%\" align=\"right\" valign=\"top\"><font size=\"1\">{s:fullPrice} EUR</font></td>\n"
		"	<td width=\"10%\" align=\"right\" valign=\"top\"><font size=\"1\">{s:reduction} %</font></td>\n"
		"	<td width=\"15%\" align=\"right\" valign=\"top\"><font size=\"1\">{s:beforeTax} EUR</font></td>\n"
		"	<td width=\"13%\" align=\"right\" valign=\"top\"><font size=\"1\">{s:tax} EUR</font></td>\n"
		"	<td width=\"20%\" align=\"right\" valign=\"top\"><font size=\"1\">{s:afterTax} EUR {s:taxLevelString}</font></td>\n"
		"</tr>\n"
		"<tr>\n"
		"	<td colspan=\"8\">{s:text}<hr></td>\n"
		"</tr>');\n"
		"INSERT INTO reports VALUES ('Product', '<h5>Produktinformationen</h5>\n"
		"\n"
		"<table>\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Produktnummer:</b></td>\n"
		"		<td valign=\"top\">{s:productId}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Artikelnummer:</b></td>\n"
		"		<td valign=\"top\">{s:articleNumber}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Gruppe:</b></td>\n"
		"		<td valign=\"top\">{s:group}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Einheit:</b></td>\n"
		"		<td valign=\"top\">{s:package}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Vorhanden:</b></td>\n"
		"		<td valign=\"top\">{s:available}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Lieferant:</b></td>\n"
		"		<td valign=\"top\">{s:distributor}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Hersteller:</b></td>\n"
		"		<td valign=\"top\">{s:manufacturer}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Herausgeber:</b></td>\n"
		"		<td valign=\"top\">{s:publisher}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>EAN:</b></td>\n"
		"		<td valign=\"top\">{s:ean}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Kurzname:</b></td>\n"
		"		<td valign=\"top\">{s:shortName}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Name 1:</b></td>\n"
		"		<td valign=\"top\">{s:name1}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Name 2:</b></td>\n"
		"		<td valign=\"top\">{s:name2}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Name 3:</b></td>\n"
		"		<td valign=\"top\">{s:name3}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Name 4:</b></td>\n"
		"		<td valign=\"top\">{s:name4}</td>\n"
		"	</tr>\n"
		"	\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>ISBN 10:</b></td>\n"
		"		<td valign=\"top\">{s:isbn10}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>ISBN 13:</b></td>\n"
		"		<td valign=\"top\">{s:isbn13}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Steuer:</b></td>\n"
		"		<td valign=\"top\">{s:tax}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Preis ohne Steuer:</b></td>\n"
		"		<td valign=\"top\" align=\"left\">{s:priceWithoutTax} EUR</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Preis mit Steuer:</b></td>\n"
		"		<td valign=\"top\" align=\"left\">{s:priceWithTax} EUR</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Einkaufspreis ohne Steuer:</b></td>\n"
		"		<td valign=\"top\" align=\"left\">{s:purchasePriceWithoutTax} EUR</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Einkaufspreis mit Steuer:</b></td>\n"
		"		<td valign=\"top\" align=\"left\">{s:purchasePriceWithTax} EUR</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Marge ohne Steuer:</b></td>\n"
		"		<td valign=\"top\" align=\"left\">{s:marginWithoutTax} EUR</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Marge mit Steuer:</b></td>\n"
		"		<td valign=\"top\" align=\"left\">{s:marginWithTax} EUR</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\"><b>Gewicht:</b></td>\n"
		"		<td valign=\"top\" align=\"left\">{s:weight} kg</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\" valign=\"top\"><b>Beschreibung:</b></td>\n"
		"		<td valign=\"top\">{s:description}</td>\n"
		"	</tr>\n"
		"\n"
		"	<tr>\n"
		"		<td valign=\"top\" align=\"right\" valign=\"top\"><b>Notiz:</b></td>\n"
		"		<td valign=\"top\">{s:note}</td>\n"
		"	</tr>\n"
		"</table>\n"
		"\n"
		"<u><h5>Weitere Daten</h5></u>\n"
		"\n"
		"{d:moreData}\n"
		"\n"
		"<u><h5>Verknüpfte Dateien</h5></u>\n"
		"\n"
		"{d:linkedFiles}');\n"
		"INSERT INTO reports VALUES ('ProductHeader', '<html>\n"
		"	<head>\n"
		"	</head>\n"
		"	<body>\n"
		"		<h3>{s:name1}</h3>\n"
		"	</body>\n"
		"</html>');\n"
		"INSERT INTO reports VALUES ('ProductFooter', '<html>\n"
		"	<head>\n"
		"	</head>\n"
		"	<body>\n"
		"		<p align=\"center\"><font size=\"1\">Seite @PAGENUM@ von @PAGESCNT@</font></p>\n"
		"	</body>\n"
		"</html>');\n"
		"INSERT INTO reports VALUES ('Statistic', '{s:table}');\n"
		"INSERT INTO reports VALUES ('StatisticHeader', '<html>\n"
		"	<head>\n"
		"	</head>\n"
		"	<body>\n"
		"		<table>\n"
		"			<tr>\n"
		"				<td align=\"right\"><b>Kundennummer:</b></td>\n"
		"				<td>{s:clientId}</td>\n"
		"				<td align=\"right\"><b>Produktnummer:</b></td>\n"
		"				<td>{s:productId}</td>\n"
		"			</tr>\n"
		"			<tr>\n"
		"				<td align=\"right\"><b>Von:</b></td>\n"
		"				<td>{s:from}</td>\n"
		"				<td align=\"right\"><b>Bis:</b></td>\n"
		"				<td>{s:to}</td>\n"
		"			</tr>\n"
		"		</table>\n"
		"	</body>\n"
		"</html>');\n"
		"INSERT INTO reports VALUES ('StatisticFooter', '<html>\n"
		"	<head>\n"
		"	</head>\n"
		"	<body>\n"
		"		<p align=\"center\">Seite @PAGENUM@ von @PAGESCNT@</p>\n"
		"	</body>\n"
		"</html>');\n"
		"INSERT INTO reports VALUES ('StockBookings', '{s:table}');\n"
		"INSERT INTO reports VALUES ('StockBookingsHeader', '<html>\n"
		"	<head>\n"
		"	</head>\n"
		"	<body>\n"
		"		<h1>Lagerbuchungen {s:date}</h1>\n"
		"	</body>\n"
		"</html>');\n"
		"INSERT INTO reports VALUES ('StockBookingsFooter', '<html>\n"
		"	<head>\n"
		"	</head>\n"
		"	<body>\n"
		"		<p align=\"center\">Seite @PAGENUM@ von @PAGESCNT@</p>\n"
		"	</body>\n"
		"</html>');\n"
		"INSERT INTO reports VALUES ('Voucher', '<u><h5>Positionen</h5></u>\n"
		"\n"
		"<table border=\"1\" width=\"100%\">\n"
		"	<tr>\n"
		"		<td><b>Beschreibung</b></td>\n"
		"		<td><b>Artikelnummer</b></td>\n"
		"		<td><b>Artikel</b></td>\n"
		"		<td><b>Steuersatz</b></td>\n"
		"		<td><b>Steuer</b></td>\n"
		"		<td><b>Preis</b></td>\n"
		"	</tr>\n"
		"	{d:positions}\n"
		"</table>\n"
		"\n"
		"<u><h5>Weitere Daten</h5></u>\n"
		"\n"
		"{d:moreData}\n"
		"\n"
		"<u><h5>Verknüpfte Dateien</h5></u>\n"
		"\n"
		"{d:linkedFiles}');\n"
		"INSERT INTO reports VALUES ('VoucherHeader', '<html>\n"
		"	<head>\n"
		"	</head>\n"
		"	<body>\n"
		"		<p align=\"center\"><font size=\"1\">Seite @PAGENUM@ von @PAGESCNT@</font></p>\n"
		"	</body>\n"
		"</html>');\n"
		"INSERT INTO reports VALUES ('VoucherFooter', '<html>\n"
		"	<head>\n"
		"	</head>\n"
		"	<body>\n"
		"		<p align=\"center\"><font size=\"1\">Seite @PAGENUM@ von @PAGESCNT@</font></p>\n"
		"	</body>\n"
		"</html>');\n"
		"INSERT INTO reports VALUES ('VoucherPosition', '<tr>\n"
		"	<td valign=\"top\">{s:description}</td>\n"
		"	<td align=\"right\" valign=\"top\">{s:articleNumber}</td>\n"
		"	<td valign=\"top\">{s:article}</td>\n"
		"	<td valign=\"top\" align=\"center\">{s:tax}</td>\n"
		"	<td valign=\"top\" align=\"right\">{s:taxPrice}</td>\n"
		"	<td valign=\"top\" align=\"right\">{s:price}</td>\n"
		"</tr>');\n"
	));
}
