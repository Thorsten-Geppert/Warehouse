#include "ReminderDialog.h"
#include "../lib/ReminderLibrary.h"
#include "../lib/RemindersType.h"
#include "../lib/BlockType.h"
#include "../lib/ClientController.h"
#include "../lib/ClientEntity.h"
#include "../lib/ProductController.h"
#include "../lib/ProductEntity.h"
#include "../lib/OrderController.h"
#include "../lib/OrderEntity.h"
#include "../lib/VoucherController.h"
#include "../lib/VoucherEntity.h"
#include "../lib/PurchaseController.h"
#include "../lib/PurchaseEntity.h"
#include "../lib/UserController.h"
#include "Configuration.h"
#include "WarehouseFrame.h"
#include "YesNoDialog.h"
#include "ClientEditFrame.h"
#include "ClientsModulePanel.h"
#include "ProductEditFrame.h"
#include "ProductsModulePanel.h"
#include "OrderEditFrame.h"
#include "OrdersModulePanel.h"
#include "VoucherEditFrame.h"
#include "VouchersModulePanel.h"
#include "PurchaseEditFrame.h"
#include "PurchasesModulePanel.h"
#include "TemplateSelectDialog.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(ReminderDialog, wxDialog)
	EVT_MENU(CANCEL_EVENT, ReminderDialog::CancelKeyEvent)
	EVT_MENU(UPDATE_EVENT, ReminderDialog::UpdateKeyEvent)
	EVT_LIST_ITEM_ACTIVATED(XRCID("remindersListCtrl"), ReminderDialog::OpenKeyEvent)
	EVT_LIST_ITEM_SELECTED(XRCID("remindersListCtrl"), ReminderDialog::SelectEvent)
	EVT_LIST_ITEM_DESELECTED(XRCID("remindersListCtrl"), ReminderDialog::SelectEvent)
	EVT_BUTTON(XRCID("saveButton"), ReminderDialog::SaveEvent)
	EVT_BUTTON(XRCID("doneButton"), ReminderDialog::DoneEvent)
	EVT_CLOSE(ReminderDialog::CloseEvent)
	EVT_CHECKBOX(XRCID("openToRemindCheckBox"), ReminderDialog::OpenToRemindEvent)
	EVT_TIMER(-1, ReminderDialog::RemindEvent)
END_EVENT_TABLE()

