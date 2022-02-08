#include "TemplateSelectDialog.h"
#include "resources/images/WarehouseIcon.xpm"
#include "Configuration.h"
#include "../lib/TemplateController.h"

BEGIN_EVENT_TABLE(TemplateSelectDialog, wxDialog)
	EVT_MENU(CLOSE_EVENT, TemplateSelectDialog::CancelKeyEvent)
	EVT_MENU(USE_EVENT, TemplateSelectDialog::UseKeyEvent)
	EVT_LISTBOX(XRCID("textTemplateListBox"), TemplateSelectDialog::SelectEvent)
	EVT_LISTBOX_DCLICK(XRCID("textTemplateListBox"), TemplateSelectDialog::UseKeyEvent)
END_EVENT_TABLE()

TemplateSelectDialog::TemplateSelectDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	wxTextCtrl *textCtrl
) {
	SetId(id);
	this->rit      = rit;
	this->textCtrl = textCtrl;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("TemplateSelectDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("TemplateSelectDialog"));

	mainPanel            = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer         = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	mainToolbar          = XRCCTRL(*this, "mainToolbar", Toolbar);
	cancelToolbarButton  = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	useToolbarButton     = XRCCTRL(*this, "useToolbarButton", ToolbarButton);
	textTemplateListBox  = XRCCTRL(*this, "textTemplateListBox", wxListBox);
	textTemplateTextCtrl = XRCCTRL(*this, "textTemplateTextCtrl", wxTextCtrl);

	mainBoxSizer->SetSizeHints(this);
	SetSize(wxSize(800, 600));

	CONNECT_TOOLBARBUTTON(cancelToolbarButton, TemplateSelectDialog::CancelEvent);
	CONNECT_TOOLBARBUTTON(useToolbarButton, TemplateSelectDialog::UseEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 'u',  USE_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	ICON();

	TextTemplateRefresh();
}

TemplateSelectDialog::~TemplateSelectDialog() {
	TextTemplateCleanUp();
}

void TemplateSelectDialog::UseEvent(wxMouseEvent &event) {
	Use();
	SKIP();
}

void TemplateSelectDialog::UseKeyEvent(wxCommandEvent &event) {
	Use();
}

void TemplateSelectDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void TemplateSelectDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void TemplateSelectDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

void TemplateSelectDialog::SelectEvent(wxCommandEvent &event) {
	Select();
}

void TemplateSelectDialog::Use() {
	if(textTemplateListBox->GetSelection() != wxNOT_FOUND) {
		if(textCtrl) {
			TemplateEntity *templateEntity = GetTemplateEntity();
			if(templateEntity)
				textCtrl->SetValue(templateEntity->Get(_N("content"))->ToString());
		}

		EndModal(wxID_OK);
	} else {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the text template, you want to use."),
			_("Select text template")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->ShowModal();
	}
}

void TemplateSelectDialog::TextTemplateRefresh() {
	TextTemplateCleanUp();

	TemplateEntity templateEntity(rit);
	TemplateController templateController(rit);
	templateController.Get();
	while(templateController.Run()) {
		templateController.Get(&templateEntity);
		textTemplateListBox->Append(
			templateEntity.Get(_N("name"))->ToString(),
			new int(templateEntity.Get(_N("templateId"))->ToUnsignedInt())
		);
	}
}

void TemplateSelectDialog::TextTemplateCleanUp() {
	unsigned int *templateId = NULL;
	const unsigned int count = textTemplateListBox->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		templateId = static_cast<unsigned int *>(textTemplateListBox->GetClientData(i));
		if(templateId)
			delete templateId;
	}

	textTemplateListBox->Clear();
}

TemplateEntity *TemplateSelectDialog::GetTemplateEntity() {
	TemplateEntity *templateEntity = NULL;

	const unsigned int *templateId = static_cast<const unsigned int *>(textTemplateListBox->GetClientData(textTemplateListBox->GetSelection()));
	if(templateId) {
		TemplateController templateController(rit);
		templateController.Get(DSCasts::ToString(*templateId));
		if(templateController.Run()) {
			templateEntity = new TemplateEntity(rit);
			templateController.Get(templateEntity);
			templateController.Reset();
		}
	}

	return templateEntity;
}

unsigned int TemplateSelectDialog::GetTemplateId() {
	unsigned int templateId = 0;
	TemplateEntity *templateEntity = GetTemplateEntity();
	if(templateEntity) {
		templateId = templateEntity->Get(_N("templateId"))->ToUnsignedInt();
		delete templateEntity;
	}

	return templateId;
}

wxString TemplateSelectDialog::GetTemplateName() {
	wxString templateName;
	TemplateEntity *templateEntity = GetTemplateEntity();
	if(templateEntity) {
		templateName = templateEntity->Get(_N("name"))->ToString();
		delete templateEntity;
	}

	return templateName;
}

wxString TemplateSelectDialog::GetTemplateContent() {
	wxString templateContent;
	TemplateEntity *templateEntity = GetTemplateEntity();
	if(templateEntity) {
		templateContent = templateEntity->Get(_N("content"))->ToString();
		delete templateEntity;
	}

	return templateContent;
}

bool TemplateSelectDialog::ShowAndSet(wxWindow *parent, RuntimeInformationType *rit, wxTextCtrl *textCtrl) {
	TemplateSelectDialog *templateSelectDialog = new TemplateSelectDialog(
		parent,
		-1,
		rit,
		textCtrl
	);
	templateSelectDialog->Center();
	const bool ok = templateSelectDialog->ShowModal() == wxID_OK;
	templateSelectDialog->Destroy();
	return ok;
}

void TemplateSelectDialog::Select() {
	TemplateEntity *templateEntity = GetTemplateEntity();
	if(templateEntity) {
		textTemplateTextCtrl->SetValue(templateEntity->Get(_N("content"))->ToString());
		delete templateEntity;
	} else {
		textTemplateTextCtrl->Clear();
	}
}
