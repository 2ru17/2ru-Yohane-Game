# Yohane The Parhelion - Complete Function Implementation Documentation

This md file provides a function-by-function reference for all of the game mechanics in my Yohane The Parhelion codebase. Each function includes its signature, detailed implementation notes, and specific editing guidance.

## Data Structures Overview

### Core Game State Structure
```c
typedef struct GameState {
    Player yohane;                    // Main player character data
    Player lailaps;                   // Companion character for final battle
    Idol aqours[MAX_IDOLS];          // Array of 8 Aqours idol members
    Achievement achievements[MAX_ACHIEVEMENTS]; // All 28 game achievements
    int tearsCount, noppoBreadCount, chocoMintCount; // Inventory items
    Dungeon currentDungeon;          // Current dungeon layout and enemies
    // ... additional state variables
} GameState;
```
**Location:** include/game_structs.h:155-200

**Details:** Master game state container that holds all persistent data

**To modify game state structure:** Edit this typedef at lines 155-200

---

## Complete Function Reference

### MAIN GAME LOOP FUNCTIONS

#### int main()
