#include <stdio.h>
#include <stdlib.h>
#include "include/game_structs.h"
#include "include/utils.h"

// Test to verify that invalid input doesn't count as moves
int main() {
    GameState state;
    int x, y;
    char input = 'F';
    int moveResult = 0;
    
    initializeStaticData(&state);
    
    state.yohane.gold = 500;
    state.yohane.hp = 3.0f;
    state.yohane.maxHp = 3.0f;
    state.yohane.currentItemOnHand = ITEM_NONE;
    state.moveCounter = 0;  // Start with 0 moves
    
    state.currentDungeon.difficulty = 1;
    state.currentDungeon.floor = 1;
    state.currentDungeon.maxFloors = 1;
    state.currentDungeon.idolIndex = IDOL_CHIKA;
    
    for (y = 0; y < DUNGEON_HEIGHT; y++) {
        for (x = 0; x < DUNGEON_WIDTH; x++) {
            if (y == 0 || y == DUNGEON_HEIGHT-1 || x == 0 || x == DUNGEON_WIDTH-1) {
                state.currentDungeon.layout[y][x] = TILE_BORDER;
            } else {
                state.currentDungeon.layout[y][x] = TILE_EMPTY;
            }
        }
    }
    
    state.yohane.pos.x = 35;
    state.yohane.pos.y = 10;
    state.currentDungeon.layout[10][35] = TILE_PLAYER;
    state.underlyingTile = TILE_EMPTY;
    
    printf("=== TESTING INVALID INPUT BEHAVIOR ===\n\n");
    printf("Initial state:\n");
    renderDungeon(&state);
    printf("Move counter before invalid input: %d\n\n", state.moveCounter);
    
    printf("Simulating invalid input 'F' (should not count as move)...\n");
    
    moveResult = 0;
    switch (toupper(input)) {
        case 'W': 
            moveResult = tryMovePlayer(&state, 0, -1);
            if (moveResult > 0) {
                processTurn(&state);
                if (moveResult == 2) {
                    printf("Player reached exit\n");
                }
            }
            break;
        case 'A': 
            moveResult = tryMovePlayer(&state, -1, 0);
            if (moveResult > 0) {
                processTurn(&state);
                if (moveResult == 2) {
                    printf("Player reached exit\n");
                }
            }
            break;
        case 'S': 
            moveResult = tryMovePlayer(&state, 0, 1);
            if (moveResult > 0) {
                processTurn(&state);
                if (moveResult == 2) {
                    printf("Player reached exit\n");
                }
            }
            break;
        case 'D': 
            moveResult = tryMovePlayer(&state, 1, 0);
            if (moveResult > 0) {
                processTurn(&state);
                if (moveResult == 2) {
                    printf("Player reached exit\n");
                }
            }
            break;
        case 'X': 
            processTurn(&state);
            state.moveCounter++;  
            break;
        case '[':
            cycleItem(&state, -1);
            
            break;
        case ']': 
            cycleItem(&state, 1);
            
            break;
        case ' ': 
            useItem(&state);
            processTurn(&state);
            state.moveCounter++;  
            break;
        case 27: 
        case 'M': 
            printf("Menu would show here\n");
            break;
        default:
            
            printf("%sERMMM WATASIGMA%s\n", COLOR_RED, COLOR_RESET);
            break;
    }
    
    printf("Move counter after invalid input: %d\n", state.moveCounter);
    printf("Expected: 1 (should change)\n\n");
    
    
    
    input = 'W';
    moveResult = 0;
    switch (toupper(input)) {
        case 'W': 
            moveResult = tryMovePlayer(&state, 0, -1);
            if (moveResult > 0) {
                processTurn(&state);
                if (moveResult == 2) {
                    printf("Player reached exit\n");
                }
            }
            break;
        case 'A': 
            moveResult = tryMovePlayer(&state, -1, 0);
            if (moveResult > 0) {
                processTurn(&state);
                if (moveResult == 2) {
                    printf("Player reached exit\n");
                }
            }
            break;
        case 'S': 
            moveResult = tryMovePlayer(&state, 0, 1);
            if (moveResult > 0) {
                processTurn(&state);
                if (moveResult == 2) {
                    printf("Player reached exit\n");
                }
            }
            break;
        case 'D': 
            moveResult = tryMovePlayer(&state, 1, 0);
            if (moveResult > 0) {
                processTurn(&state);
                if (moveResult == 2) {
                    printf("Player reached exit\n");
                }
            }
            break;
        case 'X': 
            processTurn(&state);
            state.moveCounter++;  
        case '[': 
            cycleItem(&state, -1);
        
            break;
        case ']':
            cycleItem(&state, 1);
            
            break;
        case ' ':
            useItem(&state);
            processTurn(&state);
            state.moveCounter++;  
            break;
        case 27: 
        case 'M': 
            printf("Menu would show here\n");
            break;
        default:
           
            printf("%sInvalid input. Use WASD to move, [ ] for items, Space to use item, M for menu.%s\n", COLOR_RED, COLOR_RESET);
            
            break;
    }
    
    printf("Move counter after valid input 'W': %d\n", state.moveCounter);
    printf("Expected: 2 (should have incremented)\n");
    
    return 0;
}
