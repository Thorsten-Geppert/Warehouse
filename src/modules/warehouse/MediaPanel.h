#ifndef MEDIA_PANEL_H_
#define MEDIA_PANEL_H_

#include <wx/wx.h>
#include <DSListCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/LinkedFileEntities.h"
#include "../lib/LinkedFileEntity.h"
#include "FilesPanel.h"

using namespace DynSoft;

class MediaPanel : public wxPanel {

	public:
		MediaPanel(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const wxString &table,
			LinkedFileEntities *linkedFileEntities,
			bool *changed = NULL,
			const wxString &defaultDescription = wxEmptyString
		);
		~MediaPanel();

		void SetTable(const wxString &table);
		wxString GetTable() const;
		void SetEditable(const bool editable);
		bool IsEditable() const;
	
	protected:
		DECLARE_EVENT_TABLE()

		void AddEvent(wxCommandEvent &event);
		void RemoveEvent(wxCommandEvent &event);
		void ChangeDescriptionEvent(wxCommandEvent &event);
		void OpenLinkedEvent(wxListEvent &event);
		void OpenFileEvent(wxListEvent &event);

		void AddToMediaListCtrl(LinkedFileEntity *linkedFileEntity);
		void Add();
		void Remove();
		void OpenLinked();
		void OpenFile();
		void ChangeDescription();
		wxString GetPathName(
			const wxString &name,
			const unsigned int categoryId
		);

		RuntimeInformationType *rit;
		wxString table;
		LinkedFileEntities *linkedFileEntities;
		bool *changed;
		bool editable;

		wxBoxSizer *mainBoxSizer;
		wxBoxSizer *filesBoxSizer;
		FilesPanel *filesPanel;
		wxComboBox *addAsComboBox;
		wxButton   *addButton;
		DSListCtrl *mediaListCtrl;
		wxButton   *removeButton;
		wxComboBox *changeDescriptionCombobox;
		wxButton   *changeButton;

};

#endif /* MEDIA_PANEL_H_ */
