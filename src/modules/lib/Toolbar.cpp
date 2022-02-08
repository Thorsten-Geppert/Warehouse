#include "Toolbar.h"
#include <DSCasts.h>
#include <wx/settings.h>
#include "../warehouse/Configuration.h"

IMPLEMENT_DYNAMIC_CLASS(Toolbar, wxPanel)

BEGIN_EVENT_TABLE(Toolbar, wxPanel)
	EVT_PAINT(Toolbar::OnPaint)
	EVT_KEY_DOWN(Toolbar::OnKey)
	EVT_KEY_UP(Toolbar::OnKey)
	EVT_CHAR(Toolbar::OnKey)
END_EVENT_TABLE()

Toolbar::Toolbar() {
	SetBackgroundColour(*wxWHITE);
}

Toolbar::Toolbar(
	wxWindow *parent,
	wxWindowID id,
	const wxString &name
) {
	Create(parent, id, name);
	SetBackgroundColour(*wxWHITE);
}

bool Toolbar::Create(wxWindow *parent, wxWindowID id, const wxString &name) {
	if(wxWindow::Create(parent, id, wxDefaultPosition, wxDefaultSize, GetWindowStyle() | wxFULL_REPAINT_ON_RESIZE, name)) {
		wxSize tmp(-1, HEIGHT);
		SetSize(tmp);
		SetMinSize(tmp);
		SetMaxSize(tmp);

		toolbarButtonSizer = new wxBoxSizer(wxHORIZONTAL);
		SetSizer(toolbarButtonSizer);
	}
	return false;
}

void Toolbar::OnPaint(wxPaintEvent &event) {
	wxPaintDC dc(this);
	DrawBrackground(&dc);
	event.Skip();
}

void Toolbar::DrawBrackground(wxPaintDC *dc) {
	int width, height;
	GetSize(&width, &height);

	dc->SetPen(wxPen(FIRST_COLOUR));
	dc->DrawLine(0, height - 1, width, height - 1);
	dc->SetPen(wxPen(*wxWHITE));
	dc->DrawRectangle(0, 0, width, height - 1);

/*
	unsigned char colour = 60;
	wxColour background;
	//for(int i = height - 1; i >= 0; i--) { // inner algo
	for(int i = 0; i < height; i++) {        // outer algo
		background.Set(colour, colour, colour);
		dc->SetPen(wxPen(background));
		dc->DrawLine(0, i, width, i);
		colour = colour - 1 >= 0 ? colour - 1 : 0;
	}

	dc->SetPen(wxPen(wxColour(255, 175, 75)));
	dc->SetBrush(wxBrush(wxColour(255, 175, 75)));
	dc->DrawRectangle(0, height - 4, width, 4);
	dc->SetPen(wxPen(wxColour(255, 220, 180)));
	dc->DrawLine(0, height - 5, width, height - 5);
*/
}

void Toolbar::Add(ToolbarButton *toolbarButton) {
	toolbarButtonSizer->Add(toolbarButton);
	toolbarButtons.Add(toolbarButton);
}

void Toolbar::AddSpacer(const int width) {
	toolbarButtonSizer->AddSpacer(width);
}

void Toolbar::AddStretchSpacer() {
	toolbarButtonSizer->AddStretchSpacer();
}

ToolbarButton *Toolbar::Remove(const unsigned int index) {
	ToolbarButton *toolbarButton = toolbarButtons.Get(index);
	if(toolbarButton) {
		toolbarButtonSizer->Detach(toolbarButton);
		toolbarButtons.Remove(toolbarButtons.Find(toolbarButton));
	}
	return toolbarButton;
}

bool Toolbar::Remove(ToolbarButton *toolbarButton) {
	if(!toolbarButton)
		return false;
	
	toolbarButtonSizer->Detach(toolbarButton);
	toolbarButtons.Remove(toolbarButtons.Find(toolbarButton));

	return true;
}

