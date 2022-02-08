#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/ffile.h>

void print(const wxString &str) {
	printf("%s\n", (const char *) str.mb_str(wxConvUTF8));
}

void extract_names(wxString filename, wxString &area, wxString &element) {
	int pos = filename.Find(_("."));
	if(pos != wxNOT_FOUND)
		filename = filename.Mid(0, pos);

	bool found = false;
	const size_t length = filename.Length();
	for(size_t i = 0; i < length; i++) {
		if(i == 0 || (i > 0 && (filename[i] >= 'a' && filename[i] <= 'z')))
			area += filename[i];
		else
			break;
			
	}

	element = filename;
}

void extract(const wxString &path, const wxString &filename) {
	wxFFile file(path + filename, _("r"));
	if(!file.IsOpened()) {
		fprintf(stderr, "Could not open file: %s\n", (const char *) filename.mb_str(wxConvUTF8));
	} else {
		wxString content;
		file.ReadAll(&content);
		file.Close();

		wxString area, element;
		extract_names(filename, area, element);

		bool in = false;
		wxString placeholder;
		const size_t length = content.Length();
		for(size_t i = 0; i < length; i++) {
			if(content[i] == '{') {
				in = true;
			} else if(content[i] == '}') {
				in = false;
				placeholder += content[i];
				print(area + _(";;") + element + _(";;") + placeholder + _(";"));
				placeholder.Clear();
			}

			if(in)
				placeholder += content[i];
		}
	}
}

int main(int argc, char *argv[]) {
	if(argc != 2) {
		fprintf(stderr, "Usage: %s path\n", argv[0]);
		return 1;
	}

	wxString path(wxString(argv[1], wxConvUTF8));
	if(path.Length() > 0 && path[path.Length() - 1] != '/')
		path += _("/");
	wxDir dir(path);
	if(!dir.IsOpened()) {
		fprintf(stderr, "Could not open dir %s\n", argv[1]);
		return 2;
	}

	wxString filename;
	bool ok = dir.GetFirst(&filename);
	while(ok) {
		extract(path, filename);
		ok = dir.GetNext(&filename);
	}

	return 0;
}
