'' Tileset import generator

'' What is this? 

'' Tileset import generator will make my life easier if I ever want to modify 
'' one of my old NES games. Back in the day I was either extremely lazy or
'' too afraid of spending time in writing proper converters and utilities, 
'' which was an error, but anyways. The conversion utility I wrote for the 
'' NES was so quick, dirty, and crappy that one had to manually reorder 
'' patterns and then change the pixel colours to pure red, green and blue
'' which were then translated to 01, 10 and 11 and written to tileset.chr.

'' That was incredibly cumbersome.

'' Luckily when I came back to the system months later I began writing mkts
'' which has made my life tads easier.

'' So.

'' This will get easy to edit files: a font, a tileset, a spriteset, and a
'' logo, plus two palette collections (16xN pixel files with N palettes),
'' and will generate a `tileset_import.png` file and extra data to fit in
'' the old toolchain.

'' This way if I ever want to modify my old games the amount of manual labour
'' will be nil.

'' In the incompresible revamp of MK1_NES 0.1 I use "autometasprites".
'' If this mode is activated, no metasprites need to be generated, but
'' a single array of metasprite attributes (one per metasprite).

#include "fbpng.bi"
#include "fbgfx.bi"
#include once "crt.bi"

#define RGBA_R( c ) ( CUInt( c ) Shr 16 And 255 )
#define RGBA_G( c ) ( CUInt( c ) Shr  8 And 255 )
#define RGBA_B( c ) ( CUInt( c )        And 255 )
#define RGBA_A( c ) ( CUInt( c ) Shr 24         )

Dim Shared As Integer palettes (15, 15), npalettes
Dim Shared As Integer decolours (3) = { &HFF000000, &HFFFF0000, &HFF00FF00, &HFF0000FF }
Dim Shared As Integer warning, autometasprites, debugon
Dim Shared As Integer nesPal (63) = { _
	&HFF7F7F7F, &HFF0000FF, &HFF0000BF, &HFF472BBF, &HFF970087, &HFFAB0023, &HFFAB1300, &HFF8B1700, _
	&HFF533000, &HFF007800, &HFF006B00, &HFF005B00, &HFF004358, &HFF000000, &HFF000000, &HFF000000, _
	&HFFBFBFBF, &HFF0078F8, &HFF0058F8, &HFF6B47FF, &HFFDB00CD, &HFFE7005B, &HFFF83800, &HFFE75F13, _
	&HFFAF7F00, &HFF00B800, &HFF00AB00, &HFF00AB47, &HFF008B8B, &HFF2C2C2C, &HFF000000, &HFF000000, _
	&HFFF8F8F8, &HFF3FBFFF, &HFF6B88FF, &HFF9878F8, &HFFF878F8, &HFFF85898, &HFFF87858, &HFFFFA347, _
	&HFFF8B800, &HFFB8F818, &HFF5BDB57, &HFF58F898, &HFF00EBDB, &HFF787878, &HFF000000, &HFF000000, _
	&HFFFFFFFF, &HFFA7E7FF, &HFFB8B8F8, &HFFD8B8F8, &HFFF8B8F8, &HFFFBA7C3, &HFFF0D0B0, &HFFFFE3AB, _
	&HFFFBDB7B, &HFFD8F878, &HFFB8F8B8, &HFFB8F8D8, &HFF00FFFF, &HFFF8D8F8, &HFF000000, &HFF000000 _ 
}

#define METATILE_PALETTE  1
#define METASPRITE_STRUCT 2
#define AUTOMETASPRITE    3

Sub usage
	Puts "$ tileset_import_generator.exe pats_ts.png pals_ss.png font.png ts.png ss.png [logo.png] output.png palettes.h metasprites.h [autometasprites]"
	Puts ""
	Puts "Palette files are 16xN pixels, and can contain 1<=N<=16 palettes."
	Puts "font file should be 256x16 pixels."
	Puts "ts file should be 256x48 pixels."
	Puts "ss file should be 256x48 pixels."
	Puts "logo file should be 48x16 pixels - only if not in autometasprites mode!"
	Puts "autometasprites enables autometasprites mode."
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

