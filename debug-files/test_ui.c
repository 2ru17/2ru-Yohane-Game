#include <stdio.h>
#include <stdlib.h>
#include "include/game_structs.h"
#include "include/utils.h"

// debugging to verify the UI displays Total Gold correctly
int main() {
    GameState state;
    int x, y;
    
    initializeStaticData(&state);
    
    state.yohane.gold = 1250;  
    state.yohane.hp = 2.5f;
    state.yohane.maxHp = 3.0f;
    state.yohane.currentItemOnHand = ITEM_NOPPO;
    state.moveCounter = 42;  
    
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
    
    state.yohane.pos.x = 5;
    state.yohane.pos.y = 5;
    state.currentDungeon.layout[5][5] = TILE_PLAYER;   
    renderDungeon(&state);
    
    return 0;
}
