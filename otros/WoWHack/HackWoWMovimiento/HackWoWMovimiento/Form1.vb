Public Class hackwow
    Dim boton As Button
    Dim mantenido As Boolean = False
    Dim modo As Boolean = 0

    Private Sub hackwow_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        If Process.GetProcessesByName("wow").Length = 1 Then
            Bm4_SeleccionarProcesoPorNombre("wow")
        Else
            MsgBox("Debes abrir este programa después de haber abierto el World of Warcraft versión 3.3.5a.")
            End
        End If
    End Sub
    Private Sub hackwow_Focus(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.MouseDown
        Dim address As Integer
        address = Bm4_SacarPointer("&HCD87A8", &H34, &H24, &H764, &H0, &H69)
        Bm4_EscribirMemoria(address, &H0, 1)
    End Sub
    Public Sub refrescar()
        Dim coordenada As Integer
        coordenada = Bm4_SacarPointer("&HCD87A8", &H34, &H24, &H764, &H0, &H34)
        coordx.Text = Bm4_LeerFloat(coordenada)
        coordz.Text = Bm4_LeerFloat(coordenada + 4)
        coordy.Text = Bm4_LeerFloat(coordenada + 8)
        mapId.Text = Bm4_LeerMemoria(&HADFBC4, 4)
        mantenido = True
    End Sub

    Private Sub norte_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles norte.Click
        Dim coordenadax As Integer
        Dim coordenadaz As Integer
        coordenadax = Bm4_SacarPointer("&HCD87A8", &H34, &H24, &H764, &H0, &H34)
        coordenadaz = Bm4_SacarPointer("&HCD87A8", &H34, &H24, &H764, &H0, &H38)
        If modo Then
            Dim rotacion As Single
            rotacion = Bm4_LeerFloat(Bm4_SacarPointer("&HCD87A8", &H34, &H24, &H764, &H0, &H44))
            Bm4_EscribirFloat(coordenadax, Bm4_LeerFloat(coordenadax) + Math.Cos(rotacion))
            Bm4_EscribirFloat(coordenadaz, Bm4_LeerFloat(coordenadaz) + Math.Sin(rotacion))
        Else
            Bm4_EscribirFloat(coordenadax, Bm4_LeerFloat(coordenadax) + 1)
        End If
        refrescar()
        boton = norte
        tiempo.Enabled = True
    End Sub

    Private Sub oeste_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles oeste.Click
        Dim coordenadax As Integer
        Dim coordenadaz As Integer
        coordenadax = Bm4_SacarPointer("&HCD87A8", &H34, &H24, &H764, &H0, &H34)
        coordenadaz = Bm4_SacarPointer("&HCD87A8", &H34, &H24, &H764, &H0, &H38)
        If modo Then
            Dim rotacion As Single
            rotacion = Bm4_LeerFloat(Bm4_SacarPointer("&HCD87A8", &H34, &H24, &H764, &H0, &H44)) + Math.PI / 2
            Bm4_EscribirFloat(coordenadax, Bm4_LeerFloat(coordenadax) + Math.Cos(rotacion))
            Bm4_EscribirFloat(coordenadaz, Bm4_LeerFloat(coordenadaz) + Math.Sin(rotacion))
        Else
            Bm4_EscribirFloat(coordenadaz, Bm4_LeerFloat(coordenadaz) + 1)
        End If
        refrescar()
        boton = oeste
        tiempo.Enabled = True
    End Sub

    Private Sub este_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles este.Click
        Dim coordenadax As Integer
        Dim coordenadaz As Integer
        coordenadax = Bm4_SacarPointer("&HCD87A8", &H34, &H24, &H764, &H0, &H34)
        coordenadaz = Bm4_SacarPointer("&HCD87A8", &H34, &H24, &H764, &H0, &H38)
        If modo Then
            Dim rotacion As Single
            rotacion = Bm4_LeerFloat(Bm4_SacarPointer("&HCD87A8", &H34, &H24, &H764, &H0, &H44)) - Math.PI / 2
            Bm4_EscribirFloat(coordenadax, Bm4_LeerFloat(coordenadax) + Math.Cos(rotacion))
            Bm4_EscribirFloat(coordenadaz, Bm4_LeerFloat(coordenadaz) + Math.Sin(rotacion))
        Else
            Bm4_EscribirFloat(coordenadaz, Bm4_LeerFloat(coordenadaz) - 1)
        End If
        refrescar()
        boton = este
        tiempo.Enabled = True
    End Sub

    Private Sub sur_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles sur.Click
        Dim coordenadax As Integer
        Dim coordenadaz As Integer
        coordenadax = Bm4_SacarPointer("&HCD87A8", &H34, &H24, &H764, &H0, &H34)
        coordenadaz = Bm4_SacarPointer("&HCD87A8", &H34, &H24, &H764, &H0, &H38)
        If modo Then
            Dim rotacion As Single
            rotacion = Bm4_LeerFloat(Bm4_SacarPointer("&HCD87A8", &H34, &H24, &H764, &H0, &H44))
            Bm4_EscribirFloat(coordenadax, Bm4_LeerFloat(coordenadax) - Math.Cos(rotacion))
            Bm4_EscribirFloat(coordenadaz, Bm4_LeerFloat(coordenadaz) - Math.Sin(rotacion))
        Else
            Bm4_EscribirFloat(coordenadax, Bm4_LeerFloat(coordenadax) - 1)
        End If
        refrescar()
        boton = sur
        tiempo.Enabled = True
    End Sub

    Private Sub elevarse_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ascender.Click
        Dim coordenaday As Integer
        coordenaday = Bm4_SacarPointer("&HCD87A8", &H34, &H24, &H764, &H0, &H3C)
        Bm4_EscribirFloat(coordenaday, Bm4_LeerFloat(coordenaday) + 1)
        refrescar()
        boton = ascender
        tiempo.Enabled = True
    End Sub

    Private Sub descender_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles descender.Click
        Dim coordenaday As Integer
        coordenaday = Bm4_SacarPointer("&HCD87A8", &H34, &H24, &H764, &H0, &H3C)
        Bm4_EscribirFloat(coordenaday, Bm4_LeerFloat(coordenaday) - 1)
        refrescar()
        boton = descender
        tiempo.Enabled = True
    End Sub

    Private Sub tiempo_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles tiempo.Tick
        If mantenido Then
            boton.PerformClick()
        Else
            Integer.TryParse(velocidad.Text, tiempo.Interval)
            If tiempo.Interval < 1 Then
                tiempo.Interval = 100
            Else
                tiempo.Interval = Integer.Parse(velocidad.Text)
            End If
            tiempo.Stop()
        End If
    End Sub

    Private Sub ButtonMouseUp(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles norte.MouseUp, sur.MouseUp, este.MouseUp, oeste.MouseUp, descender.MouseUp, ascender.MouseUp, girarizquierda.MouseUp, girarderecha.MouseUp
        mantenido = False
    End Sub
    Private Sub norteMouseDown(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles norte.MouseDown
        mantenido = True
        norte.PerformClick()
    End Sub

    Private Sub surMouseDown(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles sur.MouseDown
        mantenido = True
        sur.PerformClick()
    End Sub

    Private Sub esteMouseDown(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles este.MouseDown
        mantenido = True
        este.PerformClick()
    End Sub

    Private Sub oesteMouseDown(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles oeste.MouseDown
        mantenido = True
        oeste.PerformClick()
    End Sub

    Private Sub descenderMouseDown(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles descender.MouseDown
        mantenido = True
        descender.PerformClick()
    End Sub

    Private Sub ascenderMouseDown(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ascender.MouseDown
        mantenido = True
        ascender.PerformClick()
    End Sub

    Private Sub girarizquierdaMouseDown(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles girarizquierda.MouseDown
        mantenido = True
        girarizquierda.PerformClick()
    End Sub
    Private Sub girarderechaMouseDown(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles girarderecha.MouseDown
        mantenido = True
        girarderecha.PerformClick()
    End Sub

    Private Sub alternar_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles alternar.Click
        If modo Then
            modo = False
            norte.Text = "Norte"
            sur.Text = "Sur"
            este.Text = "Este"
            oeste.Text = "Oeste"
        Else
            modo = True
            norte.Text = "Adelante"
            sur.Text = "Atras"
            este.Text = "Derecha"
            oeste.Text = "Izquierda"
        End If
    End Sub

    Private Sub girarizquierda_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles girarizquierda.Click
        Dim angulo As Integer
        angulo = Bm4_SacarPointer("&HCD87A8", &H34, &H24, &H764, &H0, &H44)
        Bm4_EscribirFloat(angulo, Bm4_LeerFloat(angulo) + 0.15)
        refrescar()
        boton = girarizquierda
        tiempo.Enabled = True
    End Sub

    Private Sub girarderecha_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles girarderecha.Click
        Dim angulo As Integer
        angulo = Bm4_SacarPointer("&HCD87A8", &H34, &H24, &H764, &H0, &H44)
        Bm4_EscribirFloat(angulo, Bm4_LeerFloat(angulo) - 0.15)
        refrescar()
        boton = girarderecha
        tiempo.Enabled = True
    End Sub

    Private Sub Button1_Click(sender As System.Object, e As System.EventArgs) Handles Button1.Click
        Dim coordenada As Integer
        coordenada = Bm4_SacarPointer("&HCD87A8", &H34, &H24, &H764, &H0, &H34)
        Bm4_EscribirFloat(coordenada, coordx.Text)
        Bm4_EscribirFloat(coordenada + 4, coordy.Text)
        Bm4_EscribirFloat(coordenada + 8, coordz.Text)
        Bm4_EscribirMemoria("&HADFBC4", mapId.Text, 4)
    End Sub
End Class
