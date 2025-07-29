/**
 * Description: Utility functions for cross-platform game support
 * Programmed by: 2ru17 (S14, PROG2-T3)
 * Last modified: 2025-07-25
 * Version: 1.8
 * Acknowledgements: This code is based on the project specifications provided.
 */

#ifndef UTILS_H
#define UTILS_H

// platform detection - used for OS-specific functions
#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
    #define PLATFORM_MAC
#else
    #define PLATFORM_LINUX
#endif

// ANSI color codes for enhanced visuals
#define COLOR_RESET     "\033[0m"
#define COLOR_RED       "\033[31m"
#define COLOR_GREEN     "\033[32m"
#define COLOR_YELLOW    "\033[33m"
#define COLOR_BLUE      "\033[34m"
#define COLOR_MAGENTA   "\033[35m"
#define COLOR_CYAN      "\033[36m"
#define COLOR_WHITE     "\033[37m"
#define COLOR_BRIGHT_RED     "\033[91m"
#define COLOR_BRIGHT_GREEN   "\033[92m"
#define COLOR_BRIGHT_YELLOW  "\033[93m"
#define COLOR_BRIGHT_BLUE    "\033[94m"
#define COLOR_BRIGHT_MAGENTA "\033[95m"
#define COLOR_BRIGHT_CYAN    "\033[96m"
#define COLOR_BRIGHT_WHITE   "\033[97m"

// background colors for highlighting
#define BG_RED     "\033[41m"
#define BG_GREEN   "\033[42m"
#define BG_YELLOW  "\033[43m"
#define BG_BLUE    "\033[44m"

// text styling
#define STYLE_BOLD      "\033[1m"
#define STYLE_DIM       "\033[2m"
#define STYLE_UNDERLINE "\033[4m"

// function prototypes
void clearScreen(void);
void waitForKeypress(void);
char getCharNonBlocking(void);
char* getCurrentDateTime(void);
int getRandomNumber(int min, int max);
void printColoredChar(char c);
void setDamageCause(GameState *state, const char *cause);
// printColoredText removed - was unused

// save file management functions
int selectSaveSlot(int isLoad);
int manageSaveFiles(void);

// user interface functions
void displayStartupMenu(void);
void displayFarewellMessage(void);
void displayNewGameMessage(void);
void displayGameEndMessage(void);
void displayThankYouMessage(void);

#endif // UTILS_H