turboEMU
========

turboEMU is a PC Engine/TurboGrafx-16 emulator, it should run on Linux/Windows.

This emulator is under development and has many bugs and incomplete features, for now some parts are near done.
* HuC6280 can execute all instructions (accuracy is still very bad yet and needs some bugs fixed).
* MMU can handle memory between the HuC6280/HuC6270, however it still needs some improvements and functions to be done.
* HuC6270 was started, however it still needs many functions to work (i.e DMA Channel for SATB).


-- Dependencies --

If you want to build this project, you will need SDL 2.0 installed. 

This project is licensed under the terms of the MIT license.

-- Thanks / Documentation --
* EmuDocs: http://emu-docs.org/PC%20Engine/pce_doc/
* EmuViews / Charles MacDonald: http://cgfm2.emuviews.com/txt/pcetech.txt
* Magic Engine: http://www.magicengine.com/mkit/
* Emulatronia : http://www.emulatronia.com/doctec/consolas/pce/vdcdox.txt