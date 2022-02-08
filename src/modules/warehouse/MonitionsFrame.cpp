#include "MonitionsFrame.h"
#include <DSDateTime.h>
#include "ClientsFullFindDialog.h"
#include "Configuration.h"
#include "ProgressDialog.h"
#include "YesNoDialog.h"
#include "PrintPreviewDialog.h"
#include "../lib/CommonPreferencesController.h"
#include "../lib/MonitionController.h"
#include "../lib/MonitionTextEntity.h"
#include "../lib/MonitionTextController.h"
#include "../lib/HtmlPrintoutLibrary.h"
#include "../lib/OrderController.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(MonitionsFrame, wxFrame)
	EVT_MENU(CLOSE_EVENT, MonitionsFrame::CloseKeyEvent)
	EVT_MENU(FIND_CLIENT_EVENT, MonitionsFrame::FindClientKeyEvent)
	EVT_MENU(UPDATE_EVENT, MonitionsFrame::UpdateKeyEvent)
	EVT_MENU(ALL_EVENT, MonitionsFrame::AllKeyEvent)
	EVT_MENU(NONE_EVENT, MonitionsFrame::NoneKeyEvent)
	EVT_MENU(PLUS_EVENT, MonitionsFrame::PlusKeyEvent)
	EVT_MENU(MINUS_EVENT, MonitionsFrame::MinusKeyEvent)
	EVT_MENU(TEXTS_EVENT, MonitionsFrame::TextsKeyEvent)
	EVT_MENU(RUN_EVENT, MonitionsFrame::RunKeyEvent)
	EVT_CLOSE(MonitionsFrame::HideEvent)
END_EVENT_TABLE()

