/**
 * Description: Core data structures and constants for the Yohane game
 * Programmed by: 2ru17 (S14, PROG2-T3)
 * Last modified: 2025-07-25
 * Version: 1.8
 * Acknowledgements: This code is based on the project specifications provided.
 */

#ifndef GAME_STRUCTS_H
#define GAME_STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// ---------------------- game constants ----------------------

// game parameters
#define MAX_IDOLS 9
#define MAX_ACHIEVEMENTS 28
#define MAX_BATS_PER_FLOOR 17
#define DUNGEON_HEIGHT 20
#define DUNGEON_WIDTH 70
#define MAX_NAME_LENGTH 50
#define MAX_DESC_LENGTH 150
#define MAX_DATE_LENGTH 20
#define MAX_SPAWN_ATTEMPTS 50
#define SAVE_FILE_NAME "yohane.sav"

// item IDs
#define ITEM_NONE 0
#define ITEM_TEARS 1
#define ITEM_NOPPO 2
#define ITEM_CHOCO_MINT 3

// idol indices for aqours array
#define IDOL_CHIKA    0
#define IDOL_RIKO     1
#define IDOL_YOU      2
#define IDOL_HANAMARU 3
#define IDOL_RUBY     4
#define IDOL_YOSHIKO  5  // yoshiko/yohane
#define IDOL_DIA      6
#define IDOL_KANAN    7
#define IDOL_MARI     8

// bat AI types
#define BAT_TYPE_DUNGEON_1 1
#define BAT_TYPE_DUNGEON_2 2
#define BAT_TYPE_DUNGEON_3 3

// tile types - for the dungeon grid
#define TILE_EMPTY '.'
#define TILE_WALL 'v'
#define TILE_SPIKE 'x'
#define TILE_WATER 'w'
#define TILE_HEAT 'h'
#define TILE_TREASURE 'T'
#define TILE_EXIT 'E'
#define TILE_PLAYER 'Y'
#define TILE_BAT 'b'
#define TILE_BORDER '*'
#define TILE_LAILAPS 'L'
#define TILE_SWITCH '0'
#define TILE_SIREN 'S'
#define TILE_GOLD_BAT 'g'
#define TILE_GOLD_SIREN 'G'

// ---------------------- Data Structures ----------------------

// position struct for coordinates
typedef struct {
    int x;
    int y;
} Position;

/**
 * @struct Player
 * Contains all player-related data.
 * Precondition: None.
 */
typedef struct {
    char name[MAX_NAME_LENGTH];
    float hp;
    float maxHp;
    int gold;
    int currentItemOnHand;
    int hasShovelUpgrade;
    int hasBatTamer;
    int hasAirShoes;
    Position pos;
} Player;

/**
 * @struct Idol
 * Contains data for each Aqours idol.
 * Precondition: None.
 */
typedef struct {
    char name[MAX_NAME_LENGTH];
    char dungeonName[MAX_NAME_LENGTH];
    int isRescuedOnce;
    int rescueCount;
} Idol;

/**
 * @struct Achievement
 * Contains data for each achievement.
 * Precondition: None.
 */
typedef struct {
    char name[MAX_NAME_LENGTH];
    char description[MAX_DESC_LENGTH];
    int isEarned;
    char dateEarned[MAX_DATE_LENGTH];
} Achievement;

/**
 * @struct Enemy
 * Contains data for an enemy (bat) in the dungeon.
 * Precondition: None.
 */
typedef struct {
    Position pos;
    int isActive;
    int type;  // 1:Dungeon1Bat, 2:Dungeon2Bat, 3:Dungeon3Bat
    int turnCounter;
    int moveInterval;
    int damage;
    char underlyingTile;  // track the tile the enemy is standing on
} Enemy;

/**
 * @struct Dungeon
 * Contains data for a dungeon.
 * Precondition: None.
 */
typedef struct {
    char layout[DUNGEON_HEIGHT][DUNGEON_WIDTH+1];
    int floor;
    int maxFloors;
    int difficulty;
    int idolIndex;
    int contentHeight;  // number of lines actually read from file (dungeon rendering keeps on bugging out for some reason)
    Enemy enemies[MAX_BATS_PER_FLOOR];
    int enemyCount;
} Dungeon;

/**
 * @struct GameState
 * Master struct containing the entire game state.
 * Precondition: None.
 */
