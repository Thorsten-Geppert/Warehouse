#include "Dock.h"
#include "../warehouse/Configuration.h"

BEGIN_EVENT_TABLE(Dock, wxPanel)
	EVT_PAINT(Dock::OnPaint)
	EVT_KEY_DOWN(Dock::OnKey)
	EVT_KEY_UP(Dock::OnKey)
	EVT_CHAR(Dock::OnKey)
END_EVENT_TABLE()

Dock::Dock(
	wxWindow *parent,
	ContentPanel *contentPanel,
	wxWindowID id
) : wxPanel(
	parent,
	id,
	wxDefaultPosition,
	wxDefaultSize
) {
	SetBackgroundColour(*wxWHITE);
	SetWindowStyle(GetWindowStyle() | wxFULL_REPAINT_ON_RESIZE);
	this->contentPanel = contentPanel;
	SetPressedButton(NULL);
	
	wxSize tmp(-1, HEIGHT);
	SetSize(tmp);
	SetMinSize(tmp);
	SetMaxSize(tmp);

	wxBoxSizer *dockButtonCenterSizer = new wxBoxSizer(wxVERTICAL);
	dockButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	dockButtonCenterSizer->Add(dockButtonSizer, 0, wxALIGN_CENTER);
	SetSizer(dockButtonCenterSizer);
}

Dock::~Dock() {
}

void Dock::OnPaint(wxPaintEvent &event) {
	wxPaintDC dc(this);
	DrawBrackground(&dc);
}

void Dock::OnKey(wxKeyEvent &event) {
	event.Skip();
}

void Dock::DrawBrackground(wxPaintDC *dc) {
	int width, height;
	GetSize(&width, &height);

	dc->SetPen(wxPen(FIRST_COLOUR));
	dc->DrawLine(0, 0, width, 0);

/*
	unsigned char colour = 60;
	wxColour background;
	for(int i = 0; i < height; i++) {
		background.Set(colour, colour, colour);
		dc->SetPen(wxPen(background));
		dc->DrawLine(0, i, width, i);
		colour = colour - 1 >= 0 ? colour - 1 : 0;
	}

	dc->SetPen(wxPen(wxColour(255, 175, 75)));
	dc->SetBrush(wxBrush(wxColour(255, 175, 75)));
	dc->DrawRectangle(0, 0, width, 4);
	dc->SetPen(wxPen(wxColour(255, 220, 180)));
	dc->DrawLine(0, 4, width, 4);
*/
}

void Dock::Add(DockButton *dockButton, const int spaceLeft, const int spaceRight) {
	if(spaceLeft > 0)
		dockButtonSizer->AddSpacer(spaceLeft);
	dockButtonSizer->Add(dockButton);
	if(spaceRight > 0)
		dockButtonSizer->AddSpacer(spaceRight);
	dockButtons.Add(dockButton);
}

DockButton *Dock::Remove(const unsigned int index) {
	DockButton *dockButton = dockButtons.Get(index);
	if(dockButton) {
		dockButtonSizer->Detach(dockButton);
		dockButtons.Remove(dockButtons.Find(dockButton));
	}
	return dockButton;
}

bool Dock::Remove(DockButton *dockButton) {
	if(!dockButton)
		return false;
	
	dockButtonSizer->Detach(dockButton);
	dockButtons.Remove(dockButtons.Find(dockButton));

	return true;
}

DockButton *Dock::Get(const unsigned int index) {
	return dockButtons.Get(index);
}

void Dock::UnDownAll() {
	const unsigned int count = dockButtons.GetCount();
	for(unsigned int i = 0; i < count; i++)
		dockButtons.Get(i)->SetDown(false);
}

void Dock::SetPressedButton(DockButton *pressedDockButton) {
	this->pressedDockButton = pressedDockButton;
}

DockButton *Dock::GetPressedButton() const {
	return pressedDockButton;
}

ContentPanel *Dock::GetContentPanel() const {
	return contentPanel;
}

void Dock::Change(const unsigned int index) {
	DockButton *dockButton = Get(index);
	if(dockButton)
		dockButton->SetButton();
}

DSTemplateContainer<DockButton *> *Dock::GetDockButtons() {
	return &dockButtons;
}

BEGIN_EVENT_TABLE(DockButton, Dock)
	EVT_PAINT(DockButton::OnPaint)
	EVT_LEFT_UP(DockButton::OnLeftUp)
	EVT_LEFT_DOWN(DockButton::OnLeftDown)
	EVT_ENTER_WINDOW(DockButton::OnEnter)
	EVT_LEAVE_WINDOW(DockButton::OnLeave)
	EVT_KEY_DOWN(DockButton::OnKey)
	EVT_KEY_UP(DockButton::OnKey)
	EVT_CHAR(DockButton::OnKey)
END_EVENT_TABLE()

