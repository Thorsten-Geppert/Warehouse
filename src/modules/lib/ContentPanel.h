#ifndef CONTENTPANEL_H_
#define CONTENTPANEL_H_

#include <wx/wx.h>
#include "ModulePanel.h"

class ContentPanel : public wxPanel {

	public:
		ContentPanel(
			wxWindow *parent,
			wxWindowID id
		);
		void Change(ModulePanel *modulePanel);
	
	private:	
		wxBoxSizer *contentBoxSizer;
		ModulePanel *modulePanel;

};

#endif /* CONTENTPANEL_H_ */
