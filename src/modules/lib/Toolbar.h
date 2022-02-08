#ifndef TOOLBAR_H_
#define TOOLBAR_H_

#include <wx/wx.h>
#include <wx/xrc/xmlres.h>
#include <wx/xml/xml.h>
#include <DSTemplateContainer.h>

#define SKIP() (event.Skip())
//#define CONNECT_TOOLBARBUTTON(button, event) (button->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(event), NULL, this))
#define CONNECT_TOOLBARBUTTON(button, event) (button->SetLeftClickEvent(wxMouseEventHandler(event), this))

using namespace DynSoft;

class ToolbarButton;
class Toolbar : public wxPanel {

	public:
		DECLARE_DYNAMIC_CLASS(Toolbar)

		static const int HEIGHT = 70;
		
		Toolbar();
		Toolbar(
			wxWindow *parent,
			wxWindowID id,
			const wxString &name = _N("Toolbar")
		);
		bool Create(wxWindow *parent, wxWindowID id, const wxString &name);

		void DrawBrackground(wxPaintDC *dc);
		void Add(ToolbarButton *dockButton);
		void AddSpacer(const int width);
		void AddStretchSpacer();
		ToolbarButton *Remove(const unsigned int index);
		bool Remove(ToolbarButton *toolbarButton);
		ToolbarButton *Get(const unsigned int index);
		void OnKey(wxKeyEvent &event);
	
	protected:
		DECLARE_EVENT_TABLE()

		void OnPaint(wxPaintEvent &event);

		wxBoxSizer *toolbarButtonSizer;

		DSTemplateContainer<ToolbarButton *> toolbarButtons;

};

class ToolbarButton : public Toolbar {

	public:
		static const int WIDTH = 80;

		using Toolbar::Connect;

		ToolbarButton(
			Toolbar *toolbar,
			wxWindowID id,
			const wxString &label,
			const wxBitmap &bitmap,
			const bool toggle = false,
			const wxString &name = _N("ToolbarButton")
		);
		void SetToggled(const bool toggle);
		bool GetToggled() const;
		void SetEnabled(const bool enable);
		bool GetEnabled() const;
		void SetLabel(const wxString &label);
		wxString GetLabel() const;

		void SetLeftClickEvent(
			wxObjectEventFunction onLeftClick,
			wxEvtHandler *onLeftClickEventHandler = NULL
		);

	protected:
		DECLARE_EVENT_TABLE()

		void OnLeftUp(wxMouseEvent &event);
		void OnLeftDown(wxMouseEvent &event);
		void OnEnter(wxMouseEvent &event);
		void OnLeave(wxMouseEvent &event);
		void OnPaint(wxPaintEvent &event);
		void OnInternalLeftDown(wxMouseEvent &event);
		void OnInternalLeftUp(wxMouseEvent &event);

		void DrawBackground(wxPaintDC *dc);
		void DrawDisabledBackground(wxPaintDC *dc);
		void Connect(const bool connect);

		Toolbar *toolbar;

		wxObjectEventFunction onLeftClick;
		wxEvtHandler *onLeftClickEventHandler;

		wxFont font;

		wxString label;
		wxBitmap bitmap;
		bool toggle;

		int width;
		int height;
		int half;
		int bitmapWidth;
		int bitmapHeight;
		int bitmapX;
		int bitmapY;
		int labelWidth;
		int labelHeight;
		int labelX;
		int labelY;

		bool onEnter;
		bool onPressed;
		bool isDown;
		bool isToggled;
		bool isEnabled;
		bool isConnected;

};

class ToolbarXmlResourceHandler : public wxXmlResourceHandler {

	public:
		static const unsigned int DEFAULT_SPACER_WIDTH = 5;

		virtual wxObject *DoCreateResource();
		virtual bool CanHandle(wxXmlNode *node);
		wxObject *CreateResource(
			wxXmlNode *node,
			wxObject *parent,
			wxObject *instance
		);
		bool IsOS(const wxString &os, wxString plattform);

};

#endif /* TOOLBAR_H_ */
