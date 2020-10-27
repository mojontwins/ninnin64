' ene exporter
' generates minimal contents for usage in small ROMs.

sub usage
	Print "Usage:"
	Print 
	Print "$ eneexp3 enems.ene out.h prefix [yadjust] [nohotspots] [gencounter] [genallcounters] [bin]"
	Print
	Print "bin puts eneexp3 in binmode. out.h will contain constants and out.h.bin will contain the enemy data"
End Sub

Function inCommand (a As String) As Integer
	Dim As Integer i
	i = 1
	While (Command (i) <> "")
		If Command (i) = a Then Return -1
		i = i + 1
	Wend
	Return 0
End Function

Dim As Integer fIn, fOut, fBin
Dim As uByte d, a, b
Dim As String Dummy, prefix
Dim As Integer mapW, mapH, nEnems, mapPants, nPant, i, j, hl, yadjust, noHotspots, binMode
Dim As uByte t, xy1, xy2, mn, s1, s2
Dim As Integer typeCounters (255)
Dim As Integer enemTypeCounters (255)
Dim As Integer genCounter, genAllCounters

Print "MK1 v2.0 eneexp3 ~ ";

If Command (3) = "" Then usage: End
prefix = Command (3)

yadjust = Val (Command (4))
Print "yadjust: " & yadjust & " ~ ";
If inCommand ("noHotspots") Then 
	noHotspots = -1 : Print "No hotspots ~ "; 
Else 
	noHotspots = 0
End If
genCounter = inCommand ("gencounter")
genAllCounters = inCommand ("genallcounters")
binMode = inCommand ("bin")

' Outputs 4 bytes per entry
' T for type
' XY for ini
' XY for end
' MN for properties

' .ENE contains...
' t x1 y1 x2 y2 n s1 s2
' n is speed
' s1, s2 are unused.


fIn = FreeFile
Open Command (1) For Binary As #fIn

If binMode Then 
	fBin = FreeFile
	Open Command (2) & ".bin" For Output As #fBin
	Print "BIN ~ ";
End If 

fOut = FreeFile
Open Command (2) For Output As #fOut

' Skip header
dummy = Input (256, fIn)
Get #fIn, , d: mapW = d
Get #fIn, , d: mapH = d
Get #fIn, , d: Get #fIn, , d
Get #fIn, , d: nEnems = d

mapPants = mapW * mapH

If Not binMode Then
	Print #fOut, "// " & Command (2)
	Print #fOut, "// Generated by eneexp3.exe v0.2 [20160731]"
	Print #fOut, "// Copyleft 2016 The Mojon Twins"
	Print #fOut, ""
	Print #fOut, "// Enems structure is {T YX1 YX2 P} x " & nEnems & " x MAP_W x MAP_H"
	Print #fOut, "// y coordinates are adjusted by " & yadjust & " positions"
	Print #fOut, "const unsigned char enems_" & prefix & " [] = {"
Else
	Print #fOut, "// " + Command (1)
	Print #fOut, ""
End If

hl = 0
For i = 1 To (mapPants * nEnems)
	Get #fIn, , t

	enemTypeCounters (t) = enemTypeCounters (t) + 1
	Get #fIn, , a: Get #fIn, , b: b = b + yadjust
	xy1 = (b Shl 4) Or (a And 15)
	Get #fIn, , a: Get #fIn, , b: b = b + yadjust
	xy2 = (b Shl 4) Or (a And 15)
	Get #fIn, , mn
	Get #fIn, , s1: Get #fIn, , s2

	'' Watch out for special cases
	If t = &HFF Then
		' Warpers.
		' xy2 should be subst. by s1
		xy2 = s1
	End If

	If binMode Then
		Put #fBin, , t 
		Put #fBin, , xy1 
		Put #fBin, , xy2 
		Put #fBin, , mn
	Else
		If hl = 0 Then Print #fOut, "	";
		Print #fOut, "0x" & Hex (t, 2) & ", 0x" & Hex (xy1, 2) & ", 0x" & Hex (xy2, 2) & ", 0x" & Hex (mn, 2);
		If i < (mapPants * nEnems) Then Print #fOut, ", ";
		hl = 1 - hl
		If hl = 0 Then Print #fOut, ""
	End If
Next i

If Not binMode Then
	Print #fOut, "};"
	Print #fOut, ""
Else
	Print #fOut, "#define HOTSPOTS_OFFSET_" & prefix & " 0x" & Lcase (Hex (4*mapPants*nEnems, 4))
	Print #fOut, ""
End If

For i = 1 To 255
	If enemTypeCounters (i) Or (i < 32 And genAllCounters) Then
		print #fOut, "#define MAX_ENEMS_TYPE_" & i & "_" & prefix & " " & enemTypeCounters (i)
	End If
Next i
Print #fOut, ""

If genCounter Then
	print #fOut, "#define KILLABLE_ENEMS_" & prefix & " " & (enemTypeCounters (1) + enemTypeCounters (2) + enemTypeCounters (3) + enemTypeCounters (&H41) + enemTypeCounters (&H42) +enemTypeCounters (&H43) + enemTypeCounters (&H81) + enemTypeCounters (&H82) + enemTypeCounters (&H83) + enemTypeCounters (6) + enemTypeCounters (9) + enemTypeCounters (11) + enemTypeCounters (20) )
	Print #fOut, ""
End If

Print " Enems: " & (mapPants * nEnems) & " (" & (mapPants * nEnems * 4) & " bytes) ~ ";

If Not noHotspots Then
	If Not binMode Then
		Print #fOut, "// Hotspots structure is {T YX} x MAP_W x MAP_H"
		Print #fOut, "const unsigned char hotspots_" & prefix & " [] = {"
	End If
	hl = 0
	For i = 1 To mapPants
		Get #fIn, , xy1
		Get #fIn, , xy2: xy2 = xy2 + yadjust
		xy1 = (xy2 Shl 4) Or (xy1 And 15)
		Get #fIn, , t

		If binMode Then
			Put #fBin, , t 
			Put #fBin, , xy1
		Else
			if hl = 0 Then Print #fOut, "	";
			Print #fOut, "0x" & Hex (t, 2) & ", 0x" & Hex (xy1, 2);
			If i < mapPants Then Print #fOut, ", ";
			hl = (hl + 1) And 3
			If hl = 0 Then Print #fOut, ""
		End If
		typeCounters (t) = typeCounters (t) + 1
	Next i

	If Not binMode Then
		Print #fOut, "};"
		Print #fOut, ""
	End If
	
	For i = 0 To 255
		If typeCounters (i) <> 0 Or i = 1 then
			' Print #fOut, "// Wrote " & typecounters (i) & " hotspots of type " & i
			Print #fOut, "#define MAX_HOTSPOTS_TYPE_" & i & "_" & prefix & "	" & typeCounters (i)	
		End If
	Next i

	If binMode Then Print #fOut, ""

	Print " Hotspots: " & mapPants & " (" & (mapPants * 2) & " bytes) ~ ";
End If

Close fIn, fOut
If binMode Then Close fBin

Print "DONE!"