ReminderDialog::ReminderDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) : timer(
	this
) {
	SetId(id);
	this->rit = rit;

	timer.Start(REMINDER_INTERVAL * 1000);
	
	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("ReminderDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("ReminderDialog"));

	mainPanel              = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer           = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	cancelToolbarButton    = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	openToolbarButton      = XRCCTRL(*this, "openToolbarButton", ToolbarButton);
	updateToolbarButton    = XRCCTRL(*this, "updateToolbarButton", ToolbarButton);
	firstCalendarCtrl      = XRCCTRL(*this, "firstCalendarCtrl", wxCalendarCtrl);
	secondCalendarCtrl     = XRCCTRL(*this, "secondCalendarCtrl", wxCalendarCtrl);
	thirdCalendarCtrl      = XRCCTRL(*this, "thirdCalendarCtrl", wxCalendarCtrl);
	changeStaticText       = XRCCTRL(*this, "changeStaticText", wxStaticText);
	remindersListCtrl      = XRCCTRL(*this, "remindersListCtrl", DynSoft::DSListCtrl);
	reminderDatePickerCtrl = XRCCTRL(*this, "reminderDatePickerCtrl", wxDatePickerCtrl);
	hourComboBox           = XRCCTRL(*this, "hourComboBox", wxComboBox);
	minuteComboBox         = XRCCTRL(*this, "minuteComboBox", wxComboBox);
	saveButton             = XRCCTRL(*this, "saveButton", wxButton);
	doneButton             = XRCCTRL(*this, "doneButton", wxButton);
	notesStaticText        = XRCCTRL(*this, "notesStaticText", wxStaticText);
	notesTextCtrl          = XRCCTRL(*this, "notesTextCtrl", wxTextCtrl);
	openToRemindCheckBox   = XRCCTRL(*this, "openToRemindCheckBox", wxCheckBox);

	firstCalendarCtrl->Enable(false);
	secondCalendarCtrl->Enable(false);
	thirdCalendarCtrl->Enable(false);

	firstCalendarCtrl->SetHighlightColours(*wxBLACK, *wxWHITE);
	secondCalendarCtrl->SetHighlightColours(*wxBLACK, *wxWHITE);
	thirdCalendarCtrl->SetHighlightColours(*wxBLACK, *wxWHITE);

	wxDateTime dt(wxDateTime::Now());

	ReminderLibrary::FillTimeComboBoxes(hourComboBox, minuteComboBox);
	hourComboBox->SetSelection(dt.GetHour());
	minuteComboBox->SetSelection(dt.GetMinute());

	firstCalendarCtrl->SetDate(dt);

	int year  = dt.GetYear();
	int month = dt.GetMonth() + 1;
	if(month >= 12) {
		month = 0;
		year++;
	}
	dt.SetMonth((wxDateTime::Month) month);
	dt.SetYear(year);
	secondCalendarCtrl->SetDate(dt);

	month++;
	if(month >= 12) {
		month = 0;
		year++;
	}
	dt.SetMonth((wxDateTime::Month) month);
	dt.SetYear(year);
	thirdCalendarCtrl->SetDate(dt);

	const int style = firstCalendarCtrl->GetWindowStyle() | wxCAL_NO_YEAR_CHANGE | wxCAL_NO_MONTH_CHANGE;
	firstCalendarCtrl->SetWindowStyle(style);
	secondCalendarCtrl->SetWindowStyle(style);
	thirdCalendarCtrl->SetWindowStyle(style);

	remindersListCtrl->InsertColumn(0, _("Module"), wxLIST_FORMAT_LEFT, 100);
	remindersListCtrl->InsertColumn(1, _("ID"), wxLIST_FORMAT_RIGHT, 100);
	remindersListCtrl->InsertColumn(2, _("Description"), wxLIST_FORMAT_LEFT, 180);
	remindersListCtrl->InsertColumn(3, _("Date"), wxLIST_FORMAT_LEFT, 180);

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(cancelToolbarButton, ReminderDialog::CancelEvent);
	CONNECT_TOOLBARBUTTON(updateToolbarButton, ReminderDialog::UpdateEvent);
	CONNECT_TOOLBARBUTTON(openToolbarButton, ReminderDialog::OpenEvent);

	notesStaticText->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(ReminderDialog::TemplateSelectEvent), NULL, this);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 'r',  UPDATE_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Filling
	EnableEditing(false);

	remindersListCtrl->SetFocus();

	UserController userController(rit);
	openToRemindCheckBox->SetValue(userController.GetOpenToRemind(rit->GetUserConfigurationFileEntity()->GetLastUsername()));

	backgroundColour = remindersListCtrl->GetBackgroundColour();

	Update();

	ICON();
}

ReminderDialog::~ReminderDialog() {
	CleanUp();
}

int ReminderDialog::ShowModal() {
	return wxDialog::ShowModal();
}

void ReminderDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void ReminderDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void ReminderDialog::OpenEvent(wxMouseEvent &event) {
	Open();
	SKIP();
}

void ReminderDialog::OpenKeyEvent(wxListEvent &event) {
	Open();
}

void ReminderDialog::UpdateEvent(wxMouseEvent &event) {
	Update();
	SKIP();
}

void ReminderDialog::UpdateKeyEvent(wxCommandEvent &event) {
	Update();
}

void ReminderDialog::CloseEvent(wxCloseEvent &event) {
	Cancel();
}

void ReminderDialog::SelectEvent(wxListEvent &event) {
	Select(static_cast<ReminderType *>(remindersListCtrl->GetSingleSelectionPointer()));
}

