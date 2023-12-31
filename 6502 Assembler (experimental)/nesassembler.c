#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define NUM_INSTRUCTIONS 56
#define MNEMONIC_LEN 3	//excludes null terminator
#define NUM_PSEUDOS 3

char *mnemonics[] = {	//must be sorted alphabetically
    "ADC",
    "AND",
    "ASL",
    "BCC",
    "BCS",
    "BEQ",
    "BIT",
    "BMI",
    "BNE",
    "BPL",
    "BRK",
    "BVC",
    "BVS",
    "CLC",
    "CLD",
    "CLI",
    "CLV",
    "CMP",
    "CPX",
    "CPY",
    "DEC",
    "DEX",
    "DEY",
    "EOR",
    "INC",
    "INX",
    "INY",
    "JMP",
    "JSR",
    "LDA",
    "LDX",
    "LDY",
    "LSR",
    "NOP",
    "ORA",
    "PHA",
    "PHP",
    "PLA",
    "PLP",
    "ROL",
    "ROR",
    "RTI",
    "RTS",
    "SBC",
    "SEC",
    "SED",
    "SEI",
    "STA",
    "STX",
    "STY",
    "TAX",
    "TAY",
    "TSX",
    "TXA",
    "TXS",
    "TYA"
};
int addrModes[] = {	//must match order of mnemonics
    0x1BB4,	// x xxxx xxxx xxxx
    0x1BB4,	// | |||| |||| ||||
    0x01B2,	// | |||| |||| |||+- implied
    0x0008,	// | |||| |||| ||+-- accumulator
    0x0008,	// | |||| |||| |+--- immediate
    0x0008,	// | |||| |||| +---- relative
    0x0090,	// | |||| ||||
    0x0008,	// | |||| |||+------ zero-page
    0x0008,	// | |||| ||+------- zero-page,X
    0x0008,	// | |||| |+-------- zero-page,Y
    0x0001,	// | |||| +--------- absolute
    0x0008,	// | ||||
    0x0008,	// | |||+----------- absolute,X
    0x0001,	// | ||+------------ absolute,Y
    0x0001,	// | |+------------- indirect
    0x0001,	// | +-------------- (indirect,X)
    0x0001,	// |
    0x1BB4,	// +---------------- (indirect),Y
    0x0094,
    0x0094,
    0x01B0,
    0x0001,
    0x0001,
    0x1BB4,
    0x01B0,
    0x0001,
    0x0001,
    0x0480,
    0x0080,
    0x1BB4,
    0x02D4,
    0x01B4,
    0x01B2,
    0x0001,
    0x1BB4,
    0x0001,
    0x0001,
    0x0001,
    0x0001,
    0x01B2,
    0x01B2,
    0x0001,
    0x0001,
    0x1BB4,
    0x0001,
    0x0001,
    0x0001,
    0x1BB0,
    0x00D0,
    0x00B0,
    0x0001,
    0x0001,
    0x0001,
    0x0001,
    0x0001,
    0x0001
};
char opcodes[][13] = {	//must match order of addrModes (lower bit significance <-> lower array index)
    {0x00, 0x00, 0x69, 0x00, 0x65, 0x75, 0x00, 0x6D, 0x7D, 0x79, 0x00, 0x61, 0x71},
    {0x00, 0x00, 0x29, 0x00, 0x25, 0x35, 0x00, 0x2D, 0x3D, 0x39, 0x00, 0x21, 0x31},
    {0x00, 0x0A, 0x00, 0x00, 0x06, 0x16, 0x00, 0x0E, 0x1E, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0xD0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xD8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xC9, 0x00, 0xC5, 0xD5, 0x00, 0xCD, 0xDD, 0xD9, 0x00, 0xC1, 0xD1},
    {0x00, 0x00, 0xE0, 0x00, 0xE4, 0x00, 0x00, 0xEC, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xC0, 0x00, 0xC4, 0x00, 0x00, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0xC6, 0xD6, 0x00, 0xCE, 0xDE, 0x00, 0x00, 0x00, 0x00},
    {0xCA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x49, 0x00, 0x45, 0x55, 0x00, 0x4D, 0x5D, 0x59, 0x00, 0x41, 0x51},
    {0x00, 0x00, 0x00, 0x00, 0xE6, 0xF6, 0x00, 0xEE, 0xFE, 0x00, 0x00, 0x00, 0x00},
    {0xE8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xC8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4C, 0x00, 0x00, 0x6C, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xA9, 0x00, 0xA5, 0xB5, 0x00, 0xAD, 0xBD, 0xB9, 0x00, 0xA1, 0xB1},
    {0x00, 0x00, 0xA2, 0x00, 0xA6, 0x00, 0xB6, 0xAE, 0x00, 0xBE, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xA0, 0x00, 0xA4, 0xB4, 0x00, 0xAC, 0xBC, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x4A, 0x00, 0x00, 0x46, 0x56, 0x00, 0x4E, 0x5E, 0x00, 0x00, 0x00, 0x00},
    {0xEA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x09, 0x00, 0x05, 0x15, 0x00, 0x0D, 0x1D, 0x19, 0x00, 0x01, 0x11},
    {0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x68, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x2A, 0x00, 0x00, 0x26, 0x36, 0x00, 0x2E, 0x3E, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x6A, 0x00, 0x00, 0x66, 0x76, 0x00, 0x6E, 0x7E, 0x00, 0x00, 0x00, 0x00},
    {0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xE9, 0x00, 0xE5, 0xF5, 0x00, 0xED, 0xFD, 0xF9, 0x00, 0xE1, 0xF1},
    {0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x85, 0x95, 0x00, 0x8D, 0x9D, 0x99, 0x00, 0x81, 0x91},
    {0x00, 0x00, 0x00, 0x00, 0x86, 0x00, 0x96, 0x8E, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x84, 0x94, 0x00, 0x8C, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xA8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xBA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x9A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};
enum pIndices {	//must be sorted alphabetically
    BYTE = 0,
    ORG = 1,
    WORD = 2
};
char *pseudos[] = {	//must match order of pIndices
    "BYTE",
    "ORG",
    "WORD"
};
bool labelable[] = {	//must match order of pIndices
    true,
    false,
    true
};

//TODO: potential problem with the for loops that check if constant/label names conflict with reserved/taken keywords/names.
//	I don't remember what the issue was, though
//TODO: maybe give a warning when NMI, RES, or IRQ vectors are not explicitly set up

int main (int argc, char *argv[]) {
    if (argc < 2) {
        printf("Required argument not found: file to assemble\n");
        return EXIT_FAILURE;
    }
    if (argc < 3) {
        printf("Required argument not found: output filename\n");
        return EXIT_FAILURE;
    }
    FILE *asm = fopen(argv[1], "r");
    if (asm == NULL) {
        printf("Failed to open input file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    
    int org = 0;
    char c;
    int constc = 0;
    int labelc = 0;
    
    int linenum = 1;
    int lineind = 0;
    //while loop to count constants
    while ((c = getc(asm)) != EOF) {
        if (c == ';' || c == '\n' || c == EOF) {	//line immediately ends
            while (c != '\n' && c != EOF) c = getc(asm);
            ++linenum;
            lineind = 0;
            continue;
        }
        if (c == '\t') {	//line begins with tab
            c = getc(asm);
            ++lineind;
            if (c == ';' || c == '\n' || c == EOF) {	//tab is immediately followed by end of line
                while (c != '\n' && c != EOF) c = getc(asm);
                ++linenum;
                lineind = 0;
                continue;
            }
            if (c == '.') {	//tab is immediately followed by pseudoinstruction
                fseek(asm, 0 - lineind - 1, SEEK_CUR);
                lineind = 0;
                break;
            }
            
            if (toupper(c) == 'A' || toupper(c) == 'X' || toupper(c) == 'Y') {
                c = getc(asm);
                if (c == ',' || c == ' ' || c == '\t' || c == ';' || c == '\n' || c == EOF) {
                    printf("Error on line %d: reserved keyword cannot be used as constant name\n", linenum);
                    return EXIT_FAILURE;
                }
                fseek(asm, -2, SEEK_CUR);
                c = getc(asm);
            }
            
            int tokenStart = lineind;
            int kStart = 0;
            int kEnd = NUM_INSTRUCTIONS;
            int pStart = 0;
            int pEnd = NUM_PSEUDOS;
            while (c != ' ' && c != ';' && c != '\n' && c != EOF) {
                if ((lineind == tokenStart || (c < '0' || c > '9')) && (toupper(c) < 'A' || toupper(c) > 'Z') && c != '_') {
                    printf("Error on line %d: invalid constant name\n", linenum);
                    return EXIT_FAILURE;
                }
                if (lineind - tokenStart < MNEMONIC_LEN) {
                    int i;
                    for (i = kStart; i < kEnd; ++i) {
                        if (toupper(c) == mnemonics[i][lineind - tokenStart]) {
                            kStart = i;
                            for (int j = i + 1; j < kEnd; ++j) {
                                if (toupper(c) != mnemonics[j][lineind - tokenStart]) kEnd = j;
                            }
                            break;
                        }
                    }
                    if (i == kEnd) kEnd = 0;
                }
                int i;
                for (i = pStart; i < pEnd; ++i) {
                    int k;
                    for (k = 0; pseudos[i][k] != '\0'; ++k);
                    if (lineind - tokenStart < k && toupper(c) == pseudos[i][lineind - tokenStart]) {
                        pStart = i;
                        for (int j = i + 1; j < pEnd; ++j) {
                            if (toupper(c) != pseudos[j][lineind - tokenStart]) pEnd = j;
                        }
                        break;
                    }
                }
                if (i == pEnd) pEnd = 0;
                c = getc(asm);
                ++lineind;
            }
            if (pEnd > 0 && pEnd - pStart == 1) {
                int k;
                for (k = 0; pseudos[pStart][k] != '\0'; ++k);
                if (lineind - tokenStart == k) {
                    printf("Error on line %d: reserved keyword cannot be used as constant name\n", linenum);
                    return EXIT_FAILURE;
                }
            }
            if (kEnd > 0 && kEnd - kStart == 1 && lineind - tokenStart == MNEMONIC_LEN) {	//tab is immediately followed by instruction mnemonic
                fseek(asm, 0 - lineind - 1, SEEK_CUR);
                lineind = 0;
                break;
            }
            
            if (c != ' ') {
                printf("Error on line %d: expected definition for constant\n", linenum);
                return EXIT_FAILURE;
            }
            while (c == ' ') {
                c = getc(asm);
                ++lineind;
            }
            if (c != '=') {
                printf("Error on line %d: expected '=' in constant definition\n", linenum);
                return EXIT_FAILURE;
            }
            c = getc(asm);
            ++lineind;
            if (c != ' ') {
                printf("Error on line %d: improper token separation\n", linenum);
                return EXIT_FAILURE;
            }
            while (c == ' ') {
                c = getc(asm);
                ++lineind;
            }
            
            if (c == '-') {
                c = getc(asm);
                ++lineind;
                if (c < '0' || c > '9') {
                    printf("Error on line %d: invalid decimal number\n", linenum);
                    return EXIT_FAILURE;
                }
                while (c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                    if (c < '0' || c > '9') {
                        printf("Error on line %d: invalid decimal number\n", linenum);
                        return EXIT_FAILURE;
                    }
                    c = getc(asm);
                    ++lineind;
                }
            }
            else if (c >= '1' && c <= '9') {
                while (c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                    if (c < '0' || c > '9') {
                        printf("Error on line %d: invalid decimal number\n", linenum);
                        return EXIT_FAILURE;
                    }
                    c = getc(asm);
                    ++lineind;
                }
            }
            else if (c == '0') {
                while (c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                    if (c < '0' || c > '7') {
                        printf("Error on line %d: invalid octal number\n", linenum);
                        return EXIT_FAILURE;
                    }
                    c = getc(asm);
                    ++lineind;
                }
            }
            else if (c == '$') {
                c = getc(asm);
                ++lineind;
                if ((c < '0' || c > '9') && (toupper(c) < 'A' || toupper(c) > 'F')) {
                    printf("Error on line %d: invalid hexadecimal number\n", linenum);
                    return EXIT_FAILURE;
                }
                while (c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                    if ((c < '0' || c > '9') && (toupper(c) < 'A' || toupper(c) > 'F')) {
                        printf("Error on line %d: invalid hexadecimal number\n", linenum);
                        return EXIT_FAILURE;
                    }
                    c = getc(asm);
                    ++lineind;
                }
            }
            else if (c == '%') {
                c = getc(asm);
                ++lineind;
                if (c != '0' && c != '1') {
                    printf("Error on line %d: invalid binary number\n", linenum);
                    return EXIT_FAILURE;
                }
                while (c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                    if (c != '0' && c != '1') {
                        printf("Error on line %d: invalid binary number\n", linenum);
                        return EXIT_FAILURE;
                    }
                    c = getc(asm);
                    ++lineind;
                }
            }
            else {
                printf("Error on line %d: invalid number\n", linenum);
                return EXIT_FAILURE;
            }
            
            while (c == ' ' || c == '\t') c = getc(asm);
            if (c != ';' && c != '\n' && c != EOF) {
                printf("Error on line %d: unexpected token\n", linenum);
                return EXIT_FAILURE;
            }
            
            ++constc;
            
            while (c != '\n' && c != EOF) c = getc(asm);
            ++linenum;
            lineind = 0;
            continue;
        }
        else break;	//line begins with label
    }
    //while loop to count labels - don't reset linenum or lineind
    while ((c = getc(asm)) != EOF) {
        if (c == '\t' || c == ';' || c == '\n' || c == EOF) {
            while (c != '\n' && c != EOF) c = getc(asm);
            ++linenum;
            lineind = 0;
            continue;
        }
        
        if (toupper(c) == 'A' || toupper(c) == 'X' || toupper(c) == 'Y') {
            c = getc(asm);
            if (c == ',' || c == ' ' || c == '\t' || c == ';' || c == '\n' || c == EOF) {
                printf("Error on line %d: reserved keyword cannot be used as label name\n", linenum);
                return EXIT_FAILURE;
            }
            fseek(asm, -2, SEEK_CUR);
            c = getc(asm);
        }
        
        int kStart = 0;
        int kEnd = NUM_INSTRUCTIONS;
        int pStart = 0;
        int pEnd = NUM_PSEUDOS;
        while (c != '\t' && c != ';' && c != '\n' && c != EOF) {
            if ((lineind == 0 || (c < '0' || c > '9')) && (toupper(c) < 'A' || toupper(c) > 'Z') && c != '_') {
                printf("Error on line %d: invalid label name\n", linenum);
                return EXIT_FAILURE;
            }
            if (lineind < MNEMONIC_LEN) {
                int i;
                for (i = kStart; i < kEnd; ++i) {
                    if (toupper(c) == mnemonics[i][lineind]) {
                        kStart = i;
                        for (int j = i + 1; j < kEnd; ++j) {
                            if (toupper(c) != mnemonics[j][lineind]) kEnd = j;
                        }
                        break;
                    }
                }
                if (i == kEnd) kEnd = 0;
            }
            int i;
            for (i = pStart; i < pEnd; ++i) {
                int k;
                for (k = 0; pseudos[i][k] != '\0'; ++k);
                if (lineind < k && toupper(c) == pseudos[i][lineind]) {
                    pStart = i;
                    for (int j = i + 1; j < pEnd; ++j) {
                        if (toupper(c) != pseudos[j][lineind]) pEnd = j;
                    }
                    break;
                }
            }
            if (i == pEnd) pEnd = 0;
            c = getc(asm);
            ++lineind;
        }
        if (pEnd > 0 && pEnd - pStart == 1) {
            int k;
            for (k = 0; pseudos[pStart][k] != '\0'; ++k);
            if (lineind == k) {
                printf("Error on line %d: reserved keyword cannot be used as label name\n", linenum);
                return EXIT_FAILURE;
            }
        }
        if (kEnd > 0 && kEnd - kStart == 1 && lineind == MNEMONIC_LEN) {
            printf("Error on line %d: reserved keyword cannot be used as label name\n", linenum);
            return EXIT_FAILURE;
        }
        
        if (c != '\t') {
            printf("Error on line %d: standalone label\n", linenum);
            return EXIT_FAILURE;
        }
        c = getc(asm);
        ++lineind;
        if (c == '\t') {
            while (c == '\t') c = getc(asm);
            if (c != ';' && c != '\n' && c != EOF) {
                printf("Error on line %d: indentation error\n", linenum);
                return EXIT_FAILURE;
            }
            printf("Error on line %d: standalone label\n", linenum);
            return EXIT_FAILURE;
        }
        if (c == ';' || c == '\n' || c == EOF) {
            printf("Error on line %d: standalone label\n", linenum);
            return EXIT_FAILURE;
        }
        if (c == '.') {
            c = getc(asm);
            ++lineind;
            
            int tokenStart = lineind;
            pStart = 0;
            pEnd = NUM_PSEUDOS;
            while (c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                int i;
                for (i = pStart; i < pEnd; ++i) {
                    int k;
                    for (k = 0; pseudos[i][k] != '\0'; ++k);
                    if (lineind - tokenStart < k && toupper(c) == pseudos[i][lineind - tokenStart]) {
                        pStart = i;
                        for (int j = i + 1; j < pEnd; ++j) {
                            if (toupper(c) != pseudos[j][lineind - tokenStart]) pEnd = j;
                        }
                        break;
                    }
                }
                if (i == pEnd) pEnd = 0;
                c = getc(asm);
                ++lineind;
            }
            if (pEnd > 0 && pEnd - pStart == 1) {
                int k;
                for (k = 0; pseudos[pStart][k] != '\0'; ++k);
                if (lineind - tokenStart == k && !labelable[pStart]) {
                    printf("Error on line %d: standalone label\n", linenum);
                    return EXIT_FAILURE;
                }
            }
        }
        
        ++labelc;
        
        while (c != '\n' && c != EOF) c = getc(asm);
        ++linenum;
        lineind = 0;
        continue;
    }
    char *constk[constc];	//constant keys (names)
    int constv[constc];		//constant values
    int consti = 0;
    char *labelk[labelc];	//label keys (names)
    int labelv[labelc];		//label values
    int labeli = 0;
    fseek(asm, 0, SEEK_SET);
    
    linenum = 1;
    lineind = 0;
    //while loop to fill constk & constv
    while ((c = getc(asm)) != EOF) {
        if (c == ';' || c == '\n' || c == EOF) {	//line immediately ends
            while (c != '\n' && c != EOF) c = getc(asm);
            ++linenum;
            lineind = 0;
            continue;
        }
        if (c == '\t') {	//line begins with tab
            c = getc(asm);
            ++lineind;
            if (c == ';' || c == '\n' || c == EOF) {	//tab is immediately followed by end of line
                while (c != '\n' && c != EOF) c = getc(asm);
                ++linenum;
                lineind = 0;
                continue;
            }
            if (c == '.') {	//tab is immediately followed by pseudoinstruction
                fseek(asm, 0 - lineind - 1, SEEK_CUR);
                lineind = 0;
                break;
            }
            
            int tokenStart = lineind;
            int kStart = 0;
            int kEnd = NUM_INSTRUCTIONS;
            while (c != ' ' && c != ';' && c != '\n' && c != EOF) {
                if (lineind - tokenStart < MNEMONIC_LEN) {
                    int i;
                    for (i = kStart; i < kEnd; ++i) {
                        if (toupper(c) == mnemonics[i][lineind - tokenStart]) {
                            kStart = i;
                            for (int j = i + 1; j < kEnd; ++j) {
                                if (toupper(c) != mnemonics[j][lineind - tokenStart]) kEnd = j;
                            }
                            break;
                        }
                    }
                    if (i == kEnd) kEnd = 0;
                }
                c = getc(asm);
                ++lineind;
            }
            if (kEnd > 0 && lineind - tokenStart == MNEMONIC_LEN && kEnd - kStart == 1) {	//tab is immediately followed by instruction mnemonic
                fseek(asm, 0 - lineind - 1, SEEK_CUR);
                lineind = 0;
                break;
            }
            fseek(asm, tokenStart - lineind - 1, SEEK_CUR);
            c = getc(asm);
            lineind = tokenStart;
            int i;
            for (i = 0; i < consti; ++i) {
                 while (c != ' ' && c != ';' && c != '\n' && c != EOF) {
                     if (constk[i][lineind - tokenStart] == '\0' || c != constk[i][lineind - tokenStart]) break;
                     c = getc(asm);
                     ++lineind;
                 }
                 if (c == ' ' || c == ';' || c == '\n' || c == EOF) {
                     if (constk[i][lineind - tokenStart] == '\0') break;
                 }
                 fseek(asm, tokenStart - lineind - 1, SEEK_CUR);
                 c = getc(asm);
                 lineind = tokenStart;
            }
            if (i < consti) {
                printf("Error on line %d: constant name already taken\n", linenum);
                return EXIT_FAILURE;
            }
            while (c != ' ' && c != ';' && c != '\n' && c != EOF) {
                c = getc(asm);
                ++lineind;
            }
            fseek(asm, tokenStart - lineind - 1, SEEK_CUR);
            constk[consti] = malloc(sizeof(char) * (lineind - tokenStart + 1));
            for (int i = 0; i < lineind - tokenStart; ++i) constk[consti][i] = getc(asm);
            constk[consti][lineind - tokenStart] = '\0';
            c = getc(asm);
            
            while (c == ' ') {
                c = getc(asm);
                ++lineind;
            }
            c = getc(asm);
            ++lineind;
            while (c == ' ') {
                c = getc(asm);
                ++lineind;
            }
            
            int total = 0;
            if (c == '-') {
                c = getc(asm);
                ++lineind;
                while (c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                    total = total * 10 - (c - '0');
                    c = getc(asm);
                    ++lineind;
                }
            }
            else if (c >= '1' && c <= '9') {
                while (c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                    total = total * 10 + (c - '0');
                    c = getc(asm);
                    ++lineind;
                }
            }
            else if (c == '0') {
                while (c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                    total = total * 8 + (c - '0');
                    c = getc(asm);
                    ++lineind;
                }
            }
            else if (c == '$') {
                c = getc(asm);
                ++lineind;
                while (c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                    if (c >= '0' && c <= '9') total = total * 16 + (c - '0');
                    else total = total * 16 + 10 + (toupper(c) - 'A');
                    c = getc(asm);
                    ++lineind;
                }
            }
            else if (c == '%') {
                c = getc(asm);
                ++lineind;
                while (c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                    total = total * 2 + (c - '0');
                    c = getc(asm);
                    ++lineind;
                }
            }
            constv[consti] = total;
            
            ++consti;
            
            while (c != '\n' && c != EOF) c = getc(asm);
            ++linenum;
            lineind = 0;
            continue;
        }
        else {	//line begins with label
            fseek(asm, -1, SEEK_CUR);
            break;
        }
    }
    int endDef = ftell(asm);
    int endDefLinenum = linenum;
    //while loop to count bytes and fill labelk & labelv - don't reset linenum or lineind
    while ((c = getc(asm)) != EOF) {
        if (c == ';' || c == '\n' || c == EOF) {	//line immediately ends
            while (c != '\n' && c != EOF) c = getc(asm);
            ++linenum;
            lineind = 0;
            continue;
        }
        if (c != '\t') {	//line begins with label
            int i;
            for (i = 0; i < constc; ++i) {
                while (c != '\t' && c != ';' && c != '\n' && c != EOF) {
                    if (constk[i][lineind] == '\0' || c != constk[i][lineind]) break;
                    c = getc(asm);
                    ++lineind;
                }
                if (c == '\t' || c == ';' || c == '\n' || c == EOF) {
                    if (constk[i][lineind] == '\0') break;
                }
                fseek(asm, 0 - lineind - 1, SEEK_CUR);
                c = getc(asm);
                lineind = 0;
            }
            if (i < constc) {
                printf("Error on line %d: label name already taken by constant\n", linenum);
                return EXIT_FAILURE;
            }
            fseek(asm, 0 - lineind - 1, SEEK_CUR);
            c = getc(asm);
            lineind = 0;
            for (i = 0; i < labeli; ++i) {
                while (c != '\t' && c != ';' && c != '\n' && c != EOF) {
                    if (labelk[i][lineind] == '\0' || c != labelk[i][lineind]) break;
                    c = getc(asm);
                    ++lineind;
                }
                if (c == '\t' || c == ';' || c == '\n' || c == EOF) {
                    if (labelk[i][lineind] == '\0') break;
                }
                fseek(asm, 0 - lineind - 1, SEEK_CUR);
                c = getc(asm);
                lineind = 0;
            }
            if (i < labeli) {
                printf("Error on line %d: label name already taken\n", linenum);
                return EXIT_FAILURE;
            }
            while (c != '\t' && c != ';' && c != '\n' && c != EOF) {
                c = getc(asm);
                ++lineind;
            }
            fseek(asm, 0 - lineind - 1, SEEK_CUR);
            labelk[labeli] = malloc(sizeof(char) * (lineind + 1));
            for (int i = 0; i < lineind; ++i) labelk[labeli][i] = getc(asm);
            labelk[labeli][lineind] = '\0';
            c = getc(asm);
            
            labelv[labeli] = org;
            
            ++labeli;
        }
        c = getc(asm);
        ++lineind;
        if (c == ';' || c == '\n' || c == EOF) {	//tab is immediately followed by end of line
            while (c != '\n' && c != EOF) c = getc(asm);
            ++linenum;
            lineind = 0;
            continue;
        }
        if (c == '.') {	//tab is immediately followed by pseudoinstruction
            c = getc(asm);
            ++lineind;
            
            int tokenStart = lineind;
            int pStart = 0;
            int pEnd = NUM_PSEUDOS;
            while (c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                int i;
                for (i = pStart; i < pEnd; ++i) {
                    int k;
                    for (k = 0; pseudos[i][k] != '\0'; ++k);
                    if (lineind - tokenStart < k && toupper(c) == pseudos[i][lineind - tokenStart]) {
                        pStart = i;
                        for (int j = i + 1; j < pEnd; ++j) {
                            if (toupper(c) != pseudos[j][lineind - tokenStart]) pEnd = j;
                        }
                        break;
                    }
                }
                if (i == pEnd) pEnd = 0;
                c = getc(asm);
                ++lineind;
            }
            if (pEnd > 0 && pEnd - pStart == 1) {
                int k;
                for (k = 0; pseudos[pStart][k] != '\0'; ++k);
                if (lineind - tokenStart != k) {
                    printf("Error on line %d: unrecognized pseudoinstruction\n", linenum);
                    return EXIT_FAILURE;
                }
                if (pStart == BYTE || pStart == ORG || pStart == WORD) {
                    while (c == ' ') {
                        c = getc(asm);
                        ++lineind;
                    }
                    
                    while (c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                        int total = 0;
                        if (c == '-') {
                            c = getc(asm);
                            ++lineind;
                            if (c < '0' || c > '9') {
                                printf("Error on line %d: invalid decimal number\n", linenum);
                                return EXIT_FAILURE;
                            }
                            while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                                if (c < '0' || c > '9') {
                                    printf("Error on line %d: invalid decimal number\n", linenum);
                                    return EXIT_FAILURE;
                                }
                                total = total * 10 - (c - '0');
                                c = getc(asm);
                                ++lineind;
                            }
                        }
                        else if (c >= '1' && c <= '9') {
                            while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                                if (c < '0' || c > '9') {
                                    printf("Error on line %d: invalid decimal number\n", linenum);
                                    return EXIT_FAILURE;
                                }
                                total = total * 10 + (c - '0');
                                c = getc(asm);
                                ++lineind;
                            }
                        }
                        else if (c == '0') {
                            while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                                if (c < '0' || c > '7') {
                                    printf("Error on line %d: invalid octal number\n", linenum);
                                    return EXIT_FAILURE;
                                }
                                total = total * 8 + (c - '0');
                                c = getc(asm);
                                ++lineind;
                            }
                        }
                        else if (c == '$') {
                            c = getc(asm);
                            ++lineind;
                            if ((c < '0' || c > '9') && (toupper(c) < 'A' || toupper(c) > 'F')) {
                                printf("Error on line %d: invalid hexadecimal number\n", linenum);
                                return EXIT_FAILURE;
                            }
                            while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                                if ((c < '0' || c > '9') && (toupper(c) < 'A' || toupper(c) > 'F')) {
                                    printf("Error on line %d: invalid hexadecimal number\n", linenum);
                                    return EXIT_FAILURE;
                                }
                                if (c >= '0' && c <= '9') total = total * 16 + (c - '0');
                                else total = total * 16 + 10 + (toupper(c) - 'A');
                                c = getc(asm);
                                ++lineind;
                            }
                        }
                        else if (c == '%') {
                            c = getc(asm);
                            ++lineind;
                            if (c != '0' && c != '1') {
                                printf("Error on line %d: invalid binary number\n", linenum);
                                return EXIT_FAILURE;
                            }
                            while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                                if (c != '0' && c != '1') {
                                    printf("Error on line %d: invalid binary number\n", linenum);
                                    return EXIT_FAILURE;
                                }
                                total = total * 2 + (c - '0');
                                c = getc(asm);
                                ++lineind;
                            }
                        }
                        else {
                            tokenStart = lineind;
                            int i;
                            for (i = 0; i < constc; ++i) {
                                while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                                    if (constk[i][lineind - tokenStart] == '\0' || c != constk[i][lineind - tokenStart]) break;
                                    c = getc(asm);
                                    ++lineind;
                                }
                                if (c == ',' || c == ' ' || c == '\t' || c == ';' || c == '\n' || c == EOF) {
                                    if (constk[i][lineind - tokenStart] == '\0') break;
                                }
                                fseek(asm, tokenStart - lineind - 1, SEEK_CUR);
                                c = getc(asm);
                                lineind = tokenStart;
                            }
                            if (i < constc) total = constv[i];
                            else if (pStart == ORG) {
                                printf("Error on line %d: unrecognized constant\n", linenum);
                                return EXIT_FAILURE;
                            }
                            while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                                c = getc(asm);
                                ++lineind;
                            }
                        }
                        if (pStart == BYTE) {
                            if (total < -128 || total > 255) {
                                printf("Error on line %d: argument out of 8-bit range\n", linenum);
                                return EXIT_FAILURE;
                            }
                            ++org;
                        }
                        else if (pStart == ORG || pStart == WORD) {
                            if (total < -32768 || total > 65535) {
                                printf("Error on line %d: argument out of 16-bit range\n", linenum);
                                return EXIT_FAILURE;
                            }
                            if (pStart == ORG) {
                                if (c == ',') {
                                    printf("Error on line %d: unexpected token\n", linenum);
                                    return EXIT_FAILURE;
                                }
                                total %= 65536;
                                if (total < org) printf("Warning on line %d: backwards traversal by .ORG\n", linenum);
                                org = total;
                            }
                            else if (pStart == WORD) org += 2;
                        }
                        if (c == ',') {
                            c = getc(asm);
                            ++lineind;
                        }
                    }
                }
                
                while (c == ' ' || c == '\t') c = getc(asm);
                if (c != ';' && c != '\n' && c != EOF) {
                    printf("Error on line %d: unexpected token\n", linenum);
                    return EXIT_FAILURE;
                }
                while (c != '\n' && c != EOF) c = getc(asm);
                ++linenum;
                lineind = 0;
                continue;
            }
            printf("Error on line %d: unrecognized pseudoinstruction\n", linenum);
            return EXIT_FAILURE;
        }
        
        int tokenStart = lineind;
        int kStart = 0;
        int kEnd = NUM_INSTRUCTIONS;
        while (c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
            if (lineind - tokenStart < MNEMONIC_LEN) {
                int i;
                for (i = kStart; i < kEnd; ++i) {
                    if (toupper(c) == mnemonics[i][lineind - tokenStart]) {
                        kStart = i;
                        for (int j = i + 1; j < kEnd; ++j) {
                            if (toupper(c) != mnemonics[j][lineind - tokenStart]) kEnd = j;
                        }
                        break;
                    }
                }
                if (i == kEnd) kEnd = 0;
            }
            c = getc(asm);
            ++lineind;
        }
        if (kEnd > 0 && kEnd - kStart == 1 && lineind - tokenStart == MNEMONIC_LEN) {
            ++org;
            while (c == ' ') {
                c = getc(asm);
                ++lineind;
            }
            
            int mode = 0;	//1 for immediate; 2 for (indirect), (indirect,X), or (indirect),Y; 0 for other
            if (c == ' ' || c == '\t' || c == ';' || c == '\n' || c == EOF) {	//implied or accumulator (no args)
                while (c == '\t' || c == ' ') c = getc(asm);
                if (c != ';' && c != '\n' && c != EOF) {
                    printf("Error on line %d: whitespace error\n", linenum);
                    return EXIT_FAILURE;
                }
                if ((addrModes[kStart] & 3) == 0) {
                    printf("Error on line %d: instruction does not use implied or accumulator addressing modes\n", linenum);
                    return EXIT_FAILURE;
                }
                while (c != '\n' && c != EOF) c = getc(asm);
                ++linenum;
                lineind = 0;
                continue;
            }
            if (toupper(c) == 'A') {	//accumulator (arg: A)
                c = getc(asm);
                if (c == ',' || c == ' ' || c == '\t' || c == ';' || c == '\n' || c == EOF) {
                    if ((addrModes[kStart] & 2) == 0) {
                        printf("Error on line %d: instruction does not use accumulator addressing mode\n", linenum);
                        return EXIT_FAILURE;
                    }
                    while (c == ' ' || c == '\t') c = getc(asm);
                    if (c != ';' && c != '\n' && c != EOF) {
                        printf("Error on line %d: unexpected token\n", linenum);
                        return EXIT_FAILURE;
                    }
                    while (c != '\n' && c != EOF) c = getc(asm);
                    ++linenum;
                    lineind = 0;
                    continue;
                }
                fseek(asm, -2, SEEK_CUR);
                c = getc(asm);
            }
            
            if (c == '#') {	//immediate
                if ((addrModes[kStart] & 4) == 0) {
                    printf("Error on line %d: instruction does not use immediate addressing mode\n", linenum);
                    return EXIT_FAILURE;
                }
                c = getc(asm);
                ++lineind;
                mode = 1;
            }
            else if (c == '(') {	//(indirect), (indirect,X), or (indirect),Y
                c = getc(asm);
                ++lineind;
                mode = 2;
            }
            int total = 0;
            if (c == '-') {	//all addressing modes except implied and accumulator
                c = getc(asm);
                ++lineind;
                if (c < '0' || c > '9') {
                    printf("Error on line %d: invalid decimal number\n", linenum);
                    return EXIT_FAILURE;
                }
                while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                    if (c < '0' || c > '9') {
                        printf("Error on line %d: invalid decimal number\n", linenum);
                        return EXIT_FAILURE;
                    }
                    total = total * 10 - (c - '0');
                    c = getc(asm);
                    ++lineind;
                }
            }
            else if (c >= '1' && c <= '9') {
                while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                    if (c < '0' || c > '9') {
                        printf("Error on line %d: invalid decimal number\n", linenum);
                        return EXIT_FAILURE;
                    }
                    total = total * 10 + (c - '0');
                    c = getc(asm);
                    ++lineind;
                }
            }
            else if (c == '0') {
                while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                    if (c < '0' || c > '7') {
                        printf("Error on line %d: invalid octal number\n", linenum);
                        return EXIT_FAILURE;
                    }
                    total = total * 8 + (c - '0');
                    c = getc(asm);
                    ++lineind;
                }
            }
            else if (c == '$') {
                c = getc(asm);
                ++lineind;
                if ((c < '0' || c > '9') && (toupper(c) < 'A' || toupper(c) > 'F')) {
                    printf("Error on line %d: invalid decimal number\n", linenum);
                    return EXIT_FAILURE;
                }
                while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                    if ((c < '0' || c > '9') && (toupper(c) < 'A' || toupper(c) > 'F')) {
                        printf("Error on line %d: invalid decimal number\n", linenum);
                        return EXIT_FAILURE;
                    }
                    if (c >= '0' && c <= '9') total = total * 16 + (c - '0');
                    else total = total * 16 + (toupper(c) - 'A');
                    c = getc(asm);
                    ++lineind;
                }
            }
            else if (c == '%') {
                c = getc(asm);
                ++lineind;
                if (c != '0' && c != '1') {
                    printf("Error on line %d: invalid decimal number\n", linenum);
                    return EXIT_FAILURE;
                }
                while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                    if (c != '0' && c != '1') {
                        printf("Error on line %d: invalid decimal number\n", linenum);
                        return EXIT_FAILURE;
                    }
                    total = total * 2 + (c - '0');
                    c = getc(asm);
                    ++lineind;
                }
            }
            else {
                tokenStart = lineind;
                int i;
                for (i = 0; i < constc; ++i) {
                     while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                         if (constk[i][lineind - tokenStart] == '\0' || c != constk[i][lineind - tokenStart]) break;
                         c = getc(asm);
                         ++lineind;
                     }
                     if (c == ',' || c == ')' || c == ' ' || c == '\t' || c == ';' || c == '\n' || c == EOF) {
                         if (constk[i][lineind - tokenStart] == '\0') break;
                     }
                     fseek(asm, tokenStart - lineind - 1, SEEK_CUR);
                     c = getc(asm);
                     lineind = tokenStart;
                }
                if (i < constc) total = constv[i];
                while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                    c = getc(asm);
                    ++lineind;
                }
            }
            
            if (mode == 0) {	//relative, (indexed) zero-page, or (indexed) absolute
                if (c == ',') {	//indexed zero-page or indexed absolute
                    c = getc(asm);
                    ++lineind;
                    if (toupper(c) == 'X') {	//zero-page,X or absolute,X
                        if ((addrModes[kStart] & 32) == 0) {
                            printf("Error on line %d: instruction does not use zero-page,X or absolute,X addressing modes\n", linenum);
                            return EXIT_FAILURE;
                        }
                        else if (total < -128 || total > 255) {	//absolute,X
                            if ((addrModes[kStart] & 256) == 0) {
                                printf("Error on line %d: zero-page address out of 8-bit range\n", linenum);
                                return EXIT_FAILURE;
                            }
                            if (total < -32768 || total > 65535) {
                                printf("Error on line %d: absolute address out of 16-bit range\n", linenum);
                                return EXIT_FAILURE;
                            }
                            org += 2;
                        }
                        else ++org;	//zero-page,X
                    }
                    else if (toupper(c) == 'Y') {	//zero-page,Y or absolute,Y
                        if ((addrModes[kStart] & 512) == 0) {
                            if ((addrModes[kStart] & 64) == 0) {
                                printf("Error on line %d: instruction does not use zero-page,Y or absolute,Y addressing modes\n", linenum);
                                return EXIT_FAILURE;
                            }
                            if (total < -128 || total > 255) {
                                printf("Error on line %d: zero-page address out of 8-bit range\n", linenum);
                                return EXIT_FAILURE;
                            }
                            ++org;
                        }
                        else if (total < -32768 || total > 65535) {
                            printf("Error on line %d: absolute address out of 16-bit range\n", linenum);
                            return EXIT_FAILURE;
                        }
                        else if (total < -128 || total > 255 || (addrModes[kStart] & 64) == 0) org += 2;	//absolute,Y
                        else ++org;	//zero-page,Y
                    }
                    else {
                        printf("Error on line %d: unrecognized addressing mode\n", linenum);
                        return EXIT_FAILURE;
                    }
                    c = getc(asm);
                    ++lineind;
                }
                else if ((addrModes[kStart] & 128) == 0) {	//relative, zero-page, or absolute
                    if ((addrModes[kStart] & 8) == 0) {
                        printf("Error on line %d: instruction does not use relative, zero-page, or absolute addressing modes\n", linenum);
                        return EXIT_FAILURE;
                    }
                    if (total < -128 || total > 255) {
                        printf("Error on line %d: relative address out of 8-bit range\n", linenum);
                        return EXIT_FAILURE;
                    }
                    ++org;	//relative
                }
                else if (total < -32768 || total > 65535) {
                    printf("Error on line %d: absolute address out of 16-bit range\n", linenum);
                    return EXIT_FAILURE;
                }
                else if (total < -128 || total > 255 || (addrModes[kStart] & 16) == 0) org += 2;	//absolute
                else ++org;	//zero-page
            }
            else if (mode == 1) {	//immediate
                if (total < -128 || total > 255) {
                    printf("Error on line %d: immediate value is out of 8-bit range\n", linenum);
                    return EXIT_FAILURE;
                }
                ++org;
            }
            else if (mode == 2) {	//(indirect), (indirect,X), or (indirect),Y
                if (c == ',') {		//(indirect,X)
                    c = getc(asm);
                    ++lineind;
                    if (toupper(c) != 'X') {
                        printf("Error on line %d: unrecognized addressing mode\n", linenum);
                        return EXIT_FAILURE;
                    }
                    c = getc(asm);
                    ++lineind;
                    if (c != ')') {
                        printf("Error on line %d: unrecognized addressing mode\n", linenum);
                        return EXIT_FAILURE;
                    }
                    c = getc(asm);
                    ++lineind;
                    if ((addrModes[kStart] & 2048) == 0) {
                        printf("Error on line %d: instruction does not use indexed indirect addressing mode\n", linenum);
                        return EXIT_FAILURE;
                    }
                    if (total < -128 || total > 255) {
                        printf("Error on line %d: indirect address out of 8-bit range\n", linenum);
                        return EXIT_FAILURE;
                    }
                    ++org;
                }
                else if (c == ')') {	//(indirect) or (indirect),Y
                    c = getc(asm);
                    ++lineind;
                    if (c == ',') {	//(indirect),Y
                        c = getc(asm);
                        ++lineind;
                        if (toupper(c) != 'Y') {
                            printf("Error on line %d: unrecognized addressing mode\n", linenum);
                            return EXIT_FAILURE;
                        }
                        c = getc(asm);
                        ++lineind;
                        if ((addrModes[kStart] & 4096) == 0) {
                            printf("Error on line %d: instruction does not use indirect indexed addressing mode\n", linenum);
                            return EXIT_FAILURE;
                        }
                        if (total < -128 || total > 255) {
                            printf("Error on line %d: indirect address out of 8-bit range\n", linenum);
                            return EXIT_FAILURE;
                        }
                        ++org;
                    }
                    else {	//(indirect)
                        if ((addrModes[kStart] & 1024) == 0) {
                            printf("Error on line %d: instruction does not use absolute indirect addressing mode\n", linenum);
                            return EXIT_FAILURE;
                        }
                        if (total < -32768 || total > 65535) {
                            printf("Error on line %d: indirect address out of 16-bit range\n", linenum);
                            return EXIT_FAILURE;
                        }
                        org += 2;
                    }
                }
                else {
                    printf("Error on line %d: unrecognized addressing mode\n", linenum);
                    return EXIT_FAILURE;
                }
            }
            while (c == ' ' || c == '\t') c = getc(asm);
            if (c != ';' && c != '\n' && c != EOF) {
                printf("Error on line %d: unexpected token\n", linenum);
                return EXIT_FAILURE;
            }
            while (c != '\n' && c != EOF) c = getc(asm);
            ++linenum;
            lineind = 0;
            continue;
        }
        else {
            printf("Error on line %d: unrecognized instruction\n", linenum);
            return EXIT_FAILURE;
        }
    }
    fseek(asm, endDef, SEEK_SET);
    
    org = 0;
    linenum = endDefLinenum;
    lineind = 0;
    //while loop to ensure label references are defined and valid operands
    while ((c = getc(asm)) != EOF) {
        if (c == ';' || c == '\n' || c == EOF) {	//line immediately ends
            while (c != '\n' && c != EOF) c = getc(asm);
            ++linenum;
            lineind = 0;
            continue;
        }
        while (c != '\t' && c != ';' && c != '\n' && c != EOF) {
            c = getc(asm);
            ++lineind;
        }
        c = getc(asm);
        ++lineind;
        if (c == ';' || c == '\n' || c == EOF) {	//tab is immediately followed by end of line
            while (c != '\n' && c != EOF) c = getc(asm);
            ++linenum;
            lineind = 0;
            continue;
        }
        if (c == '.') {	//tab is immediately followed by pseudoinstruction
            c = getc(asm);
            ++lineind;
            
            int tokenStart = lineind;
            int pStart = 0;
            int pEnd = NUM_PSEUDOS;
            while (c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                int i;
                for (i = pStart; i < pEnd; ++i) {
                    int k;
                    for (k = 0; pseudos[i][k] != '\0'; ++k);
                    if (lineind - tokenStart < k && toupper(c) == pseudos[i][lineind - tokenStart]) {
                        pStart = i;
                        for (int j = i + 1; j < pEnd; ++j) {
                            if (toupper(c) != pseudos[j][lineind - tokenStart]) pEnd = j;
                        }
                        break;
                    }
                }
                if (i == pEnd) pEnd = 0;
                c = getc(asm);
                ++lineind;
            }
            if (pStart == BYTE || pStart == ORG || pStart == WORD) {
                while (c == ' ') {
                    c = getc(asm);
                    ++lineind;
                }
                
                while (c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                    int total = 0;
                    if (c == '-') {
                        c = getc(asm);
                        ++lineind;
                        while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                            total = total * 10 - (c - '0');
                            c = getc(asm);
                            ++lineind;
                        }
                    }
                    else if (c >= '1' && c <= '9') {
                        while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                            total = total * 10 + (c - '0');
                            c = getc(asm);
                            ++lineind;
                        }
                    }
                    else if (c == '0') {
                        while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                            total = total * 8 + (c - '0');
                            c = getc(asm);
                            ++lineind;
                        }
                    }
                    else if (c == '$') {
                        c = getc(asm);
                        ++lineind;
                        while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                            if (c >= '0' && c <= '9') total = total * 16 + (c - '0');
                            else total = total * 16 + 10 + (toupper(c) - 'A');
                            c = getc(asm);
                            ++lineind;
                        }
                    }
                    else if (c == '%') {
                        c = getc(asm);
                        ++lineind;
                        while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                            total = total * 2 + (c - '0');
                            c = getc(asm);
                            ++lineind;
                        }
                    }
                    else {
                        tokenStart = lineind;
                        int i;
                        for (i = 0; i < constc; ++i) {
                            while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                                if (constk[i][lineind - tokenStart] == '\0' || c != constk[i][lineind - tokenStart]) break;
                                c = getc(asm);
                                ++lineind;
                            }
                            if (c == ',' || c == ' ' || c == '\t' || c == ';' || c == '\n' || c == EOF) {
                                if (constk[i][lineind - tokenStart] == '\0') break;
                            }
                            fseek(asm, tokenStart - lineind - 1, SEEK_CUR);
                            c = getc(asm);
                            lineind = tokenStart;
                        }
                        fseek(asm, tokenStart - lineind - 1, SEEK_CUR);
                        c = getc(asm);
                        lineind = tokenStart;
                        int j;
                        for (j = 0; j < labelc; ++j) {
                            while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                                if (labelk[j][lineind - tokenStart] == '\0' || c != labelk[j][lineind - tokenStart]) break;
                                c = getc(asm);
                                ++lineind;
                            }
                            if (c == ',' || c == ' ' || c == '\t' || c == ';' || c == '\n' || c == EOF) {
                                if (labelk[j][lineind - tokenStart] == '\0') break;
                            }
                            fseek(asm, tokenStart - lineind - 1, SEEK_CUR);
                            c = getc(asm);
                            lineind = tokenStart;
                        }
                        if (i < constc) total = constv[i];
                        else if (j < labelc) total = labelv[j];
                        else {
                            printf("Error on line %d: unrecognized constant or label\n", linenum);
                            return EXIT_FAILURE;
                        }
                        while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                            c = getc(asm);
                            ++lineind;
                        }
                    }
                    if (pStart == BYTE) {
                        if (total < -128 || total > 255) {
                            printf("Error on line %d: argument out of 8-bit range\n", linenum);
                            return EXIT_FAILURE;
                        }
                        ++org;
                    }
                    else if (pStart == ORG || pStart == WORD) {
                        if (total < -32768 || total > 65535) {
                            printf("Error on line %d: argument out of 16-bit range\n", linenum);
                            return EXIT_FAILURE;
                        }
                        if (pStart == ORG) org = total % 65536;
                        else if (pStart == WORD) org += 2;
                    }
                    if (c == ',') {
                        c = getc(asm);
                        ++lineind;
                    }
                }
            }
            
            while (c != '\n' && c != EOF) c = getc(asm);
            ++linenum;
            lineind = 0;
            continue;
        }
        
        int tokenStart = lineind;
        int kStart = 0;
        int kEnd = NUM_INSTRUCTIONS;
        while (c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
            if (lineind - tokenStart < MNEMONIC_LEN) {
                int i;
                for (i = kStart; i < kEnd; ++i) {
                    if (toupper(c) == mnemonics[i][lineind - tokenStart]) {
                        kStart = i;
                        for (int j = i + 1; j < kEnd; ++j) {
                            if (toupper(c) != mnemonics[j][lineind - tokenStart]) kEnd = j;
                        }
                        break;
                    }
                }
                if (i == kEnd) kEnd = 0;
            }
            c = getc(asm);
            ++lineind;
        }
        ++org;
        while (c == ' ') {
            c = getc(asm);
            ++lineind;
        }
        
        int mode = 0;	//1 for immediate; 2 for (indirect), (indirect,X), or (indirect),Y; 0 for other
        if (c == ' ' || c == '\t' || c == ';' || c == '\n' || c == EOF) {	//implied or accumulator (no args)
            while (c != '\n' && c != EOF) c = getc(asm);
            ++linenum;
            lineind = 0;
            continue;
        }
        if (toupper(c) == 'A') {	//accumulator (arg: A)
            c = getc(asm);
            if (c == ',' || c == ' ' || c == '\t' || c == ';' || c == '\n' || c == EOF) {
                while (c != '\n' && c != EOF) c = getc(asm);
                ++linenum;
                lineind = 0;
                continue;
            }
            fseek(asm, -2, SEEK_CUR);
            c = getc(asm);
        }
        
        if (c == '#') {	//immediate
            c = getc(asm);
            ++lineind;
            mode = 1;
        }
        else if (c == '(') {	//(indirect), (indirect,X), or (indirect),Y
            c = getc(asm);
            ++lineind;
            mode = 2;
        }
        int total = 0;
        bool isLabel = false;
        if (c == '-') {
            c = getc(asm);
            ++lineind;
            while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                total = total * 10 - (c - '0');
                c = getc(asm);
                ++lineind;
            }
        }
        else if (c >= '1' && c <= '9') {
            while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                total = total * 10 + (c - '0');
                c = getc(asm);
                ++lineind;
            }
        }
        else if (c == '0') {
            while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                total = total * 8 + (c - '0');
                c = getc(asm);
                ++lineind;
            }
        }
        else if (c == '$') {
            c = getc(asm);
            ++lineind;
            while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                if (c >= '0' && c <= '9') total = total * 16 + (c - '0');
                else total = total * 16 + 10 + (toupper(c) - 'A');
                c = getc(asm);
                ++lineind;
            }
        }
        else if (c == '%') {
            c = getc(asm);
            ++lineind;
            while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                total = total * 2 + (c - '0');
                c = getc(asm);
                ++lineind;
            }
        }
        else {
            tokenStart = lineind;
            int i;
            for (i = 0; i < constc; ++i) {
                 while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                     if (constk[i][lineind - tokenStart] == '\0' || c != constk[i][lineind - tokenStart]) break;
                     c = getc(asm);
                     ++lineind;
                 }
                 if (c == ',' || c == ')' || c == ' ' || c == '\t' || c == ';' || c == '\n' || c == EOF) {
                     if (constk[i][lineind - tokenStart] == '\0') break;
                 }
                 fseek(asm, tokenStart - lineind - 1, SEEK_CUR);
                 c = getc(asm);
                 lineind = tokenStart;
            }
            fseek(asm, tokenStart - lineind - 1, SEEK_CUR);
            c = getc(asm);
            lineind = tokenStart;
            int j;
            for (j = 0; j < labelc; ++j) {
                while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                     if (labelk[j][lineind - tokenStart] == '\0' || c != labelk[j][lineind - tokenStart]) break;
                     c = getc(asm);
                     ++lineind;
                 }
                 if (c == ',' || c == ')' || c == ' ' || c == '\t' || c == ';' || c == '\n' || c == EOF) {
                     if (labelk[j][lineind - tokenStart] == '\0') break;
                 }
                 fseek(asm, tokenStart - lineind - 1, SEEK_CUR);
                 c = getc(asm);
                 lineind = tokenStart;
            }
            if (i < constc) total = constv[i];
            else if (j < labelc) {
                total = labelv[j];
                isLabel = true;
            }
            else {
                printf("Error on line %d: unrecognized constant or label\n", linenum);
                return EXIT_FAILURE;
            }
            while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                c = getc(asm);
                ++lineind;
            }
        }
        
        if (mode == 0) {	//relative, (indexed) zero-page, or (indexed) absolute
            if (c == ',') {	//indexed zero-page or indexed absolute
                c = getc(asm);
                ++lineind;
                if (toupper(c) == 'X') {	//zero-page,X or absolute,X
                    if (total < -128 || total > 255) {	//absolute,X
                        if ((addrModes[kStart] & 256) == 0) {
                            printf("Error on line %d: zero-page address out of 8-bit range\n", linenum);
                            return EXIT_FAILURE;
                        }
                        if (total < -32768 || total > 65535) {
                            printf("Error on line %d: absolute address out of 16-bit range\n", linenum);
                            return EXIT_FAILURE;
                        }
                        org += 2;
                    }
                    else ++org;	//zero-page,X
                }
                else if (toupper(c) == 'Y') {	//zero-page,Y or absolute,Y
                    if ((addrModes[kStart] & 512) == 0) {
                        if (total < -128 || total > 255) {
                            printf("Error on line %d: zero-page address out of 8-bit range\n", linenum);
                            return EXIT_FAILURE;
                        }
                        ++org;
                    }
                    else if (total < -32768 || total > 65535) {
                        printf("Error on line %d: absolute address out of 16-bit range\n", linenum);
                        return EXIT_FAILURE;
                    }
                    else if (total < -128 || total > 255 || (addrModes[kStart] & 64) == 0) org += 2;	//absolute,Y
                    else ++org;	//zero-page,Y
                }
                c = getc(asm);
                ++lineind;
            }
            else if ((addrModes[kStart] & 128) == 0) {	//relative
                if (isLabel) {
                    if (total - org < -128 || total - org > 127) {
                        printf("Error on line %d: label is too far for relative addressing\n", linenum);
                        return EXIT_FAILURE;
                    }
                }
                else if (total < -128 || total > 255) {
                    printf("Error on line %d: relative address out of 8-bit range\n", linenum);
                    return EXIT_FAILURE;
                }
                ++org;
            }
            else if (total < -32768 || total > 65535) {
                printf("Error on line %d: absolute address out of 16-bit range\n", linenum);
                return EXIT_FAILURE;
            }
            else if (total < -128 || total > 255 || (addrModes[kStart] & 16) == 0) org += 2;	//absolute
            else ++org;	//zero-page
        }
        else if (mode == 1) {	//immediate
            if (total < -128 || total > 255) {
                printf("Error on line %d: immediate value is out of 8-bit range\n", linenum);
                return EXIT_FAILURE;
            }
            ++org;
        }
        else if (mode == 2) {	//(indirect), (indirect,X), or (indirect),Y
            if (c == ',') {	//(indirect,X)
                if (total < -128 || total > 255) {
                    printf("Error on line %d: indirect address out of 8-bit range\n", linenum);
                    return EXIT_FAILURE;
                }
                ++org;
            }
            else if (c == ')') {	//(indirect) or (indirect),Y
                c = getc(asm);
                ++lineind;
                if (c == ',') {	//(indirect),Y
                    if (total < -128 || total > 255) {
                        printf("Error on line %d: indirect address out of 8-bit range\n", linenum);
                        return EXIT_FAILURE;
                    }
                    ++org;
                }
                else {	//(indirect)
                    if (total < -32768 || total > 65535) {
                        printf("Error on line %d: indirect address out of 16-bit range\n", linenum);
                        return EXIT_FAILURE;
                    }
                    org += 2;
                }
            }
        }
        while (c != '\n' && c != EOF) c = getc(asm);
        ++linenum;
        lineind = 0;
        continue;
    }
    fseek(asm, endDef, SEEK_SET);
    
    //create output file
    FILE *bin = fopen(argv[2], "wb");
    if (bin == NULL) {
        printf("Failed to open output file: %s\n", argv[2]);
        return EXIT_FAILURE;
    }
    
    org = 0;
    int baseOrg = -1;
    linenum = endDefLinenum;
    lineind = 0;
    //while loop to create output file
    while ((c = getc(asm)) != EOF) {
        if (c == ';' || c == '\n' || c == EOF) {	//line immediately ends
            while (c != '\n' && c != EOF) c = getc(asm);
            ++linenum;
            lineind = 0;
            continue;
        }
        while (c != '\t' && c != ';' && c != '\n' && c != EOF) {
            c = getc(asm);
            ++lineind;
        }
        c = getc(asm);
        ++lineind;
        if (c == ';' || c == '\n' || c == EOF) {	//tab is immediately followed by end of line
            while (c != '\n' && c != EOF) c = getc(asm);
            ++linenum;
            lineind = 0;
            continue;
        }
        if (c == '.') {	//tab is immediately followed by pseudoinstruction
            c = getc(asm);
            ++lineind;
            
            int tokenStart = lineind;
            int pStart = 0;
            int pEnd = NUM_PSEUDOS;
            while (c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                int i;
                for (i = pStart; i < pEnd; ++i) {
                    int k;
                    for (k = 0; pseudos[i][k] != '\0'; ++k);
                    if (lineind - tokenStart < k && toupper(c) == pseudos[i][lineind - tokenStart]) {
                        pStart = i;
                        for (int j = i + 1; j < pEnd; ++j) {
                            if (toupper(c) != pseudos[j][lineind - tokenStart]) pEnd = j;
                        }
                        break;
                    }
                }
                if (i == pEnd) pEnd = 0;
                c = getc(asm);
                ++lineind;
            }
            if (pStart == BYTE || pStart == ORG || pStart == WORD) {
                while (c == ' ') {
                    c = getc(asm);
                    ++lineind;
                }
                
                while (c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                    int total = 0;
                    bool isLabel = false;
                    if (c == '-') {
                        c = getc(asm);
                        ++lineind;
                        while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                            total = total * 10 - (c - '0');
                            c = getc(asm);
                            ++lineind;
                        }
                    }
                    else if (c >= '1' && c <= '9') {
                        while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                            total = total * 10 + (c - '0');
                            c = getc(asm);
                            ++lineind;
                        }
                    }
                    else if (c == '0') {
                        while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                            total = total * 8 + (c - '0');
                            c = getc(asm);
                            ++lineind;
                        }
                    }
                    else if (c == '$') {
                        c = getc(asm);
                        ++lineind;
                        while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                            if (c >= '0' && c <= '9') total = total * 16 + (c - '0');
                            else total = total * 16 + 10 + (toupper(c) - 'A');
                            c = getc(asm);
                            ++lineind;
                        }
                    }
                    else if (c == '%') {
                        c = getc(asm);
                        ++lineind;
                        while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                            total = total * 2 + (c - '0');
                            c = getc(asm);
                            ++lineind;
                        }
                    }
                    else {
                        tokenStart = lineind;
                        int i;
                        for (i = 0; i < constc; ++i) {
                            while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                                if (constk[i][lineind - tokenStart] == '\0' || c != constk[i][lineind - tokenStart]) break;
                                c = getc(asm);
                                ++lineind;
                            }
                            if (c == ',' || c == ' ' || c == '\t' || c == ';' || c == '\n' || c == EOF) {
                                if (constk[i][lineind - tokenStart] == '\0') break;
                            }
                            fseek(asm, tokenStart - lineind - 1, SEEK_CUR);
                            c = getc(asm);
                            lineind = tokenStart;
                        }
                        fseek(asm, tokenStart - lineind - 1, SEEK_CUR);
                        c = getc(asm);
                        lineind = tokenStart;
                        int j;
                        for (j = 0; j < labelc; ++j) {
                            while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                                if (labelk[j][lineind - tokenStart] == '\0' || c != labelk[j][lineind - tokenStart]) break;
                                c = getc(asm);
                                ++lineind;
                            }
                            if (c == ',' || c == ' ' || c == '\t' || c == ';' || c == '\n' || c == EOF) {
                                if (labelk[j][lineind - tokenStart] == '\0') break;
                            }
                            fseek(asm, tokenStart - lineind - 1, SEEK_CUR);
                            c = getc(asm);
                            lineind = tokenStart;
                        }
                        if (i < constc) total = constv[i];
                        else if (j < labelc) total = labelv[j];
                        while (c != ',' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                            c = getc(asm);
                            ++lineind;
                        }
                    }
                    if (pStart == BYTE) {
                        if (baseOrg < 0) baseOrg = 0;
                        putc((char)total, bin);
                        ++org;
                        ++baseOrg;
                    }
                    else if (pStart == ORG) {
                        if (baseOrg < 0) {
                            org = total % 65536;
                            baseOrg = 0;
                        }
                        else {
                            int temp = total % 65536 - org;
                            if (temp < 0) fseek(bin, temp, SEEK_CUR);
                            else for (int i = 0; i < temp; ++i) putc('\0', bin);
                            org += temp;
                            baseOrg += temp;
                        }
                    }
                    else if (pStart == WORD) {
                        if (baseOrg < 0) baseOrg = 0;
                        putc((char)total, bin);
                        putc((char)(total>>8), bin);
                        org += 2;
                        baseOrg += 2;
                    }
                    if (c == ',') {
                        c = getc(asm);
                        ++lineind;
                    }
                }
            }
            
            while (c != '\n' && c != EOF) c = getc(asm);
            ++linenum;
            lineind = 0;
            continue;
        }
        
        if (baseOrg < 0) baseOrg = 0;
        int tokenStart = lineind;
        int kStart = 0;
        int kEnd = NUM_INSTRUCTIONS;
        while (c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
            if (lineind - tokenStart < MNEMONIC_LEN) {
                int i;
                for (i = kStart; i < kEnd; ++i) {
                    if (toupper(c) == mnemonics[i][lineind - tokenStart]) {
                        kStart = i;
                        for (int j = i + 1; j < kEnd; ++j) {
                            if (toupper(c) != mnemonics[j][lineind - tokenStart]) kEnd = j;
                        }
                        break;
                    }
                }
                if (i == kEnd) kEnd = 0;
            }
            c = getc(asm);
            ++lineind;
        }
        ++org;
        ++baseOrg;
        while (c == ' ') {
            c = getc(asm);
            ++lineind;
        }
        
        int mode = 0;	//1 for immediate; 2 for (indirect), (indirect,X), or (indirect),Y; 0 for other
        if (c == ' ' || c == '\t' || c == ';' || c == '\n' || c == EOF) {	//implied or accumulator (no args)
            if ((addrModes[kStart] & 1) == 1) putc(opcodes[kStart][0], bin);
            else if ((addrModes[kStart] & 2) == 2) putc(opcodes[kStart][1], bin);
            
            while (c != '\n' && c != EOF) c = getc(asm);
            ++linenum;
            lineind = 0;
            continue;
        }
        if (toupper(c) == 'A') {	//accumulator (arg: A)
            c = getc(asm);
            if (c == ',' || c == ' ' || c == '\t' || c == ';' || c == '\n' || c == EOF) {
                putc(opcodes[kStart][1], bin);
                
                while (c != '\n' && c != EOF) c = getc(asm);
                ++linenum;
                lineind = 0;
                continue;
            }
            fseek(asm, -2, SEEK_CUR);
            c = getc(asm);
        }
        
        if (c == '#') {	//immediate
            putc(opcodes[kStart][2], bin);
            
            c = getc(asm);
            ++lineind;
            mode = 1;
        }
        else if (c == '(') {	//(indirect), (indirect,X), or (indirect),Y
            c = getc(asm);
            ++lineind;
            mode = 2;
        }
        int total = 0;
        bool isLabel = false;
        if (c == '-') {
            c = getc(asm);
            ++lineind;
            while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                total = total * 10 - (c - '0');
                c = getc(asm);
                ++lineind;
            }
        }
        else if (c >= '1' && c <= '9') {
            while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                total = total * 10 + (c - '0');
                c = getc(asm);
                ++lineind;
            }
        }
        else if (c == '0') {
            while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                total = total * 8 + (c - '0');
                c = getc(asm);
                ++lineind;
            }
        }
        else if (c == '$') {
            c = getc(asm);
            ++lineind;
            while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                if (c >= '0' && c <= '9') total = total * 16 + (c - '0');
                else total = total * 16 + 10 + (toupper(c) - 'A');
                c = getc(asm);
                ++lineind;
            }
        }
        else if (c == '%') {
            c = getc(asm);
            ++lineind;
            while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                total = total * 2 + (c - '0');
                c = getc(asm);
                ++lineind;
            }
        }
        else {
            tokenStart = lineind;
            int i;
            for (i = 0; i < constc; ++i) {
                 while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                     if (constk[i][lineind - tokenStart] == '\0' || c != constk[i][lineind - tokenStart]) break;
                     c = getc(asm);
                     ++lineind;
                 }
                 if (c == ',' || c == ')' || c == ' ' || c == '\t' || c == ';' || c == '\n' || c == EOF) {
                     if (constk[i][lineind - tokenStart] == '\0') break;
                 }
                 fseek(asm, tokenStart - lineind - 1, SEEK_CUR);
                 c = getc(asm);
                 lineind = tokenStart;
            }
            fseek(asm, tokenStart - lineind - 1, SEEK_CUR);
            c = getc(asm);
            lineind = tokenStart;
            int j;
            for (j = 0; j < labelc; ++j) {
                 while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                     if (labelk[j][lineind - tokenStart] == '\0' || c != labelk[j][lineind - tokenStart]) break;
                     c = getc(asm);
                     ++lineind;
                 }
                 if (c == ',' || c == ')' || c == ' ' || c == '\t' || c == ';' || c == '\n' || c == EOF) {
                     if (labelk[j][lineind - tokenStart] == '\0') break;
                 }
                 fseek(asm, tokenStart - lineind - 1, SEEK_CUR);
                 c = getc(asm);
                 lineind = tokenStart;
            }
            if (i < constc) total = constv[i];
            else if (j < labelc) {
                total = labelv[j];
                isLabel = true;
            }
            while (c != ',' && c != ')' && c != ' ' && c != '\t' && c != ';' && c != '\n' && c != EOF) {
                c = getc(asm);
                ++lineind;
            }
        }
        
        if (mode == 0) {	//relative, (indexed) zero-page, or (indexed) absolute
            if (c == ',') {	//indexed zero-page or indexed absolute
                c = getc(asm);
                ++lineind;
                if (toupper(c) == 'X') {	//zero-page,X or absolute,X
                    if (total < -128 || total > 255) {	//absolute,X
                        putc(opcodes[kStart][8], bin);
                        
                        putc((char)total, bin);
                        putc((char)(total>>8), bin);
                        org += 2;
                        baseOrg += 2;
                    }
                    else {	//zero-page,X
                        putc(opcodes[kStart][5], bin);
                        
                        putc((char)total, bin);
                        ++org;
                        ++baseOrg;
                    }
                }
                else if (toupper(c) == 'Y') {	//zero-page,Y or absolute,Y
                    if ((addrModes[kStart] & 512) == 0) {
                        putc(opcodes[kStart][6], bin);
                        
                        putc((char)total, bin);
                        ++org;
                        ++baseOrg;
                    }
                    else if (total < -128 || total > 255 || (addrModes[kStart] & 64) == 0) {	//absolute,Y
                        putc(opcodes[kStart][9], bin);
                        
                        putc((char)total, bin);
                        putc((char)(total>>8), bin);
                        org += 2;
                        baseOrg += 2;
                    }
                    else {	//zero-page,Y
                        putc(opcodes[kStart][6], bin);
                        
                        putc((char)total, bin);
                        ++org;
                        ++baseOrg;
                    }
                }
                c = getc(asm);
                ++lineind;
            }
            else if ((addrModes[kStart] & 128) == 0) {	//relative
                ++org;
                ++baseOrg;
                
                putc(opcodes[kStart][3], bin);
                
                if (isLabel) putc((char)(total - org), bin);
                else putc((char)total, bin);
            }
            else if (total < -128 || total > 255 || (addrModes[kStart] & 16) == 0) {	//absolute
                putc(opcodes[kStart][7], bin);
                
                putc((char)total, bin);
                putc((char)(total>>8), bin);
                org += 2;
                baseOrg += 2;
            }
            else {	//zero-page
                putc(opcodes[kStart][4], bin);
                
                putc((char)total, bin);
                ++org;
                ++baseOrg;
            }
        }
        else if (mode == 1) {	//immediate
            putc((char)total, bin);
            ++org;
            ++baseOrg;
        }
        else if (mode == 2) {	//(indirect), (indirect,X), or (indirect),Y
            if (c == ',') {	//(indirect,X)
                putc(opcodes[kStart][11], bin);
                putc((char)total, bin);
                ++org;
                ++baseOrg;
            }
            else if (c == ')') {
                c = getc(asm);
                ++lineind;
                if (c == ',') {	//(indirect),Y
                    putc(opcodes[kStart][12], bin);
                    putc((char)total, bin);
                    ++org;
                    ++baseOrg;
                }
                else {	//(indirect)
                    putc(opcodes[kStart][10], bin);
                    putc((char)total, bin);
                    putc((char)(total>>8), bin);
                    org += 2;
                    baseOrg += 2;
                }
            }
        }
        while (c != '\n' && c != EOF) c = getc(asm);
        ++linenum;
        lineind = 0;
        continue;
    }
    
    fclose(bin);
    fclose(asm);
    
    return EXIT_SUCCESS;
}