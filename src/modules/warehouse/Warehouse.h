#ifndef WAREHOUSE_H_
#define WAREHOUSE_H_

#include <wx/wx.h>
#include <DSPostgreSQLDatabase.h>
#include "../lib/RuntimeInformationType.h"

using namespace DynSoft;

class Warehouse : public wxApp {

	public:
		~Warehouse();
		bool OnInit();
	
	private:
		static const unsigned int OK = 0;
		static const unsigned int CANCEL = 1;
		static const unsigned int COULD_NOT_CONNECT_TO_DATABASE = 2;

		RuntimeInformationType *rit;

		unsigned int ConnectToDatabase(RuntimeInformationType *rit);

};

#endif /* WAREHOUSE_H_ */
