#ifndef EXPORT_TYPE_H_
#define EXPORT_TYPE_H_

#include <wx/wx.h>
#include <DSLib.h>

class ExportType {

	public:
		ExportType();
		ExportType(
			const wxString &filename,
			const wxString &fieldDelimiter,
			const wxString &textDelimiter,
			const unsigned int lineFeed,
			const wxString &replaceLineFeedWith = wxEmptyString,
			const bool replaceLineFeed = false
		);
		
		void SetFilename(const wxString &filename);
		wxString GetFilename() const;

		void SetFieldDelimiter(const wxString &fieldDelimiter);
		wxString GetFieldDelimiter() const;

		void SetTextDelimiter(const wxString &textDelimiter);
		wxString GetTextDelimiter() const;

		void SetLineFeed(const unsigned int lineFeed);
		unsigned int GetLineFeed() const;

		void SetReplaceLineFeedWith(const wxString &replaceLineFeedWith);
		wxString GetReplaceLineFeedWith() const;

		void SetReplaceLineFeed(const bool replaceLineFeed);
		bool GetReplaceLineFeed() const;
	
	private:
		wxString filename;
		wxString fieldDelimiter;
		wxString textDelimiter;
		unsigned int lineFeed;
		wxString replaceLineFeedWith;
		bool replaceLineFeed;

};

#endif /* EXPORT_TYPE_H_ */
