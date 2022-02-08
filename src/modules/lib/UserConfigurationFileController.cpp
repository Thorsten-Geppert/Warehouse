#include "UserConfigurationFileController.h"
#include <wx/file.h>
#include <wx/fileconf.h>
#include <wx/wfstream.h>
#include <DSLib.h>

using namespace DynSoft;

bool UserConfigurationFileController::CreateFile(const wxString &filename) {
	if(wxFile::Exists(filename))
		return true;
	
	wxFile file(filename, wxFile::write);
	if(!file.IsOpened())
		return false;
	
	file.Close();

	return true;
}

bool UserConfigurationFileController::Load(
	UserConfigurationFileEntity *configuration,
	const wxString &file
) {
	if(!CreateFile(file))
		return false;

	wxFileInputStream input(file);
	if(!input.IsOk())
		return false;

	wxFileConfig fileConfig(input);

	configuration->GetMainWindowPreferences()->SetPosition(
		(int) fileConfig.Read(_N("mainWindowX"), 20),
		(int) fileConfig.Read(_N("mainWindowY"), 20)
	);
	configuration->GetMainWindowPreferences()->SetSize(
		(int) fileConfig.Read(_N("mainWindowWidth"), 1200),
		(int) fileConfig.Read(_N("mainWindowHeight"), 800)
	);

	configuration->SetLastUsername(fileConfig.Read(_N("lastUsername"), wxEmptyString));
	configuration->SetLastEnterprise(fileConfig.Read(_N("lastEnterprise"), wxEmptyString));
	configuration->SetLicenceNumber((const unsigned int) fileConfig.Read(_N("licenceNumber"), 0L));

	configuration->GetDatabase()->SetHostname(fileConfig.Read(_N("hostname"), wxEmptyString));
	configuration->GetDatabase()->SetDatabase(fileConfig.Read(_N("database"), wxEmptyString));
	configuration->GetDatabase()->SetUsername(fileConfig.Read(_N("username"), wxEmptyString));
	configuration->GetDatabase()->SetPassword(fileConfig.Read(_N("password"), wxEmptyString));
	configuration->GetDatabase()->SetPort(fileConfig.Read(_N("port"), wxEmptyString));

	return true;
}

bool UserConfigurationFileController::Save(
	UserConfigurationFileEntity *configuration,
	const wxString &file,
	const int kind
) {
	if(!CreateFile(file))
		return false;

	wxFileOutputStream output(file);
	if(!output.IsOk())
		return false;

	wxFileConfig fileConfig;
	
	if(kind == GUI || kind == ALL) {
		fileConfig.Write(_N("lastUsername"), configuration->GetLastUsername());
		fileConfig.Write(_N("lastEnterprise"), configuration->GetLastEnterprise());
		fileConfig.Write(_N("mainWindowX"), (long) configuration->GetMainWindowPreferences()->GetPosition().x);
		fileConfig.Write(_N("mainWindowY"), (long) configuration->GetMainWindowPreferences()->GetPosition().y);
		fileConfig.Write(_N("mainWindowWidth"), (long) configuration->GetMainWindowPreferences()->GetSize().GetWidth());
		fileConfig.Write(_N("mainWindowHeight"), (long) configuration->GetMainWindowPreferences()->GetSize().GetHeight());
	}

	if(kind == DB || kind == ALL) {
		fileConfig.Write(_N("hostname"), configuration->GetDatabase()->GetHostname());
		fileConfig.Write(_N("database"), configuration->GetDatabase()->GetDatabase());
		fileConfig.Write(_N("username"), configuration->GetDatabase()->GetUsername());
		fileConfig.Write(_N("password"), configuration->GetDatabase()->GetPassword());
		fileConfig.Write(_N("port"), configuration->GetDatabase()->GetPort());
	}

	if(kind == SN  || kind == ALL)
		fileConfig.Write(_N("licenceNumber"), (long) configuration->GetLicenceNumber());

	return fileConfig.Save(output);
}