void ReminderDialog::SaveEvent(wxCommandEvent &event) {
	Save();
}

void ReminderDialog::DoneEvent(wxCommandEvent &event) {
	Done();
}

void ReminderDialog::OpenToRemindEvent(wxCommandEvent &event) {
	OpenToRemind();
}

void ReminderDialog::TemplateSelectEvent(wxMouseEvent &event) {
	TemplateSelect(event.GetEventObject());
}

void ReminderDialog::RemindEvent(wxTimerEvent &event) {
	Remind();
}

void ReminderDialog::Cancel() {
	static_cast<WarehouseFrame *>(GetParent())->Reminder();
}

void ReminderDialog::EnableEditing(const bool enable) {
	notesTextCtrl->Enable(enable);
	changeStaticText->Enable(enable);
	reminderDatePickerCtrl->Enable(enable);
	hourComboBox->Enable(enable);
	minuteComboBox->Enable(enable);
	saveButton->Enable(enable);
	doneButton->Enable(enable);

	if(!enable) {
		notesTextCtrl->Clear();
		wxDateTime dt(wxDateTime::Now());
		reminderDatePickerCtrl->SetValue(dt);
		hourComboBox->SetSelection(dt.GetHour());
		minuteComboBox->SetSelection(dt.GetMinute());
	}
}

void ReminderDialog::Update() {
	CleanUp();

	ReminderLibrary reminderLibrary(rit);
	RemindersType *remindersType = reminderLibrary.Get(rit->GetUserConfigurationFileEntity()->GetLastUsername());
	const unsigned int count = remindersType->GetCount();
	for(unsigned int i = 0; i < count; i++)
		AddOrChangeToRemindersListCtrl(remindersType->Get(i));
	delete remindersType;

	UpdateCalendars();
}

void ReminderDialog::CleanUp() {
	remindersListCtrl->CleanUp();
}

void ReminderDialog::CleanCalendars() {
	for(unsigned int i = 0; i < 31; i++) {
		firstCalendarCtrl->ResetAttr(i);
		secondCalendarCtrl->ResetAttr(i);
		thirdCalendarCtrl->ResetAttr(i);
	}

	firstCalendarCtrl->SetHighlightColours(*wxBLACK, *wxWHITE);
	secondCalendarCtrl->SetHighlightColours(*wxBLACK, *wxWHITE);
	thirdCalendarCtrl->SetHighlightColours(*wxBLACK, *wxWHITE);
}

void ReminderDialog::AddOrChangeToRemindersListCtrl(ReminderType *reminderType) {
	if(!reminderType)
		return;

	ReminderLibrary reminderLibrary(rit);
	DSDateTime serverTime(reminderLibrary.GetServerTime());

	DSListCtrl::DSListCtrlRow *remindersListCtrlRow = new DSListCtrl::DSListCtrlRow(reminderType);
	remindersListCtrlRow->SetBackgroundColour(serverTime >= reminderType->GetDateTime() ? *wxRED : backgroundColour);

	remindersListCtrlRow->Add(reminderType->GetModuleString());
	remindersListCtrlRow->Add(DSCasts::ToString(reminderType->GetReference()));
	remindersListCtrlRow->Add(reminderType->GetDescription());
	remindersListCtrlRow->Add(reminderType->GetDateTime().ToDatabaseDateTime());

	remindersListCtrl->Set(remindersListCtrlRow, true);

	UpdateCalendars();
}

