#include "VoucherEditFrame.h"
#include "../lib/ListEntryController.h"
#include "../lib/VoucherController.h"
#include "../lib/VoucherPositionEntity.h"
#include "../lib/VoucherPositionEntities.h"
#include "../lib/ConstantsLibrary.h"
#include "../lib/ReminderType.h"
#include "../lib/KeyValueDatabaseLibrary.h"
#include "../lib/JasperReportsLibrary.h"
#include <DSCurrency.h>
#include "PrintPreviewDialog.h"
#include "VoucherPositionEditDialog.h"
#include "TemplateSelectDialog.h"
#include "YesNoDialog.h"
#include "ClientsFullFindDialog.h"
#include "RemindersDialog.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"
#include <DSDebug.h>

BEGIN_EVENT_TABLE(VoucherEditFrame, wxFrame)
	EVT_CLOSE(VoucherEditFrame::OnClose)
	EVT_MENU(CLOSE_EVENT, VoucherEditFrame::CloseKeyEvent)
	EVT_MENU(SAVE_EVENT, VoucherEditFrame::SaveKeyEvent)
	EVT_MENU(EDITED_INFORMATIONS_EVENT, VoucherEditFrame::ShowEditedInformationsKeyEvent)
	EVT_MENU(PRINT_EVENT, VoucherEditFrame::PrintKeyEvent)
	EVT_MENU(PRINT_PREVIEW_EVENT, VoucherEditFrame::PrintPreviewKeyEvent)
	EVT_MENU(FIND_COMPANY_EVENT, VoucherEditFrame::FindCompanyKeyEvent)
	EVT_MENU(CALCULATE_EVENT, VoucherEditFrame::CalculateKeyEvent)
	EVT_MENU(REMINDERS_EVENT, VoucherEditFrame::RemindersKeyEvent)
	EVT_MENU(ADD_POSITION_EVENT, VoucherEditFrame::AddPositionKeyEvent)
	EVT_MENU(REMOVE_POSITION_EVENT, VoucherEditFrame::RemovePositionKeyEvent)
	EVT_BUTTON(XRCID("addPositionButton"), VoucherEditFrame::AddPositionEvent)
	EVT_BUTTON(XRCID("editPositionButton"), VoucherEditFrame::EditPositionEvent)
	EVT_BUTTON(XRCID("removePositionButton"), VoucherEditFrame::RemovePositionEvent)
	EVT_LIST_ITEM_ACTIVATED(XRCID("voucherPositionsListCtrl"), VoucherEditFrame::DirectEditPositionEvent)
END_EVENT_TABLE()

