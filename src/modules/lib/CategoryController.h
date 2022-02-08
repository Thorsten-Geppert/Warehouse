#ifndef CATEGORY_CONTROLLER_H_
#define CATEGORY_CONTROLLER_H_

#include "Controller.h"
#include "RuntimeInformationType.h"
#include "CategoryEntities.h"

using namespace DynSoft;

class CategoryController : public Controller {

	public:
		CategoryController(
			RuntimeInformationType *rit,
			const wxString &table
		);

		CategoryEntities *GetAll();
		bool ExistsInCategory(CategoryEntity &categoryEntity, const wxString &oldName = wxEmptyString);
		void GetPath(const unsigned int categoryId, wxArrayString *path, wxArrayString *ids = NULL);
		wxString ConvertPathToString(wxArrayString &path, const wxString &separator = _N("/"), const wxString &prepend = wxEmptyString);

};

#endif /* CATEGORY_CONTROLLER_H_ */
