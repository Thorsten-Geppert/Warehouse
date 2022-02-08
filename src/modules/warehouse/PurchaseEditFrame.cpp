#include "PurchaseEditFrame.h"
#include "../lib/ListEntryController.h"
#include "../lib/EnterprisePreferencesController.h"
#include "../lib/EnterprisePreferencesEntity.h"
#include "../lib/ClientController.h"
#include "../lib/Tools.h"
#include "../lib/ProductController.h"
#include "../lib/ProductEntity.h"
#include "../lib/PurchaseController.h"
#include "../lib/ReminderType.h"
#include "../lib/KeyValueDatabaseLibrary.h"
#include "../lib/JasperReportsLibrary.h"
#include "YesNoDialog.h"
#include "TemplateSelectDialog.h"
#include "ClientsFullFindDialog.h"
#include "PurchasePositionEditDialog.h"
#include "RemindersDialog.h"
#include "PayedDialog.h"
#include "PrintPreviewDialog.h"
#include "resources/images/WarehouseIcon.xpm"

#include "Configuration.h"

BEGIN_EVENT_TABLE(PurchaseEditFrame, wxFrame)
	EVT_CLOSE(PurchaseEditFrame::OnClose)
	EVT_MENU(CLOSE_EVENT, PurchaseEditFrame::CloseKeyEvent)
	EVT_MENU(FIND_DISTRIBUTOR_BUTTON, PurchaseEditFrame::FindDistributorKeyEvent)
	EVT_BUTTON(XRCID("findDistributorButton"), PurchaseEditFrame::FindDistributorEvent)
	EVT_MENU(PASTE_BILLING_ADDRESS_BUTTON, PurchaseEditFrame::PasteAddressKeyEvent)
	EVT_BUTTON(XRCID("pasteBillingAddressButton"), PurchaseEditFrame::PasteAddressEvent)
	EVT_MENU(PASTE_DELIVERY_ADDRESS_BUTTON, PurchaseEditFrame::PasteAddressKeyEvent)
	EVT_BUTTON(XRCID("pasteDeliveryAddressButton"), PurchaseEditFrame::PasteAddressEvent)
	EVT_BUTTON(XRCID("removePositionButton"), PurchaseEditFrame::RemovePositionEvent)
	EVT_BUTTON(XRCID("editPositionButton"), PurchaseEditFrame::EditPositionEvent)
	EVT_BUTTON(XRCID("addPositionButton"), PurchaseEditFrame::AddPositionEvent)
	EVT_MENU(ADD_POSITION_EVENT, PurchaseEditFrame::AddPositionKeyEvent)
	EVT_MENU(EDIT_POSITION_EVENT, PurchaseEditFrame::EditPositionKeyEvent)
	EVT_MENU(REMOVE_POSITION_EVENT, PurchaseEditFrame::RemovePositionKeyEvent)
	EVT_LIST_ITEM_ACTIVATED(XRCID("purchasePositionsListCtrl"), PurchaseEditFrame::DirectEditPositionEvent)
	EVT_MENU(SAVE_EVENT, PurchaseEditFrame::SaveKeyEvent)
	EVT_MENU(EDITED_INFORMATIONS_EVENT, PurchaseEditFrame::ShowEditedInformationsKeyEvent)
	EVT_MENU(REMINDERS_EVENT, PurchaseEditFrame::RemindersKeyEvent)
	EVT_MENU(DONE_EVENT, PurchaseEditFrame::DoneKeyEvent)
	EVT_MENU(PRINT_EVENT, PurchaseEditFrame::PrintKeyEvent)
	EVT_MENU(PRINT_PREVIEW_EVENT, PurchaseEditFrame::PrintPreviewKeyEvent)
END_EVENT_TABLE()

