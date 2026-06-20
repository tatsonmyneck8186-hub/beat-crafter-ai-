# LAST BELL — Arcade Boxing Game

An original arcade boxing game for Unreal Engine 5.7 inspired by the feel of
classic coin-op fighters. All characters, names, and artwork are original.

## Quick Start

1. Open `LastBell.uproject` in **Unreal Engine 5.7**
2. Click **"Yes"** when prompted to compile the C++ project
3. Follow **SETUP_GUIDE.md** to create Blueprint assets and wire up the game
4. Press **Play** and fight!

## Controls

| Key | Action |
|-----|--------|
| A | Move Left |
| D | Move Right |
| S | Duck (hold) |
| Space | Dodge |
| Left Mouse | Jab |
| Right Mouse | Hook |
| E | Uppercut |

## Opponents

| Fighter | Type | Style |
|---------|------|-------|
| **Rex "Rookie" Ramone** | Rookie | Slow, forgiving, attacks often but predictably |
| **Sal "Slip" Corrales** | Counter Boxer | Patient, dodges your attacks, counters fast |
| **Bruno "The Boulder" Mack** | Heavyweight | Slow but hits like a truck, takes many hits |

## Project Structure

```
LastBell/
├── Config/              Engine, input, game settings
├── Content/             Blueprints, maps, assets (create in editor)
└── Source/LastBell/
    ├── AI/              AI controller + Behavior Tree tasks/services
    ├── Animation/       BoxerAnimInstance C++ base
    ├── Camera/          Camera shake classes
    ├── Characters/      BoxerCharacter (base), PlayerBoxer, AIBoxer
    ├── Components/      BoxerStatsComponent, CombatComponent
    ├── Core/            BoxingTypes (enums/structs), IBoxerInterface
    ├── Data/            FighterDataAsset, LastBellGameSettings
    ├── Environment/     BoxingRingActor, CrowdActor
    ├── Game/            GameMode, GameState, PlayerController, GameInstance
    └── UI/              LastBellHUD, BoxingUserWidget base
```

## Architecture

- **C++**: All gameplay — combat, stats, AI, game mode logic
- **Blueprints**: UI widgets, maps, character BP subclasses, data assets
- **Data Assets** (`UFighterDataAsset`): Configure each fighter without touching code
- **Components**: `BoxerStatsComponent` (health/stamina/KO), `CombatComponent` (attacks/hitboxes)
- **Interface** (`IBoxerInterface`): Decouples player and AI; both speak the same contract

## Tech Notes

- Enhanced Input System (UE5.1+) — create `IA_*` and `IMC_Default` assets per guide
- Behavior Trees — one BT asset per AI archetype, shared BBKeys namespace
- Hit-pause via `SetGlobalTimeDilation` — frame-perfect stop effect
- Camera shake via `ClientStartCameraShake` — two intensity tiers (light/heavy)
- Crowd uses `UInstancedStaticMeshComponent` — hundreds of spectators at low cost
