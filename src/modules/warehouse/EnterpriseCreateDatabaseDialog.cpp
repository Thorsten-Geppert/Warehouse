#include "EnterpriseCreateDatabaseDialog.h"
#include <DSCasts.h>
#include <DSPostgreSQLDatabase.h>
#include <DSTemplate.h>
#include "../lib/EnterprisePreferencesController.h"
#include "../lib/EnterprisePreferencesEntity.h"
#include "../lib/CommonPreferencesController.h"
#include "../lib/CommonPreferencesEntity.h"
#include "../lib/DatabaseLibrary.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(EnterpriseCreateDatabaseDialog, wxDialog)
	EVT_MENU(CANCEL_EVENT, EnterpriseCreateDatabaseDialog::CancelKeyEvent)
END_EVENT_TABLE()

EnterpriseCreateDatabaseDialog::EnterpriseCreateDatabaseDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	const wxString &company,
	DatabaseType *databaseType,
	const wxString &sql,
	const bool showErrorMessage
) {
	SetId(id);
	this->rit                     = rit;
	this->enterprisePreferencesId = 0;
	this->commonPreferencesId     = 0;
	this->databaseType            = databaseType;
	this->sql                     = sql;
	this->showErrorMessage        = showErrorMessage;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("EnterpriseCreateDatabaseDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("EnterpriseCreateDatabaseDialog"));

	mainPanel                            = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer                         = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	runToolbarButton                     = XRCCTRL(*this, "runToolbarButton", ToolbarButton);
	cancelToolbarButton                  = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	companyTextCtrl                      = XRCCTRL(*this, "companyTextCtrl", wxTextCtrl);
	executiveTextCtrl                    = XRCCTRL(*this, "executiveTextCtrl", wxTextCtrl);
	address1TextCtrl                     = XRCCTRL(*this, "address1TextCtrl", wxTextCtrl);
	address2TextCtrl                     = XRCCTRL(*this, "address2TextCtrl", wxTextCtrl);
	zipcodeTextCtrl                      = XRCCTRL(*this, "zipcodeTextCtrl", wxTextCtrl);
	cityTextCtrl                         = XRCCTRL(*this, "cityTextCtrl", wxTextCtrl);
	salesTaxIdentificationNumberTextCtrl = XRCCTRL(*this, "salesTaxIdentificationNumberTextCtrl", wxTextCtrl);
	taxNumberTextCtrl                    = XRCCTRL(*this, "taxNumberTextCtrl", wxTextCtrl);
	salesTaxIndicatorTextCtrl            = XRCCTRL(*this, "salesTaxIndicatorTextCtrl", wxTextCtrl);
	telephoneTextCtrl                    = XRCCTRL(*this, "telephoneTextCtrl", wxTextCtrl);
	faxTextCtrl                          = XRCCTRL(*this, "faxTextCtrl", wxTextCtrl);
	emailTextCtrl                        = XRCCTRL(*this, "emailTextCtrl", wxTextCtrl);
	webTextCtrl                          = XRCCTRL(*this, "webTextCtrl", wxTextCtrl);
	bankTextCtrl                         = XRCCTRL(*this, "bankTextCtrl", wxTextCtrl);
	bankCodeTextCtrl                     = XRCCTRL(*this, "bankCodeTextCtrl", DynSoft::DSTextCtrl);
	holderTextCtrl                       = XRCCTRL(*this, "holderTextCtrl", wxTextCtrl);
	accountNumberTextCtrl                = XRCCTRL(*this, "accountNumberTextCtrl", DynSoft::DSTextCtrl);
	bicTextCtrl                          = XRCCTRL(*this, "bicTextCtrl", DynSoft::DSTextCtrl);
	ibanTextCtrl                         = XRCCTRL(*this, "ibanTextCtrl", DynSoft::DSTextCtrl);
	minorValueAddedTaxTextCtrl           = XRCCTRL(*this, "minorValueAddedTaxTextCtrl", DynSoft::DSTextCtrl);
	majorValueAddedTaxTextCtrl           = XRCCTRL(*this, "majorValueAddedTaxTextCtrl", DynSoft::DSTextCtrl);
	clientIdBeginTextCtrl                = XRCCTRL(*this, "clientIdBeginTextCtrl", DynSoft::DSTextCtrl);
	productIdBeginTextCtrl               = XRCCTRL(*this, "productIdBeginTextCtrl", DynSoft::DSTextCtrl);
	orderIdBeginTextCtrl                 = XRCCTRL(*this, "orderIdBeginTextCtrl", DynSoft::DSTextCtrl);
	invoiceIdBeginTextCtrl               = XRCCTRL(*this, "invoiceIdBeginTextCtrl", DynSoft::DSTextCtrl);
	creditIdBeginTextCtrl                = XRCCTRL(*this, "creditIdBeginTextCtrl", DynSoft::DSTextCtrl);
	purchaseIdBeginTextCtrl              = XRCCTRL(*this, "purchaseIdBeginTextCtrl", DynSoft::DSTextCtrl);

	bankCodeTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	accountNumberTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	minorValueAddedTaxTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);
	majorValueAddedTaxTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);
	clientIdBeginTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	productIdBeginTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	orderIdBeginTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	invoiceIdBeginTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	creditIdBeginTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	purchaseIdBeginTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);

	companyTextCtrl->SetValue(company);

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(cancelToolbarButton, EnterpriseCreateDatabaseDialog::CancelEvent);
	CONNECT_TOOLBARBUTTON(runToolbarButton, EnterpriseCreateDatabaseDialog::RunEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,   (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,   (int) 'r',  RUN_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	ICON();
}

void EnterpriseCreateDatabaseDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void EnterpriseCreateDatabaseDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void EnterpriseCreateDatabaseDialog::RunEvent(wxMouseEvent &event) {
	Run();
	SKIP();
}

void EnterpriseCreateDatabaseDialog::RunKeyEvent(wxCommandEvent &event) {
	Run();
}

void EnterpriseCreateDatabaseDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

void EnterpriseCreateDatabaseDialog::Run() {
	const unsigned int clientStartSequence   = DSCasts::ToUnsignedInt(clientIdBeginTextCtrl->GetValue());
	const unsigned int productStartSequence  = DSCasts::ToUnsignedInt(productIdBeginTextCtrl->GetValue());
	const unsigned int orderStartSequence    = DSCasts::ToUnsignedInt(orderIdBeginTextCtrl->GetValue());
	const unsigned int invoiceStartSequence  = DSCasts::ToUnsignedInt(invoiceIdBeginTextCtrl->GetValue());
	const unsigned int creditStartSequence   = DSCasts::ToUnsignedInt(creditIdBeginTextCtrl->GetValue());
	const unsigned int purchaseStartSequence = DSCasts::ToUnsignedInt(purchaseIdBeginTextCtrl->GetValue());

	if(clientStartSequence == 0 || productStartSequence == 0 || orderStartSequence == 0 || invoiceStartSequence == 0 || creditStartSequence == 0 || purchaseIdBeginTextCtrl == 0) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The starting ids must be greater than 0."),
			_("Create database")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	EnterprisePreferencesEntity enterprisePreferencesEntity(rit->GetDatabase());

	enterprisePreferencesEntity.SetValue(_N("id"), enterprisePreferencesId);
	enterprisePreferencesEntity.SetValue(_N("company"), companyTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("executive"), executiveTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("address1"), address1TextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("address2"), address2TextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("zipcode"), zipcodeTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("city"), cityTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("salesTaxIdentificationNumber"), salesTaxIdentificationNumberTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("taxNumber"), taxNumberTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("salesTaxIndicator"), salesTaxIndicatorTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("telephone"), telephoneTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("fax"), faxTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("email"), emailTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("web"), webTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("bank"), bankTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("bankCode"), bankCodeTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("holder"), holderTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("accountNumber"), accountNumberTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("bic"), bicTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("iban"), ibanTextCtrl->GetValue());

	if(
		enterprisePreferencesEntity.Get(_N("company"))->ToString().IsEmpty() ||
		enterprisePreferencesEntity.Get(_N("address1"))->ToString().IsEmpty() ||
		enterprisePreferencesEntity.Get(_N("zipcode"))->ToString().IsEmpty() ||
		enterprisePreferencesEntity.Get(_N("city"))->ToString().IsEmpty() ||
		enterprisePreferencesEntity.Get(_N("bank"))->ToString().IsEmpty() ||
		enterprisePreferencesEntity.Get(_N("holder"))->ToString().IsEmpty() ||
		enterprisePreferencesEntity.Get(_N("bic"))->ToString().IsEmpty() ||
		enterprisePreferencesEntity.Get(_N("iban"))->ToString().IsEmpty()
	) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please fill out the following fields: company, address1, zipcode, city, sales tax identification number, bank, bic, iban and holder."),
			_("Create database")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	CommonPreferencesEntity commonPreferencesEntity(rit->GetDatabase());

	commonPreferencesEntity.SetValue(_N("id"), commonPreferencesId);
	commonPreferencesEntity.SetValue(_N("minorValueAddedTax"), DSCasts::ToDouble(minorValueAddedTaxTextCtrl->GetValue()));
	commonPreferencesEntity.SetValue(_N("majorValueAddedTax"), DSCasts::ToDouble(majorValueAddedTaxTextCtrl->GetValue()));

	if(
		commonPreferencesEntity.Get(_N("minorValueAddedTax"))->ToString().IsEmpty() ||
		commonPreferencesEntity.Get(_N("majorValueAddedTax"))->ToString().IsEmpty()
	) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please fill out the following fields: minor value, major value."),
			_("Create database")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	wxBeginBusyCursor();

	DSTemplate sqlTemplate;
	sqlTemplate.SetContent(sql);

	sqlTemplate.SetVariable(_N("clientStartSequence"),   DSCasts::ToString(clientStartSequence));
	sqlTemplate.SetVariable(_N("productStartSequence"),  DSCasts::ToString(productStartSequence));
	sqlTemplate.SetVariable(_N("orderStartSequence"),    DSCasts::ToString(orderStartSequence));
	sqlTemplate.SetVariable(_N("invoiceStartSequence"),  DSCasts::ToString(invoiceStartSequence));
	sqlTemplate.SetVariable(_N("creditStartSequence"),   DSCasts::ToString(creditStartSequence));
	sqlTemplate.SetVariable(_N("purchaseStartSequence"), DSCasts::ToString(purchaseStartSequence));

	sqlTemplate.Parse();

	wxString message;
	DatabaseLibrary databaseLibrary(rit);
	switch(databaseLibrary.CreateDatabase(*databaseType, sqlTemplate.Get())) {
		case DATABASE_LIBRARY_OK:
			break;
		case DATABASE_LIBRARY_ERROR_CONNECTION:
			message = _("Could not connect to database postgres or template1. Could not create database.");
			break;
		case DATABASE_LIBRARY_ERROR_CREATION:
			message = _("Could not create database. Unknown error.");
			break;
		case DATABASE_LIBRARY_ERROR_CONNECT:
			message = _("Database was created, but there was an error on importing structure because the connection to the new database could not be established. You cannot use this database.");
			break;
		case DATABASE_LIBRARY_ERROR_INSERT:
			message = _("Database was created, but there was an error on importing structure. You cannot use this database.");
			break;
		case DATABASE_LIBRARY_ERROR_EXISTS:
			message = _("Database exists.");
			break;
		default:
			message = _("Unknown error. Could not create database.");
			break;
	}

	DSPostgreSQLDatabase db;
	if(message.IsEmpty()) {
		db.SetHostname(databaseType->GetHostname());
		db.SetDatabase(databaseType->GetDatabase());
		db.SetUsername(databaseType->GetUsername());
		db.SetPassword(databaseType->GetPassword());
		db.SetPort(databaseType->GetPort());

		if(!db.Connect())
			message = _("Database was created, but could not connect to database. The preferences coud not been saved.");
	}

	if(message.IsEmpty()) {
		enterprisePreferencesEntity.SetDatabase(&db);
		EnterprisePreferencesController enterprisePreferencesController(&db);
		if(!enterprisePreferencesController.InsertOrUpdate(&enterprisePreferencesEntity))
			message = _("Database was created, but preferences could not been saved.");
	}

	if(message.IsEmpty()) {
		commonPreferencesEntity.SetDatabase(&db);
		CommonPreferencesController commonPreferencesController(&db);
		if(!commonPreferencesController.InsertOrUpdate(&commonPreferencesEntity))
			message = _("Database was created, but preferences could not been saved.");
	}

	if(message.IsEmpty())
		db.Disconnect();
	
	wxEndBusyCursor();

	if(showErrorMessage || !message.IsEmpty()) {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			message.IsEmpty() ? wxString(_("Database was created. Please press save or test connection.")) : message,
			_("Create database")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
	}

	if(message.IsEmpty())
		EndModal(wxID_OK);
}