MonitionsFrame::MonitionsFrame(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("MonitionsFrame.xml")));
	wxXmlResource::Get()->LoadFrame(this, parent, _N("MonitionsFrame"));

	mainPanel               = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer            = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	updateToolbarButton    = XRCCTRL(*this, "updateToolbarButton", ToolbarButton);
	findClientToolbarButton = XRCCTRL(*this, "findClientToolbarButton", ToolbarButton);
	allToolbarButton        = XRCCTRL(*this, "allToolbarButton", ToolbarButton);
	noneToolbarButton       = XRCCTRL(*this, "noneToolbarButton", ToolbarButton);
	plusToolbarButton       = XRCCTRL(*this, "plusToolbarButton", ToolbarButton);
	minusToolbarButton      = XRCCTRL(*this, "minusToolbarButton", ToolbarButton);
	textsToolbarButton      = XRCCTRL(*this, "textsToolbarButton", ToolbarButton);
	runToolbarButton        = XRCCTRL(*this, "runToolbarButton", ToolbarButton);
	closeToolbarButton      = XRCCTRL(*this, "closeToolbarButton", ToolbarButton);
	clientIdTextCtrl        = XRCCTRL(*this, "clientIdTextCtrl", DynSoft::DSTextCtrl);
	orderIdTextCtrl         = XRCCTRL(*this, "orderIdTextCtrl", DynSoft::DSTextCtrl);
	invoiceIdTextCtrl       = XRCCTRL(*this, "invoiceIdTextCtrl", DynSoft::DSTextCtrl);
	dateFromStaticText      = XRCCTRL(*this, "dateFromStaticText", wxStaticText);
	dateFromDatePickerCtrl  = XRCCTRL(*this, "dateFromDatePickerCtrl", wxDatePickerCtrl);
	dateToStaticText        = XRCCTRL(*this, "dateToStaticText", wxStaticText);
	dateToDatePickerCtrl    = XRCCTRL(*this, "dateToDatePickerCtrl", wxDatePickerCtrl);
	statesListBox           = XRCCTRL(*this, "statesListBox", wxListBox);
	monitionsListCtrl       = XRCCTRL(*this, "monitionsListCtrl", DynSoft::DSListCtrl);
	priceTextCtrl           = XRCCTRL(*this, "priceTextCtrl", DynSoft::DSTextCtrl);

	clientIdTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	orderIdTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	invoiceIdTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	priceTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);

	monitionsListCtrl->InsertColumn(0, _("Pos."), wxLIST_FORMAT_RIGHT, 50);
	monitionsListCtrl->InsertColumn(1, _("State"), wxLIST_FORMAT_RIGHT, 50);
	monitionsListCtrl->InsertColumn(2, _("Customer id"), wxLIST_FORMAT_RIGHT, 110);
	monitionsListCtrl->InsertColumn(3, _("Order id"), wxLIST_FORMAT_RIGHT, 130);
	monitionsListCtrl->InsertColumn(4, _("Invoice id"), wxLIST_FORMAT_RIGHT, 130);
	monitionsListCtrl->InsertColumn(5, _("Invoice date"), wxLIST_FORMAT_LEFT, 130);
	monitionsListCtrl->InsertColumn(6, _("Price"), wxLIST_FORMAT_RIGHT, 110);

	mainBoxSizer->SetSizeHints(this);
	SetSize(wxSize(950, 600));

	CONNECT_TOOLBARBUTTON(updateToolbarButton, MonitionsFrame::UpdateEvent);
	CONNECT_TOOLBARBUTTON(findClientToolbarButton, MonitionsFrame::FindClientEvent);
	CONNECT_TOOLBARBUTTON(allToolbarButton, MonitionsFrame::AllEvent);
	CONNECT_TOOLBARBUTTON(noneToolbarButton, MonitionsFrame::NoneEvent);
	CONNECT_TOOLBARBUTTON(plusToolbarButton, MonitionsFrame::PlusEvent);
	CONNECT_TOOLBARBUTTON(minusToolbarButton, MonitionsFrame::MinusEvent);
	CONNECT_TOOLBARBUTTON(textsToolbarButton, MonitionsFrame::TextsEvent);
	CONNECT_TOOLBARBUTTON(runToolbarButton, MonitionsFrame::RunEvent);
	CONNECT_TOOLBARBUTTON(closeToolbarButton, MonitionsFrame::CloseEvent);

	wxAcceleratorEntry acceleratorEntries[10];
	acceleratorEntries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,   (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,   (int) 'r',  UPDATE_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,   (int) 'a',  ALL_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL,   (int) 'n',  NONE_EVENT);
	acceleratorEntries[5].Set(wxACCEL_CTRL,   (int) '+',  PLUS_EVENT);
	acceleratorEntries[6].Set(wxACCEL_CTRL,   (int) '-',  MINUS_EVENT);
	acceleratorEntries[7].Set(wxACCEL_CTRL,   (int) 't',  TEXTS_EVENT);
	acceleratorEntries[8].Set(wxACCEL_CTRL,   (int) 'j',  RUN_EVENT);
	acceleratorEntries[9].Set(wxACCEL_CTRL,   (int) 'g',  FIND_CLIENT_EVENT);
	wxAcceleratorTable acceleratorTable(10, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Stuff
	monitionsTextsDialog = new MonitionsTextsDialog(this, -1, rit);
	monitionsTextsDialog->CenterOnScreen();

	// Fill stuff
	MonitionTextController monitionTextController(rit);
	const int states = CommonPreferencesController::GetMonitionStates(rit, 3);
	for(int number = 1; number <= states; number++) {
		statesListBox->Append(
			_("Monition state ") + DSCasts::ToString(number),
			monitionTextController.GetDirect(number, true)
		);
		statesListBox->Select(number - 1);
	}

	priceTextCtrl->SetValue(CommonPreferencesController::GetMonitionPrice(rit).ToString());

	wxDateTime fromDateTime(dateFromDatePickerCtrl->GetValue());
	fromDateTime.SetDay(1);
	dateFromDatePickerCtrl->SetValue(fromDateTime);

	clientIdTextCtrl->SetFocus();

	ICON();
}

