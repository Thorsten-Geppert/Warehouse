#include "RuntimeInformationType.h"
#include <DSPostgreSQLQuery.h>
#include <DSDateTime.h>
#include <DSCasts.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include "Access.h"

RuntimeInformationType::RuntimeInformationType(
	const wxString &executableFile,
	const wxString &resourcePath,
	const wxString &userConfigurationFile,
	DSPostgreSQLDatabase *db,
	const bool deinitializeDbOnDelete,
	DSPostgreSQLDatabase *enterprisesDb,
	const bool deinitializeEnterprisesDbOnDelete,
	DSLog *log,
	const bool deinitializeLogOnDelete
) {
	wxFileName executablePathFileName(executableFile);

	this->executableFile                    = executableFile;
	this->executablePath                    = executablePathFileName.GetPath() + GetSep();
	this->resourcePath                      = /*this->executablePath + */resourcePath;
	this->userConfigurationFile             = userConfigurationFile;
	this->db                                = db ? db : new DSPostgreSQLDatabase;
	this->deinitializeDbOnDelete            = deinitializeDbOnDelete;
	this->enterprisesDb                     = enterprisesDb ? enterprisesDb : new DSPostgreSQLDatabase;
	this->deinitializeEnterprisesDbOnDelete = deinitializeEnterprisesDbOnDelete;
	this->log                               = log;
	this->deinitializeLogOnDelete           = deinitializeLogOnDelete;
}

RuntimeInformationType::~RuntimeInformationType() {
	if(deinitializeLogOnDelete)
		delete log;
	if(deinitializeDbOnDelete)
		delete db;
	if(deinitializeEnterprisesDbOnDelete)
		delete enterprisesDb;
	
	ObjectType *objectType = NULL;
	const unsigned int count = objects.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		objectType = objects.Get(i);
		if(objectType)
			delete objectType;
	}
}

DSPostgreSQLDatabase *RuntimeInformationType::SetDatabase(
	DSPostgreSQLDatabase *db,
	const bool deinitializeDbOnDelete
) {
	DSPostgreSQLDatabase *oldDb  = GetDatabase();
	this->db                     = db;
	this->deinitializeDbOnDelete = deinitializeDbOnDelete;
	return oldDb;
}

DSPostgreSQLDatabase *RuntimeInformationType::GetDatabase() const {
	return db;
}

DSPostgreSQLDatabase *RuntimeInformationType::SetEnterprisesDatabase(
	DSPostgreSQLDatabase *enterprisesDb,
	const bool deinitializeEnterprisesDbOnDelete
) {
	DSPostgreSQLDatabase *oldEnterprisesDb  = GetEnterprisesDatabase();
	this->enterprisesDb                     = enterprisesDb;
	this->deinitializeEnterprisesDbOnDelete = deinitializeEnterprisesDbOnDelete;
	return oldEnterprisesDb;
}

DSPostgreSQLDatabase *RuntimeInformationType::GetEnterprisesDatabase() const {
	return enterprisesDb;
}

DSLog *RuntimeInformationType::SetLog(
	DSLog *log,
	const bool deinitializeLogOnDelete
) {
	DSLog *oldLog                 = GetLog();
	this->log                     = log;
	this->deinitializeLogOnDelete = deinitializeLogOnDelete;
	return oldLog;
}

DSLog *RuntimeInformationType::GetLog() const {
	return log;
}

bool RuntimeInformationType::Log(
	const bool error,
	const wxString &module,
	const wxString &description,
	const wxString &message
) {
	DSLog *log = GetLog();
	if(!log)
		return false;
	
	DSDateTime dt;
	dt.SetWxDateTime(wxDateTime::Now());

	DSLogType entry(
		error ? 0 : 1,
		GetUserConfigurationFileEntity()->GetLastUsername(),
		module,
		description,
		message,
		dt
	);
	return log->Add(entry, true);
}

wxString RuntimeInformationType::GetSep() {
	return wxFileName::GetPathSeparator();
}

wxString RuntimeInformationType::GetUserConfigurationDirectory(const bool addSeparator) {
	return wxStandardPaths::Get().GetUserConfigDir() + (addSeparator ? GetSep() : wxString());
}

wxString RuntimeInformationType::GetConfigurationDirectory(const bool addSeparator) {
	return wxStandardPaths::Get().GetConfigDir() + (addSeparator ? GetSep() : wxString());
}

wxString RuntimeInformationType::GetResourcePath() const {
	return resourcePath;
}

