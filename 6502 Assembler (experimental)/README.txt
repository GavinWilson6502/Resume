This folder contains the source code and compiled executable for an experimental 6502 assembler I made.
For purposes of experimentation, I deliberately avoided some major optimizations I could have made to the source code. Hopefully the comments make parsing it easy enough.
Also due to this project's experimental nature, I didn't write documentation for it. However, included below is some information that may be of use.


SYNTAX:
If the first character on a line is none of the following:
1. a tab character
2. a semicolon
3. end of line
then it is interpreted as the beginning of a label, which continues up to and excluding the first of the above to be encountered
Labels may contain alphanumeric characters and underscores, but may not begin with numerals
If a label is present, so must be an instruction or pseudooperation
Instructions and pseudooperations must immediately follow tab characters, and may be followed by any sequence of space and/or tab characters
For usage and special cases of instructions and pseudooperations, see 6502assembler.c
A semicolon indicates the beginning of a comment, which continues to the end of the line.

USAGE:
To assemble a hardware-compatible binary from 6502 assembly code, execute 6502assembler.exe from a command line with the following arguments, given in the order shown:
1. the file from which the source code will be assembled
2. the file to which the assembled binary will be saved
