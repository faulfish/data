; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
AppName=MultiDownload
AppVerName=MultiDownload 1.0
AppPublisher=Qisda Tool Team
DefaultDirName={pf}\Mobile Utility\MultiDownload
DefaultGroupName=Mobile Utility\MultiDownload
AllowNoIcons=true
OutputDir=C:\
OutputBaseFilename=MultiDownloadSetup
Compression=lzma/ultra
SolidCompression=true
InternalCompressLevel=ultra
VersionInfoVersion=1.0.0.0
VersionInfoCompany=Qisda
VersionInfoDescription=MultiDownload V1.0
VersionInfoCopyright=Qisda
AppCopyright=Qisda
AppID={{3DC476A6-E016-43B3-895F-CCD6F1133548}
PrivilegesRequired=admin

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked

[Files]
; NOTE: Don't use "Flags: ignoreversion" on any shared system files
; Localization, Configuration, Pictures
Source: Pictures2\*; DestDir: {app}\Pictures2\; Flags: recursesubdirs ignoreversion createallsubdirs; Tasks: ; Languages:
Source: Localization\*; DestDir: {app}\Localization\; Flags: recursesubdirs ignoreversion createallsubdirs; Tasks: ; Languages:
Source: ..\Release\Microsoft.VC80.CRT\*; DestDir: {app}\Microsoft.VC80.CRT\; Flags: recursesubdirs ignoreversion createallsubdirs; Tasks: ; Languages:
Source: ..\Release\Microsoft.VC80.MFC\*; DestDir: {app}\Microsoft.VC80.MFC\; Flags: recursesubdirs ignoreversion createallsubdirs; Tasks: ; Languages:
Source: ..\Release\Microsoft.VC80.MFCLOC\*; DestDir: {app}\Microsoft.VC80.MFCLOC\; Flags: recursesubdirs ignoreversion createallsubdirs; Tasks: ; Languages:

; Builded Qsyncer 2.0 files
Source:..\Release\*.exe; DestDir: {app}; Flags: recursesubdirs ignoreversion createallsubdirs
Source: ..\Release\*.dll; DestDir: {app}; Flags: recursesubdirs ignoreversion createallsubdirs
Source: ..\Release\*.csv; DestDir: {app}; Flags: recursesubdirs ignoreversion createallsubdirs
Source: .\*.ini; DestDir: {app}; Flags: recursesubdirs ignoreversion createallsubdirs
Source: .\*.dll; DestDir: {app}; Flags: recursesubdirs ignoreversion createallsubdirs
Source: .\*.csv; DestDir: {app}; Flags: recursesubdirs ignoreversion createallsubdirs

[Registry]

[Icons]
Name: {group}\MultiDownload; Filename: {app}\MultiDownload_ShareU.exe; WorkingDir: {app}
Name: {group}\{cm:UninstallProgram,MultiDownload}; Filename: {uninstallexe}
Name: {commondesktop}\MultiDownload; Filename: {app}\MultiDownload_ShareU.exe; Tasks: desktopicon; WorkingDir: {app}
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\MultiDownload; Filename: {app}\MultiDownload_ShareU.exe; Tasks: quicklaunchicon; WorkingDir: {app}

[Run]
Filename: {app}\MultiDownload_ShareU.exe; Description: {cm:LaunchProgram,MultiDownload}; Flags: nowait postinstall skipifsilent

[Code]

