/**
 * Description: Manages dungeon file loading, parsing, and rendering to console
 * Programmed by: 2ru17 (S14, PROG2-T3)
 * Last modified: 2025-07-25
 * Version: 1.8
 * Acknowledgements: dungeon
 */

#include "game_structs.h"
#include "utils.h"

/**
 * Load a dungeon layout from a text file.
 * Precondition: state pointer must be valid, filename must be a valid path
 * @param state Pointer to the GameState struct
 * @param filename Name of the file to load the dungeon from
 * @return 1 if successful, 0 if an error occurred
 */
int loadDungeonFromFile(GameState *state, const char* filename) {
    FILE *dungeonFile;          // dungeon file pointer
    int i, j;                  // loop counters
    int foundPlayer = 0;       // player position found flag
    int success = 0;           // operation success flag
    int fileError = 0;         // file read error flag
    int shouldContinueLoop = 1; // file reading loop control
    int len;                   // line length for processing
    int dungeonIndex, floorIndex;  // indices for treasure tracking
    
    // attempt to open dungeon file for reading
    dungeonFile = fopen(filename, "r");
    
    // check if file opened successfully
    if (dungeonFile != NULL) {
        // initialize/clear dungeon layout to prevent data from previous dungeons
        for (i = 0; i < DUNGEON_HEIGHT; i++) {
            for (j = 0; j < DUNGEON_WIDTH; j++) {
                state->currentDungeon.layout[i][j] = ' ';
            }
            state->currentDungeon.layout[i][DUNGEON_WIDTH] = '\0'; // null terminate each row
        }
        
        // reset enemy system - clear all bats from previous dungeon
        state->currentDungeon.enemyCount = 0;
        for (i = 0; i < MAX_BATS_PER_FLOOR; i++) {
            state->currentDungeon.enemies[i].isActive = 0;  // deactivate bat
            state->currentDungeon.enemies[i].pos.x = 0;
            state->currentDungeon.enemies[i].pos.y = 0;
        }
        
        // read each line into dungeon layout
        for (i = 0; i < DUNGEON_HEIGHT && !fileError && shouldContinueLoop; i++) {
            // read line from file
            if (fgets(state->currentDungeon.layout[i], DUNGEON_WIDTH + 1, dungeonFile) == NULL) {
                // end of file reached
                shouldContinueLoop = 0;
                i = DUNGEON_HEIGHT;  // exit loop
            } else {
                // remove newline character if present
                len = strlen(state->currentDungeon.layout[i]);
                if (len > 0 && state->currentDungeon.layout[i][len-1] == '\n') {
                    state->currentDungeon.layout[i][len-1] = '\0';
                    len--;  // update length after removing newline
                }
                
                // pad the line with spaces to DUNGEON_WIDTH if it's shorter
                for (j = len; j < DUNGEON_WIDTH; j++) {
                    state->currentDungeon.layout[i][j] = ' ';
                }
                
                // ensure null termination
                state->currentDungeon.layout[i][DUNGEON_WIDTH] = '\0';
                
                // scan the line for player 'Y', Lailaps 'L', and enemies 'b'
                for (j = 0; j < DUNGEON_WIDTH && state->currentDungeon.layout[i][j] != '\0'; j++) {
                    // check for player
                    if (state->currentDungeon.layout[i][j] == TILE_PLAYER) {
                        state->yohane.pos.x = j;
                        state->yohane.pos.y = i;
                        foundPlayer = 1;
                        
                        // check if the next character is 'L' for final battle (YL pair)
                        if (state->currentDungeon.difficulty == 4 && 
                            j + 1 < DUNGEON_WIDTH && 
                            state->currentDungeon.layout[i][j + 1] == TILE_LAILAPS) {
                            // set Lailaps position to be adjacent to Yohane
                            state->lailaps.pos.x = j + 1;
                            state->lailaps.pos.y = i;
                            // replace both characters with empty space
                            state->currentDungeon.layout[i][j] = TILE_EMPTY;
                            state->currentDungeon.layout[i][j + 1] = TILE_EMPTY;
                            // set underlying tiles
                            state->underlyingTile = TILE_EMPTY;
                            state->lailapsUnderlyingTile = TILE_EMPTY;
                            j++; // skip the next character since we processed it
                        } else {
                            // replace player tile with empty space in the layout
                            state->currentDungeon.layout[i][j] = TILE_EMPTY;
                            // set underlying tile to empty as well
                            state->underlyingTile = TILE_EMPTY;
                        }
                    }
                    // check for Lailaps (final battle only - difficulty 4, but not if already processed with Y)
                    else if (state->currentDungeon.layout[i][j] == TILE_LAILAPS && 
                             state->currentDungeon.difficulty == 4) {
                        state->lailaps.pos.x = j;
                        state->lailaps.pos.y = i;
                        // replace Lailaps tile with empty space in the layout
                        state->currentDungeon.layout[i][j] = TILE_EMPTY;
                        state->lailapsUnderlyingTile = TILE_EMPTY;
                    }
                    // check for enemy
                    else if (state->currentDungeon.layout[i][j] == TILE_BAT && 
                             state->currentDungeon.enemyCount < MAX_BATS_PER_FLOOR) {
                        
                        // initialize a new enemy
                        Enemy *enemy = &state->currentDungeon.enemies[state->currentDungeon.enemyCount];
                        enemy->pos.x = j;
                        enemy->pos.y = i;
                        enemy->isActive = 1;
                        enemy->underlyingTile = TILE_EMPTY;  // bats initially spawn on empty tiles
                        
                        // determine bat type based on dungeon difficulty
                        switch (state->currentDungeon.difficulty) {
                            case 1:  // first dungeon
                                enemy->type = BAT_TYPE_DUNGEON_1;
                                enemy->moveInterval = 2;  // moves every 2 turns
                                enemy->damage = 1;  // damage field unused - actual damage calculated based on difficulty
                                break;
                            case 2:  // second dungeon
                                enemy->type = BAT_TYPE_DUNGEON_2;
                                enemy->moveInterval = 1;  // moves every turn
                                enemy->damage = 2;  // damage field unused - actual damage calculated based on difficulty
                                break;
                            case 3:  // third dungeon
                                enemy->type = BAT_TYPE_DUNGEON_3;
                                enemy->moveInterval = 1;  // moves every turn
                                enemy->damage = 3;  // damage field unused - actual damage calculated based on difficulty
                                break;
                            default:
                                enemy->type = BAT_TYPE_DUNGEON_1;
                                enemy->moveInterval = 2;
                                enemy->damage = 1;
                                break;
                        }
                        
                        enemy->turnCounter = 0;
                        state->currentDungeon.enemyCount++;
                        
                        // replace bat tile with empty space in the layout to prevent ghost characters
                        state->currentDungeon.layout[i][j] = TILE_EMPTY;
                    }
                    // check for treasure tile - remove if already consumed
                    else if (state->currentDungeon.layout[i][j] == TILE_TREASURE) {
                        // check if treasure on this floor was already consumed
                        dungeonIndex = state->currentDungeon.difficulty - 1;  // 0-based index
                        floorIndex = state->currentDungeon.floor - 1;  // 0-based index
                        
                        if (dungeonIndex >= 0 && dungeonIndex < 3 && floorIndex >= 0 && floorIndex < 5) {
                            if (state->treasureConsumed[dungeonIndex][floorIndex]) {
                                // treasure already consumed - replace with empty space
                                state->currentDungeon.layout[i][j] = TILE_EMPTY;
                            }
                        }
                    }
                }
            }
        }
        
        // fill any remaining lines with empty spaces if file was shorter than DUNGEON_HEIGHT
        for (int fillLine = i; fillLine < DUNGEON_HEIGHT; fillLine++) {
            // fill with empty space and null terminate
            for (int fillCol = 0; fillCol < DUNGEON_WIDTH; fillCol++) {
                state->currentDungeon.layout[fillLine][fillCol] = ' ';
            }
            state->currentDungeon.layout[fillLine][DUNGEON_WIDTH] = '\0';
        }
        
        // store the actual content height (number of lines read from file)
        state->currentDungeon.contentHeight = i;
        
        // close the file
        fclose(dungeonFile);
        
        // check if player was found in the dungeon and no file errors occurred
        if (!fileError) {
            if (foundPlayer) {
                // validate player position bounds
                if (state->yohane.pos.y >= 0 && state->yohane.pos.y < state->currentDungeon.contentHeight &&
                    state->yohane.pos.x >= 0 && state->yohane.pos.x < DUNGEON_WIDTH) {
                    // don't place characters back on layout - they will be rendered dynamically
                    success = 1;
                } else {
                    printf("Error: Player position (%d, %d) is out of bounds in dungeon '%s'.\n", 
                           state->yohane.pos.x, state->yohane.pos.y, filename);
                    waitForKeypress();
                    fileError = 1;
                }
                
                // validate Lailaps position for final battle
                if (!fileError && state->currentDungeon.difficulty == 4 && 
                    state->lailaps.pos.x >= 0 && state->lailaps.pos.y >= 0 &&
                    state->lailaps.pos.y < state->currentDungeon.contentHeight &&
                    state->lailaps.pos.x < DUNGEON_WIDTH) {
                    // lailaps position is valid, don't place on layout
                }
            } else {
                printf("Error: Player 'Y' not found in dungeon file '%s'.\n", filename);
                printf("The dungeon file may be corrupted.\n");
                waitForKeypress();
            }
        }
    } else {
        printf("Error: Could not open dungeon file '%s'.\n", filename);
        printf("Please check that all game files are installed correctly.\n");
        waitForKeypress();
    }
    
    return success;
}

