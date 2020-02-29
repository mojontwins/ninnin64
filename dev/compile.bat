@echo off
echo Exporting chr
cd ..\gfx
..\utils\nescnv tileset-import.png
copy tileset.chr ..\dev
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
echo Making map
cd ..\dev 
..\utils\rle44mapMK1 ..\map\level0.map assets\mapa0.h 10 2 15 0 0 scrsizes nodecos
..\utils\rle44mapMK1 ..\map\level1.map assets\mapa1.h 8 3 15 1 0 scrsizes nodecos
..\utils\rle44mapMK1 ..\map\level2.map assets\mapa2.h 8 3 15 2 0 scrsizes nodecos
..\utils\rle44mapMK1 ..\map\level3.map assets\mapa3.h 8 3 15 3 0 scrsizes nodecos
..\utils\rle44mapMK1 ..\map\level4.map assets\mapa4.h 12 1 99 4 0 scrsizes nodecos
..\utils\rle44mapMK1 ..\map\level5.map assets\mapa5.h 12 1 99 5 0 scrsizes nodecos
..\utils\rle44mapMK1 ..\map\level6.map assets\mapa6.h 4 4 15 6 0 scrsizes nodecos
..\utils\rle44mapMK1 ..\map\level7.map assets\mapa7.h 4 6 15 7 0 scrsizes nodecos
..\utils\rle44mapMK1 ..\map\level8.map assets\mapa8.h 10 2 99 8 0 scrsizes nodecos
..\utils\rle44mapMK1 ..\map\level9.map assets\mapa9.h 1 10 99 9 1 scrsizes nodecos
..\utils\rle44mapMK1 ..\map\levelA.map assets\mapaA.h 2 10 99 A 1 scrsizes nodecos
..\utils\cmpfixed2 ..\map\fixed.map ..\map\pallist.txt assets\screens.h 2
del game.s
..\cc\cc65 -Oi game.c --add-source
..\cc\ca65 crt0.s
..\cc\ca65 game.s
..\cc\ld65 -v -C nes.cfg -o cart.nes crt0.o game.o runtime.lib -m labels.txt -vm
del *.o