Function findColourInNesPal (c As Integer) As Integer
	Dim As Integer res, i
	res = 0
	For i = 0 To 63
		If (c And &HFFFFFF) = (nesPal (i) And &HFFFFFF) Then res = i: Exit For
	Next i
	Return res
End Function

Function inArray (needle As Integer, haystack () As Integer) As Integer
	Dim As Integer i
	For i = Lbound (haystack) To Ubound (haystack)
		If debugon Then Puts haystack (i) & " - " & needle
		If haystack (i) = needle Then Return -1
	Next i
	Return 0
End Function

Sub readPalettesFromFile (fileName As String)
	Dim As Any Ptr img
	Dim As Integer w, h, x, y
	img = png_load (fileName)
	If ImageInfo (img, w, h, , , , ) Then
		Puts "Wrong palette file " & fileName: System
	End If
	If h > 16 Then h = 16
	npalettes = h
	For y = 0 To h - 1
		For x = 0 To 15
			palettes (y, x) = Point (x, y, img)
		Next x
	Next y
	For y = h To 15
		For x = 0 To 15
			palettes (y, x) = &HFFFF00FF
		Next x
	Next y
End Sub

Sub printPal (pp () As Integer)
	Dim As String o 
	Dim As Integer i
	o = "  "
	For i = 0 To 3
		o = o & Hex (pp (i)) & " "
	Next i
	Puts o 
End Sub

Sub whichSubPal (img As Any Ptr, ByRef wp As Integer, ByRef wsp As Integer, ByRef orwith As Integer)
	Dim As Integer ic (3), ict (3)
	Dim As Integer x, y, w, h, i, c, pali, spali, found

	orwith = 0

	' Find four colours in img.
	For i = 0 To 3: ic (i) = &HFFFE00FE: Next i
	If ImageInfo (img, w, h, , , , ) Then 
		Puts "Something went incredibly wrong at whichSubpal": System
	End If
	i = 0
	For y = 0 To h - 1
		For x = 0 To w - 1
			c = Point (x, y, img)
			If Not inArray (c, ic ()) Then
				If i > 3  Then 
					If Not warning Then Puts ("Warning! More than 4 colours found...")
					warning = -1
				Else
					ic (i) = c: i = i + 1
				End If
			End If
		Next x
	Next y

	'printPal ic ()

	' Find the pal which contains all the colours in img.
	For pali = 0 To npalettes - 1
		For spali = 0 To 3
			For i = 0 To 3: ict (i) = palettes (pali, 4 * spali + i): Next i
			' Are all colours from the image in subpalete spali?
			'printPal ict ()

			found = -1
			For i = 0 To 3
				If ic (i) <> &HFFFE00FE And ic (i) <> &HFFFF00FF Then
					If Not inArray (ic (i), ict ()) Then
						' Not this
						found = 0
						Exit For
					End If
				End If
			Next i
			If found Then 				
				wp = pali: wsp = spali
				If inArray (&HFFFF00FF, ic ()) Then
					orwith = &H20 
				End If
				Exit Sub
			End If
		Next spali		
	Next pali
End Sub

Function findColour (c As Integer, wp As Integer, wsp As Integer) As Integer
	Dim As Integer i, res
	res = 0
	If c = &HFFFF00FF Then Return 0
	For i = 0 To 3
		If (c And &HFFFFFF) = (palettes (wp, wsp * 4 + i) And &HFFFFFF) Then res = i: Exit For
	Next i
	Return res
End Function

Sub decolourImage (img As Any Ptr, wp As Integer, wsp As Integer)
	Dim As Integer x, y, w, h

	If ImageInfo (img, w, h, , , , ) Then 
		Puts "Something went incredibly wrong at decolourImage": System
	End If

	For y = 0 To h - 1
		For x = 0 To w - 1
			Pset img, (x, y), decolours (findColour (Point (x, y, img), wp, wsp))
		Next x
	Next y
End Sub

