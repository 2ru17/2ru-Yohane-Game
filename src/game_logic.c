/**
 * Description: Core game logic including menus, battle system, and game flow
 * Programmed by: 2ru17 (S14, PROG2-T3)
 * Last modified: 2025-07-25
 * Version: 1.8
 * Acknowledgements: This code is based on the project specifications provided.
 */

#include "game_structs.h"
#include "utils.h"

/**
 * Run the final battle against the Siren.
 * Precondition: state is a valid pointer to an initialized GameState struct.
 * @param state Pointer to the current GameState.
 * @return 1 if victorious, 0 if defeated or quit.
 */
int runFinalBattle(GameState *state) {
    char filename[100];
    char input;
    int battleWon = 0;
    int exitBattle = 0;
    int phase = 1;
    int sirenX = 0, sirenY = 0;
    int originalSirenX = 0, originalSirenY = 0;
    int loadSuccess = 0;
    int movesSinceLastBat = 0;
    int newBatType = BAT_TYPE_DUNGEON_1;
    int sirenFound = 0;
    int x, y;
    int directions[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    int separated = 0;
    int i, newX, newY;
    char tile;
    char yohaneUnderlying, lailapsUnderlying;
    const char* causeOfDeath;

    
    // set final battle parameters
    state->currentDungeon.difficulty = 4;
    state->currentDungeon.floor = 1;
    state->currentDungeon.maxFloors = 1;
    
    // load final battle dungeon
    strncpy(filename, "assets/dungeon-env/dungeons_final_battle.txt", sizeof(filename) - 1);
    filename[sizeof(filename) - 1] = '\0';  // ensure null termination
    loadSuccess = loadDungeonFromFile(state, filename);
    
    if (loadSuccess) {
        // show loading tip before battle
        showLoadingTip();
        
        // reset battle state to ensure clean initialization
        state->turnCounter = 0;
        state->finalBattleSwitchesHit = 0;  // Initialize to 0 for Phase 1 (no switches pressed)
        state->heatTileCounter = 0;
        movesSinceLastBat = 0;
        
        // reset lailaps hp for battle
        state->lailaps.hp = state->lailaps.maxHp;
        
        // find siren position
        sirenFound = 0;
        
        for (y = 0; y < DUNGEON_HEIGHT && !sirenFound; y++) {
            for (x = 0; x < DUNGEON_WIDTH && !sirenFound; x++) {
                if (state->currentDungeon.layout[y][x] == TILE_SIREN) {
                    sirenX = x;         // store siren x
                    sirenY = y;         // store siren y
                    originalSirenX = x;  // store original siren x for exit placement
                    originalSirenY = y;  // store original siren y for exit placement
                    sirenFound = 1;     // exit loops
                }
            }
        }
        
        // spawn first switch pair
        spawnRandomSwitchPair(state);
        
        // safety check: ensure Yohane and Lailaps are not at the same position
        if (state->yohane.pos.x == state->lailaps.pos.x && state->yohane.pos.y == state->lailaps.pos.y) {
            separated = 0;
            
            printf("%sWarning: Characters started at same position! Separating them...%s\n", COLOR_YELLOW, COLOR_RESET);
            
            // try to move Lailaps to an adjacent empty position
            for (i = 0; i < 4 && !separated; i++) {
                newX = state->lailaps.pos.x + directions[i][0];
                newY = state->lailaps.pos.y + directions[i][1];
                
                if (newX >= 0 && newX < DUNGEON_WIDTH && newY >= 0 && newY < DUNGEON_HEIGHT) {
                    tile = state->currentDungeon.layout[newY][newX];
                    if (tile == TILE_EMPTY || tile == TILE_SWITCH) {
                        state->lailaps.pos.x = newX;
                        state->lailaps.pos.y = newY;
                        separated = 1;
                        printf("%sLailaps moved to adjacent position (%d, %d).%s\n", COLOR_GREEN, newX, newY, COLOR_RESET);
                    }
                }
            }
            
            if (!separated) {
                printf("%sError: Could not separate characters! Game may be unstable.%s\n", COLOR_RED, COLOR_RESET);
            }
            waitForKeypress();
        }
        
        // initialize underlying tile tracking
        yohaneUnderlying = state->currentDungeon.layout[state->yohane.pos.y][state->yohane.pos.x];
        lailapsUnderlying = state->currentDungeon.layout[state->lailaps.pos.y][state->lailaps.pos.x];
        state->underlyingTile = yohaneUnderlying;
        state->lailapsUnderlyingTile = lailapsUnderlying;
        
        // main battle loop
        while (!exitBattle && loadSuccess) {
            // check character death
            if (state->yohane.hp <= 0 || state->lailaps.hp <= 0) {
                if (state->yohane.hp <= 0) {
                    if (strlen(state->lastDamageCause) > 0) {
                        causeOfDeath = state->lastDamageCause;
                    } else {
                        causeOfDeath = "Unknown";
                    }
                } else {
                    causeOfDeath = "Bat";
                }
                showDefeatScreen(state, causeOfDeath);
                exitBattle = 1;
                battleWon = 0;
            }
            
            // only continue if battle hasn't ended
            if (!exitBattle) {
                // render the dungeon
                renderDungeon(state);
            
            // display battle info
            if (phase == 1) {
                printf("%sPhase 1: Break the barrier! (Switches paired: %d/3)%s\n", 
                       COLOR_BRIGHT_YELLOW, state->finalBattleSwitchesHit, COLOR_RESET);
                printf("%sLailaps HP: %s%.1f/%.1f%s\n", 
                       COLOR_WHITE, COLOR_BRIGHT_GREEN, state->lailaps.hp, state->lailaps.maxHp, COLOR_RESET);
            } else {
                printf("%sPhase 2: Defeat the Siren!%s\n", COLOR_BRIGHT_RED, COLOR_RESET);
                printf("%sLailaps HP: %s%.1f/%.1f%s\n", 
                       COLOR_WHITE, COLOR_BRIGHT_GREEN, state->lailaps.hp, state->lailaps.maxHp, COLOR_RESET);
            }
            
            // get player input (this will block until input is received)
            input = getCharNonBlocking();
            
            // process input
            int moveMade = 0;
            
            switch (toupper(input)) {
                case 'W':  // move both characters up
                    {
                        // first check if Yohane can attack in this direction
                        int attackResult = tryAttackInDirection(state, 0, -1, &sirenX, &sirenY, originalSirenX, originalSirenY, phase);
                        if (attackResult == 1) {
                            // attack occurred, set moveMade
                            moveMade = 1;
                        } else if (attackResult == 0) {
                            // no attack, check if Siren would attack after this movement
                            int sirenAttackResult = wouldSirenAttackAfterMovement(state, 0, -1, sirenX, sirenY, phase);
                            if (sirenAttackResult == 2) {
                                // Siren attacks Lailaps - instant game over
                                printf("%s%sThe Siren attacks Lailaps with a piercing scream!%s\n", STYLE_BOLD, COLOR_BRIGHT_RED, COLOR_RESET);
                                printf("%s%sLailaps is defeated! Game Over!%s\n", STYLE_BOLD, COLOR_BRIGHT_RED, COLOR_RESET);
                                waitForKeypress();
                                
                                // set Lailaps HP to 0 and track damage cause
                                state->lailaps.hp = 0;
                                setDamageCause(state, "Siren (killed Lailaps)");
                                
                                // show defeat screen and end battle
                                showDefeatScreen(state, "Siren (killed Lailaps)");
                                exitBattle = 1;
                                battleWon = 0;
                            } else if (sirenAttackResult == 1) {
                                // Siren would attack Yohane - apply fatal damage
                                float damage = state->yohane.maxHp + 100000.0f;
                                state->yohane.hp -= damage;
                                state->damageTakenThisRun = 1;
                                setDamageCause(state, "Siren");
                                
                                printf("%s%sYohane moves into the Siren's attack range!%s\n", STYLE_BOLD, COLOR_BRIGHT_RED, COLOR_RESET);
                                printf("%s%sThe Siren attacks with a piercing scream! %s-%.1f HP%s\n", 
                                       STYLE_BOLD, COLOR_BRIGHT_RED, COLOR_BRIGHT_RED, damage, COLOR_RESET);
                                
                                // check for fatal damage and handle choco-mint saves
                                if (checkFatalDamageAndChocoMintSave(state)) {
                                    // Yohane died - end battle
                                    exitBattle = 1;
                                    battleWon = 0;
                                }
                                moveMade = 1;
                                waitForKeypress();
                            } else {
                                // safe to move normally
                                int moveResult = moveCharactersInFinalBattle(state, 0, -1);
                                if (moveResult == 2) {
                                    // Player reached exit after defeating Siren
                                    battleWon = 1;
                                    exitBattle = 1;
                                } else {
                                    moveMade = moveResult;
                                }
                            }
                        }
                        // attackResult == -1 means battle ended due to attack outcome
                    }
                    break;
                case 'A':  // move both characters left
                    {
                        // first check if Yohane can attack in this direction
                        int attackResult = tryAttackInDirection(state, -1, 0, &sirenX, &sirenY, originalSirenX, originalSirenY, phase);
                        if (attackResult == 1) {
                            // attack occurred, set moveMade
                            moveMade = 1;
                        } else if (attackResult == 0) {
                            // no attack, check if Siren would attack after this movement
                            int sirenAttackResult = wouldSirenAttackAfterMovement(state, -1, 0, sirenX, sirenY, phase);
                            if (sirenAttackResult == 2) {
                                // Siren attacks Lailaps - instant game over
                                printf("%s%sThe Siren attacks Lailaps with a piercing scream!%s\n", STYLE_BOLD, COLOR_BRIGHT_RED, COLOR_RESET);
                                printf("%s%sLailaps is defeated! Game Over!%s\n", STYLE_BOLD, COLOR_BRIGHT_RED, COLOR_RESET);
                                waitForKeypress();
                                
                                // set Lailaps HP to 0 and track damage cause
                                state->lailaps.hp = 0;
                                setDamageCause(state, "Siren (killed Lailaps)");
                                
                                // show defeat screen and end battle
                                showDefeatScreen(state, "Siren (killed Lailaps)");
                                exitBattle = 1;
                                battleWon = 0;
                            } else if (sirenAttackResult == 1) {
                                // Siren would attack Yohane - apply fatal damage
                                float damage = state->yohane.maxHp + 100000.0f;
                                state->yohane.hp -= damage;
                                state->damageTakenThisRun = 1;
                                setDamageCause(state, "Siren");
                                
                                printf("%s%sYohane moves into the Siren's attack range!%s\n", STYLE_BOLD, COLOR_BRIGHT_RED, COLOR_RESET);
                                printf("%s%sThe Siren attacks with a piercing scream! %s-%.1f HP%s\n", 
                                       STYLE_BOLD, COLOR_BRIGHT_RED, COLOR_BRIGHT_RED, damage, COLOR_RESET);
                                
                                // check for fatal damage and handle choco-mint saves
                                if (checkFatalDamageAndChocoMintSave(state)) {
                                    // Yohane died - end battle
                                    exitBattle = 1;
                                    battleWon = 0;
                                }
                                moveMade = 1;
                                waitForKeypress();
                            } else {
                                // safe to move normally
                                int moveResult = moveCharactersInFinalBattle(state, -1, 0);
                                if (moveResult == 2) {
                                    // Player reached exit after defeating Siren
                                    battleWon = 1;
                                    exitBattle = 1;
                                } else {
                                    moveMade = moveResult;
                                }
                            }
                        }
                        // attackResult == -1 means battle ended due to attack outcome
                    }
                    break;
                case 'S':  // move both characters down
                    {
                        // first check if Yohane can attack in this direction
                        int attackResult = tryAttackInDirection(state, 0, 1, &sirenX, &sirenY, originalSirenX, originalSirenY, phase);
                        if (attackResult == 1) {
                            // attack occurred, set moveMade
                            moveMade = 1;
                        } else if (attackResult == 0) {
                            // no attack, check if Siren would attack after this movement
                            int sirenAttackResult = wouldSirenAttackAfterMovement(state, 0, 1, sirenX, sirenY, phase);
                            if (sirenAttackResult == 2) {
                                // Siren attacks Lailaps - instant game over
                                printf("%s%sThe Siren attacks Lailaps with a piercing scream!%s\n", STYLE_BOLD, COLOR_BRIGHT_RED, COLOR_RESET);
                                printf("%s%sLailaps is defeated! Game Over!%s\n", STYLE_BOLD, COLOR_BRIGHT_RED, COLOR_RESET);
                                waitForKeypress();
                                
                                // set Lailaps HP to 0 and track damage cause
                                state->lailaps.hp = 0;
                                setDamageCause(state, "Siren (killed Lailaps)");
                                
                                // show defeat screen and end battle
                                showDefeatScreen(state, "Siren (killed Lailaps)");
                                exitBattle = 1;
                                battleWon = 0;
                            } else if (sirenAttackResult == 1) {
                                // Siren would attack Yohane - apply fatal damage
                                float damage = state->yohane.maxHp + 100000.0f;
                                state->yohane.hp -= damage;
                                state->damageTakenThisRun = 1;
                                setDamageCause(state, "Siren");
                                
                                printf("%s%sYohane moves into the Siren's attack range!%s\n", STYLE_BOLD, COLOR_BRIGHT_RED, COLOR_RESET);
                                printf("%s%sThe Siren attacks with a piercing scream! %s-%.1f HP%s\n", 
                                       STYLE_BOLD, COLOR_BRIGHT_RED, COLOR_BRIGHT_RED, damage, COLOR_RESET);
                                
                                // check for fatal damage and handle choco-mint saves
                                if (checkFatalDamageAndChocoMintSave(state)) {
                                    // Yohane died - end battle
                                    exitBattle = 1;
                                    battleWon = 0;
                                }
                                moveMade = 1;
                                waitForKeypress();
                            } else {
                                // safe to move normally
                                int moveResult = moveCharactersInFinalBattle(state, 0, 1);
                                if (moveResult == 2) {
                                    // Player reached exit after defeating Siren
                                    battleWon = 1;
                                    exitBattle = 1;
                                } else {
                                    moveMade = moveResult;
                                }
                            }
                        }
                        // attackResult == -1 means battle ended due to attack outcome
                    }
                    break;
                case 'D':  // move both characters right
                    {
                        // first check if Yohane can attack in this direction
                        int attackResult = tryAttackInDirection(state, 1, 0, &sirenX, &sirenY, originalSirenX, originalSirenY, phase);
                        if (attackResult == 1) {
                            // attack occurred, set moveMade
                            moveMade = 1;
                        } else if (attackResult == 0) {
                            // no attack, check if Siren would attack after this movement
                            int sirenAttackResult = wouldSirenAttackAfterMovement(state, 1, 0, sirenX, sirenY, phase);
                            if (sirenAttackResult == 2) {
                                // Siren attacks Lailaps - instant game over
                                printf("%s%sThe Siren attacks Lailaps with a piercing scream!%s\n", STYLE_BOLD, COLOR_BRIGHT_RED, COLOR_RESET);
                                printf("%s%sLailaps is defeated! Game Over!%s\n", STYLE_BOLD, COLOR_BRIGHT_RED, COLOR_RESET);
                                waitForKeypress();
                                
                                // set Lailaps HP to 0 and track damage cause
                                state->lailaps.hp = 0;
                                setDamageCause(state, "Siren (killed Lailaps)");
                                
                                // show defeat screen and end battle
                                showDefeatScreen(state, "Siren (killed Lailaps)");
                                exitBattle = 1;
                                battleWon = 0;
                            } else if (sirenAttackResult == 1) {
                                // Siren would attack Yohane - apply fatal damage
                                float damage = state->yohane.maxHp + 100000.0f;
                                state->yohane.hp -= damage;
                                state->damageTakenThisRun = 1;
                                setDamageCause(state, "Siren");
                                
                                printf("%s%sYohane moves into the Siren's attack range!%s\n", STYLE_BOLD, COLOR_BRIGHT_RED, COLOR_RESET);
                                printf("%s%sThe Siren attacks with a piercing scream! %s-%.1f HP%s\n", 
                                       STYLE_BOLD, COLOR_BRIGHT_RED, COLOR_BRIGHT_RED, damage, COLOR_RESET);
                                
                                // check for fatal damage and handle choco-mint saves
                                if (checkFatalDamageAndChocoMintSave(state)) {
                                    // Yohane died - end battle
                                    exitBattle = 1;
                                    battleWon = 0;
                                }
                                moveMade = 1;
                                waitForKeypress();
                            } else {
                                // safe to move normally
                                int moveResult = moveCharactersInFinalBattle(state, 1, 0);
                                if (moveResult == 2) {
                                    // Player reached exit after defeating Siren
                                    battleWon = 1;
                                    exitBattle = 1;
                                } else {
                                    moveMade = moveResult;
                                }
                            }
                        }
                        // attackResult == -1 means battle ended due to attack outcome
                    }
                    break;

                case '[':  // previous item
                    cycleItem(state, -1);
                    moveMade = 1;
                    break;
                case ']':  // next item
                    cycleItem(state, 1);
                    moveMade = 1;
                    break;
                case ' ':  // use item
                    if (state->yohane.currentItemOnHand != ITEM_NONE) {
                        useItem(state);
                    }
                    moveMade = 1;
                    break;
                case 27:  // eSC key
                case 'M':  // menu key (alternative to ESC)
                    if (showDungeonPauseMenu(state)) {
                        exitBattle = 1;
                    }
                    break;
                default:
                    // any other input makes character stay in place
                    printf("%sInvalid input - staying in place.%s\n", COLOR_YELLOW, COLOR_RESET);
                    moveMade = 1;
                    waitForKeypress();
                    break;
            }
            
            // process turn if a move was made
            if (moveMade) {
                // handle heat tiles and other turn-based mechanics
                processTurn(state, moveMade);
                
                movesSinceLastBat++;
                // check if both characters are on switches (phase 1)
                if (phase == 1) {
                    // check if both characters are standing on switches using underlying tiles
                    int yohaneOnSwitch = 0;
                    int lailapsOnSwitch = 0;
                    
                    // check what each character is standing on
                    if (state->underlyingTile == TILE_SWITCH) {
                        yohaneOnSwitch = 1;
                    }
                    
                    if (state->lailapsUnderlyingTile == TILE_SWITCH) {
                        lailapsOnSwitch = 1;
                    }
                    
                    if (yohaneOnSwitch && lailapsOnSwitch) {
                        // switch pair activated
                        state->finalBattleSwitchesHit++;
                        printf("%s%sSwitch pair activated! (%d/3)%s\n", STYLE_BOLD, COLOR_BRIGHT_YELLOW, state->finalBattleSwitchesHit, COLOR_RESET);
                        
                        // determine new bat type based on switches hit
                        switch (state->finalBattleSwitchesHit) {
                            case 1:
                                newBatType = BAT_TYPE_DUNGEON_2;  // Phase 2: 1 switch pair activated
                                break;
                            case 2:
                            case 3:
                                newBatType = BAT_TYPE_DUNGEON_3;  // Phase 3: 2+ switch pairs activated
                                break;
                            default:
                                newBatType = BAT_TYPE_DUNGEON_2;
                                break;
                        }
                        
                        // update all existing active bats to the new type
                        for (int i = 0; i < state->currentDungeon.enemyCount; i++) {
                            if (state->currentDungeon.enemies[i].isActive) {
                                state->currentDungeon.enemies[i].type = newBatType;
                                // update bat properties based on new type
                                switch (newBatType) {
                                    case BAT_TYPE_DUNGEON_1:
                                        state->currentDungeon.enemies[i].moveInterval = 2;
                                        state->currentDungeon.enemies[i].damage = 1;
                                        break;
                                    case BAT_TYPE_DUNGEON_2:
                                        state->currentDungeon.enemies[i].moveInterval = 1;
                                        state->currentDungeon.enemies[i].damage = 2;
                                        break;
                                    case BAT_TYPE_DUNGEON_3:
                                        state->currentDungeon.enemies[i].moveInterval = 1;
                                        state->currentDungeon.enemies[i].damage = 3;
                                        break;
                                }
                            }
                        }
                        
                        waitForKeypress();
                        
                        // clear current switches
                        clearAllSwitches(state);
                        
                        // update underlying tiles since switches are cleared
                        state->underlyingTile = TILE_EMPTY;
                        state->lailapsUnderlyingTile = TILE_EMPTY;
                        
                        // check if phase 1 is complete
                        if (state->finalBattleSwitchesHit >= 3) {
                            phase = 2;
                            printf("%s%sThe barrier is down! The Siren is vulnerable!%s\n", STYLE_BOLD, COLOR_BRIGHT_RED, COLOR_RESET);
                            
                            // remove barriers from siren chamber (left wall, right wall, and bottom wall)
                            // based on dungeon layout: remove specific barrier positions to open the chamber
                            
                            // remove left barrier walls (column 22, rows 1-2)
                            if (state->currentDungeon.layout[1][22] == TILE_BORDER) {
                                state->currentDungeon.layout[1][22] = TILE_EMPTY;
                            }
                            if (state->currentDungeon.layout[2][22] == TILE_BORDER) {
                                state->currentDungeon.layout[2][22] = TILE_EMPTY;
                            }
                            
                            // remove right barrier walls (column 39, rows 1-2) 
                            if (state->currentDungeon.layout[1][39] == TILE_BORDER) {
                                state->currentDungeon.layout[1][39] = TILE_EMPTY;
                            }
                            if (state->currentDungeon.layout[2][39] == TILE_BORDER) {
                                state->currentDungeon.layout[2][39] = TILE_EMPTY;
                            }
                            
                            // remove bottom barrier wall (row 3, columns 22-39)
                            for (int x = 22; x <= 39; x++) {
                                if (state->currentDungeon.layout[3][x] == TILE_BORDER) {
                                    state->currentDungeon.layout[3][x] = TILE_EMPTY;
                                }
                            }
                            waitForKeypress();
                        } else {
                            // spawn next pair of switches
                            spawnRandomSwitchPair(state);
                        }
                    }
                }
                
                // phase 2: siren movement and combat (only if siren is still alive)
                //  S attacks Y first - Player loses (if no choco mint) 
                // Y attacks S first - Player wins and all bats die + exit spawns 
                // S attacks Y first but we have choco mint - Player survives, can counter-attack to win, and Siren won't continue attacking
                if (phase == 2 && !state->finalBattleWon) {
                    // move siren towards player
                    moveSirenTowardsPlayer(state, &sirenX, &sirenY);
                    
                    // check if Lailaps is within Siren's attack range after Siren movement
                    if (sirenX >= 0 && sirenY >= 0 && arePositionsAdjacent(state->lailaps.pos.x, state->lailaps.pos.y, sirenX, sirenY)) {
                        // Siren attacks Lailaps - instant game over
                        printf("%s%sThe Siren attacks Lailaps with a piercing scream!%s\n", STYLE_BOLD, COLOR_BRIGHT_RED, COLOR_RESET);
                        printf("%s%sLailaps is defeated! Game Over!%s\n", STYLE_BOLD, COLOR_BRIGHT_RED, COLOR_RESET);
                        waitForKeypress();
                        
                        // set Lailaps HP to 0 and track damage cause
                        state->lailaps.hp = 0;
                        setDamageCause(state, "Siren (killed Lailaps)");
                        
                        // show defeat screen and end battle
                        showDefeatScreen(state, "Siren (killed Lailaps)");
                        exitBattle = 1;
                        battleWon = 0;
                    }
                }
                
                // process enemy/siren turns if battle hasn't ended
                if (!exitBattle) {
                    // increment turn counter
                    state->turnCounter++;
                    
                    // spawn bats every 8 moves (only in phase 1)
                    if (phase == 1 && movesSinceLastBat >= 8) {
                        spawnBatInFinalBattle(state);
                        movesSinceLastBat = 0;
                    }
                    
                    // process enemy turns
                    // first update all active bats' behavior based on current switches pressed
                    for (int i = 0; i < state->currentDungeon.enemyCount; i++) {
                        if (state->currentDungeon.enemies[i].isActive) {
                            // update moveInterval based on current phase
                            if (state->finalBattleSwitchesHit == 0) {
                                state->currentDungeon.enemies[i].moveInterval = 2;  // Phase 1: every 2 turns
                            } else {
                                state->currentDungeon.enemies[i].moveInterval = 1;  // Phase 2+: every turn
                            }
                        }
                    }
                    
                    // move all active bats
                    // reset attack flags for this turn
                    static int turnBatAttackedYohane = 0;
                    static int turnBatAttackedLailaps = 0;
                    turnBatAttackedYohane = 0;
                    turnBatAttackedLailaps = 0;
                    
                    for (int i = 0; i < state->currentDungeon.enemyCount && !exitBattle; i++) {
                        if (state->currentDungeon.enemies[i].isActive) {
                            // store previous position to check if bat moved to adjacent position
                            int oldX = state->currentDungeon.enemies[i].pos.x;
                            int oldY = state->currentDungeon.enemies[i].pos.y;
                            
                            moveBatInFinalBattle(state, i, &turnBatAttackedYohane, &turnBatAttackedLailaps);
                            
                            // check if bat moved to adjacent position and should attack
                            // only attack if bat just moved into adjacent position this turn
                            int batMoved = (oldX != state->currentDungeon.enemies[i].pos.x || 
                                           oldY != state->currentDungeon.enemies[i].pos.y);
                            
                            if (batMoved && isPositionAdjacentToYohaneOrLailaps(state, 
                                                                state->currentDungeon.enemies[i].pos.x, 
                                                                state->currentDungeon.enemies[i].pos.y)) {
                                // bat attacks after moving to adjacent position
                                float damage = state->currentDungeon.enemies[i].damage * 0.5f;
                                
                                // apply bat tamer upgrade
                                if (state->yohane.hasBatTamer) {
                                    damage = 0.5f;
                                }
                                
                                // determine who was attacked
                                if (arePositionsAdjacent(state->yohane.pos.x, state->yohane.pos.y,
                                                       state->currentDungeon.enemies[i].pos.x,
                                                       state->currentDungeon.enemies[i].pos.y)) {
                                    // yohane was attacked
                                    state->yohane.hp -= damage;
                                    state->damageTakenThisRun = 1;
                                    setDamageCause(state, "Bat");  // track damage source
                                    printf("%sYohane was attacked by a bat! %s-%.1f HP%s\n", COLOR_RED, COLOR_BRIGHT_RED, damage, COLOR_RESET);
                                    
                                    // check for fatal damage and handle choco-mint saves
                                    if (checkFatalDamageAndChocoMintSave(state)) {
                                        // Yohane died - end battle
                                        exitBattle = 1;
                                        battleWon = 0;
                                    }
                                } else {
                                    // lailaps was attacked
                                    state->lailaps.hp -= damage;
                                    printf("%sLailaps was attacked by a bat! %s-%.1f HP%s\n", COLOR_RED, COLOR_BRIGHT_RED, damage, COLOR_RESET);
                                    
                                    // check if Lailaps is defeated
                                    if (state->lailaps.hp <= 0) {
                                        showDefeatScreen(state, "Bat");
                                        exitBattle = 1;
                                        battleWon = 0;
                                    }
                                }
                                if (!exitBattle) {
                                    waitForKeypress();
                                }
                            }
                        }
                    }
                }
            }
            } // close the if (!exitBattle) block
        }
    }
    
    return battleWon;
}

/**
 * Move both Yohane and Lailaps in the final battle.
 * Precondition: state is a valid pointer to an initialized GameState struct, dx and dy are valid direction values (-1, 0, or 1).
 * @param state Pointer to the current GameState.
 * @param dx X direction to move (-1, 0, or 1).
 * @param dy Y direction to move (-1, 0, or 1).
 * @return 1 if a move was made, 0 otherwise.
 */
int moveCharactersInFinalBattle(GameState *state, int dx, int dy) {
    // check if characters are already overlapping - this is a bug state that needs fixing
    if (state->yohane.pos.x == state->lailaps.pos.x && state->yohane.pos.y == state->lailaps.pos.y) {
        // characters are overlapping - separate them by moving Lailaps to adjacent position
        printf("%sCharacters are overlapping! Attempting to separate them...%s\n", COLOR_YELLOW, COLOR_RESET);
        
        // find an adjacent empty position for Lailaps
        // i dont get how to implement the correct one for this, so this will be the fix for now...
        int directions[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}}; // up, right, down, left
        int separated = 0;
        
        for (int i = 0; i < 4 && !separated; i++) {
            int newX = state->lailaps.pos.x + directions[i][0];
            int newY = state->lailaps.pos.y + directions[i][1];
            
            // check if position is valid and empty
            if (newX >= 0 && newX < DUNGEON_WIDTH && newY >= 0 && newY < DUNGEON_HEIGHT) {
                char tile = state->currentDungeon.layout[newY][newX];
                if (tile == TILE_EMPTY || tile == TILE_SWITCH || tile == TILE_EXIT || tile == TILE_GOLD_BAT || tile == TILE_GOLD_SIREN) {
                    // move Lailaps to this position
                    state->lailaps.pos.x = newX;
                    state->lailaps.pos.y = newY;
                    state->lailapsUnderlyingTile = tile;
                    separated = 1;
                    printf("%sLailaps moved to separate position.%s\n", COLOR_GREEN, COLOR_RESET);
                    waitForKeypress();
                }
            }
        }
        
        if (!separated) { // this should not happen
            printf("%sCouldn't find empty position to separate characters! Game may be unstable.%s\n", COLOR_RED, COLOR_RESET);
            waitForKeypress();
        }
    }
    
    // calculate the new positions for both characters based on movement direction
    // these represent where each character would move if the movement is valid
    int yohaneNewX = state->yohane.pos.x + dx;  // yohane's potential new x coordinate
    int yohaneNewY = state->yohane.pos.y + dy;  // yohane's potential new y coordinate  
    int lailapsNewX = state->lailaps.pos.x + dx; // lailaps' potential new x coordinate
    int lailapsNewY = state->lailaps.pos.y + dy; // lailaps' potential new y coordinate
    int moveMade = 0;                            // flag indicating if any movement occurred
                                                // will be returned to caller to indicate success
    
    // movement validation flags - these determine independently whether each
    // character can move to their intended destination based on game rules
    int yohaneCanMove = 0;   // set to 1 if Yohane's movement is valid and allowed
    int lailapsCanMove = 0;  // set to 1 if Lailaps' movement is valid and allowed
    
    // check Yohane's movement validity
    if (yohaneNewX >= 0 && yohaneNewX < DUNGEON_WIDTH && yohaneNewY >= 0 && yohaneNewY < DUNGEON_HEIGHT) {
        char tile = state->currentDungeon.layout[yohaneNewY][yohaneNewX];
        // yohane can move to passable tiles
        if (tile == TILE_EMPTY || tile == TILE_SWITCH || tile == TILE_EXIT || tile == TILE_GOLD_BAT || tile == TILE_GOLD_SIREN) {
            yohaneCanMove = 1;
        }
        // Yohane cannot move into Siren or bats - attacks are handled separately
    }
    
    // check Lailaps' movement validity  
    if (lailapsNewX >= 0 && lailapsNewX < DUNGEON_WIDTH && lailapsNewY >= 0 && lailapsNewY < DUNGEON_HEIGHT) {
        char tile = state->currentDungeon.layout[lailapsNewY][lailapsNewX];
        // lailaps can move to passable tiles, but CANNOT attack
        if (tile == TILE_EMPTY || tile == TILE_SWITCH || tile == TILE_EXIT || tile == TILE_GOLD_BAT || tile == TILE_GOLD_SIREN) {
            lailapsCanMove = 1;
        }
        // Lailaps cannot move into Siren or bats (cannot attack)
    }
    
    // check for collision at destination (both trying to move to same spot)
    if (yohaneCanMove && lailapsCanMove && 
        yohaneNewX == lailapsNewX && yohaneNewY == lailapsNewY) {
        // both trying to move to same destination - neither can move
        yohaneCanMove = 0;
        lailapsCanMove = 0;
        printf("%sCharacters cannot move to the same location!%s\n", COLOR_YELLOW, COLOR_RESET);
        waitForKeypress();
    }
    
    
    
        // process movement if either character can move
        if (yohaneCanMove || lailapsCanMove) {
            // check if either character is moving to the exit (after Siren is defeated)
            if (state->finalBattleWon) {
                if ((yohaneCanMove && state->currentDungeon.layout[yohaneNewY][yohaneNewX] == TILE_EXIT) ||
                    (lailapsCanMove && state->currentDungeon.layout[lailapsNewY][lailapsNewX] == TILE_EXIT)) {
                    printf("%s%sFinal Battle Complete! You have won!%s\n", STYLE_BOLD, COLOR_BRIGHT_GREEN, COLOR_RESET);
                    waitForKeypress();
                    moveMade = 2; // Special return code indicating victory and exit
                } else {
                    // handle normal movement and gold collection when exit not reached
                    
                    // handle gold collection - allow both characters to collect simultaneously
                    if (yohaneCanMove) {
                        char yohaneTargetTile = state->currentDungeon.layout[yohaneNewY][yohaneNewX];
                        if (yohaneTargetTile == TILE_GOLD_BAT) {
                            state->yohane.gold += 50; // gold from defeated bats
                            printf("%sYohane collected 50 gold!%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
                            // remove gold from layout
                            state->currentDungeon.layout[yohaneNewY][yohaneNewX] = TILE_EMPTY;
                            waitForKeypress();
                        } else if (yohaneTargetTile == TILE_GOLD_SIREN) {
                            state->yohane.gold += 750; // special gold from defeated Siren
                            printf("%sYou picked up the Siren's treasure! +750 Gold!%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
                            // remove gold from layout
                            state->currentDungeon.layout[yohaneNewY][yohaneNewX] = TILE_EMPTY;
                            waitForKeypress();
                        }
                        
                        // update Yohane's position
                        state->yohane.pos.x = yohaneNewX;
                        state->yohane.pos.y = yohaneNewY;
                        
                        // store what tile Yohane is standing on
                        if (yohaneTargetTile == TILE_GOLD_BAT || yohaneTargetTile == TILE_GOLD_SIREN) {
                            state->underlyingTile = TILE_EMPTY;
                        } else {
                            state->underlyingTile = yohaneTargetTile;
                        }
                    }
                    
                    if (lailapsCanMove) {
                        char lailapsTargetTile = state->currentDungeon.layout[lailapsNewY][lailapsNewX];
                        if (lailapsTargetTile == TILE_GOLD_BAT) {
                            state->yohane.gold += 50; // gold from defeated bats goes to Yohane
                            printf("%sLailaps collected 50 gold for Yohane!%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
                            // remove gold from layout
                            state->currentDungeon.layout[lailapsNewY][lailapsNewX] = TILE_EMPTY;
                            waitForKeypress();
                        } else if (lailapsTargetTile == TILE_GOLD_SIREN) {
                            state->yohane.gold += 750; // special gold from defeated Siren goes to Yohane
                            printf("%sLailaps picked up the Siren's treasure for Yohane! +750 Gold!%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
                            // remove gold from layout
                            state->currentDungeon.layout[lailapsNewY][lailapsNewX] = TILE_EMPTY;
                            waitForKeypress();
                        }
                        
                        // update Lailaps' position
                        state->lailaps.pos.x = lailapsNewX;
                        state->lailaps.pos.y = lailapsNewY;
                        
                        // store what tile Lailaps is standing on
                        if (lailapsTargetTile == TILE_GOLD_BAT || lailapsTargetTile == TILE_GOLD_SIREN) {
                            state->lailapsUnderlyingTile = TILE_EMPTY;
                        } else {
                            state->lailapsUnderlyingTile = lailapsTargetTile;
                        }
                    }
                    
                    // a move was made
                    moveMade = 1;
                }
            } else {
                // handle normal movement when final battle not yet won
                
                // handle gold collection - allow both characters to collect simultaneously
                if (yohaneCanMove) {
                    char yohaneTargetTile = state->currentDungeon.layout[yohaneNewY][yohaneNewX];
                    if (yohaneTargetTile == TILE_GOLD_BAT) {
                        state->yohane.gold += 50; // gold from defeated bats
                        printf("%sYohane collected 50 gold!%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
                        // remove gold from layout
                        state->currentDungeon.layout[yohaneNewY][yohaneNewX] = TILE_EMPTY;
                        waitForKeypress();
                    } else if (yohaneTargetTile == TILE_GOLD_SIREN) {
                        state->yohane.gold += 750; // special gold from defeated Siren
                        printf("%sYou picked up the Siren's treasure! +750 Gold!%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
                        // remove gold from layout
                        state->currentDungeon.layout[yohaneNewY][yohaneNewX] = TILE_EMPTY;
                        waitForKeypress();
                    }
                    
                    // update Yohane's position
                    state->yohane.pos.x = yohaneNewX;
                    state->yohane.pos.y = yohaneNewY;
                    
                    // store what tile Yohane is standing on
                    if (yohaneTargetTile == TILE_GOLD_BAT || yohaneTargetTile == TILE_GOLD_SIREN) {
                        state->underlyingTile = TILE_EMPTY;
                    } else {
                        state->underlyingTile = yohaneTargetTile;
                    }
                }
                
                if (lailapsCanMove) {
                    char lailapsTargetTile = state->currentDungeon.layout[lailapsNewY][lailapsNewX];
                    if (lailapsTargetTile == TILE_GOLD_BAT) {
                        state->yohane.gold += 50; // gold from defeated bats goes to Yohane
                        printf("%sLailaps collected 50 gold for Yohane!%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
                        // remove gold from layout
                        state->currentDungeon.layout[lailapsNewY][lailapsNewX] = TILE_EMPTY;
                        waitForKeypress();
                    } else if (lailapsTargetTile == TILE_GOLD_SIREN) {
                        state->yohane.gold += 750; // special gold from defeated Siren goes to Yohane
                        printf("%sLailaps picked up the Siren's treasure for Yohane! +750 Gold!%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
                        // remove gold from layout
                        state->currentDungeon.layout[lailapsNewY][lailapsNewX] = TILE_EMPTY;
                        waitForKeypress();
                    }
                    
                    // update Lailaps' position
                    state->lailaps.pos.x = lailapsNewX;
                    state->lailaps.pos.y = lailapsNewY;
                    
                    // store what tile Lailaps is standing on
                    if (lailapsTargetTile == TILE_GOLD_BAT || lailapsTargetTile == TILE_GOLD_SIREN) {
                        state->lailapsUnderlyingTile = TILE_EMPTY;
                    } else {
                        state->lailapsUnderlyingTile = lailapsTargetTile;
                    }
                }
                
                // a move was made
                moveMade = 1;
            }
        }
    
    return moveMade;
}

/**
 * Check if a position is valid for movement.
 * Precondition: state pointer must be valid and currentDungeon must be loaded
 * @param state Pointer to the current GameState
 * @param x X coordinate to check
 * @param y Y coordinate to check
 * @return 1 if the position is valid, 0 otherwise
 */
int isPositionValidForMovement(GameState *state, int x, int y) {
    int isValid = 0;
    
    // check if position is within bounds
    if (x >= 0 && x < DUNGEON_WIDTH && y >= 0 && y < DUNGEON_HEIGHT) {
        // check what's at the position
        char tile = state->currentDungeon.layout[y][x];
        
        // valid tiles to move onto (but NOT the Siren)
        if (tile == TILE_EMPTY || tile == TILE_SWITCH || tile == TILE_EXIT) {
            isValid = 1;
        }
    }
    
    return isValid;
}

/**
 * Check if a position is adjacent to Yohane or Lailaps.
 * Precondition: state is a valid pointer to an initialized GameState struct, x and y are valid coordinates.
 * @param state Pointer to the current GameState.
 * @param x X coordinate to check.
 * @param y Y coordinate to check.
 * @return 1 if adjacent to either character, 0 otherwise.
 */
int isPositionAdjacentToYohaneOrLailaps(GameState *state, int x, int y) {
    return arePositionsAdjacent(state->yohane.pos.x, state->yohane.pos.y, x, y) ||
           arePositionsAdjacent(state->lailaps.pos.x, state->lailaps.pos.y, x, y);
}

/**
 * Check if two positions are adjacent (including diagonally).
 * Precondition: x1, y1, x2, y2 are valid coordinate values.
 * @param x1 First position X.
 * @param y1 First position Y.
 * @param x2 Second position X.
 * @param y2 Second position Y.
 * @return 1 if positions are adjacent, 0 otherwise.
 */
int arePositionsAdjacent(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) <= 1 && abs(y1 - y2) <= 1 && !(x1 == x2 && y1 == y2);
}

/**
 * Spawn a new bat in the final battle.
 * Precondition: state is a valid pointer to an initialized GameState struct.
 * @param state Pointer to the current GameState.
 */

 /**
 * Spawn a new bat in the final battle.
 * Precondition: state is a valid pointer to an initialized GameState struct.
 * @param state Pointer to the current GameState.
 */
void spawnBatInFinalBattle(GameState *state) {
    int i;
    int canSpawn = 0;
    int x = 0, y = 0;
    int attempts = 0;
    int batType; // Determine bat type based on number of switches hit
    int positionOccupied; // check if occup or no
    
    // check if we can spawn a bat
    if (state->currentDungeon.enemyCount < MAX_BATS_PER_FLOOR) {
        // find a random empty position that doesn't have characters
        do {
            x = getRandomNumber(1, DUNGEON_WIDTH - 2);
            y = getRandomNumber(1, DUNGEON_HEIGHT - 2);
            attempts++;
            
            // check if position is truly empty (not occupied by characters)
            positionOccupied = 0;
            
            // check if Yohane or Lailaps are at this position
            if ((x == state->yohane.pos.x && y == state->yohane.pos.y) ||
                (x == state->lailaps.pos.x && y == state->lailaps.pos.y)) {
                positionOccupied = 1;
            }
            
            // check if there's already a bat at this position
            for (i = 0; i < state->currentDungeon.enemyCount && !positionOccupied; i++) {
                if (state->currentDungeon.enemies[i].isActive &&
                    state->currentDungeon.enemies[i].pos.x == x &&
                    state->currentDungeon.enemies[i].pos.y == y) {
                    positionOccupied = 1;
                }
            }
            
            // check if the underlying tile is valid for spawning
            char tile = state->currentDungeon.layout[y][x];
            if (tile != TILE_EMPTY && tile != TILE_SWITCH) {
                positionOccupied = 1;
            }
            
            if (!positionOccupied) {
                canSpawn = 1;
            }
            
        } while (attempts < MAX_SPAWN_ATTEMPTS && !canSpawn);
    }
    
    // only spawn bat if conditions are met
    if (canSpawn) {
        
        
        // Ensure finalBattleSwitchesHit is within valid range
        if (state->finalBattleSwitchesHit < 0) {
            state->finalBattleSwitchesHit = 0;
        }
        
        // Correct type assignment based on switches hit
        switch (state->finalBattleSwitchesHit) {
            case 0:
                batType = BAT_TYPE_DUNGEON_1;  // Phase 1: No switches pressed
                break;
            case 1:
                batType = BAT_TYPE_DUNGEON_2;  // Phase 2: 1 switch pair activated
                break;
            case 2:
            case 3:
            default:
                batType = BAT_TYPE_DUNGEON_3;  // Phase 3: 2+ switch pairs activated
                break;
        }
        
        // initialize the new bat
        Enemy *bat = &state->currentDungeon.enemies[state->currentDungeon.enemyCount];
        bat->pos.x = x;
        bat->pos.y = y;
        bat->isActive = 1;
        bat->type = batType;
        
        bat->turnCounter = 0; // start fresh - bat will move after moveInterval turns
        
        bat->underlyingTile = state->currentDungeon.layout[y][x];  // remember what tile the bat spawned on
        
        // Set bat properties correctly based on type (# of switch activated)
        switch (batType) {
            case BAT_TYPE_DUNGEON_1:
                bat->moveInterval = 2;  // Move every 2 turns
                bat->damage = 1;        // 0.5 HP damage (will be halved in combat)
                break;
            case BAT_TYPE_DUNGEON_2:
                bat->moveInterval = 1;  // Move every turn
                bat->damage = 2;        // 1.0 HP damage (will be halved in combat)
                break;
            case BAT_TYPE_DUNGEON_3:
                bat->moveInterval = 1;  // Move every turn
                bat->damage = 3;        // 1.5 HP damage (will be halved in combat)
                break;
            default:
                // fallback to type 1 properties for safety
                bat->type = BAT_TYPE_DUNGEON_1;
                bat->moveInterval = 2;
                bat->damage = 1;
                break;
        }
        
        // additional validation to ensure consistent state
        if (bat->moveInterval <= 0) {
            bat->moveInterval = 2;  // safe default
        }
        
        // increment enemy count
        state->currentDungeon.enemyCount++;
        
        printf("%sA bat has appeared!%s\n", COLOR_BRIGHT_RED, COLOR_RESET);
        waitForKeypress();
    }
}

/**
 * Move a bat in the final battle.
 * Precondition: state is a valid pointer to an initialized GameState struct, batIndex is a valid index.
 * @param state Pointer to the current GameState.
 * @param batIndex Index of the bat to move.
 * @param turnBatAttackedYohane Pointer to flag tracking if any bat attacked Yohane this turn.
 * @param turnBatAttackedLailaps Pointer to flag tracking if any bat attacked Lailaps this turn.
 */
void moveBatInFinalBattle(GameState *state, int batIndex, int *turnBatAttackedYohane, int *turnBatAttackedLailaps) {
    // Variable declarations
    int i, j;
    Enemy *bat;
    int directions[8][2];
    int dirCount;
    int validDirs[8];
    int validCount;
    int chosenDir;
    int adjRow, adjCol;
    int attackDirectionYohane;
    int attackDirectionLailaps;
    int newX, newY;
    int positionOccupied;
    char tile;
    float damage;
    
    // Initialize variables
    bat = &state->currentDungeon.enemies[batIndex];
    dirCount = 0;
    validCount = 0;
    attackDirectionYohane = -1;
    attackDirectionLailaps = -1;
    
    // increment movement counter
    bat->turnCounter++;
    
    // check if it's time for this bat to move based on moveInterval
    if (bat->turnCounter >= bat->moveInterval) {
        bat->turnCounter = 0; // reset counter
        
        // determine possible directions to move based on switches activated
        dirCount = 0;
        if (state->finalBattleSwitchesHit == 0 || state->finalBattleSwitchesHit == 1) {
            // up, down, left, right only (cardinal directions)
            directions[dirCount][0] = -1; directions[dirCount][1] = 0; dirCount++; // up
            directions[dirCount][0] = 1; directions[dirCount][1] = 0; dirCount++;  // down
            directions[dirCount][0] = 0; directions[dirCount][1] = -1; dirCount++; // left
            directions[dirCount][0] = 0; directions[dirCount][1] = 1; dirCount++;  // right
        } else {
            // All 8 directions (cardinal + diagonal)
            directions[dirCount][0] = -1; directions[dirCount][1] = 0; dirCount++; // up
            directions[dirCount][0] = 1; directions[dirCount][1] = 0; dirCount++;  // down
            directions[dirCount][0] = 0; directions[dirCount][1] = -1; dirCount++; // left
            directions[dirCount][0] = 0; directions[dirCount][1] = 1; dirCount++;  // right
            directions[dirCount][0] = -1; directions[dirCount][1] = -1; dirCount++; // up-left
            directions[dirCount][0] = -1; directions[dirCount][1] = 1; dirCount++; // up-right
            directions[dirCount][0] = 1; directions[dirCount][1] = -1; dirCount++; // down-left
            directions[dirCount][0] = 1; directions[dirCount][1] = 1; dirCount++;  // down-right
        }
        
        // check if player is adjacent (attack has priority over movement)
        attackDirectionYohane = -1;
        attackDirectionLailaps = -1;
        for (i = 0; i < dirCount; ++i) {
            adjRow = bat->pos.y + directions[i][0];
            adjCol = bat->pos.x + directions[i][1];
            if (adjRow == state->yohane.pos.y && adjCol == state->yohane.pos.x && *turnBatAttackedYohane == 0) {
                attackDirectionYohane = i;
                *turnBatAttackedYohane = 1;
            }
            
            if (adjRow == state->lailaps.pos.y && adjCol == state->lailaps.pos.x && *turnBatAttackedLailaps == 0) {
                attackDirectionLailaps = i;
                *turnBatAttackedLailaps = 1;
            }
        }
        
        if (attackDirectionLailaps != -1) {
            // attack Lailaps
            damage = bat->damage * 0.5f; // damage is halved in combat
            
            // apply bat tamer upgrade
            if (state->yohane.hasBatTamer) {
                damage = 0.5f;
            }
            
            state->lailaps.hp -= damage;
            setDamageCause(state, "Bat");
            printf("%sLailaps was attacked by a bat! %s-%.1f HP%s\n", COLOR_RED, COLOR_BRIGHT_RED, damage, COLOR_RESET);
            
            // check if Lailaps is defeated
            if (state->lailaps.hp <= 0) {
                printf("%sLailaps is defeated! Game Over!%s\n", COLOR_BRIGHT_RED, COLOR_RESET);
            }
            // bat stays in place when attacking
        } else if (attackDirectionYohane != -1) {
            // attack Yohane
            damage = bat->damage * 0.5f; // damage is halved in combat
            
            // apply bat tamer upgrade
            if (state->yohane.hasBatTamer) {
                damage = 0.5f;
            }
            
            state->yohane.hp -= damage;
            state->damageTakenThisRun = 1;
            setDamageCause(state, "Bat");
            printf("%sYohane was attacked by a bat! %s-%.1f HP%s\n", COLOR_RED, COLOR_BRIGHT_RED, damage, COLOR_RESET);
            
            // check for fatal damage and handle choco-mint saves
            checkFatalDamageAndChocoMintSave(state);
            // bat stays in place when attacking
        } else {
            // move randomly into valid location
            
            // find valid directions
            validCount = 0;
            for (i = 0; i < dirCount; ++i) {
                newY = bat->pos.y + directions[i][0];
                newX = bat->pos.x + directions[i][1];
                
                // check bounds
                if (newX >= 1 && newX < DUNGEON_WIDTH - 1 && newY >= 1 && newY < DUNGEON_HEIGHT - 1) {
                    tile = state->currentDungeon.layout[newY][newX];
                    
                    // check if position is valid and not occupied
                    positionOccupied = 0;
                    
                    // check if characters are at this position
                    if ((newX == state->yohane.pos.x && newY == state->yohane.pos.y) ||
                        (newX == state->lailaps.pos.x && newY == state->lailaps.pos.y)) {
                        positionOccupied = 1;
                    }
                    
                    // check if another bat is already there
                    for (j = 0; j < state->currentDungeon.enemyCount && !positionOccupied; j++) {
                        if (j != batIndex && state->currentDungeon.enemies[j].isActive &&
                            state->currentDungeon.enemies[j].pos.x == newX &&
                            state->currentDungeon.enemies[j].pos.y == newY) {
                            positionOccupied = 1;
                        }
                    }
                    
                    // bats can move to empty tiles, switches, and water tiles
                    if (!positionOccupied && (tile == TILE_EMPTY || tile == TILE_SWITCH || tile == TILE_WATER)) {
                        validDirs[validCount++] = i;
                    }
                }
            }
            
            // if found valid directions, move into one randomly
            if (validCount > 0) {
                chosenDir = validDirs[getRandomNumber(0, validCount - 1)];
                
                // restore underlying tile at old position
                state->currentDungeon.layout[bat->pos.y][bat->pos.x] = bat->underlyingTile;
                
                // update bat position
                bat->pos.y += directions[chosenDir][0];
                bat->pos.x += directions[chosenDir][1];
                
                // remember what tile the bat is now on
                bat->underlyingTile = state->currentDungeon.layout[bat->pos.y][bat->pos.x];
            }
            // else don't move (no valid directions)
        }
    }
}

/**
 * Use the currently equipped item.
 * Precondition: state is a valid pointer to an initialized GameState struct.
 * @param state Pointer to the current GameState.
 */
void useItem(GameState *state) {
    switch (state->yohane.currentItemOnHand) {
        case ITEM_TEARS:
            if (state->tearsCount > 0) {
                // tears of a fallen angel: Heal 0.5 HP
                if (state->yohane.hp < state->yohane.maxHp) {
                    state->yohane.hp += 0.5f;
                    if (state->yohane.hp > state->yohane.maxHp) {
                        state->yohane.hp = state->yohane.maxHp;
                    }
                    state->tearsCount--;
                    state->yohane.currentItemOnHand = ITEM_NONE;
                    printf("%sYou used Tears of a fallen angel and recovered 0.5 HP!%s\n", COLOR_BRIGHT_GREEN, COLOR_RESET);
                    waitForKeypress();
                } else {
                    printf("%sYour HP is already full!%s\n", COLOR_YELLOW, COLOR_RESET);
                    waitForKeypress();
                }
            } else {
                printf("%sYou don't have any Tears of a fallen angel!%s\n", COLOR_RED, COLOR_RESET);
                waitForKeypress();
            }
            break;
            
        case ITEM_NOPPO:
            if (state->noppoBreadCount > 0) {
                // noppo bread: Heal 0.5 HP
                if (state->yohane.hp < state->yohane.maxHp) {
                    state->yohane.hp += 0.5f;
                    if (state->yohane.hp > state->yohane.maxHp) {
                        state->yohane.hp = state->yohane.maxHp;
                    }
                    state->noppoBreadCount--;
                    state->yohane.currentItemOnHand = ITEM_NONE;
                    printf("%sYou used Noppo bread and recovered 0.5 HP!%s\n", COLOR_BRIGHT_GREEN, COLOR_RESET);
                    waitForKeypress();
                } else {
                    printf("%sYour HP is already full!%s\n", COLOR_YELLOW, COLOR_RESET);
                    waitForKeypress();
                }
            } else {
                printf("%sYou don't have any Noppo bread!%s\n", COLOR_RED, COLOR_RESET);
                waitForKeypress();
            }
            break;
            
        case ITEM_CHOCO_MINT:
            printf("%sChoco-mint ice cream will be used automatically if you take fatal damage.%s\n", COLOR_BRIGHT_MAGENTA, COLOR_RESET);
            waitForKeypress();
            break;
            
        default:
            printf("%sNo item equipped!%s\n", COLOR_YELLOW, COLOR_RESET);
            waitForKeypress();
            break;
    }
}


/**
 * Check if Yohane has taken fatal damage and handle choco-mint ice cream saves.
 * This function centralizes all fatal damage handling to ensure choco-mint saves work for all damage sources.
 * Precondition: state is a valid pointer to an initialized GameState struct, Yohane's HP and damage cause should be set.
 * @param state Pointer to the current GameState.
 * @return 1 if Yohane died (game over), 0 if Yohane survived (either didn't die or was saved by choco-mint).
 */
int checkFatalDamageAndChocoMintSave(GameState *state) {
    int yohaneDied = 0;  // assume Yohane survived
    
    // only check if Yohane's HP is at or below 0
    if (state->yohane.hp <= 0) {
        // check if choco-mint ice cream can save Yohane
        if (state->yohane.currentItemOnHand == ITEM_CHOCO_MINT && state->chocoMintCount > 0) {
            // choco-mint ice cream saves Yohane from fatal damage
            state->yohane.hp = state->yohane.maxHp;  // heal to full health
            state->chocoMintCount--;                 // consume one choco-mint
            state->yohane.currentItemOnHand = ITEM_NONE;  // remove from hand
            state->chocoMintUsedToSave = 1;         // track for achievement
            
            printf("%s%sThe Choco-mint ice cream saved Yohane from certain doom!%s\n", 
                   STYLE_BOLD, COLOR_BRIGHT_MAGENTA, COLOR_RESET);
            waitForKeypress();
            yohaneDied = 0;  // Yohane survived
        } else {
            // no choco-mint save available - Yohane dies
            showDefeatScreen(state, state->lastDamageCause);
            yohaneDied = 1;  // Yohane died (game over)
        }
    }
    
    return yohaneDied;
}




/**
 * Run a dungeon with multiple floors.
 * Precondition: state is a valid pointer to an initialized GameState struct, dungeonIndex is valid (0-2).
 * @param state Pointer to the current GameState.
 * @param dungeonIndex Index of the dungeon to run (0-2).
 * @return 1 if entire dungeon was completed, 0 if player quit or was defeated.
 */
int runDungeon(GameState *state, int dungeonIndex) {
    char filename[100];
    int success = 0;
    int validIdol = 1;
    int idolIndex = state->rescuedInThisRun[dungeonIndex];
    
    // set up dungeon properties
    state->currentDungeon.difficulty = dungeonIndex + 1;
    state->currentDungeon.idolIndex = idolIndex;
    
    // set floor count based on dungeon difficulty according to specs
    switch (dungeonIndex) {
        case 0:  // 1st dungeon = 1 floor
            state->currentDungeon.maxFloors = 1;
            break;
        case 1:  // 2nd dungeon = 2-3 floors (randomly choose)
            // only regenerate if starting fresh, preserve if resuming
            if (state->currentDungeon.difficulty != (dungeonIndex + 1) || state->currentDungeon.floor <= 1) {
                state->currentDungeon.maxFloors = getRandomNumber(2, 3);
            }
            break;
        case 2:  // 3rd dungeon = 3-4 floors (randomly choose)
            // only regenerate if starting fresh, preserve if resuming
            if (state->currentDungeon.difficulty != (dungeonIndex + 1) || state->currentDungeon.floor <= 1) {
                state->currentDungeon.maxFloors = getRandomNumber(3, 4);
            }
            break;
        default:
            state->currentDungeon.maxFloors = 1;
    }
    
    // check if this is a continuation of a partially completed dungeon
    int isResuming = 0;
    if (state->currentDungeon.difficulty == (dungeonIndex + 1) && 
        state->currentDungeon.floor > 1 && 
        state->currentDungeon.floor <= state->currentDungeon.maxFloors) {
        // player is resuming from a saved floor
        isResuming = 1;
        printf("%s%sResuming dungeon from floor %d/%d...%s\n", 
               STYLE_BOLD, COLOR_BRIGHT_CYAN, 
               state->currentDungeon.floor, state->currentDungeon.maxFloors, 
               COLOR_RESET);
        waitForKeypress();
    } else {
        // start from floor 1 (new dungeon attempt)
        state->currentDungeon.floor = 1;
    }
    
    // reset damage taken flag ONLY if starting fresh (not resuming)
    if (!isResuming) {
        state->damageTakenThisRun = 0;
    }
    
    // run through all floors of the dungeon
    int dungeonComplete = 0;
    int dungeonFailed = 0;
    while (state->currentDungeon.floor <= state->currentDungeon.maxFloors && !dungeonComplete && !dungeonFailed) {
        // load appropriate dungeon file based on idol and current floor
        switch (idolIndex) {
            case IDOL_CHIKA:
                snprintf(filename, sizeof(filename), "assets/dungeon-env/dungeons_yasudaya_ryokan_floor%d.txt", state->currentDungeon.floor);
                break;
            case IDOL_RIKO:
                snprintf(filename, sizeof(filename), "assets/dungeon-env/dungeons_numazu_deep_sea_aquarium_floor%d.txt", state->currentDungeon.floor);
                break;
            case IDOL_YOU:
                snprintf(filename, sizeof(filename), "assets/dungeon-env/dungeons_izu_mito_sea_paradise_floor%d.txt", state->currentDungeon.floor);
                break;
            case IDOL_HANAMARU:
                snprintf(filename, sizeof(filename), "assets/dungeon-env/dungeons_shougetsu_confectionary_floor%d.txt", state->currentDungeon.floor);
                break;
            case IDOL_RUBY:
                snprintf(filename, sizeof(filename), "assets/dungeon-env/dungeons_nagahama_castle_ruins_floor%d.txt", state->currentDungeon.floor);
                break;
            case IDOL_DIA:
                snprintf(filename, sizeof(filename), "assets/dungeon-env/dungeons_numazugoyotei_floor%d.txt", state->currentDungeon.floor);
                break;
            case IDOL_KANAN:
                snprintf(filename, sizeof(filename), "assets/dungeon-env/dungeons_uchiura_bay_pier_floor%d.txt", state->currentDungeon.floor);
                break;
            case IDOL_MARI:
                snprintf(filename, sizeof(filename), "assets/dungeon-env/dungeons_awashima_marine_park_floor%d.txt", state->currentDungeon.floor);
                break;
            default:
                printf("Invalid idol index!\n");
                waitForKeypress();
                validIdol = 0;
        }
        
        if (!validIdol) {
            dungeonFailed = 1;
        } else if (loadDungeonFromFile(state, filename)) {
            // reset turn counter and heat tile counter for each floor
            state->turnCounter = 0;
            state->heatTileCounter = 0;  // reset heat tile counter for each floor
            
            // reset move counter for each dungeon floor to prevent save/load exploits - COMMENTED OUT !! For Debugging only to check if it is actually moving.
            // state->moveCounter = 0;
            
            // show loading tip before entering floor
            showLoadingTip();
            
            // run the actual dungeon gameplay loop for this floor
            int floorResult = runDungeonGameplay(state);
            
            if (floorResult) {
                // floor completed successfully
                if (state->currentDungeon.floor < state->currentDungeon.maxFloors) {
                    // more floors to go
                    printf("%s%sFloor %d completed!%s\n", STYLE_BOLD, COLOR_BRIGHT_GREEN, state->currentDungeon.floor, COLOR_RESET);
                    printf("%sProceeding to floor %d...%s\n", COLOR_BRIGHT_CYAN, state->currentDungeon.floor + 1, COLOR_RESET);
                    waitForKeypress();
                    state->currentDungeon.floor++;
                } else {
                    // last floor completed - entire dungeon finished
                    printf("%s%sDungeon completed!%s\n", STYLE_BOLD, COLOR_BRIGHT_GREEN, COLOR_RESET);
                    printf("%sYou rescued %s%s%s!%s\n", COLOR_WHITE, COLOR_BRIGHT_CYAN, state->aqours[idolIndex].name, COLOR_WHITE, COLOR_RESET);
                    
                    
                    // check for "Aozora Jumping Heart!" achievement (clear dungeon without damage)
                    if (!state->achievements[24].isEarned && !state->damageTakenThisRun) {
                        state->achievements[24].isEarned = 1;
                        char *currentDate = getCurrentDateTime();
                        strcpy(state->achievements[24].dateEarned, currentDate);
                        printf("\n%s%sACHIEVEMENT UNLOCKED!%s\n", STYLE_BOLD, COLOR_BRIGHT_YELLOW, COLOR_RESET);
                        printf("%s%s%s\n", COLOR_BRIGHT_CYAN, state->achievements[24].name, COLOR_RESET);
                        waitForKeypress();
                    }
                    
                    waitForKeypress();
                    success = 1;
                    dungeonComplete = 1;  // signal to exit the while loop
                    
                    // reset dungeon state since it's completed
                    state->currentDungeon.floor = 1;
                    state->currentDungeon.difficulty = 0;
                }
            } else {
                // player quit - dungeon state remains for possible resumption
                dungeonFailed = 1;
            }
        } else {
            printf("%sError: Could not load dungeon floor %d.%s\n", COLOR_RED, state->currentDungeon.floor, COLOR_RESET);
            waitForKeypress();
            dungeonFailed = 1;
        }
    }
    
    return success;
}

/**
 * Run the actual dungeon gameplay loop with WASD movement and interactions.
 * Precondition: state is a valid pointer to an initialized GameState struct.
 * @param state Pointer to the current GameState.
 * @return 1 if dungeon completed successfully, 0 if player died/quit.
 */
int runDungeonGameplay(GameState *state) {
    char input;                // stores the single character input from player
                              // used to determine movement direction and actions
    int dungeonComplete = 0;   // flag indicating whether the current floor is completed
                              // set to 1 when player reaches the exit tile successfully
    int playerDead = 0;        // flag indicating whether the player character has died
                              // set to 1 when Yohane's HP reaches zero or below
    int quitDungeon = 0;       // flag indicating whether player chose to quit the dungeon
                              // set to 1 when player selects quit from the pause menu
    
    while (!dungeonComplete && !playerDead && !quitDungeon) {
        // render the current dungeon state
        renderDungeon(state);
        
        // get player input using non-blocking input
        input = getCharNonBlocking();
        
        // process player input
        int moveResult = 0;
        switch (toupper(input)) {
            case 'W': // move up
                moveResult = tryMovePlayer(state, 0, -1);
                if (moveResult > 0) {
                    processTurn(state, 1);  // player moved successfully
                    if (moveResult == 2) {
                        dungeonComplete = 1;  // player reached exit
                    }
                }
                break;
            case 'A': // move left
                moveResult = tryMovePlayer(state, -1, 0);
                if (moveResult > 0) {
                    processTurn(state, 1);  // player moved successfully
                    if (moveResult == 2) {
                        dungeonComplete = 1;  // player reached exit
                    }
                }
                break;
            case 'S': // move down
                moveResult = tryMovePlayer(state, 0, 1);
                if (moveResult > 0) {
                    processTurn(state, 1);  // player moved successfully
                    if (moveResult == 2) {
                        dungeonComplete = 1;  // player reached exit
                    }
                }
                break;
            case 'D': // move right
                moveResult = tryMovePlayer(state, 1, 0);
                if (moveResult > 0) {
                    processTurn(state, 1);  // player moved successfully
                    if (moveResult == 2) {
                        dungeonComplete = 1;  // player reached exi
                    }
                }
                break;
            case 'X': // stay still
                // staying still will increment heat tile counter in processTurn
                processTurn(state, 0);  // player stayed still (no movement)
                // state->moveCounter++;  // staying still still counts as a move - COMMENTED OUT
                // !! For Debugging only to check if it is actually moving. (check others pls)
                break;
            case '[': // previous item
                cycleItem(state, -1);
                processTurn(state, 1);  // item switching counts as movement per specs
                // state->moveCounter++;  // item switching counts as a move - COMMENTED OUT
                break;
            case ']': // next item
                cycleItem(state, 1);
                processTurn(state, 1);  // item switching counts as movement per specs
                // state->moveCounter++;  // item switching counts as a move - COMMENTED OUT
                break;
            case ' ': // use item (spacebar)
                useItem(state);
                processTurn(state, 1);  // using item counts as movement per specs
                // state->moveCounter++;  // using item counts as a move - COMMENTED OUT
                break;
            case 27: // ESC key - pause menu
            case 'M': // 'M' for menu (alternative to ESC)
                if (showDungeonPauseMenu(state)) {
                    quitDungeon = 1;
                }
                break;
            default:
                // any other input makes character stay still (same as 'X')
                printf("%sInvalid input - staying in place.%s\n", COLOR_YELLOW, COLOR_RESET);
                processTurn(state, 0);  // invalid input = staying still (no movement)
                // state->moveCounter++;  // staying still still counts as a move - COMMENTED OUT
                // don't call waitForKeypress() here since processTurn() may already call it
                break;
        }
        
        // check win/lose conditions
        if (state->yohane.hp <= 0) {
            // use centralized fatal damage check with choco-mint saves
            if (checkFatalDamageAndChocoMintSave(state)) {
                playerDead = 1;  // Yohane died
            }
            // if checkFatalDamageAndChocoMintSave returned 0, Yohane was saved by choco-mint and game continues
        }
        
        // exit completion is now handled in the movement logic above
    }
    
    return dungeonComplete && !playerDead;
}

/**
 * Try to move the player in the specified direction.
 * Precondition: state must be valid, dx/dy must be valid direction values
 * @param state Pointer to the current GameState
 * @param dx Change in x direction (-1, 0, 1)
 * @param dy Change in y direction (-1, 0, 1)
 * @return 1 if move was successful, 0 otherwise
 */
int tryMovePlayer(GameState *state, int dx, int dy) {
    // calculate new position
    int newX = state->yohane.pos.x + dx;
    int newY = state->yohane.pos.y + dy;
    int moveResult = 0;
    int canContinue = 1;
    
    // check bounds
    // just in case i did an oopsie in the dungeon map
    if (newX < 0 || newX >= DUNGEON_WIDTH || newY < 0 || newY >= DUNGEON_HEIGHT) {
        printf("Can't move there - out of bounds!\n");
        waitForKeypress();
        moveResult = 0;
        canContinue = 0;
    }
    
    // check what tile we're trying to move to
    char targetTile;
    if (canContinue) {
        targetTile = state->currentDungeon.layout[newY][newX];
    }
    
    // check if movement is blocked
    if (canContinue) {
        switch (targetTile) {
            case TILE_WALL:
                // wall can be dug - player stays in place and wall becomes empty
                printf("%sYou dig through the wall!%s\n", COLOR_BRIGHT_GREEN, COLOR_RESET);
                state->currentDungeon.layout[newY][newX] = TILE_EMPTY;
                // state->moveCounter++; // cOMMENTED OUT
                waitForKeypress();
                
                // process turn since digging counts as a turn - digging is NOT movement per specs
                processTurn(state, 0);  // digging = no movement (can cause heat damage)
                moveResult = 1;  // move was processed but player didn't move
                canContinue = 0;
                break;
            case TILE_SPIKE:
                // spike can be dug - damage unless shovel upgrade
                if (!state->yohane.hasShovelUpgrade) {
                    printf("%sYohane digs through the spikes and takes damage! %s-0.5 HP%s\n", COLOR_BRIGHT_RED, COLOR_BRIGHT_RED, COLOR_RESET);
                    state->yohane.hp -= 0.5f;
                    state->damageTakenThisRun = 1;  // track damage for achievement
                    setDamageCause(state, "Spike Walls");
                    
                    // check for fatal damage and handle choco-mint saves
                    checkFatalDamageAndChocoMintSave(state);
                    // Note: if Yohane died, the death will be detected in the main game loop
                } else {
                    printf("%sYour shovel upgrade protects you while digging spikes!%s\n", COLOR_BRIGHT_GREEN, COLOR_RESET);
                }
                state->currentDungeon.layout[newY][newX] = TILE_EMPTY;
                // state->moveCounter++; 
                waitForKeypress();
                
                // process turn since digging counts as a turn - digging is NOT movement per specs
                processTurn(state, 0);  // digging = no movement (can cause heat damage)
                moveResult = 1;  // move was processed but player didn't move
                canContinue = 0;
                break;
            case TILE_BORDER:
                printf("%sCan't move there - blocked by border!%s\n", COLOR_RED, COLOR_RESET);
                waitForKeypress();
                moveResult = 0;
                canContinue = 0;
                break;
            case TILE_BAT:
                // this case should not happen since bats are not placed in layout
                // fall through to default
            default:
                // check if there's a bat at the target position
                {
                    int batFound = 0;
                    for (int i = 0; i < state->currentDungeon.enemyCount && !batFound; i++) {
                        if (state->currentDungeon.enemies[i].pos.x == newX && 
                            state->currentDungeon.enemies[i].pos.y == newY && 
                            state->currentDungeon.enemies[i].isActive) {
                            
                            // attack the bat by moving into it - Yohane stays in place
                            printf("%sYou attack the bat!%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
                            
                            // determine gold amount based on dungeon difficulty (per specs)
                            int goldAmount;
                            switch (state->currentDungeon.difficulty) {
                                case 1: goldAmount = 5; break;   // 1st dungeon: 5g
                                case 2: goldAmount = 10; break;  // 2nd dungeon: 10g
                                case 3: goldAmount = 15; break;  // 3rd dungeon: 15g
                                default: goldAmount = 5; break;
                            }
                            
                            state->currentDungeon.enemies[i].isActive = 0;  // kill the bat
                            printf("%sThe bat is defeated and drops %d gold!%s\n", COLOR_BRIGHT_YELLOW, goldAmount, COLOR_RESET);
                            batFound = 1;
                            
                            // place gold on the underlying tile
                            char underlyingTile = state->currentDungeon.enemies[i].underlyingTile;
                            if (underlyingTile == TILE_EMPTY) {
                                state->currentDungeon.layout[newY][newX] = TILE_GOLD_BAT;
                            } else {
                                state->currentDungeon.layout[newY][newX] = underlyingTile;
                            }
                            waitForKeypress();
                            
                            // process turn since attacking counts as a turn - attacking is NOT movement per specs
                            processTurn(state, 0);  // attacking = no movement (can cause heat damage)
                            moveResult = 1;  // move was processed but player didn't move
                            canContinue = 0;
                            break;
                        }
                    }
                    
                    // if no bat found, process normal tile movement
                    if (!batFound) {
                        // continue with normal movement processing - nothing special to do
                    }
                }
                break;
            case TILE_LAILAPS:
                printf("%sCan't move there - Lailaps is in the way!%s\n", COLOR_CYAN, COLOR_RESET);
                waitForKeypress();
                moveResult = 0;
                canContinue = 0;
                break;
        }
    }
    
    // clear old position and restore underlying tile
    if (canContinue) {
        // add bounds checking for old position
        if (state->yohane.pos.y >= 0 && state->yohane.pos.y < DUNGEON_HEIGHT &&
            state->yohane.pos.x >= 0 && state->yohane.pos.x < DUNGEON_WIDTH) {
            state->currentDungeon.layout[state->yohane.pos.y][state->yohane.pos.x] = state->underlyingTile;
        }
    }
    
    // handle special tiles at new position before moving
    if (canContinue) {
        switch (targetTile) {
            case TILE_WATER:
                if (!state->yohane.hasAirShoes) {
                    printf("%sYou can't swim! You need Air Shoes to walk on water.%s\n", COLOR_BRIGHT_BLUE, COLOR_RESET);
                    waitForKeypress();
                    moveResult = 0;
                    canContinue = 0;
                } else {
                    // with air shoes, preserve the water tile as underlying tile
                    state->underlyingTile = TILE_WATER;
                }
                break;
            case TILE_TREASURE:
                // random treasure: either gold (10-100g) or Noppo bread
                if (getRandomNumber(1, 100) <= 70) {  // 70% chance for gold
                    int goldAmount = getRandomNumber(10, 100);
                    state->yohane.gold += goldAmount;
                    printf("%sYou found treasure! +%d Gold!%s\n", COLOR_BRIGHT_YELLOW, goldAmount, COLOR_RESET);
                } else {  // 30% chance for Noppo bread
                    state->noppoBreadCount++;
                    printf("%sYou found treasure! +1 Noppo Bread!%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
                }
                waitForKeypress();
                
                // mark treasure as consumed to prevent duplication exploit
                int dungeonIndex = state->currentDungeon.difficulty - 1;  // 0-based index
                int floorIndex = state->currentDungeon.floor - 1;  // 0-based index
                if (dungeonIndex >= 0 && dungeonIndex < 3 && floorIndex >= 0 && floorIndex < 5) {
                    state->treasureConsumed[dungeonIndex][floorIndex] = 1;
                }
                
                // treasure is consumed permanently - mark as empty in layout so it doesn't respawn
                state->currentDungeon.layout[newY][newX] = TILE_EMPTY;
                state->underlyingTile = TILE_EMPTY;
                break;
            case TILE_EXIT:
                printf("%s%sDungeon floor completed!%s\n", STYLE_BOLD, COLOR_BRIGHT_GREEN, COLOR_RESET);
                waitForKeypress();
                state->underlyingTile = targetTile;
                break;
            case TILE_GOLD_BAT: {  // gold dropped by bats
                // give player gold based on dungeon difficulty (per specs)
                int goldAmount;
                switch (state->currentDungeon.difficulty) {
                    case 1: goldAmount = 5; break;   // 1st dungeon: 5g
                    case 2: goldAmount = 10; break;  // 2nd dungeon: 10g
                    case 3: goldAmount = 15; break;  // 3rd dungeon: 15g
                    default: goldAmount = 5; break;
                }
                state->yohane.gold += goldAmount;
                printf("%sYou picked up %d gold!%s\n", COLOR_BRIGHT_YELLOW, goldAmount, COLOR_RESET);
                waitForKeypress();
                // gold is consumed, so underlying tile becomes empty
                state->underlyingTile = TILE_EMPTY;
                break;
            }
            case TILE_GOLD_SIREN: {  // special gold dropped by Siren
                // Siren drops 750 gold
                state->yohane.gold += 750;
                printf("%sYou picked up the Siren's treasure! +750 Gold!%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
                waitForKeypress();
                // gold is consumed, so underlying tile becomes empty
                state->underlyingTile = TILE_EMPTY;
                break;
            }
            case TILE_SWITCH:
                // switch tiles should remain as switches, not be consumed
                printf("%sYou step on the switch!%s\n", COLOR_BRIGHT_CYAN, COLOR_RESET);
                state->underlyingTile = TILE_SWITCH;
                break;
            case TILE_HEAT:
                // heat tiles - just step on them, damage is handled in processTurn
                state->underlyingTile = TILE_HEAT;
                break;
            default:
                // store the underlying tile at new position
                state->underlyingTile = targetTile;
                break;
        }
    }
    
    // update player position and finalize move
    if (canContinue) {
        // update player position
        state->yohane.pos.x = newX;
        state->yohane.pos.y = newY;
        
        // add bounds checking for new position before placing player
        if (newY >= 0 && newY < DUNGEON_HEIGHT && newX >= 0 && newX < DUNGEON_WIDTH) {
            // place player at new position
            state->currentDungeon.layout[newY][newX] = TILE_PLAYER;
        } else {
            printf("Error: Attempted to place player at invalid position (%d, %d)\n", newX, newY);
            // revert position change
            state->yohane.pos.x = newX - dx;
            state->yohane.pos.y = newY - dy;
            canContinue = 0;
            moveResult = 0;
        }
        // increment move counter - COMMENTED OUT
        // state->moveCounter++;
        
        // store what tile the player is standing on for exit detection
        if (canContinue && targetTile == TILE_EXIT) {
            // set special value to indicate exit was reached
            moveResult = 2;  // 2 means "moved and reached exit"
        } else if (canContinue) {
            moveResult = 1;  // 1 means "moved successfully"
        }
    } else {
        // move failed - place player back at current position with bounds checking
        if (state->yohane.pos.y >= 0 && state->yohane.pos.y < DUNGEON_HEIGHT &&
            state->yohane.pos.x >= 0 && state->yohane.pos.x < DUNGEON_WIDTH) {
            state->currentDungeon.layout[state->yohane.pos.y][state->yohane.pos.x] = TILE_PLAYER;
        }
    }
    
    return moveResult;
}

/**
 * Process a game turn (enemy movement, status effects, etc).
 * Precondition: state must be valid
 * @param state Pointer to the current GameState
 * @param playerMoved 1 if player moved/performed movement action, 0 if stayed still/non-movement action
 */
void processTurn(GameState *state, int playerMoved) {
    // increment turn counter
    state->turnCounter++;
    
    // move enemies
    moveEnemies(state);
    
    // check for heat damage at end of turn if standing on heat tile
    // per specs: Heat tiles damage if player doesn't move while standing on one
    // this includes: digging, attacking enemy, or intentionally not moving
    // but switching items or using items (those count as movement per specs)
    if (state->underlyingTile == TILE_HEAT) {
        if (!state->yohane.hasAirShoes) {
            if (!playerMoved) {
                // per specs: damage occurs immediately when not moving while on heat tile
                printf("%sYohane takes heat damage from staying still on hot ground! %s-1.0 HP%s\n", 
                       COLOR_BRIGHT_RED, COLOR_BRIGHT_RED, COLOR_RESET);
                state->yohane.hp -= 1.0f;
                state->damageTakenThisRun = 1;  // track damage for achievement
                setDamageCause(state, "Heat Tiles");
                
                // check for fatal damage and handle choco-mint saves
                checkFatalDamageAndChocoMintSave(state);
                // Note: if Yohane died, the game will end in the main game loop
                waitForKeypress();
            }
        }
    }
    
    // might add more "duration/turn based" features here if needed
    // 
}

/**
 * Cycle through available items.
 * Precondition: state must be valid, direction must be -1 or 1
 * @param state Pointer to the current GameState
 * @param direction Direction to cycle (-1 for previous, 1 for next)
 */
void cycleItem(GameState *state, int direction) {
    int availableItems[4];
    int itemCount = 0;
    int currentIndex = -1;
    int itemFound = 0;
    int i;
    
    // always have "None" option
    availableItems[itemCount++] = ITEM_NONE;
    
    // add items based on inventory
    if (state->tearsCount > 0) {
        availableItems[itemCount++] = ITEM_TEARS;
    }
    if (state->noppoBreadCount > 0) {
        availableItems[itemCount++] = ITEM_NOPPO;
    }
    if (state->chocoMintCount > 0) {
        availableItems[itemCount++] = ITEM_CHOCO_MINT;
    }
    
    // find current item index
    itemFound = 0;
    for (i = 0; i < itemCount && !itemFound; i++) {
        if (availableItems[i] == state->yohane.currentItemOnHand) {
            currentIndex = i;
            itemFound = 1;
        }
    }
    
    // if current item not found (shouldn't happen), default to first item
    if (currentIndex == -1) {
        currentIndex = 0;
    }
    
    // cycle to next/previous item
    currentIndex += direction;
    
    // wrap around
    if (currentIndex < 0) {
        currentIndex = itemCount - 1;
    } else if (currentIndex >= itemCount) {
        currentIndex = 0;
    }
    
    // set new item
    state->yohane.currentItemOnHand = availableItems[currentIndex];
    
    // show what item was selected
    switch (state->yohane.currentItemOnHand) {
        case ITEM_NONE:
            printf("%sSelected: None%s\n", COLOR_WHITE, COLOR_RESET);
            break;
        case ITEM_TEARS:
            printf("%sSelected: Tears of a fallen angel (x%d)%s\n", COLOR_CYAN, state->tearsCount, COLOR_RESET);
            break;
        case ITEM_NOPPO:
            printf("%sSelected: Noppo bread (x%d)%s\n", COLOR_YELLOW, state->noppoBreadCount, COLOR_RESET);
            break;
        case ITEM_CHOCO_MINT:
            printf("%sSelected: Choco-mint ice cream (x%d)%s\n", COLOR_MAGENTA, state->chocoMintCount, COLOR_RESET);
            break;
    }
    waitForKeypress();
}

// might be needed ?
// /**
//  * Process tile interaction at specified coordinates.
//  * Precondition: state must be valid, x/y must be valid coordinates
//  * @param state Pointer to the current GameState
//  * @param x X coordinate
//  * @param y Y coordinate
//  */
// void processTileInteraction(GameState *state, int x, int y) {
//     // 
//     return to myself;
// }

/**
 * Move enemies in the dungeon.
 * Precondition: state must be valid
 * @param state Pointer to the current GameState
 */
void moveEnemies(GameState *state) {
    int i, dir;
    Enemy *enemy;
    int shouldMove;
    int isPlayerAdjacent;
    int adjacentPositions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    int checkX, checkY;
    
    // move each active enemy based on their movement interval
    for (i = 0; i < state->currentDungeon.enemyCount; i++) {
        enemy = &state->currentDungeon.enemies[i];
        
        if (enemy->isActive) {
            // check if it's time for this enemy to move based on dungeon difficulty
            shouldMove = 0;
            switch (state->currentDungeon.difficulty) {
                case 1:  // 1st dungeon: move every 2 turns
                    if (state->turnCounter % 2 == 0) shouldMove = 1;
                    break;
                case 2:  // 2nd dungeon: move every turn
                case 3:  // 3rd dungeon: move every turn
                    shouldMove = 1;
                    break;
                default:
                    shouldMove = 1;
                    break;
            }
        
        if (shouldMove) {
            // check if player is adjacent - if so, attack instead of moving
            isPlayerAdjacent = 0;
            
            for (dir = 0; dir < 4 && !isPlayerAdjacent; dir++) {
                checkX = enemy->pos.x + adjacentPositions[dir][0];
                checkY = enemy->pos.y + adjacentPositions[dir][1];
                
                if (checkX == state->yohane.pos.x && checkY == state->yohane.pos.y) {
                    isPlayerAdjacent = 1;
                }
            }
            
            if (isPlayerAdjacent) {
                // attack player instead of moving (bat stays in place)
                float damage = 0.5f * state->currentDungeon.difficulty;  // 0.5, 1.0, 1.5 based on dungeon
                
                // apply bat tamer upgrade
                if (state->yohane.hasBatTamer) {
                    damage = 0.5f;
                }
                
                state->yohane.hp -= damage;
                state->damageTakenThisRun = 1;  // track damage for achievement
                setDamageCause(state, "Bat");
                
                printf("%sYohane was attacked by a bat! %s-%.1f HP%s\n", COLOR_RED, COLOR_BRIGHT_RED, damage, COLOR_RESET);
                // check for fatal damage and handle choco-mint saves
                checkFatalDamageAndChocoMintSave(state);
                // Note: if Yohane died, the death will be detected in the main game loop
                waitForKeypress();
                
                // bat does NOT move when attacking - stays in same position
            } else {
                // move randomly (as per specification)
                int dx = 0, dy = 0;
                int randomDir = getRandomNumber(0, 3);
                
                switch (randomDir) {
                    case 0: dx = 1; break;   // right
                    case 1: dx = -1; break;  // left
                    case 2: dy = 1; break;   // down
                    case 3: dy = -1; break;  // up
                }
                
                // for 3rd dungeon bats, they can also move diagonally
                if (state->currentDungeon.difficulty == 3 && getRandomNumber(0, 1) == 0) {
                    // 50% chance for diagonal movement in 3rd dungeon
                    int diagDir = getRandomNumber(0, 3);
                    switch (diagDir) {
                        case 0: dx = 1; dy = 1; break;   // down-Right
                        case 1: dx = -1; dy = 1; break;  // down-Left
                        case 2: dx = 1; dy = -1; break;  // up-Right
                        case 3: dx = -1; dy = -1; break; // up-Left
                    }
                }
                
                // calculate new position
                int newX = enemy->pos.x + dx;
                int newY = enemy->pos.y + dy;
                
                // check if new position is valid and not blocked
                if (newX >= 0 && newX < DUNGEON_WIDTH && newY >= 0 && newY < DUNGEON_HEIGHT) {
                    char targetTile = state->currentDungeon.layout[newY][newX];
                    
                    // bats can move to empty tiles and water tiles (per specifications)
                    if (targetTile == TILE_EMPTY || targetTile == TILE_WATER) {
                        // remember what tile we're moving onto
                        enemy->underlyingTile = targetTile;
                        
                        // move to new position
                        enemy->pos.x = newX;
                        enemy->pos.y = newY;
                    }
                    // if movement is blocked, bat simply stays in place
                }
            }
        }
    }
}
}

// ---------------------- Final Battle Helper Functions ---------------------- :)

/**
 * Spawn a random pair of switches for the final battle.
 * Precondition: state is a valid pointer to an initialized GameState struct.
 * @param state Pointer to the current GameState.
 */
void spawnRandomSwitchPair(GameState *state) {
    // clear any existing switches
    clearAllSwitches(state);
    
    // find valid positions for switches (not on borders, not occupied)
    int attempts = 0;
    int switch1X, switch1Y, switch2X, switch2Y;
    int switchesPlaced = 0;
    
    while (attempts < 100 && !switchesPlaced) {  // prevent infinite loop
        // generate first switch position
        switch1X = getRandomNumber(2, DUNGEON_WIDTH - 3);
        switch1Y = getRandomNumber(2, DUNGEON_HEIGHT - 3);
        
        // check if position is valid (empty space)
        if (state->currentDungeon.layout[switch1Y][switch1X] == TILE_EMPTY) {
            // generate second switch position within constraints
            int minX, maxX, minY, maxY;
            
            if (switch1X - 5 < 2) {
                minX = 2;
            } else {
                minX = switch1X - 5;
            }
            
            if (switch1X + 5 >= DUNGEON_WIDTH - 2) {
                maxX = DUNGEON_WIDTH - 3;
            } else {
                maxX = switch1X + 5;
            }
            
            if (switch1Y - 2 < 2) {
                minY = 2;
            } else {
                minY = switch1Y - 2;
            }
            
            if (switch1Y + 2 >= DUNGEON_HEIGHT - 2) {
                maxY = DUNGEON_HEIGHT - 3;
            } else {
                maxY = switch1Y + 2;
            }
            
            switch2X = getRandomNumber(minX, maxX);
            switch2Y = getRandomNumber(minY, maxY);
            
            // make sure second switch is not on same position and is empty
            if (switch2X != switch1X || switch2Y != switch1Y) {
                if (state->currentDungeon.layout[switch2Y][switch2X] == TILE_EMPTY) {
                    // place both switches
                    state->currentDungeon.layout[switch1Y][switch1X] = TILE_SWITCH;
                    state->currentDungeon.layout[switch2Y][switch2X] = TILE_SWITCH;
                    printf("%sNew switch pair spawned!%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
                    waitForKeypress();
                    switchesPlaced = 1;
                }
            }
        }
        attempts++;
    }
    
    // fallback: place switches at fixed positions if random placement fails
    if (!switchesPlaced) {
        state->currentDungeon.layout[10][15] = TILE_SWITCH;
        state->currentDungeon.layout[12][20] = TILE_SWITCH;
        printf("%sSwitch pair spawned at fallback positions!%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
        waitForKeypress();
    }
}

/**
 * Clear all switches from the dungeon.
 * Precondition: state is a valid pointer to an initialized GameState struct.
 * @param state Pointer to the current GameState.
 */
void clearAllSwitches(GameState *state) {
    int x, y;
    
    for (y = 0; y < DUNGEON_HEIGHT; y++) {
        for (x = 0; x < DUNGEON_WIDTH; x++) {
            if (state->currentDungeon.layout[y][x] == TILE_SWITCH) {
                state->currentDungeon.layout[y][x] = TILE_EMPTY;
            }
        }
    }
}

/**
 * Move the Siren towards the player in the final battle. (TRACK YOHANE ONLY)
 * Precondition: state is a valid pointer, sirenX and sirenY are valid pointers.
 * @param state Pointer to the current GameState.
 * @param sirenX Pointer to Siren's X position.
 * @param sirenY Pointer to Siren's Y position.
 */
void moveSirenTowardsPlayer(GameState *state, int *sirenX, int *sirenY) {
    // siren AI movement algorithm for final battle phase 2
    // uses prioritized pathfinding: diagonal -> horizontal -> vertical
    
    // calculate movement direction towards yohane using directional vectors
    int dx = 0, dy = 0;  // movement deltas: -1, 0, or +1 for each axis
    int moved = 0;       // flag to track if movement succeeded
    
    // determine horizontal direction using sign function
    // dx = sign(yohane.x - siren.x) where sign(n) = {-1 if n<0, 0 if n=0, +1 if n>0}
    if (state->yohane.pos.x > *sirenX) dx = 1;       // yohane is right, move right
    else if (state->yohane.pos.x < *sirenX) dx = -1; // yohane is left, move left
    // if horizontally aligned, dx remains 0
    
    // determine vertical direction using sign function  
    // dy = sign(yohane.y - siren.y) where sign(n) = {-1 if n<0, 0 if n=0, +1 if n>0}
    if (state->yohane.pos.y > *sirenY) dy = 1;       // yohane is below, move down
    else if (state->yohane.pos.y < *sirenY) dy = -1; // yohane is above, move up
    // if vertically aligned, dy remains 0
    
    // movement priority system to make siren navigation intelligent
    // diagonal movement minimizes manhattan distance: |dx| + |dy| -> sqrt(dx² + dy²)
    
    // priority 1: attempt diagonal movement first
    if (!moved) {
        // calculate diagonal target position using vector addition
        // newPosition = currentPosition + movementVector
        // (newX, newY) = (sirenX, sirenY) + (dx, dy)
        int newX = *sirenX + dx;
        int newY = *sirenY + dy;
        
        // boundary check: stay within safe dungeon area
        // constraint: 1 <= x < DUNGEON_WIDTH-1 AND 1 <= y < DUNGEON_HEIGHT-1
        // avoid moving to border tiles (position 0 or max-1)
        if (newX >= 1 && newX < DUNGEON_WIDTH - 1 && newY >= 1 && newY < DUNGEON_HEIGHT - 1) {
            // check target tile type
            char tile = state->currentDungeon.layout[newY][newX];
            
            // movement validation- empty tile only, avoid character collision
            // collision check- newPosition != yohanePosition AND newPosition != lailapsPosition
            if (tile == TILE_EMPTY && 
                !(newX == state->yohane.pos.x && newY == state->yohane.pos.y) &&
                !(newX == state->lailaps.pos.x && newY == state->lailaps.pos.y)) {
                
                // execute diagonal movement
                state->currentDungeon.layout[*sirenY][*sirenX] = TILE_EMPTY; // clear old position
                *sirenX = newX;  // update x coordinate
                *sirenY = newY;  // update y coordinate  
                state->currentDungeon.layout[*sirenY][*sirenX] = TILE_SIREN; // place at new position
                moved = 1; // mark movement complete
            }
        }
    }
    
    // priority 2- horizontal movement fallback
    if (!moved) {
        // horizontal-only movement, y coordinate unchanged
        // newPosition = (sirenX + dx, sirenY + 0) = (sirenX + dx, sirenY)
        int newX = *sirenX + dx;
        int newY = *sirenY;
        
        // boundary check for horizontal movement
        // constraint: 1 <= newX < DUNGEON_WIDTH-1
        if (newX >= 1 && newX < DUNGEON_WIDTH - 1) {
            char tile = state->currentDungeon.layout[newY][newX];
            
            // same validation as diagonal movement
            if (tile == TILE_EMPTY && 
                !(newX == state->yohane.pos.x && newY == state->yohane.pos.y) &&
                !(newX == state->lailaps.pos.x && newY == state->lailaps.pos.y)) {
                
                // execute horizontal movement
                state->currentDungeon.layout[*sirenY][*sirenX] = TILE_EMPTY;
                *sirenX = newX;  // only x changes
                state->currentDungeon.layout[*sirenY][*sirenX] = TILE_SIREN;
                moved = 1;
            }
        }
    }
    
    // priority 3-  vertical movement fallback
    if (!moved) {
        // vertical-only movement, x coordinate unchanged
        // newPosition = (sirenX + 0, sirenY + dy) = (sirenX, sirenY + dy)
        int newX = *sirenX;
        int newY = *sirenY + dy;
        
        // boundary check for vertical movement
        // constraint: 1 <= newY < DUNGEON_HEIGHT-1
        if (newY >= 1 && newY < DUNGEON_HEIGHT - 1) {
            char tile = state->currentDungeon.layout[newY][newX];
            
            // same validation rules
            if (tile == TILE_EMPTY && 
                !(newX == state->yohane.pos.x && newY == state->yohane.pos.y) &&
                !(newX == state->lailaps.pos.x && newY == state->lailaps.pos.y)) {
                
                // execute vertical movement
                state->currentDungeon.layout[*sirenY][*sirenX] = TILE_EMPTY;
                *sirenY = newY;  // only y changes
                state->currentDungeon.layout[*sirenY][*sirenX] = TILE_SIREN;
                moved = 1;
            }
        }
    }
    
    // if moved is still 0, siren is completely blocked
    // can happen when surrounded by walls or characters block all paths
    // siren stays in place this turn, will retry next turn
    // distance to yohane remains: sqrt((yohane.x - siren.x)² + (yohane.y - siren.y)²)
}

/**
 * Try to attack in a given direction during final battle.
 * Precondition: state is a valid pointer, direction values are valid.
 * @param state Pointer to the current GameState.
 * @param dx X direction to attack (-1, 0, or 1).
 * @param dy Y direction to attack (-1, 0, or 1).
 * @param sirenX Pointer to Siren's X coordinate.
 * @param sirenY Pointer to Siren's Y coordinate.
 * @param originalSirenX Original Siren X position for exit placement.
 * @param originalSirenY Original Siren Y position for exit placement.
 * @param phase Current battle phase.
 * @return 1 if attack occurred, 0 if no attack, -1 if battle ended.
 */
int tryAttackInDirection(GameState *state, int dx, int dy, int *sirenX, int *sirenY, int originalSirenX, int originalSirenY, int phase) {
    int attackResult = 0; // default: no attack possible
    int attackX = state->yohane.pos.x + dx;
    int attackY = state->yohane.pos.y + dy;
    
    // check bounds
    if (attackX >= 0 && attackX < DUNGEON_WIDTH && attackY >= 0 && attackY < DUNGEON_HEIGHT) {
        // check if attacking the Siren in phase 2
        if (phase == 2 && attackX == *sirenX && attackY == *sirenY) {
            // treat Siren like a bat - one hit defeat
            printf("%sYou attack the Siren with all your power!%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
            printf("%s%sYou defeated the Siren and saved the idols' voices!%s\n", STYLE_BOLD, COLOR_BRIGHT_GREEN, COLOR_RESET);
            
            // turn all remaining bats to gold now that the Siren is defeated
            for (int i = 0; i < state->currentDungeon.enemyCount; i++) {
                if (state->currentDungeon.enemies[i].isActive) {
                    int batX = state->currentDungeon.enemies[i].pos.x;
                    int batY = state->currentDungeon.enemies[i].pos.y;
                    // restore underlying tile first, then place gold on top
                    state->currentDungeon.layout[batY][batX] = state->currentDungeon.enemies[i].underlyingTile;
                    if (state->currentDungeon.layout[batY][batX] == TILE_EMPTY) {
                        state->currentDungeon.layout[batY][batX] = TILE_GOLD_BAT;
                    }
                    state->currentDungeon.enemies[i].isActive = 0;
                }
            }
            
            // remove the Siren from its current position and place special gold (750g value)
            state->currentDungeon.layout[*sirenY][*sirenX] = TILE_GOLD_SIREN;  // capital G for Siren gold
            
            // move the Siren position out of bounds so it won't be detected for attacks anymore
            *sirenX = -1;
            *sirenY = -1;
            
            // place exit at siren's original position
            state->currentDungeon.layout[originalSirenY][originalSirenX] = TILE_EXIT;
            printf("%s%sThe Siren dropped valuable gold and an exit has appeared!%s\n", STYLE_BOLD, COLOR_BRIGHT_CYAN, COLOR_RESET);
            printf("%s%sYou can now collect the gold and reach the exit to complete the battle!%s\n", STYLE_BOLD, COLOR_BRIGHT_GREEN, COLOR_RESET);
            waitForKeypress();
            state->finalBattleWon = 1;
            attackResult = 1; // attack occurred
        } else {
            // check if attacking a bat
            int batFound = 0;
            for (int i = 0; i < state->currentDungeon.enemyCount && !batFound; i++) {
                if (state->currentDungeon.enemies[i].isActive &&
                    state->currentDungeon.enemies[i].pos.x == attackX &&
                    state->currentDungeon.enemies[i].pos.y == attackY) {
                    
                    printf("%sYohane attacks the bat!%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET);
                    
                    // deactivate the bat
                    state->currentDungeon.enemies[i].isActive = 0;
                    
                    // place gold where the bat was
                    state->currentDungeon.layout[attackY][attackX] = TILE_GOLD_BAT;
                    
                    waitForKeypress();
                    attackResult = 1; // attack occurred
                    batFound = 1; // mark that we found and attacked a bat
                }
            }
        }
    }
    
    return attackResult;
}

/**
 * Check if the Siren would attack Yohane after she moves in a given direction.
 * This implements the correct Siren attack logic from specs - Siren attacks if Yohane
 * moves to a position where the Siren can hit her, but NOT if she's attacking the Siren.
 * According to specs, moving Up/Down when adjacent to Siren triggers attack,
 * but attacking the Siren (moving towards it) is safe.
 * Precondition: state is valid, direction values are valid, phase 2 active
 * @param state Pointer to the current GameState
 * @param dx X direction Yohane wants to move (-1, 0, or 1)
 * @param dy Y direction Yohane wants to move (-1, 0, or 1) 
 * @param sirenX Current Siren X position
 * @param sirenY Current Siren Y position
 * @param phase Current battle phase (should be 2)
 * @return 1 if Siren would attack, 0 otherwise
 */
int wouldSirenAttackAfterMovement(GameState *state, int dx, int dy, int sirenX, int sirenY, int phase) {
    int wouldAttack = 0;
    
    // only check in phase 2 when Siren is active
    if (phase == 2 && sirenX >= 0 && sirenY >= 0) {
        // First priority: check if Lailaps is currently adjacent to Siren
        // If so, Siren will attack Lailaps regardless of Yohane's movement
        if (arePositionsAdjacent(state->lailaps.pos.x, state->lailaps.pos.y, sirenX, sirenY)) {
            // Siren will attack Lailaps - this is handled separately and causes immediate game over
            wouldAttack = 2; // Special return value indicating Lailaps will be attacked
        } else {
            // calculate Yohane's new position after movement
            int yohaneNewX = state->yohane.pos.x + dx;
            int yohaneNewY = state->yohane.pos.y + dy;
            
            // check if new position is valid
            if (yohaneNewX >= 0 && yohaneNewX < DUNGEON_WIDTH && 
                yohaneNewY >= 0 && yohaneNewY < DUNGEON_HEIGHT) {
                
                // check if this movement would be an attack on the Siren
                int attackX = state->yohane.pos.x + dx;
                int attackY = state->yohane.pos.y + dy;
                
                if (attackX != sirenX || attackY != sirenY) {
                    // This is NOT an attack on the Siren
                    // check if after Yohane moves, she would be adjacent to the Siren
                    if (arePositionsAdjacent(yohaneNewX, yohaneNewY, sirenX, sirenY)) {
                        // Yohane would be adjacent to Siren after movement (but not attacking)
                        // According to specs, this can trigger a Siren attack
                        wouldAttack = 1;
                    }
                }
                // If it IS an attack on the Siren, wouldAttack stays 0 (safe)
            }
        }
    }
    
    return wouldAttack;
}