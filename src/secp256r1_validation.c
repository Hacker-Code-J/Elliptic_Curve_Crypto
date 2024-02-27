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

bool is_blank_line(const char* line) {
    while (*line != '\0') {
        if (!isspace((u8)*line))
            return 0; // Not a blank line
        line++;
    }
    return 1; // Blank line
}

void create_p256_rspFile(const char* rspFileName, const char* reqFileName1, const char* reqFileName2, u8 option) {
    FILE *reqFile1, *reqFile2, *rspFile;
    size_t bufsize = MAX_LINE_LENGTH;

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
        
    // Read the source file line by line
    while (fgets(line1, bufsize, reqFile1) && fgets(line2, bufsize, reqFile2)) { 
        if (!is_blank_line(line1) && !is_blank_line(line2)) {
            word result[SIZE] = { 0x00, };
            stringToWord(data1, line1);
            stringToWord(data2, line2);
            // addition_p256(result, data1, data2);
            if (option == 0)
                addition_p256(result, data1, data2);
            else if (option == 1)
                subtraction_p256(result, data1, data2);
            else if (option == 2)
                multiplication_p256(result, data1, data2);
            else
                printf("\nInvalid Option!\n");
            for (i8 i = SIZE-1; i >= 0; i--) {
#ifdef IS_32_BIT_ENV
                fprintf(rspFile, "%08X", result[i]);
#else
                fprintf(rspFile, "%016lX", result[i]);
#endif
            }
            fputs("\n\n", rspFile);
        } else {
            continue;
        }
    }
    
    free(line1); free(line2);
    fclose(reqFile1); fclose(reqFile2);
    fclose(rspFile);

    // printf("TV_MY_PFADD.rsp file has been successfully created in 'test_vector/add_and_sub' folder.\n");
}

void addition_p256_test() {
    const char* folderPath = "../test_vector/add_and_sub/";
    char reqFileName1[100], reqFileName2[100], faxFileName[100], rspFileName[100];
    
    // Construct full paths for input and output files
    snprintf(reqFileName1, sizeof(reqFileName1), "%s%s", folderPath, "TV_opA.txt");
    snprintf(reqFileName2, sizeof(reqFileName2), "%s%s", folderPath, "TV_opB.txt");
    snprintf(faxFileName, sizeof(faxFileName), "%s%s", folderPath, "TV_PFADD.txt");
    snprintf(rspFileName, sizeof(rspFileName), "%s%s", folderPath, "TV_MY_PFADD.rsp");
    
    create_p256_rspFile(rspFileName, reqFileName1, reqFileName2, 0);

    printf("\nP-256 Addition Test:\n");

    FILE *file1, *file2;
    char line1[MAX_LINE_LENGTH], line2[MAX_LINE_LENGTH];
    u8 isEqual = 2; // Assume files are equal until proven otherwise

    // Open both files
    file1 = fopen(faxFileName, "r");
    file2 = fopen(rspFileName, "r");

    if (file1 == NULL || file2 == NULL) {
        perror("Error opening file");
        return;
    }

    u32 total = 10000;
    u32 idx = 1;

    // Read and compare each line
    while (fgets(line1, sizeof(line1), file1) && fgets(line2, sizeof(line2), file2)) {
        if (is_blank_line(line1) || is_blank_line(line2)) {
            continue;
        } else {
            isEqual = 1;    
            if (strncmp(line1, line2, 64) != 0) {
                field data1, data2;
                stringToWord(data1, line1);
                stringToWord(data2, line2);
                printf("\nAnswer: "); printData(data1);
                printf("My-Ans: "); printData(data2);
                isEqual = 0; // Files are not equal
                break;
            }
            printProgressBar(idx++, total);
        }
    }

    // Close the files
    fclose(file1);
    fclose(file2);

    // Output the result
    if (isEqual == 1) {
        printf("\nPASS!\n");
    } else if (isEqual == 0) {
        printf("\nFAIL!\n");
    } else {
        printf("\nError!\n");
    }
}