Sub processImageDirect (imgIn As Any Ptr, imgOut As Any Ptr, x0 As Integer, y0 As Integer)
	Dim As FB.image ptr pattern
	Dim As Integer w, h, x, y, wp, wsp, orwith
	
	pattern = ImageCreate (8, 8)

	If ImageInfo (imgIn, w, h, , , , ) Then 
		Puts "Something went incredibly wrong at processImageDirect": System
	End If

	For y = 0 To h - 1 Step 8
		For x = 0 To w - 1 Step 8
			'' Cut chunk
			Get imgIn, (x, y) - (x + 7, y + 7), pattern

			'' Find palette & subpalette
			whichSubpal pattern, wp, wsp, orwith

			'' Decolour
			decolourImage pattern, wp, wsp

			'' Show some info
			If warning Then Puts "At ~ (" & x & ", " & y & ")": warning = 0

			'' Paste chunk
			Put imgOut, (x0 + x, y0 + y), pattern, Pset
		Next x
	Next y

	ImageDestroy (pattern)
End Sub

Sub processImageReorder2x2 (imgIn As Any Ptr, imgOut As Any Ptr, x0 As Integer, y0 As Integer, metadataType As Integer, metadataFH As Integer)
	Dim As FB.image ptr pattern, patternAux
	Dim As Integer w, h, x, y, xx, yy, wp, wsp, xw, yw, wo, ho, n, nc, orwith
	
	pattern = ImageCreate (8, 8)
	patternAux = ImageCreate (16, 16)

	If ImageInfo (imgIn, w, h, , , , ) Then 
		Puts "Something went incredibly wrong at processImageDirect": System
	End If
	If ImageInfo (imgOut, wo, ho, , , , ) Then 
		Puts "Something went incredibly wrong at processImageDirect": System
	End If

	If metadataType = METATILE_PALETTE Then Print #metadataFH, "const unsigned char tileset_pals [] = {"
	If metadataType = AUTOMETASPRITE   Then Print #metadataFH, "const unsigned char spriteset_pals [] = {"

	xw = x0: yw = y0
	n = 0: nc = 0
	For y = 0 To h - 1 Step 16
		If metadataType = METATILE_PALETTE Then Print #metadataFH, "	";

		For x = 0 To w - 1 Step 16
			'Puts "BOXY " & n			
			If metadataType = METASPRITE_STRUCT And n < 33 Then
				Print #metadataFH, "const unsigned char spr_";
				If n < 8 Then 
					Print #metadataFH, "pl1_" & n + 1;
				ElseIf n < 16 Then
					Print #metadataFH, "pl2_" & n - 7;
				ElseIf n < 24 Then
					Print #metadataFH, "en1_" & n - 15;
				ElseIf n < 32 Then
					Print #metadataFH, "en2_" & n - 23;
				Else
					Print #metadataFH, "explosion";
				End If

				Print #metadataFH, " [] = {"
				Print #metadataFh, "	";
			End If

			If metadataType = METATILE_PALETTE Or metadataType = AUTOMETASPRITE Then
				Get imgIn, (x, y) - (x + 15, y + 15), patternAux
				whichSubpal patternAux, wp, wsp, orwith
				If warning Then Puts "At ~ (" & x & ", " & y & ")": warning = 0
				Print #metadataFH, "0x" & Hex (wsp Or orwith, 2); ", ";
			End If

			If n < 33 Or metadataType <> METASPRITE_STRUCT Then
				For yy = 0 To 15 Step 8
					For xx = 0 To 15 Step 8		
						'' Cut chunk					
						Get imgIn, (x + xx, y + yy) - (x + xx + 7, y + yy + 7), pattern

						'' Find palette & subpalette
						If metadataType = METASPRITE_STRUCT Then whichSubpal pattern, wp, wsp, orwith

						'' Decolour
						decolourImage pattern, wp, wsp

						'' Show some info
						If warning Then Puts "At ~ (" & (x + xx) & ", " & (y + yy) & ")": warning = 0

						'' Paste chunk
						Put imgOut, (xw, yw), pattern, Pset
						xw = xw + 8: If xw >= wo Then xw = x0: yw = yw + 8

						If metadataType = METASPRITE_STRUCT Then
							Print #metadataFH, "0x" & Hex (xx, 2) & ", 0x" & Hex (yy, 2) & ", 0x" & Hex (nc, 2) & ", 0x" & Hex (wsp Or orwith, 2) & ", ";
						End If

						nc = nc + 1
					Next xx
				Next yy		
			End If

			If metadataType = METASPRITE_STRUCT And n < 33 Then 
				Print #metadataFH, "128"
				Print #metadataFH, "};"
				Print #metadataFH, ""
			End If

			n = n + 1	
		Next x

		If metadataType = METATILE_PALETTE Or metadataType = AUTOMETASPRITE Then Print #metadataFH, ""
	Next y

	If metadataType = METATILE_PALETTE Or metadataType = AUTOMETASPRITE Then Print #metadataFH, "};": Print #metadataFH, ""

	ImageDestroy (pattern)
	ImageDestroy (patternAux)
