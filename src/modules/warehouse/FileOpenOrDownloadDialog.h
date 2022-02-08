#ifndef FILE_OPEN_OR_DOWNLOAD_H_
#define FILE_OPEN_OR_DOWNLOAD_H_

#include <wx/wx.h>
#include "../lib/RuntimeInformationType.h"

using namespace DynSoft;

class FileOpenOrDownloadDialog : public wxDialog {

	public:
		FileOpenOrDownloadDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);

		static const unsigned int NOTHING  = 0;
		static const unsigned int OPEN     = 1;
		static const unsigned int DOWNLOAD = 2;

		unsigned int GetTodo() const;
	
	protected:
		enum {
			CANCEL_EVENT
		};

		DECLARE_EVENT_TABLE()

		void OpenEvent(wxCommandEvent &event);
		void DownloadEvent(wxCommandEvent &event);
		void CancelEvent(wxCommandEvent &event);

		void Open();
		void Download();
		void Cancel();

		RuntimeInformationType *rit;
		unsigned int todo;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;
		wxButton   *openButton;
		wxButton   *downloadButton;

};

#endif /* FILE_OPEN_OR_DOWNLOAD_H_ */