MonitionsFrame::~MonitionsFrame() {
	CleanUp();
	CleanUpMonitions();
}

void MonitionsFrame::CloseEvent(wxMouseEvent &event) {
	Close();
	SKIP();
}

void MonitionsFrame::CloseKeyEvent(wxCommandEvent &event) {
	Close();
}

void MonitionsFrame::UpdateEvent(wxMouseEvent &event) {
	Update();
	SKIP();
}

void MonitionsFrame::FindClientEvent(wxMouseEvent &event) {
	FindClient();
	SKIP();
}

void MonitionsFrame::FindClientKeyEvent(wxCommandEvent &event) {
	FindClient();
}

void MonitionsFrame::UpdateKeyEvent(wxCommandEvent &event) {
	Update();
}

void MonitionsFrame::AllEvent(wxMouseEvent &event) {
	All();
	SKIP();
}

void MonitionsFrame::AllKeyEvent(wxCommandEvent &event) {
	All();
}

void MonitionsFrame::NoneEvent(wxMouseEvent &event) {
	None();
	SKIP();
}

void MonitionsFrame::NoneKeyEvent(wxCommandEvent &event) {
	None();
}

void MonitionsFrame::PlusEvent(wxMouseEvent &event) {
	Plus();
	SKIP();
}

void MonitionsFrame::PlusKeyEvent(wxCommandEvent &event) {
	Plus();
}

void MonitionsFrame::MinusEvent(wxMouseEvent &event) {
	Minus();
	SKIP();
}

void MonitionsFrame::MinusKeyEvent(wxCommandEvent &event) {
	Minus();
}

void MonitionsFrame::TextsEvent(wxMouseEvent &event) {
	Texts();
	SKIP();
}

void MonitionsFrame::TextsKeyEvent(wxCommandEvent &event) {
	Texts();
}

void MonitionsFrame::RunEvent(wxMouseEvent &event) {
	Run();
	SKIP();
}

void MonitionsFrame::RunKeyEvent(wxCommandEvent &event) {
	Run();
}

void MonitionsFrame::HideEvent(wxCloseEvent &event) {
	Close();
}

void MonitionsFrame::Update() {
	if(!rit->Access(A_READ, _("You have no permission to view monitions."), this))
		return;

	wxArrayString monitionStates;
	const unsigned int monitionStatesCount = statesListBox->GetCount();
	for(unsigned int i = 0; i < monitionStatesCount; i++) {
		if(statesListBox->IsSelected(i))
			monitionStates.Add(DSCasts::ToString(i));
	}

	if(monitionStates.GetCount() == 0) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select at least one monition state to filter."),
			_("Error")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	CleanUpMonitions();

	DSDateTime from, to;
	from.SetWxDateTime(dateFromDatePickerCtrl->GetValue());
	to.SetWxDateTime(dateToDatePickerCtrl->GetValue());

	MonitionController monitionController(rit);
	monitionController.GetUnpayedOrders(
		orderIdTextCtrl->GetValue(),
		invoiceIdTextCtrl->GetValue(),
		clientIdTextCtrl->GetValue(),
		from.ToDatabaseDate(),
		to.ToDatabaseDate(),
		monitionStates
	);

	const unsigned int count = monitionController.GetCount();
	ProgressDialog *progressDialog = new ProgressDialog(this, -1, rit, count);
	progressDialog->Center();
	progressDialog->Start();
	for(unsigned int i = 0; i < count; i++) {
		AddToMonitionsListCtrl(i + 1, monitionController.GetUnpayedOrder());
		progressDialog->SetValue(i + 1);
		if(progressDialog->IsCanceled()) {
			monitionController.Reset();
			break;
		}
	}
	progressDialog->Stop();

	monitionsListCtrl->SetFocus();
}

void MonitionsFrame::All() {
	monitionsListCtrl->SelectAll(true);
}

void MonitionsFrame::None() {
	monitionsListCtrl->SelectAll(false);
}

void MonitionsFrame::Plus() {
	SetMonition(true);
}

