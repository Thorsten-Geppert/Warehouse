#ifndef REFERENCE_TYPE_H_
#define REFERENCE_TYPE_H_

#include <DSContainerType.h>

using namespace DynSoft;

class ReferenceType : public DSContainerType {

	public:
		ReferenceType(
			const unsigned int kind,
			const unsigned int id,
			const wxString &message
		);

		void SetKind(const unsigned int kind);
		unsigned int GetKind() const;

		void SetId(const unsigned int id);
		unsigned int GetId() const;

		void SetMessage(const wxString &message);
		wxString GetMessage() const;
	
	private:
		unsigned int kind;
		unsigned int id;
		wxString message;

};

#endif /* REFERENCE_TYPE_H_ */
