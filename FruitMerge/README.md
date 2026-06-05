# FruitMerge - Individual Project

## 1. Project Theme
The application allows the user to play a "Fruit Merge" puzzle game where the goal is to combine identical fruits to create larger ones and achieve the highest score without crossing the danger line.

## 2. Problem Statement
**Target User:** Casual gamers looking for a relaxing yet strategic physics-based puzzle.
**Problem:** Players often seek simple, "pick-up-and-play" games that provide satisfying visual feedback and a sense of progression through a clear evolution mechanic.
**Goal:** Provide a functional desktop application with realistic physics, score tracking, and an intuitive interface.
**Constraints:** Desktop (Qt 6), offline, local score storage, C++17.

## 3. Entities and Data
- **FruitItem:**
  - `Tier` (enum/int) - mandatory
  - `Position` (QPointF) - mandatory
  - `Velocity` (QPointF) - mandatory
  - `ID` (unique identifier)
- **ScoreEntry:**
  - `Name` (QString) - mandatory
  - `Score` (int) - mandatory

## 4. List of Screens
- **Main Menu:** Options to start the game, view high scores, or quit.
- **Game Scene:** The main interaction area where fruits are dropped and merged. Displays current score and next fruit preview.
- **High Scores View:** A list showing the top saved scores.

## 5. User Scenarios
- **Start & Play:** Open app -> Click "Start" -> Drop fruits -> View score increase.
- **Game Over & Save:** Play until fruit crosses red line -> Game Over screen appears -> Score is automatically saved to high scores.
- **View Records:** Open app -> Click "High Scores" -> Review previous achievements.

## 6. Architecture Layers
- **UI (View Layer):** `MainWindow`, `GameScene`, `FruitItem` (visuals). Handles rendering and user input.
- **Logic (Domain Layer):** Collision resolution, merging rules, physics calculations (gravity, friction).
- **Data (Data Layer):** `ScoreEntry` structure, file I/O for saving/loading high scores in a text/binary format.

## 7. MVP Definition
- **Core "End-to-End" Scenario:** Drop fruit -> Collision -> Merge -> Score Update -> Game Over detection.
- **Must-have functions:**
  - Physics-based fruit dropping.
  - Merging logic for 11 tiers of fruit.
  - Score accumulation.
  - Game Over condition (Danger line).
- **Post-MVP Functions:**
  - Sound effects and music.
  - Particle effects on merge.
  - Online leaderboard integration.
  - Advanced physics (e.g., fruit "shaking" or squashing).

## 8. MVP Readiness Criteria
- Game Scene is functional with fruit dropping.
- Merging works correctly (Tier N + Tier N = Tier N+1).
- Game Over triggers when fruits stay above the danger line for too long.
- High scores persist after application restart.
