#include "StockDialog.h"
#include "../lib/StockController.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(StockDialog, wxDialog)
	EVT_MENU(SAVE_EVENT, StockDialog::SaveKeyEvent)
	EVT_MENU(CANCEL_EVENT, StockDialog::CancelKeyEvent)
END_EVENT_TABLE()

StockDialog::StockDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	StockEntity *stockEntity
) {
	SetId(id);
	this->rit         = rit;
	this->stockEntity = stockEntity;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("StockDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("StockDialog"));

	mainPanel           = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer        = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	mainToolbar         = XRCCTRL(*this, "mainToolbar", Toolbar);
	cancelToolbarButton = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	saveToolbarButton   = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	nameTextCtrl        = XRCCTRL(*this, "nameTextCtrl", wxTextCtrl);
	addressTextCtrl     = XRCCTRL(*this, "addressTextCtrl", wxTextCtrl);
	numberTextCtrl      = XRCCTRL(*this, "numberTextCtrl", wxTextCtrl);
	zipcodeTextCtrl     = XRCCTRL(*this, "zipcodeTextCtrl", wxTextCtrl);
	cityTextCtrl        = XRCCTRL(*this, "cityTextCtrl", wxTextCtrl);
	commentTextCtrl     = XRCCTRL(*this, "commentTextCtrl", wxTextCtrl);
	activeCheckBox      = XRCCTRL(*this, "activeCheckBox", wxCheckBox);

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(cancelToolbarButton, StockDialog::CancelEvent);
	CONNECT_TOOLBARBUTTON(saveToolbarButton, StockDialog::SaveEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,   (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,   (int) 's',  SAVE_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	if(stockEntity) {
		nameTextCtrl->Disable();

		nameTextCtrl->SetValue(stockEntity->Get(_N("name"))->ToString());
		addressTextCtrl->SetValue(stockEntity->Get(_N("address"))->ToString());
		numberTextCtrl->SetValue(stockEntity->Get(_N("number"))->ToString());
		zipcodeTextCtrl->SetValue(stockEntity->Get(_N("zipcode"))->ToString());
		cityTextCtrl->SetValue(stockEntity->Get(_N("city"))->ToString());
		commentTextCtrl->SetValue(stockEntity->Get(_N("comment"))->ToString());
		activeCheckBox->SetValue(stockEntity->Get(_N("active"))->ToBool());
	}

	nameTextCtrl->SetFocus();

	SetSize(wxSize(450, -1));

	ICON();
}

void StockDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void StockDialog::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}

void StockDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void StockDialog::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void StockDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

void StockDialog::Save() {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to save this stock."), this))
		return;

	const wxString name(nameTextCtrl->GetValue());
	if(name.IsEmpty()) {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Please fill out the name"),
			_("Save")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
		return;
	}

	StockEntity *stockEntity = new StockEntity(rit);
	StockController stockController(rit);
	if(stockController.Exists(
		stockEntity->GetPrimaryKey(),
		name,
		this->stockEntity ? this->stockEntity->Get(_N("name"))->ToString() : wxString()
	)) {
		rit->Log(true, _N("StockDialog"), _("The stock ") + name + _(" is not saved."), _("A stock with this name already exists. Please choose another name."));
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("A stock with this name already exists. Please choose another name."),
			_("Save")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();

		delete stockEntity;
		return;
	}

	stockEntity->SetValue(_N("name"), nameTextCtrl->GetValue());
	stockEntity->SetValue(_N("address"), addressTextCtrl->GetValue());
	stockEntity->SetValue(_N("number"), numberTextCtrl->GetValue());
	stockEntity->SetValue(_N("zipcode"), zipcodeTextCtrl->GetValue());
	stockEntity->SetValue(_N("city"), cityTextCtrl->GetValue());
	stockEntity->SetValue(_N("comment"), commentTextCtrl->GetValue());
	stockEntity->SetValue(_N("active"), activeCheckBox->GetValue());

	bool ok = false;
	if(this->stockEntity) {
		ok = stockController.Update(stockEntity);
	} else {
		const wxString pk(stockEntity->GetPrimaryKey());
		stockEntity->SetPrimaryKey(wxEmptyString);
		ok = stockController.Insert(stockEntity);
		stockEntity->SetPrimaryKey(pk);
	}

	if(!ok) {
		rit->Log(true, _N("StockDialog"), _("The stock ") + name + _(" is not saved."), _("Maybe there is a database error."));
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("The stock could not be saved."),
			_("Save")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
		delete stockEntity;
	} else {
		if(this->stockEntity) {
			this->stockEntity->SetValue(_N("name"), stockEntity->Get(_N("name"))->ToString());
			this->stockEntity->SetValue(_N("address"), stockEntity->Get(_N("address"))->ToString());
			this->stockEntity->SetValue(_N("number"), stockEntity->Get(_N("number"))->ToString());
			this->stockEntity->SetValue(_N("zipcode"), stockEntity->Get(_N("zipcode"))->ToString());
			this->stockEntity->SetValue(_N("city"), stockEntity->Get(_N("city"))->ToString());
			this->stockEntity->SetValue(_N("comment"), stockEntity->Get(_N("comment"))->ToString());
			this->stockEntity->SetValue(_N("active"), stockEntity->Get(_N("active"))->ToString());
		} else {
			this->stockEntity = stockEntity;
		}
		rit->Log(false, _N("StockDialog"), _("The stock ") + name + _(" is saved."));
		EndModal(wxID_OK);
	}
}

StockEntity *StockDialog::GetStock() const {
	return stockEntity;
}
