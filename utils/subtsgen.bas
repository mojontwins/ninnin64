'' Subtileset generator

'' MK1_NES 0.1 introduced the concept of "simple tilesetmaps" where
'' 16 metatiles metatilesets were made seelcting 16 metatiles from the
'' full 48 global metatileset. Metatiles were still not pattern
'' mapped, in the sense that metatile N was still made of patterns
'' 64+N*4 and the next three patterns.

'' This tools takes the full 48 metatiles global metatileset and a 
'' list of 16 numbers to generate a new partial, "submetatileset"

#include "fbpng.bi"
#include "fbgfx.bi"
#include once "crt.bi"

Sub usage
	Puts "$ subtsgen.exe ts.png list.txt sts_out.png"
	Puts ""
	Puts "ts.png is considered to contain 16x3 16x16 pixels metatiles"
	Puts "list.txt should contain 16 comma separated values. Missing will be 0"
	Puts "sts_out.png will contain the output"
End Sub

Dim As String ts_fn, list_fn, out_fn
Dim As Integer fin
Dim As Integer x, y, i
Dim As Integer tslist (15)
Dim As FB.image ptr imgOut
Dim As FB.image ptr imgIn

'' Needs to have an internal pixel format
Screenres 640, 480, 32, , -1

'' Read command line arguments
If Command (3) = "" Then usage: End

ts_fn =   Command (1)
list_fn = Command (2)
out_fn =  Command (3)

imgOut = ImageCreate (256, 16, &HFF000000)

'' Read image
imgIn = png_load (ts_fn)

'' Read list
fin = FreeFile
Open list_fn For Input As #fin
For i = 0 To 15
	Input #fin, tslist (i)
Next i
Close #fin

'' Generating mts
For i = 0 To 15
	x = (tslist (i) Mod 16) * 16
	y = (tslist (i) \ 16) * 16
	Put imgOut, (i * 16, 0), imgIn, (x, y) - (x + 15, y + 15), Pset
Next i

'' Save
png_save (out_fn, imgOut)

Puts "DONE"