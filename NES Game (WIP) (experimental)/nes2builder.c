#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define NUM_FIELDS 16
#define NUM_PRESENCE 2
#define NUM_CONSOLETYPES 5
#define NUM_MIRRORING 4
#define NUM_TIMINGS 4

enum fIndices {	//must match order of fields
    F_BATTERY = 0,
    F_CHR_NVRAM = 1,
    F_CHR_RAM = 2,
    F_CONSOLE_TYPE = 3,
    F_DEFAULT_EXPANSION_DEVICE = 4,
    F_EXTENDED_CONSOLE_TYPE = 5,
    F_MAPPER = 6,
    F_MIRRORING = 7,
    F_MISCELLANEOUS_ROMS = 8,
    F_PRG_NVRAM = 9,
    F_PRG_RAM = 10,
    F_SUBMAPPER = 11,
    F_TIMING = 12,
    F_TRAINER = 13,
    F_VS_HARDWARE_TYPE = 14,
    F_VS_PPU_TYPE = 15
};
char *fields[] = {	//must be sorted alphabetically
    "battery",
    "chr-nvram",
    "chr-ram",
    "console type",
    "default expansion device",
    "extended console type",
    "mapper",
    "mirroring",
    "miscellaneous roms",
    "prg-nvram",
    "prg-ram",
    "submapper",
    "timing",
    "trainer",
    "vs. hardware type",
    "vs. ppu type"
};
enum pIndices {	//must match order of presence
    P_NOT_PRESENT = 0,
    P_PRESENT = 1
};
char *presence[] = {	//must be sorted alphabetically
    "not present",
    "present"
};
enum cIndices {	//must match order of consoleTypes
    C_EXTENDED = 0,
    C_FAMICOM = 1,
    C_NES = 2,
    C_PC10 = 3,
    C_VS_SYSTEM = 4
};
char *consoleTypes[] = {	//must be sorted alphabetically
    "extended",
    "famicom",
    "nes",
    "pc10",
    "vs. system"
};
enum mIndices {	//must match order of mirroring
    M_FOUR_SCREEN = 0,
    M_HORIZONTAL = 1,
    M_MAPPER = 2,
    M_VERTICAL = 3
};
char *mirroring[] = {	//must be sorted alphabetically
    "four-screen",
    "horizontal",
    "mapper",
    "vertical"
};
enum tIndices {	//must match order of timings
    T_DENDY = 0,
    T_MULTIPLE = 1,
    T_NTSC = 2,
    T_PAL = 3
};
char *timings[] = {	//must be sorted alphabetically
    "dendy",
    "multiple",
    "ntsc",
    "pal"
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Required argument not found: PRG-ROM binary\n");
        return EXIT_FAILURE;
    }
    if (argc < 3) {
        printf("Required argument not found: CHR-ROM binary\n");
        return EXIT_FAILURE;
    }
    if (argc < 4) {
        printf("Required argument not found: config file\n");
        return EXIT_FAILURE;
    }
    if (argc < 5) {
        printf("Required argument not found: output filename\n");
        return EXIT_FAILURE;
    }
    FILE *prg = fopen(argv[1], "rb");
    if (prg == NULL) {
        printf("Failed to open input file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    fseek(prg, 0, SEEK_END);
    if (ftell(prg) != 0x8000) {
        printf("Incorrect PRG-ROM binary file size\n");
        return EXIT_FAILURE;
    }
    FILE *chr = fopen(argv[2], "rb");
    if (chr == NULL) {
        printf("Failed to open input file: %s\n", argv[2]);
        return EXIT_FAILURE;
    }
    fseek(chr, 0, SEEK_END);
    if (ftell(chr) != 0x2000) {
        printf("Incorrect CHR-ROM binary file size\n");
        return EXIT_FAILURE;
    }
    
    FILE *config = fopen(argv[3], "r");
    if (config == NULL) {
        printf("Failed to open config file: %s\n", argv[3]);
        return EXIT_FAILURE;
    }
    char flags6 = 0;
    char flags7 = 8;
    char mapper8 = 0;
    char prgram10 = 0;
    char chrram11 = 0;
    char timing12 = 0;
    char consoleType13 = 0;
    char miscroms14 = 0;
    char defExpDev15 = 0;
    char c;
    int lineind = 0;
    while ((c = getc(config)) != EOF) {
        if (c == '\n' || c == EOF) {
            c = getc(config);
            lineind = 0;
            continue;
        }
        int fStart = 0;
        int fEnd = NUM_FIELDS;
        while (c != ':' && c != '\n' && c != EOF) {
            int i;
            for (i = fStart; i < fEnd; ++i) {
                int k;
                for (k = 0; fields[i][k] != '\0'; ++k);
                if (lineind < k && tolower(c) == fields[i][lineind]) {
                    fStart = i;
                    for (int j = i + 1; j < fEnd; ++j) {
                        if (tolower(c) != fields[j][lineind]) fEnd = j;
                    }
                    break;
                }
            }
            if (i == fEnd) fEnd = 0;
            c = getc(config);
            ++lineind;
        }
        if (c != ':') {
            printf("Error in config file: reached end of line before encountering colon\n");
            return EXIT_FAILURE;
        }
        if (fEnd > 0 && fEnd - fStart == 1) {
            int k;
            for (k = 0; fields[fStart][k] != '\0'; ++k);
            if (lineind != k) {
                printf("Error in config file: encountered unrecognized field\n");
                return EXIT_FAILURE;
            }
            c = getc(config);
            ++lineind;
            while (c == ' ') {
                c = getc(config);
                ++lineind;
            }
            
            if (c == '\n' || c == EOF) {
                if (fStart != F_DEFAULT_EXPANSION_DEVICE) {
                    printf("Error in config file: reached end of line before encountering field value\n");
                    return EXIT_FAILURE;
                }
                lineind = 0;
                continue;
            }
            if (fStart == F_BATTERY || fStart == F_TRAINER) {
                int tokenStart = lineind;
                int pStart = 0;
                int pEnd = NUM_PRESENCE;
                while (c != '\n' && c != EOF) {
                    int i;
                    for (i = pStart; i < pEnd; ++i) {
                        int k;
                        for (k = 0; presence[i][k] != '\0'; ++k);
                        if (lineind - tokenStart < k && tolower(c) == presence[i][lineind - tokenStart]) {
                            pStart = i;
                            for (int j = i + 1; j < pEnd; ++j) {
                                if (tolower(c) != presence[j][lineind - tokenStart]) pEnd = j;
                            }
                            break;
                        }
                    }
                    if (i == pEnd) pEnd = 0;
                    c = getc(config);
                    ++lineind;
                }
                if (pEnd <= 0 || pEnd - pStart != 1) {
                    printf("Error in config file: encountered invalid field value\n");
                    return EXIT_FAILURE;
                }
                int k;
                for (k = 0; presence[pStart][k] != '\0'; ++k);
                if (lineind - tokenStart != k) {
                    printf("Error in config file: encountered invalid field value\n");
                    return EXIT_FAILURE;
                }
                if (fStart == F_BATTERY) {
                    if (pStart == P_PRESENT) flags6 |= 2;
                    else flags6 &= 253;
                }
                else if (fStart == F_TRAINER) {
                    if (pStart == P_PRESENT) flags6 |= 4;
                    else flags6 &= 251;
                }
            }
            else if (fStart == F_CONSOLE_TYPE) {	//Maybe have this set timing if it's not already set?
                int tokenStart = lineind;
                int cStart = 0;
                int cEnd = NUM_CONSOLETYPES;
                while (c != '\n' && c != EOF) {
                    int i;
                    for (i = cStart; i < cEnd; ++i) {
                        int k;
                        for (k = 0; consoleTypes[i][k] != '\0'; ++k);
                        if (lineind - tokenStart < k && tolower(c) == consoleTypes[i][lineind - tokenStart]) {
                            cStart = i;
                            for (int j = i + 1; j < cEnd; ++j) {
                                if (tolower(c) != consoleTypes[j][lineind - tokenStart]) cEnd = j;
                            }
                            break;
                        }
                    }
                    if (i == cEnd) cEnd = 0;
                    c = getc(config);
                    ++lineind;
                }
                if (cEnd <= 0 || cEnd - cStart != 1) {
                    printf("Error in config file: encountered invalid field value\n");
                    return EXIT_FAILURE;
                }
                int k;
                for (k = 0; consoleTypes[cStart][k] != '\0'; ++k);
                if (lineind - tokenStart != k) {
                    printf("Error in config file: encountered invalid field value\n");
                    return EXIT_FAILURE;
                }
                flags7 &= 252;
                if (cStart == C_EXTENDED) flags7 |= 3;
                else if (cStart == C_FAMICOM || cStart == C_NES) flags7 |= 0;
                else if (cStart == C_PC10) flags7 |= 2;
                else if (cStart == C_VS_SYSTEM) flags7 |= 1;
            }
            else if (fStart == F_MIRRORING) {
                int tokenStart = lineind;
                int mStart = 0;
                int mEnd = NUM_MIRRORING;
                while (c != '\n' && c != EOF) {
                    int i;
                    for (i = mStart; i < mEnd; ++i) {
                        int k;
                        for (k = 0; mirroring[i][k] != '\0'; ++k);
                        if (lineind - tokenStart < k && tolower(c) == mirroring[i][lineind - tokenStart]) {
                            mStart = i;
                            for (int j = i + 1; j < mEnd; ++j) {
                                if (tolower(c) != mirroring[j][lineind - tokenStart]) mEnd = j;
                            }
                            break;
                        }
                    }
                    if (i == mEnd) mEnd = 0;
                    c = getc(config);
                    ++lineind;
                }
                if (mEnd <= 0 || mEnd - mStart != 1) {
                    printf("Error in config file: encountered invalid field value\n");
                    return EXIT_FAILURE;
                }
                int k;
                for (k = 0; mirroring[mStart][k] != '\0'; ++k);
                if (lineind - tokenStart != k) {
                    printf("Error in config file: encountered invalid field value\n");
                    return EXIT_FAILURE;
                }
                flags6 &= 246;
                if (mStart == M_FOUR_SCREEN) flags6 |= 8;
                else if (mStart == M_HORIZONTAL || mStart == M_MAPPER) flags6 |= 0;
                else if (mStart == M_VERTICAL) flags6 |= 1;
            }
            else if (fStart == F_TIMING) {
                int tokenStart = lineind;
                int tStart = 0;
                int tEnd = NUM_TIMINGS;
                while (c != '\n' && c != EOF) {
                    int i;
                    for (i = tStart; i < tEnd; ++i) {
                        int k;
                        for (k = 0; timings[i][k] != '\0'; ++k);
                        if (lineind - tokenStart < k && tolower(c) == timings[i][lineind - tokenStart]) {
                            tStart = i;
                            for (int j = i + 1; j < tEnd; ++j) {
                                if (tolower(c) != timings[j][lineind - tokenStart]) tEnd = j;
                            }
                            break;
                        }
                    }
                    if (i == tEnd) tEnd = 0;
                    c = getc(config);
                    ++lineind;
                }
                if (tEnd <= 0 || tEnd - tStart != 1) {
                    printf("Error in config file: encountered invalid field value\n");
                    return EXIT_FAILURE;
                }
                int k;
                for (k = 0; timings[tStart][k] != '\0'; ++k);
                if (lineind - tokenStart != k) {
                    printf("Error in config file: encountered invalid field value\n");
                    return EXIT_FAILURE;
                }
                timing12 &= 252;
                if (tStart == T_DENDY) timing12 |= 3;
                else if (tStart == T_MULTIPLE) timing12 |= 2;
                else if (tStart == T_NTSC) timing12 |= 0;
                else if (tStart == T_PAL) timing12 |= 1;
            }
            else {
                int total = 0;
                if (c >= '0' && c <= '9') {
                    while (c != '\n' && c != EOF) {
                        if (c < '0' || c > '9') {
                            printf("Error in config file: invalid decimal number\n");
                            return EXIT_FAILURE;
                        }
                        total = total * 10 + (c - '0');
                        c = getc(config);
                        ++lineind;
                    }
                }
                else if (c == '$') {
                    c = getc(config);
                    ++lineind;
                    if ((c < '0' || c > '9') && (toupper(c) < 'A' && toupper(c) > 'F')) {
                        printf("Error in config file: invalid hexadecimal number\n");
                        return EXIT_FAILURE;
                    }
                    while (c != '\n' && c != EOF) {
                        if ((c < '0' || c > '9') && (toupper(c) < 'A' && toupper(c) > 'F')) {
                            printf("Error in config file: invalid hexadecimal number\n");
                            return EXIT_FAILURE;
                        }
                        if (c >= '0' && c <= '9') total = total * 16 + (c - '0');
                        else total = total * 16 + (toupper(c) - 'A');
                        c = getc(config);
                        ++lineind;
                    }
                }
                if (fStart == F_CHR_NVRAM) {
                    chrram11 &= 15;
                    chrram11 |= ((total & 15) << 4);
                }
                else if (fStart == F_CHR_RAM) {
                    chrram11 &= 240;
                    chrram11 |= (total & 15);
                }
                else if (fStart == F_DEFAULT_EXPANSION_DEVICE) {
                    defExpDev15 &= 192;
                    defExpDev15 |= (total & 63);
                }
                else if (fStart == F_EXTENDED_CONSOLE_TYPE) {
                    consoleType13 &= 240;
                    consoleType13 |= (total & 15);
                }
                else if (fStart == F_MAPPER) {
                    flags6 &= 15;
                    flags6 |= ((total & 15) << 4);
                    flags7 &= 15;
                    flags7 |= (total & 240);
                    mapper8 &= 240;
                    mapper8 |= ((total >> 8) & 15);
                }
                else if (fStart == F_MISCELLANEOUS_ROMS) {
                    miscroms14 &= 252;
                    miscroms14 |= (total & 3);
                }
                else if (fStart == F_PRG_NVRAM) {
                    prgram10 &= 15;
                    prgram10 |= ((total & 15) << 4);
                }
                else if (fStart == F_PRG_RAM) {
                    prgram10 &= 240;
                    prgram10 |= (total & 15);
                }
                else if (fStart == F_SUBMAPPER) {
                    mapper8 &= 15;
                    mapper8 |= ((total & 15) << 4);
                }
                else if (fStart == F_VS_HARDWARE_TYPE) {
                    consoleType13 &= 15;
                    consoleType13 |= ((total & 15) << 4);
                }
                else if (fStart == F_VS_PPU_TYPE) {
                    consoleType13 &= 240;
                    consoleType13 |= (total & 15);
                }
                else {
                    printf("Error in config file: encountered invalid field value\n");
                    return EXIT_FAILURE;
                }
            }
            lineind = 0;
            continue;
        }
        else {
            printf("Error in config file: encountered unrecognized field\n");
            return EXIT_FAILURE;
        }
    }
    
    int ext = 0;	//0 for no extension given, 1 for extension other than .nes given, 2 for .nes given
    int oi, oj = -1;
    for (oi = 0; argv[4][oi] != '\0'; ++oi) {
        if (argv[4][oi] == '.') {
            oj = oi;
            ext = 1;
        }
    }
    if (ext == 0) printf(".nes file extension will be added to output filename\n");
    else if (oi - oj != 4 || argv[4][oj + 1] != 'n' || argv[4][oj + 2] != 'e' || argv[4][oj + 3] != 's') printf("Output file extension will be changed to .nes\n");
    else ext = 2;
    
    char outname[(ext == 1 ? oj : oi) + (ext == 2 ? 0 : 4) + 1];	//includes null terminator
    int ok;
    for (ok = 0; ok < (ext == 1 ? oj : oi); ++ok) outname[ok] = argv[4][ok];
    if (ext == 2) outname[ok] = '\0';
    else {
        outname[ok] = '.';
        outname[ok + 1] = 'n';
        outname[ok + 2] = 'e';
        outname[ok + 3] = 's';
        outname[ok + 4] = '\0';
    }
    
    FILE *outfile = fopen(outname, "wb");
    if (outfile == NULL) {
        printf("Failed to open output file: %s\n", outname);
        return EXIT_FAILURE;
    }
    
    //header
    putc('N', outfile);
    putc('E', outfile);
    putc('S', outfile);
    putc((char)0x1A, outfile);
    int temp = ftell(prg);
    if (temp < 62898177) {
        if (temp % 16384 == 0) putc((char)(temp / 16384), outfile);
        else putc((char)(temp / 16384 + 1), outfile);
    }
    else {
        int i = 0;
        bool pad = false;
        while ((temp & -8) != 0) {
            if ((temp & 1) == 1) pad = true;
            temp = temp >> 1;
            temp &= 2147483647;
            ++i;
        }
        if (temp == 4) {
            temp = 1;
            i += 2;
        }
        else if (temp == 6) {
            temp = 3;
            ++i;
        }
        temp = (temp - 1) / 2;
        temp |= i << 2;
        if (pad) ++temp;
        putc((char)temp, outfile);
    }
    temp = ftell(chr);
    if (temp < 31449089) {
        if (temp % 8192 == 0) putc((char)(temp / 8192), outfile);
        else putc((char)(temp / 8192 + 1), outfile);
    }
    else {
        int i = 0;
        bool pad = false;
        while ((temp & -8) != 0) {
            if ((temp & 1) == 1) pad = true;
            temp = temp >> 1;
            temp &= 2147483647;
            ++i;
        }
        if (temp == 4) {
            temp = 1;
            i += 2;
        }
        else if (temp == 6) {
            temp = 3;
            ++i;
        }
        temp = (temp - 1) / 2;
        temp |= i << 2;
        if (pad) ++temp;
        putc((char)temp, outfile);
    }
    putc(flags6, outfile);
    putc(flags7, outfile);
    putc(mapper8, outfile);
    temp = ftell(prg);
    if (temp < 62898177) temp = ((temp / 16384) >> 8) & 15;
    else temp = 15;
    if (ftell(chr) < 31449089) temp |= ((ftell(chr) / 8192) >> 4) & 240;
    else temp |= 240;
    putc((char)temp, outfile);
    putc(prgram10, outfile);
    putc(chrram11, outfile);
    putc(timing12, outfile);
    putc(consoleType13, outfile);
    putc(miscroms14, outfile);
    putc(defExpDev15, outfile);
    
    //trainer (incomplete)
    if ((flags6 & 4) == 4) {
        for (int i = 0; i < 512; ++i) putc('\0', outfile);
    }
    
    //PRG-ROM
    fseek(prg, 0, SEEK_SET);
    c = getc(prg);
    while (!feof(prg)) {
        putc(c, outfile);
        c = getc(prg);
    }
    
    //CHR-ROM
    fseek(chr, 0, SEEK_SET);
    c = getc(chr);
    while (!feof(chr)) {
        putc(c, outfile);
        c = getc(chr);
    }
    
    //miscellaneous ROM (incomplete)
    
    
    fclose(outfile);
    fclose(prg);
    fclose(chr);
    
    return EXIT_SUCCESS;
}