VoucherEditFrame::VoucherEditFrame(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	VoucherEntity *voucherEntity,
	GuiCallbackInterface *callbackInterface
) : bind(
	voucherEntity
), changed(
	false
) {
	SetId(id);
	this->rit = rit;
	this->callbackInterface = callbackInterface;
	voucherId = voucherEntity ? voucherEntity->Get(_N("voucherId"))->ToInt() : 0;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("VoucherEditFrame.xml")));
	wxXmlResource::Get()->LoadFrame(this, parent, _N("VoucherEditFrame"));

	mainPanel                        = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer                     = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	saveToolbarButton                = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	closeToolbarButton               = XRCCTRL(*this, "closeToolbarButton", ToolbarButton);
	findCompanyToolbarButton         = XRCCTRL(*this, "findCompanyToolbarButton", ToolbarButton);
	editedInformationsToolbarButton  = XRCCTRL(*this, "editedInformationsToolbarButton", ToolbarButton);
	printToolbarButton               = XRCCTRL(*this, "printToolbarButton", ToolbarButton);
	printPreviewToolbarButton        = XRCCTRL(*this, "printPreviewToolbarButton", ToolbarButton);
	calculateToolbarButton           = XRCCTRL(*this, "calculateToolbarButton", ToolbarButton);
	remindersToolbarButton           = XRCCTRL(*this, "remindersToolbarButton", ToolbarButton);
	voucherIdTextCtrl                = XRCCTRL(*this, "voucherIdTextCtrl", DynSoft::DSTextCtrl);
	descriptionComboBox              = XRCCTRL(*this, "descriptionComboBox", DynSoft::DSComboBox);
	companyTextCtrl                  = XRCCTRL(*this, "companyTextCtrl", DynSoft::DSTextCtrl);
	notesStaticText                  = XRCCTRL(*this, "notesStaticText", wxStaticText);
	notesTextCtrl                    = XRCCTRL(*this, "notesTextCtrl", DynSoft::DSTextCtrl);
	invoiceIdTextCtrl                = XRCCTRL(*this, "invoiceIdTextCtrl", DynSoft::DSTextCtrl);
	invoiceDateDatePickerCtrl        = XRCCTRL(*this, "invoiceDateDatePickerCtrl", DynSoft::DSDatePickerCtrl);
	voucherPositionsListCtrl         = XRCCTRL(*this, "voucherPositionsListCtrl", DynSoft::DSListCtrl);
	priceTextCtrl                    = XRCCTRL(*this, "priceTextCtrl", DynSoft::DSTextCtrl);
	reducedTaxTextCtrl               = XRCCTRL(*this, "reducedTaxTextCtrl", DynSoft::DSTextCtrl);
	fullTaxTextCtrl                  = XRCCTRL(*this, "fullTaxTextCtrl", DynSoft::DSTextCtrl);
	fullPriceTextCtrl                = XRCCTRL(*this, "fullPriceTextCtrl", DynSoft::DSTextCtrl);
	
	KeyValueDatabaseLibrary keyValueLibrary(rit, _N("advanced_preferences"));
	const wxString mediaDefault(keyValueLibrary.Get(_N("vouchers.media.default_description")).ToString());
	wxPanel *tmpMediaPanel           = XRCCTRL(*this, "mediaPanel", wxPanel);
	mediaBoxSizer                    = static_cast<wxBoxSizer *>(tmpMediaPanel->GetSizer());
	mediaPanel                       = new MediaPanel(tmpMediaPanel, -1, rit, _N("linked_files_vouchers"), voucherEntity ? voucherEntity->GetLinkedFileEntities() : NULL, &changed, mediaDefault);
	mediaBoxSizer->Add(mediaPanel, 1, wxEXPAND | wxALL, 5);

	wxPanel *tmpMoreDataPanel        = XRCCTRL(*this, "moreDataPanel", wxPanel);
	moreDataBoxSizer                 = static_cast<wxBoxSizer *>(tmpMoreDataPanel->GetSizer());
	moreDataPanel                    = new MoreDataPanel(tmpMoreDataPanel, -1, rit, voucherEntity->GetKeysValuesType(), &changed);
	moreDataBoxSizer->Add(moreDataPanel, 1, wxEXPAND | wxALL, 5);

	voucherPositionsListCtrl->InsertColumn(0, _("Quantity"), wxLIST_FORMAT_RIGHT, 80);
	voucherPositionsListCtrl->InsertColumn(1, _("Description"), wxLIST_FORMAT_LEFT, 150);
	voucherPositionsListCtrl->InsertColumn(2, _("Article number"), wxLIST_FORMAT_RIGHT, 150);
	voucherPositionsListCtrl->InsertColumn(3, _("Article"), wxLIST_FORMAT_LEFT, 150);
	voucherPositionsListCtrl->InsertColumn(4, _("Kind of tax"), wxLIST_FORMAT_LEFT, 80);
	voucherPositionsListCtrl->InsertColumn(5, _("Tax"), wxLIST_FORMAT_RIGHT, 120);
	voucherPositionsListCtrl->InsertColumn(6, _("Price"), wxLIST_FORMAT_RIGHT, 120);

	// Events
	CONNECT_TOOLBARBUTTON(closeToolbarButton, VoucherEditFrame::CloseEvent);
	CONNECT_TOOLBARBUTTON(saveToolbarButton, VoucherEditFrame::SaveEvent);
	CONNECT_TOOLBARBUTTON(editedInformationsToolbarButton, VoucherEditFrame::ShowEditedInformationsEvent);
	CONNECT_TOOLBARBUTTON(printToolbarButton, VoucherEditFrame::PrintEvent);
	CONNECT_TOOLBARBUTTON(printPreviewToolbarButton, VoucherEditFrame::PrintPreviewEvent);
	CONNECT_TOOLBARBUTTON(findCompanyToolbarButton, VoucherEditFrame::FindCompanyEvent);
	CONNECT_TOOLBARBUTTON(calculateToolbarButton, VoucherEditFrame::CalculateEvent);
	CONNECT_TOOLBARBUTTON(remindersToolbarButton, VoucherEditFrame::RemindersEvent);

	notesStaticText->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(VoucherEditFrame::TemplateSelectEvent), NULL, this);

	wxAcceleratorEntry acceleratorEntries[11];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,              WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,                (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,                (int) 's',  SAVE_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,                (int) 'i',  EDITED_INFORMATIONS_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL,                (int) 'p',  PRINT_EVENT);
	acceleratorEntries[5].Set(wxACCEL_CTRL | wxACCEL_ALT,  (int) 'p',  PRINT_PREVIEW_EVENT);
	acceleratorEntries[6].Set(wxACCEL_CTRL,                (int) '+',  ADD_POSITION_EVENT);
	acceleratorEntries[7].Set(wxACCEL_CTRL,                (int) '-',  REMOVE_POSITION_EVENT);
	acceleratorEntries[8].Set(wxACCEL_CTRL,                (int) 'f',  FIND_COMPANY_EVENT);
	acceleratorEntries[9].Set(wxACCEL_CTRL,                (int) 'k',  CALCULATE_EVENT);
	acceleratorEntries[10].Set(wxACCEL_CTRL | wxACCEL_ALT, (int) 'w',  REMINDERS_EVENT);
	wxAcceleratorTable acceleratorTable(11, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Filling
	ListEntryController::FillComboBox(rit, descriptionComboBox, _N("descriptions"), _N("description"), ListEntryController::FILL_NEW, true);

	// Binding
	bind.Bind(_N("voucherId"),       voucherIdTextCtrl);
	bind.Bind(_N("description"),     descriptionComboBox, DSBind::VALUE);
	bind.Bind(_N("company"),         companyTextCtrl);
	bind.Bind(_N("notes"),           notesTextCtrl);
	bind.Bind(_N("invoiceId"),       invoiceIdTextCtrl);
	bind.Bind(_N("invoiceDate"),     invoiceDateDatePickerCtrl);
	bind.Bind(_N("priceWithoutTax"), priceTextCtrl);
	bind.Bind(_N("reducedTax"),      reducedTaxTextCtrl);
	bind.Bind(_N("fullTax"),         fullTaxTextCtrl);
	bind.Bind(_N("priceWithTax"),    fullPriceTextCtrl);

	if(voucherEntity) {
		rit->Block(_N("vouchers"), voucherId, this);

		DSCurrency price;
		price = voucherEntity->Get(_N("priceWithoutTax"))->ToCurrency();
		voucherEntity->SetValue(_N("priceWithoutTax"), price);
		price = voucherEntity->Get(_N("priceWithTax"))->ToCurrency();
		voucherEntity->SetValue(_N("priceWithTax"), price);
		price = voucherEntity->Get(_N("fullTax"))->ToCurrency();
		voucherEntity->SetValue(_N("fullTax"), price);
		price = voucherEntity->Get(_N("reducedTax"))->ToCurrency();
		voucherEntity->SetValue(_N("reducedTax"), price);

		bind.Push();

		VoucherPositionEntities *voucherPositionEntities = voucherEntity->GetVoucherPositions();
		const unsigned int count = voucherPositionEntities->GetCount();
		for(unsigned int i = 0; i < count; i++)
			AddOrChangeToVoucherPositionsListCtrl(voucherPositionEntities->Get(i));
		
		if(voucherId == 0)
			voucherIdTextCtrl->SetValue(_("<automatic>"));
	}

	SetSize(wxSize(450, 350));

	priceTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);
	fullPriceTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);
	reducedTaxTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);
	fullTaxTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);

	descriptionComboBox->SetChange(&changed);
	companyTextCtrl->SetChange(&changed);
	notesTextCtrl->SetChange(&changed);
	invoiceIdTextCtrl->SetChange(&changed);
	invoiceDateDatePickerCtrl->SetChange(&changed);
	voucherPositionsListCtrl->SetChange(&changed);
	priceTextCtrl->SetChange(&changed);
	reducedTaxTextCtrl->SetChange(&changed);
	fullTaxTextCtrl->SetChange(&changed);
	fullPriceTextCtrl->SetChange(&changed);

	this->editedInformationsDialog = new EditedInformationsDialog(this, -1, rit, voucherEntity);
	this->editedInformationsDialog->Show(false);

	descriptionComboBox->SetFocus();

	changed = false;

	mainBoxSizer->SetSizeHints(this);

	mediaBoxSizer->RecalcSizes();
	moreDataBoxSizer->RecalcSizes();

	SetSize(wxSize(1000, 600));

	ICON();
}