PurchaseEditFrame::PurchaseEditFrame(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	PurchaseEntity *purchaseEntity,
	GuiCallbackInterface *callbackInterface,
	const bool deletePurchaseEntity,
	const bool copy
) : bind(
	purchaseEntity
) {
	changed                    = false;
	SetId(id);
	this->rit                  = rit;
	this->callbackInterface    = callbackInterface;
	this->deletePurchaseEntity = deletePurchaseEntity;
	purchaseId                 = purchaseEntity ? purchaseEntity->Get(_N("purchaseId"))->ToUnsignedInt() : 0;
	isDone                     = false;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("PurchaseEditFrame.xml")));
	wxXmlResource::Get()->LoadFrame(this, parent, _N("PurchaseEditFrame"));

	mainPanel                           = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer                        = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	saveToolbarButton                   = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	printToolbarButton                  = XRCCTRL(*this, "printToolbarButton", ToolbarButton);
	printPreviewToolbarButton           = XRCCTRL(*this, "printPreviewToolbarButton", ToolbarButton);
	doneToolbarButton                   = XRCCTRL(*this, "doneToolbarButton", ToolbarButton);
	remindersToolbarButton              = XRCCTRL(*this, "remindersToolbarButton", ToolbarButton);
	editedInformationsToolbarButton     = XRCCTRL(*this, "editedInformationsToolbarButton", ToolbarButton);
	closeToolbarButton                  = XRCCTRL(*this, "closeToolbarButton", ToolbarButton);
	mainToolbar                         = XRCCTRL(*this, "mainToolbar", Toolbar);
	distributorAddressTextCtrl          = XRCCTRL(*this, "distributorAddressTextCtrl", DynSoft::DSTextCtrl);
	findDistributorButton               = XRCCTRL(*this, "findDistributorButton", wxButton);
	clientIdTextCtrl                    = XRCCTRL(*this, "clientIdTextCtrl", DynSoft::DSTextCtrl);
	orderIdTextCtrl                     = XRCCTRL(*this, "orderIdTextCtrl", DynSoft::DSTextCtrl);
	orderDateDatePickerCtrl             = XRCCTRL(*this, "orderDateDatePickerCtrl", DynSoft::DSDatePickerCtrl);
	purchaseIdTextCtrl                  = XRCCTRL(*this, "purchaseIdTextCtrl", DynSoft::DSTextCtrl);
	purchaserTextCtrl                   = XRCCTRL(*this, "purchaserTextCtrl", DynSoft::DSTextCtrl);
	informationsTextCtrl                = XRCCTRL(*this, "informationsTextCtrl", DynSoft::DSTextCtrl);
	billingAddressTextCtrl              = XRCCTRL(*this, "billingAddressTextCtrl", DynSoft::DSTextCtrl);
	pasteBillingAddressButton           = XRCCTRL(*this, "pasteBillingAddressButton", wxButton);
	deliveryAddressTextCtrl             = XRCCTRL(*this, "deliveryAddressTextCtrl", DynSoft::DSTextCtrl);
	pasteDeliveryAddressButton          = XRCCTRL(*this, "pasteDeliveryAddressButton", wxButton);
	textHeadStaticText                  = XRCCTRL(*this, "textHeadStaticText", wxStaticText);
	textHeadTextCtrl                    = XRCCTRL(*this, "textHeadTextCtrl", DynSoft::DSTextCtrl);
	textFootStaticText                  = XRCCTRL(*this, "textFootStaticText", wxStaticText);
	textFootTextCtrl                    = XRCCTRL(*this, "textFootTextCtrl", DynSoft::DSTextCtrl);
	distributorIdTextCtrl               = XRCCTRL(*this, "distributorIdTextCtrl", DynSoft::DSTextCtrl);
	purchasePositionsListCtrl           = XRCCTRL(*this, "purchasePositionsListCtrl", DynSoft::DSListCtrl);
	removePositionButton                = XRCCTRL(*this, "removePositionButton", wxButton);
	editPositionButton                  = XRCCTRL(*this, "editPositionButton", wxButton);
	addPositionButton                   = XRCCTRL(*this, "addPositionButton", wxButton);
	mainStatusBar                       = XRCCTRL(*this, "mainStatusBar", wxStatusBar);

	KeyValueDatabaseLibrary keyValueLibrary(rit, _N("advanced_preferences"));
	const wxString mediaDefault(keyValueLibrary.Get(_N("purchases.media.default_description")).ToString());
	wxPanel *tmpMediaPanel              = XRCCTRL(*this, "mediaPanel", wxPanel);
	mediaBoxSizer                       = static_cast<wxBoxSizer *>(tmpMediaPanel->GetSizer());
	mediaPanel                          = new MediaPanel(tmpMediaPanel, -1, rit, _N("linked_files_purchases"), purchaseEntity ? purchaseEntity->GetLinkedFileEntities() : NULL, &changed, mediaDefault);
	mediaBoxSizer->Add(mediaPanel, 1, wxEXPAND | wxALL, 5);

	wxPanel *tmpMoreDataPanel           = XRCCTRL(*this, "moreDataPanel", wxPanel);
	moreDataBoxSizer                    = static_cast<wxBoxSizer *>(tmpMoreDataPanel->GetSizer());
	moreDataPanel                       = new MoreDataPanel(tmpMoreDataPanel, -1, rit, purchaseEntity->GetKeysValuesType(), &changed);
	moreDataBoxSizer->Add(moreDataPanel, 1, wxEXPAND | wxALL, 5);

	distributorIdTextCtrl->Disable();

	// Positions
	purchasePositionsListCtrl->InsertColumn(0, _("Amount"), wxLIST_FORMAT_RIGHT, 60);
	purchasePositionsListCtrl->InsertColumn(1, _("Product id"), wxLIST_FORMAT_RIGHT, 120);
	purchasePositionsListCtrl->InsertColumn(2, _("Article number"), wxLIST_FORMAT_RIGHT, 120);
	purchasePositionsListCtrl->InsertColumn(3, _("Short name"), wxLIST_FORMAT_LEFT, 120);
	purchasePositionsListCtrl->InsertColumn(4, _("Price"), wxLIST_FORMAT_RIGHT, 90);
	purchasePositionsListCtrl->InsertColumn(5, _("Full price"), wxLIST_FORMAT_RIGHT, 90);

	// Events
	CONNECT_TOOLBARBUTTON(closeToolbarButton, PurchaseEditFrame::CloseEvent);
	CONNECT_TOOLBARBUTTON(saveToolbarButton, PurchaseEditFrame::SaveEvent);
	CONNECT_TOOLBARBUTTON(editedInformationsToolbarButton, PurchaseEditFrame::ShowEditedInformationsEvent);
	CONNECT_TOOLBARBUTTON(remindersToolbarButton, PurchaseEditFrame::RemindersEvent);
	CONNECT_TOOLBARBUTTON(doneToolbarButton, PurchaseEditFrame::DoneEvent);
	CONNECT_TOOLBARBUTTON(printToolbarButton, PurchaseEditFrame::PrintEvent);
	CONNECT_TOOLBARBUTTON(printPreviewToolbarButton, PurchaseEditFrame::PrintPreviewEvent);

	textHeadStaticText->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(PurchaseEditFrame::TemplateSelectEvent), NULL, this);
	textFootStaticText->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(PurchaseEditFrame::TemplateSelectEvent), NULL, this);

	wxAcceleratorEntry acceleratorEntries[12];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,              WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,                (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,                (int) 'f',  FIND_DISTRIBUTOR_BUTTON);
	acceleratorEntries[3].Set(wxACCEL_CTRL,                (int) 'n',  ADD_POSITION_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL,                (int) 'e',  EDIT_POSITION_EVENT);
	acceleratorEntries[5].Set(wxACCEL_CTRL,                (int) 'd',  REMOVE_POSITION_EVENT);
	acceleratorEntries[6].Set(wxACCEL_CTRL,                (int) 's',  SAVE_EVENT);
	acceleratorEntries[7].Set(wxACCEL_CTRL,                (int) 'i',  EDITED_INFORMATIONS_EVENT);
	acceleratorEntries[8].Set(wxACCEL_CTRL | wxACCEL_ALT,  (int) 'w',  REMINDERS_EVENT);
	acceleratorEntries[9].Set(wxACCEL_CTRL | wxACCEL_ALT,  (int) 'd',  DONE_EVENT);
	acceleratorEntries[10].Set(wxACCEL_CTRL,               (int) 'p',  PRINT_EVENT);
	acceleratorEntries[11].Set(wxACCEL_CTRL | wxACCEL_ALT, (int) 'p',  PRINT_PREVIEW_EVENT);
	wxAcceleratorTable acceleratorTable(12, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Binding
	bind.Bind(_N("distributorId"), distributorIdTextCtrl);
	bind.Bind(_N("distributorAddress"), distributorAddressTextCtrl);
	bind.Bind(_N("clientId"), clientIdTextCtrl);
	bind.Bind(_N("orderId"), orderIdTextCtrl);
	bind.Bind(_N("orderDate"), orderDateDatePickerCtrl);
	bind.Bind(_N("purchaser"), purchaserTextCtrl);
	bind.Bind(_N("purchaseId"), purchaseIdTextCtrl);
	bind.Bind(_N("informations"), informationsTextCtrl);
	bind.Bind(_N("textHead"), textHeadTextCtrl);
	bind.Bind(_N("textFoot"), textFootTextCtrl);
	bind.Bind(_N("billingAddress"), billingAddressTextCtrl);
	bind.Bind(_N("deliveryAddress"), deliveryAddressTextCtrl);

	if(purchaseEntity) {
		rit->Block(_N("purchases"), purchaseId, this);
		
		bind.Push();
		if(purchaseId == 0)
			purchaseIdTextCtrl->SetValue(_("<automatic>"));
		if(purchaseEntity->Get(_N("distributorId"))->ToUnsignedInt() == 0)
			distributorIdTextCtrl->Clear();

		PurchasePositionEntities *purchasePositionEntities = purchaseEntity->GetPositions();
		unsigned int count = purchasePositionEntities->GetCount();
		for(unsigned int i = 0; i < count; i++)
			AddOrChangeToPurchasePositionsListCtrl(purchasePositionEntities->Get(i), true);
	}

	mainBoxSizer->SetSizeHints(this);

	distributorAddressTextCtrl->SetChange(&changed);
	clientIdTextCtrl->SetChange(&changed);
	orderIdTextCtrl->SetChange(&changed);
	orderDateDatePickerCtrl->SetChange(&changed);
	purchaseIdTextCtrl->SetChange(&changed);
	purchaserTextCtrl->SetChange(&changed);
	informationsTextCtrl->SetChange(&changed);
	billingAddressTextCtrl->SetChange(&changed);
	deliveryAddressTextCtrl->SetChange(&changed);
	textHeadTextCtrl->SetChange(&changed);
	textFootTextCtrl->SetChange(&changed);
	distributorIdTextCtrl->SetChange(&changed);

	distributorIdTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);

	this->editedInformationsDialog = new EditedInformationsDialog(this, -1, rit, purchaseEntity);
	this->editedInformationsDialog->Show(false);

	Transfer();
	SetDone();

	changed = copy;

	ICON();
}