ToolbarButton *Toolbar::Get(const unsigned int index) {
	return toolbarButtons.Get(index);
}

BEGIN_EVENT_TABLE(ToolbarButton, Toolbar)
	EVT_PAINT(ToolbarButton::OnPaint)
	/*EVT_LEFT_UP(ToolbarButton::OnLeftUp)
	EVT_LEFT_DOWN(ToolbarButton::OnLeftDown)
	EVT_ENTER_WINDOW(ToolbarButton::OnEnter)
	EVT_LEAVE_WINDOW(ToolbarButton::OnLeave)*/
	EVT_KEY_DOWN(Toolbar::OnKey)
	EVT_KEY_UP(Toolbar::OnKey)
	EVT_CHAR(Toolbar::OnKey)
END_EVENT_TABLE()

ToolbarButton::ToolbarButton(
	Toolbar *toolbar,
	wxWindowID id,
	const wxString &label,
	const wxBitmap &bitmap,
	const bool toggle,
	const wxString &name
) : Toolbar(
	toolbar,
	id,
	name
) {
	this->toolbar     = toolbar;
	this->bitmap      = bitmap;
	this->toggle      = toggle;
	this->onEnter     = false;
	this->onPressed   = false;
	this->isDown      = false;
	this->isToggled   = false;
	this->isEnabled   = true;
	this->isConnected = true;
	this->onLeftClick = NULL;

	SetLabel(label);

	Connect(true);
}

void Toolbar::OnKey(wxKeyEvent &event) {
	event.Skip();
}

void ToolbarButton::SetLabel(const wxString &label) {
	this->label = label;

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

	wxSize tmp(labelWidth > WIDTH ? labelWidth + 20 : WIDTH, toolbar->HEIGHT);
	SetSize(tmp);
	SetMinSize(tmp);

	this->width        = labelWidth > WIDTH ? labelWidth + 20 : WIDTH;
	this->height       = toolbar->HEIGHT - 1;
	this->half         = width / 2;
	this->bitmapWidth  = bitmap.GetWidth();
	this->bitmapHeight = bitmap.GetHeight();
	this->bitmapX      = half - bitmapWidth / 2;
	this->bitmapY      = 8;
	this->labelX       = half - labelWidth / 2;
	this->labelY       = height - labelHeight - 8;
}

wxString ToolbarButton::GetLabel() const {
	return label;
}

void ToolbarButton::SetToggled(const bool toggle) {
	this->isToggled = toggle;
	Refresh();
}
bool ToolbarButton::GetToggled() const {
	return isToggled;
}

void ToolbarButton::OnLeftUp(wxMouseEvent &event) {
	onPressed = false;
	isToggled = !isToggled;

/*	wxPoint mp(wxGetMousePosition());
	wxPoint wp(GetPosition());
	wxSize ws(GetSize());

	if(
		(mp.x < wp.x && mp.x > wp.x + ws.GetWidth())
		||
		(mp.y < wp.y && mp.y > wp.y + ws.GetHeight())
	)*/
		onEnter = false;

	Refresh();
	event.Skip();
}

void ToolbarButton::OnLeftDown(wxMouseEvent &event) {
	onPressed = true;
	Refresh();
	event.Skip();
}

void ToolbarButton::OnEnter(wxMouseEvent &event) {
	onEnter = true;
	Refresh();
	event.Skip();
}

void ToolbarButton::OnLeave(wxMouseEvent &event) {
	onEnter = false;
	onPressed = false;
	if(onLeftClick && GetEnabled())
		Disconnect(wxEVT_LEFT_UP, this->onLeftClick, NULL, this->onLeftClickEventHandler);
	Refresh();
	event.Skip();
}

