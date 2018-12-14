<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class hackwow
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
        Me.norte = New System.Windows.Forms.Button()
        Me.oeste = New System.Windows.Forms.Button()
        Me.este = New System.Windows.Forms.Button()
        Me.sur = New System.Windows.Forms.Button()
        Me.descender = New System.Windows.Forms.Button()
        Me.ascender = New System.Windows.Forms.Button()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.coordx = New System.Windows.Forms.TextBox()
        Me.coordy = New System.Windows.Forms.TextBox()
        Me.coordz = New System.Windows.Forms.TextBox()
        Me.tiempo = New System.Windows.Forms.Timer(Me.components)
        Me.alternar = New System.Windows.Forms.Button()
        Me.girarderecha = New System.Windows.Forms.Button()
        Me.girarizquierda = New System.Windows.Forms.Button()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.velocidad = New System.Windows.Forms.TextBox()
        Me.Button1 = New System.Windows.Forms.Button()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.mapId = New System.Windows.Forms.TextBox()
        Me.SuspendLayout()
        '
        'norte
        '
        Me.norte.Location = New System.Drawing.Point(125, 75)
        Me.norte.Name = "norte"
        Me.norte.Size = New System.Drawing.Size(75, 50)
        Me.norte.TabIndex = 0
        Me.norte.Text = "Norte"
        Me.norte.UseVisualStyleBackColor = True
        '
        'oeste
        '
        Me.oeste.Location = New System.Drawing.Point(50, 125)
        Me.oeste.Name = "oeste"
        Me.oeste.Size = New System.Drawing.Size(75, 50)
        Me.oeste.TabIndex = 1
        Me.oeste.Text = "Oeste"
        Me.oeste.UseVisualStyleBackColor = True
        '
        'este
        '
        Me.este.Location = New System.Drawing.Point(200, 125)
        Me.este.Name = "este"
        Me.este.Size = New System.Drawing.Size(75, 50)
        Me.este.TabIndex = 2
        Me.este.Text = "Este"
        Me.este.UseVisualStyleBackColor = True
        '
        'sur
        '
        Me.sur.Location = New System.Drawing.Point(125, 175)
        Me.sur.Name = "sur"
        Me.sur.Size = New System.Drawing.Size(75, 50)
        Me.sur.TabIndex = 3
        Me.sur.Text = "Sur"
        Me.sur.UseVisualStyleBackColor = True
        '
        'descender
        '
        Me.descender.Location = New System.Drawing.Point(285, 175)
        Me.descender.Name = "descender"
        Me.descender.Size = New System.Drawing.Size(75, 50)
        Me.descender.TabIndex = 4
        Me.descender.Text = "Descender"
        Me.descender.UseVisualStyleBackColor = True
        '
        'ascender
        '
        Me.ascender.Location = New System.Drawing.Point(285, 75)
        Me.ascender.Name = "ascender"
        Me.ascender.Size = New System.Drawing.Size(75, 50)
        Me.ascender.TabIndex = 5
        Me.ascender.Text = "Ascender"
        Me.ascender.UseVisualStyleBackColor = True
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(54, 9)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(75, 13)
        Me.Label1.TabIndex = 6
        Me.Label1.Text = "Coordenada X"
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Location = New System.Drawing.Point(149, 9)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(110, 13)
        Me.Label2.TabIndex = 7
        Me.Label2.Text = "Coordenada Y (altura)"
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Location = New System.Drawing.Point(274, 9)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(75, 13)
        Me.Label3.TabIndex = 8
        Me.Label3.Text = "Coordenada Z"
        '
        'coordx
        '
        Me.coordx.Location = New System.Drawing.Point(40, 25)
        Me.coordx.Name = "coordx"
        Me.coordx.Size = New System.Drawing.Size(102, 20)
        Me.coordx.TabIndex = 9
        '
        'coordy
        '
        Me.coordy.Location = New System.Drawing.Point(150, 25)
        Me.coordy.Name = "coordy"
        Me.coordy.Size = New System.Drawing.Size(108, 20)
        Me.coordy.TabIndex = 10
        '
        'coordz
        '
        Me.coordz.Location = New System.Drawing.Point(264, 25)
        Me.coordz.Name = "coordz"
        Me.coordz.Size = New System.Drawing.Size(96, 20)
        Me.coordz.TabIndex = 11
        '
        'tiempo
        '
        '
        'alternar
        '
        Me.alternar.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.alternar.Location = New System.Drawing.Point(50, 250)
        Me.alternar.Name = "alternar"
        Me.alternar.Size = New System.Drawing.Size(310, 50)
        Me.alternar.TabIndex = 12
        Me.alternar.Text = "Alternar movimiento normal/movimiento coordenadas"
        Me.alternar.UseVisualStyleBackColor = True
        '
        'girarderecha
        '
        Me.girarderecha.Location = New System.Drawing.Point(200, 75)
        Me.girarderecha.Name = "girarderecha"
        Me.girarderecha.Size = New System.Drawing.Size(75, 50)
        Me.girarderecha.TabIndex = 13
        Me.girarderecha.Text = "Girar Derecha"
        Me.girarderecha.UseVisualStyleBackColor = True
        '
        'girarizquierda
        '
        Me.girarizquierda.Location = New System.Drawing.Point(50, 75)
        Me.girarizquierda.Name = "girarizquierda"
        Me.girarizquierda.Size = New System.Drawing.Size(75, 50)
        Me.girarizquierda.TabIndex = 14
        Me.girarizquierda.Text = "Girar Izquierda"
        Me.girarizquierda.UseVisualStyleBackColor = True
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label4.Location = New System.Drawing.Point(132, 128)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(63, 13)
        Me.Label4.TabIndex = 15
        Me.Label4.Text = "Velocidad"
        '
        'velocidad
        '
        Me.velocidad.Location = New System.Drawing.Point(131, 149)
        Me.velocidad.Name = "velocidad"
        Me.velocidad.Size = New System.Drawing.Size(64, 20)
        Me.velocidad.TabIndex = 16
        Me.velocidad.Text = "100"
        Me.velocidad.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'Button1
        '
        Me.Button1.Location = New System.Drawing.Point(367, 51)
        Me.Button1.Name = "Button1"
        Me.Button1.Size = New System.Drawing.Size(68, 29)
        Me.Button1.TabIndex = 17
        Me.Button1.Text = "Ir"
        Me.Button1.UseVisualStyleBackColor = True
        '
        'Label5
        '
        Me.Label5.AutoSize = True
        Me.Label5.Location = New System.Drawing.Point(382, 9)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(42, 13)
        Me.Label5.TabIndex = 18
        Me.Label5.Text = "Map ID"
        '
        'mapId
        '
        Me.mapId.Location = New System.Drawing.Point(367, 25)
        Me.mapId.Name = "mapId"
        Me.mapId.Size = New System.Drawing.Size(68, 20)
        Me.mapId.TabIndex = 19
        '
        'hackwow
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.AutoSize = True
        Me.ClientSize = New System.Drawing.Size(478, 312)
        Me.Controls.Add(Me.mapId)
        Me.Controls.Add(Me.Label5)
        Me.Controls.Add(Me.Button1)
        Me.Controls.Add(Me.velocidad)
        Me.Controls.Add(Me.Label4)
        Me.Controls.Add(Me.girarizquierda)
        Me.Controls.Add(Me.girarderecha)
        Me.Controls.Add(Me.alternar)
        Me.Controls.Add(Me.coordz)
        Me.Controls.Add(Me.coordy)
        Me.Controls.Add(Me.coordx)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.ascender)
        Me.Controls.Add(Me.descender)
        Me.Controls.Add(Me.sur)
        Me.Controls.Add(Me.este)
        Me.Controls.Add(Me.oeste)
        Me.Controls.Add(Me.norte)
        Me.Name = "hackwow"
        Me.Text = "Movimiento Asistido WoW"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents norte As System.Windows.Forms.Button
    Friend WithEvents oeste As System.Windows.Forms.Button
    Friend WithEvents este As System.Windows.Forms.Button
    Friend WithEvents sur As System.Windows.Forms.Button
    Friend WithEvents descender As System.Windows.Forms.Button
    Friend WithEvents ascender As System.Windows.Forms.Button
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents coordx As System.Windows.Forms.TextBox
    Friend WithEvents coordy As System.Windows.Forms.TextBox
    Friend WithEvents coordz As System.Windows.Forms.TextBox
    Friend WithEvents tiempo As System.Windows.Forms.Timer
    Friend WithEvents alternar As System.Windows.Forms.Button
    Friend WithEvents girarderecha As System.Windows.Forms.Button
    Friend WithEvents girarizquierda As System.Windows.Forms.Button
    Friend WithEvents Label4 As System.Windows.Forms.Label
    Friend WithEvents velocidad As System.Windows.Forms.TextBox
    Friend WithEvents Button1 As System.Windows.Forms.Button
    Friend WithEvents Label5 As System.Windows.Forms.Label
    Friend WithEvents mapId As System.Windows.Forms.TextBox

End Class