void subtraction_p256_test() {
    const char* folderPath = "../test_vector/add_and_sub/";
    char reqFileName1[100], reqFileName2[100], faxFileName[100], rspFileName[100];
    
    // Construct full paths for input and output files
    snprintf(reqFileName1, sizeof(reqFileName1), "%s%s", folderPath, "TV_opA.txt");
    snprintf(reqFileName2, sizeof(reqFileName2), "%s%s", folderPath, "TV_opB.txt");
    snprintf(faxFileName, sizeof(faxFileName), "%s%s", folderPath, "TV_PFSUB.txt");
    snprintf(rspFileName, sizeof(rspFileName), "%s%s", folderPath, "TV_MY_PFSUB.rsp");
    
    create_p256_rspFile(rspFileName, reqFileName1, reqFileName2, 1);

    printf("\nP-256 Subtraction Test:\n");

    FILE *file1, *file2;
    char line1[MAX_LINE_LENGTH], line2[MAX_LINE_LENGTH];
    u8 isEqual = 2; // Assume files are equal until proven otherwise

    // Open both files
    file1 = fopen(faxFileName, "r");
    file2 = fopen(rspFileName, "r");

    if (file1 == NULL || file2 == NULL) {
        perror("Error opening file");
        return;
    }

    u32 total = 10000;
    u32 idx = 1;

    // Read and compare each line
    while (fgets(line1, sizeof(line1), file1) && fgets(line2, sizeof(line2), file2)) {
        if (is_blank_line(line1) || is_blank_line(line2)) {
            continue;
        } else {
            isEqual = 1;    
            if (strncmp(line1, line2, 64) != 0) {
                field data1, data2;
                stringToWord(data1, line1);
                stringToWord(data2, line2);
                printf("\nAnswer: "); printData(data1);
                printf("My-Ans: "); printData(data2);
                isEqual = 0; // Files are not equal
                break;
            }
            printProgressBar(idx++, total);
        }
    }

    // Close the files
    fclose(file1);
    fclose(file2);

    // Output the result
    if (isEqual == 1) {
        printf("\nPASS!\n");
    } else if (isEqual == 0) {
        printf("\nFAIL!\n");
    } else {
        printf("\nError!\n");
    }
}

void create_mul_squ_rspFile(const char* rspFileName, const char* reqFileName1, const char* reqFileName2, u8 option) {
    FILE *reqFile1, *reqFile2, *rspFile;
    size_t bufsize = MAX_LINE_LENGTH;

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
        
    // Read the source file line by line
    while (fgets(line1, bufsize, reqFile1) && fgets(line2, bufsize, reqFile2)) { 
        if (!is_blank_line(line1) && !is_blank_line(line2)) {
            field result[2];
            stringToWord(data1, line1);
            stringToWord(data2, line2);
            // multiplication_ps(result, data1, data2);
            if (!option)
                multiplication_ps(result, data1, data2);
            else
                multiplication_ps2(result, data1, data2);
#ifdef IS_32_BIT_ENV
            for (i32 i = SIZE-1; i >= 0; i--)
                fprintf(rspFile, "%08X", result[1][i]);
            for (i32 i = SIZE-1; i >= 0; i--)
                fprintf(rspFile, "%08X", result[0][i]);
#else
            for (i32 i = SIZE-1; i >= 0; i--)
                fprintf(rspFile, "%016lX", result[1][i]);
            for (i32 i = SIZE-1; i >= 0; i--)
                fprintf(rspFile, "%016lX", result[0][i]);
#endif
            fputs("\n\n", rspFile);
        } else {
            continue;
        }
    }
    
    free(line1); free(line2);
    fclose(reqFile1); fclose(reqFile2);
    fclose(rspFile);
}