void ToolbarButton::OnPaint(wxPaintEvent &event) {
	wxPaintDC dc(this);
	dc.SetFont(font);

	toolbar->DrawBrackground(&dc);

	if(!isEnabled)
		//DrawDisabledBackground(&dc);
		return;
	else if(onPressed || isDown || (toggle && isToggled) || onEnter)
		DrawBackground(&dc);

	dc.SetTextForeground(onPressed || isDown || (toggle && isToggled) ? *wxWHITE : *wxBLACK);
	dc.DrawText(label, labelX, labelY);
	dc.DrawBitmap(bitmap, bitmapX, bitmapY);
}

void ToolbarButton::DrawBackground(wxPaintDC *dc) {
	dc->SetPen(wxPen(onPressed || isDown || (toggle && isToggled) ? FIRST_COLOUR : SECOND_COLOUR));
	for(int i = 0; i < height; i++)
		dc->DrawLine(0, i, width, i);
	/*
	unsigned int start = 175;
	for(int i = 0; i < height; i++) {
		dc->SetPen(wxPen(wxColour(255, start, start - 100)));
		dc->DrawRectangle(0, i, width, 1);
		if(i < 12 && !isDown)
			start -= i;
		else
			start++;
	}

	dc->SetPen(wxPen(wxColour(255, 220, 180)));
	dc->DrawLine(1, 1, 1, height - 1); // Left
	dc->DrawLine(width - 2, 0, width - 2 , height); // Right
	dc->DrawLine(1, 1, width - 2, 1); // Top
	dc->DrawLine(1, height - 2, width - 2, height - 2); // Bottom
	*/
}

void ToolbarButton::DrawDisabledBackground(wxPaintDC *dc) {
	/*for(unsigned int i = 0; i < height; i++) {
		dc->SetPen(wxPen(i % 2 == 0 ? *wxBLACK : *wxLIGHT_GREY));
		dc->DrawLine(0, i, width, i);
	}*/
}

void ToolbarButton::Connect(const bool connect) {
	if(connect) {
		Connect(wxEVT_LEFT_UP, wxMouseEventHandler(ToolbarButton::OnLeftUp));
		Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ToolbarButton::OnLeftDown));
		Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(ToolbarButton::OnEnter));
		Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(ToolbarButton::OnLeave));
		if(onLeftClick) {
			//Connect(wxEVT_LEFT_UP, onLeftClick, NULL, this->onLeftClickEventHandler);
		}
	} else {
		Disconnect(wxEVT_LEFT_UP, wxMouseEventHandler(ToolbarButton::OnLeftUp));
		Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ToolbarButton::OnLeftDown));
		Disconnect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(ToolbarButton::OnEnter));
		Disconnect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(ToolbarButton::OnLeave));
		if(onLeftClick) {
			//Disconnect(wxEVT_LEFT_UP, onLeftClick, NULL, this->onLeftClickEventHandler);
		}
	}
}

void ToolbarButton::SetEnabled(const bool enable) {
	this->isEnabled = enable;

	if(!enable && isConnected) {
		Connect(false);
		isConnected = false;
	} else if(enable && !isConnected) {
		Connect(true);
		isConnected = true;
	}

	onPressed = false;
	onEnter   = false;

	Refresh();
}

bool ToolbarButton::GetEnabled() const {
	return isEnabled;
}

void ToolbarButton::SetLeftClickEvent(
	wxObjectEventFunction onLeftClick,
	wxEvtHandler *onLeftClickEventHandler
) {
	this->onLeftClick             = onLeftClick;
	this->onLeftClickEventHandler = onLeftClickEventHandler;
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(ToolbarButton::OnInternalLeftUp), NULL, this);
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ToolbarButton::OnInternalLeftDown), NULL, this);
}

void ToolbarButton::OnInternalLeftDown(wxMouseEvent &event) {
	if(onLeftClick && GetEnabled())
		Connect(wxEVT_LEFT_UP, this->onLeftClick, NULL, this->onLeftClickEventHandler);
	event.Skip();
}

void ToolbarButton::OnInternalLeftUp(wxMouseEvent &event) {
	if(onLeftClick && GetEnabled())
		Disconnect(wxEVT_LEFT_UP, this->onLeftClick, NULL, this->onLeftClickEventHandler);
	event.Skip();
}

