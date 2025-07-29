/**
 * Description: Handles game initialization, static data setup, and achievement system
 * Programmed by: 2ru17 (S14, PROG2-T3)
 * Last modified: 2025-07-22
 * Version: 1.7
 * Acknowledgements: This code is based on the project specifications provided.
 */

#include "game_structs.h"
#include "utils.h"



/**
 * Initialize all static game data.
 * Precondition: state pointer must be valid and memory allocated
 * @param state Pointer to the GameState struct to initialize
 */
void initializeStaticData(GameState *state) {
    // initialize aqours idols and dungeons (per specifications)
    strcpy(state->aqours[IDOL_CHIKA].name, "Chika");
    strcpy(state->aqours[IDOL_CHIKA].dungeonName, "Yasudaya Ryokan");
    
    strcpy(state->aqours[IDOL_RIKO].name, "Riko");
    strcpy(state->aqours[IDOL_RIKO].dungeonName, "Numazu Deep Sea Aquarium");
    
    strcpy(state->aqours[IDOL_YOU].name, "You");
    strcpy(state->aqours[IDOL_YOU].dungeonName, "Izu-Mito Sea Paradise");
    
    strcpy(state->aqours[IDOL_HANAMARU].name, "Hanamaru");
    strcpy(state->aqours[IDOL_HANAMARU].dungeonName, "Shougetsu Confectionary");
    
    strcpy(state->aqours[IDOL_RUBY].name, "Ruby");
    strcpy(state->aqours[IDOL_RUBY].dungeonName, "Nagahama Castle Ruins");
    
    strcpy(state->aqours[IDOL_DIA].name, "Dia");
    strcpy(state->aqours[IDOL_DIA].dungeonName, "Numazugoyotei");
    
    strcpy(state->aqours[IDOL_KANAN].name, "Kanan");
    strcpy(state->aqours[IDOL_KANAN].dungeonName, "Uchiura Bay Pier");
    
    strcpy(state->aqours[IDOL_MARI].name, "Mari");
    strcpy(state->aqours[IDOL_MARI].dungeonName, "Awashima Marine Park");
    
    // initialize achievements (all 28 achievements according to specifications)
    strcpy(state->achievements[0].name, "Yohane Descends!");
    strcpy(state->achievements[0].description, "Cleared first dungeon");
    
    strcpy(state->achievements[1].name, "Mikan Power!");
    strcpy(state->achievements[1].description, "Rescued Chika for the first time");
    
    strcpy(state->achievements[2].name, "Riko-chan BEAM!");
    strcpy(state->achievements[2].description, "Rescued Riko for the first time");
    
    strcpy(state->achievements[3].name, "Yousoro!");
    strcpy(state->achievements[3].description, "Rescued You for the first time");
    
    strcpy(state->achievements[4].name, "It's the future, zura!");
    strcpy(state->achievements[4].description, "Rescued Hanamaru for the first time");
    
    strcpy(state->achievements[5].name, "Ganbaruby!");
    strcpy(state->achievements[5].description, "Rescued Ruby for the first time");
    
    strcpy(state->achievements[6].name, "Buu-buu desu wa!");
    strcpy(state->achievements[6].description, "Rescued Dia for the first time");
    
    strcpy(state->achievements[7].name, "Hug!!!");
    strcpy(state->achievements[7].description, "Rescued Kanan for the first time");
    
    strcpy(state->achievements[8].name, "Shiny!");
    strcpy(state->achievements[8].description, "Rescued Mari for the first time");
    
    strcpy(state->achievements[9].name, "In This Unstable World!");
    strcpy(state->achievements[9].description, "Beat the Final boss for the first time");
    
    strcpy(state->achievements[10].name, "One more sunshine story!");
    strcpy(state->achievements[10].description, "Rescued Chika twice");
    
    strcpy(state->achievements[11].name, "Pianoforte Monologue!");
    strcpy(state->achievements[11].description, "Rescued Riko twice");
    
    strcpy(state->achievements[12].name, "Beginner's Sailing!");
    strcpy(state->achievements[12].description, "Rescued You twice");
    
    strcpy(state->achievements[13].name, "Oyasuminasan!");
    strcpy(state->achievements[13].description, "Rescued Hanamaru twice");
    
    strcpy(state->achievements[14].name, "Red Gem Wink!");
    strcpy(state->achievements[14].description, "Rescued Ruby twice");
    
    strcpy(state->achievements[15].name, "White First Love!");
    strcpy(state->achievements[15].description, "Rescued Dia twice");
    
    strcpy(state->achievements[16].name, "Sakana ka Nandaka!");
    strcpy(state->achievements[16].description, "Rescued Kanan twice");
    
    strcpy(state->achievements[17].name, "New Winding Road!");
    strcpy(state->achievements[17].description, "Rescued Mari twice");
    
    strcpy(state->achievements[18].name, "Deep Resonance!");
    strcpy(state->achievements[18].description, "Beat the Final boss twice");
    
    strcpy(state->achievements[19].name, "No. 10!");
    strcpy(state->achievements[19].description, "Clear 10 dungeons");
    
    strcpy(state->achievements[20].name, "CYaRon!");
    strcpy(state->achievements[20].description, "Rescued Chika, You, and Ruby (Not necessarily in one playthrough)");
    
    strcpy(state->achievements[21].name, "AZALEA!");
    strcpy(state->achievements[21].description, "Rescued Hanamaru, Dia, and Kanan (Not necessarily in one playthrough)");
    
    strcpy(state->achievements[22].name, "Guilty Kiss!");
    strcpy(state->achievements[22].description, "Rescued Riko and Mari (Not necessarily in one playthrough)");
    
    strcpy(state->achievements[23].name, "Eikyuu Hours!");
    strcpy(state->achievements[23].description, "Have Yohane rescue all Aqours members for the first time");
    
    strcpy(state->achievements[24].name, "Aozora Jumping Heart!");
    strcpy(state->achievements[24].description, "Clear a dungeon without incurring any damage");
    
    strcpy(state->achievements[25].name, "Mitaiken Horizon!");
    strcpy(state->achievements[25].description, "Accumulate a total of 5000G spent on Hanamaru's stores across multiple playthroughs");
    
    strcpy(state->achievements[26].name, "Ruby-chan! Hai? Nani ga suki?");
    strcpy(state->achievements[26].description, "Get saved by a fatal blow from Ruby's choco-mint ice cream item.");
    
    strcpy(state->achievements[27].name, "Step! ZERO to ONE!");
    strcpy(state->achievements[27].description, "Complete a playthrough with 0G on-hand at the end");
    
    // reset all dynamic data
    for (int i = 0; i < MAX_IDOLS; i++) {
        state->aqours[i].isRescuedOnce = 0;
        state->aqours[i].rescueCount = 0;
    }
    
    for (int i = 0; i < MAX_ACHIEVEMENTS; i++) {
        state->achievements[i].isEarned = 0;
        state->achievements[i].dateEarned[0] = '\0';
    }
    
    // initialize player (Yohane)
    strcpy(state->yohane.name, "Yohane");
    state->yohane.maxHp = 3.0f;
    state->yohane.hp = 3.0f;
    state->yohane.gold = 0;
    state->yohane.currentItemOnHand = ITEM_NONE;
    state->yohane.hasShovelUpgrade = 0;
    state->yohane.hasBatTamer = 0;
    state->yohane.hasAirShoes = 0;
    
    // initialize Lailaps (for final battle)
    strcpy(state->lailaps.name, "Lailaps");
    state->lailaps.maxHp = 4.0f;
    state->lailaps.hp = 4.0f;

    // Well i may or may not initiliaze (cause technically lailaps doesnt need this)
    // state->lailaps.gold = 0;
    // state->lailaps.currentItemOnHand = ITEM_NONE;
    // state->lailaps.hasShovelUpgrade = 0;
    // state->lailaps.hasBatTamer = 0;
    // state->lailaps.hasAirShoes = 0;
    
    // initialize Lailaps position to invalid coordinates (only used in final battle)
    state->lailaps.pos.x = -1;
    state->lailaps.pos.y = -1;
    
    // lailaps HP is already initialized in the Player struct above
    
    // initialize item counts
    state->tearsCount = 0;
    state->noppoBreadCount = 0;
    state->chocoMintCount = 0;
    
    // initialize game state
    state->isPlaythroughActive = 0;
    state->totalGoldCollected = 0;
    state->totalGoldSpent = 0;
    state->totalDungeonsCleared = 0;  
    state->finalBattleWonCount = 0;
    state->finalBattleWon = 0;  // initialize final battle won flag
    state->finalBattleSwitchesHit = 0;
    state->turnCounter = 0;
    
    // initialize shop purchase tracking
    state->tearsPurchasedThisPlaythrough = 0;
    
    // initialize achievement tracking variables
    state->damageTakenThisRun = 0;
    state->chocoMintUsedToSave = 0;
    state->underlyingTile = TILE_EMPTY;
    state->lailapsUnderlyingTile = TILE_EMPTY;
    
    // clear dungeon tracking arrays
    for (int i = 0; i < 3; i++) {
        state->rescuedInThisRun[i] = -1;
        state->dungeonsCleared[i] = 0;
        // initialize treasure tracking for each dungeon and floor
        for (int j = 0; j < 5; j++) {
            state->treasureConsumed[i][j] = 0;
        }
    }
    
    // initialize dungeon layout
    for (int i = 0; i < DUNGEON_HEIGHT; i++) {
        for (int j = 0; j < DUNGEON_WIDTH; j++) {
            state->currentDungeon.layout[i][j] = ' ';
        }
        state->currentDungeon.layout[i][DUNGEON_WIDTH] = '\0';
    }
    
    // initialize dungeon properties
    state->currentDungeon.floor = 1;
    state->currentDungeon.maxFloors = 1;
    state->currentDungeon.difficulty = 1;
    state->currentDungeon.idolIndex = -1;
    state->currentDungeon.enemyCount = 0;
    
    // initialize position tracking
    state->yohane.pos.x = 0;
    state->yohane.pos.y = 0;
    
    // initialize enemy arrays
    for (int i = 0; i < MAX_BATS_PER_FLOOR; i++) {
        state->currentDungeon.enemies[i].isActive = 0;
        state->currentDungeon.enemies[i].type = 0;
        state->currentDungeon.enemies[i].pos.x = 0;
        state->currentDungeon.enemies[i].pos.y = 0;
        state->currentDungeon.enemies[i].turnCounter = 0;
        state->currentDungeon.enemies[i].moveInterval = 1;
        state->currentDungeon.enemies[i].damage = 1;
        
        // enemy array is tracked within currentDungeon.enemies
    }
}
