void multiplication_ps_test() {
    const char* folderPath = "../test_vector/mul_and_squ/";
    char reqFileName1[100], reqFileName2[100], faxFileName[100], rspFileName[100];
    
    // Construct full paths for input and output files
    snprintf(reqFileName1, sizeof(reqFileName1), "%s%s", folderPath, "TV_opA.txt");
    snprintf(reqFileName2, sizeof(reqFileName2), "%s%s", folderPath, "TV_opB.txt");
    snprintf(faxFileName, sizeof(faxFileName), "%s%s", folderPath, "TV_MUL.txt");
    snprintf(rspFileName, sizeof(rspFileName), "%s%s", folderPath, "TV_MY_MUL.rsp");
    
    create_mul_squ_rspFile(rspFileName, reqFileName1, reqFileName2, 0);

    printf("\n256-bit Multiplication PS Test:\n");

    FILE *file1, *file2;
    char line1[MAX_LINE_LENGTH], line2[MAX_LINE_LENGTH];
    u8 isEqual = 2; // Assume files are equal until proven otherwise

    // Open both files
    file1 = fopen(faxFileName, "r");
    file2 = fopen(rspFileName, "r");

    if (file1 == NULL || file2 == NULL) {
        perror("Error opening file");
        return;
    }

    u32 total = 10000;
    u32 idx = 1;

    // Read and compare each line
    while (fgets(line1, sizeof(line1), file1) && fgets(line2, sizeof(line2), file2)) {
        if (is_blank_line(line1) || is_blank_line(line2)) {
            continue;
        } else {
            isEqual = 1;    
            if (strncmp(line1, line2, 128) != 0) {
                printf("\nAnswer: "); printf("%s\n", line1);
                printf("My-Ans: "); printf("%s\n", line2);
                isEqual = 0; // Files are not equal
                break;
            }
            printProgressBar(idx++, total);
        }
    }

    // Close the files
    fclose(file1);
    fclose(file2);

    // Output the result
    // Output the result
    if (isEqual == 1) {
        printf("\nPASS!\n");
    } else if (isEqual == 0) {
        printf("\nFAIL!\n");
    } else {
        printf("\nError!\n");
    }
}

void multiplication_ps2_test() {
    const char* folderPath = "../test_vector/mul_and_squ/";
    char reqFileName1[100], reqFileName2[100], faxFileName[100], rspFileName[100];
    
    // Construct full paths for input and output files
    snprintf(reqFileName1, sizeof(reqFileName1), "%s%s", folderPath, "TV_opA.txt");
    snprintf(reqFileName2, sizeof(reqFileName2), "%s%s", folderPath, "TV_opB.txt");
    snprintf(faxFileName, sizeof(faxFileName), "%s%s", folderPath, "TV_MUL.txt");
    snprintf(rspFileName, sizeof(rspFileName), "%s%s", folderPath, "TV_MY_MUL.rsp");
    
    create_mul_squ_rspFile(rspFileName, reqFileName1, reqFileName2, 1);

    printf("\n256-bit Multiplication PS2 Test:\n");

    FILE *file1, *file2;
    char line1[MAX_LINE_LENGTH], line2[MAX_LINE_LENGTH];
    u8 isEqual = 1; // Assume files are equal until proven otherwise

    // Open both files
    file1 = fopen(faxFileName, "r");
    file2 = fopen(rspFileName, "r");

    if (file1 == NULL || file2 == NULL) {
        perror("Error opening file");
        return;
    }

    u32 total = 10000;
    u32 idx = 1;

    // Read and compare each line
    while (fgets(line1, sizeof(line1), file1) && fgets(line2, sizeof(line2), file2)) {
        if (is_blank_line(line1) || is_blank_line(line2)) {
            continue;
        } else {
            isEqual = 1;    
            if (strncmp(line1, line2, 128) != 0) {
                printf("\nAnswer: "); printf("%s\n", line1);
                printf("My-Ans: "); printf("%s\n", line2);
                isEqual = 0; // Files are not equal
                break;
            }
            printProgressBar(idx++, total);
        }
    }

    // Close the files
    fclose(file1);
    fclose(file2);

    // Output the result
    if (isEqual == 1) {
        printf("\nPASS!\n");
    } else if (isEqual == 0) {
        printf("\nFAIL!\n");
    } else {
        printf("\nError!\n");
    }
}