void ReminderDialog::UpdateCalendars() {
	CleanCalendars();

	ReminderType *reminderType = NULL;

	DSDateTime dt;
	wxDateTime firstDateTime(firstCalendarCtrl->GetDate());
	wxDateTime secondDateTime(secondCalendarCtrl->GetDate());
	wxDateTime thirdDateTime(thirdCalendarCtrl->GetDate());

	const unsigned int count = remindersListCtrl->GetItemCount();
	for(unsigned int i = 0; i < count; i++) {
		reminderType = static_cast<ReminderType *>(remindersListCtrl->GetPointer(i, 0));

		dt = reminderType->GetDateTime();

		wxCalendarDateAttr *settedCalendarDateAttr = new wxCalendarDateAttr;
		settedCalendarDateAttr->SetTextColour(*wxWHITE);
		settedCalendarDateAttr->SetBackgroundColour(*wxBLACK);

		if(firstDateTime.GetMonth() == dt.GetMonth() && firstDateTime.GetYear() == dt.GetYear())
			firstCalendarCtrl->SetAttr(dt.GetDay(), settedCalendarDateAttr);
		if(secondDateTime.GetMonth() == dt.GetMonth() && secondDateTime.GetYear() == dt.GetYear())
			secondCalendarCtrl->SetAttr(dt.GetDay(), settedCalendarDateAttr);
		if(thirdDateTime.GetMonth() == dt.GetMonth() && thirdDateTime.GetYear() == dt.GetYear())
			thirdCalendarCtrl->SetAttr(dt.GetDay(), settedCalendarDateAttr);
		
		if(firstCalendarCtrl->GetDate().GetDay() == dt.GetDay() && firstCalendarCtrl->GetDate().GetMonth() == dt.GetMonth() && firstCalendarCtrl->GetDate().GetYear() == dt.GetYear())
			firstCalendarCtrl->SetHighlightColours(*wxWHITE, *wxBLACK);
		else if(secondCalendarCtrl->GetDate().GetDay() == dt.GetDay() && secondCalendarCtrl->GetDate().GetMonth() == dt.GetMonth() && secondCalendarCtrl->GetDate().GetYear() == dt.GetYear())
			secondCalendarCtrl->SetHighlightColours(*wxWHITE, *wxBLACK);
		else if(thirdCalendarCtrl->GetDate().GetDay() == dt.GetDay() && thirdCalendarCtrl->GetDate().GetMonth() == dt.GetMonth() && thirdCalendarCtrl->GetDate().GetYear() == dt.GetYear())
			thirdCalendarCtrl->SetHighlightColours(*wxWHITE, *wxBLACK);
	}

	firstCalendarCtrl->Refresh();
	secondCalendarCtrl->Refresh();
	thirdCalendarCtrl->Refresh();
}

void ReminderDialog::Select(ReminderType *reminderType) {
	if(!rit->Access(A_READ, _("You have no permission to read this remind."), this))
		return;

	if(!reminderType) {
		EnableEditing(false);
	} else {
		notesTextCtrl->SetValue(reminderType->GetNotes());

		const DSDateTime dt(reminderType->GetDateTime());
		reminderDatePickerCtrl->SetValue(dt);
		hourComboBox->SetSelection(dt.GetHour());
		minuteComboBox->SetSelection(dt.GetMinute());

		EnableEditing(true);
	}
}

void ReminderDialog::Save() {
	if(!rit->Access(A_WRITE, _("You have no permission to save this remind."), this))
		return;

	ReminderType *reminderType = static_cast<ReminderType *>(remindersListCtrl->GetSingleSelectionPointer());
	if(!reminderType) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the entry you want to save."),
			_("Save")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	DSDateTime dt;
	dt.SetWxDateTime(reminderDatePickerCtrl->GetValue());
	dt.SetHour(hourComboBox->GetSelection());
	dt.SetMinute(minuteComboBox->GetSelection());

	ReminderType *tmpReminderType = new ReminderType(*reminderType);
	tmpReminderType->SetDateTime(dt);
	tmpReminderType->SetNotes(notesTextCtrl->GetValue());

	ReminderLibrary reminderLibrary(rit);
	if(!reminderLibrary.Update(*tmpReminderType)) {
		rit->Log(true, _N("ReminderDialog"), _("Could not save. Database error."), reminderType->ToString());
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Could not save. Database error."),
			_("Save")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
	} else {
		rit->Log(false, _N("ReminderDialog"), _("Reminder saved."), reminderType->ToString());
		reminderType->SetId(tmpReminderType->GetId());
		reminderType->SetDateTime(tmpReminderType->GetDateTime());
		reminderType->SetNotes(tmpReminderType->GetNotes());
		AddOrChangeToRemindersListCtrl(reminderType);
	}

	delete tmpReminderType;
}