void MonitionsFrame::Minus() {
	SetMonition(false);
}

void MonitionsFrame::Texts() {
	monitionsTextsDialog->Show(!monitionsTextsDialog->IsShown());
}

void MonitionsFrame::Run() {
	if(!rit->Access(A_WRITE, _("You have no permission to generate monitions."), this))
		return;

	if(monitionsListCtrl->GetSingleSelectionPosition() == wxNOT_FOUND) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select at least one monition."),
			_("Error")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	YesNoDialog *runYesNoDialog = new YesNoDialog(
		this,
		-1,
		rit,
		_(
			"Please select \"Generate monitions\" to generate monitions.\n\n"
			"After creating you get the monitions in letter form.\n\n"
			"After closing the preview (use print to print the monitions) "
			"you will be asked to increment the monition state. If the "
			"monition creation was successful, you should increment the states. "
		),
		_("Generate monitions"),
		_("Generate monitions"),
		_("Cancel")
	);
	runYesNoDialog->Center();
	const unsigned int run = runYesNoDialog->ShowModal() == wxID_YES;
	runYesNoDialog->Destroy();

	if(!run)
		return;

	HtmlPrintoutLibrary *htmlPrintoutLibrary = new HtmlPrintoutLibrary(_("Monitions"), rit);
	wxString content;
	MonitionController monitionController(rit);

	unsigned int generateCount = 0;
	const unsigned int count = monitionsListCtrl->GetItemCount();
	ProgressDialog *progressDialog = new ProgressDialog(this, -1, rit, count);
	progressDialog->Center();
	progressDialog->Start();
	for(unsigned int i = 0; i < count; i++) {
		if(monitionsListCtrl->IsRowSelected(i)) {
			generateCount++;

			content += monitionController.Print(
				htmlPrintoutLibrary,
				static_cast<MonitionEntity *>(monitionsListCtrl->GetPointer(i, 0)),
				priceTextCtrl->GetCurrencyValue()
			);

			progressDialog->SetValue(i + 1);
			if(progressDialog->IsCanceled()) {
				monitionController.Reset();
				break;
			}
		}
	}
	progressDialog->Stop();

	htmlPrintoutLibrary->SetWithContainer(content);
	PrintPreviewDialog *printPreviewDialog = new PrintPreviewDialog(this, -1, rit, htmlPrintoutLibrary);
	printPreviewDialog->Center();
	printPreviewDialog->ShowModal();
	const bool printed = printPreviewDialog->Printed();
	printPreviewDialog->Destroy();

	delete htmlPrintoutLibrary;

	rit->Log(false, _N("MonitionsFrame"), _("Generated ") + DSCasts::ToString(generateCount) + _N(" monition letters."));
	if(printed)
		SetMonition(true);
}

void MonitionsFrame::Close() {
	Hide();
}

void MonitionsFrame::FindClient() {
	ClientsFullFindDialog *clientsFullFindDialog = new ClientsFullFindDialog(this, -1, rit);
	clientsFullFindDialog->Center();
	if(clientsFullFindDialog->ShowModal() == wxID_OK) {
		ClientEntity *clientEntity = clientsFullFindDialog->GetClientEntity();
		if(clientEntity)
			clientIdTextCtrl->SetValue(clientEntity->Get(_N("clientId"))->ToString());
	}

	clientIdTextCtrl->SetFocus();
}

void MonitionsFrame::CleanUp() {
	const unsigned int count = statesListBox->GetCount();
	for(unsigned int i = 0; i < count; i++)
		delete static_cast<MonitionTextEntity *>(statesListBox->GetClientData(i));
	statesListBox->Clear();
}