// Events
void PurchaseEditFrame::OnClose(wxCloseEvent &event) {
	bool ok = true;
	if(changed) {
		YesNoDialog *yesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_("You have changed the data. Do you want to save the changes?"),
			_("Close"),
			_("Save"),
			_("Cancel")
		);
		yesNoDialog->Center();
		if(yesNoDialog->ShowModal() == wxID_YES)
			ok = Save();
		yesNoDialog->Destroy();
	}

	if(ok) {
		PurchaseEntity *purchaseEntity = static_cast<PurchaseEntity *>(bind.GetEntity());
		if(deletePurchaseEntity)
			delete purchaseEntity;
		rit->Unblock(_N("purchases"), purchaseId);

		Destroy();
		event.Skip();
	} else {
		event.Veto();
	}
}

void PurchaseEditFrame::CloseKeyEvent(wxCommandEvent &event) {
	Close();
}

void PurchaseEditFrame::CloseEvent(wxMouseEvent &event) {
	Close();
	SKIP();
}

void PurchaseEditFrame::TemplateSelectEvent(wxMouseEvent &event) {
	TemplateSelect(event.GetEventObject());
}

void PurchaseEditFrame::FindDistributorEvent(wxCommandEvent &event) {
	FindDistributor();
}

void PurchaseEditFrame::FindDistributorKeyEvent(wxCommandEvent &event) {
	FindDistributor();
}

