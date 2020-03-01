@echo off
echo Exporting chr
cd ..\gfx
..\utils\nescnv tileset-import.png
copy tileset.chr ..\dev\tileset0.chr
del tileset.chr
echo Copying enems
cd ..\enems
..\utils\eneexp3c level0.ene ..\dev\assets\enems0.h 0 0 gencounter
..\utils\eneexp3c level1.ene ..\dev\assets\enems1.h 1 0 gencounter
..\utils\eneexp3c level2.ene ..\dev\assets\enems2.h 2 0 gencounter
..\utils\eneexp3c level3.ene ..\dev\assets\enems3.h 3 0 gencounter
..\utils\eneexp3c level4.ene ..\dev\assets\enems4.h 4 0 gencounter
..\utils\eneexp3c level5.ene ..\dev\assets\enems5.h 5 0 gencounter
..\utils\eneexp3c level6.ene ..\dev\assets\enems6.h 6 0 gencounter
..\utils\eneexp3c level7.ene ..\dev\assets\enems7.h 7 0 gencounter
..\utils\eneexp3c level8.ene ..\dev\assets\enems8.h 8 0 gencounter
..\utils\eneexp3c level9.ene ..\dev\assets\enems9.h 9 0 gencounter
..\utils\eneexp3c levelA.ene ..\dev\assets\enemsA.h A 0 gencounter
..\utils\eneexp3c levelB.ene ..\dev\assets\enemsB.h B 0 gencounter
echo Making map
cd ..\map
..\utils\rle44mapchrrom.exe in=maplist.txt bin=..\dev\work\mapchr.bin out=..\dev\assets\chr_rom_maps.h chr=2
cd ..\dev
..\utils\cmpfixed2 ..\map\fixed.map ..\map\pallist.txt assets\screens.h 2

..\utils\fillto.exe tileset1.chr 8192
copy work\mapchr.bin.2 tileset2.chr 
..\utils\fillto.exe tileset3.chr 8192

del game.s
..\cc\cc65 -Oi game.c --add-source
..\cc\ca65 crt0.s -D CNROM=1
..\cc\ca65 game.s
..\cc\ld65 -v -C nes-CNROM.cfg -o cart.nes crt0.o game.o runtime.lib -m labels.txt -vm
del *.o
