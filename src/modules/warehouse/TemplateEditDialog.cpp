#include "TemplateEditDialog.h"
#include "resources/images/WarehouseIcon.xpm"
#include "Configuration.h"
#include "../lib/TemplateEntity.h"
#include "../lib/TemplateController.h"

BEGIN_EVENT_TABLE(TemplateEditDialog, wxDialog)
	EVT_MENU(CLOSE_EVENT, TemplateEditDialog::CancelKeyEvent)
	EVT_MENU(SAVE_EVENT, TemplateEditDialog::SaveKeyEvent)
END_EVENT_TABLE()

TemplateEditDialog::TemplateEditDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	unsigned int templateId
) {
	SetId(id);
	this->rit = rit;
	this->templateId = templateId;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("TemplateEditDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("TemplateEditDialog"));

	mainPanel           = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer        = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	mainToolbar         = XRCCTRL(*this, "mainToolbar", Toolbar);
	cancelToolbarButton = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	saveToolbarButton   = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	nameTextCtrl        = XRCCTRL(*this, "nameTextCtrl", wxTextCtrl);
	contentTextCtrl     = XRCCTRL(*this, "contentTextCtrl", wxTextCtrl);

	mainBoxSizer->SetSizeHints(this);
	SetSize(wxSize(800, 600));

	CONNECT_TOOLBARBUTTON(cancelToolbarButton, TemplateEditDialog::CancelEvent);
	CONNECT_TOOLBARBUTTON(saveToolbarButton, TemplateEditDialog::SaveEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 's',  SAVE_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	ICON();

	// Fill
	if(templateId != 0) {
		TemplateController templateController(rit);
		templateController.Get(DSCasts::ToString(templateId));
		if(templateController.Run()) {
			TemplateEntity templateEntity(rit);
			templateController.Get(&templateEntity);
			templateController.Reset();

			oldName = templateEntity.Get(_N("name"))->ToString();
			nameTextCtrl->SetValue(oldName);
			contentTextCtrl->SetValue(templateEntity.Get(_N("content"))->ToString());
		}
	}
}

void TemplateEditDialog::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}

void TemplateEditDialog::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void TemplateEditDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void TemplateEditDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void TemplateEditDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

void TemplateEditDialog::Save() {
	if(nameTextCtrl->GetValue().IsEmpty()) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please fill in a name."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	TemplateController templateController(rit);
	if(templateController.Exists(
		nameTextCtrl->GetValue(),
		oldName
	)) {
		rit->Log(true, _N("TemplateEditDialog"), _("Text template could not been saved. An entry with this name exists."));
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("An entry with this name already exists. Please choose another name."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	TemplateEntity templateEntity(rit);
	templateEntity.SetValue(_N("templateId"), templateId);
	templateEntity.SetValue(_N("name"), nameTextCtrl->GetValue());
	templateEntity.SetValue(_N("content"), contentTextCtrl->GetValue());

	bool ok = false;

	if(templateId == 0)
		ok = templateController.Insert(&templateEntity);
	else
		ok = templateController.Update(&templateEntity);
	
	if(!ok) {
		rit->Log(true, _N("TemplateEditDialog"), _("Text template could not been saved. Database error."), templateEntity.Get(_N("name"))->ToString());
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Could not save entry. Database error."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	} else {
		rit->Log(false, _N("TemplateEditDialog"), _("Text template was saved."), templateEntity.Get(_N("name"))->ToString());
	}

	EndModal(wxID_OK);
}
