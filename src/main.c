/**
 * Description: Main entry point for Yohane The Parhelion game - handles startup and game loop
 * Programmed by: 2ru17 (S14, PROG2-T3)
 * Last modified: 2025-07-25
 * Version: 2.0
 * Acknowledgements: This code is based on the project specifications provided.
 */

#include "game_structs.h"
#include "utils.h"

/**
 * Main function and entry point of the program.
 * Precondition: None.
 * @return 0 on successful execution.
 */
int main() {
    GameState gameState;       // game state data
    int loadSuccess = 0;      // load success flag
    int slot;                 // save slot number
    char choice;              // player menu input
    int exitStartup = 0;      // startup loop control
    
    // initialize random seed for game mechanics (once at program start)
    srand((unsigned int)time(NULL));
    
    // main startup menu loop - continues until user chooses to play or quit
    while (!exitStartup) {
        clearScreen();
        
        // display startup menu
        displayStartupMenu();
        
        // get user input and normalize to uppercase
        choice = getchar();
        while (getchar() != '\n');  // clear input buffer
        choice = toupper(choice);   // convert to uppercase
        
        // handle menu choices
        if (choice == 'Q') {
            // quit option - display farewell message and exit
            displayFarewellMessage();
            exitStartup = 1;  // exit startup loop
            gameState.yohane.maxHp = 0;  // quit flag
        } else if (choice == 'M') {
            // manage save files option - call save file manager
            manageSaveFiles();
        } else if (choice == 'L') {
            // load game option - select slot and attempt to load
            slot = selectSaveSlot(1);  // loading mode
            if (slot > 0) {           // valid slot selected
                loadSuccess = loadGameFromSlot(&gameState, slot);
                if (loadSuccess) {
                    // successful load - show confirmation and start game
                    printf("\n%sGame loaded from slot %d successfully!%s\n", COLOR_BRIGHT_GREEN, slot, COLOR_RESET);
                    waitForKeypress();
                    exitStartup = 1;  // exit and start game
                } else {
                    // load failed - show error message
                    printf("\n%sFailed to load game from slot %d!%s\n", COLOR_RED, slot, COLOR_RESET);
                    waitForKeypress();  // show error message
                }
            }
        } else if (choice == 'N') {
            // new game option - initialize fresh game state
            displayNewGameMessage();
            
            initializeStaticData(&gameState);        // set up base game data
            setupNewGame(&gameState, 0);             // fresh game (no preserve gold)
            
            printf("%sNew game created successfully!%s\n", COLOR_BRIGHT_GREEN, COLOR_RESET);
            waitForKeypress();
            exitStartup = 1;  // exit and start game
        } else {
            printf("\n%sInvalid choice. Please try again!%s\n", COLOR_YELLOW, COLOR_RESET);
            waitForKeypress();
        }
    }
    
    // proceed if not quitting
    if (gameState.yohane.maxHp != 0) {    // maxHp = 0 is quit flag
        // main game loop
        displayMainMenu(&gameState);      // enter game interface
        
        // save game before exit (not creator mode)
        displayGameEndMessage();
        
        if (gameState.yohane.maxHp < 999.0f) {  // not creator mode (999 HP)
            printf("\n%sWould you like to save your progress?%s\n", COLOR_BRIGHT_CYAN, COLOR_RESET);
            slot = selectSaveSlot(0);  // saving mode
            if (slot > 0) {           // valid slot selected
                if (saveGameToSlot(&gameState, slot)) {
                    printf("\n%sGame saved to slot %d successfully!%s\n", COLOR_BRIGHT_GREEN, slot, COLOR_RESET);
                } else {
                    printf("\n%sFailed to save game to slot %d!%s\n", COLOR_RED, slot, COLOR_RESET);
                }
            } else {
                printf("\n%sGame not saved.%s\n", COLOR_YELLOW, COLOR_RESET);
            }
        } else {
            printf("\n%screator mode session ended - progress not saved.%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
        }
        
        displayThankYouMessage();
    }
    return 0;
}