typedef struct {
    // player data
    Player yohane;
    Player lailaps;
    
    // idol data
    Idol aqours[MAX_IDOLS];
    
    // achievement data
    Achievement achievements[MAX_ACHIEVEMENTS];
    
    // inventory data
    int tearsCount;
    int noppoBreadCount;
    int chocoMintCount;
    
    // game progress data
    int isPlaythroughActive;
    int totalGoldCollected;
    int totalGoldSpent;
    int totalDungeonsCleared;
    int finalBattleWonCount;
    int rescuedInThisRun[3];
    int dungeonsCleared[3];
    int finalBattleSwitchesHit;
    int finalBattleWon;
    
    // shop purchase tracking per playthrough (tears item)
    int tearsPurchasedThisPlaythrough;
    
    // special achievement tracking
    int damageTakenThisRun;  // for "Aozora Jumping Heart!" achievement
    int chocoMintUsedToSave;  // for "Ruby-chan! Hai? Nani ga suki?" achievement
    
    // treasure tracking to prevent duplication exploits
    // treasureConsumed[dungeonIndex][floor] = 1 if treasure consumed on that floor
    int treasureConsumed[3][5];  // max 3 dungeons, max 5 floors per dungeon
    
    // current dungeon data
    Dungeon currentDungeon;
    int turnCounter;
    char underlyingTile;  // tile that player is standing on
    char lailapsUnderlyingTile;  // tile that Lailaps is standing on (final battle only)
    int heatTileCounter;  // number of turns spent on heat tiles
    char lastDamageCause[50];  // track what caused the last damage for better Game Over screens
} GameState;

// ---------------------- Function Prototypes ----------------------

// from game_setup.c
void initializeStaticData(GameState *state);
void setupNewGame(GameState *state, int preserveGold);
void checkAchievements(GameState *state);

// from file_io.c
int saveGame(GameState *state);
int loadGame(GameState *state);
int saveGameToSlot(GameState *state, int slot);
int loadGameFromSlot(GameState *state, int slot);
int saveFileExists(int slot);
int deleteSaveFile(int slot);
void getSaveFileName(int slot, char *filename);
void initializeGodMode(GameState *state);

// from game_logic.c
void displayMainMenu(GameState *state);
void runDungeonSelection(GameState *state);
void displayDungeonSelectionMenu(GameState *state);
void viewInventory(GameState *state);
void viewAchievements(GameState *state);
void displayShopMenu(GameState *state);
int runDungeon(GameState *state, int dungeonIndex);
int runFinalBattle(GameState *state);
void useItem(GameState *state);

// helper functions for final battle
int moveCharactersInFinalBattle(GameState *state, int dx, int dy);
int isPositionValidForMovement(GameState *state, int x, int y);
int isPositionAdjacentToYohaneOrLailaps(GameState *state, int x, int y);
int arePositionsAdjacent(int x1, int y1, int x2, int y2);
void spawnBatInFinalBattle(GameState *state);
void moveBatInFinalBattle(GameState *state, int batIndex, int *turnBatAttackedYohane, int *turnBatAttackedLailaps);
void spawnRandomSwitchPair(GameState *state);
void moveSirenTowardsPlayer(GameState *state, int *sirenX, int *sirenY);
void clearAllSwitches(GameState *state);
int tryAttackInDirection(GameState *state, int dx, int dy, int *sirenX, int *sirenY, int originalSirenX, int originalSirenY, int phase);
int wouldSirenAttackAfterMovement(GameState *state, int dx, int dy, int sirenX, int sirenY, int phase);
void showDefeatScreen(GameState *state, const char *causeOfDeath);
void showLoadingTip();

// damage and death handling
int checkFatalDamageAndChocoMintSave(GameState *state);

// dungeon gameplay helper functions
int runDungeonGameplay(GameState *state);
int tryMovePlayer(GameState *state, int dx, int dy);
void processTurn(GameState *state, int playerMoved);
void cycleItem(GameState *state, int direction);
int showDungeonPauseMenu(GameState *state);
void moveEnemies(GameState *state);

// from dungeon_handler.c
int loadDungeonFromFile(GameState *state, const char* filename);
void renderDungeon(GameState *state);

// from utils.c
void clearScreen(void);
void waitForKeypress(void);
char getCharNonBlocking(void);
char* getCurrentDateTime(void);
int getRandomNumber(int min, int max);

#endif // GAME_STRUCTS_H