VoucherEditFrame::~VoucherEditFrame() {
	editedInformationsDialog->Destroy();
}

// Events
void VoucherEditFrame::OnClose(wxCloseEvent &event) {
	bool ok = true;

	if(changed) {
		YesNoDialog *yesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_("You have changed the data. Do you want to save the changes?"),
			_("Close"),
			_("Save"),
			_("Close")
		);
		yesNoDialog->Center();
		if(yesNoDialog->ShowModal() == wxID_YES)
			ok = Save();
		yesNoDialog->Destroy();
	}

	if(ok) {
		VoucherEntity *voucherEntity = static_cast<VoucherEntity *>(bind.GetEntity());
		delete voucherEntity;
		rit->Unblock(_N("vouchers"), voucherId);

		Destroy();
		event.Skip();
	} else {
		event.Veto();
	}
}

void VoucherEditFrame::CloseEvent(wxMouseEvent &event) {
	Close();
	SKIP();
}

void VoucherEditFrame::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}

void VoucherEditFrame::PrintEvent(wxMouseEvent &event) {
	Print();
	SKIP();
}

void VoucherEditFrame::PrintPreviewEvent(wxMouseEvent &event) {
	Print(true);
	SKIP();
}

void VoucherEditFrame::ShowEditedInformationsEvent(wxMouseEvent &event) {
	ShowEditedInformations();
	SKIP();
}

