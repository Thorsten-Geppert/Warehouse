#include "SendController.h"
#include <wx/uri.h>
#include <wx/protocol/http.h>
#include <wx/stream.h>
#include <wx/sstream.h>

unsigned int SendController::Send(
	const wxString &contactUrl,
	const wxString &contactFile,
	DSEntity *entity,
	wxString *reply
) {
	if(!entity)
		return ENTITY_IS_NULL;
	
	wxString send;
	DSEntity::DSEntityType *entry = NULL;
	const unsigned int count = entity->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		entry = entity->Get(i);
		if(!entry)
			continue;

		wxURI uri(entry->ToString());
		send.Append(entry->GetKey() + _N("=") + uri.BuildURI());
		if(i < count - 1)
			send.Append(_N("&"));
	}

	wxHTTP http;
	wxString url;
	http.SetHeader(_N("Content-type"), _N("application/x-www-form-urlencoded"));
	http.SetTimeout(10);
	#if wxMAJOR_VERSION < 3
		http.SetPostBuffer(send);
	#else
		http.SetPostText(_N("text/html"), send);
	#endif
	if(!http.Connect(contactUrl))
		return NO_CONNECTION;

	wxInputStream *input  = http.GetInputStream(contactFile);
	wxProtocolError error = http.GetError();
	const int response    = http.GetResponse();
	if(response != 200)
		return INCORRECT_RESPONSE;
	if(error != wxPROTO_NOERR)
		return UNKNOWN_ERROR;
	
	wxStringOutputStream output;
	input->Read(output);
	wxString result(output.GetString());
	if(reply)
		*reply = result;
	if(result != _N("OK"))
		return SERVER_ERROR;

	return SEND;
}
