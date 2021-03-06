; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
AppName=ImagerAnalysisTool
AppVerName=ImagerAnalysisTool 1.0
AppPublisher=Qisda Tool Team
DefaultDirName={pf}\Mobile Utility\ImagerAnalysisTool
DefaultGroupName=Mobile Utility\ImagerAnalysisTool
AllowNoIcons=true
OutputDir=C:\
OutputBaseFilename=ImagerAnalysisToolSetup
Compression=lzma/ultra
SolidCompression=true
InternalCompressLevel=ultra
VersionInfoVersion=1.0.0.0
VersionInfoCompany=Qisda
VersionInfoDescription=ImagerAnalysisTool V1.0
VersionInfoCopyright=Qisda
AppCopyright=Qisda
AppID={{46F2C81A-418C-4b7e-96BF-5A809C3EC7DB}
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
Source: Microsoft.VC80.CRT\*; DestDir: {app}\Microsoft.VC80.CRT\; Flags: recursesubdirs ignoreversion createallsubdirs; Tasks: ; Languages:
Source: Microsoft.VC80.MFC\*; DestDir: {app}\Microsoft.VC80.MFC\; Flags: recursesubdirs ignoreversion createallsubdirs; Tasks: ; Languages:
Source: Microsoft.VC80.MFCLOC\*; DestDir: {app}\Microsoft.VC80.MFCLOC\; Flags: recursesubdirs ignoreversion createallsubdirs; Tasks: ; Languages:

; Builded Qsyncer 2.0 files
Source: .\*.exe; DestDir: {app}; Flags: recursesubdirs ignoreversion createallsubdirs
Source: .\*.dll; DestDir: {app}; Flags: recursesubdirs ignoreversion createallsubdirs

[Registry]

[Icons]
Name: {group}\ImagerAnalysisTool; Filename: {app}\ImagerAnalysisTool.exe; WorkingDir: {app}
Name: {group}\{cm:UninstallProgram,ImagerAnalysisTool}; Filename: {uninstallexe}
Name: {commondesktop}\ImagerAnalysisTool; Filename: {app}\ImagerAnalysisTool.exe; Tasks: desktopicon; WorkingDir: {app}
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\ImagerAnalysisTool; Filename: {app}\ImagerAnalysisTool.exe; Tasks: quicklaunchicon; WorkingDir: {app}

[Run]
Filename: {app}\ImagerAnalysisTool.exe; Description: {cm:LaunchProgram,ImagerAnalysisTool}; Flags: nowait postinstall skipifsilent

[Code]

