Option Strict On
Option Explicit On

Imports System
Imports System.IO

'General notes about this project:
'DO NOT reference any of the STE framework dlls (such as ICommandModule) from
'this project.  Linking to any STE code may result in dll mismatch errors.


''' <summary>
''' ODM Command Module Template
''' </summary>
''' <remarks>This is a skeleton command module, which implements all of the 
''' Phase 1 command module functions.  It can be used as a template to build on 
''' for the actual ODM command modules.  This skeleton command module can be 
''' tested in the ODMTester application.</remarks>
Public Class CommandModule

    'Private variable to hold the callback function.
    Private _callBack As [Delegate] = Nothing

    'Private variable to hold the dll wrapper object.
    Private _dllWrapper As New DllWrapper

    'Private variables to hold the current model/technology to use.
    Private _model As String
    Private _technology As String

#Region "Public CommandModule Interface"

    ''' <summary>
    ''' Delegate for the Engine API standard callback function.
    ''' </summary>
    ''' <param name="type">Type of callback being used.</param>
    ''' <param name="data">Callback data, specific to the type of callback.</param>
    ''' <returns>The callback result, if any.</returns>
    ''' <remarks>See the Engine API document for details on the valid types of callbacks.</remarks>
    Public Delegate Function CallBackDelegate(ByVal type As String, ByVal data As String) As String

    ''' <summary>
    ''' Default constructor for the command module.
    ''' </summary>
    ''' <remarks>This constructor is called to load the command module.  It must be available
    ''' (without any required arguments) for all command modules.</remarks>
    Public Sub New()
        'Do any initialization here.
        'Note that this constructor cannot take any parameters.
    End Sub


    ''' <summary>
    ''' Sends a phone command to the phone.
    ''' </summary>
    ''' <param name="parameters">Parameters for the phone command being sent.</param>
    ''' <returns>The result of the phone command.</returns>
    ''' <remarks>See the Engine API document for details on the standard phone commands.</remarks>
    Public Function PhoneCommand(ByVal parameters As String) As String

        'Format of the parameters string:
        '<CommandName>,<Parameter1>,<Parameter2>...
        'Everything after <CommandName> is optional.

        'Create a variable to hold the command name.
        Dim command As String = parameters

        'If the parameters string contains a comma...
        If parameters.Contains(",") Then
            'Remove everything after the comma.
            command = command.Substring(0, parameters.IndexOf(","c))
        End If

        'Create a variable to hold the return from the command.
        Dim returnValue As String = String.Empty

        'Catch any exceptions thrown while executing the command.
        Try
            'Select which command to run and run it.
            Select Case command.ToLower
                Case "ReadIMEI".ToLower
                    Return _dllWrapper.ReadIMEI()
                Case "ReadESN".ToLower
                    returnValue = ReadESN()
                Case "ReadMotoXPRSLogs".ToLower
                    Return _dllWrapper.ReadMotoXPRSLogs()
                Case "ManualCITTest".ToLower
                    Return DoManualCITTestUnderDllWrapper()
                Case "Calibration".ToLower
                    Dim retVal As String
                    Dim compvalue As Integer

                    retVal = _dllWrapper.Calibration(parameters)
                    Dim state As String = retVal
                    state = state.Substring(0, retVal.IndexOf(","c))
                    compvalue = String.Compare(state, "Success")
                    If compvalue = 0 Then
                        Return "Success, ,"
                    Else
                        Return retVal
                    End If
                Case "CallTest".ToLower
                    Dim retVal As String
                    Dim compvalue As Integer

                    retVal = _dllWrapper.CallTest(parameters)
                    Dim state As String = retVal
                    state = state.Substring(0, retVal.IndexOf(","c))
                    compvalue = String.Compare(state, "Success")
                    If compvalue = 0 Then
                        Return "Success, ,"
                    Else
                        Return retVal
                    End If
                Case "CompletePhaseAndCallTest".ToLower
                    Dim retVal As String
                    Dim compValue As Integer

                    retVal = _dllWrapper.CompletePhaseAndCallTest(parameters)
                    Dim state As String = retVal
                    state = state.Substring(0, retVal.IndexOf(","c))
                    compValue = String.Compare(state, "Success")
                    If compValue = 0 Then
                        Return "Success, ,"
                    Else
                        Return retVal
                    End If
                Case "LCDTest".ToLower
                    _dllWrapper.LCDTest()
                Case "CompletePhase".ToLower
                    _dllWrapper.CompletePhase()
                Case "ReadBootLoaderVersion".ToLower
                    Dim szVersion As String = ""
                    Dim nLength As Integer = 0
                    returnValue = _dllWrapper.ReadBootLoaderVersion(szVersion, nLength)
                Case Else
                    'Return a failure if the command was not recognized.
                    Return "Failure,Command not recognized: " & parameters & ","
            End Select

        Catch ex As Exception

            'Write the exception information to the debug window.
            Debug.WriteLine(ex.ToString)

            'Remove any commas from the exception message.
            Dim messageWithoutCommas As String = ex.Message.Replace(","c, " ")

            'Return the exception message as the command status message, with Failure as the status.
            Return "Failure,Exception occured during testing: " & messageWithoutCommas & ","

        End Try

        'Return the Success as the status, and the return from the command.
        Return "Success,," & returnValue

    End Function

    Private Function DoManualCITTestUnderDllWrapper() As String
        Dim retString(1024) As Byte
        Dim retVal As Boolean = _dllWrapper.ManualCITTest(retString)
        If retVal = True Then
            Return "Success,," & System.Text.Encoding.ASCII.GetString(retString)
        Else
            Return "Failure,,ManualCITTest"
        End If
    End Function
    ''' <summary>
    ''' Returns a list of capabilities for this command module.
    ''' </summary>
    ''' <param name="configFile">The path to the config.xml file.</param>
    ''' <returns>A list of capabilities for this command module.</returns>
    ''' <remarks>See the Engine API document for details on the capability format.
    '''   The config.xml file is not needed for Phase 1.</remarks>
    Public Function ReportCapabilities(ByVal configFile As String) As String

        'Make a new capability list object to help generate the list.
        Dim capabilities As New CapabilityList

        'Capabilities for the Wellfleet (GSM/WCDMA) phone:

        'ReadIMEI
        Dim strCurrentPath As String = System.IO.Directory.GetCurrentDirectory()
        Dim strFilePath As String = strCurrentPath & "\Qisda\Capability.csv"
        Dim fileReader As StreamReader = File.OpenText(strFilePath)

        Dim line As String = fileReader.ReadLine()
        Do While line <> Nothing
            Dim token() As String = line.Split(","c)
            Dim para5 As Boolean = False
            Dim para6 As Boolean = False
            Dim para7 As Boolean = False
            If token(5).Equals("True") Then
                para5 = True
            End If
            If token(6).Equals("True") Then
                para6 = True
            End If
            If token(7).Equals("True") Then
                para7 = True
            End If
            capabilities.AddCapability(token(0), token(1), token(2), token(3), token(4), para5, para6, para7)
            line = fileReader.ReadLine()
        Loop

        'Return the capabilities in a string.
        Return capabilities.ToString

    End Function

    ''' <summary>
    ''' Sets the callback to be used by the command module.
    ''' </summary>
    ''' <param name="callback">The callback function to be called from the command module.</param>
    ''' <remarks>See the Engine API document for details on the callbacks.</remarks>
    Public Sub SetCallback(ByVal callback As [Delegate])

        'Set the callback private variable to the callback parameter.
        _callBack = callback

        'Set the dll callback function to the callback parameter.
        _dllWrapper.SetCallBack(AddressOf DoCallBack)

    End Sub

    ''' <summary>
    ''' Sets the current model and technology to be used by the command module.
    ''' </summary>
    ''' <param name="model">The model of the phone (as specified in the capability list).</param>
    ''' <param name="technology">The technology of the phone (as specified in the capability list).</param>
    ''' <remarks>This model and technology should be used for any phone commands sent to the phone.</remarks>
    Public Sub SetModel(ByVal model As String, ByVal technology As String)

        'Set the model and technology private variables.
        _model = model
        _technology = technology

    End Sub

    ''' <summary>
    ''' Shuts down the command module, cleaning up any reasources used by the command module.
    ''' </summary>
    ''' <remarks></remarks>
    Public Sub Shutdown()

        'Shutdown the dll.
        _dllWrapper.Shutdown()

    End Sub

    ''' <summary>
    ''' Returns a list of any supported standard Engine API dialogs.
    ''' </summary>
    ''' <returns>A list of any supported dialogs.</returns>
    ''' <remarks>See the Engine API document for details about standard dialogs.  
    ''' No dialogs are supported for Phase 1.</remarks>
    Public Function ReportDialogs() As String
        'No dialogs are supported.
        Dim allLines() As String = File.ReadAllLines(".\Qisda\ReportDialog.txt")
        Dim retString As String = ""
        For Each line As String In allLines
            retString += line & ","
        Next
        Return retString.Substring(0, retString.Length - 1)
    End Function

    ''' <summary>
    ''' Opens one of the supported standard Engine API dialogs.
    ''' </summary>
    ''' <param name="dialog">The dialog (as specified in the dialog list) to be opened.</param>
    ''' <remarks>See the Engine API document for details about standard dialogs.  
    ''' No dialogs are supported for Phase 1.</remarks>
    Public Sub OpenDialog(ByVal dialog As String)

        'No dialogs are supported.
        'Format of the parameters string:
        '<CommandName>,<Parameter1>,<Parameter2>...
        'Everything after <CommandName> is optional.

        'Create a variable to hold the command name.
        Dim command As String = dialog

        'Create a variable to hold the return from the command.
        Dim returnValue As String = String.Empty

        'Catch any exceptions thrown while executing the command.
        Try
            Dim allLines() As String = File.ReadAllLines(".\Qisda\ReportDialog.txt")
            'Select which command to run and run it.
            Select Case command.ToLower
                Case "UserDataBackupAndRestore".ToLower
                    _dllWrapper.BRDialog()
                Case "FlashFlex".ToLower
                    _dllWrapper.Multidownload()
                Case "SecurityProgramming".ToLower
                    _dllWrapper.DlgSecurityTool()
                Case "IMEIOTP".ToLower
                    _dllWrapper.DlgIMEIOTP()
                Case "FailureCount".ToLower
                    _dllWrapper.AddFailure()
                Case "QFuse".ToLower
                    _dllWrapper.DlgQFuseTool()
                Case "RestoreBRT".ToLower
                    _dllWrapper.RestoreBRTDialog()
                Case "GSensorCalibration".ToLower
                    _dllWrapper.CalibrateGSensor()
                Case Else
                    'Return a failure if the command was not recognized.
                    'Return "Failure,Command not recognized: " & parameters & ","
                    Return
            End Select

        Catch ex As Exception

            'Write the exception information to the debug window.
            Debug.WriteLine(ex.ToString)

            'Remove any commas from the exception message.
            Dim messageWithoutCommas As String = ex.Message.Replace(","c, " ")

            'Return the exception message as the command status message, with Failure as the status.
            'Return "Failure,Exception occured during testing: " & messageWithoutCommas & ","
            Return

        End Try

        'Return the Success as the status, and the return from the command.
        'Return "Success,," & returnValue
        Return

    End Sub

#End Region

#Region "Phone Commands"

    ''' <summary>
    ''' Reads the ESN from the phone.
    ''' </summary>
    ''' <returns>The ESN.</returns>
    ''' <remarks>See the Engine API document for more information about this phone command.</remarks>
    Private Function ReadESN() As String

        'Read the ESN from the dll.
        Return _dllWrapper.ReadESN()

    End Function

    ''' <summary>
    ''' Guides the operator through all required manual CIT tests.
    ''' </summary>
    ''' <returns>An empty string.</returns>
    ''' <remarks>See the Engine API document for more information about this phone command.</remarks>
    Private Function ManualCITTest() As String

        'Create variables to hold the results of each test.
        Dim response As String = String.Empty
        Dim passed As Boolean = False

        'Ask the user to start the phone.
        OutputMessage("Please power on the phone")
        _dllWrapper.UnlockKeyPad()
        _dllWrapper.CleanScreenKeyBuffer()
        _dllWrapper.EnterEM()
        _dllWrapper.EnterLCDTest()
        'Ask the user if the display is working.
        response = OutputPrompt("Display Test", "Did the LCD show a rectangle around the edges?", PromptType.YesNo)

        'Figure out if the test passed.
        If response = "Yes" Then
            passed = True
        Else
            passed = False
        End If

        'Output the result of the test.
        OutputResult("DisplayEdgeTest", passed)

        _dllWrapper.TestLCDNextPattern()

        response = OutputPrompt("Display Test", "Did the LCD trun to red?", PromptType.YesNo)

        'Figure out if the test passed.
        If response = "Yes" Then
            passed = True
        Else
            passed = False
        End If

        'Output the result of the test.
        OutputResult("DisplayRedTest", passed)

        _dllWrapper.TestLCDNextPattern()

        response = OutputPrompt("Display Test", "Did the LCD trun to green?", PromptType.YesNo)

        'Figure out if the test passed.
        If response = "Yes" Then
            passed = True
        Else
            passed = False
        End If

        'Output the result of the test.
        OutputResult("DisplayGreenTest", passed)

        _dllWrapper.TestLCDNextPattern()

        response = OutputPrompt("Display Test", "Did the LCD trun to blue?", PromptType.YesNo)

        'Figure out if the test passed.
        If response = "Yes" Then
            passed = True
        Else
            passed = False
        End If

        'Output the result of the test.
        OutputResult("DisplayBlueTest", passed)

        _dllWrapper.BackToEM()

        _dllWrapper.EnterVibratorTest()

        response = OutputPrompt("Vibrator Test", "Is the vibrator working correctly?", PromptType.YesNo)

        If response = "Yes" Then
            passed = True
        Else
            passed = False
        End If

        'Output the result of the test.
        OutputResult("Vibrator", passed)
        _dllWrapper.BackToEM()
        _dllWrapper.ReturnToHomeScreen()
        'Return an empty string.
        Return String.Empty

    End Function

    ''' <summary>
    ''' Complete RF phasing
    ''' </summary>
    ''' <returns>The ESN.</returns>
    ''' <remarks>See the Engine API document for more information about this phone command.</remarks>
    Private Function CompletePhase() As Boolean

        'Read the ESN from the dll.
        Return _dllWrapper.CompletePhase()

    End Function

    Private Function ReadFactoryByte() As String
        Return "True"
    End Function

