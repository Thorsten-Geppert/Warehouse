#ifndef PREFERENCESMODULEPANEL_H_
#define PREFERENCESMODULEPANEL_H_

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/treebook.h>
#include <DSListCtrl.h>
#include <DSTextCtrl.h>
#include <DSCheckBox.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/ModulePanel.h"
#include "../lib/UserEntity.h"
#include "../lib/Toolbar.h"
#include "../lib/BankEntity.h"
#include "../lib/StockEntity.h"
#include "../lib/KeysValuesType.h"
#include "BugDialog.h"
#include "ContactDialog.h"

using namespace DynSoft;

class PreferencesModulePanel : public ModulePanel {

	public:
		PreferencesModulePanel(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		~PreferencesModulePanel();

		virtual void Load();
		virtual void Unload();
	
	protected:
		enum {
			LOG_EVENT,
			CONTACT_EVENT,
			BUG_EVENT,
			INFORMATION_EVENT,
			HELP_EVENT
		};

		DECLARE_EVENT_TABLE()

		void SaveEnterprisePreferencesEvent(wxCommandEvent &event);
		void SaveCommonPreferencesEvent(wxCommandEvent &event);
		void AddUserEvent(wxCommandEvent &event);
		void EditUserEvent(wxListEvent &event);
		void RemoveUserEvent(wxCommandEvent &event);
		void InformationEvent(wxMouseEvent &event);
		void LogEvent(wxMouseEvent &event);
		void HelpEvent(wxMouseEvent &event);
		void BugEvent(wxMouseEvent &event);
		void ContactEvent(wxMouseEvent &event);
		void ChangeTableEntryEvent(wxCommandEvent &event);
		void SaveTableEntryEvent(wxCommandEvent &event);
		void RemoveTableEntryEvent(wxCommandEvent &event);
		void CheckForUpdatesEvent(wxCommandEvent &event);
		void LogKeyEvent(wxCommandEvent &event);
		void ContactKeyEvent(wxCommandEvent &event);
		void BugKeyEvent(wxCommandEvent &event);
		void InformationKeyEvent(wxCommandEvent &event);
		void HelpKeyEvent(wxCommandEvent &event);
		void ExportUsersEvent(wxCommandEvent &event);
		void FindBankEvent(wxCommandEvent &event);
		void SaveBankEvent(wxCommandEvent &event);
		void ImportBankEvent(wxCommandEvent &event);
		void ExportBankEvent(wxCommandEvent &event);
		void EditBankEvent(wxListEvent &event);
		void RemoveBankEvent(wxCommandEvent &event);
		void BankFindEvent(wxCommandEvent &event);
		void TextTemplateSelectEvent(wxCommandEvent &event);
		void TextTemplateRemoveEvent(wxCommandEvent &event);
		void TextTemplateEditEvent(wxCommandEvent &event);
		void TextTemplateAddEvent(wxCommandEvent &event);
		void TextTemplateRefreshEvent(wxCommandEvent &event);
		void StockAddEvent(wxCommandEvent &event);
		void StockEditEvent(wxListEvent &event);
		void StockRemoveEvent(wxCommandEvent &event);
		void ReportGeneratorStartEvent(wxCommandEvent &event);
		void AdvancedPreferencesEditEvent(wxListEvent &event);
		void ReportPathBrowseEvent(wxCommandEvent &event);
		void JreHomeBrowseEvent(wxCommandEvent &event);

		void SaveEnterprisePreferences();
		void SaveCommonPreferences();
		void AddUser();
		void EditUser();
		void RemoveUser();
		void Information();
		void Log();
		void Help();
		void Bug();
		void Contact();
		void ChangeTableEntry();
		void SaveTableEntry();
		void RemoveTableEntry();
		void CheckForUpdates();
		void ExportUsers();
		void FindBank(const wxString &bank, const wxString &bankCode, const wxString &bic);
		void SaveBank();
		void ImportBank();
		void ExportBank();
		void RemoveBank(unsigned int *id);
		void BankFind();
		void EditBank(unsigned int *id);
		void EnableBankFields(const bool edit, unsigned int *id);
		void TextTemplateSelect(const int selection);
		void TextTemplateRemove(const int selection);
		void TextTemplateEdit(const int selection);
		void TextTemplateAdd();
		void TextTemplateRefresh();
		void TextTemplateCleanUp();
		void StockAdd();
		void StockEdit();
		void StockRemove();
		void ReportGeneratorStart();
		void AdvancedPreferencesEdit(KeyValueType *keyValue);
		void ReportPathBrowse(wxTextCtrl *pathTextCtrl);
		void JreHomeBrowse(wxTextCtrl *jreHomeTextCtrl);

		void AppendUser(UserEntity *userEntity);
		void EnableFillInData(const bool enable, const bool withTableListBox = false);
		void GetFillInDataTable(wxString *table, wxString *field);
		void AddOrChangeToBanksListCtrl(BankEntity *bankEntity, const bool justAppend);
		void AddOrChangeToStocksListCtrl(StockEntity *stockEntity);
		void AdvancedPreferencesLoad();

		// General
		ContactDialog *contactDialog;
		BugDialog *bugDialog;
		unsigned int *actualBankId;
		wxString bankDefaultDirectory;

		// Toolbar
		ToolbarButton *informationToolbarButton;
		ToolbarButton *logToolbarButton;
		ToolbarButton *helpToolbarButton;
		ToolbarButton *bugToolbarButton;
		ToolbarButton *contactToolbarButton;
		
		// Tree
		wxTreebook *optionTreebook;

		// Version
		wxStaticText *versionStaticText;

		// Informations
		wxTextCtrl *connectionTextCtrl;

		// System
		wxTextCtrl *systemTextCtrl;

		// Enterprise Preferences
		unsigned int enterprisePreferencesId;
		wxTextCtrl *companyTextCtrl;
		wxTextCtrl *executiveTextCtrl;
		wxTextCtrl *address1TextCtrl;
		wxTextCtrl *address2TextCtrl;
		wxTextCtrl *zipcodeTextCtrl;
		wxTextCtrl *cityTextCtrl;
		wxTextCtrl *salesTaxIdentificationNumberTextCtrl;
		wxTextCtrl *taxNumberTextCtrl;
		wxTextCtrl *salesTaxIndicatorTextCtrl;
		wxTextCtrl *telephoneTextCtrl;
		wxTextCtrl *faxTextCtrl;
		wxTextCtrl *emailTextCtrl;
		wxTextCtrl *webTextCtrl;
		wxTextCtrl *bankTextCtrl;
		DSTextCtrl *bankCodeTextCtrl;
		wxTextCtrl *holderTextCtrl;
		DSTextCtrl *accountNumberTextCtrl;
		DSTextCtrl *bicTextCtrl;
		DSTextCtrl *ibanTextCtrl;
		/// Mandatory fields
		wxCheckBox *mandatoryClientsOwnClientIdCheckBox;
		wxCheckBox *mandatoryProductsArticleNumberCheckBox;

		// Common Preferences
		unsigned int commonPreferencesId;
		DSTextCtrl *minorValueAddedTaxTextCtrl;
		DSTextCtrl *majorValueAddedTaxTextCtrl;
		wxSpinCtrl *monitionStatesSpinCtrl;
		DSTextCtrl *monitionPriceTextCtrl;
		DSCheckBox *reportUseCheckBox;
		DSTextCtrl *reportPathWindowsTextCtrl;
		DSTextCtrl *reportPathUnixTextCtrl;
		DSTextCtrl *reportPathMacOSTextCtrl;
		wxButton   *reportPathWindowsBrowseButton;
		wxButton   *reportPathUnixBrowseButton;
		wxButton   *reportPathMacOSBrowseButton;

		DSTextCtrl *jreHomeWindowsTextCtrl;
		DSTextCtrl *jreHomeUnixTextCtrl;
		DSTextCtrl *jreHomeMacOSTextCtrl;
		wxButton   *jreHomeWindowsBrowseButton;
		wxButton   *jreHomeUnixBrowseButton;
		wxButton   *jreHomeMacOSBrowseButton;

		// Advanced preferences
		DSListCtrl *advancedPreferencesListCtrl;

		// User management
		DSListCtrl *userListCtrl;

		// Fill in data
		wxListBox    *fillInDataTablesListBox;
		wxStaticText *fillInDataEntryStaticText;
		wxTextCtrl   *fillInDataEntryTextCtrl;
		wxListBox    *fillInDataEntriesListBox;
		wxButton     *fillInDataSaveEntryButton;
		wxButton     *fillInDataRemoveEntryButton;

		// Bank
		wxTextCtrl *fillInDataBankTextCtrl;
		DSTextCtrl *fillInDataBankCodeTextCtrl;
		DSTextCtrl *fillInDataBankBicTextCtrl;
		wxButton   *fillInDataBankFindButton;
		wxButton   *fillInDataBankSaveButton;
		DSListCtrl *fillInDataBankListCtrl;
		wxButton   *fillInDataBankImportButton;
		wxButton   *fillInDataBankExportButton;
		wxButton   *fillInDataBankRemoveButton;

		// Text templates
		wxListBox *textTemplateListBox;
		wxTextCtrl *textTemplateTextCtrl;
		wxButton *textTemplateRemoveButton;
		wxButton *textTemplateEditButton;
		wxButton *textTemplateAddButton;
		wxButton *textTemplateRefreshButton;

		// Stock management
		DSListCtrl *stockListCtrl;
		wxButton   *removeStockButton;
		wxButton   *addStockButton;

		// Reports
		wxButton *reportGeneratorStartButton;

};

#endif /* PREFERENCESMODULEPANEL_H_ */