End Sub

Sub processLogo (imgIn As Any Ptr, imgOut As Any Ptr, x0 As Integer, y0 As Integer)
	Dim As FB.image ptr pattern
	Dim As Integer w, h, x, y, xx, yy, wp, wsp, xw, yw, wo, ho, orwith
	
	pattern = ImageCreate (8, 8)

	If ImageInfo (imgOut, wo, ho, , , , ) Then 
		Puts "Something went incredibly wrong at processImageDirect": System
	End If

	xw = x0: yw = y0
	For y = 0 To 15 Step 8
		For x = 0 To 47 Step 8
			'' Cut chunk
			Get imgIn, (x, y) - (x + 7, y + 7), pattern

			'' Find palette & subpalette
			whichSubpal pattern, wp, wsp, orwith

			'' Decolour
			decolourImage pattern, wp, wsp

			'' Show some info
			If warning Then Puts "At ~ (" & x & ", " & y & ")": warning = 0

			'' Paste chunk
			Put imgOut, (xw, yw), pattern, Pset
			xw = xw + 8: If xw >= wo Then xw = x0: yw = yw + 8
		Next x
	Next y

	ImageDestroy (pattern)
End Sub

Dim As String pals_ts_fn, pals_ss_fn, font_fn, ts_fn, ss_fn, logo_fn, output_fn, palettes_fn, metasprites_fn
Dim As FB.image ptr imgOut
Dim As FB.image ptr imgIn
Dim As Integer palettesFH, metaspritesFH
Dim As Integer i, j, pofs

'' Needs to have an internal pixel format
Screenres 640, 480, 32, , -1

'' Read command line arguments
If Command (6) = "" Then usage: End

autometasprites = inCommand ("autometasprites")
If autometasprites Then pofs = 6 Else pofs = 7

pals_ts_fn     = Command (1)
pals_ss_fn     = Command (2)
font_fn        = Command (3)
ts_fn          = Command (4)
ss_fn          = Command (5)
logo_fn        = Command (6)
output_fn      = Command (pofs)
palettes_fn    = Command (pofs+1)
metasprites_fn = Command (pofs+2)

imgOut = ImageCreate (256, 128, &HFF000000)

'' Open files
palettesFH = FreeFile
Open palettes_fn For Output As palettesFH

metaspritesFH = FreeFile
Open metasprites_fn For Output as metaspritesFH

'' Palettes
Puts "Palettes"
readPalettesFromFile pals_ts_fn
For i = 0 To npalettes - 1
	Print #palettesFH, "const unsigned char mypal_game_bg" & Hex (i, 1) & " [] = {"	
	For j = 0 To 15
		If j Mod 4 = 0 Then Print #palettesFH, "	";
		Print #palettesFH, "0x" & Ucase (Hex (findColourInNesPal (palettes (i, j)), 2));
		If j < 15 Then Print #palettesFH, ", ";
		If j Mod 4 = 3 Then Print #palettesFH, ""
	Next j
	Print #palettesFh, "};"
	Print #palettesFh, ""
