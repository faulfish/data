[Setup]
InternalCompressLevel=ultra64
OutputDir=.\Output
OutputBaseFilename=MD-300HSetup
SolidCompression=true
VersionInfoVersion=1.0.0.0
Compression=lzma/ultra
AppName=MD-300H Setup
AppVerName=1.0.0.0
UsePreviousAppDir=false
UsePreviousGroup=false
ShowLanguageDialog=auto
AppID={{D7354854-BD77-48AF-B087-633530F8BF4E}
SourceDir=..\Release
DefaultGroupName=Mobile Utility\MD-300H Updrade\
DefaultDirName={pf}\Mobile Utility\MD-300H Updrade\

[Dirs]
Name: {app}\Microsoft.VC80.CRT
Name: {app}\Microsoft.VC80.MFC
Name: {app}\Microsoft.VC80.MFCLOC

[Files]
; MFC 8.0 Runtime
Source: .\Microsoft.VC80.CRT\Microsoft.VC80.CRT.manifest; DestDir: {app}\Microsoft.VC80.CRT\; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\Microsoft.VC80.CRT\msvcm80.dll; DestDir: {app}\Microsoft.VC80.CRT\; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\Microsoft.VC80.CRT\msvcp80.dll; DestDir: {app}\Microsoft.VC80.CRT\; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\Microsoft.VC80.CRT\msvcr80.dll; DestDir: {app}\Microsoft.VC80.CRT\; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\Microsoft.VC80.MFC\mfc80.dll; DestDir: {app}\Microsoft.VC80.MFC\; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\Microsoft.VC80.MFC\mfc80u.dll; DestDir: {app}\Microsoft.VC80.MFC\; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\Microsoft.VC80.MFC\mfcm80.dll; DestDir: {app}\Microsoft.VC80.MFC\; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\Microsoft.VC80.MFC\mfcm80u.dll; DestDir: {app}\Microsoft.VC80.MFC\; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\Microsoft.VC80.MFC\Microsoft.VC80.MFC.manifest; DestDir: {app}\Microsoft.VC80.MFC\; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\Microsoft.VC80.MFCLOC\MFC80CHS.dll; DestDir: {app}\Microsoft.VC80.MFCLOC\; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\Microsoft.VC80.MFCLOC\MFC80CHT.dll; DestDir: {app}\Microsoft.VC80.MFCLOC\; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\Microsoft.VC80.MFCLOC\MFC80DEU.dll; DestDir: {app}\Microsoft.VC80.MFCLOC\; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\Microsoft.VC80.MFCLOC\MFC80ENU.dll; DestDir: {app}\Microsoft.VC80.MFCLOC\; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\Microsoft.VC80.MFCLOC\MFC80ESP.dll; DestDir: {app}\Microsoft.VC80.MFCLOC\; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\Microsoft.VC80.MFCLOC\MFC80FRA.dll; DestDir: {app}\Microsoft.VC80.MFCLOC\; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\Microsoft.VC80.MFCLOC\MFC80ITA.dll; DestDir: {app}\Microsoft.VC80.MFCLOC\; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\Microsoft.VC80.MFCLOC\MFC80JPN.dll; DestDir: {app}\Microsoft.VC80.MFCLOC\; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\Microsoft.VC80.MFCLOC\MFC80KOR.dll; DestDir: {app}\Microsoft.VC80.MFCLOC\; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\Microsoft.VC80.MFCLOC\Microsoft.VC80.MFCLOC.manifest; DestDir: {app}\Microsoft.VC80.MFCLOC\; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion

; Main software load
Source: .\amss.mbn; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\amsshd.mbn; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\oemsbl.mbn; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\oemsblhd.mbn; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\partition.mbn; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\*.du; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\*qsnv; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\*.qvcefs; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\*.qzcs; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\NPRG6260.hex; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\BackupNV-Items.csv; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\Download.ini; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion

Source: .\ConfigureTool_ShareU.dll; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\Download_Export_ShareM.dll; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\gdiplus.dll; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\LocalizationTool_ShareU.dll; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\MobileFinder_ShareU.dll; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\MobileToolKit_ShareM.dll; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\ODMCoachModule_ShareM.dll; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\QisdaMiscellaneousModule_ShareM.dll; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\QisdaNVModule_ShareM.dll; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\QisdaQualcommDownloadModule.dll; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\QisdaSimLockModule.dll; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\QMSL_MSVC7R.dll; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\RequestManager_ShareM.dll; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\SimpleQDownload.exe; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion
Source: .\UIFramework_ShareU.dll; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly replacesameversion

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked

[Icons]
Name: {group}\MD-300H Upgrade; Filename: {app}\SimpleQDownload.exe; WorkingDir: {app}; IconIndex: 0
Name: {group}\{cm:UninstallProgram,MD-300H Upgrade}; Filename: {uninstallexe}
Name: {commondesktop}\MD-300H Upgrade; Filename: {app}\SimpleQDownload.exe; Tasks: desktopicon; WorkingDir: {app}
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\MD-300H Upgrade; Filename: {app}\SimpleQDownload.exe; Tasks: quicklaunchicon; WorkingDir: {app}

[Run]
Filename: {app}\SimpleQDownload.exe; WorkingDir: {app}; Description: Run upgrade tool right now?; Flags: nowait postinstall unchecked skipifsilent; Tasks: 
