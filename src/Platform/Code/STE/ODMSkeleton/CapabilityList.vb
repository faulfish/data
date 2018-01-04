Option Strict On
Option Explicit On

''' <summary>
''' Represents a capability list, to which capabilities can be added.
''' </summary>
''' <remarks>See the Engine API document for more information of the capability list.</remarks>
Public Class CapabilityList

    'A string builder object to hold the capabilities.
    Private _builder As New Text.StringBuilder

    ''' <summary>
    ''' Add a new capability to the list.
    ''' </summary>
    ''' <param name="category">The category of the function.</param>
    ''' <param name="model">The model that supports the function.</param>
    ''' <param name="protocol">The protocol/technology of the model that supports the function.</param>
    ''' <param name="band">The band that the function is supported for.</param>
    ''' <param name="func">The function that is supported.</param>
    ''' <param name="hardwareEnable">False if the command is disabled due to client hardware limitations.</param>
    ''' <param name="softwareEnable">False if the command is disabled due to client software limitations.</param>
    ''' <param name="securityEnable">False if the command is disabled due to client security limitations.</param>
    ''' <remarks>See the Engine API document for more information of the capability list.</remarks>
    Public Sub AddCapability(ByVal category As String, ByVal model As String, ByVal protocol As String, ByVal band As String, ByVal func As String, ByVal hardwareEnable As Boolean, ByVal softwareEnable As Boolean, ByVal securityEnable As Boolean)

        'Append all of the information to the capability list in the correct format.
        _builder.Append(category)
        _builder.Append("|")
        _builder.Append(model)
        _builder.Append("|")
        _builder.Append(protocol)
        _builder.Append("|")
        _builder.Append(band)
        _builder.Append("|")
        _builder.Append(func)
        _builder.Append("=")
        _builder.Append(hardwareEnable.ToString)
        _builder.Append("|")
        _builder.Append(softwareEnable.ToString)
        _builder.Append("|")
        _builder.Append(securityEnable.ToString)
        _builder.Append(vbCrLf)

    End Sub

    ''' <summary>
    ''' Returns a string representation of the capability list.
    ''' </summary>
    ''' <returns>A string representation of the capability list.</returns>
    ''' <remarks>The string returned follows the Engine API standards for capability lists.</remarks>
    Public Overrides Function ToString() As String
        'Return the string builder's string.
        Return _builder.ToString
    End Function

End Class
