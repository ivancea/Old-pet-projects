Public Class Form1
    Dim changed As Boolean = True
    Private Sub agregar_Click(sender As System.Object, e As System.EventArgs) Handles agregar.Click
        textprecio.Text = textprecio.Text.Replace(".", ",")
        If IsNumeric(textcantidad.Text) And IsNumeric(textprecio.Text) Then
            datagrid.Rows.Add(textid.Text, textnombre.Text, Double.Parse(textcantidad.Text), Double.Parse(textprecio.Text), Double.Parse(textprecio.Text * textcantidad.Text), "X")
            textid.Text = ""
            textnombre.Text = ""
            textcantidad.Text = ""
            textprecio.Text = ""
            numcantidad.Visible = False
            numprecio.Visible = False
            If datagrid.SortOrder <> 0 Then
                datagrid.Sort(datagrid.SortedColumn, datagrid.SortOrder - 1)
            End If
        Else       ''''''Si no es un numero la cantidad o el precio, hace visible el mensaje bajo el miembro inválido
            If Not IsNumeric(textcantidad.Text) Then
                numcantidad.Visible = True
            Else
                numcantidad.Visible = False
            End If
            If Not IsNumeric(textprecio.Text) Then
                numprecio.Visible = True
            Else
                numprecio.Visible = False
            End If
        End If
        changed = True
    End Sub

    Private Sub timer_Tick(sender As System.Object, e As System.EventArgs) Handles timer.Tick
        hora.Text = TimeOfDay
        fecha.Text = Char.ToUpperInvariant(Date.Now.ToLongDateString.Split(",")(0)(0)) + Date.Now.ToLongDateString.Split(",")(0).Substring(1) + ",  " + Date.Now.ToShortDateString
        If changed Then
            Dim totalPrize As Double
            For i As Integer = 0 To datagrid.RowCount - 1
                totalPrize = totalPrize + datagrid.Item(4, i).Value
            Next
            precioTotal.Text = totalPrize.ToString + "€"
            changed = False
        End If
    End Sub

    Private Sub datagrid_CellContentClick(sender As System.Object, e As System.Windows.Forms.DataGridViewCellEventArgs) Handles datagrid.CellContentClick
        If e.ColumnIndex >= 0 And e.RowIndex >= 0 Then
            If datagrid.Columns.Item(e.ColumnIndex).HeaderText = "Eliminar" Then
                datagrid.Rows.RemoveAt(e.RowIndex)
            End If
        End If
        changed = True
    End Sub
End Class
