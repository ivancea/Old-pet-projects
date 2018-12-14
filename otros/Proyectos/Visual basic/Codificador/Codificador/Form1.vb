Public Class Form1

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load

    End Sub


    Private Sub TextBox1_TextChanged(sender As Object, e As EventArgs)

    End Sub
    Private Sub Button9_Click(sender As Object, e As EventArgs) Handles Button9.Click
        TextBox1.Text = ((label1.Text + (2 * (CInt(label1.Text) + 1))) * 23) + CInt(TextBox1.Text)
        label1.Text = CInt(label1.Text) + 1
    End Sub

    Private Sub Button7_Click(sender As Object, e As EventArgs) Handles Button7.Click
        TextBox1.Text = ((label1.Text + (2 * (CInt(label1.Text) + 1))) * 17) + CInt(TextBox1.Text)
        label1.Text = CInt(label1.Text) + 1
    End Sub
    Private Sub Button6_Click(sender As Object, e As EventArgs) Handles Button6.Click
        TextBox1.Text = ((label1.Text + (2 * (CInt(label1.Text) + 1))) * 13) + CInt(TextBox1.Text)
        label1.Text = CInt(label1.Text) + 1
    End Sub

    Private Sub Button5_Click(sender As Object, e As EventArgs) Handles Button5.Click
        TextBox1.Text = ((label1.Text + (2 * (CInt(label1.Text) + 1))) * 11) + CInt(TextBox1.Text)
        label1.Text = CInt(label1.Text) + 1
    End Sub
    Private Sub Button4_Click(sender As Object, e As EventArgs) Handles Button4.Click
        TextBox1.Text = ((label1.Text + (2 * (CInt(label1.Text) + 1))) * 7) + CInt(TextBox1.Text)
        label1.Text = CInt(label1.Text) + 1
    End Sub

    Private Sub Button3_Click(sender As Object, e As EventArgs) Handles Button3.Click
        TextBox1.Text = ((label1.Text + (2 * (CInt(label1.Text) + 1))) * 5) + CInt(TextBox1.Text)
        label1.Text = CInt(label1.Text) + 1
    End Sub
    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        TextBox1.Text = ((label1.Text + (2 * (CInt(label1.Text) + 1))) * 2) + CInt(TextBox1.Text)
        label1.Text = CInt(label1.Text) + 1
    End Sub

    Private Sub Button10_Click(sender As Object, e As EventArgs) Handles Button10.Click
        TextBox1.Text = 0
        label1.Text = 0
    End Sub
    Private Sub Label1_Click(sender As Object, e As EventArgs)

    End Sub

    Private Sub Button8_Click(sender As Object, e As EventArgs) Handles Button8.Click
        TextBox1.Text = ((label1.Text + (2 * (CInt(label1.Text) + 1))) * 19) + CInt(TextBox1.Text)
        label1.Text = CInt(label1.Text) + 1
    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        TextBox1.Text = ((label1.Text + (2 * (CInt(label1.Text) + 1))) * 3) + CInt(TextBox1.Text)
        label1.Text = CInt(label1.Text) + 1
    End Sub

End Class