void VoucherEditFrame::CloseKeyEvent(wxCommandEvent &event) {
	Close();
}

void VoucherEditFrame::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void VoucherEditFrame::PrintKeyEvent(wxCommandEvent &event) {
	Print();
}

void VoucherEditFrame::PrintPreviewKeyEvent(wxCommandEvent &event) {
	Print(true);
}

void VoucherEditFrame::ShowEditedInformationsKeyEvent(wxCommandEvent &event) {
	ShowEditedInformations();
}

void VoucherEditFrame::AddPositionEvent(wxCommandEvent &event) {
	AddPosition();
	SKIP();
}

void VoucherEditFrame::EditPositionEvent(wxCommandEvent &event) {
	EditPosition();
	SKIP();
}

void VoucherEditFrame::RemovePositionEvent(wxCommandEvent &event) {
	RemovePosition();
	SKIP();
}

void VoucherEditFrame::AddPositionKeyEvent(wxCommandEvent &event) {
	AddPosition();
}

void VoucherEditFrame::EditPositionKeyEvent(wxCommandEvent &event) {
	EditPosition();
}

void VoucherEditFrame::DirectEditPositionEvent(wxListEvent &event) {
	EditPosition();
}

void VoucherEditFrame::RemovePositionKeyEvent(wxCommandEvent &event) {
	RemovePosition();
}

void VoucherEditFrame::TemplateSelectEvent(wxMouseEvent &event) {
	TemplateSelect(event.GetEventObject());
}

void VoucherEditFrame::FindCompanyEvent(wxMouseEvent &event) {
	FindCompany();
	SKIP();
}

void VoucherEditFrame::FindCompanyKeyEvent(wxCommandEvent &event) {
	FindCompany();
}

void VoucherEditFrame::CalculateEvent(wxMouseEvent &event) {
	Calculate();
	SKIP();
}

void VoucherEditFrame::CalculateKeyEvent(wxCommandEvent &event) {
	Calculate();
}

void VoucherEditFrame::RemindersEvent(wxMouseEvent &event) {
	Reminders();
	SKIP();
}

void VoucherEditFrame::RemindersKeyEvent(wxCommandEvent &event) {
	Reminders();
}

// Methods
void VoucherEditFrame::Close() {
	wxWindow::Close();
}