/**
 * Render the current dungeon to the console with colors and required information.
 * Precondition: state pointer must be valid and currentDungeon must be loaded
 * @param state Pointer to the GameState struct
 */
void renderDungeon(GameState *state) {
    int maxLines;
    char charToPrint;
    int batFound;
    int i, j, k;
    
    clearScreen();
    
    // display dungeon information according to specs
    // dungeon no. and name of the stage
    if (state->currentDungeon.difficulty == 4) {
        // final battle - show special name
        printf("%sFinal Battle: %s%s%s\n", COLOR_WHITE, 
               COLOR_BRIGHT_RED, "Face the Siren!", COLOR_RESET);
    } else {
        printf("%sDungeon #%d: %s%s%s\n", COLOR_WHITE, state->currentDungeon.difficulty, 
               COLOR_BRIGHT_CYAN, state->aqours[state->currentDungeon.idolIndex].dungeonName, COLOR_RESET);
    }
    
    
    printf("%sFloor %d of %d%s\n", COLOR_BRIGHT_YELLOW, state->currentDungeon.floor, 
           state->currentDungeon.maxFloors, COLOR_RESET);
    
   
    printf("%sHP: ", COLOR_WHITE);
    if (state->yohane.hp <= state->yohane.maxHp * 0.3) {
        printf("%s%.1f%s", COLOR_BRIGHT_RED, state->yohane.hp, COLOR_RESET);
    } else if (state->yohane.hp >= state->yohane.maxHp * 0.8) {
        printf("%s%.1f%s", COLOR_BRIGHT_GREEN, state->yohane.hp, COLOR_RESET);
    } else {
        printf("%s%.1f%s", COLOR_YELLOW, state->yohane.hp, COLOR_RESET);
    }
    printf("%s / %.1f%s\t", COLOR_WHITE, state->yohane.maxHp, COLOR_RESET);
    
    
    printf("%sItem on hand: %s", COLOR_WHITE, COLOR_RESET);
    switch (state->yohane.currentItemOnHand) {
        case ITEM_NONE:
            printf("%sNone%s\t", COLOR_WHITE, COLOR_RESET);
            break;
        case ITEM_TEARS:
            printf("%sTears of a fallen angel%s\t", COLOR_CYAN, COLOR_RESET);
            break;
        case ITEM_NOPPO:
            printf("%sNoppo bread%s\t", COLOR_YELLOW, COLOR_RESET);
            break;
        case ITEM_CHOCO_MINT:
            printf("%sChoco-mint ice cream%s\t", COLOR_MAGENTA, COLOR_RESET);
            break;
        default:
            printf("%sUnknown%s\t", COLOR_RED, COLOR_RESET);
            break;
    }
    
    // 5.) Total Gold
    printf("%sTotal Gold: %s%d%s", COLOR_WHITE, COLOR_YELLOW, state->yohane.gold, COLOR_RESET);
    
    
    // printf("%sMoves: %s%d%s\n\n", COLOR_WHITE, COLOR_BRIGHT_WHITE, state->moveCounter, COLOR_RESET);
    printf("\n\n");
    
    // display the dungeon layout with colors
    maxLines = state->currentDungeon.contentHeight;
    if (maxLines > DUNGEON_HEIGHT) maxLines = DUNGEON_HEIGHT;  // safety check
    if (maxLines < 0) maxLines = 0;  // additional safety check
    
    for (i = 0; i < maxLines; i++) {
        for (j = 0; state->currentDungeon.layout[i][j] != '\0' && j < DUNGEON_WIDTH; j++) {
            charToPrint = state->currentDungeon.layout[i][j];
            
            // check if both Yohane and Lailaps are at this position (final battle only - this should not happen but handle gracefully)
            if (state->currentDungeon.difficulty == 4 && 
                i == state->yohane.pos.y && j == state->yohane.pos.x &&
                i == state->lailaps.pos.y && j == state->lailaps.pos.x) {
                // both characters at same position - prioritize Yohane but this indicates a bug
                charToPrint = TILE_PLAYER;
            }
            // check if Yohane is at this position
            else if (i == state->yohane.pos.y && j == state->yohane.pos.x) {
                charToPrint = TILE_PLAYER;
            }
            // check if Lailaps is at this position (final battle only)
            else if (state->currentDungeon.difficulty == 4 && 
                     i == state->lailaps.pos.y && j == state->lailaps.pos.x) {
                charToPrint = TILE_LAILAPS;
            }
            // check if any bat is at this position
            else {
                batFound = 0;
                for (k = 0; k < state->currentDungeon.enemyCount && !batFound; k++) {
                    if (state->currentDungeon.enemies[k].isActive &&
                        i == state->currentDungeon.enemies[k].pos.y && 
                        j == state->currentDungeon.enemies[k].pos.x) {
                        charToPrint = TILE_BAT;
                        batFound = 1;
                    }
                }
            }
            
            printColoredChar(charToPrint);
        }
        printf("\n");
    }
    
    // display controls with colors
    printf("\n%sControls: %sWASD%s to move, %s[ ]%s to cycle items, %sSpace%s to use item, %sESC%s for menu\n", 
           COLOR_WHITE, COLOR_BRIGHT_CYAN, COLOR_WHITE, COLOR_BRIGHT_CYAN, COLOR_WHITE, 
           COLOR_BRIGHT_CYAN, COLOR_WHITE, COLOR_BRIGHT_CYAN, COLOR_RESET);
}