void ReminderDialog::Done() {
	if(!rit->Access(A_WRITE, _("You have no permission to set this remind as done."), this))
		return;

	ReminderType *reminderType = static_cast<ReminderType *>(remindersListCtrl->GetSingleSelectionPointer());
	if(!reminderType) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the entry you want to set done."),
			_("Done")
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
		_("Do you really want to set this remind done?"),
		_("Close"),
		_("Done"),
		_("Cancel")
	);
	yesNoDialog->Center();
	if(yesNoDialog->ShowModal() == wxID_YES) {
		ReminderLibrary reminderLibrary(rit);
		if(!reminderLibrary.Delete(reminderType->GetId())) {
			rit->Log(true, _N("ReminderDialog"), _("Could not set done. Database error."), reminderType->ToString());
			wxMessageDialog *errorMessage = new wxMessageDialog(
				this,
				_("Could not set done. Database error."),
				_("Save")
			);
			errorMessage->Center();
			errorMessage->ShowModal();
			errorMessage->Destroy();
		} else {
			rit->Log(false, _N("ReminderDialog"), _("Reminder set as done."), reminderType->ToString());
			remindersListCtrl->DeleteItem(remindersListCtrl->GetSingleSelectionPosition());
			delete reminderType;
			EnableEditing(false);
			UpdateCalendars();
		}
	}
	yesNoDialog->Destroy();
}