DockButton::DockButton(
	Dock *dock,
	wxWindowID id,
	const wxString &label,
	const wxBitmap &bitmap,
	ModulePanel *modulePanel
) : Dock(
	dock,
	NULL,
	id
) {
	this->dock        = dock;
	this->label       = label;
	this->bitmap      = bitmap;
	this->modulePanel = modulePanel;
	this->onEnter     = false;
	this->onPressed   = false;
	this->isDown      = false;

	wxSize tmp(WIDTH, dock->HEIGHT);
	SetSize(tmp);
	SetMinSize(tmp);

	font = GetFont();
	font.SetFamily(wxFONTFAMILY_SWISS);

	#ifdef __WXMAC__
		font.SetPointSize(12);
	#elif defined __WXMSW__
		font.SetPointSize(8);
	#else
		font.SetPointSize(10);
	#endif

	wxMemoryDC dc;
	dc.SetFont(font);
	dc.GetTextExtent(label, &labelWidth, &labelHeight);

	this->width        = WIDTH;
	this->height       = dock->HEIGHT;
	this->half         = width / 2;
	this->bitmapWidth  = bitmap.GetWidth();
	this->bitmapHeight = bitmap.GetHeight();
	this->bitmapX      = half - bitmapWidth / 2;
	this->bitmapY      = 7;
	this->labelX       = half - labelWidth / 2;
	this->labelY       = height - labelHeight - 5;
}

void DockButton::SetDown(const bool isDown) {
	this->isDown = isDown;
	Refresh();
}

bool DockButton::GetDown() const {
	return isDown;
}

ModulePanel *DockButton::GetModulePanel() const {
	return modulePanel;
}

void DockButton::OnLeftUp(wxMouseEvent &event) {
	SetButton();
}

void DockButton::OnLeftDown(wxMouseEvent &event) {
	onPressed = true;
	Refresh();
}

void DockButton::OnEnter(wxMouseEvent &event) {
	onEnter = true;
	Refresh();
}

void DockButton::OnLeave(wxMouseEvent &event) {
	onEnter = false;
	onPressed = false;
	Refresh();
}

void DockButton::OnPaint(wxPaintEvent &event) {
	wxPaintDC dc(this);

	if(onPressed || onEnter || isDown)
		DrawBackground(&dc);
	else
		dock->DrawBrackground(&dc);

	dc.SetTextForeground(onPressed || onEnter || isDown ? *wxWHITE : *wxBLACK);
	dc.SetFont(font);
	dc.DrawText(label, labelX, labelY);
	dc.DrawBitmap(bitmap, bitmapX, bitmapY);
}

void DockButton::DrawBackground(wxPaintDC *dc) {
	dc->SetPen(wxPen(FIRST_COLOUR));
	for(int i = 0; i < height; i++)
		dc->DrawLine(0, i, width, i);

	/*unsigned int start = 175;
	for(int i = 0; i < height; i++) {
		dc->SetPen(wxPen(wxColour(255, start, start - 100)));
		dc->DrawRectangle(0, i, width, 1);
		if(i < 12 && (onPressed || isDown))
			start -= i;
		else
			start++;
	}

	dc->SetPen(wxPen(wxColour(255, 220, 180)));
	dc->DrawLine(0, 4, 0, height); // Left
	dc->DrawLine(width - 1, 4, width -1 , height); // Right
	dc->DrawLine(0, height - 1, width - 1, height - 1); // Bottom*/
}

void DockButton::SetButton() {
	DockButton *lastDockButton = dock->GetPressedButton();
	if(lastDockButton)
		lastDockButton->SetDown(false);
	dock->SetPressedButton(this);
	dock->GetContentPanel()->Change(modulePanel);
	onPressed = false;
	isDown    = true;
	Refresh();
}

DockToggleButton::DockToggleButton(
	Dock *dock,
	wxWindowID id,
	const wxString &label,
	const wxBitmap &bitmap
) : DockButton(
	dock,
	id,
	label,
	bitmap,
	NULL
) {
}

void DockToggleButton::OnLeftUp(wxMouseEvent &event) {
	event.Skip();
}

void DockToggleButton::SetButton(const bool enabled) {
	onPressed = enabled;
	isDown = enabled;
	Refresh();
}

void DockToggleButton::DrawBackground(wxPaintDC *dc) {
	dc->SetPen(wxPen(FIRST_COLOUR));
	for(int i = 0; i < height; i++)
		dc->DrawLine(0, i, width, i);

	/*unsigned int start = 175;
	for(int i = 0; i < height; i++) {
		dc->SetPen(wxPen(wxColour(255, start, start - 100)));
		dc->DrawRectangle(0, i, width, 1);
		if(i < 12 && (onPressed || isDown))
			start -= i;
	}

	dc->SetPen(wxPen(wxColour(255, 220, 180)));
	dc->DrawLine(0, 4, 0, height); // Left
	dc->DrawLine(width - 1, 4, width -1 , height); // Right
	dc->DrawLine(0, height - 1, width - 1, height - 1); // Bottom*/
}
