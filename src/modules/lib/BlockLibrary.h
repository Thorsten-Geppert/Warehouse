#ifndef BLOCK_LIBRARY_H_
#define BLOCK_LIBRARY_H_

#include <DSTemplateContainer.h>
#include "BlockType.h"

using namespace DynSoft;

class BlockLibrary : public DSTemplateContainer<BlockType *> {

	public:
		BlockLibrary();

		void Set(const wxString &key, const wxString &value, void *data = NULL, const bool deleteOldData = false);
		void Unset(const wxString &key, const wxString &value, const bool deleteData = false);
		BlockType *Find(const wxString &key, const wxString &value);
		unsigned int GetCountByKey(const wxString &key);

};

#endif /* BLOCK_LIBRARY_H_ */