void PurchaseEditFrame::PasteAddressEvent(wxCommandEvent &event) {
	PasteAddress(event.GetId() == XRCID("pasteDeliveryAddressButton") ? PASTE_DELIVERY_ADDRESS_BUTTON : PASTE_BILLING_ADDRESS_BUTTON);
}

void PurchaseEditFrame::PasteAddressKeyEvent(wxCommandEvent &event) {
	PasteAddress(event.GetId());
}

void PurchaseEditFrame::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}

void PurchaseEditFrame::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void PurchaseEditFrame::ShowEditedInformationsEvent(wxMouseEvent &event) {
	ShowEditedInformations();
	SKIP();
}

void PurchaseEditFrame::ShowEditedInformationsKeyEvent(wxCommandEvent &event) {
	ShowEditedInformations();
}

void PurchaseEditFrame::RemindersEvent(wxMouseEvent &event) {
	Reminders();
	SKIP();
}

void PurchaseEditFrame::RemindersKeyEvent(wxCommandEvent &event) {
	Reminders();
}

void PurchaseEditFrame::DoneEvent(wxMouseEvent &event) {
	Done();
	SKIP();
}

void PurchaseEditFrame::DoneKeyEvent(wxCommandEvent &event) {
	Done();
}

void PurchaseEditFrame::PrintEvent(wxMouseEvent &event) {
	Print();
	SKIP();
}

void PurchaseEditFrame::PrintPreviewEvent(wxMouseEvent &event) {
	Print(true);
	SKIP();
}

void PurchaseEditFrame::PrintKeyEvent(wxCommandEvent &event) {
	Print();
}

void PurchaseEditFrame::PrintPreviewKeyEvent(wxCommandEvent &event) {
	Print(true);
}

// Methods
void PurchaseEditFrame::Close() {
	wxWindow::Close();
}

