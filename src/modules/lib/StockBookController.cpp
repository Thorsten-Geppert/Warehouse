#include "StockBookController.h"
#include <DSPostgreSQLQuery.h>
#include <DSDateTime.h>

StockBookController::StockBookController(
	RuntimeInformationType *rit
) : Controller(
	rit,
	_N("stock_books"), // Table
	_N("stockBookId"),  // Primary key
	_N("created")
) {
	LoadResource(rit->GRP(_N("sql"), _N("StockBook.res")));
}

bool StockBookController::GetByStockAndProduct(StockBookEntity &stockBookEntity) {
	unsigned int id = 0;

	DSPostgreSQLQuery query(ndb);
	query.Prepare(GetResourceByName(_N("StockBook.GetResourceByName")));
	query.Bind(_N("table"), GetTable(), false);
	query.Bind(_N("stock"), stockBookEntity.Get(_N("stock"))->ToString());
	query.Bind(_N("product"), stockBookEntity.Get(_N("product"))->ToUnsignedInt());
	query.Exec();
	if(query.NextRecord())
		id = query.GetField(_N("id")).ToUnsignedInt();
	query.FreeResult();

	if(id) {
		StockBookController stockBookController(rit);
		stockBookController.Get(id);
		if(stockBookController.Run()) {
			stockBookController.Get(&stockBookEntity);
			stockBookController.Reset();
		} else {
			id = 0;
		}
	}

	return id != 0;
}

StockBookEntities *StockBookController::GetSeparately(const bool autoCleanUp) {
	StockBookEntities *stockBookEntities = new StockBookEntities(autoCleanUp);

	StockBookEntity *stockBookEntity = NULL;
	StockBookController stockBookController(rit);
	stockBookController.Get();
	while(stockBookController.Run()) {
		stockBookEntity = new StockBookEntity(rit);
		stockBookController.Get(stockBookEntity);
		stockBookEntities->Add(stockBookEntity);
	}

	return stockBookEntities;
}

StockBookEntities *StockBookController::GetSummarized(const bool autoCleanUp) {
	StockBookEntities *stockBookEntities = new StockBookEntities(autoCleanUp);

	bool found = false;
	DSCurrency quantity;
	unsigned int count = 0;
	StockBookEntity *oldStockBookEntity = NULL;
	StockBookEntity *stockBookEntity = NULL;
	StockBookController stockBookController(rit);
	stockBookController.Get();
	while(stockBookController.Run()) {
		found = false;

		stockBookEntity = new StockBookEntity(rit);
		stockBookController.Get(stockBookEntity);

		count = stockBookEntities->GetCount();
		for(unsigned int i = 0; i < count && !found; i++) {
			oldStockBookEntity = stockBookEntities->Get(i);
			if(oldStockBookEntity) {
				if(
					oldStockBookEntity->Get(_N("stock"))->ToString() == stockBookEntity->Get(_N("stock"))->ToString() &&
					oldStockBookEntity->Get(_N("product"))->ToUnsignedInt() == stockBookEntity->Get(_N("product"))->ToUnsignedInt()
				) {
					quantity = stockBookEntity->Get(_N("quantity"))->ToCurrency() + oldStockBookEntity->Get(_N("quantity"))->ToCurrency();
					oldStockBookEntity->SetValue(_N("quantity"), quantity);
					delete stockBookEntity;
					found = true;
				}
			}
		}

		if(!found)
			stockBookEntities->Add(stockBookEntity);
	}

	return stockBookEntities;
}

HtmlPrintoutLibrary *StockBookController::PrintList(DSListCtrl *listCtrl) {
	HtmlPrintoutLibrary *htmlPrintoutLibrary = new HtmlPrintoutLibrary(_("Stock bookings"), rit);

	DSTemplate stockBookingsTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("StockBookings"));
	stockBookingsTemplate.SetVariable(_N("table"), htmlPrintoutLibrary->CreateTableByListCtrl(listCtrl));
	stockBookingsTemplate.Parse();
	htmlPrintoutLibrary->SetWithContainer(stockBookingsTemplate.Get());

	// Header
	DSDateTime dt;
	dt.SetWxDateTime(wxDateTime::Now());

	DSTemplate stockBookingsHeaderTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("StockBookingsHeader"));
	stockBookingsHeaderTemplate.SetVariable(_N("date"), dt.ToDatabaseDateTime());
	stockBookingsHeaderTemplate.Parse();
	htmlPrintoutLibrary->SetHeader(stockBookingsHeaderTemplate.Get());

	// Footer
	DSTemplate stockBookingsFooterTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("StockBookingsFooter"));
	stockBookingsFooterTemplate.Parse();
	htmlPrintoutLibrary->SetFooter(stockBookingsFooterTemplate.Get());

	return htmlPrintoutLibrary;
}

DSCurrency StockBookController::GetQuantityByStockAndProduct(const wxString &stock, const unsigned int product) {
	DSCurrency quantity;

	DSPostgreSQLQuery query(ndb);
	query.Prepare(GetResourceByName(_N("StockBook.GetQuantityByStockAndProduct")));
	query.Bind(_N("table"), GetTable(), false);
	query.Bind(_N("stock"), stock);
	query.Bind(_N("product"), product);
	query.Exec();
	if(query.NextRecord())
		quantity = query.GetField(_N("quantity")).ToCurrency();
	query.FreeResult();

	return quantity;
}

DSCurrency StockBookController::GetReservedByStockAndProduct(const wxString &stock, const unsigned int product) {
	DSCurrency quantity;

	DSPostgreSQLQuery query(ndb);
	query.Prepare(GetResourceByName(_N("StockBook.GetReservedByStockAndProduct")));
	query.Bind(_N("stock"), stock);
	query.Bind(_N("product"), product);
	query.Exec();
	if(query.NextRecord())
		quantity = query.GetField(_N("quantity")).ToCurrency();
	query.FreeResult();

	return quantity;
}
