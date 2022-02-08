#ifndef RUNTIMEINFORMATIONTYPE_H_
#define RUNTIMEINFORMATIONTYPE_H_

#include <wx/wx.h>
#include <DSLib.h>
#include <DSPostgreSQLDatabase.h>
#include <DSLog.h>
#include <DSLogType.h>
#include <DSTemplateContainer.h>
#include "UserConfigurationFileEntity.h"
#include "BlockType.h"
#include "BlockLibrary.h"
#include "ObjectType.h"

#define A_NONE          0
#define A_READ          1
#define A_WRITE         2
#define A_DELETE        3
#define A_ADMINISTRATOR 4

using namespace DynSoft;

class RuntimeInformationType {

	public:
		RuntimeInformationType(
			const wxString &executableFile,
			const wxString &resourcePath,
			const wxString &userConfigurationFile,
			DSPostgreSQLDatabase *db                     = NULL,
			const bool deinitializeDbOnDelete            = true,
			DSPostgreSQLDatabase *enterprisesDb          = NULL,
			const bool deinitializeEnterprisesDbOnDelete = true,
			DSLog *log                                   = NULL,
			const bool deinitializeLogOnDelete           = true
		);
		~RuntimeInformationType();

		DSPostgreSQLDatabase *SetDatabase(
			DSPostgreSQLDatabase *db,
			const bool deinitializeDbOnDelete
		);
		DSPostgreSQLDatabase *GetDatabase() const;

		DSPostgreSQLDatabase *SetEnterprisesDatabase(
			DSPostgreSQLDatabase *enterprisesDb,
			const bool deinitializeEnterprisesDbOnDelete
		);
		DSPostgreSQLDatabase *GetEnterprisesDatabase() const;

		DSLog *SetLog(
			DSLog *log,
			const bool deinitializeLogOnDelete
		);
		DSLog *GetLog() const;
		bool Log(
			const bool error,
			const wxString &module,
			const wxString &description,
			const wxString &message = wxEmptyString
		);

		wxString GetResourcePath() const;
		wxString GetResourcePath(
			const wxString &directory,
			const wxString &file
		) const;
		wxString GRP() const;
		wxString GRP(
			const wxString &directory,
			const wxString &file
		) const;

		static wxString GetSep();
		static wxString GetUserConfigurationDirectory(const bool addSeparator = true);
		static wxString GetConfigurationDirectory(const bool addSeparator = true);
		wxString GetUserConfigurationFileWithPath() const;

		void SetUserConfigurationFile(const wxString &userConfigurationFile);
		wxString GetUserConfigurationFile() const;

		wxString GetTemporaryPath() const;

		UserConfigurationFileEntity *GetUserConfigurationFileEntity();
		
		bool Access(const unsigned int status, const wxString &message = wxEmptyString, wxWindow *parent = NULL);
		void Block(const wxString &module, const wxString &number, void *data = NULL, const bool deleteOldData = false);
		void Block(const wxString &module, const unsigned int number, void *data = NULL, const bool deleteOldData = false);
		void Unblock(const wxString &module, const wxString &number, const bool deleteOldData = false);
		void Unblock(const wxString &module, const unsigned int number, const bool deleteOldData = false);
		BlockType *IsBlocked(const wxString &module, const wxString &number);
		BlockType *IsBlocked(const wxString &module, const unsigned int number);

		wxString GetExecutableFile() const;
		wxString GetExecutablePath() const;

		void SetObject(const wxString &name, void *object);
		void *GetObject(const wxString &name);

		void SetDemoVersion(const bool demoVersion);
		bool IsDemoVersion() const;

		void SetLocale(wxLocale *locale);
		wxLocale *GetLocale();
		wxString Translate(const wxString &name);

		void SetEnterpriseName(const wxString &enterpriseName);
		wxString GetEnterprisesName() const;

	private:
		wxString executableFile;
		wxString executablePath;
		wxString resourcePath;
		wxString userConfigurationFile;
		DSPostgreSQLDatabase *db;
		DSPostgreSQLDatabase *enterprisesDb;
		DSLog *log;
		UserConfigurationFileEntity userConfigurationFileEntity;
		bool deinitializeLogOnDelete;
		bool deinitializeDbOnDelete;
		bool deinitializeEnterprisesDbOnDelete;
		BlockLibrary blockLibrary;
		DSTemplateContainer<ObjectType *> objects;
		bool demoVersion;
		wxLocale *locale;
		wxString enterpriseName;

};

#endif /* RUNTIMEINFORMATIONTYPE_H_ */
