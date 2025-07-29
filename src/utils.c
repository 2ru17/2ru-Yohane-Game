/**
 * Description: Cross-platform utility functions for input, output, colors, and random numbers
 * Please check REFERENCES.md for the specific OS type functions that I have used since
 * it is not specifically discussed in class; Thanks :>
 * Programmed by: 2ru17 (S14, PROG2-T3)
 * Last modified: 2025-07-22
 * Version: 1.7
 * Acknowledgements: This code is based on the project specifications provided.
 */

#include "game_structs.h"
#include "utils.h"

#ifdef PLATFORM_WINDOWS
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

/**
 * Clear the console screen in a cross-platform way.
 * Precondition: None.
 */
void clearScreen(void) {
    #ifdef PLATFORM_WINDOWS
        system("cls");
    #else
        system("clear");
    #endif
}

/**
 * Get a character without waiting for Enter key.
 * Precondition: None.
 * @return The character pressed by the user.
 */
char getCharNonBlocking(void) {
    char ch;                    // user input character
    
    #ifdef PLATFORM_WINDOWS
        ch = _getch();         // windows immediate input
    #else
        struct termios old_tio, new_tio;  // terminal settings
        
        // get current terminal settings
        tcgetattr(STDIN_FILENO, &old_tio);
        
        // copy settings for modification
        new_tio = old_tio;
        
        // disable canonical mode and echo
        new_tio.c_lflag &= (~ICANON & ~ECHO);
        
        // apply settings immediately
        tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
        
        // read character
        ch = getchar();
        
        // restore old settings
        tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
    #endif
    
    return ch;
}

/**
 * Wait for the user to press any key.
 * Precondition: None.
 */
void waitForKeypress(void) {
    int c;
    
    printf("Press Enter to continue...");
    fflush(stdout);  // ensure prompt display
    
    // consume input buffer until newline or EOF
    while ((c = getchar()) != '\n' && c != EOF) {
        // continue consuming characters
    }
}

/**
 * Safely copy damage cause string to prevent buffer overflow.
 * Precondition: state is valid, cause is valid string
 * @param state Pointer to GameState
 * @param cause Damage cause string to copy
 */
void setDamageCause(GameState *state, const char *cause) {
    strncpy(state->lastDamageCause, cause, 49);
    state->lastDamageCause[49] = '\0';  // ensure null termination
}

/**
 * Get the current date and time as a formatted string.
 * Precondition: None.
 * @return Pointer to a static string containing the formatted date and time.
 */
char* getCurrentDateTime(void) {
    static char dateTimeStr[MAX_DATE_LENGTH]; // static to prevent time duplication
    time_t now;
    struct tm *timeInfo;
    
    // get current time
    time(&now);
    timeInfo = localtime(&now);
    
    // format as YYYY-MM-DD HH:MM:SS
    strftime(dateTimeStr, MAX_DATE_LENGTH, "%Y-%m-%d %H:%M:%S", timeInfo);
    
    return dateTimeStr;
}

/**
 * Generate a random number in a specified range.
 * Precondition: min and max are valid integers, min <= max.
 * @param min The minimum value (inclusive).
 * @param max The maximum value (inclusive).
 * @return A random integer between min and max.
 */
int getRandomNumber(int min, int max) {
    int temp;  // temporary variable for swapping
    
    // ensure range is valid
    if (min > max) {
        temp = min;
        min = max;
        max = temp;
    }
    
    // calculate and return random number in range
    return min + rand() % (max - min + 1);
}

/**
 * Print a single character with appropriate color based on its type.
 * Precondition: c is a valid character.
 * @param c The character to print with color.
 */
void printColoredChar(char c) {
    switch (c) {
        case TILE_WATER:  // water - Blue
            printf("%s%c%s", COLOR_BLUE, c, COLOR_RESET);
            break;
        case TILE_HEAT:  // heat - Red
            printf("%s%c%s", COLOR_RED, c, COLOR_RESET);
            break;
        case TILE_SPIKE:  // spikes - Bright Red
            printf("%s%c%s", COLOR_BRIGHT_RED, c, COLOR_RESET);
            break;
        case TILE_TREASURE:  // treasure - Yellow
            printf("%s%c%s", COLOR_YELLOW, c, COLOR_RESET);
            break;
        case TILE_EXIT:  // exit - Bright Green
            printf("%s%c%s", COLOR_BRIGHT_GREEN, c, COLOR_RESET);
            break;
        case TILE_PLAYER:  // yohane (Player) - Bright Cyan
            printf("%s%c%s", COLOR_BRIGHT_CYAN, c, COLOR_RESET);
            break;
        case TILE_LAILAPS:  // lailaps - Cyan
            printf("%s%c%s", COLOR_CYAN, c, COLOR_RESET);
            break;
        case TILE_BAT:  // bats (enemies) - Red
            printf("%s%c%s", COLOR_RED, c, COLOR_RESET);
            break;
        case TILE_SIREN:  // siren - Magenta
            printf("%s%c%s", COLOR_MAGENTA, c, COLOR_RESET);
            break;
        case TILE_SWITCH:  // switches - Bright Yellow
            printf("%s%c%s", COLOR_BRIGHT_YELLOW, c, COLOR_RESET);
            break;
        case TILE_WALL:  // walls - White
            printf("%s%c%s", COLOR_WHITE, c, COLOR_RESET);
            break;
        case TILE_BORDER:  // borders - Bright White (try and see the difference kekw)
            printf("%s%c%s", COLOR_BRIGHT_WHITE, c, COLOR_RESET);
            break;
        case TILE_GOLD_BAT:  // gold drops - Yellow
            printf("%s%c%s", COLOR_YELLOW, c, COLOR_RESET);
            break;
        case TILE_GOLD_SIREN:  // gold drops - bold yellow (SIREN)
            printf("%s%s%c%s", STYLE_BOLD, COLOR_YELLOW, c, COLOR_RESET);
            break;
        default:   // default tiles - no color
            printf("%c", c);
            break;
    }
}

// Note: printColoredText function removed as it was unused in the codebase
// If needed in the future, use: printf("%s%s%s", color, text, COLOR_RESET);