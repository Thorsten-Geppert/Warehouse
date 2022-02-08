#ifndef EXPORTPROGRESSDIALOG_H_
#define EXPORTPROGRESSDIALOG_H_

#include <wx/wx.h>
#include <wx/utils.h>
#include <wx/gauge.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"

using namespace DynSoft;

class ExportProgressDialog : public wxDialog {

	public:
		static const unsigned int FIRST  = 0;
		static const unsigned int SECOND = 1;

	public:
		ExportProgressDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);

		void SetGauge(const unsigned int index);
		unsigned int GetGauge() const;
		void SetCount(const unsigned int count);
		unsigned int GetCount() const;
		void SetValue(const unsigned int value);
		unsigned int GetValue() const;
		bool IsCanceled() const;
		void Start();
		void Stop();
	
	protected:
		enum {
			CANCEL_EVENT
		};

		DECLARE_EVENT_TABLE()

		void CancelEvent(wxMouseEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);

		void Cancel();

		RuntimeInformationType *rit;
		unsigned int count;
		wxString countString;
		unsigned int value;
		bool canceled;
		unsigned gauge;
		wxWindowDisabler *windowDisabler;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		ToolbarButton *cancelToolbarButton;
		wxStaticText  *informationStaticText;
		wxGauge       *firstProgressGauge;
		wxGauge       *secondProgressGauge;
		wxGauge       *progressGauge;

};

#endif /* EXPORTPROGRESSDIALOG_H_ */
