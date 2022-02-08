#include "ProductController.h"
#include <DSPostgreSQLQuery.h>
#include <DSCgi.h>
#include "ConstantsLibrary.h"
#include "LinkedFileController.h"
#include "Tools.h"

ProductController::ProductController(
	RuntimeInformationType *rit
) : Controller(
	rit,
	_N("products"), // Table
	_N("productId"),  // Primary key
	_N("productId")
) {
	LoadResource(rit->GRP(_N("sql"), _N("Products.res")));
}

void ProductController::Find(ProductEntity *productEntity, const bool onlyAvailable) {
	const wxString productId(WildCard(productEntity->Get(_N("productId"))->ToString()));
	const wxString articleNumber(WildCard(productEntity->Get(_N("articleNumber"))->ToString()));
	const wxString shortName(WildCard(productEntity->Get(_N("shortName"))->ToString()));
	const wxString name1(WildCard(productEntity->Get(_N("name1"))->ToString()));
	const wxString name2(WildCard(productEntity->Get(_N("name1"))->ToString()));
	const wxString name3(WildCard(productEntity->Get(_N("name1"))->ToString()));
	const wxString name4(WildCard(productEntity->Get(_N("name1"))->ToString()));
	const wxString description(WildCard(productEntity->Get(_N("description"))->ToString()));
	const wxString note(WildCard(productEntity->Get(_N("note"))->ToString()));
	const wxString isbn10(WildCard(productEntity->Get(_N("isbn10"))->ToString()));
	const wxString isbn13(WildCard(productEntity->Get(_N("isbn13"))->ToString()));
	const wxString ean(WildCard(productEntity->Get(_N("ean"))->ToString()));
	const wxString group(WildCard(productEntity->Get(_N("group"))->ToString()));
	const wxString distributorId(WildCard(productEntity->Get(_N("distributorId"))->ToString(), true));
	const wxString manufacturer(WildCard(productEntity->Get(_N("manufacturer"))->ToString()));
	const wxString publisher(WildCard(productEntity->Get(_N("publisher"))->ToString()));
	
	DSQueryParser parser;
	parser.Prepare(GetResourceByName(onlyAvailable ? _N("Products.FindAvailable") : _N("Products.Find")));
	parser.Bind(_N("table"), GetTable(), false);
	parser.Bind(_N("productId"), productId);
	parser.Bind(_N("articleNumber"), articleNumber);
	parser.Bind(_N("shortName"), shortName);
	parser.Bind(_N("name1"), name1);
	parser.Bind(_N("name2"), name2);
	parser.Bind(_N("name3"), name3);
	parser.Bind(_N("name4"), name4);
	parser.Bind(_N("description"), description);
	parser.Bind(_N("note"), note);
	parser.Bind(_N("isbn10"), isbn10);
	parser.Bind(_N("isbn13"), isbn13);
	parser.Bind(_N("ean"), ean);
	parser.Bind(_N("group"), group);
	parser.Bind(_N("distributorId"), distributorId);
	parser.Bind(_N("manufacturer"), manufacturer);
	parser.Bind(_N("publisher"), publisher);
	Create(parser.GetParsedQuery());
}

int ProductController::GetTax(RuntimeInformationType *rit, const unsigned int productId) {
	unsigned int tax = -1;

	ProductController productController(rit);
	productController.Get(DSCasts::ToString(productId));
	if(productController.Run()) {
		ProductEntity productEntity(rit);
		productController.Get(&productEntity);
		productController.Reset();
		tax = productEntity.Get(_N("tax"))->ToInt();
	}

	return tax;
}

HtmlPrintoutLibrary *ProductController::Print(ProductEntity *productEntity) {
	HtmlPrintoutLibrary *htmlPrintoutLibrary = new HtmlPrintoutLibrary(_("Product"), rit);

	DSTemplate productTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("Product"));
	productTemplate.SetEntity(productEntity);
	productTemplate.SetVariable(_N("tax"), ConstantsLibrary::TaxRateToString(productEntity->Get(_N("tax"))->ToUnsignedInt()));
	productTemplate.SetVariable(_N("available"), productEntity->Get(_N("available"))->ToBool() ? _("yes") : _("no"));

	productTemplate.SetVariable(_N("marginWithoutTax"), DSCurrency(productEntity->Get(_N("priceWithoutTax"))->ToCurrency() - productEntity->Get(_N("purchasePriceWithoutTax"))->ToCurrency()).ToString());
	productTemplate.SetVariable(_N("marginWithTax"), DSCurrency(productEntity->Get(_N("priceWithTax"))->ToCurrency() - productEntity->Get(_N("purchasePriceWithTax"))->ToCurrency()).ToString());

	productTemplate.SetVariable(_N("description"), nl2br(HtmlSpecialChars(productEntity->Get(_N("description"))->ToString())));
	productTemplate.SetVariable(_N("weight"), Tools::RemoveLastZeros(productEntity->Get(_N("weight"))->ToString()));

	// Linked files
	LinkedFileController linkedFileController(rit, _N("linked_files_products"));
	productTemplate.SetDynamicVariable(_N("linkedFiles"), productEntity->GetLinkedFileEntities()->GetCount() == 0 ? wxString(_("No linked files exist")) : linkedFileController.Print(*htmlPrintoutLibrary, *productEntity->GetLinkedFileEntities()));

	// More data
	productTemplate.SetDynamicVariable(_N("moreData"), productEntity->GetKeysValuesType()->GetCount() == 0 ? wxString(_("No more data exist")) : PrintKeysValues(*htmlPrintoutLibrary, *productEntity->GetKeysValuesType()));

	productTemplate.Parse();
	htmlPrintoutLibrary->SetWithContainer(productTemplate.Get());

	// Header
	DSTemplate productHeaderTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("ProductHeader"));
	productHeaderTemplate.SetVariable(_N("name1"), productEntity->Get(_N("name1"))->ToString(), true);
	productHeaderTemplate.Parse();
	htmlPrintoutLibrary->SetHeader(productHeaderTemplate.Get());

	// Footer
	DSTemplate productFooterTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("ProductFooter"));
	productFooterTemplate.Parse();
	htmlPrintoutLibrary->SetFooter(productFooterTemplate.Get());

	return htmlPrintoutLibrary;
}

