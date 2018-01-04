Option Strict On
Option Explicit On

Imports System.Runtime.InteropServices

''' <summary>
''' This class provides access to an external C++ dll.
''' </summary>
''' <remarks>This class requires access to the QisdaService.dll file.</remarks>
''' Embert Tsai. For the purpose of DLL management, change QisdaService.dll to a more meaning name.
Public Class DllWrapper
    'Declare a delegate in the correct format to send to the C++ dll.
    Private Delegate Function DllCallBack(ByVal type As String, ByVal data As String) As String
    Private Declare Function DllSetCallBack Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "SetCallBack" (<MarshalAs(UnmanagedType.FunctionPtr)> ByVal func As DllCallBack) As Boolean

#Region "Functions Export -- Phone Commands"
    Private Declare Function DllAlert Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "Alert" (ByVal alertOn As Boolean, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllAntennaPosition Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "AntennaPosition" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllAudioLookback Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "AudioLoopback" (ByVal loopbackState() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllAudioMidi Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "AudioMidi" (ByVal playState() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllAudioTest Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "AudioTest" (ByVal audioState() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllCallDropLogger Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "CallDropLogger" (ByVal readType() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllCameraFlashLED Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "CameraFlashLED" (ByVal ledOn() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllCameraImage Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "CameraImage" (ByVal cameraState() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllCameraViewfinder Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "CameraViewfinder" (ByVal viewFinderOn() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllCliDisplay Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "CliDisplay" (ByVal onOrOff() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllCliDisplayBacklight Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "CliDisplayBacklight" (ByVal backLightOn() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllClock Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "Clock" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllDtmf Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "Dtmf" (ByVal tone() As Byte, ByVal volume() As Byte, ByVal audioState() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllFlip Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "Flip" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllFunLight Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "FunLight" (ByVal ledColor() As Byte, ByVal lefOn() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllHeadSet Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "HeadSet" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllHeadsetLoopback Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "HeadsetLoopback" (ByVal loopbackState() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllInternalCameraImage Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "InternalCameraImage" (ByVal cameraState() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllInternalCameraViewfinder Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "InternalCameraViewfinder" (ByVal viewFinderOn() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllINVMMasterClear Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "INVMMasterClear" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllKeypadBacklight Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "KeypadBacklight" (ByVal backLightOn() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllLedStatusLight Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "LedStatusLight" (ByVal ledColor() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllLightSensor Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "LightSensor" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllMainDisplay Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "MainDisplay" (ByVal onOrOff() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllMainDisplayBacklight Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "MainDisplayBacklight" (ByVal backLightOn() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllMemoryCardDetect Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "MemoryCardDetect" (ByVal suspend() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllMemoryCardRead Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "MemoryCardRead" (ByVal restart() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllOpProfLogger Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "OpProfLogger" (ByVal purge() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllPanicAccumulatorLogger Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "PanicAccumulatorLogger" (ByVal purge() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllPanicCodeReader Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "PanicCodeReader" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllPing Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "Ping" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllPowerOff Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "PowerOff" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllPressKey Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "PressKey" (ByVal keyCode() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllReadBand Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadBand" (ByVal outputString() As Byte) As Boolean
    'Private Declare Function DllReadBootLoaderVersion Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadBootLoaderVersion" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllReadChargingStatus Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadChargingStatus" (ByVal chargeType() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllReadESN Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadESN" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllReadFactoryBoardAssembly Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadFactoryBoardAssembly" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllReadFactoryByte Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadFactoryByte" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllReadFactoryLastStationDateTime Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadFactoryLastStationDateTime" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllReadFactoryTrackId Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadFactoryTrackId" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllReadFlexVersion Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadFlexVersion" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllReadGatewayIP Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadGatewayIP" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllReadKeypress Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadKeypress" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllReadLifetimeTimer Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadLifetimeTimer" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllReadLockCode Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadLockCode" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllReadMEID Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadMEID" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllReadMin Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadMin" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllReadModel Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadModel" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllReadMotoXPRSLogs Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadMotoXPRSLogs" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllReadPrlVersion Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadPrlVersion" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllReadProductionState Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadProductionState" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllReadSecurityCode Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadSecurityCode" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllReadSWVersion Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadSWVersion" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllReadWapIPAddress Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadWapIPAddress" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllRestart Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "Restart" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllRFProbeConnected Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "RFProbeConnected" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllVerifyBluetooth Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "VerifyBluetooth" (ByVal outputString() As Byte) As Boolean
    Private Declare Function DllVibrator Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "Vibrator" (ByVal vibratoeOn() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllWriteFactoryByte Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "WriteFactoryByte" (ByVal factoryByte() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllWriteLifetimeTimer Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "WriteLifetimeTimer" (ByVal livetime() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllWriteLockCode Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "WriteLockCode" (ByVal lockCode() As Byte, ByVal outputString() As Byte) As Boolean
    Private Declare Function DllWriteSecurityCode Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "WriteSecurityCode" (ByVal securityCode() As Byte, ByVal outputString() As Byte) As Boolean

    Private Declare Sub DllIMEIOTP Lib ".\Qisda\ServiceIMEIOTPTool.dll" Alias "ShowDlgServiceTool" ()
    Private Declare Sub DllSecurityTool Lib ".\Qisda\ServiceProgramTool.dll" Alias "ShowDlgServiceTool" ()
    Private Declare Sub DllQFuseTool Lib ".\Qisda\QFuseTool.dll" Alias "ShowDlgServiceTool" ()
    Private Declare Function DllBRDialog Lib ".\Qisda\BRDialog.dll" Alias "ShowDialog" () As Boolean
    Private Declare Function DllOpenBRDialog Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "OpenBackupRestoreDialog" (ByVal hWnd As Long) As Boolean
    Private Declare Function DllOpenMultidownload Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "OpenMultiDownload" (ByVal hWnd As Long) As Boolean
    Private Declare Function DllOpenRestoreBRTDialog Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "OpenRestoreBRTDialog" (ByVal hWnd As Long) As Boolean
    Private Declare Function DllAddFailure Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "AddFailure" (ByVal hWnd As Long) As Boolean
    Private Declare Function DllGSensorCalibration Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "GSensorCalibration" (ByVal hWnd As Long) As Boolean
    'Private Declare Function DllManualCITTest1 Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ManualCITTest" (ByVal outputStrng() As Byte) As Boolean
    Private Declare Function DllManualCITTest Lib ".\Qisda\ManualCITTestTool.dll" Alias "ManualCITTest" (ByVal outputStrng() As Byte) As Boolean
    Private Declare Function DllCalibration Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "Calibration" (ByVal band() As Byte, ByVal outputString() As Byte, Byval StartEnd As Byte) As Integer
    Private Declare Function DllCallTest Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "CallTest" (ByVal band As String, ByVal outputString() As Byte, ByVal bOutput As Boolean) As Integer
    Private Declare Function DllRFCalibrationM Lib ".\Qisda\RFCalibrationTool_StaticM.dll" Alias "RFCalibration" (ByVal CalItems() As Byte) As Boolean
    Private Declare Function DllRFTestM Lib ".\Qisda\RFCalibrationTool_StaticM.dll" Alias "RFTest" (ByVal TestItems() As Byte) As Boolean
    Private Declare Function DllRFCompletePhaseAndCallTestM Lib ".\Qisda\RFCalibrationTool_StaticM.dll" Alias "RFCompletePhaseAndCallTest" (ByVal TestItems() As Byte) As Boolean

#End Region


    Public Function ManualCITTest(ByRef retString() As Byte) As Boolean
        Dim retVal As Boolean = False
        retVal = DllManualCITTest(retString)
        Return retVal
    End Function

    Public Function BRDialog() As Boolean
        Dim process As System.Diagnostics.Process
        process = System.Diagnostics.Process.GetCurrentProcess()
        Return DllOpenBRDialog(process.MainWindowHandle.ToInt64())
    End Function

    Public Function Multidownload() As Boolean
        Dim process As System.Diagnostics.Process
        process = System.Diagnostics.Process.GetCurrentProcess()
        Return DllOpenMultidownload(process.MainWindowHandle.ToInt64())
    End Function

    Public Function RestoreBRTDialog() As Boolean
        Dim process As System.Diagnostics.Process
        process = System.Diagnostics.Process.GetCurrentProcess()
        Return DllOpenRestoreBRTDialog(process.MainWindowHandle.ToInt64())
    End Function

    Public Function AddFailure() As Boolean
        Dim process As System.Diagnostics.Process
        process = System.Diagnostics.Process.GetCurrentProcess()
        Return DllAddFailure(process.MainWindowHandle.ToInt64())
    End Function
    Public Function CalibrateGSensor() As Boolean
        Dim process As System.Diagnostics.Process
        process = System.Diagnostics.Process.GetCurrentProcess()
        Return DllGSensorCalibration(process.MainWindowHandle.ToInt64())
    End Function

    Public Sub DlgSecurityTool()
        DllSecurityTool()
    End Sub

    Public Sub DlgIMEIOTP()
        DllIMEIOTP()
    End Sub
    Public Sub DlgQFuseTool()
        DllQFuseTool()
    End Sub

#Region "Test Function for The MiddleWare"

    'Declare all of the needed functions from the C++ dll.
    'These functions can now be used directly in the code.
    'Private Declare Function DllShutdown Lib ".\Qisda\QisdaService.dll" Alias "Shutdown" () As Boolean
    'Private Declare Function DllVibrator Lib ".\Qisda\QisdaService.dll" Alias "Vibrator" (ByVal turnOn As Boolean) As Boolean

    Private Declare Function DllGetIMEI Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "GetIMEI" (ByVal szIMEI() As Byte) As Boolean

    Public Function ReadIMEI() As String

        Dim szRes As String

        Dim bIMEI(20) As Byte

        If (DllGetIMEI(bIMEI)) Then
            szRes = "Success,," & System.Text.Encoding.ASCII.GetString(bIMEI)
        Else
            szRes = "Failure,,"
        End If

        Return szRes

    End Function

    Private Declare Function DllGetMotoXPRSLogs Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "GetMotoXPRSLogs" (ByVal szLogPath() As Byte) As Boolean

    Public Function ReadMotoXPRSLogs() As String

        Dim szRes As String

        Dim szLogPath(512) As Byte

        If (DllGetMotoXPRSLogs(szLogPath)) Then
            szRes = "Success,," + System.Text.Encoding.ASCII.GetString(szLogPath)
        Else
            szRes = "Failure,,"
        End If

        Return szRes

    End Function


    Private Declare Function DllReadBootLoaderVersion Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "ReadBootLoaderVersion" () As Integer

    Public Function ReadBootLoaderVersion(ByVal szBootLoaderVersion As String, ByVal nLength As Integer) As String
        If (DllReadBootLoaderVersion() = 1) Then
            Return "Success,,"
        Else
            Return "Failure,,"
        End If
    End Function

#End Region

    'Private variable to hold the callback delegate sent from the test engine.
    Private _callBackDelegate As CommandModule.CallBackDelegate = Nothing

    'Private variable to hold the dll callback to be sent to the dll.
    Private _dllCallBack As DllCallBack = AddressOf CallBackWrapper

    ''' <summary>
    ''' Creates a new dll wrapper using the C++ dll.
    ''' </summary>
    ''' <remarks></remarks>
    Public Sub New()
        'Link the dll back to this class by sending the dll callback function.
        DllSetCallBack(_dllCallBack)
    End Sub

    ''' <summary>
    ''' Sets the callback function from the engine.
    ''' </summary>
    ''' <param name="callBack">Callback function to be used for output.</param>
    ''' <remarks>This callback function provides communication back to the engine.</remarks>
    Public Sub SetCallBack(ByVal callBack As CommandModule.CallBackDelegate)

        'Set the private variable to the callback sent from the engine.
        _callBackDelegate = callBack

    End Sub

    ''' <summary>
    ''' Shuts down the dll.
    ''' </summary>
    ''' <returns>The success/fail code returned from the dll.</returns>
    ''' <remarks></remarks>
    Public Function Shutdown() As Boolean

        'Shut down the dll.
        Return True
        'Return DllShutdown()

    End Function

    ''' <summary>
    ''' Executes the call test on the dll.
    ''' </summary>
    ''' <returns>The success/fail code returned from the dll.</returns>
    ''' <remarks>The dll calls OutputResult during this test, using the callback function.</remarks>
    Public Function CallTest() As Boolean

        'Execute the command.
        Return True
        'Return DllRfTest()

    End Function

    ''' <summary>
    ''' Reads the ESN from the dll.
    ''' </summary>
    ''' <returns>The ESN.</returns>
    ''' <remarks></remarks>
    Public Function ReadESN() As String

        'Execute the command.
        Return ""
        'Return DllGetESN()

    End Function

    ''' <summary>
    ''' Accepts callbacks from the C++ dll.
    ''' </summary>
    ''' <param name="type">The type of callback being performed.</param>
    ''' <param name="data">The data of the callback.</param>
    ''' <returns>The value returned from the engine callback delegate.</returns>
    ''' <remarks></remarks>
    Private Function CallBackWrapper(ByVal type As String, ByVal data As String) As String

        'Make sure the engine callback has been set using SetCallback.
        If Not _callBackDelegate Is Nothing Then
            'Call the engine callback and return the result to the dll.
            Return _callBackDelegate(type, data)
        Else
            'Throw an exception, the engine callback function has not been set yet.
            Throw New NotSupportedException("Callback function is not supported")
        End If

    End Function

    ''' <summary>
    ''' Executes the LCD test on the dll.
    ''' </summary>
    ''' <returns>The success/fail code returned from the dll.</returns>
    ''' <remarks>The dll calls OutputResult during this test, using the callback function.</remarks>
    Public Function LCDTest() As Boolean

        'Execute the command.
        Return True
        'Return DllLCDTest()

    End Function

    ''' <summary>
    ''' Executes unlock keypad command on the dll.
    ''' </summary>
    ''' <returns>The success/fail code returned from the dll.</returns>
    ''' <remarks>The dll calls OutputResult during this test, using the callback function.</remarks>
    Public Function UnlockKeyPad() As Boolean

        'Execute the command.
        Return True
        'Return DllUnlockKeyPad()

    End Function

    ''' <summary>
    ''' Executes clear key buffer command on the dll.
    ''' </summary>
    ''' <returns>The success/fail code returned from the dll.</returns>
    ''' <remarks>The dll calls OutputResult during this test, using the callback function.</remarks>
    Public Function CleanScreenKeyBuffer() As Boolean

        'Execute the command.
        Return True
        'Return DllCleanScreenKeyBuffer()

    End Function

    ''' <summary>
    ''' Executes  enter LCD Test on the dll.
    ''' </summary>
    ''' <returns>The success/fail code returned from the dll.</returns>
    ''' <remarks>The dll calls OutputResult during this test, using the callback function.</remarks>
    Public Function EnterLCDTest() As Boolean

        'Execute the command.
        Return True
        'Return DllEnterLCDTest()

    End Function

    ''' <summary>
    ''' Executes  TestLCDNextPattern on the dll.
    ''' </summary>
    ''' <returns>The success/fail code returned from the dll.</returns>
    ''' <remarks>The dll calls OutputResult during this test, using the callback function.</remarks>
    Public Function TestLCDNextPattern() As Boolean

        'Execute the command.
        Return True
        'Return DllTestLCDNextPattern()

    End Function

    ''' <summary>
    ''' Executes  ReturnToHomeScreen on the dll.
    ''' </summary>
    ''' <returns>The success/fail code returned from the dll.</returns>
    ''' <remarks>The dll calls OutputResult during this test, using the callback function.</remarks>
    Public Function ReturnToHomeScreen() As Boolean

        'Execute the command.
        Return True
        'Return DllReturnToHomeScreen()

    End Function

    ''' <summary>
    ''' Executes  EnterEM on the dll.
    ''' </summary>
    ''' <returns>The success/fail code returned from the dll.</returns>
    ''' <remarks>The dll calls OutputResult during this test, using the callback function.</remarks>
    Public Function EnterEM() As Boolean

        'Execute the command.
        Return True
        'Return DllEnterEM()

    End Function

    ''' <summary>
    ''' Executes  EnterVibratorTest on the dll.
    ''' </summary>
    ''' <returns>The success/fail code returned from the dll.</returns>
    ''' <remarks>The dll calls OutputResult during this test, using the callback function.</remarks>
    Public Function EnterVibratorTest() As Boolean

        'Execute the command.
        Return True
        'Return DllEnterVibratorTest()

    End Function

    ''' <summary>
    ''' Executes  BackToEM on the dll.
    ''' </summary>
    ''' <returns>The success/fail code returned from the dll.</returns>
    ''' <remarks>The dll calls OutputResult during this test, using the callback function.</remarks>
    Public Function BackToEM() As Boolean

        'Execute the command.
        Return True
        'Return DllBackToEM()

    End Function


    ''' <summary>
    ''' Executes  CompletePhase on the dll.
    ''' </summary>
    ''' <returns>The success/fail code returned from the dll.</returns>
    ''' <remarks>The dll calls OutputResult during this test, using the callback function.</remarks>
    Public Function CompletePhase() As Boolean

        'Execute the command.
        Return True
        'Return DllCompletePhase()

    End Function

    Public Function Calibration(ByVal parameters As String) As String
        Dim MobileModel(30) As Byte
        Dim strModel As String
        Dim bRes As Boolean = False
        Dim szRes As String

        bRes = DllReadModel(MobileModel)

        If (bRes = False) Then
            szRes = "Failure,,Read mobile model failed"
        Else
            strModel = System.Text.Encoding.ASCII.GetString(MobileModel)
            If (strModel.CompareTo("Athens") = 0) Then
                szRes = Calibration_Athens(parameters)
            ElseIf (strModel.CompareTo("Lavernock") = 0) Then
                szRes = Calibration_Lavernock(parameters)
            Else
                szRes = "Failure,,Mobile model not support"
            End If
        End If
        Return szRes
    End Function

    Public Function CallTest(ByVal parameters As String) As String
        Dim MobileModel(30) As Byte
        Dim strModel As String
        Dim bRes As Boolean = False
        Dim szRes As String

        bRes = DllReadModel(MobileModel)

        If (bRes = False) Then
            szRes = "Failure,,Read mobile model failed"
        Else
            strModel = System.Text.Encoding.ASCII.GetString(MobileModel)
            If (strModel.CompareTo("Athens") = 0) Then
                szRes = CallTest_Athens(parameters)
            ElseIf (strModel.CompareTo("Lavernock") = 0) Then
                szRes = CallTest_Lavernock(parameters)
            Else
                szRes = "Failure,,Mobile model not support"
            End If
        End If
        Return szRes
    End Function

    Public Function CompletePhaseAndCallTest(ByVal parameters As String) As String
        Dim MobileModel(30) As Byte
        Dim strModel As String
        Dim bRes As Boolean = False
        Dim szRes As String

        bRes = DllReadModel(MobileModel)

        If (bRes = False) Then
            szRes = "Failure,,Read mobile model failed"
        Else
            strModel = System.Text.Encoding.ASCII.GetString(MobileModel)
            If (strModel.CompareTo("Athens") = 0) Then
                szRes = CompletePhaseAndCallTest_Athens(parameters)
            ElseIf (strModel.CompareTo("Lavernock") = 0) Then
                szRes = CompletePhaseAndCallTest_Lavernock(parameters)
            Else
                szRes = "Failure,,Mobile model not support"
            End If
        End If
        Return szRes
    End Function

    ' 2008/06/10 Enzo
    Public Function Calibration_Athens(ByVal parameters As String) As String
        Dim message(512) As Byte
        Dim band(20) As Byte
        Dim CommadBand As String()
        Dim BandTemp As String()
        Dim charSeparatorsComma() As Char = {","c}
        Dim charSeparatorsAnd() As Char = {"|"c}
        Dim Bandcount As Integer
        Dim i As Integer
        Dim temp As String
        Dim szRes As String
        Dim flag As Boolean = False
        Dim flagcheckInstrument As Boolean = False
        Dim flagcheckPS As Boolean = False
        Dim comport As Integer = 0
        Dim tempflag(20) As Byte
        Dim retVal As Integer = 0

        BandTemp = parameters.Split(charSeparatorsComma, StringSplitOptions.None)
        CommadBand = BandTemp(1).Split(charSeparatorsAnd, StringSplitOptions.None)
        Bandcount = CommadBand.Length
        tempflag = System.Text.Encoding.Unicode.GetBytes(Chr(1))
        If (flagcheckInstrument = False) Then
            retVal = DllCalibration(tempflag, message, 0)
            flagcheckInstrument = True
        End If
        For i = (Bandcount - 1) To 0 Step -1
            'Select which Band to run and run it.
            Select Case CommadBand(i).ToLower
                Case "GSM900".ToLower
                    temp = Chr(10)
                Case "GSM1800".ToLower
                    temp = Chr(11)
                Case "GSM1900".ToLower
                    temp = Chr(12)
                Case "UMTSBand1".ToLower
                    temp = Chr(9)
                Case Else
                    szRes = "Failure,,Select Band Fail"
                    Return szRes
            End Select
            band = System.Text.Encoding.Unicode.GetBytes(temp)

            If (i <> 0) Then
                retVal = DllCalibration(band, message, 1)
            Else
                retVal = DllCalibration(band, message, 255)
            End If

            If (retVal <> 1) Then
                szRes = "Failure," + System.Text.Encoding.ASCII.GetString(message) + ","
                Return szRes
            End If
        Next i
        szRes = "Success,,Finish to Test"
        Return szRes
    End Function
    ' 2008/06/27 Magan
    Public Function CallTest_Athens(ByVal parameters As String) As String
        Dim message(512) As Byte
        Dim BandTemp As String()
        Dim charSeparatorsComma() As Char = {","c}
        Dim szRes As String

        BandTemp = parameters.Split(charSeparatorsComma, StringSplitOptions.None)

        Dim retVal As Integer = 0
        retVal = DllCallTest(BandTemp(1), message, False)

        If (retVal <> 1) Then
            szRes = "Failure," + System.Text.Encoding.ASCII.GetString(message) + ","
            Return szRes
        End If
        szRes = "Success,,Finish to Test"
        Return szRes
    End Function
    '2008/08/28 Magan
    Public Function CompletePhaseAndCallTest_Athens(ByVal parameters As String) As String
        Dim message(512) As Byte
        Dim band(20) As Byte
        Dim CommadBand As String()
        Dim BandTemp As String()
        Dim charSeparatorsComma() As Char = {","c}
        Dim charSeparatorsAnd() As Char = {"|"c}
        Dim Bandcount As Integer
        Dim i As Integer
        Dim temp As String
        Dim szRes As String
        Dim flag As Boolean = False
        Dim flagcheckInstrument As Boolean = False
        Dim flagcheckPS As Boolean = False
        Dim comport As Integer = 0
        Dim tempflag(20) As Byte
        Dim retVal As Integer = 0

        BandTemp = parameters.Split(charSeparatorsComma, StringSplitOptions.None)
        CommadBand = BandTemp(1).Split(charSeparatorsAnd, StringSplitOptions.None)
        Bandcount = CommadBand.Length
        tempflag = System.Text.Encoding.Unicode.GetBytes(Chr(1))
        If (flagcheckInstrument = False) Then
            retVal = DllCalibration(tempflag, message, 0)
            flagcheckInstrument = True
        End If
        If (retVal <> 1) Then
            szRes = "Failure," + System.Text.Encoding.ASCII.GetString(message) + ","
            Return szRes
        End If
        For i = (Bandcount - 1) To 0 Step -1
            'Select which Band to run and run it.
            Select Case CommadBand(i).ToLower
                Case "GSM900".ToLower
                    temp = Chr(10)
                Case "GSM1800".ToLower
                    temp = Chr(11)
                Case "GSM1900".ToLower
                    temp = Chr(12)
                Case "UMTSBand1".ToLower
                    temp = Chr(9)
                Case Else
                    szRes = "Failure,,Select Band Fail"
                    Return szRes
            End Select
            band = System.Text.Encoding.Unicode.GetBytes(temp)

            retVal = DllCalibration(band, message, 1)

            If (retVal <> 1) Then
                szRes = "Failure," + System.Text.Encoding.ASCII.GetString(message) + ","
                Return szRes
            End If
        Next i

        '        BandTemp = parameters.Split(charSeparatorsComma, StringSplitOptions.None)
        retVal = DllCallTest(BandTemp(1), message, True)

        If (retVal <> 1) Then
            szRes = "Failure," + System.Text.Encoding.ASCII.GetString(message) + ","
            Return szRes
        End If
        szRes = "Success,,Finish to Test"
        Return szRes
    End Function

    ' 2009/03/17 Jae Liao
    Public Function Calibration_Lavernock(ByVal parameters As String) As String
        Dim BandTemp As String()
        Dim charSeparatorsComma() As Char = {","c}
        Dim szRes As String
        Dim retVal As Boolean
        Dim band(100) As Byte

        BandTemp = parameters.Split(charSeparatorsComma, StringSplitOptions.None)
        band = System.Text.Encoding.ASCII.GetBytes(BandTemp(1))
        retVal = DllRFCalibrationM(band)

        If (retVal <> True) Then
            szRes = "Failure,,"
        Else
            szRes = "Success,,Finish to Test"
        End If
        Return szRes
    End Function

    ' 2009/03/17 Jae Liao
    Public Function CallTest_Lavernock(ByVal parameters As String) As String
        Dim BandTemp As String()
        Dim charSeparatorsComma() As Char = {","c}
        Dim szRes As String
        Dim retVal As Boolean
        Dim band(100) As Byte

        BandTemp = parameters.Split(charSeparatorsComma, StringSplitOptions.None)
        band = System.Text.Encoding.ASCII.GetBytes(BandTemp(1))
        retVal = DllRFTestM(band)

        If (retVal <> True) Then
            szRes = "Failure,,"
        Else
            szRes = "Success,,Finish to Test"
        End If
        Return szRes
    End Function

    Public Function CompletePhaseAndCallTest_Lavernock(ByVal parameters As String) As String
        Dim BandTemp As String()
        Dim charSeparatorsComma() As Char = {","c}
        Dim szRes As String
        Dim retVal As Boolean
        Dim band(100) As Byte

        BandTemp = parameters.Split(charSeparatorsComma, StringSplitOptions.None)
        band = System.Text.Encoding.ASCII.GetBytes(BandTemp(1))
        retVal = DllRFCompletePhaseAndCallTestM(band)

        If (retVal <> True) Then
            szRes = "Failure,,"
        Else
            szRes = "Success,,Finish to Test"
        End If
        Return szRes
    End Function

    ''' <summary>
    ''' Executes  CompletePhase on the dll.
    ''' </summary>
    ''' <returns>The success/fail code returned from the dll.</returns>
    ''' <remarks>The dll calls OutputResult during this test, using the callback function.</remarks>
    Public Function DummyDlg() As Integer

        'Execute the command.
        Return 1
        'Return DllDummyDlg()

    End Function
#If DEBUG Then

#Region "Test Function for EFS"

    Private Declare Function DllReadFile Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "EFS_ReadFile" (ByVal sFileSrc As String, ByVal sFileDst As String) As Boolean

    Public Function ReadFile(ByVal sFileSrc As String, ByVal sFileDst As String) As String
        If (DllReadFile(sFileSrc, sFileDst)) Then
            Return "Success,Read File,"
        Else
            Return "Failure,Read File,"
        End If
    End Function

    Private Declare Function DllWriteFile Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "EFS_WriteFile" (ByVal sFileSrc As String, ByVal sFileDst As String) As Boolean

    Public Function WriteFile(ByVal sFileSrc As String, ByVal sFileDst As String) As String
        If (DllWriteFile(sFileSrc, sFileDst)) Then
            Return "Success,Write File,"
        Else
            Return "Failure,Write File,"
        End If
    End Function

    Private Declare Function DllRename Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "EFS_Rename" (ByVal sOldFile As String, ByVal sNewFile As String) As Boolean

    Public Function Rename(ByVal sOldFile As String, ByVal sNewFile As String) As String
        If (DllRename(sOldFile, sNewFile)) Then
            Return "Success,Rename,"
        Else
            Return "Failure,Rename,"
        End If
    End Function

    Private Declare Function DllDeleteFile Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "EFS_DeleteFolder" (ByVal sFile As String) As Boolean

    Public Function DeleteFile(ByVal sFile As String) As String
        If (DllDeleteFile(sFile)) Then
            Return "Success,Delete File,"
        Else
            Return "Failure,Delete File,"
        End If
    End Function


    Private Declare Function DllMakeFolder Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "EFS_NewFolder" (ByVal sFolder As String) As Boolean

    Public Function NewFolder(ByVal sFolder As String) As String
        If (DllMakeFolder(sFolder)) Then
            Return "Success,New Folder,"
        Else
            Return "Failure,New Folder,"
        End If
    End Function

    Private Declare Function DllRemoveFolder Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "EFS_RemoveFolder" (ByVal sFolder As String) As Boolean

    Public Function RemoveFolder(ByVal sFolder As String) As String
        If (DllRemoveFolder(sFolder)) Then
            Return "Success,Remove Folder,"
        Else
            Return "Failure,Remove Folder,"
        End If
    End Function

    Private Declare Function DllCreateLink Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "EFS_CreateLink" (ByVal sFile As String, ByVal sPointedFile As String) As Boolean

    Public Function CreateLink(ByVal sFile As String, ByVal sPointedFile As String) As String
        If (DllCreateLink(sFile, sPointedFile)) Then
            Return "Success,Create Link,"
        Else
            Return "Failure,Create Link,"
        End If
    End Function

    Private Declare Function DllReadLink Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "EFS_ReadLink" (ByVal sFile As String, ByVal sPointedFile As String) As Boolean

    Public Function ReadLink(ByVal sFile As String, ByVal sPointedFile As String) As String
        If (DllReadLink(sFile, sPointedFile)) Then
            Return "Success,Read Link,"
        Else
            Return "Failure,Read Link,"
        End If
    End Function

#End Region
#Region "Test Function for Audio"

    Private Declare Function DllSetAudioPath Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "Audio_SetPath" (ByVal iAudioDevice As Short) As Boolean

    Public Function AudioSetPath(ByVal iAudioDevice As Short) As String
        If (DllSetAudioPath(iAudioDevice)) Then
            Return "Success,Set Path,"
        Else
            Return "Failure,Set Path,"
        End If
    End Function

#End Region

#Region "Test Function for Video"

    Private Declare Function DllPowerOn Lib ".\Qisda\MiddleWare_StaticM.dll" Alias "Video_PowerOn" (ByVal iStatus As Short) As Boolean

    Public Function VideoPowerOn(ByVal iStatus As Short) As String
        If (DllPowerOn(iStatus)) Then
            Return "Success,Read Link,"
        Else
            Return "Failure,Read Link,"
        End If
    End Function

#End Region
#End If
End Class

