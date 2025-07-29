/**
 * Description: User interface functions for displaying menus and graphics
 * Programmed by: 2ru17 (S14, PROG2-T3)
 * Last modified: 2025-07-28
 * Version: 1.0
 * Acknowledgements: This code is based on the project specifications provided.
 */

#include "game_structs.h"
#include "utils.h"

/**
 * Display the main startup menu with title banner.
 * Precondition: None.
 */
void displayStartupMenu() {
    clearScreen();
    
    // display the game title banner with decorative borders
    printf("%s%s╔═══════════════════════════════════════════════════════╗%s\n", STYLE_BOLD, COLOR_BRIGHT_MAGENTA, COLOR_RESET);
    printf("%s%s║              YOHANE THE PARHELION                     ║%s\n", STYLE_BOLD, COLOR_BRIGHT_MAGENTA, COLOR_RESET);
    printf("%s%s║            The Siren in the Mirror World              ║%s\n", STYLE_BOLD, COLOR_BRIGHT_MAGENTA, COLOR_RESET);
    printf("%s%s╚═══════════════════════════════════════════════════════╝%s\n", STYLE_BOLD, COLOR_BRIGHT_MAGENTA, COLOR_RESET);
    printf("\n%sWelcome to the Mirror World, Little Demon!%s\n\n", COLOR_BRIGHT_CYAN, COLOR_RESET);
    
    // display main menu options in a formatted box
    printf("%s┌─────────────── %sMAIN MENU%s ─────────────────────┐%s\n", COLOR_WHITE, COLOR_BRIGHT_YELLOW, COLOR_WHITE, COLOR_RESET);
    printf("%s│                                               │%s\n", COLOR_WHITE, COLOR_RESET);
    printf("%s│  %s[L]%s %-40s │%s\n", COLOR_WHITE, COLOR_BRIGHT_YELLOW, COLOR_WHITE, "Load Game", COLOR_RESET);
    printf("%s│  %s[N]%s %-40s │%s\n", COLOR_WHITE, COLOR_BRIGHT_YELLOW, COLOR_WHITE, "New Game", COLOR_RESET);
    printf("%s│  %s[M]%s %-40s │%s\n", COLOR_WHITE, COLOR_BRIGHT_YELLOW, COLOR_WHITE, "Manage Save Files", COLOR_RESET);
    printf("%s│  %s[Q]%s %-40s │%s\n", COLOR_WHITE, COLOR_BRIGHT_YELLOW, COLOR_WHITE, "Quit", COLOR_RESET);
    printf("%s│                                               │%s\n", COLOR_WHITE, COLOR_RESET);
    printf("%s└───────────────────────────────────────────────┘%s\n\n", COLOR_WHITE, COLOR_RESET);
    
    printf("%sChoice: %s", COLOR_BRIGHT_CYAN, COLOR_RESET);
}

/**
 * Display farewell message when quitting.
 * Precondition: None.
 */
