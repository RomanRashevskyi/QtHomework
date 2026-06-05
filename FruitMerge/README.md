# FruitMerge - Individual Project

## 1. Project Description
**FruitMerge** is a physics-based puzzle game inspired by the popular "Suika Game" (Watermelon Game). Players drop various fruits into a container, where identical fruits merge into a single larger fruit of the next tier. The objective is to achieve the highest possible score by merging fruits strategically while ensuring the container doesn't overflow.

## 2. MVP Functions
- **Physics Engine:** Simulated gravity, collisions, and friction for realistic fruit movement.
- **Merge Mechanic:** 11 tiers of fruits that evolve upon contact with their identical counterpart.
- **Score System:** Points awarded for each successful merge, with higher tiers giving more points.
- **Game Over Detection:** A "danger line" mechanic that triggers a game over if fruits stay above it for too long (1.5 seconds).
- **High Scores:** Persistent local storage for player names and their best scores using JSON.
- **Interactive UI:** Smooth transitions between the main menu, high scores, and the game scene.

## 3. Technologies Used
- **Language:** C++17
- **Framework:** Qt 6 (Widgets & Graphics View Framework)
- **Build System:** CMake 3.19+
- **Version Control:** Git

## 4. How to Build (Release Mode)
To build the application from source in Release mode, ensure you have Qt 6 and CMake installed.

```bash
mkdir build-release
cd build-release
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/path/to/your/qt6 ..
make
```

## 5. How to Run
### From Build Directory:
Run the executable generated in the `build-release` folder:
```bash
./FruitMerge
```

### Portable Release:
1. Navigate to the `FruitMerge-release` folder.
2. Run the `FruitMerge` executable.
*(Note: Ensure all required Qt libraries are available in your system path or included in the folder for a standalone experience.)*

## 6. Demo Scenario
1. **Launch:** Open the application. You will see the main menu with "Start Game", "High Scores", and "Quit".
2. **Start:** Click "Start Game". A blank container appears with a fruit held at the top following your mouse.
3. **Drop:** Click anywhere in the container to drop the fruit.
4. **Merge:** Drop two identical fruits (e.g., two small berries) on top of each other. Observe them merging into a larger fruit and your score increasing.
5. **Score Recording:** Intentionally fill the container until fruits cross the red danger line. Wait for 1.5 seconds.
6. **Save:** When the "Game Over" prompt appears, enter your name and click OK.
7. **Verify:** Go to "High Scores" from the main menu to see your recorded entry.

## 7. Known Limitations
- **Local Only:** Scores are stored locally on the machine and are not synchronized online.
- **Screen Size:** The UI is optimized for a fixed desktop window size (600x800).
- **Physics Edge Cases:** Very high-speed collisions may occasionally result in slight overlapping before resolution.
- **No Sound:** This MVP version focuses on core mechanics and does not yet include audio effects.

## 8. Release Version
**v1.0-mvp** - Final version for Practical Work №29.
