; DynSoft.com Warehouse Installer
; DynSoft.com - Thorsten Geppert - 08.2011 - last: 04.11.2021

[Setup]
AppName=DynSoft.com Warehouse
AppVersion=4.0.0
AppPublisher=DynSoft.com - Thorsten Geppert
AppPublisherURL=https://www.dynsoft.com
AppSupportURL=https://www.dynsoft.com
AppUpdatesURL=https://www.dynsoft.com
DefaultDirName={pf}\DynSoft.com Warehouse
DefaultGroupName=DynSoft.com Warehouse
UninstallDisplayIcon={app}\Warehouse.exe
WizardImageFile=InstallerLogo.bmp
OutputBaseFilename=DynSoftWarehouse
OutputDir=.\
  
[Languages]
Name: de; MessagesFile: "compiler:Languages\German.isl"

[Messages]
de.BeveledLabel=Deutsch

[Types]
Name: "full"; Description: "Vollständige Installation"
Name: "custom"; Description: "Individuelle Installation"; Flags: iscustom
Name: "database"; Description: "Datenbank installieren"
Name: "warehouse"; Description: "GuG-IT Warehouse installieren"
Name: "admin"; Description: "GuG-IT Warehouse Admin installieren"
Name: "licence"; Description: "GuG-IT LicenceGenerator installieren"

[Components]
Name: postgresql; Description: PostgreSQL 14 Datenbank; Types: full database;
Name: warehouse; Description: DynSoft.com Warehouse; Types: full warehouse;
Name: admin; Description: DynSoft.com Warehouse Admin; Types: full admin;
Name: licence; Description: DynSoft.com LicenceGenerator; Types: full licence;

[Files]
Source: "postgresql-14.0-1-windows-x64.exe"; DestDir: "{tmp}"; AfterInstall: InstallPostgreSQL(); Components: postgresql;

Source: "../../../src/modules/warehouse/Warehouse.exe"; DestDir: "{app}"; Components: warehouse;
Source: "../../../src/modules/warehouse/resources/*.*"; DestDir: {app}\resources; Flags: ignoreversion recursesubdirs; Components: warehouse;
Source: "../../../src/modules/reports/target/reports-0.0.1.jar"; DestDir: {app}\reports\; DestName: reports.jar; Flags: ignoreversion recursesubdirs; Components: warehouse;
Source: "../../../src/modules/reports/target/lib/*.*"; DestDir: {app}\reports\lib\; Flags: ignoreversion recursesubdirs; Components: warehouse;
Source: "../../../src/modules/reports/reports/*.*"; DestDir: {app}\reports\reports\; Flags: ignoreversion recursesubdirs; Components: warehouse;

Source: "libcrypto-1_1-x64.dll"; DestDir: "{app}"; Components: warehouse admin licence;
Source: "libgcc_s_seh-1.dll"; DestDir: "{app}"; Components: warehouse admin licence;
Source: "libiconv-2.dll"; DestDir: "{app}"; Components: warehouse admin licence;
Source: "libintl-9.dll"; DestDir: "{app}"; Components: warehouse admin licence;
Source: "libpq.dll"; DestDir: "{app}"; Components: warehouse admin licence;
Source: "libssl-1_1-x64.dll"; DestDir: "{app}"; Components: warehouse admin licence;
Source: "libstdc++-6.dll"; DestDir: "{app}"; Components: warehouse admin licence;
Source: "libwinpthread-1.dll"; DestDir: "{app}"; Components: warehouse admin licence;
Source: "wxmsw312u_gcc_custom.dll"; DestDir: "{app}"; Components: warehouse admin licence;
Source: "wxmsw312u_gl_gcc_custom.dll"; DestDir: "{app}"; Components: warehouse admin licence;
Source: "zlib1.dll"; DestDir: "{app}"; Components: warehouse admin licence;
   
