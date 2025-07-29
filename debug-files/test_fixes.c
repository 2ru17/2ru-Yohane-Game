#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/game_structs.h"
#include "include/utils.h"

// Test to verify the bug fixes on game play
int main() {
    GameState state;
    
    
    initializeStaticData(&state);
    
    
    state.yohane.gold = 500;
    state.yohane.hp = 3.0f;
    state.yohane.maxHp = 3.0f;
    state.yohane.currentItemOnHand = ITEM_NONE;
    state.moveCounter = 0;
    strcpy(state.lastDamageCause, "Unknown");
    
   
    state.currentDungeon.difficulty = 1;
    state.currentDungeon.floor = 1;
    state.currentDungeon.maxFloors = 1;
    state.currentDungeon.idolIndex = IDOL_CHIKA;
    
    printf("=== TESTING BUG FIXES ===\n\n");
    
   // last damageCause (what killed you last)
    printf("Test 1: Damage cause tracking\n");
    printf("Initial damage: %s\n", state.lastDamageCause);
    
    
    strcpy(state.lastDamageCause, "Heat Tiles");
    printf("After heat damage: %s\n", state.lastDamageCause);
    
    
    strcpy(state.lastDamageCause, "Bat");
    printf("After bat damage: %s\n", state.lastDamageCause);
    
    
    strcpy(state.lastDamageCause, "Spike Walls");
    printf("After spike damage: %s\n\n", state.lastDamageCause);
    
    printf("Test 2: Game Over screen would now show: 'Killed by: %s'\n\n", state.lastDamageCause);
    
    return 0;
}
