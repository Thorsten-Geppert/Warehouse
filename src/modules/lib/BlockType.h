#ifndef BLOCK_TYPE_H_
#define BLOCK_TYPE_H_

#include <DSLib.h>

using namespace DynSoft;

class BlockType {

	public:
		BlockType(	
			const wxString &key,
			const wxString &value,
			void *data
		);

		void Set(
			const wxString &key,
			const wxString &value,
			void *data
		);

		void SetKey(const wxString &key);
		void SetValue(const wxString &value);
		void SetData(void *data);

		wxString GetKey() const;
		wxString GetValue() const;
		void *GetData() const;
	
	protected:
		wxString key;
		wxString value;
		void *data;

};

#endif /* BLOCK_TYPE_H_ */
