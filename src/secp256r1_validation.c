#include "secp256r1_validation.h"
#include "secp256r1_bigint.h"

void printProgressBar(i32 current, i32 total) {
    i32 width = 50; // Width of the progress bar
    float progress = (float)current / total;
    i32 pos = width * progress;

    // ANSI Escape Codes for colors
    const char* GREEN = "\x1b[32m";
    const char* YELLOW = "\x1b[33m";
    const char* RED = "\x1b[31m";
    const char* RESET = "\x1b[0m";

    printf("\r[");
    for (i32 i = 0; i < width; ++i) {
        if (i < pos) printf("%s=", GREEN); // White for completed part
        else if (i == pos) printf("%s>", YELLOW); // Yellow for current position
        else printf("%s ", RED); // Red for remaining part
    }
    printf("%s] %d%% (%d/%d)", RESET, (i32)(progress * 100.0), current, total);
}

void parseHexLine(word* dst, const char* src) {
#ifdef IS_32_BIT_ENV
    for (u8 i = 0; i < SIZE; i++) {
        word value;
        if (sscanf(src + i * 8, "%8x", &value) != 1)
           dst[i] = 0; // Example: set to zero if parsing fails
        else
            dst[i] = value;
#else
        if (sscanf(src + i * 16, "%16x", &value) != 1)
           dst[i] = 0; // Example: set to zero if parsing fails
        else
            dst[i] = value;
#endif
    }
}

void create_rspFile(const char* rspFileName, const char* reqFileName1, const char* reqFileName2) {
    FILE *reqFile1, *reqFile2, *rspFile;
    size_t bufsize = 64;

    reqFile1 = fopen(reqFileName1, "r");
    reqFile2 = fopen(reqFileName2, "r");
    if ((reqFile1 == NULL) || (reqFile2 == NULL)) {
        perror("Error opening TV_opA.txt or TV_opB.txt file");
        return;
    }

    // Open the .req file for writing
    rspFile = fopen(rspFileName, "w");
    if (rspFile == NULL) {
        perror("Error opening .rsp file");
        fclose(reqFile1);
        fclose(reqFile2);
        return;
    }
    
    // Allocate initial buffer
    char* line1 = (char*)calloc(bufsize, sizeof(char));
    char* line2 = (char*)calloc(bufsize, sizeof(char));
    
    if((line1 == NULL) || (line2 == NULL)) {
        perror("Unable to allocate memory");
        fclose(reqFile1);
        fclose(reqFile2);
        fclose(rspFile);
        return;
    }
    word data1[SIZE] = { 0x00, };
    word data2[SIZE] = { 0x00, };
    word result[SIZE] = { 0x00, };

    // Read the source file line by line
    while (fgets(line1, bufsize, reqFile1) && fgets(line2, bufsize, reqFile2)) { 
        // if ((line1 == NULL) || (line2 == NULL)) {
        //     fputc('\n', rspFile);
        //     continue;
        // }
        if ((line1 != NULL) && (line2 != NULL)) {
            parseHexLine(data1, line1);
            parseHexLine(data2, line2);
            addition_p256(result, data1, data2);
            for (u8 i = 0; i < SIZE; i++) {
                fprintf(rspFile, "%08X", result[i]);
            }
        }
    }
    
    free(line1); free(line2);
    fclose(reqFile1); fclose(reqFile2);
    fclose(rspFile);

    printf("TV_MY_PFADD.rsp file has been successfully created in 'test_vector/add_and_sub' folder.\n");
}

void addition_test() {
    const char* folderPath = "../test_vector/add_and_sub/";
    char reqFileName1[100], reqFileName2[100], TV_PFADD[100], rspFileName[100];
    
    // Construct full paths for input and output files
    snprintf(reqFileName1, sizeof(reqFileName1), "%s%s", folderPath, "TV_opA.txt");
    snprintf(reqFileName2, sizeof(reqFileName2), "%s%s", folderPath, "TV_opB.txt");
    snprintf(TV_PFADD, sizeof(TV_PFADD), "%s%s", folderPath, "TV_PFADD.txt");
    snprintf(rspFileName, sizeof(rspFileName), "%s%s", folderPath, "TV_MY_PFADD.rsp");
    
    create_rspFile(rspFileName, reqFileName1, reqFileName2);
}