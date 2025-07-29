/**
 * Description: Manages saving and loading of game state to/from multiple save files
 * Programmed by: 2ru17 (S14, PROG2-T3)
 * Last modified: 2025-07-22
 * Version: 1.7
 * Acknowledgements: This code is based on the project specifications provided.
 */

#include "game_structs.h"

/**
 * Get filename for a specific save slot.
 * Precondition: slot must be between 1 and 3, or special value 17298 for CREATOR MODE
 * @param slot Save slot number (1-3) or 17298 for CREATOR MODE
 * @param filename Buffer to store the filename
 */
void getSaveFileName(int slot, char *filename) {
    // check if this is the special CREATOR MODE slot
    if (slot == 17298 || slot == 17) {
        // use special filename for CREATOR MODE saves
        strncpy(filename, "yohane_god.sav", 99);
        filename[99] = '\0';  // ensure null termination
    } else {
        // generate standard filename format for normal save slots (1-3)
        snprintf(filename, 100, "yohane%d.sav", slot);
    }
}

/**
 * Initialize CREATOR MODE with overpowered stats and all achievements.
 * Precondition: state pointer must be valid and not NULL
 * @param state Pointer to the GameState struct to initialize with CREATOR MODE
 */
void initializeGodMode(GameState *state) {
    int selectedIdols[] = {IDOL_CHIKA, IDOL_RIKO, IDOL_YOU};  // predefined idol selection
    char* currentDate;  // timestamp for achievement dates
    
    // initialize base game structure
    initializeStaticData(state);
    
    // set overpowered stats for testing
    state->yohane.maxHp = 999.0f;     // maximum health
    state->yohane.hp = 999.0f;        // current health  
    state->yohane.gold = 99999;       // massive gold for unlimited purchasing
    state->yohane.hasShovelUpgrade = 1;  // shovel upgrade unlocked for enhanced digging
    state->yohane.hasBatTamer = 1;       // bat tamer ability for easier bat encounters
    state->yohane.hasAirShoes = 1;       // air shoes for movement over dangerous tiles
    state->yohane.currentItemOnHand = ITEM_NONE;  // no item selected initially
    
    // set MASSIVE :) inventory for unlimited consumable usage
    state->tearsCount = 999;          // tears for healing - effectively unlimited supply
    state->noppoBreadCount = 999;     // noppo bread for healing - unlimited supply  
    state->chocoMintCount = 999;      // choco mint for healing - unlimited supply
    
    // set up Lailaps with overpowered stats to match Yohane
    state->lailaps.maxHp = 999.0f;    // maximum health for Lailaps companion
    state->lailaps.hp = 999.0f;       // current health matching max for full health
    
    // set up active playthrough state for CREATOR MODE testing
    state->isPlaythroughActive = 1;         // mark as active game session, 0 to unclear 3 dungeon
    state->finalBattleSwitchesHit = 0;      // reset final battle progress,  0 to start from beginning
    state->finalBattleWon = 0;              // reset final battle victory status
    state->turnCounter = 0;                 // reset turn counter for fresh start
    // state->moveCounter = 0; // cOMMENTED OUT - For Debugging only
                               // this counter was used during development for testing
    
    // unlock all idols for full roster access in CREATOR MODE
    for (int i = 0; i < MAX_IDOLS; i++) {
        state->aqours[i].isRescuedOnce = 1;    // mark each idol as rescued at least once
        state->aqours[i].rescueCount = 10;     // set high rescue count for achievement testing
    }
    
    // set up a playthrough with the first 3 idols (excluding Yoshiko)
    // provides immediate access to dungeons
    for (int i = 0; i < 3; i++) {
        state->rescuedInThisRun[i] = selectedIdols[i];    // mark these idols as rescued this run
        state->dungeonsCleared[i] = 1;  // mark dungeons as cleared for final battle access
                                       // allows immediate testing of final battle content
    }
    
    // unlock all achievements including the special CREATOR MODE achievement
    currentDate = getCurrentDateTime();             // get timestamp for achievement dates
                                                         // provides realistic achievement earning dates
    for (int i = 0; i < MAX_ACHIEVEMENTS; i++) {
        state->achievements[i].isEarned = 1;             // unlock each achievement
        strcpy(state->achievements[i].dateEarned, currentDate);  // set earning date to now
    }
    
    // creator mode activation message to confirm successful initialization
    printf("CREATOR MODE ACTIVATED!\n");
    printf("HP: 999, Gold: 999999, All achievements unlocked!\n");
    printf("All dungeons available for testing!\n");  
    printf("Note: Creator Mode progress will not be saved.\n");
}

/**
 * Check if a save file exists for the given slot.
 * Precondition: slot must be between 1 and 3, or 17298 for CREATOR MODE
 * @param slot Save slot number to check
 * @return 1 if file exists, 0 otherwise
 */
