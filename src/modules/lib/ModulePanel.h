#ifndef MODULEPANEL_H_
#define MODULEPANEL_H_

#include <wx/wx.h>
#include "RuntimeInformationType.h"

class ModulePanel : public wxPanel {

	public:
		ModulePanel();
		ModulePanel(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		ModulePanel(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const wxString &object,
			wxString path = wxEmptyString,
			wxString file = wxEmptyString
		);
		virtual ~ModulePanel();

		virtual void Load()   = 0;
		virtual void Unload() = 0;

		void SetRuntimeInformationType(RuntimeInformationType *rit);
		RuntimeInformationType *GetRuntimeInformationType() const;
	
	protected:
		RuntimeInformationType *rit;

		void Defaults();

};

#endif /* MODULEPANEL_H_ */