bool ToolbarXmlResourceHandler::IsOS(const wxString &os, wxString plattform) {
	plattform = plattform.Trim();
	plattform = plattform.Trim(false);
	if(plattform.IsEmpty() || plattform == os)
		return true;
	if(plattform.Find(_N("|")) != wxNOT_FOUND) {
		wxString entry;
		wxArrayString plattforms(DSLib::Split(plattform, '|'));
		const unsigned int count = plattforms.GetCount();
		for(unsigned int i = 0; i < count; i++) {
			entry = plattforms[i];
			entry = entry.Trim();
			entry = entry.Trim(false);
			if(entry == os)
				return true;
		}
	}
	return false;
}

wxObject *ToolbarXmlResourceHandler::DoCreateResource() {
	XRC_MAKE_INSTANCE(toolbar, Toolbar)
	toolbar->Create(m_parentAsWindow, GetID(), GetName());

#ifdef __WXMSW__
	const wxString os = _N("win");
#elif defined __WXMAC__
	const wxString os = _N("mac");
#else
	const wxString os = _N("unix");
#endif

	wxString name;
	wxString object;
	wxString content;
	wxString label;
	wxString bitmap;
	bool disabled;
	bool toggle;
	wxXmlNode *params   = NULL;
	wxXmlNode *contents = GetParamNode(_N("buttons"));
	if(contents)
		contents = contents->GetChildren();
	if(contents) {
		do {
			if(contents->GetName() == _N("ToolbarButton")) {
				label.Clear();
				bitmap.Clear();
				toggle   = false;
				disabled = false;
				#if wxMAJOR_VERSION < 3
					name     = contents->GetPropVal(_N("name"), _N("ToolbarButton"));
				#else
					name     = contents->GetAttribute(_N("name"), _N("ToolbarButton"));
				#endif
				params   = contents->GetChildren();
				if(params) {
					do {
						object    = params->GetName();
						content   = params->GetNodeContent();
						#if wxMAJOR_VERSION < 3
							if(IsOS(os, params->GetPropVal(_N("plattform"), wxEmptyString))) {
						#else
							if(IsOS(os, params->GetAttribute(_N("plattform"), wxEmptyString))) {
						#endif
							if(object == _N("label"))
								label = wxGetTranslation(content);
							else if(object == _N("bitmap"))
								bitmap = content;
							else if(object == _N("toggle"))
								toggle = content != _N("0");
							else if(object == _N("disabled"))
								disabled = content != _N("0");
						}
					} while((params = params->GetNext()));
				}
				if(!label.IsEmpty() && !bitmap.IsEmpty()) {
					ToolbarButton *btn = new ToolbarButton(
						toolbar,
						wxXmlResource::Get()->GetXRCID(name),
						label,
						wxBitmap(bitmap, wxBITMAP_TYPE_PNG),
						toggle,
						name
					);
					btn->SetEnabled(!disabled);
					toolbar->Add(btn);
				}
			} else if(contents->GetName() == _N("ToolbarButtonSpacer")) {
				#if wxMAJOR_VERSION < 3
					toolbar->AddSpacer(DSCasts::ToInt(contents->GetPropVal(_N("width"), DSCasts::ToString(DEFAULT_SPACER_WIDTH))));
				#else
					toolbar->AddSpacer(DSCasts::ToInt(contents->GetAttribute(_N("width"), DSCasts::ToString(DEFAULT_SPACER_WIDTH))));
				#endif
			} else if(contents->GetName() == _N("ToolbarButtonStretchSpacer")) {
				toolbar->AddStretchSpacer();
			}
		} while((contents = contents->GetNext()));
	}

	return toolbar;
}

bool ToolbarXmlResourceHandler::CanHandle(wxXmlNode *node) {
	return IsOfClass(node, _N("Toolbar"));
}
