#ifndef USER_CONTROLLER_H_
#define USER_CONTROLLER_H_

#include <DSPostgreSQLDatabase.h>
#include <DSPostgreSQLEntityData.h>
#include "RuntimeInformationType.h"
#include "UserEntity.h"

using namespace DynSoft;

class UserController : public DSPostgreSQLEntityData {

	public:
		UserController(
			DSPostgreSQLDatabase *db
		);

		UserController(
			RuntimeInformationType *rit
		);

		bool Login(
			const wxString &username,
			const wxString &password
		);

		bool SetOpenToRemind(const wxString &username, const bool openToRemind);
		bool GetOpenToRemind(const wxString &username);
	
	protected:
		RuntimeInformationType *rit;

};

#endif /* USER_CONTROLLER_H_ */