bool VoucherEditFrame::Save() {
	voucherIdTextCtrl->SetFocus();

	if(!rit->Access(A_WRITE, _("You have no permission to save this voucher."), this))
		return false;

	if(
		invoiceIdTextCtrl->GetValue().IsEmpty() ||
		companyTextCtrl->GetValue().IsEmpty() ||
		priceTextCtrl->GetValue().IsEmpty() ||
		reducedTaxTextCtrl->GetValue().IsEmpty() ||
		fullTaxTextCtrl->GetValue().IsEmpty() ||
		fullPriceTextCtrl->GetValue().IsEmpty()
	) {
		rit->Log(true, _N("VoucherEditFrame"), _("The voucher is not saved."), _("Not all necessary fiels are filled out."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please fill in the mandatory fields."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return false;
	}

	VoucherEntity *voucherEntity = static_cast<VoucherEntity *>(bind.GetEntity());
	if(!voucherEntity) {
		rit->Log(true, _N("VoucherEditFrame"), _("The voucher is not saved."), _("Null Pointer."));
		return false;
	}

	bool ok = true;
	if(!CalculateCheck()) {
		YesNoDialog *priceYesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_("The prices you entered are not equal to the position prices. Do you want to continue?\n\nPlease use Calculate to recalculate the prices."),
			_("Close"),
			_("Go on"),
			_("Cancel")
		);
		priceYesNoDialog->Center();
		ok = priceYesNoDialog->ShowModal() == wxID_YES;
		priceYesNoDialog->Destroy();

		if(!ok)
			return false;
	}

	bind.Pull();
	voucherEntity->SetValue(_N("voucherId"), voucherId);
	DSCurrency price;
	price = voucherEntity->Get(_N("priceWithoutTax"))->ToCurrency();
	voucherEntity->SetValue(_N("priceWithoutTax"), price);
	price = voucherEntity->Get(_N("priceWithTax"))->ToCurrency();
	voucherEntity->SetValue(_N("priceWithTax"), price);
	price = voucherEntity->Get(_N("reducedTax"))->ToCurrency();
	voucherEntity->SetValue(_N("reducedTax"), price);
	price = voucherEntity->Get(_N("fullTax"))->ToCurrency();
	voucherEntity->SetValue(_N("fullTax"), price);

	VoucherController voucherController(rit);
	ok = voucherController.Save(voucherEntity);

	if(ok) {
		voucherId = voucherEntity->Get(_N("voucherId"))->ToInt();
		voucherController.Get(DSCasts::ToString(voucherId)); // Autofelder refreshen
		voucherController.Run();
		voucherController.Get(voucherEntity);
		voucherController.Reset();
		
		price = voucherEntity->Get(_N("priceWithoutTax"))->ToCurrency();
		voucherEntity->SetValue(_N("priceWithoutTax"), price);
		price = voucherEntity->Get(_N("priceWithTax"))->ToCurrency();
		voucherEntity->SetValue(_N("priceWithTax"), price);
		price = voucherEntity->Get(_N("fullTax"))->ToCurrency();
		voucherEntity->SetValue(_N("fullTax"), price);
		price = voucherEntity->Get(_N("reducedTax"))->ToCurrency();
		voucherEntity->SetValue(_N("reducedTax"), price);

		bind.Push();

		if(callbackInterface)
			callbackInterface->Callback(static_cast<void *>(voucherEntity));

		rit->Block(_N("vouchers"), voucherId, this);

		changed = false;

		rit->Log(false, _N("VoucherEditFrame"), _("The voucher is saved."), _("The voucher with voucher id \"") + DSCasts::ToString(voucherId) +  _("\" is saved."));

		editedInformationsDialog->Update();
	} else {
		rit->Log(true, _N("VoucherEditFrame"), _("The voucher is not saved."), _("Database error."));

		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Could not save."),
			_("Save voucher")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
	}

	return ok;
}

void VoucherEditFrame::ShowEditedInformations() {
	editedInformationsDialog->Get(!editedInformationsDialog->IsShown());
}

void VoucherEditFrame::Print(const bool preview) {
	voucherIdTextCtrl->SetFocus();

	if(!rit->Access(A_READ, _("You have no permission to print."), this))
		return;

	if(changed) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Save first this entry and press print again. Only saved entries can be printed."),
			_("Print")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	if(!JasperReportsLibrary::DoReport(
		this,
		rit,
		_N("vouchers"),
		voucherId,
		preview ? JasperReportsController::SHOW : JasperReportsController::PRINT
	)) {
		VoucherController voucherController(rit);
		HtmlPrintoutLibrary *htmlPrintoutLibrary = voucherController.Print(static_cast<VoucherEntity *>(bind.GetEntity()));

		if(preview) {
			PrintPreviewDialog *printPreviewDialog = new PrintPreviewDialog(this, -1, rit, htmlPrintoutLibrary);
			printPreviewDialog->Center();
			printPreviewDialog->ShowModal();
			printPreviewDialog->Destroy();
		} else {
			wxPrinter printer;
			printer.Print(this, htmlPrintoutLibrary, true);
		}

		delete htmlPrintoutLibrary;
	}
}

void VoucherEditFrame::TemplateSelect(wxObject *object) {
	DSTextCtrl *textCtrl = NULL;

	if(object == notesStaticText)
		textCtrl = notesTextCtrl;

	TemplateSelectDialog::ShowAndSet(this, rit, textCtrl);
}

void VoucherEditFrame::AddPosition() {
	if(!rit->Access(A_WRITE, _("You have no permission to add a new entry."), this))
		return;

	VoucherPositionEntity *voucherPositionEntity         = new VoucherPositionEntity(rit);
	VoucherPositionEditDialog *voucherPositionEditDialog = new VoucherPositionEditDialog(
		this,
		-1,
		rit,
		voucherPositionEntity
	);
	voucherPositionEditDialog->Center();
	if(voucherPositionEditDialog->ShowModal() == wxID_OK) {
		VoucherEntity *voucherEntity = static_cast<VoucherEntity *>(bind.GetEntity());
		voucherEntity->GetVoucherPositions()->Add(voucherPositionEntity);
		AddOrChangeToVoucherPositionsListCtrl(voucherPositionEntity);

		changed = true;
	} else {
		delete voucherPositionEntity;
	}
	voucherPositionEditDialog->Destroy();
}

void VoucherEditFrame::EditPosition() {
	if(!rit->Access(A_READ, _("You have no permission to edit this entry."), this))
		return;

	VoucherPositionEntity *voucherPositionEntity = static_cast<VoucherPositionEntity *>(voucherPositionsListCtrl->GetSingleSelectionPointer());
	if(!voucherPositionEntity) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the entry you want to edit."),
			_("Remove")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();

		return;
	}

	VoucherPositionEditDialog *voucherPositionEditDialog = new VoucherPositionEditDialog(
		this,
		-1,
		rit,
		voucherPositionEntity,
		rit->Access(A_WRITE)
	);
	voucherPositionEditDialog->Center();
	if(voucherPositionEditDialog->ShowModal() == wxID_OK)
		AddOrChangeToVoucherPositionsListCtrl(voucherPositionEntity);
	voucherPositionEditDialog->Destroy();
}