void ReminderDialog::Open() {
	if(!rit->Access(A_READ, _("You have no permission to read this remind."), this))
		return;

	ReminderType *reminderType = static_cast<ReminderType *>(remindersListCtrl->GetSingleSelectionPointer());
	if(!reminderType) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select the entry you want to open."),
			_("Open")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	if(reminderType->GetModule() == ReminderType::CLIENTS) {
		if(!rit->Access(A_READ, _("You have no permission to open a client."), this))
			return;

		BlockType *blockType = rit->IsBlocked(_N("clients"), reminderType->GetReference());
		if(blockType) {
			ClientEditFrame *clientEditFrame = static_cast<ClientEditFrame *>(blockType->GetData());
			if(clientEditFrame) {
				clientEditFrame->Raise();
			} else {
				wxMessageDialog *errorMessageDialog = new wxMessageDialog(
					this,
					_("The client you want to edit is allready opened."),
					_("Edit client")
				);
				errorMessageDialog->Center();
				errorMessageDialog->ShowModal();
				errorMessageDialog->Destroy();
			}
			return;
		}

		ClientController clientController(rit);
		clientController.Get(DSCasts::ToString(reminderType->GetReference()));
		if(clientController.Run()) {
			ClientEntity *clientEntity = new ClientEntity(rit);
			clientController.Get(clientEntity);
			clientController.Reset();

			ClientEditFrame *clientEditFrame = new ClientEditFrame(this, -1, rit, clientEntity, static_cast<ClientsModulePanel *>(rit->GetObject(_N("clientsModulePanel"))));
			clientEditFrame->Center();
			clientEditFrame->Show();

			rit->Block(_N("clients"), clientEntity->Get(_N("clientId"))->ToUnsignedInt(), clientEditFrame);
		} else {
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("Could not find the client in the database."),
				_("Edit client")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		}
	} else if(reminderType->GetModule() == ReminderType::PRODUCTS) {
		BlockType *blockType = rit->IsBlocked(_N("products"), reminderType->GetReference());
		if(blockType) {
			ProductEditFrame *productEditFrame = static_cast<ProductEditFrame *>(blockType->GetData());
			if(productEditFrame) {
				productEditFrame->Raise();
			} else {
				wxMessageDialog *errorMessageDialog = new wxMessageDialog(
					this,
					_("The product you want to edit is allready opened."),
					_("Edit product")
				);
				errorMessageDialog->Center();
				errorMessageDialog->ShowModal();
				errorMessageDialog->Destroy();
			}
			return;
		}

		ProductController productController(rit);
		ProductEntity *productEntity = productController.Load(reminderType->GetReference());
		if(productEntity) {
			ProductEditFrame *productEditFrame = new ProductEditFrame(this, -1, rit, productEntity, static_cast<ProductsModulePanel *>(rit->GetObject(_N("productsModulePanel"))));
			productEditFrame->Center();
			productEditFrame->Show();

			rit->Block(_N("products"), productEntity->Get(_N("productId"))->ToUnsignedInt(), productEditFrame);
		} else {
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("Could not find the product in the database."),
				_("Edit product")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		}
	} else if(reminderType->GetModule() == ReminderType::ORDERS) {
		BlockType *blockType = rit->IsBlocked(_N("orders"), reminderType->GetReference());
		if(blockType) {
			OrderEditFrame *orderEditFrame = static_cast<OrderEditFrame *>(blockType->GetData());
			if(orderEditFrame) {
				orderEditFrame->Raise();
			} else {
				wxMessageDialog *errorMessageDialog = new wxMessageDialog(
					this,
					_("The order you want to edit is allready opened."),
					_("Edit order")
				);
				errorMessageDialog->Center();
				errorMessageDialog->ShowModal();
				errorMessageDialog->Destroy();
			}
			return;
		}

		OrderController orderController(rit);
		orderController.Get(DSCasts::ToString(reminderType->GetReference()));
		if(orderController.Run()) {
			OrderEntity *orderEntity = new OrderEntity(rit);
			orderController.Get(orderEntity);
			orderController.Reset();

			OrderEditFrame *orderEditFrame = new OrderEditFrame(this, -1, rit, orderEntity, static_cast<OrdersModulePanel *>(rit->GetObject(_N("ordersModulePanel"))));
			orderEditFrame->Center();
			orderEditFrame->Show();

			rit->Block(_N("orders"), orderEntity->Get(_N("orderId"))->ToUnsignedInt(), orderEditFrame);
		} else {
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("Could not find the order in the database."),
				_("Edit order")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		}
	} else if(reminderType->GetModule() == ReminderType::VOUCHERS) {
		BlockType *blockType = rit->IsBlocked(_N("vouchers"), reminderType->GetReference());
		if(blockType) {
			VoucherEditFrame *voucherEditFrame = static_cast<VoucherEditFrame *>(blockType->GetData());
			if(voucherEditFrame) {
				voucherEditFrame->Raise();
			} else {
				wxMessageDialog *errorMessageDialog = new wxMessageDialog(
					this,
					_("The voucher you want to edit is allready opened."),
					_("Edit voucher")
				);
				errorMessageDialog->Center();
				errorMessageDialog->ShowModal();
				errorMessageDialog->Destroy();
			}
			return;
		}

		VoucherController voucherController(rit);
		VoucherEntity *voucherEntity = voucherController.Load(reminderType->GetReference());
		if(voucherEntity) {
			VoucherEditFrame *voucherEditFrame = new VoucherEditFrame(this, -1, rit, voucherEntity, static_cast<VouchersModulePanel *>(rit->GetObject(_N("vouchersModulePanel"))));
			voucherEditFrame->Center();
			voucherEditFrame->Show();

			rit->Block(_N("vouchers"), voucherEntity->Get(_N("voucherId"))->ToUnsignedInt(), voucherEditFrame);
		} else {
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("Could not find the voucher in the database."),
				_("Edit voucher")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		}
	} else if(reminderType->GetModule() == ReminderType::PURCHASES) {
		BlockType *blockType = rit->IsBlocked(_N("purchases"), reminderType->GetReference());
		if(blockType) {
			PurchaseEditFrame *purchaseEditFrame = static_cast<PurchaseEditFrame *>(blockType->GetData());
			if(purchaseEditFrame) {
				purchaseEditFrame->Raise();
			} else {
				wxMessageDialog *errorMessageDialog = new wxMessageDialog(
					this,
					_("The purchase you want to edit is allready opened."),
					_("Edit purchase")
				);
				errorMessageDialog->Center();
				errorMessageDialog->ShowModal();
				errorMessageDialog->Destroy();
			}
			return;
		}

		PurchaseController purchaseController(rit);
		purchaseController.Get(DSCasts::ToString(reminderType->GetReference()));
		if(purchaseController.Run()) {
			PurchaseEntity *purchaseEntity = new PurchaseEntity(rit);
			purchaseController.Get(purchaseEntity);
			purchaseController.Reset();

			PurchaseEditFrame *purchaseEditFrame = new PurchaseEditFrame(this, -1, rit, purchaseEntity, static_cast<PurchasesModulePanel *>(rit->GetObject(_N("purchasesModulePanel"))));
			purchaseEditFrame->Center();
			purchaseEditFrame->Show();

			rit->Block(_N("purchases"), purchaseEntity->Get(_N("purchaseId"))->ToUnsignedInt(), purchaseEditFrame);
		} else {
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("Could not find the purchase in the database."),
				_("Edit purchase")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		}
	} else {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Could not open entry. Unknown Type."),
			_("Open")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
	}
}