void multiplication_p256_test() {
    const char* folderPath = "../test_vector/reduction/";
    char reqFileName1[100], reqFileName2[100], faxFileName[100], rspFileName[100];
    char faxFileName2[100], rspFileName2[100];

    // Construct full paths for input and output files
    snprintf(reqFileName1, sizeof(reqFileName1), "%s%s", folderPath, "TV_opA.txt");
    snprintf(reqFileName2, sizeof(reqFileName2), "%s%s", folderPath, "TV_opB.txt");
    snprintf(faxFileName, sizeof(faxFileName), "%s%s", folderPath, "TV_MUL.txt");
    snprintf(rspFileName, sizeof(rspFileName), "%s%s", folderPath, "TV_MY_MUL.rsp");
    snprintf(faxFileName2, sizeof(faxFileName2), "%s%s", folderPath, "TV_PFMUL.txt");
    snprintf(rspFileName2, sizeof(rspFileName2), "%s%s", folderPath, "TV_MY_PFMUL.rsp");
    
    create_mul_squ_rspFile(rspFileName, reqFileName1, reqFileName2, 1);
    create_p256_rspFile(rspFileName2, reqFileName1, reqFileName2, 2);

    printf("\nP-256 Multiplication Test (Non-Reduction):\n");

    FILE *file1, *file2, *file3, *file4;
    char line1[MAX_LINE_LENGTH], line2[MAX_LINE_LENGTH];
    u8 isEqual = 2; // Assume files are equal until proven otherwise

    // Open both files
    file1 = fopen(faxFileName, "r");
    file2 = fopen(rspFileName, "r");
    file3 = fopen(faxFileName2, "r");
    file4 = fopen(rspFileName2, "r");

    if (file1 == NULL || file2 == NULL || file3 == NULL || file4 == NULL) {
        perror("Error opening file");
        return;
    }

    u32 total = 10000;
    u32 idx = 1;

    // Read and compare each line
    while (fgets(line1, sizeof(line1), file1) && fgets(line2, sizeof(line2), file2)) {
        if (is_blank_line(line1) || is_blank_line(line2)) {
            continue;
        } else {
            isEqual = 1;    
            if (strncmp(line1, line2, 128) != 0) {
                printf("\nAnswer: "); printf("%s\n", line1);
                printf("My-Ans: "); printf("%s\n", line2);
                isEqual = 0; // Files are not equal
                break;
            }
            printProgressBar(idx++, total);
        }
    }

    // Close the files
    fclose(file1);
    fclose(file2);

    // Output the result
    if (isEqual == 1) {
        printf("\nPASS!\n");
    } else if (isEqual == 0) {
        printf("\nFAIL!\n");
    } else {
        printf("\nError!\n");
    }

    printf("\nP-256 Multiplication Test (Reduction):\n");

    isEqual = 2;
    idx = 1;

    while (fgets(line1, sizeof(line1), file3) && fgets(line2, sizeof(line2), file4)) {
        if (is_blank_line(line1) || is_blank_line(line2)) {
            continue;
        } else {
            isEqual = 1;    
            if (strncmp(line1, line2, 64) != 0) {
                printf("\nAnswer: "); printf("%s\n", line1);
                printf("My-Ans: "); printf("%s\n", line2);
                isEqual = 0; // Files are not equal
                break;
            }
            printProgressBar(idx++, total);
        }
    }

    fclose(file3);
    fclose(file4);

    if (isEqual == 1) {
        printf("\nPASS!\n");
    } else if (isEqual == 0) {
        printf("\nFAIL!\n");
    } else {
        printf("\nError!\n");
    }
}