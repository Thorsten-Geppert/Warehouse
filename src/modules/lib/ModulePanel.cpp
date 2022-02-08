#include "ModulePanel.h"

#include <wx/xrc/xmlres.h>

ModulePanel::ModulePanel() : wxPanel(), rit(NULL) {
	Defaults();
}

ModulePanel::ModulePanel(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) : wxPanel(
	parent,
	id
) {
	this->rit = rit;
	Defaults();
}

ModulePanel::ModulePanel(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	const wxString &object,
	wxString path,
	wxString file
) {
	SetId(id);
	this->rit = rit;
	path      = path.IsEmpty() ? _N("gui") : path;
	file      = file.IsEmpty() ? object + _N(".xml") : file;

	wxXmlResource::Get()->Load(this->rit->GRP(path, file));
	wxXmlResource::Get()->LoadPanel(this, parent, object);

	Defaults();
}

ModulePanel::~ModulePanel() {
}

void ModulePanel::SetRuntimeInformationType(RuntimeInformationType *rit) {
	this->rit = rit;
}

RuntimeInformationType *ModulePanel::GetRuntimeInformationType() const {
	return rit;
}

void ModulePanel::Defaults() {
	Show(false);
}
