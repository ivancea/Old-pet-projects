Public Class Form1

    Private ActualDirectory As String

    Private Sub Form1_Load(sender As System.Object, e As System.EventArgs) Handles MyBase.Load
        ActualDirectory = "C:\"
        ShowDirectory("C:\")
    End Sub

    Private Sub ListFiles_SelectedIndexChanged(sender As Object, e As System.EventArgs) Handles ListFiles.SelectedIndexChanged
        Dim item As ListedItem
        item = ListFiles.SelectedItem
        If item.IsDirectory Then
            If item.Name = ".." Then
                Dim parent As System.IO.DirectoryInfo
                parent = System.IO.Directory.GetParent(ActualDirectory)
                If Not IsNothing(parent) Then
                    ShowDirectory(parent.FullName)
                End If
            Else
                If ActualDirectory.EndsWith("\") Or ActualDirectory.EndsWith("/") Then
                    ShowDirectory(ActualDirectory + item.Name)
                End If
                ShowDirectory(ActualDirectory + "\" + item.Name)
            End If
        End If
    End Sub

    Private Sub ShowDirectory(directory As String)
        Dim dirs, files As System.Collections.ObjectModel.ReadOnlyCollection(Of String)
        Try
            dirs = My.Computer.FileSystem.GetDirectories(directory)
            files = My.Computer.FileSystem.GetFiles(directory)

            ListFiles.Items.Clear()
            If Not IsNothing(System.IO.Directory.GetParent(directory)) Then
                ListFiles.Items.Add(New ListedItem("..", True))
            End If
            For Each dir As String In dirs
                ListFiles.Items.Add(New ListedItem(System.IO.Path.GetFileName(dir), True))
            Next
            For Each file As String In files
                ListFiles.Items.Add(New ListedItem(System.IO.Path.GetFileName(file), False))
            Next
            ActualDirectory = directory
            TextDirectory.Text = ActualDirectory
        Catch ex As System.IO.DirectoryNotFoundException
        Catch ex As UnauthorizedAccessException
            MessageBox.Show("Acceso denegado")
        End Try
    End Sub

    Private Sub TextDirectory_KeyPress(sender As Object, e As System.Windows.Forms.KeyPressEventArgs) Handles TextDirectory.KeyPress
        If e.KeyChar = Microsoft.VisualBasic.ChrW(Keys.Return) Then
            ShowDirectory(TextDirectory.Text)
        End If
    End Sub
End Class
