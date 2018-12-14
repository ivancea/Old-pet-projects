<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form reemplaza a Dispose para limpiar la lista de componentes.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Requerido por el Diseñador de Windows Forms
    Private components As System.ComponentModel.IContainer

    'NOTA: el Diseñador de Windows Forms necesita el siguiente procedimiento
    'Se puede modificar usando el Diseñador de Windows Forms.  
    'No lo modifique con el editor de código.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Dim DataGridViewCellStyle3 As System.Windows.Forms.DataGridViewCellStyle = New System.Windows.Forms.DataGridViewCellStyle()
        Dim DataGridViewCellStyle1 As System.Windows.Forms.DataGridViewCellStyle = New System.Windows.Forms.DataGridViewCellStyle()
        Dim DataGridViewCellStyle2 As System.Windows.Forms.DataGridViewCellStyle = New System.Windows.Forms.DataGridViewCellStyle()
        Me.datagrid = New System.Windows.Forms.DataGridView()
        Me.id = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.nombre = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.cantidad = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.precio = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.total = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.eliminar = New System.Windows.Forms.DataGridViewButtonColumn()
        Me.textid = New System.Windows.Forms.TextBox()
        Me.textnombre = New System.Windows.Forms.TextBox()
        Me.textcantidad = New System.Windows.Forms.TextBox()
        Me.textprecio = New System.Windows.Forms.TextBox()
        Me.agregar = New System.Windows.Forms.Button()
        Me.hora = New System.Windows.Forms.Label()
        Me.timer = New System.Windows.Forms.Timer(Me.components)
        Me.numcantidad = New System.Windows.Forms.Label()
        Me.numprecio = New System.Windows.Forms.Label()
        Me.fecha = New System.Windows.Forms.Label()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.precioTotal_static = New System.Windows.Forms.Label()
        Me.precioTotal = New System.Windows.Forms.Label()
        CType(Me.datagrid, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'datagrid
        '
        Me.datagrid.AccessibleRole = System.Windows.Forms.AccessibleRole.None
        Me.datagrid.AllowUserToAddRows = False
        Me.datagrid.AllowUserToDeleteRows = False
        Me.datagrid.AllowUserToResizeColumns = False
        Me.datagrid.AllowUserToResizeRows = False
        Me.datagrid.BackgroundColor = System.Drawing.Color.White
        Me.datagrid.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText
        Me.datagrid.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.DisableResizing
        Me.datagrid.Columns.AddRange(New System.Windows.Forms.DataGridViewColumn() {Me.id, Me.nombre, Me.cantidad, Me.precio, Me.total, Me.eliminar})
        Me.datagrid.ImeMode = System.Windows.Forms.ImeMode.NoControl
        Me.datagrid.Location = New System.Drawing.Point(12, 12)
        Me.datagrid.MultiSelect = False
        Me.datagrid.Name = "datagrid"
        Me.datagrid.ReadOnly = True
        Me.datagrid.RowHeadersVisible = False
        Me.datagrid.RowHeadersWidthSizeMode = System.Windows.Forms.DataGridViewRowHeadersWidthSizeMode.DisableResizing
        DataGridViewCellStyle3.BackColor = System.Drawing.Color.White
        DataGridViewCellStyle3.ForeColor = System.Drawing.SystemColors.MenuText
        DataGridViewCellStyle3.NullValue = Nothing
        DataGridViewCellStyle3.SelectionBackColor = System.Drawing.Color.White
        DataGridViewCellStyle3.SelectionForeColor = System.Drawing.Color.Black
        Me.datagrid.RowsDefaultCellStyle = DataGridViewCellStyle3
        Me.datagrid.ShowEditingIcon = False
        Me.datagrid.Size = New System.Drawing.Size(428, 209)
        Me.datagrid.TabIndex = 0
        '
        'id
        '
        Me.id.DividerWidth = 1
        Me.id.HeaderText = "Id"
        Me.id.Name = "id"
        Me.id.ReadOnly = True
        Me.id.Resizable = System.Windows.Forms.DataGridViewTriState.[False]
        Me.id.Width = 25
        '
        'nombre
        '
        Me.nombre.DividerWidth = 1
        Me.nombre.HeaderText = "Nombre"
        Me.nombre.Name = "nombre"
        Me.nombre.ReadOnly = True
        Me.nombre.Resizable = System.Windows.Forms.DataGridViewTriState.[False]
        '
        'cantidad
        '
        Me.cantidad.DividerWidth = 1
        Me.cantidad.HeaderText = "Cantidad"
        Me.cantidad.Name = "cantidad"
        Me.cantidad.ReadOnly = True
        Me.cantidad.Resizable = System.Windows.Forms.DataGridViewTriState.[False]
        Me.cantidad.Width = 75
        '
        'precio
        '
        DataGridViewCellStyle1.Format = "C2"
        DataGridViewCellStyle1.NullValue = "0"
        Me.precio.DefaultCellStyle = DataGridViewCellStyle1
        Me.precio.DividerWidth = 1
        Me.precio.HeaderText = "Precio"
        Me.precio.Name = "precio"
        Me.precio.ReadOnly = True
        Me.precio.Resizable = System.Windows.Forms.DataGridViewTriState.[False]
        Me.precio.Width = 75
        '
        'total
        '
        DataGridViewCellStyle2.Format = "C2"
        DataGridViewCellStyle2.NullValue = "0"
        Me.total.DefaultCellStyle = DataGridViewCellStyle2
        Me.total.DividerWidth = 1
        Me.total.HeaderText = "Precio total"
        Me.total.Name = "total"
        Me.total.ReadOnly = True
        Me.total.Resizable = System.Windows.Forms.DataGridViewTriState.[False]
        '
        'eliminar
        '
        Me.eliminar.HeaderText = "Eliminar"
        Me.eliminar.Name = "eliminar"
        Me.eliminar.ReadOnly = True
        Me.eliminar.Resizable = System.Windows.Forms.DataGridViewTriState.[False]
        Me.eliminar.Width = 50
        '
        'textid
        '
        Me.textid.Location = New System.Drawing.Point(30, 272)
        Me.textid.Name = "textid"
        Me.textid.Size = New System.Drawing.Size(47, 20)
        Me.textid.TabIndex = 1
        '
        'textnombre
        '
        Me.textnombre.Location = New System.Drawing.Point(83, 272)
        Me.textnombre.Name = "textnombre"
        Me.textnombre.Size = New System.Drawing.Size(93, 20)
        Me.textnombre.TabIndex = 2
        '
        'textcantidad
        '
        Me.textcantidad.Location = New System.Drawing.Point(182, 272)
        Me.textcantidad.Name = "textcantidad"
        Me.textcantidad.Size = New System.Drawing.Size(68, 20)
        Me.textcantidad.TabIndex = 3
        '
        'textprecio
        '
        Me.textprecio.Location = New System.Drawing.Point(256, 272)
        Me.textprecio.Name = "textprecio"
        Me.textprecio.Size = New System.Drawing.Size(69, 20)
        Me.textprecio.TabIndex = 4
        '
        'agregar
        '
        Me.agregar.Location = New System.Drawing.Point(338, 262)
        Me.agregar.Name = "agregar"
        Me.agregar.Size = New System.Drawing.Size(56, 38)
        Me.agregar.TabIndex = 5
        Me.agregar.Text = "Agregar"
        Me.agregar.UseVisualStyleBackColor = True
        '
        'hora
        '
        Me.hora.AutoSize = True
        Me.hora.Font = New System.Drawing.Font("Microsoft Sans Serif", 18.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.hora.ForeColor = System.Drawing.Color.Firebrick
        Me.hora.Location = New System.Drawing.Point(484, 21)
        Me.hora.Name = "hora"
        Me.hora.Size = New System.Drawing.Size(71, 29)
        Me.hora.TabIndex = 6
        Me.hora.Text = "Hora"
        '
        'timer
        '
        Me.timer.Enabled = True
        Me.timer.Interval = 250
        '
        'numcantidad
        '
        Me.numcantidad.AutoSize = True
        Me.numcantidad.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.numcantidad.ForeColor = System.Drawing.Color.DarkSlateBlue
        Me.numcantidad.Location = New System.Drawing.Point(179, 295)
        Me.numcantidad.Name = "numcantidad"
        Me.numcantidad.Size = New System.Drawing.Size(78, 13)
        Me.numcantidad.TabIndex = 7
        Me.numcantidad.Text = "No numérico"
        Me.numcantidad.Visible = False
        '
        'numprecio
        '
        Me.numprecio.AutoSize = True
        Me.numprecio.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.numprecio.ForeColor = System.Drawing.Color.DarkSlateBlue
        Me.numprecio.Location = New System.Drawing.Point(254, 295)
        Me.numprecio.Name = "numprecio"
        Me.numprecio.Size = New System.Drawing.Size(78, 13)
        Me.numprecio.TabIndex = 8
        Me.numprecio.Text = "No numérico"
        Me.numprecio.Visible = False
        '
        'fecha
        '
        Me.fecha.AutoSize = True
        Me.fecha.Font = New System.Drawing.Font("Microsoft Sans Serif", 18.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.fecha.ForeColor = System.Drawing.Color.Firebrick
        Me.fecha.Location = New System.Drawing.Point(484, 75)
        Me.fecha.Name = "fecha"
        Me.fecha.Size = New System.Drawing.Size(88, 29)
        Me.fecha.TabIndex = 9
        Me.fecha.Text = "Fecha"
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(44, 256)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(16, 13)
        Me.Label1.TabIndex = 10
        Me.Label1.Text = "Id"
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Location = New System.Drawing.Point(108, 256)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(44, 13)
        Me.Label2.TabIndex = 11
        Me.Label2.Text = "Nombre"
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Location = New System.Drawing.Point(192, 256)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(49, 13)
        Me.Label3.TabIndex = 12
        Me.Label3.Text = "Cantidad"
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Location = New System.Drawing.Point(253, 256)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(76, 13)
        Me.Label4.TabIndex = 13
        Me.Label4.Text = "Precio/Unidad"
        '
        'precioTotal_static
        '
        Me.precioTotal_static.AutoSize = True
        Me.precioTotal_static.Font = New System.Drawing.Font("Microsoft Sans Serif", 10.28!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.precioTotal_static.Location = New System.Drawing.Point(232, 224)
        Me.precioTotal_static.Name = "precioTotal_static"
        Me.precioTotal_static.Size = New System.Drawing.Size(50, 17)
        Me.precioTotal_static.TabIndex = 14
        Me.precioTotal_static.Text = "Total:"
        '
        'precioTotal
        '
        Me.precioTotal.AutoSize = True
        Me.precioTotal.Font = New System.Drawing.Font("Microsoft Sans Serif", 10.28!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.precioTotal.Location = New System.Drawing.Point(288, 224)
        Me.precioTotal.Name = "precioTotal"
        Me.precioTotal.Size = New System.Drawing.Size(24, 17)
        Me.precioTotal.TabIndex = 15
        Me.precioTotal.Text = "0€"
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(772, 330)
        Me.Controls.Add(Me.precioTotal)
        Me.Controls.Add(Me.precioTotal_static)
        Me.Controls.Add(Me.Label4)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.fecha)
        Me.Controls.Add(Me.numprecio)
        Me.Controls.Add(Me.numcantidad)
        Me.Controls.Add(Me.hora)
        Me.Controls.Add(Me.agregar)
        Me.Controls.Add(Me.textprecio)
        Me.Controls.Add(Me.textcantidad)
        Me.Controls.Add(Me.textnombre)
        Me.Controls.Add(Me.textid)
        Me.Controls.Add(Me.datagrid)
        Me.Name = "Form1"
        Me.Text = "Prueba"
        CType(Me.datagrid, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents datagrid As System.Windows.Forms.DataGridView
    Friend WithEvents textid As System.Windows.Forms.TextBox
    Friend WithEvents textnombre As System.Windows.Forms.TextBox
    Friend WithEvents textcantidad As System.Windows.Forms.TextBox
    Friend WithEvents textprecio As System.Windows.Forms.TextBox
    Friend WithEvents agregar As System.Windows.Forms.Button
    Friend WithEvents hora As System.Windows.Forms.Label
    Friend WithEvents timer As System.Windows.Forms.Timer
    Friend WithEvents numcantidad As System.Windows.Forms.Label
    Friend WithEvents numprecio As System.Windows.Forms.Label
    Friend WithEvents fecha As System.Windows.Forms.Label
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents Label4 As System.Windows.Forms.Label
    Friend WithEvents id As System.Windows.Forms.DataGridViewTextBoxColumn
    Friend WithEvents nombre As System.Windows.Forms.DataGridViewTextBoxColumn
    Friend WithEvents cantidad As System.Windows.Forms.DataGridViewTextBoxColumn
    Friend WithEvents precio As System.Windows.Forms.DataGridViewTextBoxColumn
    Friend WithEvents total As System.Windows.Forms.DataGridViewTextBoxColumn
    Friend WithEvents eliminar As System.Windows.Forms.DataGridViewButtonColumn
    Friend WithEvents precioTotal_static As System.Windows.Forms.Label
    Friend WithEvents precioTotal As System.Windows.Forms.Label

End Class
