#include "ListEntryController.h"
#include "ListEntryEntity.h"

ListEntryController::ListEntryController(
	RuntimeInformationType *rit,
	const wxString &table,
	const wxString &field
) : Controller(
	rit,
	table,
	field,
	field
) {
	pk = field;
}

wxArrayString ListEntryController::ToArrayString() {
	wxArrayString results;

	ListEntryEntity listEntity(rit, GetTable(), pk);
	ListEntryController listController(rit, GetTable(), pk);
	listController.Get();
	while(listController.Run()) {
		listController.Get(&listEntity);
		results.Add(listEntity.Get(pk)->ToString());
	}

	return results;
}

unsigned int ListEntryController::FillComboBox(
	RuntimeInformationType *rit,
	wxComboBox *comboBox,
	const wxString &table,
	const wxString &field,
	const unsigned int fillMethod,
	const bool prependEmpty
) {
	unsigned int count = 0;
	if(!rit || !comboBox || table.IsEmpty() || field.IsEmpty())
		return count;

	if(fillMethod == FILL_NEW)
		comboBox->Clear();

	if(prependEmpty)
		comboBox->Append(wxEmptyString);
	
	wxString value;
	ListEntryEntity listEntryEntity(rit, table, field);
	ListEntryController listEntryController(rit, table, field);
	listEntryController.Get();
	while(listEntryController.Run()) {
		listEntryController.Get(&listEntryEntity);
		value = listEntryEntity.Get(field)->ToString();

		if(fillMethod == FILL_APPEND && comboBox->FindString(value) != wxNOT_FOUND)
			continue;

		comboBox->Append(value);
		count++;
	}

	return count;
}