#End Region

#Region "Output Functions"

    ''' <summary>
    ''' Outputs a pass/fail only test result.
    ''' </summary>
    ''' <param name="description">Description of the result.</param>
    ''' <param name="passed">True if the result passed, False if it failed.</param>
    ''' <remarks>This private helper function calls the Engine API standard OutputResult function.</remarks>
    Private Sub OutputResult(ByVal description As String, ByVal passed As Boolean)

        'Convert the boolean Pass/Fail to a numeric result.
        Dim numericResult As Integer
        If passed Then
            numericResult = 1
        Else
            numericResult = 0
        End If

        'Use the normal OutputResult function with 1 for the limits.
        OutputResult(description, 1, 1, numericResult)

    End Sub

    ''' <summary>
    ''' Outputs a test result from the command module.
    ''' </summary>
    ''' <param name="description">Description of the test result.</param>
    ''' <param name="lowLimit">Lowest possible pass value.</param>
    ''' <param name="highLimit">Highest possible pass value.</param>
    ''' <param name="result">The actual value of the test.</param>
    ''' <remarks>See the Engine API document for information about output functions.</remarks>
    Private Sub OutputResult(ByVal description As String, ByVal lowLimit As Double, ByVal highLimit As Double, ByVal result As Double)

        'Create a variable to hold the Pass/Fail flag.
        Dim passFailFlag As String = String.Empty

        'Check the limits to see if the test passed or failed.
        If result <= highLimit And result >= lowLimit Then
            passFailFlag = "P"
        Else
            passFailFlag = "F"
        End If

        'Send the information to the callback function.
        DoCallBack("OutputResult", description & "," & lowLimit.ToString & "," & highLimit.ToString & "," & result.ToString & "," & passFailFlag)

    End Sub

    ''' <summary>
    ''' Outputs data from the command module.
    ''' </summary>
    ''' <param name="description">Description of the data.</param>
    ''' <param name="data">The data to output.</param>
    ''' <remarks>See the Engine API document for information about output functions.</remarks>
    Private Sub OutputData(ByVal description As String, ByVal data As String)

        'Send the information to the callback function.
        DoCallBack("OutputData", description & "," & data)

    End Sub

    ''' <summary>
    ''' Outputs a header to describe upcoming data and results.
    ''' </summary>
    ''' <param name="header">The header to output.</param>
    ''' <remarks>See the Engine API document for information about output functions.</remarks>
    Private Sub OutputHeader(ByVal header As String)

        'Send the information to the callback function.
        DoCallBack("OutputHeader", header)

    End Sub

    ''' <summary>
    ''' Outputs a message which might be displayed to the user.
    ''' </summary>
    ''' <param name="message">Message suitable to display to the user.</param>
    ''' <remarks>This message might be translated before being shown to the user.
    ''' All of the possible messages that can be sent by this function must be
    ''' sent to the STE team so that translations can be produced.
    ''' See the Engine API document for information about output functions.</remarks>
    Private Sub OutputMessage(ByVal message As String)

        'Send the information to the callback function.
        DoCallBack("OutputMessage", message)

    End Sub

    ''' <summary>
    ''' Outputs an Engine API standard status code to the engine.
    ''' </summary>
    ''' <param name="statusCode">The standard status code to send.</param>
    ''' <remarks>See the Engine API document for information about output functions.</remarks>
    Private Sub OutputStatus(ByVal statusCode As String)

        'Send the information to the callback function.
        DoCallBack("OutputStatus", statusCode)

    End Sub

    ''' <summary>
    ''' A type of prompt for the OutputPrompt function.
    ''' </summary>
    ''' <remarks>See the Engine API document for information about OutputPrompt.</remarks>
    Private Enum PromptType
        PassFailQuit
        OK
        YesNo
        TextPrompt
    End Enum

    ''' <summary>
    ''' Outputs a Engine API standard prompt to the user.
    ''' </summary>
    ''' <param name="title">Title of the prompt window.</param>
    ''' <param name="prompt">Prompt to display to the user.</param>
    ''' <param name="type">Type of prompt.</param>
    ''' <returns>The response from the user.</returns>
    ''' <remarks>See the Engine API document for information about prompts.</remarks>
    Private Function OutputPrompt(ByVal title As String, ByVal prompt As String, ByVal type As PromptType) As String

        'Send the information to the callback function and return the response.
        Return DoCallBack("OutputPrompt", title & "|" & prompt & "|" & type.ToString)

    End Function

    ''' <summary>
    ''' Sends a message to the engine using the callback function.
    ''' </summary>
    ''' <param name="type">Type of callback being used.</param>
    ''' <param name="data">Callback data, specific to the type of callback.</param>
    ''' <returns>The callback result, if any.</returns>
    ''' <remarks>See the Engine API document for information about callbacks.</remarks>
    Private Function DoCallBack(ByVal type As String, ByVal data As String) As String

        'Make sure the callback function has been set.
        If Not _callBack Is Nothing Then

            'Put the parameters into an array.
            Dim parameters As Object() = {type, data}

            'Invoke the callback function using the parameters.
            Dim returnObject As Object = _callBack.DynamicInvoke(parameters)

            'Convert the return value to a string.
            Dim returnString As String = CType(returnObject, String)

            'Return the return value.
            Return returnString

        Else

            'Ignore this call if there is no callback function set.
            Return String.Empty

        End If

    End Function

#End Region

End Class