/**
 * Set up a new game or playthrough.
 * Precondition: state pointer must be valid and static data initialized
 * @param state Pointer to the GameState struct to set up
 * @param preserveGold 1 to preserve gold (after Game Over), 0 to reset it (fresh game)
 */
void setupNewGame(GameState *state, int preserveGold) {
    int availableIdols[MAX_IDOLS];  // array to hold indices of idols that can be rescued
    int availableCount = 0;         // counter for how many idols are available to rescue
    int isCreatorMode = (state->yohane.maxHp >= 999.0f); // detects if creator mode is active
    int isNewGamePlus;              // flag for New Game+ detection
    int allIdolsRescuedOnce;        // flag for all idols rescued status
    int randIndex, selectedIdol;    // variables for random idol selection
    
    // reset player stats for the new run (preserve creator mode stats)
    if (!isCreatorMode) {
        // Rogue-lite HP mechanics: distinguish between New Game+ and Game Over
        // New Game+ (after successful completion): preserve HP upgrades (specs does not clearly specify this, ask sir later)
        // Game Over or fresh game: reset HP to base 3
        isNewGamePlus = (state->finalBattleWonCount > 0 && !preserveGold);
        
        if (isNewGamePlus) {
            // New Game+: Keep permanent HP upgrades (Stewshine, Mikan Mochi, Kurosawa Macha)
            // Heal to full HP in case player was damaged
            state->yohane.hp = state->yohane.maxHp;
        } else {
            // Fresh game or Game Over: Reset HP to base 3
            state->yohane.maxHp = 3.0f;
            state->yohane.hp = 3.0f;
        }
        
        if (!preserveGold) {
            state->yohane.gold = 0;
        }
        // Always reset non-permanent upgrades (these are "one-time use per playthrough") (this )
        state->yohane.hasShovelUpgrade = 0;
        state->yohane.hasBatTamer = 0;
        state->yohane.hasAirShoes = 0;
    } else {
        // in creator mode, preserve the higher HP
        state->yohane.hp = state->yohane.maxHp;
    }
    state->yohane.currentItemOnHand = ITEM_NONE;
    
    // reset Lailaps for the final battle
    if (!isCreatorMode) {
        state->lailaps.maxHp = 4.0f;
        state->lailaps.hp = 4.0f;
    } else {
        state->lailaps.hp = state->lailaps.maxHp;
    }
    // reset Lailaps position (only used in final battle)
    state->lailaps.pos.x = -1;
    state->lailaps.pos.y = -1;
    
    // reset current playthrough tracking
    state->isPlaythroughActive = 1;
    state->finalBattleSwitchesHit = 0;
    state->finalBattleWon = 0;  // reset final battle flag for new playthrough
    state->turnCounter = 0;
    // state->moveCounter = 0;  // reset move counter for new game - COMMENTED OUT
    state->heatTileCounter = 0;  // reset heat tile counter
    setDamageCause(state, "Unknown");  // initialize damage cause tracking safely
    
    // reset per-playthrough shop purchases 
    state->tearsPurchasedThisPlaythrough = 0;
    
    // reset achievement tracking for this run
    state->damageTakenThisRun = 0;
    state->chocoMintUsedToSave = 0;
    
    // reset treasure consumption tracking for new playthrough
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            state->treasureConsumed[i][j] = 0;
        }
    }
    
    // build a list of available idols based on specs requirements
    // exclude Yoshiko (IDOL_YOSHIKO) since she's the player character
    
    // first, check if all 8 idols have been rescued at least once
    allIdolsRescuedOnce = 1;
    for (int i = 0; i < MAX_IDOLS && allIdolsRescuedOnce; i++) {
        if (i != IDOL_YOSHIKO && !state->aqours[i].isRescuedOnce) {
            allIdolsRescuedOnce = 0;
        }
    }
    
    if (allIdolsRescuedOnce) {
        // all 8 idols have been rescued - add all of them to available list
        for (int i = 0; i < MAX_IDOLS; i++) {
            if (i != IDOL_YOSHIKO) {
                availableIdols[availableCount++] = i;
            }
        }
    } else {
        // not all idols rescued yet - only add unrescued ones
        for (int i = 0; i < MAX_IDOLS; i++) {
            if (i != IDOL_YOSHIKO && !state->aqours[i].isRescuedOnce) {
                availableIdols[availableCount++] = i;
            }
        }
    }
    
    // reset selected idols and cleared dungeons
    for (int i = 0; i < 3; i++) {
        state->rescuedInThisRun[i] = -1;
        state->dungeonsCleared[i] = 0;
    }
    
    // select 3 random idols
    for (int i = 0; i < 3 && availableCount > 0; i++) {
        // get a random index from the available idols
        randIndex = getRandomNumber(0, availableCount - 1);
        selectedIdol = availableIdols[randIndex];
        
        // add the selected idol to our list
        state->rescuedInThisRun[i] = selectedIdol;
        
        // remove the selected idol from available list by shifting elements
        for (int j = randIndex; j < availableCount - 1; j++) {
            availableIdols[j] = availableIdols[j + 1];
        }
        availableCount--;
    }
}