void displayFarewellMessage() {
    clearScreen();
    printf("%s%s╔═══════════════════════════════════════════════════════╗%s\n", STYLE_BOLD, COLOR_BRIGHT_CYAN, COLOR_RESET);
    printf("%s%s║                   FAREWELL!                           ║%s\n", STYLE_BOLD, COLOR_BRIGHT_CYAN, COLOR_RESET);
    printf("%s%s╚═══════════════════════════════════════════════════════╝%s\n", STYLE_BOLD, COLOR_BRIGHT_CYAN, COLOR_RESET);
    printf("\n%s Thanks for playing Yohane The Parhelion! %s\n", COLOR_BRIGHT_CYAN, COLOR_RESET);
    printf("%sMay the light of Yohane guide your path! %s\n\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
}

/**
 * Display new game creation message.
 * Precondition: None.
 */
void displayNewGameMessage() {
    clearScreen();
    printf("%s%s╔═══════════════════════════════════════════════════════╗%s\n", STYLE_BOLD, COLOR_BRIGHT_GREEN, COLOR_RESET);
    printf("%s%s║                 NEW ADVENTURE                         ║%s\n", STYLE_BOLD, COLOR_BRIGHT_GREEN, COLOR_RESET);
    printf("%s%s╚═══════════════════════════════════════════════════════╝%s\n", STYLE_BOLD, COLOR_BRIGHT_GREEN, COLOR_RESET);
    printf("\n%sStarting a new journey in the Mirror World...%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
}

/**
 * Display game session end message.
 * Precondition: None.
 */
void displayGameEndMessage() {
    clearScreen();
    printf("%s%s╔═══════════════════════════════════════════════════════╗%s\n", STYLE_BOLD, COLOR_BRIGHT_MAGENTA, COLOR_RESET);
    printf("%s%s║                 GAME SESSION END                      ║%s\n", STYLE_BOLD, COLOR_BRIGHT_MAGENTA, COLOR_RESET);
    printf("%s%s╚═══════════════════════════════════════════════════════╝%s\n", STYLE_BOLD, COLOR_BRIGHT_MAGENTA, COLOR_RESET);
    printf("\n%sPress any key to continue...%s\n", COLOR_BRIGHT_CYAN, COLOR_RESET);
    waitForKeypress();
}

/**
 * Display final thank you message.
 * Precondition: None.
 */
void displayThankYouMessage() {
    printf("\n%sThanks for playing Yohane The Parhelion!%s\n", COLOR_BRIGHT_CYAN, COLOR_RESET);
    printf("%sMay your adventures in the Mirror World continue!%s\n\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
}

/**
 * Display a random loading tip with bright yellow color
 * Precondition: None.
 */
void showLoadingTip() {
    int tipCount;
    int randomTip;
    const char *tips[] = {
        "Controls: WASD to move, [ ] to cycle items, Space to use item, ESC for menu",
        "The following are obstacles that can considerably cause a game over:\n1.) Bat  \n2.) Heat Tiles  \n3.) Spike Walls  \n4.) Siren",
        "Should a player end up with a Game Over, the following are saved:\n1.) The Aqours members that have been rescued\n2.) Gold obtained",
        "Tip: Heat tiles will damage you if you stay on them!",
        "Tip: Use Tears of a Fallen Angel to heal yourself!",
        "Tip: Noppo Bread can restore your health completely!",
        "Tip: Choco-mint Ice Cream can save you from certain death!",
        "Tip: Water tiles require Air Shoes to cross!",
        "Tip: Bats move randomly and will attack if adjacent!",
        "Tip: Rescue all 9 Aqours members to unlock special achievements!",
        "Tip: The Siren is the final boss - prepare well before facing her!",
        "Tip: Use the shovel upgrade to dig through certain walls!",
        "Tip: Kill all bats first on final dungeon first before attempting the switches",
        "Tip: There's only a set amount of bats the Siren can spawn; and it is not less than 10. Be careful or be swarmed."
    };
    
    clearScreen();
    
    tipCount = sizeof(tips) / sizeof(tips[0]);
    randomTip = getRandomNumber(0, tipCount - 1);
    
    printf("%s=== LOADING ===%s\n\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
    printf("%s%s%s\n\n", COLOR_BRIGHT_YELLOW, tips[randomTip], COLOR_RESET);
    
    waitForKeypress();
    clearScreen();
}

/**
 * Display the main menu and handle user input.
 * Precondition: state pointer must be valid and game state initialized
 * @param state Pointer to the current GameState
 */
void displayMainMenu(GameState *state) {
    char input;
    int exitMenu = 0;
    
    while (!exitMenu) {
        clearScreen();
        printf("%s%s=== YOHANE THE PARHELION ===%s\n", STYLE_BOLD, COLOR_BRIGHT_MAGENTA, COLOR_RESET);
        printf("%sThe Siren in the Mirror World!%s\n\n", COLOR_CYAN, COLOR_RESET);
        
        // display menu options based on game state
        if (state->isPlaythroughActive) {
            printf("%s[C]%s %sContinue current run%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_WHITE, COLOR_RESET);
        } else {
            printf("%s[N]%s %sNew Game%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_WHITE, COLOR_RESET);
        }
        printf("%s[V]%s %sView Achievements%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_WHITE, COLOR_RESET);
        printf("%s[Q]%s %sQuit%s\n\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_WHITE, COLOR_RESET);
        printf("%sChoice: %s", COLOR_BRIGHT_CYAN, COLOR_RESET);
        
        scanf(" %c", &input);
        while (getchar() != '\n');  // clear input buffer
        
        switch (toupper(input)) {
            case 'C':
                if (state->isPlaythroughActive) {
                    if (state->yohane.hp <= 0) {
                        printf("%sYou cannot enter dungeons with 0 HP! Please start a new playthrough.%s\n", COLOR_RED, COLOR_RESET);
                        waitForKeypress();
                    } else {
                        runDungeonSelection(state);
                    }
                } else {
                    printf("%sNo active playthrough. Please start a new game.%s\n", COLOR_RED, COLOR_RESET);
                    waitForKeypress();
                }
                break;
            case 'N':
                setupNewGame(state, 1);  // 1 = preserve gold (new playthrough after Game Over)
                runDungeonSelection(state);
                break;
            case 'V':
                viewAchievements(state);
                break;
            case 'Q':
                exitMenu = 1;
                break;
            default:
                printf("%sInvalid choice. Please try again.%s\n", COLOR_RED, COLOR_RESET);
                waitForKeypress();
                break;
        }
    }
}

/**
 * Run the dungeon selection menu.
 * Precondition: state pointer must be valid and playthrough must be active
 * @param state Pointer to the current GameState
 */
void runDungeonSelection(GameState *state) {
    char input;
    int exitMenu = 0;
    
    while (!exitMenu && state->isPlaythroughActive) {
        displayDungeonSelectionMenu(state);
        
        scanf(" %c", &input);
        while (getchar() != '\n');  // clear input buffer
        
        switch (toupper(input)) {
            case '1':
                if (!state->dungeonsCleared[0] && state->rescuedInThisRun[0] >= 0 && state->rescuedInThisRun[0] < MAX_IDOLS) {
                    if (runDungeon(state, 0)) {
                        state->dungeonsCleared[0] = 1;
                        state->totalDungeonsCleared++;  // track total dungeons cleared
                        state->aqours[state->rescuedInThisRun[0]].isRescuedOnce = 1;
                        state->aqours[state->rescuedInThisRun[0]].rescueCount++;
                        checkAchievements(state);
                    }
                } else if (state->rescuedInThisRun[0] < 0 || state->rescuedInThisRun[0] >= MAX_IDOLS) {
                    printf("%sError: Invalid dungeon data. Please restart the game.%s\n", COLOR_RED, COLOR_RESET);
                    waitForKeypress();
                } else {
                    printf("%sThis dungeon has already been cleared.%s\n", COLOR_YELLOW, COLOR_RESET);
                    waitForKeypress();
                }
                break;
            case '2':
                if (!state->dungeonsCleared[1] && state->rescuedInThisRun[1] >= 0 && state->rescuedInThisRun[1] < MAX_IDOLS) {
                    if (runDungeon(state, 1)) {
                        state->dungeonsCleared[1] = 1;
                        state->totalDungeonsCleared++;  // track total dungeons cleared
                        state->aqours[state->rescuedInThisRun[1]].isRescuedOnce = 1;
                        state->aqours[state->rescuedInThisRun[1]].rescueCount++;
                        checkAchievements(state);
                    }
                } else if (state->rescuedInThisRun[1] < 0 || state->rescuedInThisRun[1] >= MAX_IDOLS) {
                    printf("%sError: Invalid dungeon data. Please restart the game.%s\n", COLOR_RED, COLOR_RESET);
                    waitForKeypress();
                } else {
                    printf("%sThis dungeon has already been cleared.%s\n", COLOR_YELLOW, COLOR_RESET);
                    waitForKeypress();
                }
                break;
            case '3':
                if (!state->dungeonsCleared[2] && state->rescuedInThisRun[2] >= 0 && state->rescuedInThisRun[2] < MAX_IDOLS) {
                    if (runDungeon(state, 2)) {
                        state->dungeonsCleared[2] = 1;
                        state->totalDungeonsCleared++;  // track total dungeons cleared
                        state->aqours[state->rescuedInThisRun[2]].isRescuedOnce = 1;
                        state->aqours[state->rescuedInThisRun[2]].rescueCount++;
                        checkAchievements(state);
                    }
                } else if (state->rescuedInThisRun[2] < 0 || state->rescuedInThisRun[2] >= MAX_IDOLS) {
                    printf("%sError: Invalid dungeon data. Please restart the game.%s\n", COLOR_RED, COLOR_RESET);
                    waitForKeypress();
                } else {
                    printf("%sThis dungeon has already been cleared.%s\n", COLOR_YELLOW, COLOR_RESET);
                    waitForKeypress();
                }
                break;
            case 'F':
                // check if all dungeons are cleared
                if (state->dungeonsCleared[0] && state->dungeonsCleared[1] && state->dungeonsCleared[2]) {
                    if (runFinalBattle(state)) {
                        printf("%s%sCongratulations! You have completed the playthrough!%s\n", STYLE_BOLD, COLOR_BRIGHT_GREEN, COLOR_RESET);
                        state->isPlaythroughActive = 0;
                        state->finalBattleWonCount++;  // track final battle wins
                        checkAchievements(state);
                        waitForKeypress();
                        exitMenu = 1;
                    } else {
                        // player was defeated in final battle - Game Over mechanics already handled by showDefeatScreen
                        exitMenu = 1;
                    }
                } else {
                    printf("%sYou must clear all three dungeons before the final battle.%s\n", COLOR_RED, COLOR_RESET);
                    waitForKeypress();
                }
                break;
            case 'I':
                viewInventory(state);
                break;
            case 'H':
                if (state->aqours[IDOL_HANAMARU].isRescuedOnce) {
                    displayShopMenu(state);
                } else {
                    printf("%sShop is not available. Rescue Hanamaru first!%s\n", COLOR_RED, COLOR_RESET);
                    waitForKeypress();
                }
                break;
            case 'M':
                exitMenu = 1;
                break;
            default:
                printf("%sInvalid choice. Please try again.%s\n", COLOR_RED, COLOR_RESET);
                waitForKeypress();
                break;
        }
    }
}

/**
 * Display the dungeon selection menu.
 * Precondition: state is a valid pointer to an initialized GameState struct.
 * @param state Pointer to the current GameState.
 */
void displayDungeonSelectionMenu(GameState *state) {
    int i;
    
    clearScreen();
    printf("%s%s=== DUNGEON SELECTION ===%s\n\n", STYLE_BOLD, COLOR_BRIGHT_MAGENTA, COLOR_RESET);
    
    // display available dungeons
    for (i = 0; i < 3; i++) {
        if (state->rescuedInThisRun[i] >= 0 && state->rescuedInThisRun[i] < MAX_IDOLS) {
            printf("%s[%d]%s %s%s%s (%s%s%s) - ", 
                   COLOR_BRIGHT_YELLOW, i + 1, COLOR_RESET,
                   COLOR_WHITE, state->aqours[state->rescuedInThisRun[i]].dungeonName, COLOR_RESET,
                   COLOR_BRIGHT_CYAN, state->aqours[state->rescuedInThisRun[i]].name, COLOR_RESET);
            if (state->dungeonsCleared[i]) {
                printf("%s%sCLEARED%s", STYLE_BOLD, COLOR_BRIGHT_GREEN, COLOR_RESET);
            } else if (state->currentDungeon.difficulty == (i + 1) && 
                       state->currentDungeon.floor > 1 && 
                       state->currentDungeon.floor <= state->currentDungeon.maxFloors) {
                printf("%s%sIN PROGRESS (Floor %d/%d)%s", STYLE_BOLD, COLOR_BRIGHT_YELLOW, 
                       state->currentDungeon.floor, state->currentDungeon.maxFloors, COLOR_RESET);
            } else {
                printf("%sAvailable%s", COLOR_WHITE, COLOR_RESET);
            }
            printf("\n");
        }
    }
    
    // check if final battle is available
    if (state->dungeonsCleared[0] && state->dungeonsCleared[1] && state->dungeonsCleared[2]) {
        printf("%s[F]%s %sFinal Battle - Face the Siren!%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_BRIGHT_RED, COLOR_RESET);
    }
    
    printf("\n%s[I]%s %sView Inventory%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_WHITE, COLOR_RESET);
    
    // show shop if Hanamaru is rescued
    if (state->aqours[IDOL_HANAMARU].isRescuedOnce) {
        printf("%s[H]%s %sHanamaru's Shop%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_WHITE, COLOR_RESET);
    }
    
    printf("%s[M]%s %sReturn to Main Menu%s\n\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_WHITE, COLOR_RESET);
    printf("%sGold: %s%d%s\n", COLOR_WHITE, COLOR_YELLOW, state->yohane.gold, COLOR_RESET);
    printf("%sHP: ", COLOR_WHITE);
    if (state->yohane.hp <= state->yohane.maxHp * 0.3) {
        printf("%s%.1f%s", COLOR_BRIGHT_RED, state->yohane.hp, COLOR_RESET);
    } else if (state->yohane.hp >= state->yohane.maxHp * 0.8) {
        printf("%s%.1f%s", COLOR_BRIGHT_GREEN, state->yohane.hp, COLOR_RESET);
    } else {
        printf("%s%.1f%s", COLOR_YELLOW, state->yohane.hp, COLOR_RESET);
    }
    printf("%s/%.1f%s\n\n", COLOR_WHITE, state->yohane.maxHp, COLOR_RESET);
    printf("%sChoice: %s", COLOR_BRIGHT_CYAN, COLOR_RESET);
}

/**
 * Display the inventory screen.
 * Precondition: state is a valid pointer to an initialized GameState struct.
 * @param state Pointer to the current GameState.
 */
void viewInventory(GameState *state) {
    char input;
    int exitInventory = 0;
    
    while (!exitInventory) {
        clearScreen();
        printf("%s%s=== INVENTORY ===%s\n", STYLE_BOLD, COLOR_BRIGHT_MAGENTA, COLOR_RESET);
        printf("%sLailaps: These are the items you have, Yohane!%s\n\n", COLOR_CYAN, COLOR_RESET);
        
        printf("%sHP: ", COLOR_WHITE);
        if (state->yohane.hp <= state->yohane.maxHp * 0.3) {
            printf("%s%.1f%s", COLOR_BRIGHT_RED, state->yohane.hp, COLOR_RESET);
        } else if (state->yohane.hp >= state->yohane.maxHp * 0.8) {
            printf("%s%.1f%s", COLOR_BRIGHT_GREEN, state->yohane.hp, COLOR_RESET);
        } else {
            printf("%s%.1f%s", COLOR_YELLOW, state->yohane.hp, COLOR_RESET);
        }
        printf("%s/%.1f%s\n", COLOR_WHITE, state->yohane.maxHp, COLOR_RESET);
        printf("%sTotal Gold: %s%d GP%s\n\n", COLOR_WHITE, COLOR_YELLOW, state->yohane.gold, COLOR_RESET);
        printf("%sItems available:%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
        
        // always show all three item types with colors
        printf("%s1. %sTears of a fallen angel: %s%d%s\n", COLOR_BRIGHT_YELLOW, COLOR_CYAN, COLOR_WHITE, state->tearsCount, COLOR_RESET);
        printf("%s2. %sNoppo bread: %s%d%s\n", COLOR_BRIGHT_YELLOW, COLOR_YELLOW, COLOR_WHITE, state->noppoBreadCount, COLOR_RESET);
        printf("%s3. %sChoco-mint ice cream: %s%d%s\n", COLOR_BRIGHT_YELLOW, COLOR_BRIGHT_MAGENTA, COLOR_WHITE, state->chocoMintCount, COLOR_RESET);
        
        printf("\n%s[R]%s %sReturn%s\n\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_WHITE, COLOR_RESET);
        printf("%sChoice: %s", COLOR_BRIGHT_CYAN, COLOR_RESET);
        
        scanf(" %c", &input);
        while (getchar() != '\n');  // clear input buffer
        
        switch (toupper(input)) {
            case 'R':
                exitInventory = 1;
                break;
            default:
                printf("%sInvalid choice. Please try again.%s\n", COLOR_RED, COLOR_RESET);
                waitForKeypress();
                break;
        }
    }
}

/**
 * Display the achievements screen.
 * Precondition: state is a valid pointer to an initialized GameState struct.
 * @param state Pointer to the current GameState.
 */
void viewAchievements(GameState *state) {
    char input;
    int currentPage = 0;
    const int achievementsPerPage = 8;
    int totalPages = (MAX_ACHIEVEMENTS + achievementsPerPage - 1) / achievementsPerPage;
    int earnedCount = 0;
    int exitMenu = 0;
    int i, startIdx, endIdx, displayNum;
    
    // count earned achievements
    for (i = 0; i < MAX_ACHIEVEMENTS; i++) {
        if (state->achievements[i].isEarned) {
            earnedCount++;
        }
    }
    
    while (!exitMenu) {
        clearScreen();
        printf("%s%s=== ACHIEVEMENTS ===%s\n", STYLE_BOLD, COLOR_BRIGHT_MAGENTA, COLOR_RESET);
        printf("%sObtained: %s%d%s / %s%d%s\n", COLOR_WHITE, COLOR_BRIGHT_GREEN, earnedCount, COLOR_WHITE, COLOR_BRIGHT_CYAN, MAX_ACHIEVEMENTS, COLOR_RESET);
        printf("%sPage: %s%d%s / %s%d%s\n\n", COLOR_WHITE, COLOR_BRIGHT_YELLOW, currentPage + 1, COLOR_WHITE, COLOR_BRIGHT_YELLOW, totalPages, COLOR_RESET);
        
        // display achievements for current page
        startIdx = currentPage * achievementsPerPage;
        endIdx = startIdx + achievementsPerPage;
        if (endIdx > MAX_ACHIEVEMENTS) endIdx = MAX_ACHIEVEMENTS;
        
        for (i = startIdx; i < endIdx; i++) {
            displayNum = (i - startIdx) + 1;
            printf("%s[%d]%s %s%s%s - ", COLOR_BRIGHT_YELLOW, displayNum, COLOR_RESET, COLOR_WHITE, state->achievements[i].name, COLOR_RESET);
            if (state->achievements[i].isEarned) {
                printf("%s%sEARNED%s", STYLE_BOLD, COLOR_BRIGHT_GREEN, COLOR_RESET);
            } else {
                printf("%sNOT EARNED%s", COLOR_RED, COLOR_RESET);
            }
            printf("\n");
        }
        
        printf("\n%s[N]%s %sNext page%s  %s[P]%s %sPrevious page%s  %s[1-%d]%s %sView details%s  %s[B]%s %sBack%s\n\n", 
               COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_WHITE, COLOR_RESET,
               COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_WHITE, COLOR_RESET,
               COLOR_BRIGHT_YELLOW, endIdx - startIdx, COLOR_RESET, COLOR_WHITE, COLOR_RESET,
               COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_WHITE, COLOR_RESET);
        printf("%sChoice: %s", COLOR_BRIGHT_CYAN, COLOR_RESET);
        
        scanf(" %c", &input);
        while (getchar() != '\n');  // clear input buffer
        
        switch (toupper(input)) {
            case 'N':
                if (currentPage < totalPages - 1) {
                    currentPage++;
                }
                break;
            case 'P':
                if (currentPage > 0) {
                    currentPage--;
                }
                break;
            case 'B':
                exitMenu = 1;
                break;
            default:
                // check if it's a number for viewing details
                if (input >= '1' && input <= '9') {
                    int relativeIdx = input - '1';  // 0-based relative index on current page
                    int achievementIdx = startIdx + relativeIdx;  // actual achievement index
                    
                    // check if the achievement index is valid for current page
                    if (relativeIdx < (endIdx - startIdx) && achievementIdx < MAX_ACHIEVEMENTS) {
                        clearScreen();
                        printf("%s%s=== ACHIEVEMENT DETAILS ===%s\n\n", STYLE_BOLD, COLOR_BRIGHT_MAGENTA, COLOR_RESET);
                        printf("%sName: %s%s%s\n", COLOR_WHITE, COLOR_BRIGHT_CYAN, state->achievements[achievementIdx].name, COLOR_RESET);
                        printf("%sStatus: ", COLOR_WHITE);
                        if (state->achievements[achievementIdx].isEarned) {
                            printf("%s%sEARNED%s", STYLE_BOLD, COLOR_BRIGHT_GREEN, COLOR_RESET);
                        } else {
                            printf("%sNOT EARNED%s", COLOR_RED, COLOR_RESET);
                        }
                        printf("\n");
                        if (state->achievements[achievementIdx].isEarned) {
                            printf("%sDate Earned: %s%s%s\n", COLOR_WHITE, COLOR_YELLOW, state->achievements[achievementIdx].dateEarned, COLOR_RESET);
                        }
                        printf("%sDescription: %s%s%s\n", COLOR_WHITE, COLOR_CYAN, state->achievements[achievementIdx].description, COLOR_RESET);
                        waitForKeypress();
                    }
                } else {
                    printf("%sInvalid choice. Please try again.%s\n", COLOR_RED, COLOR_RESET);
                    waitForKeypress();
                }
                break;
        }
    }
}

/**
 * Display the shop menu.
 * Precondition: state is a valid pointer to an initialized GameState struct.
 * @param state Pointer to the current GameState.
 */
void displayShopMenu(GameState *state) {
    char input;
    int exitShop = 0;
    
    while (!exitShop) {
        clearScreen();
        printf("%s%s=== HANAMARU'S SHOP ===%s\n", STYLE_BOLD, COLOR_BRIGHT_MAGENTA, COLOR_RESET);
        printf("%s\"Welcome to my shop, zura!\"%s\n\n", COLOR_CYAN, COLOR_RESET);
        printf("%sYour Gold: %s%d%s\n\n", COLOR_WHITE, COLOR_YELLOW, state->yohane.gold, COLOR_RESET);
        
        printf("%s[1]%s %sTears of a fallen angel %s(%s30g%s)%s - Heal 0.5 HP (Once per playthrough)\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_CYAN, COLOR_WHITE, COLOR_YELLOW, COLOR_WHITE, COLOR_RESET);
        printf("%s[2]%s %sNoppo bread %s(%s100g%s)%s - Heal 0.5 HP\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_YELLOW, COLOR_WHITE, COLOR_YELLOW, COLOR_WHITE, COLOR_RESET);
        printf("%s[3]%s %sShovel upgrade %s(%s300g%s)%s - No damage from spikes (Requires Kanan rescued)\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_WHITE, COLOR_WHITE, COLOR_YELLOW, COLOR_WHITE, COLOR_RESET);
        printf("%s[4]%s %sBat tamer %s(%s400g%s)%s - Reduce all bat damage to 0.5 (Requires Riko rescued)\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_RED, COLOR_WHITE, COLOR_YELLOW, COLOR_WHITE, COLOR_RESET);
        printf("%s[5]%s %sAir shoes %s(%s500g%s)%s - Walk on water and no heat damage (Requires You rescued)\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_BLUE, COLOR_WHITE, COLOR_YELLOW, COLOR_WHITE, COLOR_RESET);
        printf("%s[6]%s %sStewshine %s(%s1000g%s)%s - Increase max HP by 1.0 (Requires Mari rescued)\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_GREEN, COLOR_WHITE, COLOR_YELLOW, COLOR_WHITE, COLOR_RESET);
        printf("%s[7]%s %sMikan Mochi %s(%s1000g%s)%s - Increase max HP by 1.0 (Requires Chika rescued)\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_BRIGHT_YELLOW, COLOR_WHITE, COLOR_YELLOW, COLOR_WHITE, COLOR_RESET);
        printf("%s[8]%s %sKurosawa Macha %s(%s1000g%s)%s - Increase max HP by 1.0 (Requires Dia rescued)\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_MAGENTA, COLOR_WHITE, COLOR_YELLOW, COLOR_WHITE, COLOR_RESET);
        printf("%s[9]%s %sChoco-mint ice cream %s(%s2000g%s)%s - Auto-revive on fatal damage (Requires Ruby rescued)\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_BRIGHT_MAGENTA, COLOR_WHITE, COLOR_YELLOW, COLOR_WHITE, COLOR_RESET);
        printf("%s[B]%s %sBack to dungeon selection%s\n\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_WHITE, COLOR_RESET);
        printf("%sChoice: %s", COLOR_BRIGHT_CYAN, COLOR_RESET);
        
        scanf(" %c", &input);
        while (getchar() != '\n');  // clear input buffer
        
        switch (input) {
            case '1':
                if (state->tearsPurchasedThisPlaythrough >= 1) {
                    printf("%sTears of a fallen angel can only be purchased once per playthrough!%s\n", COLOR_YELLOW, COLOR_RESET);
                } else if (state->yohane.gold >= 30) {
                    state->yohane.gold -= 30;
                    state->totalGoldSpent += 30;
                    state->tearsCount++;
                    state->tearsPurchasedThisPlaythrough++;
                    printf("%sPurchased Tears of a fallen angel!%s\n", COLOR_BRIGHT_GREEN, COLOR_RESET);
                } else {
                    printf("%sNot enough gold!%s\n", COLOR_RED, COLOR_RESET);
                }
                waitForKeypress();
                break;
            case '2':
                if (state->yohane.gold >= 100) {
                    state->yohane.gold -= 100;
                    state->totalGoldSpent += 100;
                    state->noppoBreadCount++;
                    printf("%sPurchased Noppo bread!%s\n", COLOR_BRIGHT_GREEN, COLOR_RESET);
                } else {
                    printf("%sNot enough gold!%s\n", COLOR_RED, COLOR_RESET);
                }
                waitForKeypress();
                break;
            case '3':
                if (!state->aqours[IDOL_KANAN].isRescuedOnce) {
                    printf("%sYou need to rescue Kanan first!%s\n", COLOR_RED, COLOR_RESET);
                } else if (state->yohane.gold >= 300 && !state->yohane.hasShovelUpgrade) {
                    state->yohane.gold -= 300;
                    state->totalGoldSpent += 300;
                    state->yohane.hasShovelUpgrade = 1;
                    printf("%sPurchased Shovel upgrade!%s\n", COLOR_BRIGHT_GREEN, COLOR_RESET);
                } else if (state->yohane.hasShovelUpgrade) {
                    printf("%sYou already have this upgrade!%s\n", COLOR_YELLOW, COLOR_RESET);
                } else {
                    printf("%sNot enough gold!%s\n", COLOR_RED, COLOR_RESET);
                }
                waitForKeypress();
                break;
            case '4':
                if (!state->aqours[IDOL_RIKO].isRescuedOnce) {
                    printf("%sYou need to rescue Riko first!%s\n", COLOR_RED, COLOR_RESET);
                } else if (state->yohane.gold >= 400 && !state->yohane.hasBatTamer) {
                    state->yohane.gold -= 400;
                    state->totalGoldSpent += 400;
                    state->yohane.hasBatTamer = 1;
                    printf("%sPurchased Bat tamer!%s\n", COLOR_BRIGHT_GREEN, COLOR_RESET);
                } else if (state->yohane.hasBatTamer) {
                    printf("%sYou already have this upgrade!%s\n", COLOR_YELLOW, COLOR_RESET);
                } else {
                    printf("%sNot enough gold!%s\n", COLOR_RED, COLOR_RESET);
                }
                waitForKeypress();
                break;
            case '5':
                if (!state->aqours[IDOL_YOU].isRescuedOnce) {
                    printf("%sYou need to rescue You first!%s\n", COLOR_RED, COLOR_RESET);
                } else if (state->yohane.gold >= 500 && !state->yohane.hasAirShoes) {
                    state->yohane.gold -= 500;
                    state->totalGoldSpent += 500;
                    state->yohane.hasAirShoes = 1;
                    printf("%sPurchased Air shoes!%s\n", COLOR_BRIGHT_GREEN, COLOR_RESET);
                } else if (state->yohane.hasAirShoes) {
                    printf("%sYou already have this upgrade!%s\n", COLOR_YELLOW, COLOR_RESET);
                } else {
                    printf("%sNot enough gold!%s\n", COLOR_RED, COLOR_RESET);
                }
                waitForKeypress();
                break;
            case '6':
                if (!state->aqours[IDOL_MARI].isRescuedOnce) {
                    printf("%sYou need to rescue Mari first!%s\n", COLOR_RED, COLOR_RESET);
                } else if (state->yohane.gold >= 1000) {
                    state->yohane.gold -= 1000;
                    state->totalGoldSpent += 1000;
                    state->yohane.maxHp += 1.0f;
                    state->yohane.hp += 1.0f;  // also increase current HP
                    printf("%sPurchased Stewshine! Max HP increased by 1.0!%s\n", COLOR_BRIGHT_GREEN, COLOR_RESET);
                } else {
                    printf("%sNot enough gold!%s\n", COLOR_RED, COLOR_RESET);
                }
                waitForKeypress();
                break;
            case '7':
                if (!state->aqours[IDOL_CHIKA].isRescuedOnce) {
                    printf("%sYou need to rescue Chika first!%s\n", COLOR_RED, COLOR_RESET);
                } else if (state->yohane.gold >= 1000) {
                    state->yohane.gold -= 1000;
                    state->totalGoldSpent += 1000;
                    state->yohane.maxHp += 1.0f;
                    state->yohane.hp += 1.0f;  // also increase current HP
                    printf("%sPurchased Mikan Mochi! Max HP increased by 1.0!%s\n", COLOR_BRIGHT_GREEN, COLOR_RESET);
                } else {
                    printf("%sNot enough gold!%s\n", COLOR_RED, COLOR_RESET);
                }
                waitForKeypress();
                break;
            case '8':
                if (!state->aqours[IDOL_DIA].isRescuedOnce) {
                    printf("%sYou need to rescue Dia first!%s\n", COLOR_RED, COLOR_RESET);
                } else if (state->yohane.gold >= 1000) {
                    state->yohane.gold -= 1000;
                    state->totalGoldSpent += 1000;
                    state->yohane.maxHp += 1.0f;
                    state->yohane.hp += 1.0f;  // also increase current HP
                    printf("%sPurchased Kurosawa Macha! Max HP increased by 1.0!%s\n", COLOR_BRIGHT_GREEN, COLOR_RESET);
                } else {
                    printf("%sNot enough gold!%s\n", COLOR_RED, COLOR_RESET);
                }
                waitForKeypress();
                break;
            case '9':
                if (!state->aqours[IDOL_RUBY].isRescuedOnce) {
                    printf("%sYou need to rescue Ruby first!%s\n", COLOR_RED, COLOR_RESET);
                } else if (state->yohane.gold >= 2000) {
                    state->yohane.gold -= 2000;
                    state->totalGoldSpent += 2000;
                    state->chocoMintCount++;
                    printf("%sPurchased Choco-mint ice cream!%s\n", COLOR_BRIGHT_GREEN, COLOR_RESET);
                } else {
                    printf("%sNot enough gold!%s\n", COLOR_RED, COLOR_RESET);
                }
                waitForKeypress();
                break;
            case 'B':
            case 'b':
                exitShop = 1;
                break;
            default:
                printf("%sInvalid choice. Please try again.%s\n", COLOR_RED, COLOR_RESET);
                waitForKeypress();
                break;
        }
    }
}

/**
 * Shows the defeat screen with current dungeon state and cause of death
 * Implements proper Game Over mechanics according to specifications
 * @param state Pointer to the current game state
 * @param causeOfDeath String describing what killed the player
 */
void showDefeatScreen(GameState *state, const char *causeOfDeath) {
    clearScreen();
    
    // print defeat message
    printf("%s%sYohane Died!%s\n", STYLE_BOLD, COLOR_BRIGHT_RED, COLOR_RESET);
    printf("%sKilled by: %s%s\n\n", COLOR_RED, causeOfDeath, COLOR_RESET);
    
    // show dungeon info - we don't have filename, so use a generic approach
    const char *dungeonName = "Current Dungeon";
    
    printf("%s%s:%s %s%s%s\n", COLOR_WHITE, dungeonName, COLOR_RESET, COLOR_BRIGHT_CYAN, dungeonName, COLOR_RESET);
    printf("%sFloor %s%d%s of %s%d%s\n", COLOR_WHITE, COLOR_BRIGHT_YELLOW, state->currentDungeon.floor, COLOR_WHITE, COLOR_BRIGHT_YELLOW, state->currentDungeon.maxFloors, COLOR_RESET);
    printf("%sHP: %s%.1f%s / %s%.1f%s\n", COLOR_WHITE, COLOR_BRIGHT_RED, state->yohane.hp, COLOR_WHITE, COLOR_BRIGHT_GREEN, state->yohane.maxHp, COLOR_RESET);
    
    // show current item
    printf("%sItem on hand: %s", COLOR_WHITE, COLOR_RESET);
    switch (state->yohane.currentItemOnHand) {
        case ITEM_NOPPO:
            printf("%sNoppo Bread%s\n", COLOR_YELLOW, COLOR_RESET);
            break;
        case ITEM_TEARS:
            printf("%sTears of a Fallen Angel%s\n", COLOR_CYAN, COLOR_RESET);
            break;
        case ITEM_CHOCO_MINT:
            printf("%sChoco-mint Ice Cream%s\n", COLOR_BRIGHT_MAGENTA, COLOR_RESET);
            break;
        case ITEM_NONE:
        default:
            printf("%sNone%s\n", COLOR_WHITE, COLOR_RESET);
    }
    
    printf("%sTotal Gold: %s%d GP%s\n", COLOR_WHITE, COLOR_YELLOW, state->yohane.gold, COLOR_RESET);
    
    // show the dungeon layout
    renderDungeon(state);
    
    waitForKeypress();
    
    // honestly im confused on the proper specs, since its supposed to be roguelite but upgrades are not followed (sa specs)
    // handle Game Over mechanics according to specs
    // things that are preserved: rescued Aqours members, gold, achievements, consumables, PERMANENT UPGRADES
    // things that are reset: dungeon progress, current HP (but not max HP from Stewshine)
    
    // Don't reset permanent upgrades - these should carry over between playthroughs
    // state->yohane.hasShovelUpgrade = 0;  // Keep this upgrade
    // state->yohane.hasBatTamer = 0;       // Keep this upgrade
    // state->yohane.hasAirShoes = 0;       // Keep this upgrade
    
    // For Stewshine: Keep the max HP increase but reset current HP to max
    // This maintains the permanent nature of Stewshine while starting fresh
    if (state->yohane.maxHp > 3.0f) {
        // Player has purchased Stewshine, keep the increased max HP
        state->yohane.hp = state->yohane.maxHp;  // Start with full HP at the increased max
    } else {
        // No Stewshine purchased, reset to default
        state->yohane.hp = 3.0f;
        state->yohane.maxHp = 3.0f;
    }
    
    // reset current playthrough state
    state->isPlaythroughActive = 0;
    
    // reset dungeon progress for this playthrough
    state->dungeonsCleared[0] = 0;
    state->dungeonsCleared[1] = 0;
    state->dungeonsCleared[2] = 0;
    
    // reset current dungeon state to prevent "IN PROGRESS" display
    // added this since it fresh game > continue game > D3 F2 > Defeat > new game > continue D3 F2 (different IDOL already)
    state->currentDungeon.floor = 1;
    state->currentDungeon.maxFloors = 1;
    state->currentDungeon.difficulty = 0;
    state->currentDungeon.idolIndex = -1;
    
    // reset current item
    state->yohane.currentItemOnHand = ITEM_NONE;
    
    // display Game Over message as per specs
    printf("\n%s%s=== GAME OVER ===%s\n", STYLE_BOLD, COLOR_BRIGHT_RED, COLOR_RESET);
    printf("%sThe following are preserved:%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
    printf("%s- Rescued Aqours members%s\n", COLOR_WHITE, COLOR_RESET);
    printf("%s- Gold obtained%s\n", COLOR_WHITE, COLOR_RESET);
    printf("%s- Progress towards achievements%s\n", COLOR_WHITE, COLOR_RESET);
    printf("%s- Noppo bread and Tears of a Fallen Angel%s\n", COLOR_WHITE, COLOR_RESET);
    printf("%s- Permanent upgrades (Shovel, Bat Tamer, Air Shoes, Stewshine)%s\n", COLOR_BRIGHT_GREEN, COLOR_RESET);
    
    printf("\n%sThe following are reset:%s\n", COLOR_BRIGHT_RED, COLOR_RESET);
    printf("%s- Dungeon progress%s\n", COLOR_WHITE, COLOR_RESET);
    printf("%s- Current HP (but max HP from Stewshine is preserved)%s\n", COLOR_WHITE, COLOR_RESET);
    
    printf("\n%sPress Enter to return to main menu...%s", COLOR_BRIGHT_CYAN, COLOR_RESET);
    waitForKeypress();
}

/**
 * Show dungeon pause menu.
 * Precondition: state must be valid
 * @param state Pointer to the current GameState
 * @return 1 if player wants to quit, 0 to continue
 */
int showDungeonPauseMenu(GameState *state) {
    char choice;
    int result = 0; // default to continue
    
    clearScreen();
    printf("%s%s=== DUNGEON PAUSE MENU ===%s\n\n", STYLE_BOLD, COLOR_BRIGHT_MAGENTA, COLOR_RESET);
    printf("%s[C]%s %sContinue dungeon%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_WHITE, COLOR_RESET);
    printf("%s[I]%s %sView Inventory%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_WHITE, COLOR_RESET);
    printf("%s[S]%s %sSave and quit to dungeon selection%s\n\n", COLOR_BRIGHT_YELLOW, COLOR_RESET, COLOR_WHITE, COLOR_RESET);
    printf("%sChoice: %s", COLOR_BRIGHT_CYAN, COLOR_RESET);
    
    scanf(" %c", &choice);
    while (getchar() != '\n');  // clear input buffer
    
    switch (toupper(choice)) {
        case 'C':
            result = 0;  // continue
            break;
        case 'I':
            // show simplified inventory (no shop access in dungeon)
            clearScreen();
            printf("%s%s=== DUNGEON INVENTORY ===%s\n\n", STYLE_BOLD, COLOR_BRIGHT_MAGENTA, COLOR_RESET);
            printf("%sLailaps: These are the items you have, Yohane!%s\n\n", COLOR_CYAN, COLOR_RESET);
            
            printf("%sHP: ", COLOR_WHITE);
            if (state->yohane.hp <= state->yohane.maxHp * 0.3) {
                printf("%s%.1f%s", COLOR_BRIGHT_RED, state->yohane.hp, COLOR_RESET);
            } else if (state->yohane.hp >= state->yohane.maxHp * 0.8) {
                printf("%s%.1f%s", COLOR_BRIGHT_GREEN, state->yohane.hp, COLOR_RESET);
            } else {
                printf("%s%.1f%s", COLOR_YELLOW, state->yohane.hp, COLOR_RESET);
            }
            printf("%s/%.1f%s\n", COLOR_WHITE, state->yohane.maxHp, COLOR_RESET);
            printf("%sTotal Gold: %s%d G%s\n\n", COLOR_WHITE, COLOR_YELLOW, state->yohane.gold, COLOR_RESET);
            printf("%sItems available:%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
            
            printf("%s1. %sTears of a fallen angel: %s%d%s\n", COLOR_BRIGHT_YELLOW, COLOR_CYAN, COLOR_WHITE, state->tearsCount, COLOR_RESET);
            printf("%s2. %sNoppo bread: %s%d%s\n", COLOR_BRIGHT_YELLOW, COLOR_YELLOW, COLOR_WHITE, state->noppoBreadCount, COLOR_RESET);
            printf("%s3. %sChoco-mint ice cream: %s%d%s\n", COLOR_BRIGHT_YELLOW, COLOR_BRIGHT_MAGENTA, COLOR_WHITE, state->chocoMintCount, COLOR_RESET);
            
            printf("\n%sPress any key to return to dungeon...%s", COLOR_BRIGHT_CYAN, COLOR_RESET);
            waitForKeypress();
            result = 0;  // continue after showing inventory
            break;
        case 'S':
            result = 1;  // quit dungeon
            break;
        default:
            printf("%sInvalid choice.%s\n", COLOR_RED, COLOR_RESET);
            waitForKeypress();
            result = 0;  // continue by default
            break;
    }
    
    return result;
}

