#ifndef PROGRESSDIALOG_H_
#define PROGRESSDIALOG_H_

#include <wx/wx.h>
#include <wx/gauge.h>
#include <wx/utils.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"

using namespace DynSoft;

class ProgressDialog : public wxDialog {

	public:
		ProgressDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const unsigned int count
		);

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
		wxWindowDisabler *windowDisabler;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		ToolbarButton *cancelToolbarButton;
		wxStaticText  *informationStaticText;
		wxGauge       *progressGauge;

};

#endif /* PROGRESSDIALOG_H_ */