Next i
readPalettesFromFile pals_ss_fn
For i = 0 To npalettes - 1
	Print #palettesFH, "const unsigned char mypal_game_fg" & Hex (i, 1) & " [] = {"	
	For j = 0 To 15
		If j Mod 4 = 0 Then Print #palettesFH, "	";
		Print #palettesFH, "0x" & Ucase (Hex (findColourInNesPal (palettes (i, j)), 2));
		If j < 15 Then Print #palettesFH, ", ";
		If j Mod 4 = 3 Then Print #palettesFH, ""
	Next j
	Print #palettesFh, "};"
	Print #palettesFh, ""
Next i

'' Bank 0
Puts "BANK 0"
readPalettesFromFile pals_ts_fn

'' Copy font
Puts "Font " & font_fn
imgIn = png_load (font_fn)
processImageDirect imgIn, imgOut, 0, 0

'' Reorder tileset & copy
Puts "Tileset " & ts_fn
imgIn = png_load (ts_fn)
processImageReorder2x2 imgIn, imgOut, 0, 16, METATILE_PALETTE, palettesFH

'' Bank 1
Puts "BANK 1"
readPalettesFromFile pals_ss_fn

'' Reorder spriteset & copy
Puts "Spriteset " & ss_fn
imgIn = png_load (ss_fn)
If autometasprites Then
	processImageReorder2x2 imgIn, imgOut, 0, 64, AUTOMETASPRITE, metaspritesFH
Else
	processImageReorder2x2 imgIn, imgOut, 0, 64, METASPRITE_STRUCT, metaspritesFH
End If

'' Process logo
If Not autometasprites Then
	Puts "Logo " & logo_fn
	imgIn = png_load (logo_fn)
	processLogo imgIn, imgOut, 152, 120
End If

Puts "Saving " & palettes_fn
Close palettesFH

Puts "Saving " & metasprites_fn
If Not autometasprites Then
	Print #metaspritesFH, "const unsigned char spr_empty [] = {"
	Print #metaspritesFH, "	";
	Print #metaspritesFH, "0, 0, 255, 0, 8, 0, 255, 0, 0, 8, 255, 0, 8, 8, 255, 0, 128"
	Print #metaspritesFH, "};"
	Print #metaspritesFH, ""

	Print #metaspritesFH, "const unsigned char * const spr_enems0 [] = {"
	Print #metaspritesFH, "	";
	Print #metaspritesFH, "spr_en1_1, spr_en1_2, spr_en1_3, spr_en1_4, spr_en1_5, spr_en1_6, spr_en1_7, spr_en1_8"
	Print #metaspritesFH, "};"
	Print #metaspritesFH, ""
	Print #metaspritesFH, "const unsigned char * const spr_enems1 [] = {"
	Print #metaspritesFH, "	";	
	Print #metaspritesFH, "spr_en2_1, spr_en2_2, spr_en2_3, spr_en2_4, spr_en2_5, spr_en2_6, spr_en2_7, spr_en2_8"
	Print #metaspritesFH, "};"
	Print #metaspritesFH, ""
	Print #metaspritesFH, "const unsigned char * const spr_player0 [] = {"
	Print #metaspritesFH, "	";	
	Print #metaspritesFH, "spr_pl1_1, spr_pl1_2, spr_pl1_3, spr_pl1_4, spr_pl1_5, spr_pl1_6, spr_pl1_7, spr_pl1_8"
	Print #metaspritesFH, "};"
	Print #metaspritesFH, ""
	Print #metaspritesFH, "const unsigned char * const spr_player1 [] = {"
	Print #metaspritesFH, "	";	
	Print #metaspritesFH, "spr_pl2_1, spr_pl2_2, spr_pl2_3, spr_pl2_4, spr_pl2_5, spr_pl2_6, spr_pl2_7, spr_pl2_8"
	Print #metaspritesFH, "};"
	Print #metaspritesFH, ""
End If 

Close metaspritesFH

Puts "Saving " & output_fn
png_save (output_fn, imgOut)
Puts "Done"
