This folder contains the files associated with my ongoing development of a Nintendo Entertainment System game, as part of my experiments to reverse-engineer the hardware.
The file "buildconfig.txt" contains information regarding the environment on which the game is to run.
The files "nesprg.txt" and "neschr.txt" contain assembly code, and "nesprg.bin" and "neschr.bin" are binaries assembled from their respective source code files using the 6502 assembler located elsewhere in this repository, in the "6502 Assembler (experimental)" directory.
Program data is found in "nesprg.txt" and "nesprg.bin", while "neschr.txt" and "neschr.bin" contain graphical character data.
The game can be run on physical hardware by storing the binaries "nesprg.bin" and "neschr.bin" on corresponding ROM chips in an NES cartridge, and ensuring the hardware matches the information in "buildconfig.txt".
The file "nesgame.nes" is an emulator-compatible NES 2.0 file generated from "nesprg.bin", "neschr.bin", and "buildconfig.txt" using the program "nes2builder.exe".
To generate emulator-compatible NES 2.0 files, execute "nes2builder.exe" from a command line with the following arguments, given in the order shown:
1. the file from which to draw program data ("nesprg.bin" in this case)
2. the file from which to draw character data ("neschr.bin" in this case)
3. the file from which to read configuration settings ("buildconfig.txt" in this case)
4. the file to which the result will be saved ("nesgame.nes" in this case)
The source code for "nes2builder.exe" is included in "nes2builder.c". Note that this code was not optimized due to the experimental and unfinished nature of this project.
