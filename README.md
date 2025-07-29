# Yohane The Parhelion! The Siren in the Mirror World!

<div align="center">

![Yohane the Parhelion](https://static.wikia.nocookie.net/love-live/images/8/8b/Yohane_of_the_Parhelion_Anime_Key_Visual.jpg/revision/latest/scale-to-width-down/1000?cb=20230212120145)

[![Language](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License](https://img.shields.io/badge/license-Educational_Use-green.svg)](LICENSE)

A C-based, console rogue-lite game based on the "Yohane the Parhelion -SUNSHINE in the MIRROR-" anime.  
This project was developed as a machine project for a programming course.

**Created by: 2ru17 (S14, PROG2-T3)**

</div>

---

## Game Intro

A mysterious phenomenon is causing idols everywhere to lose their voices. The school idol group Aqours is no exception. To solve this crisis, Yoshiko Tsushima awakens her fallen angel alter-ego, "Yohane," to venture into the Mirror World and save her friends.

Guided by her loyal companion Lailaps, Yohane discovers that a powerful Siren is stealing the voices of idols to perfect her own. Yohane must venture through treacherous dungeons, rescue the alternate versions of her friends, and ultimately confront the Siren to restore harmony to both worlds.

---

## Getting the Code

### Git Clone

To get started with the game, clone this repository to your local machine:

```bash
git clone https://github.com/2ru17/2ru-Yohane-Game.git
cd 2ru-Yohane-Game
```


---

## Game Startup and Menu System

When you first launch the game, you'll be presented with a startup menu with several options:

```
╔═══════════════════════════════════════════════════════╗
║              YOHANE THE PARHELION                     ║
║            The Siren in the Mirror World              ║
╚═══════════════════════════════════════════════════════╝

Welcome to the Mirror World, Little Demon!

┌─────────────── MAIN MENU ─────────────────────┐
│                                               │
│  [L] Load Game                                │
│  [N] New Game                                 │
│  [M] Manage Save Files                        │
│  [Q] Quit                                     │
│                                               │
└───────────────────────────────────────────────┘
```

### Menu Options:
- **[L] Load Game**: Load a previously saved game from one of 3 save slots
- **[N] New Game**: Start fresh with a new playthrough
- **[M] Manage Save Files**: View and delete existing save files across 3 slots
- **[Q] Quit**: Exit the game

### In-Game Menu
Once you start or load a game, you'll access the in-game menu with different options:
- **[C] Continue** (if a playthrough is active) or **[N] New Game** (if no active playthrough)
- **[V] View Achievements**: Browse through all 28 unlockable achievements
- **[Q] Quit**: Return to startup menu

---

## Game Compiling and Running

This is a standard C project and can be compiled with `gcc`.

### Prerequisites
- **C Compiler**: `gcc` with C99 standard support
- **Terminal/Command Prompt**: For running commands and playing the game

### Compilation

**Standard Compilation:**
```bash
gcc -Wall -std=c99 -Iinclude src/*.c -o YohaneGame
```

**Alternative (for Windows):**
```bash
gcc -Wall -std=c99 -Iinclude src/*.c -o YohaneGame.exe
```

### Running the Game

#### Windows
```bash
YohaneGame.exe
```

#### Linux/macOS
```bash
./YohaneGame
```

### Troubleshooting
- If you encounter "command not found" errors, ensure your compiler is properly installed
- For Windows users, you may need to add gcc to your PATH environment variable
- Make sure all game files (including .txt dungeon files) are in the correct directory

---

## Game Mechanics

This game is a rogue-lite, where each playthrough presents new challenges. Progress, such as rescued idols and achievements, carries over between runs.

### The Main Loop
1. **Start a Playthrough**: From the main menu, you can start a `New Game` or `Continue` an existing one.
2. **Rescue Mission**: At the start of a new run, 3 of the 8 Aqours members are randomly chosen to be rescued. Each idol is trapped in a unique dungeon with its own location name.
3. **Clear Dungeons**: You must clear all three dungeons in any order you choose. Once a dungeon is cleared, its corresponding idol is rescued, and the dungeon becomes unselectable (marked with `X`) for the current playthrough.
4. **Shop Access**: After rescuing Hanamaru, her shop becomes available with items that unlock as you rescue more members.
5. **Face the Siren**: After rescuing all three idols, the final dungeon unlocks, where the Siren awaits in a special dual-character battle.
6. **Victory & New Game+**: Defeating the Siren completes the playthrough. You can then start a new game with your rescued idols, gold, items, and achievements carried over, while upgrades and HP bonuses reset.

### Dungeon Exploration
The dungeons are grid-based environments loaded from `.txt` files. You control Yohane (`Y`) using the **WASD** keys.

**Dungeon Structure:**
- **1st Dungeon**: 1 floor
- **2nd Dungeon**: 2-3 floors  
- **3rd Dungeon**: 3-4 floors

**Dungeon Tiles:**
| Char | Tile Type | Description |
| :--: | :---------- | :------------------------------------------------------------------ |
| `Y` | Yohane | The player character. |
| `*` | Border | An impassable wall surrounding the dungeon. |
| `.` | Passable | An empty space Yohane and enemies can move through. |
| `v` | Wall | Can be dug through. Yohane stays in place for one turn to dig. |
| `x` | Spike | Can be dug, but deals 0.5 HP damage to Yohane. |
| `w` | Water | Impassable to Yohane (unless she has Air Shoes). |
| `h` | Heat | Deals 1 HP damage if Yohane ends her turn on it. |
| `T` | Treasure | Contains gold (10-100g) or a Noppo Bread. One per floor. |
| `E` | Exit | Takes Yohane to the next floor or clears the dungeon. |
| `b` | Bat | An enemy that moves and attacks. |

**Important Mechanics:**
- **Digging**: When digging walls (`v`) or spikes (`x`), Yohane stays in place for that turn
- **Heat Damage**: Standing still on heat tiles (`h`) causes 1 HP damage - keep moving!
- **Water Navigation**: Water tiles block movement unless you have Air Shoes from the shop
- **Treasure**: Each floor has exactly one treasure tile with random rewards

**Dungeon Names by Character:**
| Character | Dungeon Location |
|-----------|------------------|
| Chika | Yasudaya Ryokan |
| Riko | Numazu Deep Sea Aquarium |
| You | Izu-Mito Sea Paradise |
| Hanamaru | Shougetsu Confectionary |
| Ruby | Nagahama Castle Ruins |
| Dia | Numazugoyotei |
| Kanan | Uchiura Bay Pier |
| Mari | Awashima Marine Park |

### Combat
Combat is turn-based. Every action you take (moving, digging, using an item, switching items) counts as one turn.

- **Attacking**: To attack an adjacent enemy (like a bat `b`), simply move into its tile.
- **Turn System**: Everything Yohane does is considered a movement/turn, including:
  - Moving up, down, left, or right
  - Pressing an invalid input (Yohane won't move but turn still counts)
  - Digging walls or spikes
  - Using items
  - Switching between items

- **Enemies (Bats)**: The Siren's minions that drop gold when defeated. Their behavior becomes more aggressive as you progress:
  - **1st Dungeon**: Move every 2 player turns. Deal 0.5 HP damage. Drop 5g when defeated.
  - **2nd Dungeon**: Move every player turn. Deal 1.0 HP damage. Drop 10g when defeated.
  - **3rd Dungeon**: Move every player turn (including diagonally). Deal 1.5 HP damage. Drop 15g when defeated.

- **Bat AI**: When a bat is about to move and Yohane is on an adjacent tile, the bat will attack instead of moving randomly. Be careful not to accidentally move next to a bat when it's their turn to move!

### Inventory System
You can manage your items from the Dungeon Selection menu or during dungeon exploration.

- **View Inventory**: Press `I` on the Dungeon Selection screen to see all your items.
- **Cycle Items**: In a dungeon, use `[` (previous) and `]` (next) to switch the item in your hand.
- **Use Item**: Press `Spacebar` to use the currently equipped item.
- **Item Display**: When you have multiple of the same item, the quantity is shown (e.g., "Tears of a fallen angel (4)")

**Available Items:**
  - **Tears of a fallen angel**: Heals 0.5 HP.
  - **Noppo bread**: Heals 0.5 HP.
  - **Choco-mint ice cream**: A passive item. If equipped, it saves Yohane from a fatal blow once, restoring her to full health.

**Note**: Switching between items counts as a movement/turn, so enemies will move when you cycle through your inventory!

### Hanamaru's Shop
After you rescue Hanamaru, her shop becomes available from the Dungeon Selection menu. Here, you can spend your hard-earned gold on healing items and powerful permanent upgrades.

**Available Items:**
- **Tears of a fallen angel** (30g) - Heals Yohane 0.5 HP. Can only be purchased once per playthrough.
- **Noppo bread** (100g) - Heals Yohane 0.5 HP
- **Shovel Upgrade** (300g) - Allows Yohane to dig spike walls without damage (Requires rescuing Kanan)
- **Bat Tamer** (400g) - Turns damage from all bats to a constant 0.5 HP damage (Requires rescuing Riko)
- **Air Shoes** (500g) - Allows walking on water tiles and prevents damage from heat tiles (Requires rescuing You)
- **Stewshine** (1000g) - Increases Yohane's HP by 1 permanently (Requires rescuing Mari)
- **Mikan Mochi** (1000g) - Increases Yohane's HP by 1 permanently (Requires rescuing Chika)
- **Kurosawa Macha** (1000g) - Increases Yohane's HP by 1 permanently (Requires rescuing Dia)
- **Choco-mint ice cream** (2000g) - Saves Yohane from a fatal hit when equipped, then heals to full health (Requires rescuing Ruby)

### The Final Battle
The final encounter is a unique battle against the Siren that requires careful strategy and positioning.

**Battle Mechanics:**
- **Dual Control**: You control both Yohane (`Y`) and Lailaps (`L`) simultaneously using WASD.
- **Movement**: Both characters move together. If one is blocked by an obstacle, only the unblocked character moves.
- **The Switches**: You must position Yohane and Lailaps to stand on two switches (`0`) simultaneously. This breaks part of the Siren's barrier.
- **Three Phases**: You must successfully trigger switches **three times** to completely break the Siren's barrier.
- **Switch Positioning**: New switch pairs spawn in different locations after each successful trigger. Switches are never further than 2 rows and 5 columns apart.

**Enemy Spawning:**
- The Siren summons a bat every 8 moves during the switch phases
- Bat behavior depends on switch progress:
  - 0 switches: Bats act like 1st dungeon (move every 2 turns, 0.5 damage)
  - 1 switch: Bats act like 2nd dungeon (move every turn, 1.0 damage)  
  - 2 switches: Bats act like 3rd dungeon (move every turn + diagonally, 1.5 damage)

**Final Phase:**
- After breaking all barriers, the Siren moves toward Yohane and Lailaps
- The Siren can move diagonally and deals fatal damage in one hit
- **Victory Condition**: Land one attack on the Siren to win (defeating her drops 750g)
- **Defeat Conditions**: 
  - Yohane's HP reaches 0 (unless saved by Choco-mint ice cream)
  - Lailaps' HP reaches 0 (always fatal - Lailaps cannot attack or use items)

**Strategy Tips:**
- Use walls (`*`) to separate Yohane and Lailaps for switch positioning
- Keep track of bat movement patterns during switch phases
- Position carefully during the final phase - the Siren is extremely deadly!

## Controls Summary

| Key           | Action                           |
|---------------|----------------------------------|
| `W/A/S/D`     | Move Yohane (and Lailaps in final battle) |
| `I`           | Open inventory (in dungeon menu) |
| `H`           | Open Hanamaru's shop (when available) |
| `[` / `]`     | Cycle through items              |
| `Spacebar`    | Use selected item                |
| `X`           | Stay still (counts as a turn)    |
| `ESC`         | Open pause menu (in dungeon)     |
| `Enter/Return`| Confirm selection in menus       |

---

## Achievement System

There are **28 achievements** to unlock by completing various milestones in the game. You can view your progress from the in-game menu (accessible after starting or loading a game using the [V] View Achievements option). The achievement screen is paginated, allowing you to browse through all available and locked achievements.

### Complete Achievement List

**Progression Achievements:**
- **Yohane Descends!** - Cleared first dungeon
- **In This Unstable World!** - Beat the Final boss for the first time
- **Deep Resonance!** - Beat the Final boss twice
- **No. 10!** - Clear 10 dungeons

**First-Time Rescue Achievements:**
- **Mikan Power!** - Rescued Chika for the first time
- **Riko-chan BEAM!** - Rescued Riko for the first time
- **Yousoro!** - Rescued You for the first time
- **It's the future, zura!** - Rescued Hanamaru for the first time
- **Ganbaruby!** - Rescued Ruby for the first time
- **Buu-buu desu wa!** - Rescued Dia for the first time
- **Hug!!!** - Rescued Kanan for the first time
- **Shiny!** - Rescued Mari for the first time

**Second-Time Rescue Achievements:**
- **One more sunshine story!** - Rescued Chika twice
- **Pianoforte Monologue!** - Rescued Riko twice
- **Beginner's Sailing!** - Rescued You twice
- **Oyasuminasan!** - Rescued Hanamaru twice
- **Red Gem Wink!** - Rescued Ruby twice
- **White First Love!** - Rescued Dia twice
- **Sakana ka Nandaka!** - Rescued Kanan twice
- **New Winding Road!** - Rescued Mari twice

**Group Combination Achievements:**
- **CYaRon!** - Rescued Chika, You, and Ruby (not necessarily in one playthrough)
- **AZALEA!** - Rescued Hanamaru, Dia, and Kanan (not necessarily in one playthrough)
- **Guilty Kiss!** - Rescued Riko and Mari (not necessarily in one playthrough)
- **Eikyuu Hours!** - Rescued all Aqours members at least once

**Challenge Achievements:**
- **Aozora Jumping Heart!** - Clear a dungeon without taking any damage
- **Mitaiken Horizon!** - Spend a total of 5000g at Hanamaru's shop across multiple playthroughs
- **Ruby-chan! Hai? Nani ga suki?** - Get saved by Choco-mint ice cream from a fatal blow
- **Step! ZERO to ONE!** - Complete a playthrough with 0g at the end

**Achievement Features:**
- Achievements display unlock date and time when earned
- Multiple achievements can be unlocked simultaneously and will be displayed together
- Achievement progress is tracked across all playthroughs
- Pagination system allows browsing through all 28 achievements
- Detailed descriptions help guide players toward unlocking specific achievements

---

## New Game+ System

After successfully defeating the Siren, you can start a **New Game+** that carries over your progress while providing fresh challenges.

### What Carries Over:
- **Rescued Aqours members** from previous playthroughs
- **Total gold** accumulated
- **Achievement progress** across all runs
- **Consumable items**: Noppo bread and Tears of a fallen angel quantities

### What Resets:
- **Maximum HP** returns to 3 at the start of each playthrough
- **Purchased upgrades** (Air Shoes, Shovel Upgrade, etc.) must be rebought
- **Choco-mint ice cream** must be repurchased
- **Current dungeon progress** starts fresh

### Idol Selection Rules:
- 3 random Aqours members are selected to rescue each playthrough
- Previously rescued members can be selected again **only after** all 8 members have been rescued at least once
- This ensures variety while allowing completionist runs

---

## Game Over System

**Game Over Conditions:**
- Yohane's HP reaches 0
- Lailaps' HP reaches 0 during the final battle

**Causes of Damage:**
- **Bat attacks** (0.5, 1.0, or 1.5 HP depending on dungeon)
- **Heat tiles** (1 HP if you end your turn on them)
- **Spike walls** (0.5 HP when digging through them)
- **Siren attack** (fatal unless protected by Choco-mint ice cream)

The game displays what caused your defeat (e.g., "Killed by: Bat") to help you learn from mistakes.

---

## Project Structure

The game is built with a modular approach in C, following structured programming principles.

### Source Files
- **`main.c`**: Handles the main game loop and startup menu navigation.
- **`game_setup.c`**: Initializes static game data (idol names, achievements, etc.).
- **`game_logic.c`**: Implements core gameplay, including dungeon traversal, combat, and in-game menus.
- **`dungeon_handler.c`**: Manages dungeon loading, parsing, and tile interactions.
- **`file_io.c`**: Handles save/load operations and file management.
- **`save_manager.c`**: Manages save file operations and slot management.
- **`user_interface.c`**: Handles user interface elements and display functions.
- **`utils.c`**: Utility functions used throughout the project.

### Header Files
- **`game_structs.h`**: Contains all core data structures (`GameState`, `Player`, `Idol`, `Achievement`).
- **`utils.h`**: Function declarations for utility functions.

### Data Files
- **Dungeon Files (`assets/*.txt`)**: The game loads dungeon layouts from simple text files, allowing for easy level design.

### Data Structures
- **`GameState`**: Tracks overall game progress, including rescued idols and achievements
- **`Player`**: Manages player stats, inventory, and position
- **`Idol`**: Contains information about each Aqours member
- **`Achievement`**: Tracks unlock conditions and status for each achievement

### Game Flow
```
Main Menu → Dungeon Selection → Dungeon Exploration → Combat → 
Item Management → Idol Rescue → Shop → Final Battle → Victory/Game Over
```

### Programming Standards
- **C99 Standard**: Uses modern C features while maintaining compatibility
- **No Global Variables**: All data is passed through function parameters
- **Modular Design**: Each major game feature is implemented in separate functions
- **Standard Library Only**: Uses only standard C libraries for maximum portability

---

## © Disclaimer

This is a non-profit, fan-made project created for educational purposes.

*Yohane the Parhelion* and all related characters are properties of ©PROJECT YOHANE, ©2017 PROJECT Lovelive! Sunshine!!, and ©SUNRISE Bandai Namco Filmworks Inc. Please support the official releases of Love Live! Sunshine!! and Yohane the Parhelion.

---

## Acknowledgments

- Special thanks to Sir Art da GOAT for his guidance
- This project was created using only standard C libraries
- Inspiration drawn from classic rogueliTe games and the Yohane the Parhelion anime