Source: "../../../../DSLicenceGenerator/src/modules/licencegenerator/LicenceGenerator.exe"; DestDir: "{app}"; Components: licence;
Source: "../../../../DSLicenceGenerator/src/modules/licencegenerator/LicenceGeneratorIcon.ico"; DestDir: "{app}"; Components: licence;

Source: "../../../src/modules/admin/WarehouseAdmin.exe"; DestDir: "{app}"; Components: admin;
Source: "../../../src/modules/admin/resources/postgresql/windows/*.*"; DestDir: {app}\resources\postgresql\windows\; Flags: ignoreversion recursesubdirs; Components: admin;
Source: "../../../src/modules/admin/resources/languages/*.*"; DestDir: {app}\resources\languages\; Flags: ignoreversion recursesubdirs; Components: admin;
Source: "../../../src/modules/admin/AdminIcon.ico"; DestDir: "{app}"; Components: admin;

[CustomMessages]
LaunchWarehouse=Warehouse starten
LaunchLicenceGenerator=LicenceGenerator starten
LaunchWarehouseAdmin=Warehouse Admin starten

[Run]
Filename: "{app}\Warehouse.exe"; Description: {cm:LaunchWarehouse,Warehouse}; Flags: nowait postinstall skipifsilent; Components: warehouse;
Filename: "{app}\LicenceGenerator.exe"; Description: {cm:LaunchLicenceGenerator,LicenceGenerator}; Flags: nowait postinstall skipifsilent; Components: licence;

[Tasks]
Name: "desktopicon"; Description: "&Desktop-Verknüpfungen erstellen"; GroupDescription: "Zusätzliche Verknüpfungen:"; Flags: unchecked; Components: warehouse admin licence;
Name: "quicklaunchicon"; Description: "&Schnellstart-Verknüpfung erstellen"; GroupDescription: "Zusätzliche Verknüpfungen:"; Flags: unchecked; Components: warehouse admin licence;

[Icons]
Name: "{group}\Warehouse"; Filename: "{app}\Warehouse.exe"; Components: warehouse;
Name: "{group}\Warehouse Admin"; Filename: "{app}\WarehouseAdmin.exe"; Components: admin;
Name: "{group}\LicenceGenerator"; Filename: "{app}\LicenceGenerator.exe"; Components: licence;
Name: "{group}\{cm:UninstallProgram,Warehouse}"; Filename: "{uninstallexe}"; Components: warehouse;

Name: "{commondesktop}\Warehouse"; Filename: "{app}\Warehouse.exe"; Tasks: desktopicon; Components: warehouse;
Name: "{commondesktop}\Warehouse Admin"; Filename: "{app}\WarehouseAdmin.exe"; Tasks: desktopicon; Components: admin;
Name: "{commondesktop}\LicenceGenerator"; Filename: "{app}\LicenceGenerator.exe"; Tasks: desktopicon; Components: licence;

Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Warehouse"; Filename: "{app}\Warehouse.exe"; Tasks: quicklaunchicon; Components: warehouse;
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\WarehouseAdmin"; Filename: "{app}\WarehouseAdmin.exe"; Tasks: quicklaunchicon; Components: admin;
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\LicenceGenerator"; Filename: "{app}\LicenceGenerator.exe"; Tasks: quicklaunchicon; Components: licence;

[Code]
procedure InstallPostgreSQL();
var
  AppPath: String;
  WorkingDir: String;
  ReturnCode: Integer;
begin
    WizardForm.StatusLabel.Caption := 'PostgreSQL-Datenbank wird installiert, dies kann einige Minuten in Anspruch nehmen...';
    WorkingDir := ExpandConstant('{tmp}');
    AppPath := expandconstant('{tmp}\postgresql-14.0-1-windows-x64.exe')
    Exec(AppPath, '--mode unattended --unattendedmodeui none --servicepassword postgres --superpassword postgres --locale de', WorkingDir, SW_SHOW, ewWaitUntilTerminated, ReturnCode);
end;