int saveFileExists(int slot) {
    char filename[50];        // buffer for constructed filename for save slot
    FILE *file;              // file pointer for attempting to open save file
    int exists = 0;          // flag indicating if file exists
    
    getSaveFileName(slot, filename);  // construct filename based on slot number
    file = fopen(filename, "rb");     // attempt to open file in binary read mode
    
    if (file != NULL) {      // file opened successfully, meaning it exists
        fclose(file);        // close file handle immediately after verification
        exists = 1;          // set flag indicating file exists
    }
    
    return exists;           // return existence flag
}

/**
 * Delete a save file for the given slot.
 * Precondition: slot must be between 1 and 3
 * @param slot Save slot number to delete
 * @return 1 if successful, 0 if failed
 */
int deleteSaveFile(int slot) {
    char filename[50];       // buffer to store constructed filename for deletion
    int result = 0;          // stores the operation result, 1 for success, 0 for failure
    
    if (slot >= 1 && slot <= 3) {  // validate slot number within acceptable range
        getSaveFileName(slot, filename);  // construct filename for specified slot
        
        if (remove(filename) == 0) {     // attempt file deletion using system remove function
            result = 1;                  // set success flag for successful deletion
        }
    }
    
    return result;                       // return final operation result
}

/**
 * Save the current game state to a specific save slot.
 * Precondition: state pointer must be valid and not NULL, slot between 1-3
 * @param state Pointer to the GameState struct to save
 * @param slot Save slot number (1-3)
 * @return 1 if successful, 0 if an error occurred
 */
int saveGameToSlot(GameState *state, int slot) {
    FILE *saveFile = NULL;           // file pointer for save file operations
    size_t itemsWritten = 0;         // tracks successfully written data structures
    int success = 0;                 // flag for overall save operation success
    char filename[50];               // buffer for constructed save filename
    
    if (slot >= 1 && slot <= 3) {  // validate slot number within valid range
        getSaveFileName(slot, filename);  // construct filename based on slot number
        
        // attempt to open save file for writing binary data
        saveFile = fopen(filename, "wb");    // open file in binary write mode
        
        if (saveFile != NULL) {           // file opened successfully
            // write entire GameState struct to file as binary data
            itemsWritten = fwrite(state, sizeof(GameState), 1, saveFile);
            
            // close file to ensure data is flushed to disk
            fclose(saveFile);             // releases file handle and commits changes
            
            // check if write operation was successful
            if (itemsWritten == 1) {      // exactly one GameState struct written
                success = 1;              // mark operation as successful
            } else {
                printf("Error: Failed to write save data to slot %d.\n", slot);
            }
        } else {
            printf("Error: Could not open save file for slot %d.\n", slot);
        }
    } else {
        printf("Error: Invalid save slot. Must be 1, 2, or 3.\n");
    }
    
    return success;                   // return success flag to caller
}

/**
 * Load a game state from a specific save slot.
 * Precondition: state pointer must be valid and not NULL, slot between 1-3 or 17
 * @param state Pointer to the GameState struct to load into
 * @param slot Save slot number (1-3) or 17 for CREATOR MODE
 * @return 1 if successful, 0 if an error occurred
 */
int loadGameFromSlot(GameState *state, int slot) {
    FILE *saveFile = NULL;       // file pointer for reading save file data
    size_t itemsRead = 0;        // tracks successfully read data structures
    int success = 0;             // flag for overall load operation success
    char filename[50];           // buffer to store constructed save filename
    
    // handle special CREATOR MODE slot with predetermined magic number
    if (slot == 17298) {         // special slot number for CREATOR MODE access
        initializeGodMode(state); // set up overpowered testing state
        success = 1;              // mark CREATOR MODE load as successful
    } else if (slot >= 1 && slot <= 3) {  // validate slot number within valid range
        getSaveFileName(slot, filename);  // construct filename based on slot number
        
        // attempt to open save file for reading binary data
        saveFile = fopen(filename, "rb");    // open file in binary read mode
        
        if (saveFile != NULL) {           // file opened successfully
            // read entire GameState struct from file as binary data
            itemsRead = fread(state, sizeof(GameState), 1, saveFile);
            
            // close file to release file handle
            fclose(saveFile);             // releases file handle after reading
            
            // check if read operation was successful
            if (itemsRead == 1) {         // exactly one GameState struct read
                success = 1;              // mark operation as successful
            } else {
                printf("Error: Failed to read save data from slot %d.\n", slot);
            }
        }
        // if saveFile is NULL, no save file exists for this slot - silent failure
    } else {
        printf("Error: Invalid save slot. Must be 1, 2, or 3.\n");
    }
    
    return success;                   // return success flag to caller
}

/**
 * Save the current game state to a binary file (legacy function for compatibility).
 * Precondition: state pointer must be valid and not NULL
 * @param state Pointer to the GameState struct to save
 * @return 1 if successful, 0 if an error occurred
 */
int saveGame(GameState *state) {
    return saveGameToSlot(state, 1);  // use slot 1 as default
}

/**
 * Load a game state from a binary file (legacy function for compatibility).
 * Precondition: state pointer must be valid and not NULL
 * @param state Pointer to the GameState struct to load into
 * @return 1 if successful, 0 if an error occurred
 */
int loadGame(GameState *state) {
    return loadGameFromSlot(state, 1);  // use slot 1 as default
}