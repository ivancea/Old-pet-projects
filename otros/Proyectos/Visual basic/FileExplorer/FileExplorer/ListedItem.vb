Public Class ListedItem

    Public IsDirectory As Boolean
    Public Name As String

    Public Sub New(name As String, isDirectory As Boolean)
        Me.Name = name
        Me.IsDirectory = isDirectory
    End Sub

    Public Overrides Function ToString() As String
        If IsDirectory Then
            Return "-> " + Name
        End If
        Return Name
    End Function

End Class
