#include "StockBookEntities.h"

StockBookEntities::StockBookEntities(const bool autoCleanUp) : DSTemplateContainer<StockBookEntity *>() {
	this->autoCleanUp = autoCleanUp;
}

StockBookEntities::~StockBookEntities() {
	if(autoCleanUp)
		Clear(true);
}