void ReminderDialog::OpenToRemind() {
	UserController userController(rit);
	if(!userController.SetOpenToRemind(
		rit->GetUserConfigurationFileEntity()->GetLastUsername(),
		openToRemindCheckBox->GetValue()
	)) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Could not save. Database error."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
	}
}

void ReminderDialog::TemplateSelect(wxObject *object) {
	ReminderType *reminderType = static_cast<ReminderType *>(remindersListCtrl->GetSingleSelectionPointer());
	if(!reminderType) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the entry you want to edit."),
			_("Save")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	wxTextCtrl *textCtrl = NULL;

	if(object == notesStaticText)
		textCtrl = notesTextCtrl;
	
	TemplateSelectDialog::ShowAndSet(this, rit, textCtrl);
}

void ReminderDialog::Remind() {
	ReminderLibrary reminderLibrary(rit);

	DSDateTime serverTime(reminderLibrary.GetServerTime());

	ReminderType *firstReminderType = NULL;
	ReminderType *secondReminderType = NULL;
	bool add = false;
	bool raise = false;
	const unsigned int reminderCount = remindersListCtrl->GetItemCount();
	RemindersType *remindersType = reminderLibrary.Get(rit->GetUserConfigurationFileEntity()->GetLastUsername());
	const unsigned int remindersCount = remindersType->GetCount();
	for(unsigned int i = 0; i < remindersCount; i++) {
		add = true;

		firstReminderType = remindersType->Get(i);
		if(!firstReminderType)
			continue;

		if(firstReminderType->GetDateTime() > serverTime)
			continue;

		for(unsigned int j = 0; j < reminderCount; j++) {
			secondReminderType = static_cast<ReminderType *>(remindersListCtrl->GetPointer(j, 0));
			if(secondReminderType) {
				if(secondReminderType->GetId() == firstReminderType->GetId()) {
					remindersListCtrl->SetItemBackgroundColour(j, *wxRED);
					add = false;
					raise = true;
					break;
				}
			}	
		}

		if(add) {
			AddOrChangeToRemindersListCtrl(firstReminderType);
			raise = true;
		} else {
			delete firstReminderType;
		}
	}

	if(raise) {
		UserController userController(rit);
		if(userController.GetOpenToRemind(rit->GetUserConfigurationFileEntity()->GetLastUsername())) {
			static_cast<WarehouseFrame *>(GetParent())->Reminder(true);
			wxBell();
		}
	}

	delete remindersType;

	UpdateCalendars();
}
