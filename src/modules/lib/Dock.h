#ifndef DOCK_H_
#define DOCK_H_

#include <wx/wx.h>
#include <DSTemplateContainer.h>
#include "ContentPanel.h"

using namespace DynSoft;

class DockButton;
class Dock : public wxPanel {

	public:	
		static const int HEIGHT = 65;

		Dock(
			wxWindow *parent,
			ContentPanel *contentPanel,
			wxWindowID id
		);
		~Dock();

		void Add(DockButton *dockButton, const int spaceLeft = 0, const int spaceRight = 0);
		DockButton *Remove(const unsigned int index);
		bool Remove(DockButton *dockButton);
		DockButton *Get(const unsigned int index);
		void DrawBrackground(wxPaintDC *dc);
		void UnDownAll();
		void SetPressedButton(DockButton *pressedDockButton);
		DockButton *GetPressedButton() const;
		ContentPanel *GetContentPanel() const;
		void Change(const unsigned int index);
		DSTemplateContainer<DockButton *> *GetDockButtons();
	
	protected:
		DECLARE_EVENT_TABLE()

		void OnPaint(wxPaintEvent &event);
		void OnKey(wxKeyEvent &event);

		wxBoxSizer *dockButtonSizer;

		// Don't remove those self from the context
		// All those buttons will be deleted by wx
		DSTemplateContainer<DockButton *> dockButtons;
		DockButton *pressedDockButton;
		ContentPanel *contentPanel;

};

class DockButton : public Dock {

	public:
		static const unsigned int WIDTH = 100;

		DockButton(
			Dock *dock,
			wxWindowID id,
			const wxString &label,
			const wxBitmap &bitmap,
			ModulePanel *modulePanel = NULL
		);
		void SetDown(const bool isPressed);
		bool GetDown() const;
		ModulePanel *GetModulePanel() const;
		void SetButton();
	
	protected:
		DECLARE_EVENT_TABLE()

		virtual void OnLeftUp(wxMouseEvent &event);
		void OnLeftDown(wxMouseEvent &event);
		void OnEnter(wxMouseEvent &event);
		void OnLeave(wxMouseEvent &event);
		void OnPaint(wxPaintEvent &event);

		virtual void DrawBackground(wxPaintDC *dc);

		wxFont font;
		wxString label;
		wxBitmap bitmap;
		Dock *dock;
		ModulePanel *modulePanel;

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

};

class DockToggleButton : public DockButton {

	public:
		DockToggleButton(
			Dock *dock,
			wxWindowID id,
			const wxString &label,
			const wxBitmap &bitmap
		);

		void SetButton(const bool enabled);
	
	protected:
		void DrawBackground(wxPaintDC *dc);
		void OnLeftUp(wxMouseEvent &event);

};

#endif /* DOCK_H_ */
