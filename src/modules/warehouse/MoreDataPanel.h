#ifndef MOREDATAPANEL_H_
#define MOREDATAPANEL_H_

#include <wx/wx.h>
#include <DSListCtrl.h>
#include <DSTextCtrl.h>
#include <DSComboBox.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/KeysValuesType.h"

using namespace DynSoft;

class MoreDataPanel : public wxPanel {

	public:
		MoreDataPanel(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			KeysValuesType *keysValuesType,
			bool *changed = NULL
		);
		~MoreDataPanel();

		void SetEditable(const bool editable);
		bool IsEditable() const;

	protected:
		DECLARE_EVENT_TABLE()

		void SaveEvent(wxCommandEvent &event);
		void EditEvent(wxListEvent &event);
		void AbortEvent(wxCommandEvent &event);
		void RemoveEvent(wxCommandEvent &event);
		void SaveTextEvent(wxCommandEvent &event);

		void Save();
		void Edit();
		void Abort();
		void Remove();
		void AddOrChangeToDataListCtrl(KeyValueType *keyValueType, const bool justAppend);

		RuntimeInformationType *rit;
		KeysValuesType *keysValuesType;
		KeyValueType *editingKeyValueType;
		bool *changed;
		bool editable;

		wxBoxSizer *mainBoxSizer;
		DSComboBox *descriptionComboBox;
		DSTextCtrl *valueTextCtrl;
		wxButton   *saveButton;
		wxButton   *abortButton;
		DSListCtrl *dataListCtrl;
		wxButton   *removeButton;

};

#endif /* MOREDATAPANEL_H_ */