void PurchaseEditFrame::AddPositionEvent(wxCommandEvent &event) {
	AddPosition();
}

void PurchaseEditFrame::EditPositionEvent(wxCommandEvent &event) {
	EditPosition(static_cast<PurchasePositionEntity *>(purchasePositionsListCtrl->GetSingleSelectionPointer()));
}

void PurchaseEditFrame::DirectEditPositionEvent(wxListEvent &event) {
	EditPosition(static_cast<PurchasePositionEntity *>(purchasePositionsListCtrl->GetSingleSelectionPointer()));
}

void PurchaseEditFrame::RemovePositionEvent(wxCommandEvent &event) {
	RemovePosition(static_cast<PurchasePositionEntity *>(purchasePositionsListCtrl->GetSingleSelectionPointer()));
}

void PurchaseEditFrame::AddPositionKeyEvent(wxCommandEvent &event) {
	AddPosition();
}

void PurchaseEditFrame::EditPositionKeyEvent(wxCommandEvent &event) {
	EditPosition(static_cast<PurchasePositionEntity *>(purchasePositionsListCtrl->GetSingleSelectionPointer()));
}

void PurchaseEditFrame::RemovePositionKeyEvent(wxCommandEvent &event) {
	RemovePosition(static_cast<PurchasePositionEntity *>(purchasePositionsListCtrl->GetSingleSelectionPointer()));
}

void PurchaseEditFrame::AddOrChangeToPurchasePositionsListCtrl(PurchasePositionEntity *purchasePositionEntity, const bool dontRecalcPositions) {
	DSCurrency amount(purchasePositionEntity->Get(_N("amount"))->ToCurrency());
	DSCurrency price(purchasePositionEntity->Get(_N("price"))->ToCurrency());
	DSCurrency fullPrice(price * amount);

	DSListCtrl::DSListCtrlRow *purchasePositionsListCtrlRow = new DSListCtrl::DSListCtrlRow(purchasePositionEntity);
	purchasePositionsListCtrlRow->Add(amount.ToString());
	purchasePositionsListCtrlRow->Add(purchasePositionEntity->Get(_N("productId"))->ToString());
	purchasePositionsListCtrlRow->Add(purchasePositionEntity->Get(_N("articleNumber"))->ToString());
	purchasePositionsListCtrlRow->Add(purchasePositionEntity->Get(_N("shortName"))->ToString());
	purchasePositionsListCtrlRow->Add(price.ToString());
	purchasePositionsListCtrlRow->Add(fullPrice.ToString());

	purchasePositionsListCtrl->Set(purchasePositionsListCtrlRow, true);

	if(!dontRecalcPositions)
		Transfer();

	changed = true;
}

void PurchaseEditFrame::TemplateSelect(wxObject *object) {
	DSTextCtrl *textCtrl = NULL;

	if(object == textHeadStaticText)
		textCtrl = textHeadTextCtrl;
	else if(object == textFootStaticText)
		textCtrl = textFootTextCtrl;
	
	TemplateSelectDialog::ShowAndSet(this, rit, textCtrl);
}

void PurchaseEditFrame::FindDistributor() {
	if(purchasePositionsListCtrl->GetItemCount() > 0) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("You have added positions from selected distributor. To change the distributor remove the added positions."),
			_("Cannot change distributor")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
	} else {
		ClientsFullFindDialog *clientsFullFindDialog = new ClientsFullFindDialog(this, -1, rit, ClientsFullFindDialog::KIND_DISTRIBUTOR);
		clientsFullFindDialog->Center();
		if(clientsFullFindDialog->ShowModal() == wxID_OK) {
			ClientEntity *clientEntity = clientsFullFindDialog->GetClientEntity();
			if(clientEntity) {
				distributorIdTextCtrl->SetValue(clientEntity->Get(_N("clientId"))->ToString());
				distributorAddressTextCtrl->SetValue(clientEntity->GetMainData());
			}
		}
	}
}

void PurchaseEditFrame::PasteAddress(const int buttonId) {
	DSTextCtrl *addressTextCtrl = NULL;
	
	switch(buttonId) {
		case PASTE_BILLING_ADDRESS_BUTTON:
			addressTextCtrl = billingAddressTextCtrl;
			break;
		case PASTE_DELIVERY_ADDRESS_BUTTON:
			addressTextCtrl = deliveryAddressTextCtrl;
			break;
	}

	if(addressTextCtrl) {
		EnterprisePreferencesEntity enterprisePreferencesEntity(rit->GetDatabase());
		EnterprisePreferencesController enterprisePreferencesController(rit->GetDatabase());
		enterprisePreferencesController.Get();
		if(enterprisePreferencesController.Run()) {
			enterprisePreferencesController.Get(&enterprisePreferencesEntity);
			enterprisePreferencesController.Reset();
			addressTextCtrl->SetValue(enterprisePreferencesEntity.AsPostAddress());
		}
	}
}

