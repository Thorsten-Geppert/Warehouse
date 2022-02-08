#include "ContentPanel.h"

ContentPanel::ContentPanel(
	wxWindow *parent,
	wxWindowID id
) : wxPanel(
	parent,
	id
) {
	contentBoxSizer = new wxBoxSizer(wxVERTICAL);
	modulePanel     = NULL;
	if(modulePanel)
		contentBoxSizer->Add(modulePanel, 1, wxEXPAND);
	SetSizer(contentBoxSizer);
}

void ContentPanel::Change(ModulePanel *modulePanel) {
	if(this->modulePanel) {
		contentBoxSizer->Detach(this->modulePanel);
		this->modulePanel->Show(false);
		this->modulePanel->Unload();
	}

	this->modulePanel = modulePanel;
	if(this->modulePanel) {
		this->modulePanel->Load();
		contentBoxSizer->Prepend(this->modulePanel, 1, wxEXPAND);
		this->modulePanel->Show(true);
	}

	contentBoxSizer->Layout();
}
