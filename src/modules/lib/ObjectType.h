#ifndef OBJECT_TYPE_H_
#define OBJECT_TYPE_H_

#include <wx/wx.h>

class ObjectType {

	public:
		ObjectType(const wxString &name, void *object);
		void SetName(const wxString &name);
		wxString GetName() const;
		void SetObject(void *object);
		void *GetObject() const;
	
	private:
		wxString name;
		void *object;

};

#endif /* OBJECT_TYPE_H_ */
