#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#define NAME       _N("DynSoft.com - Thorsten Geppert - Warehouse Admin")
#define SOFTWARE   _N("Warehouse Admin")
#define VERSION    _N("4.0.0")
#define ENTERPRISE _N("DynSoft.com - Thorsten Geppert")
#define TITLE      ENTERPRISE + _N(" - ") + SOFTWARE + _N(" ") + VERSION

#define USER_CONFIGURATION_FILE _N(".DynSoftWarehouse.ini")
#define URL _N("https://www.dynsoft.com")

#ifdef __FREEBSD__
	#define PG_DUMP _N("pg_dump")
	#define PSQL _N("psql")
#else
	#ifdef __INTEL__
		#ifdef __WXMAC__
			#define PG_DUMP _N("resources/postgresql/mac/pg_dump")
			#define PSQL _N("resources/postgresql/mac/psql")
		#elif defined __WXMSW__
			#define PG_DUMP _N("resources/postgresql/windows/pg_dump.exe")
			#define PSQL _N("resources/postgresql/windows/psql.exe")
		#else
			#define PG_DUMP _N("pg_dump")
			#define PSQL _N("psql")
		#endif
	#else
		#ifdef __WXMAC__
			#define PG_DUMP _N("resources/postgresql/mac/pg_dump")
			#define PSQL _N("resources/postgresql/mac/psql")
		#elif defined __WXMSW__
			#define PG_DUMP _N("resources/postgresql/windows/pg_dump.exe")
			#define PSQL _N("resources/postgresql/windows/psql.exe")
		#else
			#define PG_DUMP _N("pg_dump")
			#define PSQL _N("psql")
		#endif
	#endif
#endif

#ifdef __WXMSW__
	#define ICON() SetIcon(wxIcon(_N("AdminIcon.ico"), wxBITMAP_TYPE_ICO))
#else
	#define ICON() SetIcon(wxIcon(AdminIcon_xpm))
#endif

#ifdef __WXMAC__
	#define RESOURCE_PATH _N("WarehouseAdmin.app/Contents/Resources/languages/")
#elif defined __WXMSW__
	#define RESOURCE_PATH _N(".\\resources\\languages\\")
#else
	#define RESOURCE_PATH _N("./resources/languages/")
#endif

#endif /* CONFIGURATION_H_ */