ProductEntity *ProductController::GetDirect(RuntimeInformationType *rit, const unsigned int id) {
	ProductEntity *productEntity = NULL;
	ProductController productController(rit);
	productController.Get(DSCasts::ToString(id));
	if(productController.Run()) {
		productEntity = new ProductEntity(rit);
		productController.Get(productEntity);
		productController.Reset();

		LinkedFileController linkedFileController(rit, _N("linked_files_products"));
		linkedFileController.Load(
			productEntity->Get(_N("productId"))->ToUnsignedInt(),
			*productEntity->GetLinkedFileEntities()
		);
		
		productController.LoadKeysValues(
			productEntity->Get(_N("productId"))->ToUnsignedInt(),
			_N("keys_values_products"),
			*productEntity->GetKeysValuesType()
		);
	}
	return productEntity;
}

void ProductController::Change(ProductEntity *productEntity) {
	productEntity->SetValue(_N("productId"), 0);
	productEntity->SetValue(_N("createdAt"), _N("NOW()"));
	productEntity->SetValue(_N("changeAt"), _N("NOW()"));

	productEntity->GetLinkedFileEntities()->ResetId();
}

ProductEntity *ProductController::Load(const unsigned int productId) {
	ProductEntity *productEntity = NULL;

	if(productId == 0)
		return productEntity;
	
	ProductController productController(rit);
	productController.Get(DSCasts::ToString(productId));
	if(productController.Run()) {
		productEntity = new ProductEntity(rit);
		productController.Get(productEntity);
		productController.Reset();

		LinkedFileController linkedFileController(rit, _N("linked_files_products"));
		linkedFileController.Load(
			productEntity->Get(_N("productId"))->ToUnsignedInt(),
			*productEntity->GetLinkedFileEntities()
		);
		
		LoadKeysValues(
			productEntity->Get(_N("productId"))->ToUnsignedInt(),
			_N("keys_values_products"),
			*productEntity->GetKeysValuesType()
		);
	}

	return productEntity;
}

bool ProductController::Save(ProductEntity *productEntity) {
	bool ok = true;

	DSPostgreSQLQuery query(ndb);
	query.Begin();

	ProductController productController(rit);
	if(productEntity->Get(_N("productId"))->ToUnsignedInt()) {
		productEntity->ForceChanged();
		ok = productController.Update(productEntity, wxEmptyString, true, productEntity->GetUpdateExcludes(), &query);
	} else {
		ok = productController.Insert(productEntity, true, productEntity->GetInsertExcludes(), &query);
	}

	if(ok) {
		LinkedFileController linkedFileController(rit, _N("linked_files_products"));
		ok = linkedFileController.Save(
			query,
			productEntity->Get(_N("productId"))->ToUnsignedInt(),
			*productEntity->GetLinkedFileEntities()
		);
	}

	if(ok) {
		ok = SaveKeysValues(
			query,
			productEntity->Get(_N("productId"))->ToUnsignedInt(),
			_N("keys_values_products"),
			*productEntity->GetKeysValuesType()
		);
	}

	if(!ok || !query.Commit()) {
		query.Rollback();
		return false;
	} else {
		productEntity->GetLinkedFileEntities()->CleanRemoved();
	}

	return true;
}

wxString ProductController::GetReminderDescription(const unsigned int id) {
	ProductController productController(rit);
	productController.Get(DSCasts::ToString(id));
	if(productController.Run()) {
		ProductEntity productEntity(rit);
		productController.Get(&productEntity);
		productController.Reset();

		return productEntity.Get(_N("shortName"))->ToString();
	}
	return wxEmptyString;
}

unsigned int ProductController::GetClientReferences(
	const unsigned int primaryKey,
	ReferencesType *referencesType
) {
	unsigned int counter = 0;
	if(!referencesType)
		return counter;

	DSPostgreSQLQuery query(ndb);
	query.Prepare(GetResourceByName(_N("Products.ReferenceByClientId")));
	query.Bind(_N("distributorId"), primaryKey);
	query.Exec();
	while(query.NextRecord()) {
		referencesType->Append(
			ConstantsLibrary::CLIENT_KIND,
			query.GetField(_N("id")).ToUnsignedInt(),
			wxEmptyString
		);
		counter++;
	}
	query.FreeResult();

	return counter;
}