void PurchaseEditFrame::AddPosition() {
	purchasePositionsListCtrl->SetFocus();

	if(!rit->Access(A_WRITE, _("You have no permission to add a position."), this))
		return;
	
	const unsigned int distributorId = DSCasts::ToUnsignedInt(distributorIdTextCtrl->GetValue());
	ClientController clientController(rit);
	if(clientController.IsDistributor(distributorId)) {
		PurchasePositionEntity *purchasePositionEntity = new PurchasePositionEntity(rit);
		PurchasePositionEditDialog *purchasePositionEditDialog = new PurchasePositionEditDialog(
			this,
			-1,
			rit,
			purchasePositionEntity,
			distributorId
		);
		purchasePositionEditDialog->Center();
		if(purchasePositionEditDialog->ShowModal() == wxID_OK) {
			AddOrChangeToPurchasePositionsListCtrl(purchasePositionEntity);
			Transfer();
		} else {
			delete purchasePositionEntity;
		}
	} else {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("You have not chosen a valid distributor."),
			_("No valid distributor")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
	}
}

void PurchaseEditFrame::EditPosition(PurchasePositionEntity *purchasePositionEntity) {
	purchasePositionsListCtrl->SetFocus();

	if(!rit->Access(A_WRITE, _("You have no permission to edit a position."), this))
		return;

	if(!purchasePositionEntity) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select an entry."),
			_("Edit position")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}
	PurchasePositionEditDialog *purchasePositionEditDialog = new PurchasePositionEditDialog(
		this,
		-1,
		rit,
		purchasePositionEntity,
		purchasePositionEntity->Get(_N("distributorId"))->ToUnsignedInt()
	);
	purchasePositionEditDialog->Center();
	if(purchasePositionEditDialog->ShowModal() == wxID_OK)
		AddOrChangeToPurchasePositionsListCtrl(purchasePositionEntity);
}

void PurchaseEditFrame::RemovePosition(PurchasePositionEntity *purchasePositionEntity) {
	purchasePositionsListCtrl->SetFocus();

	if(!rit->Access(A_WRITE, _("You have no permission to remove a position."), this))
		return;

	if(!purchasePositionEntity) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select an entry."),
			_("Edit position")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	YesNoDialog *yesNoDialog = new YesNoDialog(
		this,
		-1,
		rit,
		_("Do you really want to remove this position?"),
		_("Remove position"),
		_("Remove"),
		_("Cancel")
	);
	yesNoDialog->Center();
	if(yesNoDialog->ShowModal() == wxID_YES) {
		const unsigned int purchasePositionId = purchasePositionEntity->Get(_N("purchasePositionId"))->ToUnsignedInt();
		if(purchasePositionId != 0)
			purchasePositionsRemoved.Add(DSCasts::ToString(purchasePositionId));
		purchasePositionsListCtrl->DeleteItem(purchasePositionsListCtrl->GetPosition(purchasePositionEntity));

		PurchaseEntity *purchaseEntity = static_cast<PurchaseEntity *>(bind.GetEntity());
		if(purchaseEntity)
			purchaseEntity->GetPositions()->Remove(purchasePositionEntity, true);
	}
	yesNoDialog->Destroy();

	Transfer();
}

void PurchaseEditFrame::Transfer(PurchaseEntity *purchaseEntity) {
	bool isChanged = changed;

	if(!purchaseEntity)
		purchaseEntity = static_cast<PurchaseEntity *>(bind.GetEntity());

	// Clean up positions
	PurchasePositionEntities *purchasePositionEntities = purchaseEntity->GetPositions();
	purchasePositionEntities->Clear(false);

	DSCurrency tmp;
	PurchasePositionEntity *purchasePositionEntity = NULL;
	unsigned int count = purchasePositionsListCtrl->GetItemCount();
	for(unsigned int i = 0; i < count; i++) {
		purchasePositionEntity = static_cast<PurchasePositionEntity *>(purchasePositionsListCtrl->GetPointer(i, 0));
		if(purchasePositionEntity) {
			purchasePositionEntities->Add(purchasePositionEntity);
			AddOrChangeToPurchasePositionsListCtrl(purchasePositionEntity, true);
		}
	}

	bind.Pull();
	purchaseEntity->SetValue(_N("purchaseId"), purchaseId);
	DSCurrency sum;
	sum = purchaseEntity->Get(_N("sum"))->ToCurrency();
	purchaseEntity->SetValue(_N("sum"), sum);
	purchaseEntity->Recalc();

	mainStatusBar->SetStatusText(
		_("Sum: ") + purchaseEntity->Get(_N("sum"))->ToCurrency().ToString(),
		0
	);

	if(!isChanged)
		changed = false;
}