void VoucherEditFrame::RemovePosition() {
	if(!rit->Access(A_DELETE, _("You have no permission to delete this entry."), this))
		return;

	VoucherPositionEntity *voucherPositionEntity = static_cast<VoucherPositionEntity *>(voucherPositionsListCtrl->GetSingleSelectionPointer());
	if(!voucherPositionEntity) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the entry you want to remove."),
			_("Remove")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();

		return;
	}

	YesNoDialog *yesNoDialog = new YesNoDialog(
		this,
		-1,
		rit,
		_("Do you really want to remove the selected entry?"),
		_("Remove"),
		_("Remove"),
		_("Cancel")
	);
	yesNoDialog->Center();
	if(yesNoDialog->ShowModal() == wxID_YES) {
		if(voucherPositionEntity->Get(_N("voucherPositionId"))->ToUnsignedInt() != 0)
			removedVoucherPositions.Add(voucherPositionEntity->Get(_N("voucherPositionId"))->ToString());
		voucherPositionsListCtrl->DeleteItem(voucherPositionsListCtrl->GetPosition(voucherPositionEntity));
		VoucherEntity *voucherEntity = static_cast<VoucherEntity *>(bind.GetEntity());
		voucherEntity->GetVoucherPositions()->Remove(voucherPositionEntity, true);

		changed = true;
	}
	yesNoDialog->Destroy();
}

