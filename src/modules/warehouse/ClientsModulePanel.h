#ifndef CLIENTSMODULEPANEL_H_
#define CLIENTSMODULEPANEL_H_

#include <wx/wx.h>
#include <DSListCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/ModulePanel.h"
#include "../lib/Toolbar.h"
#include "../lib/ClientEntity.h"
#include "GuiCallbackInterface.h"
#include "ClientsFindDialog.h"

using namespace DynSoft;

class ClientsModulePanel : public ModulePanel, public GuiCallbackInterface {

	public:
		ClientsModulePanel(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		~ClientsModulePanel();

		virtual void Load();
		virtual void Unload();
		virtual void Callback(void *data);
	
	protected:
		enum {
			NEW_EVENT,
			EDIT_EVENT,
			REMOVE_EVENT,
			FIND_EVENT,
			EXPORT_EVENT
		};

		DECLARE_EVENT_TABLE()

		void NewEvent(wxMouseEvent &event);
		void EditEvent(wxMouseEvent &event);
		void DirectEditEvent(wxListEvent &event);
		void RemoveEvent(wxMouseEvent &event);
		void FindEvent(wxMouseEvent &event);
		void NewKeyEvent(wxCommandEvent &event);
		void EditKeyEvent(wxCommandEvent &event);
		void RemoveKeyEvent(wxCommandEvent &event);
		void FindKeyEvent(wxCommandEvent &event);
		void ExportEvent(wxMouseEvent &event);
		void ExportKeyEvent(wxCommandEvent &event);

		void New();
		void Edit(unsigned int *clientId);
		void Remove();
		void Find();
		void Export();

		void AddOrChangeToClientsListCtrl(ClientEntity *clientEntity, const bool justAppend);
		void ClearClientsListCtrl();

		// Stuff
		ClientsFindDialog *clientsFindDialog;

		ToolbarButton *newToolbarButton;
		ToolbarButton *editToolbarButton;
		ToolbarButton *removeToolbarButton;
		ToolbarButton *findToolbarButton;
		ToolbarButton *exportToolbarButton;
		DSListCtrl *clientsListCtrl;

};

#endif /* CLIENTSMODULEPANEL_H_ */
