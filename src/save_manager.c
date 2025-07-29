/**
 * Description: Handles save file management operations including save slot selection and file management
 * Programmed by: 2ru17 (S14, PROG2-T3)
 * Last modified: 2025-07-25
 * Version: 1.4
 * Acknowledgements:ExPLOSIONnNNN
 * I call it "Creator Mode" but its basically a glorified debugging tool that acts like a cheat mode xDDDDDDDD 
 * >>> This code is based on the project specifications provided.
 */

#include "game_structs.h"
#include "utils.h"

/**
 * Display save slot menu and get user selection.
 * Precondition: None
 * @param isLoad 1 if loading, 0 if saving
 * @return Selected slot number (1-3) or special codes, 0 to cancel
 */
int selectSaveSlot(int isLoad) {
    char input[10];
    int inputValue;
    int validSelection = 0;
    int returnValue = 0;
    int validSlot;  // moved declaration to beginning
    
    while (!validSelection) {
        clearScreen();
        
        if (isLoad) {
            printf("%s%s=== LOAD GAME ===%s\n", STYLE_BOLD, COLOR_BRIGHT_MAGENTA, COLOR_RESET);
            printf("%sAvailable save slots:%s\n\n", COLOR_CYAN, COLOR_RESET);
            
            for (int i = 1; i <= 3; i++) {
                if (saveFileExists(i)) {
                    printf("%s[%d]%s %sSave Slot %d%s - %s%sOCCUPIED%s\n", 
                           COLOR_BRIGHT_YELLOW, i, COLOR_RESET, 
                           COLOR_WHITE, i, COLOR_RESET, 
                           COLOR_BRIGHT_GREEN, STYLE_BOLD, COLOR_RESET);
                } else {
                    printf("%s[%d]%s %sSave Slot %d%s - %sEMPTY%s\n", 
                           COLOR_BRIGHT_YELLOW, i, COLOR_RESET, 
                           COLOR_WHITE, i, COLOR_RESET, 
                           COLOR_RED, COLOR_RESET);
                }
            }
            
            printf("\n%s[0]%s %sCancel%s\n\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_WHITE, COLOR_RESET);
            printf("%sEnter slot number to load: %s", COLOR_BRIGHT_CYAN, COLOR_RESET);
        } else {
            printf("%s%s=== SAVE GAME ===%s\n", STYLE_BOLD, COLOR_BRIGHT_MAGENTA, COLOR_RESET);
            printf("%sSave slots:%s\n\n", COLOR_CYAN, COLOR_RESET);
            
            for (int i = 1; i <= 3; i++) {
                if (saveFileExists(i)) {
                    printf("%s[%d]%s %sSave Slot %d%s - %s%sOCCUPIED%s %s(will overwrite)%s\n", 
                           COLOR_BRIGHT_YELLOW, i, COLOR_RESET, 
                           COLOR_WHITE, i, COLOR_RESET, 
                           COLOR_BRIGHT_GREEN, STYLE_BOLD, COLOR_RESET,
                           COLOR_YELLOW, COLOR_RESET);
                } else {
                    printf("%s[%d]%s %sSave Slot %d%s - %sEMPTY%s\n", 
                           COLOR_BRIGHT_YELLOW, i, COLOR_RESET, 
                           COLOR_WHITE, i, COLOR_RESET, 
                           COLOR_RED, COLOR_RESET);
                }
            }
            
            printf("\n%s[0]%s %sCancel%s\n\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_WHITE, COLOR_RESET);
            printf("%sEnter slot number to save: %s", COLOR_BRIGHT_CYAN, COLOR_RESET);
        }
        
        if (fgets(input, sizeof(input), stdin) != NULL) {
            // remove newline if present
            input[strcspn(input, "\n")] = 0;
            
            // check for CREATOR MODE secret code
            if (isLoad && strcmp(input, "17") == 0) {
                returnValue = 17298;  // keep using 17298 internally for compatibility
                validSelection = 1;
            } else {
                // try to parse as number
                inputValue = atoi(input);
                
                if (inputValue == 0) {
                    returnValue = 0;  // cancel
                    validSelection = 1;
                } else if (inputValue == 17298) {
                    returnValue = 17298;  // handle creator mode special case
                    validSelection = 1;
                } else if (inputValue >= 1 && inputValue <= 3) {
                    validSlot = 1;
                    if (isLoad && !saveFileExists(inputValue)) {
                        printf("%sError: Save slot %d is empty.%s\n", COLOR_RED, inputValue, COLOR_RESET);
                        waitForKeypress();
                        validSlot = 0;
                    }
                    if (validSlot) {
                        returnValue = inputValue;
                        validSelection = 1;
                    }
                } else {
                    printf("%sError: Please enter a number between 0-3.%s\n", COLOR_RED, COLOR_RESET);
                    waitForKeypress();
                }
            }
        } else {
            // EOF or input error - default to cancel
            returnValue = 0;
            validSelection = 1;
        }
    }
    
    return returnValue;
}

/**
 * Display save management menu.
 * Precondition: None
 * @return 0 to continue, 1 to exit program
 */
int manageSaveFiles(void) {
    char choice;
    int slot;
    int exitMenu = 0;
    int returnValue = 0;
    
    while (!exitMenu) {
        clearScreen();
        printf("%s%s=== SAVE FILE MANAGEMENT ===%s\n", STYLE_BOLD, COLOR_BRIGHT_MAGENTA, COLOR_RESET);
        printf("%sSave files:%s\n\n", COLOR_CYAN, COLOR_RESET);
        
        for (int i = 1; i <= 3; i++) {
            if (saveFileExists(i)) {
                printf("%sSlot %d:%s %s%sOCCUPIED%s\n", 
                       COLOR_WHITE, i, COLOR_RESET, 
                       COLOR_BRIGHT_GREEN, STYLE_BOLD, COLOR_RESET);
            } else {
                printf("%sSlot %d:%s %sEMPTY%s\n", 
                       COLOR_WHITE, i, COLOR_RESET, 
                       COLOR_RED, COLOR_RESET);
            }
        }
        
        printf("\n%s[D]%s %sDelete save file%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_WHITE, COLOR_RESET);
        printf("%s[B]%s %sBack to main menu%s\n\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_WHITE, COLOR_RESET);
        printf("%sChoice: %s", COLOR_BRIGHT_CYAN, COLOR_RESET);
        
        choice = getchar();
        while (getchar() != '\n');  // clear input buffer
        choice = toupper(choice);
        
        if (choice == 'B') {
            exitMenu = 1;
            returnValue = 0;
        } else if (choice == 'D') {
            printf("%sEnter slot number to delete (1-3): %s", COLOR_BRIGHT_CYAN, COLOR_RESET);
            if (scanf("%d", &slot) == 1) {
                while (getchar() != '\n');  // clear input buffer
                
                if (slot >= 1 && slot <= 3) {
                    if (saveFileExists(slot)) {
                        printf("%sAre you sure you want to delete save slot %d? %s[Y/N]: %s", 
                               COLOR_YELLOW, slot, COLOR_BRIGHT_YELLOW, COLOR_RESET);
                        choice = getchar();
                        while (getchar() != '\n');  // clear input buffer
                        choice = toupper(choice);
                        
                        if (choice == 'Y') {
                            if (deleteSaveFile(slot)) {
                                printf("%sSave slot %d deleted successfully.%s\n", COLOR_BRIGHT_GREEN, slot, COLOR_RESET);
                            } else {
                                printf("%sFailed to delete save slot %d.%s\n", COLOR_RED, slot, COLOR_RESET);
                            }
                        } else {
                            printf("%sDelete operation cancelled.%s\n", COLOR_YELLOW, COLOR_RESET);
                        }
                    } else {
                        printf("%sSave slot %d is already empty.%s\n", COLOR_YELLOW, slot, COLOR_RESET);
                    }
                } else {
                    printf("%sInvalid slot number.%s\n", COLOR_RED, COLOR_RESET);
                }
            } else {
                while (getchar() != '\n');  // clear input buffer
                printf("%sInvalid input.%s\n", COLOR_RED, COLOR_RESET);
            }
            
            waitForKeypress();
        } else {
            printf("%sInvalid choice.%s\n", COLOR_RED, COLOR_RESET);
            waitForKeypress();
        }
    }
    
    return returnValue;
}