wxString RuntimeInformationType::GetResourcePath(
	const wxString &directory,
	const wxString &file
) const {
	return resourcePath + directory + GetSep() + file;
}

wxString RuntimeInformationType::GRP() const {
	return GetResourcePath();
}

wxString RuntimeInformationType::GRP(
	const wxString &directory,
	const wxString &file
) const {
	return GetResourcePath(directory, file);
}

void RuntimeInformationType::SetUserConfigurationFile(const wxString &userConfigurationFile) {
	this->userConfigurationFile = userConfigurationFile;
}

wxString RuntimeInformationType::GetUserConfigurationFile() const {
	return userConfigurationFile;
}

wxString RuntimeInformationType::GetUserConfigurationFileWithPath() const {
	return GetUserConfigurationDirectory(true) + GetUserConfigurationFile();
}

UserConfigurationFileEntity *RuntimeInformationType::GetUserConfigurationFileEntity() {
	return &userConfigurationFileEntity;
}

bool RuntimeInformationType::Access(const unsigned int status, const wxString &message, wxWindow *parent) {
	const bool access = Access::Has(GetDatabase(), GetUserConfigurationFileEntity()->GetLastUsername(), status);
	if(!access && !message.IsEmpty()) {
		wxMessageDialog *msg = new wxMessageDialog(parent, message, _("Access denied"));
		msg->Center();
		msg->ShowModal();
		msg->Destroy();
	}
	return access;
}

void RuntimeInformationType::Block(const wxString &module, const wxString &number, void *data, const bool deleteOldData) {
	blockLibrary.Set(module, number, data, deleteOldData);
}

void RuntimeInformationType::Block(const wxString &module, const unsigned int number, void *data, const bool deleteOldData) {
	Block(module, DSCasts::ToString(number), data, deleteOldData);
}

void RuntimeInformationType::Unblock(const wxString &module, const wxString &number, const bool deleteOldData) {
	blockLibrary.Unset(module, number, deleteOldData);
}

void RuntimeInformationType::Unblock(const wxString &module, const unsigned int number, const bool deleteOldData) {
	Unblock(module, DSCasts::ToString(number), deleteOldData);
}

BlockType *RuntimeInformationType::IsBlocked(const wxString &module, const wxString &number) {
	return blockLibrary.Find(module, number);
}

BlockType *RuntimeInformationType::IsBlocked(const wxString &module, const unsigned int number) {
	return IsBlocked(module, DSCasts::ToString(number));
}

void RuntimeInformationType::SetObject(const wxString &name, void *object) {
	ObjectType *objectType = NULL;
	const unsigned int count = objects.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		objectType = objects.Get(i);
		if(objectType && objectType->GetName() == name)
			break;
		objectType = NULL;
	}

	if(!objectType) {
		objectType = new ObjectType(name, object);
		objects.Add(objectType);
	} else {
		objectType->SetObject(object);
	}
}

void *RuntimeInformationType::GetObject(const wxString &name) {
	ObjectType *objectType = NULL;
	const unsigned int count = objects.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		objectType = objects.Get(i);
		if(objectType && objectType->GetName() == name)
			return objectType->GetObject();
	}

	return NULL;
}

wxString RuntimeInformationType::GetExecutableFile() const {
	return executableFile;
}

wxString RuntimeInformationType::GetExecutablePath() const {
	return executablePath;
}

wxString RuntimeInformationType::GetTemporaryPath() const {
	//wxStandardPaths;
	wxString temporaryDir(wxStandardPaths::Get().GetTempDir());
	const wxString sep(GetSep());
	if(temporaryDir.Length() > 0 && temporaryDir.EndsWith(sep))
		return temporaryDir;
	return temporaryDir + sep;
}

void RuntimeInformationType::SetDemoVersion(const bool demoVersion) {
	this->demoVersion = demoVersion;
}

bool RuntimeInformationType::IsDemoVersion() const {
	return demoVersion;
}

void RuntimeInformationType::SetLocale(wxLocale *locale) {
	this->locale = locale;
}

wxLocale *RuntimeInformationType::GetLocale() {
	return locale;
}

wxString RuntimeInformationType::Translate(const wxString &name) {
	return GetLocale() ? wxString(GetLocale()->GetString(name)) : name;
}

void RuntimeInformationType::SetEnterpriseName(const wxString &enterpriseName) {
	this->enterpriseName = enterpriseName;
}

wxString RuntimeInformationType::GetEnterprisesName() const {
	return enterpriseName;
}
