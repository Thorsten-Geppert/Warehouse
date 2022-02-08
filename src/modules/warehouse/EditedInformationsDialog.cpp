#include "EditedInformationsDialog.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(EditedInformationsDialog, wxDialog)
END_EVENT_TABLE()

EditedInformationsDialog::EditedInformationsDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	InformationEntity *informationEntity
) : bind(
	informationEntity
) {
	SetId(id);
	this->rit = rit;
	this->informationEntity = informationEntity;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("EditedInformationsDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("EditedInformationsDialog"));

	mainPanel         = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer      = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	createdByTextCtrl = XRCCTRL(*this, "createdByTextCtrl", wxTextCtrl);
	createdAtTextCtrl = XRCCTRL(*this, "createdAtTextCtrl", wxTextCtrl);
	changedByTextCtrl = XRCCTRL(*this, "changedByTextCtrl", wxTextCtrl);
	changedAtTextCtrl = XRCCTRL(*this, "changedAtTextCtrl", wxTextCtrl);

	mainBoxSizer->SetSizeHints(this);
	SetSize(wxSize(250, -1));
	
	bind.Bind(_N("createdBy"), createdByTextCtrl);
	bind.Bind(_N("createdAt"), createdAtTextCtrl);
	bind.Bind(_N("changedBy"), changedByTextCtrl);
	bind.Bind(_N("changedAt"), changedAtTextCtrl);

	Update();

	ICON();
}

void EditedInformationsDialog::Update() {
	if(informationEntity && informationEntity->Get(informationEntity->GetPrimaryKey())->ToUnsignedInt() != 0) {
		bind.Push();

		createdAtTextCtrl->SetValue(createdAtTextCtrl->GetValue().Mid(0, createdAtTextCtrl->GetValue().Find(_N("."))));
		changedAtTextCtrl->SetValue(changedAtTextCtrl->GetValue().Mid(0, changedAtTextCtrl->GetValue().Find(_N("."))));
	}
}

void EditedInformationsDialog::Get(bool show) {
	if(show) {
		wxWindow *parent = GetParent();
		if(parent) {
			int x = 0, y = 0, w = 0, h = 0, a = 0, b = 0, s = GetSize().GetWidth(); // Wollte auch mal daemlich Variablennamen benutzen (=
			wxDisplaySize(&a, &b);
			parent->GetPosition(&x, &y);
			parent->GetSize(&w, &h);

			x += w + 5;
			if(x + s >= a)
				x -= s;
			
			SetPosition(wxPoint(x, y));
		}
	}

	Show(show);
}