void MonitionsFrame::AddToMonitionsListCtrl(const unsigned int position, MonitionEntity *monitionEntity) {
	DSListCtrl::DSListCtrlRow *monitionsListCtrlRow = new DSListCtrl::DSListCtrlRow(monitionEntity);

	monitionsListCtrlRow->Add(DSCasts::ToString(position));
	monitionsListCtrlRow->Add(DSCasts::ToString(monitionEntity->Get(_N("monitionState"))->ToUnsignedInt() + 1));
	monitionsListCtrlRow->Add(monitionEntity->Get(_N("clientId"))->ToString());
	monitionsListCtrlRow->Add(monitionEntity->Get(_N("orderId"))->ToString());
	monitionsListCtrlRow->Add(monitionEntity->Get(_N("invoiceId"))->ToString());
	monitionsListCtrlRow->Add(monitionEntity->Get(_N("invoiceDate"))->ToString());
	monitionsListCtrlRow->Add(monitionEntity->Get(_N("price"))->ToString());

	monitionsListCtrl->Set(monitionsListCtrlRow, true);
}

void MonitionsFrame::CleanUpMonitions() {
	monitionsListCtrl->CleanUp();
}

void MonitionsFrame::SetMonition(const bool increment) {
	if(!rit->Access(A_WRITE, _("You have no permission to change monition state."), this))
		return;

	YesNoDialog *setYesNoDialog = new YesNoDialog(
		this,
		-1,
		rit,
		_("Do you really want to set the monition state to the selected monitions?"),
		_("Set monition state")
	);
	setYesNoDialog->Center();
	const bool doIt = setYesNoDialog->ShowModal() == wxID_YES;
	setYesNoDialog->Destroy();

	if(!doIt)
		return;

	OrderController orderController(rit);
	MonitionController monitionController(rit);
	MonitionEntity *monitionEntity = NULL;
	bool ok = false;
	int newMonitionState = 0;

	const unsigned int count = monitionsListCtrl->GetItemCount();
	ProgressDialog *progressDialog = new ProgressDialog(this, -1, rit, count);
	progressDialog->Center();
	progressDialog->Start();
	for(unsigned int i = 0; i < count; i++) {
		if(monitionsListCtrl->IsRowSelected(i)) {
			monitionEntity = static_cast<MonitionEntity *>(monitionsListCtrl->GetPointer(i, 0));
			ok = monitionEntity != NULL;

			if(ok)
				ok = orderController.SetMonitionState(monitionEntity->Get(_N("orderId"))->ToUnsignedInt(), increment, &newMonitionState);

			if(ok) {
				monitionEntity->SetValue(_N("monitionState"), newMonitionState);
				AddToMonitionsListCtrl(i + 1, monitionEntity);
				rit->Log(false, _N("MonitionsFrame"), _("Setted monition state to ") + DSCasts::ToString(newMonitionState));

				if(monitionController.Insert(monitionEntity)) {
					rit->Log(false, _N("MonitionsFrame"), _("Monition state persisted in log table ") + DSCasts::ToString(newMonitionState));
				} else {
					rit->Log(true, _N("MonitionsFrame"), _("Could not persist monition state in log table ") + DSCasts::ToString(newMonitionState) + _(". Database error"));
					wxMessageDialog *errorMessageDialog = new wxMessageDialog(
						this,
						_("Could not persist monition state in log table ") + DSCasts::ToString(newMonitionState) + _(". Database error"),
						_("Error")
					);
					errorMessageDialog->Center();
					errorMessageDialog->ShowModal();
					errorMessageDialog->Destroy();
				}
			} else {
				rit->Log(true, _N("MonitionsFrame"), _("Could not set monition state to ") + DSCasts::ToString(newMonitionState) + _(". Database error"));
				wxMessageDialog *errorMessageDialog = new wxMessageDialog(
					this,
					_("Could not set monition state to ") + DSCasts::ToString(newMonitionState) + _(". Database error"),
					_("Error")
				);
				errorMessageDialog->Center();
				errorMessageDialog->ShowModal();
				errorMessageDialog->Destroy();
			}

			progressDialog->SetValue(i + 1);
			if(progressDialog->IsCanceled())
				break;
		}
	}
	progressDialog->Stop();
}