bool PurchaseEditFrame::Save() {
	purchaseIdTextCtrl->SetFocus();

	if(isDone)
		return false;
	
	if(!rit->Access(A_WRITE, _("You have no permission to save this purchase."), this))
		return false;

	ClientController clientController(rit);
	if(!clientController.Exists(_N("clientId"), distributorIdTextCtrl->GetValue())) {
		rit->Log(true, _N("PurchaseEditFrame"), _("The purchase is not saved."), _("The distributor id you entered does not exist \"") + distributorIdTextCtrl->GetValue() + _N("\"."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The distributor id you entered does not exist."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return false;
	}

	if(!clientController.IsActive(DSCasts::ToUnsignedInt(distributorIdTextCtrl->GetValue()))) {
		rit->Log(true, _N("PurchaseEditFrame"), _("The purchase is not saved."), _("The distributor id you entered is not active \"") + distributorIdTextCtrl->GetValue() + _N("\"."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The distributor id you entered is not active."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return false;
	}

	if(billingAddressTextCtrl->GetValue().IsEmpty() || DSCasts::ToUnsignedInt(distributorIdTextCtrl->GetValue()) == 0) {
		rit->Log(true, _N("PurchaseEditFrame"), _("The purchase is not saved."), _("Not all necessary fields are filled out."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The following fields are necessary: distributor id and billing address."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return false;
	}

	PurchaseEntity *purchaseEntity = static_cast<PurchaseEntity *>(bind.GetEntity());
	if(!purchaseEntity) {
		rit->Log(true, _N("PurchaseEditFrame"), _("The purchase is not saved."), _("Null Pointer."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The purchase is not saved. Null Pointer."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return false;
	}

	Transfer(purchaseEntity);

	wxArrayString products;
	const bool valid = purchaseEntity->IsValid(&products);

	if(!valid) {
		wxString message;
		if(products.GetCount() > 0)
			message += _("The following products do not exist any more: ") + Tools::ArrayStringToCommaSeparatedList(products) + _N(". ");

		rit->Log(true, _N("PurchaseEditFrame"), _("The purchase is not saved."), message);
		
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			message,
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return false;
	}

	PurchaseController purchaseController(rit);
	const bool ok = purchaseController.Save(purchaseEntity, &purchasePositionsRemoved);
	if(ok) {
		purchaseId = purchaseEntity->Get(_N("purchaseId"))->ToUnsignedInt();
		purchaseController.Get(DSCasts::ToString(purchaseId)); // Autofelder refreshen
		purchaseController.Run();
		purchaseController.Get(purchaseEntity, true);
		purchaseController.Reset();

		bind.Push();
		purchaseIdTextCtrl->SetValue(DSCasts::ToString(purchaseId));

		if(callbackInterface)
			callbackInterface->Callback(static_cast<void *>(purchaseEntity));

		rit->Block(_N("purchases"), purchaseId, this);

		rit->Log(false, _N("PurchaseEditFrame"), _("The purchase is saved."), _("The purchase with purchase id \"") + DSCasts::ToString(purchaseId) + _N("\" is saved."));

		editedInformationsDialog->Update();
		if(purchaseEntity->Get(_N("distributorId"))->ToUnsignedInt() == 0)
			distributorIdTextCtrl->Clear();

		changed = false;
	} else {
		rit->Log(true, _N("PurchaseEditFrame"), _("The purchase is not saved."), _("Database error."));

		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Could not save."),
			_("Save purchase")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
	}

	return ok;
}

void PurchaseEditFrame::ShowEditedInformations() {
	editedInformationsDialog->Get(!editedInformationsDialog->IsShown());
}

void PurchaseEditFrame::Reminders() {
	if(!rit->Access(A_READ, _("You have no permission to open reminders."), this))
		return;

	if(purchaseId == 0) {
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
		ReminderType::PURCHASES,
		purchaseId
	);
	remindersDialog->Center();
	remindersDialog->ShowModal();
	remindersDialog->Destroy();
}

void PurchaseEditFrame::Done() {
	if(purchaseId == 0) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("You first have to save the purchase to set it as done."),
			_("Set done")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	if(!rit->Access(A_WRITE, _("You have no permission to set this purchase as done."), this))
		return;
	
	PurchaseController purchaseController(rit);
	wxString done(purchaseController.GetDone(purchaseId));

	bool ok = false;
	DSDateTime doneTime;

	if(done.IsEmpty()) {
		PayedDialog *payedDialog = new PayedDialog(
			this,
			-1,
			rit,
			_("Set done")
		);
		payedDialog->Center();
		ok = payedDialog->ShowModal() == wxID_OK;
		doneTime = payedDialog->GetValue();
		payedDialog->Destroy();
	} else {
		YesNoDialog *yesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_("Do you want to set this purchase undone?"),
			_("Purchase"),
			_("Undone"),
			_("Cancel")
		);
		yesNoDialog->Center();
		ok = yesNoDialog->ShowModal() == wxID_YES;
		yesNoDialog->Destroy();
	}
	
	if(ok) {
		if(purchaseController.SetDone(purchaseId, done.IsEmpty(), doneTime)) {
			rit->Log(false, _N("PurchaseEditFrame"), _("The purchase is set as done."), _("The purchase \"") + DSCasts::ToString(purchaseId) + _("\" is set as done."));

			if(callbackInterface) {
				purchaseController.Get(DSCasts::ToString(purchaseId));
				if(purchaseController.Run()) {
					PurchaseEntity purchaseEntity(rit);
					purchaseController.Get(&purchaseEntity);
					purchaseController.Reset();
					callbackInterface->Callback(static_cast<void *>(&purchaseEntity));
				}
			}
		} else {
			rit->Log(true, _N("PurchaseEditFrame"), _("The purchase is not set as done."), _("The purchase \"") + DSCasts::ToString(purchaseId) + _("\" is not set as done."));
			wxMessageDialog *errorMessage = new wxMessageDialog(
				this,
				_("The purchase could not be set as done. Database error."),
				_("Error")
			);
			errorMessage->Center();
			errorMessage->ShowModal();
			errorMessage->Destroy();
		}
	}

	SetDone();
}

void PurchaseEditFrame::SetDone() {
	PurchaseController purchaseController(rit);
	wxString done(purchaseController.GetDone(purchaseId));
	done = done.Mid(0, done.Find(_N(" ")));
	if(done.IsEmpty()) {
		mainStatusBar->SetStatusText(_("Not done"), 1);
		doneToolbarButton->SetToggled(false);
		doneToolbarButton->SetLabel(_("Set done"));
		isDone = false;
	} else {
		mainStatusBar->SetStatusText(_("Done: ") + done, 1);
		doneToolbarButton->SetToggled(true);
		doneToolbarButton->SetLabel(_("Set undone"));
		isDone = true;
	}

	SetDisabled();
}

void PurchaseEditFrame::SetDisabled() {
	saveToolbarButton->SetEnabled(!isDone);
	distributorAddressTextCtrl->Enable(!isDone);
	findDistributorButton->Enable(!isDone);
	clientIdTextCtrl->Enable(!isDone);
	orderIdTextCtrl->Enable(!isDone);
	orderDateDatePickerCtrl->Enable(!isDone);
	purchaseIdTextCtrl->Enable(!isDone);
	purchaserTextCtrl->Enable(!isDone);
	informationsTextCtrl->Enable(!isDone);
	billingAddressTextCtrl->Enable(!isDone);
	pasteBillingAddressButton->Enable(!isDone);
	deliveryAddressTextCtrl->Enable(!isDone);
	pasteDeliveryAddressButton->Enable(!isDone);
	textHeadStaticText->Enable(!isDone);
	textHeadTextCtrl->Enable(!isDone);
	textFootStaticText->Enable(!isDone);
	textFootTextCtrl->Enable(!isDone);
	distributorIdTextCtrl->Enable(!isDone);
	purchasePositionsListCtrl->Enable(!isDone);
	removePositionButton->Enable(!isDone);
	editPositionButton->Enable(!isDone);
	addPositionButton->Enable(!isDone);
	mediaPanel->SetEditable(!isDone);
	moreDataPanel->SetEditable(!isDone);
}

void PurchaseEditFrame::Print(const bool preview) {
	purchaseIdTextCtrl->SetFocus();

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
		_N("purchases"),
		purchaseId,
		preview ? JasperReportsController::SHOW : JasperReportsController::PRINT
	)) {
		PurchaseController purchaseController(rit);
		HtmlPrintoutLibrary *htmlPrintoutLibrary = purchaseController.Print(static_cast<PurchaseEntity *>(bind.GetEntity()));
		
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
