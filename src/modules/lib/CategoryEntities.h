#ifndef CATEGORY_ENTITIES_H_
#define CATEGORY_ENTITIES_H_

#include <wx/treectrl.h>
#include <wx/treebase.h>
#include <DSTemplateContainer.h>
#include "CategoryEntity.h"

using namespace DynSoft;

class CategoryEntities : public DSTemplateContainer<CategoryEntity *> {

	public:
		CategoryEntities();

		void FillTreeCtrl(wxTreeCtrl &treeCtrl, wxTreeItemId treeItem, const unsigned int select, const bool clean = false);
		bool HasChildren(const unsigned int id);

};

#endif /* CATEGORY_ENTITIES_H_ */
