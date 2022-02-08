#ifndef FILE_ENTTITY_H_
#define FILE_ENTTITY_H_

#include "Entity.h"
#include "RuntimeInformationType.h"

class FileEntity : public Entity {

	public:
		FileEntity(RuntimeInformationType *rit);

		static FileEntity *CreateByFile(
			RuntimeInformationType *rit,
			const wxString &filename,
			const unsigned int fileId = 0,
			const unsigned int categoryId = 0,
			const wxString &name = wxEmptyString,
			const wxString &description = wxEmptyString
		);

};

#endif /* FILE_ENTTITY_H_ */
