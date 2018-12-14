Module Bm4Hack

#Region "Declaraciones"
    Private Declare Function WriteProcessMemory Lib "kernel32" (ByVal hProcess As Integer, ByVal lpBaseAddress As Integer, ByRef lpBuffer As Integer, ByVal nSize As Integer, ByRef lpNumberOfBytesWritten As Integer) As Integer
    Private Declare Function ReadProcessMemory Lib "kernel32" Alias "ReadProcessMemory" (ByVal hProcess As Integer, ByVal lpBaseAddress As Integer, ByRef lpBuffer As Integer, ByVal nSize As Integer, ByRef lpNumberOfBytesWritten As Integer) As Integer
    Private Declare Function WriteFloatMemory Lib "kernel32" Alias "WriteProcessMemory" (ByVal hProcess As Integer, ByVal lpBaseAddress As Integer, ByRef lpBuffer As Single, ByVal nSize As Integer, ByRef lpNumberOfBytesWritten As Integer) As Integer
    Private Declare Function ReadFloat Lib "kernel32" Alias "ReadProcessMemory" (ByVal hProcess As IntPtr, ByVal lpBaseAddress As IntPtr, ByRef buffer As Single, ByVal size As Int32, ByRef lpNumberOfBytesRead As Int32) As Boolean
    Private Declare Function ReadInt32 Lib "kernel32" Alias "ReadProcessMemory" (ByVal hProcess As Integer, ByVal lpBaseAddress As Integer, ByRef lpBuffer As Integer, ByVal nSize As Integer, ByRef lpNumberOfBytesWritten As Integer) As Integer
    Private Declare Function OpenProcess Lib "kernel32" (ByVal dwDesiredAccess As Integer, ByVal bInheritHandle As Integer, ByVal dwProcessId As Integer) As Integer
    Private Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Integer) As Integer
#End Region
    Private value As String
    Public P_PID As String = 0
    Public Function Bm4_MatarProceso()
        If P_PID Then
            Process.GetProcessById(P_PID).Kill()
        End If
        Return False
    End Function
    Public Function Bm4_SacarTituloProceso()
        If P_PID Then
            Return Process.GetProcessById(P_PID).MainWindowTitle
        Else
            Return ""
        End If
    End Function
    Public Function Bm4_SacarNombreProceso()
        If P_PID Then
            Return Process.GetProcessById(P_PID).ProcessName
        Else
            Return ""
        End If
    End Function
    Public Function Bm4_SeleccionarProcesoPorTitulo(ByVal title As String)
        Dim ps() As Process
        Dim p As Process
        ps = Process.GetProcesses()
        For Each p In ps
            If (p.MainWindowTitle = title) Then
                P_PID = p.Id
            End If
        Next
        Return False
    End Function
    Public Function Bm4_SeleccionarProcesoPorNombre(ByVal Name As String)
        Dim gameLookUp As Process() = Process.GetProcessesByName(Name)
        P_PID = gameLookUp(0).Id
        Return False
    End Function
    Public Function Bm4_SeleccionarProcesoPorID(ByVal ID As String)
        P_PID = ID
        Return False
    End Function
    Public Function Bm4_CerrarProceso()
        Dim processHandle As IntPtr = OpenProcess(&H1F0FFF, 0, P_PID)
        If P_PID Then
            CloseHandle(processHandle)
        End If
        Return False
    End Function
#Region "Write"
    Public Function Bm4_EscribirMemoria(ByVal Address As Integer, ByVal value As Long, ByVal Bytes As Integer)
        Dim processHandle As IntPtr = OpenProcess(&H1F0FFF, 0, P_PID)
        If P_PID Then
            WriteProcessMemory(processHandle, Address, value, Bytes, Nothing)
        End If
        Return False
    End Function
    Public Function Bm4_EscribirFloat(ByVal Address As Integer, ByVal value As Single)
        Dim processHandle As IntPtr = OpenProcess(&H1F0FFF, 0, P_PID)
        If P_PID Then
            WriteFloatMemory(processHandle, Address, value, 4, Nothing)
        End If
        Return False
    End Function
    Public Function Bm4_EscribirString(ByVal address As Long, ByVal str As String) As Boolean
        Dim processHandle As IntPtr = OpenProcess(&H1F0FFF, 0, P_PID)
        If P_PID Then
            For i As Integer = 0 To Len(str) - 1
                WriteProcessMemory(processHandle, address + i, Asc(Mid(str, i + 1, 1)), 1, 0)
            Next
            WriteProcessMemory(processHandle, address + Len(str), 0, 1, 0)
        End If
        Return False
    End Function
    Public Function Bm4_EscribirStringUnicode(ByVal address As Long, ByVal str As String) As Boolean
        Dim processHandle As IntPtr = OpenProcess(&H1F0FFF, 0, P_PID)
        Dim count2 As Integer
        For i As Integer = 0 To (Len(str) * 2) - 1
            count2 += 1
            WriteProcessMemory(processHandle, address + i, Asc(Mid(str, count2, 1)), 1, 0)
            i += 1
            WriteProcessMemory(processHandle, address + i, 0, 1, 0)
        Next
        WriteProcessMemory(processHandle, address + (Len(str) * 2), 0, 1, 0)
        Return False
    End Function
    Public Function Bm4_EscribirArray(ByVal Address As Integer, ByVal bytes() As Byte)
        Dim processHandle As IntPtr = OpenProcess(&H1F0FFF, 0, P_PID)
        If P_PID Then
            Dim i As Integer
            For i = 0 To bytes.Length - 1
                WriteProcessMemory(processHandle, Address + i, bytes(i), 1, Nothing)
            Next
        End If
        Return False
    End Function
    Public Function Bm4_EscribirNop(ByVal Address As Integer, ByVal Cantidad As Integer)
        Dim processHandle As IntPtr = OpenProcess(&H1F0FFF, 0, P_PID)
        For i = 0 To Cantidad
            If i = Cantidad Then
                Exit For
            End If
            If P_PID Then
                WriteProcessMemory(processHandle, "&H" & Conversion.Hex(Address), &H90, 1, Nothing)
            End If
            Address = Address + &H1
        Next
        Return False
    End Function