void VoucherEditFrame::AddOrChangeToVoucherPositionsListCtrl(VoucherPositionEntity *voucherPositionEntity) {
	if(!voucherPositionEntity)
		return;

	DSListCtrl::DSListCtrlRow *voucherPositionsListCtrlRow = new DSListCtrl::DSListCtrlRow(voucherPositionEntity);

	wxString description(voucherPositionEntity->Get(_N("description"))->ToString());
	description.Replace(_N("\n"), _N(", "));

	voucherPositionsListCtrlRow->Add(voucherPositionEntity->Get(_N("quantity"))->ToString());
	voucherPositionsListCtrlRow->Add(description);
	voucherPositionsListCtrlRow->Add(voucherPositionEntity->Get(_N("articleNumber"))->ToString());
	voucherPositionsListCtrlRow->Add(voucherPositionEntity->Get(_N("article"))->ToString());
	voucherPositionsListCtrlRow->Add(ConstantsLibrary::TaxKindToString(voucherPositionEntity->Get(_N("tax"))->ToUnsignedInt()));
	voucherPositionsListCtrlRow->Add(voucherPositionEntity->Get(_N("taxPrice"))->ToCurrency().ToString());
	voucherPositionsListCtrlRow->Add(voucherPositionEntity->Get(_N("price"))->ToCurrency().ToString());

	voucherPositionsListCtrl->Set(voucherPositionsListCtrlRow, true);
}

void VoucherEditFrame::FindCompany() {
	companyTextCtrl->SetFocus();

	ClientsFullFindDialog *clientsFullFindDialog = new ClientsFullFindDialog(this, -1, rit);
	clientsFullFindDialog->Center();
	if(clientsFullFindDialog->ShowModal() == wxID_OK) {
		ClientEntity *clientEntity = clientsFullFindDialog->GetClientEntity();
		if(clientEntity)
			companyTextCtrl->SetValue(clientEntity->GetMainData());
	}
}

void VoucherEditFrame::Calculate() {
	DSCurrency price, reducedTax, fullTax, sum;

	CalculateProof(price, reducedTax, fullTax, sum);

	priceTextCtrl->SetValue(price.ToString());
	reducedTaxTextCtrl->SetValue(reducedTax.ToString());
	fullTaxTextCtrl->SetValue(fullTax.ToString());
	fullPriceTextCtrl->SetValue(sum.ToString());
}

void VoucherEditFrame::CalculateProof(DSCurrency &price, DSCurrency &reducedTax, DSCurrency &fullTax, DSCurrency &sum) {
	VoucherPositionEntity *voucherPositionEntity = NULL;
	VoucherPositionEntities *voucherPositionEntites = static_cast<VoucherEntity *>(bind.GetEntity())->GetVoucherPositions();
	const unsigned int count = voucherPositionEntites->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		voucherPositionEntity = voucherPositionEntites->Get(i);
		if(!voucherPositionEntity)
			continue;

		price += voucherPositionEntity->Get(_N("price"))->ToCurrency();
		switch(voucherPositionEntity->Get(_N("tax"))->ToUnsignedInt()) {
			case 0:
				fullTax += voucherPositionEntity->Get(_N("taxPrice"))->ToCurrency();
				break;
			case 1:
				reducedTax += voucherPositionEntity->Get(_N("taxPrice"))->ToCurrency();
				break;
		}
	}

	sum = price + reducedTax + fullTax;
}

bool VoucherEditFrame::CalculateCheck() {
	DSCurrency price, reducedTax, fullTax, sum;

	CalculateProof(price, reducedTax, fullTax, sum);

	return
		priceTextCtrl->GetValue() == price.ToString() &&
		reducedTaxTextCtrl->GetValue() == reducedTax.ToString() &&
		fullTaxTextCtrl->GetValue() == fullTax.ToString() &&
		fullPriceTextCtrl->GetValue() == sum.ToString();
}

void VoucherEditFrame::Reminders() {
	if(!rit->Access(A_READ, _("You have no permission to open reminders."), this))
		return;

	if(voucherId == 0) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please save first."),
			_("Reminders")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	RemindersDialog *remindersDialog = new RemindersDialog(
		this,
		-1,
		rit,
		ReminderType::VOUCHERS,
		voucherId
	);
	remindersDialog->Center();
	remindersDialog->ShowModal();
	remindersDialog->Destroy();
}
