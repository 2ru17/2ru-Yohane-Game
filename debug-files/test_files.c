#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char filename[100];
    FILE *file;
    FILE *outputFile;
    const char *dungeonNames[] = {
        "yasudaya_ryokan",
        "numazu_deep_sea_aquarium", 
        "izu_mito_sea_paradise",
        "shougetsu_confectionary",
        "nagahama_castle_ruins",
        "numazugoyotei",
        "uchiura_bay_pier",
        "awashima_marine_park"
    };
    int numDungeons = sizeof(dungeonNames) / sizeof(dungeonNames[0]);
    int maxFloors = 4;
    int dungeon, floor;
    
    outputFile = fopen("dungeon_file_report.txt", "w");
    if (!outputFile) {
        printf("\n");
        return 1;
    }
    
    // Write to both console and file
    printf("=== DUNGEON FILE VERIFICATION ===\n\n");
    fprintf(outputFile, "=== DUNGEON FILE VERIFICATION ===\n\n");
    
    // Test all dungeons
    for (dungeon = 0; dungeon < numDungeons; dungeon++) {
        printf("Testing %s:\n", dungeonNames[dungeon]);
        fprintf(outputFile, "Testing %s:\n", dungeonNames[dungeon]);
        
        for (floor = 1; floor <= maxFloors; floor++) {
            sprintf(filename, "assets/dungeon-env/dungeons_%s_floor%d.txt", 
                    dungeonNames[dungeon], floor);
            
            file = fopen(filename, "r");
            if (file) {
                printf("  [OK] Floor %d: %s\n", floor, filename);
                fprintf(outputFile, "  [OK] Floor %d: %s\n", floor, filename);
                fclose(file);
            } else {
                printf("  [FAIL] Floor %d: %s (NOT FOUND)\n", floor, filename);
                fprintf(outputFile, "  [FAIL] Floor %d: %s (NOT FOUND)\n", floor, filename);
            }
        }
        printf("\n");
        fprintf(outputFile, "\n");
    }
    
    // Also test the final battle filess
    printf("TEST:\n");
    fprintf(outputFile, "TEST:\n");
    strcpy(filename, "assets/dungeon-env/dungeons_final_battle.txt");
    file = fopen(filename, "r");
    if (file) {
        printf("  [OK] Final Battle: %s\n", filename);
        fprintf(outputFile, "  [OK] Final Battle: %s\n", filename);
        fclose(file);
    } else {
        printf("  [FAIL] Final Battle: %s (NOT FOUND)\n", filename);
        fprintf(outputFile, "  [FAIL] Final Battle: %s (NOT FOUND)\n", filename);
    }
    
    fclose(outputFile);
    
    return 0;
}