#End Region
#Region "Leer"
    Public Function Bm4_LeerMemoria(ByVal Address As Integer, ByVal Bytes As Integer)
        Dim processHandle As IntPtr = OpenProcess(&H1F0FFF, 0, P_PID)
        If P_PID Then
            ReadProcessMemory(processHandle, Address, value, Bytes, Nothing)
            Return value
        Else
            Return False
        End If
    End Function
    Public Function Bm4_LeerFloat(ByVal Address As Integer)
        Dim processHandle As IntPtr = OpenProcess(&H1F0FFF, 0, P_PID)
        If P_PID Then
            Readfloat(processHandle, Address, value, 4, Nothing)
            Return value
        Else
            Return False
        End If
    End Function
    Public charmaxlengh = 32
    Public Function Bm4_LeerString(ByVal Address As Long) As String
        Dim processHandle As IntPtr = OpenProcess(&H1F0FFF, 0, P_PID)
        If P_PID Then
            Dim ret As Byte() = Nothing
            Dim vBuffer As Long
            Dim tStr(charmaxlengh) As Char
            Dim retStr As String = ""
            For i As Long = 0 To charmaxlengh - 1
                ReadProcessMemory(processHandle, Address + i, vBuffer, 1, 0)
                If (vBuffer = 0) Then
                    GoTo 1
                Else
                    ret = BitConverter.GetBytes(vBuffer)
                    tStr(i) = System.Text.Encoding.ASCII.GetString(ret) : retStr += tStr(i)
                End If
            Next
1:
            Return retStr
        Else
            Return False
        End If
    End Function
    Public Function Bm4_LeerStringUnicode(ByVal Address As Long) As String
        Dim processHandle As IntPtr = OpenProcess(&H1F0FFF, 0, P_PID)
        If P_PID Then
            Dim ret As Byte() = Nothing
            Dim vBuffer As Long
            Dim tStr(charmaxlengh) As Char
            Dim retStr As String = ""
            For i As Long = 0 To (charmaxlengh * 2) - 1
                ReadProcessMemory(processHandle, Address + i, vBuffer, 1, 0)
                i += 1
                If (vBuffer = 0) Then
                    GoTo 1
                Else
                    ret = BitConverter.GetBytes(vBuffer)
                    tStr(i) = System.Text.Encoding.ASCII.GetString(ret) : retStr += tStr(i)
                End If
            Next
1:
            Return retStr
        Else
            Return False
        End If
    End Function
    Public Function Bm4_LeerArray(ByVal Address As Integer, ByVal lenght As Integer)
        Dim processHandle As IntPtr = OpenProcess(&H1F0FFF, 0, P_PID)
        If P_PID Then
            Dim cadena As String = ""
            Dim i As Integer
            For i = 0 To lenght - 1
                ReadProcessMemory(processHandle, Address + i, value, 1, Nothing)
                cadena = cadena & Hex(value)
                If (i = lenght - 1) Then
                Else
                    cadena = cadena & " "
                End If
            Next
            Return cadena
        Else
            Return False
        End If
    End Function
    Public Function Bm4_LeerDLL(ByVal modulename As String)
        If P_PID Then
            Dim procmodule As ProcessModule
            Dim constant1 As Integer
            Dim constant2 As Long
            Dim constant3 As Process = Process.GetProcessById(P_PID)
            For Each procmodule In constant3.Modules
                If modulename = procmodule.ModuleName Then
                    constant1 = procmodule.BaseAddress
                End If
            Next
            constant2 = constant1
            Return "&H " & Hex(constant2)
        Else
            Return False
        End If
    End Function
    Public Function Bm4_SacarPointer(ByVal Addybase As String, ByVal Off1 As Integer, Optional ByVal Off2 As Integer = 99999, Optional ByVal Off3 As Integer = 99999, Optional ByVal Off4 As Integer = 99999, Optional ByVal Off5 As Integer = 99999) As String
        Dim processHandle As IntPtr = OpenProcess(&H1F0FFF, 0, P_PID)
        If P_PID Then
            Dim addy As String
            ReadProcessMemory(processHandle, Addybase, value, 4, Nothing)
            addy = value + Off1
            ReadProcessMemory(processHandle, addy, value, 4, Nothing)
            If (Off2 = 99999) Then
            Else
                addy = value + Off2
                ReadProcessMemory(processHandle, addy, value, 4, Nothing)
            End If
            If (Off3 = 99999) Then
            Else
                addy = value + Off3
                ReadProcessMemory(processHandle, addy, value, 4, Nothing)
            End If
            If (Off4 = 99999) Then
            Else
                addy = value + Off4
                ReadProcessMemory(processHandle, addy, value, 4, Nothing)
            End If
            If (Off5 = 99999) Then
            Else
                addy = value + Off5
                ReadProcessMemory(processHandle, addy, value, 4, Nothing)
            End If
            addy = Conversion.Hex(addy)
            addy = "&H" & addy
            Return addy
        Else
            Return False
        End If
    End Function
#End Region
End Module
