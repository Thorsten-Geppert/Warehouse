#ifndef WINDOWPREFERENCESTYPE_H_
#define WINDOWPREFERENCESTYPE_H_

#include <wx/wx.h>

class WindowPreferencesType {

	public:
		void SetSize(const wxSize &size);
		void SetSize(const int width, const int height);
		wxSize GetSize() const;
		void GetSize(int *width, int *height) const;

		void SetPosition(const wxPoint &position);
		void SetPosition(const int x, const int y);
		wxPoint GetPosition() const;
		void GetPosition(int *x, int *y) const;

	protected:
		wxSize size;
		wxPoint position;

};

#endif /* WINDOWPREFERENCESTYPE_H_ */