/**
 * Check if any achievements have been earned.
 * Precondition: state is a valid pointer to an initialized GameState struct.
 * @param state Pointer to the GameState struct.
 */
void checkAchievements(GameState *state) {
    char *currentDate;
    int allIdolsRescued = 1;
    int achievementMapping[MAX_IDOLS] = {1, 2, 3, 4, 5, -1, 6, 7, 8}; // -1 for YOSHIKO since she's the player
    int secondTimeMapping[MAX_IDOLS] = {10, 11, 12, 13, 14, -1, 15, 16, 17}; // -1 for YOSHIKO since she's the player
    int achievementIndex;
    
    // get current date and time for newly earned achievements
    currentDate = getCurrentDateTime();
    
    // check progression achievements
    
    // "Yohane Descends!" - Cleared first dungeon
    if (!state->achievements[0].isEarned) {
        if (state->dungeonsCleared[0] || state->dungeonsCleared[1] || state->dungeonsCleared[2]) {
            state->achievements[0].isEarned = 1;
            strcpy(state->achievements[0].dateEarned, currentDate);
            printf("\n%s%sACHIEVEMENT UNLOCKED!%s\n", STYLE_BOLD, COLOR_BRIGHT_YELLOW, COLOR_RESET);
            printf("%s%s%s\n", COLOR_BRIGHT_CYAN, state->achievements[0].name, COLOR_RESET);
            waitForKeypress();
        }
    }
    
    // individual idol rescue achievements (first time)
    // map each idol to their correct achievement index
    
    for (int i = 0; i < MAX_IDOLS; i++) {
        achievementIndex = achievementMapping[i];
        if (achievementIndex != -1) { // skip YOSHIKO
            // rescue achievements for first time (indices 1-8)
            if (!state->achievements[achievementIndex].isEarned && state->aqours[i].isRescuedOnce) {
                state->achievements[achievementIndex].isEarned = 1;
                strcpy(state->achievements[achievementIndex].dateEarned, currentDate);
                printf("\n%s%sACHIEVEMENT UNLOCKED!%s\n", STYLE_BOLD, COLOR_BRIGHT_YELLOW, COLOR_RESET);
                printf("%s%s%s\n", COLOR_BRIGHT_CYAN, state->achievements[achievementIndex].name, COLOR_RESET);
                waitForKeypress();
            }
        }
    }

    // "In This Unstable World!" - Beat final boss first time (index 9)
    if (!state->achievements[9].isEarned) {
        if (state->finalBattleWon) {
            state->achievements[9].isEarned = 1;
            strcpy(state->achievements[9].dateEarned, currentDate);
            printf("\n%s%sACHIEVEMENT UNLOCKED!%s\n", STYLE_BOLD, COLOR_BRIGHT_YELLOW, COLOR_RESET);
            printf("%s%s%s\n", COLOR_BRIGHT_CYAN, state->achievements[9].name, COLOR_RESET);
            waitForKeypress();
        }
    }
    
    // individual idol rescue achievements (second time) (indices 10-17)
    
    for (int i = 0; i < MAX_IDOLS; i++) {
        achievementIndex = secondTimeMapping[i];
        if (achievementIndex != -1) { // skip YOSHIKO
            if (!state->achievements[achievementIndex].isEarned && state->aqours[i].rescueCount >= 2) {
                state->achievements[achievementIndex].isEarned = 1;
                strcpy(state->achievements[achievementIndex].dateEarned, currentDate);
                printf("\n%s%sACHIEVEMENT UNLOCKED!%s\n", STYLE_BOLD, COLOR_BRIGHT_YELLOW, COLOR_RESET);
                printf("%s%s%s\n", COLOR_BRIGHT_CYAN, state->achievements[achievementIndex].name, COLOR_RESET);
                waitForKeypress();
            }
        }
    }
    
    // "Deep Resonance!" - Beat final boss twice (index 18)
    if (!state->achievements[18].isEarned && state->finalBattleWonCount >= 2) {
        state->achievements[18].isEarned = 1;
        strcpy(state->achievements[18].dateEarned, currentDate);
        printf("\n%s%sACHIEVEMENT UNLOCKED!%s\n", STYLE_BOLD, COLOR_BRIGHT_YELLOW, COLOR_RESET);
        printf("%s%s%s\n", COLOR_BRIGHT_CYAN, state->achievements[18].name, COLOR_RESET);
        waitForKeypress();
    }
    
    // "No. 10!" - Clear 10 dungeons (index 19)
    if (!state->achievements[19].isEarned && state->totalDungeonsCleared >= 10) {
        state->achievements[19].isEarned = 1;
        strcpy(state->achievements[19].dateEarned, currentDate);
        printf("\n%s%sACHIEVEMENT UNLOCKED!%s\n", STYLE_BOLD, COLOR_BRIGHT_YELLOW, COLOR_RESET);
        printf("%s%s%s\n", COLOR_BRIGHT_CYAN, state->achievements[19].name, COLOR_RESET);
        waitForKeypress();
    }
    
    // "CYaRon!" - Rescued Chika, You, and Ruby (index 20)
    if (!state->achievements[20].isEarned && 
        state->aqours[IDOL_CHIKA].isRescuedOnce && 
        state->aqours[IDOL_YOU].isRescuedOnce && 
        state->aqours[IDOL_RUBY].isRescuedOnce) {
        state->achievements[20].isEarned = 1;
        strcpy(state->achievements[20].dateEarned, currentDate);
        printf("\n%s%sACHIEVEMENT UNLOCKED!%s\n", STYLE_BOLD, COLOR_BRIGHT_YELLOW, COLOR_RESET);
        printf("%s%s%s\n", COLOR_BRIGHT_CYAN, state->achievements[20].name, COLOR_RESET);
        waitForKeypress();
    }
    
    // "AZALEA!" - Rescued Hanamaru, Dia, and Kanan (index 21)
    if (!state->achievements[21].isEarned && 
        state->aqours[IDOL_HANAMARU].isRescuedOnce && 
        state->aqours[IDOL_DIA].isRescuedOnce && 
        state->aqours[IDOL_KANAN].isRescuedOnce) {
        state->achievements[21].isEarned = 1;
        strcpy(state->achievements[21].dateEarned, currentDate);
        printf("\n%s%sACHIEVEMENT UNLOCKED!%s\n", STYLE_BOLD, COLOR_BRIGHT_YELLOW, COLOR_RESET);
        printf("%s%s%s\n", COLOR_BRIGHT_CYAN, state->achievements[21].name, COLOR_RESET);
        waitForKeypress();
    }
    
    // "Guilty Kiss!" - Rescued Riko and Mari (index 22) 
    if (!state->achievements[22].isEarned && 
        state->aqours[IDOL_RIKO].isRescuedOnce && 
        state->aqours[IDOL_MARI].isRescuedOnce) {
        state->achievements[22].isEarned = 1;
        strcpy(state->achievements[22].dateEarned, currentDate);
        printf("\n%s%sACHIEVEMENT UNLOCKED!%s\n", STYLE_BOLD, COLOR_BRIGHT_YELLOW, COLOR_RESET);
        printf("%s%s%s\n", COLOR_BRIGHT_CYAN, state->achievements[22].name, COLOR_RESET);
        waitForKeypress();
    }
    
    // check if all idols have been rescued at least once for "Eikyuu Hours!" (index 23)
    // exclude Yoshiko since she's the player character and can't rescue herself
    allIdolsRescued = 1;
    for (int i = 0; i < MAX_IDOLS && allIdolsRescued; i++) {
        if (i != IDOL_YOSHIKO && !state->aqours[i].isRescuedOnce) {
            allIdolsRescued = 0;
        }
    }
    if (!state->achievements[23].isEarned && allIdolsRescued) {
        state->achievements[23].isEarned = 1;
        strcpy(state->achievements[23].dateEarned, currentDate);
        printf("\n%s%sACHIEVEMENT UNLOCKED!%s\n", STYLE_BOLD, COLOR_BRIGHT_YELLOW, COLOR_RESET);
        printf("%s%s%s\n", COLOR_BRIGHT_CYAN, state->achievements[23].name, COLOR_RESET);
        waitForKeypress();
    }
    
    // other achievements that need special tracking func
    // "Aozora Jumping Heart!" - Clear a dungeon without taking damage (index 24)
    // this is checked when a dungeon is completed (see runDungeon function)
    
    // "Mitaiken Horizon!" - Spend 5000G total (index 25)
    if (!state->achievements[25].isEarned && state->totalGoldSpent >= 5000) {
        state->achievements[25].isEarned = 1;
        strcpy(state->achievements[25].dateEarned, currentDate);
        printf("\n%s%sACHIEVEMENT UNLOCKED!%s\n", STYLE_BOLD, COLOR_BRIGHT_YELLOW, COLOR_RESET);
        printf("%s%s%s\n", COLOR_BRIGHT_CYAN, state->achievements[25].name, COLOR_RESET);
        waitForKeypress();
    }
    
    // "Ruby-chan! Hai? Nani ga suki?" - Get saved by choco-mint ice cream (index 26)
    if (!state->achievements[26].isEarned && state->chocoMintUsedToSave) {
        state->achievements[26].isEarned = 1;
        strcpy(state->achievements[26].dateEarned, currentDate);
        printf("\n%s%sACHIEVEMENT UNLOCKED!%s\n", STYLE_BOLD, COLOR_BRIGHT_YELLOW, COLOR_RESET);
        printf("%s%s%s\n", COLOR_BRIGHT_CYAN, state->achievements[26].name, COLOR_RESET);
        waitForKeypress();
    }
    
    // "Step! ZERO to ONE!" - Complete playthrough with 0G (index 27)
    if (!state->achievements[27].isEarned && state->finalBattleWon && state->yohane.gold == 0) {
        state->achievements[27].isEarned = 1;
        strcpy(state->achievements[27].dateEarned, currentDate);
        printf("\n%s%sACHIEVEMENT UNLOCKED!%s\n", STYLE_BOLD, COLOR_BRIGHT_YELLOW, COLOR_RESET);
        printf("%s%s%s\n", COLOR_BRIGHT_CYAN, state->achievements[27].name, COLOR_RESET);
        waitForKeypress();
    }
}