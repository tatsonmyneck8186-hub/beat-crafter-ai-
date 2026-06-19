# LAST BELL — Blueprint Setup Guide

After compiling the C++ project follow every section below in order.
All paths are relative to the **Content Browser** root (`/Game/`).

---

## 1. Folder Structure (create in Content Browser)

```
Content/
└── LastBell/
    ├── Blueprints/
    │   ├── Characters/
    │   ├── AI/
    │   ├── Game/
    │   └── Environment/
    ├── Data/
    │   ├── Fighters/
    │   └── Settings/
    ├── Input/
    ├── Maps/
    ├── UI/
    │   ├── Widgets/
    │   └── HUD/
    ├── Materials/
    └── Audio/
```

---

## 2. Input Actions (Enhanced Input)

In `Content/LastBell/Input/` create these assets (**Add → Input → Input Action**):

| Asset Name | Value Type | Description |
|------------|-----------|-------------|
| `IA_MoveHorizontal` | Axis1D (float) | A/D movement |
| `IA_Jab` | Digital (bool) | Left Mouse |
| `IA_Hook` | Digital (bool) | Right Mouse |
| `IA_Uppercut` | Digital (bool) | E key |
| `IA_Duck` | Digital (bool) | S key (hold) |
| `IA_Dodge` | Digital (bool) | Spacebar |

Then create **`IMC_Default`** (Add → Input → Input Mapping Context):

| Action | Key | Modifiers |
|--------|-----|----------|
| `IA_MoveHorizontal` | D | — |
| `IA_MoveHorizontal` | A | Negate |
| `IA_Jab` | Left Mouse Button | — |
| `IA_Hook` | Right Mouse Button | — |
| `IA_Uppercut` | E | — |
| `IA_Duck` | S | — |
| `IA_Dodge` | Spacebar | — |

---

## 3. Data Assets — Fighters

In `Content/LastBell/Data/Fighters/` create three assets
(**Add → Miscellaneous → Data Asset → FighterDataAsset**):

### DA_PlayerFighter
- Fighter Name: `"The Challenger"`
- Fighter Type: `Player`
- Stats: MaxHealth 100, MaxStamina 100, StaminaRegenRate 18, MoveSpeed 390
- Jab: Damage 8, Stamina 8, KO Gain 4
- Hook: Damage 14, Stamina 14, KO Gain 8
- Uppercut: Damage 20, Stamina 20, KO Gain 14
- Primary Color: Blue, Secondary Color: White

### DA_Rookie (Rex "Rookie" Ramone)
- Fighter Name: `"Rex 'Rookie' Ramone"`
- Fighter Type: `Rookie`
- Stats: MaxHealth 90, MoveSpeed 300, DamageMultiplier 0.8, DefenseMultiplier 0.9
- AI Personality: AttackInterval 2.8, AttackIntervalVariance 0.8,
  DodgeProbability 0.1, DodgeReactionTime 0.6, AggressionLevel 0.4
- Primary Color: Yellow, Secondary Color: Black

### DA_CounterBoxer (Sal "Slip" Corrales)
- Fighter Name: `"Sal 'Slip' Corrales"`
- Fighter Type: `CounterBoxer`
- Stats: MaxHealth 85, MoveSpeed 420, DamageMultiplier 1.1, DefenseMultiplier 1.1
- AI Personality: AttackInterval 3.2, DodgeProbability 0.75,
  DodgeReactionTime 0.18, bIsCounterFighter true, AggressionLevel 0.3,
  CounterWindowDuration 0.5
- Primary Color: Green, Secondary Color: White

### DA_Heavyweight (Bruno "The Boulder" Mack)
- Fighter Name: `"Bruno 'The Boulder' Mack"`
- Fighter Type: `Heavyweight`
- Stats: MaxHealth 150, MoveSpeed 240, DamageMultiplier 1.8, DefenseMultiplier 1.4
- AI Personality: AttackInterval 3.8, DodgeProbability 0.05,
  bIsHeavyweight true, AggressionLevel 0.7, PreferredRange 120
- Primary Color: Red, Secondary Color: Black

---

## 4. Game Settings

In `Content/LastBell/Data/Settings/` create
(**Add → Miscellaneous → Data Asset → LastBellGameSettings**):

`DA_GameSettings` — leave defaults (3 rounds, 120s, etc.)
Assign sounds and particle effects once audio assets are imported.

---

## 5. Blueprint Characters

### BP_PlayerBoxer
- Parent class: `PlayerBoxer`
- Location: `Content/LastBell/Blueprints/Characters/`
- Assign in Class Defaults:
  - **DefaultMappingContext** → `IMC_Default`
  - **IA_MoveHorizontal/Jab/Hook/Uppercut/Duck/Dodge** → respective IA_ assets
  - **FighterData** → `DA_PlayerFighter`
  - Mesh: assign a humanoid skeletal mesh (use Mannequin or custom boxer mesh)
  - Mesh relative location: `(0, 0, -90)`, rotation: `(0, -90, 0)`

### BP_AIBoxer
- Parent class: `AIBoxer`
- Location: `Content/LastBell/Blueprints/Characters/`
- Set mesh same as player but mirrored rotation `(0, 90, 0)`
- Leave FighterData blank (set at runtime by GameMode)

---

## 6. Behavior Trees

Create three Behavior Trees in `Content/LastBell/AI/`:

### BT_Rookie
Blackboard: Create `BB_Boxer` with keys:
- `PlayerActor` (Object/Actor)
- `SelfActor` (Object/Actor)
- `DistanceToPlayer` (Float)
- `bCanAttack` (Bool)
- `bPlayerAttacking` (Bool)
- `HealthPercent` (Float)
- `bShouldRetreat` (Bool)

Tree structure:
```
Root
└─ Selector
   ├─ [Service: UpdateCombatState]
   ├─ Sequence (bShouldRetreat==true)
   │  └─ BTTask_MoveTowardPlayer (bMoveAway=true)
   ├─ Sequence (bCanAttack AND DistToPlayer > 200)
   │  └─ BTTask_MoveTowardPlayer
   └─ Sequence (bCanAttack AND DistToPlayer <= 200)
      ├─ Wait (2.8 ± 0.8 sec)
      └─ BTTask_ExecuteAttack (bPickRandomMove=true, weights: 0.6/0.3/0.1)
```

### BT_CounterBoxer
```
Root
└─ Selector
   ├─ [Service: UpdateCombatState]
   ├─ Sequence (bPlayerAttacking==true AND bCanAttack)
   │  ├─ BTTask_ExecuteDodge (DodgeProbability=0.75)
   │  ├─ Wait (0.15 sec)
   │  └─ BTTask_ExecuteAttack (Hook or Uppercut, weights: 0.1/0.5/0.4)
   ├─ Sequence (bCanAttack AND DistToPlayer > 180)
   │  └─ BTTask_MoveTowardPlayer
   └─ Sequence (bCanAttack)
      ├─ Wait (3.2 sec)
      └─ BTTask_ExecuteAttack (bPickRandomMove=true)
```

### BT_Heavyweight
```
Root
└─ Selector
   ├─ [Service: UpdateCombatState]
   ├─ Sequence (bCanAttack AND DistToPlayer > 130)
   │  └─ BTTask_MoveTowardPlayer
   └─ Sequence (bCanAttack AND DistToPlayer <= 130)
      ├─ Wait (3.8 sec)
      └─ BTTask_ExecuteAttack (Uppercut or Hook, weights: 0.2/0.4/0.4)
```

Assign BT in `BP_AIBoxer` → **BehaviorTree** property per opponent.
Or create three separate AI BP subclasses (recommended).

---

## 7. AI Controller

### BP_BoxingAIController
- Parent: `BoxingAIController`
- Does not need Behavior Tree assigned — set per AIBoxer subclass

In `BP_AIBoxer` → Pawn settings → **AI Controller Class** = `BP_BoxingAIController`

---

## 8. Game Mode

### BP_LastBellGameMode
- Parent: `LastBellGameMode`
- Assign: **GameSettings** → `DA_GameSettings`
- Default Pawn Class: `BP_PlayerBoxer`
- Player Controller: `BP_LastBellPlayerController`
- Game State: `LastBellGameState` (no BP needed)

In `Config/DefaultGame.ini` update:
```
GlobalDefaultGameMode=/Game/LastBell/Blueprints/Game/BP_LastBellGameMode.BP_LastBellGameMode_C
```

---

## 9. HUD Blueprint

### BP_LastBellHUD
- Parent: `LastBellHUD`
- Assign widget class properties to the WB_ widgets you create below

### BP_LastBellPlayerController
- Parent: `LastBellPlayerController`
- Override `OnMatchStateChanged` event to show/hide UI as needed

---

## 10. UI Widgets (UMG)

Create all widgets in `Content/LastBell/UI/Widgets/`.
Parent class for all: **`BoxingUserWidget`**

### WB_MainMenu
- Background image (dark arena)
- Title text: `"LAST BELL"`
- Buttons: **FIGHT** → calls `GoToCharacterSelect()`, **QUIT** → `QuitGame()`

### WB_CharacterSelect
- Grid of 3 opponent portraits (assign `DA_Rookie`, `DA_CounterBoxer`, `DA_Heavyweight`)
- On select: `GameInstance.SetSelectedOpponent(data)` then `StartFight()`
- Back button: `GoToMainMenu()`

### WB_HUD
Layout (design freely, reference positions):
- **Top-left**: Player health bar (bind to `PlayerHealthPercent`), stamina bar, KO meter
- **Top-right**: Opponent health bar, stamina bar, opponent name text
- **Top-center**: Round number, round timer (MM:SS format)
- Assign this class to `BP_LastBellHUD → HUDWidgetClass`

In the `Event Construct` → bind to `LastBellGameMode` delegates:
- `OnTimerTick` → update timer text
- `OnRoundChanged` → update round number
- Tick-bind to `StatsComponent` changed events on both fighters

### WB_KnockdownCount
- Large centered number (1–10)
- Text: `"DOWN!"`
- Animate count number scaling up each beat
- Assign to `BP_LastBellHUD → KnockdownCountWidgetClass`

### WB_WinScreen
- Background: celebratory
- Text: `"WINNER!"`
- Show player name, round wins
- Button: **PLAY AGAIN** → `GoToCharacterSelect()`, **MENU** → `GoToMainMenu()`

### WB_LoseScreen
- Background: darker tone
- Text: `"KNOCKOUT!"`
- Button: **TRY AGAIN** → `GoToCharacterSelect()`, **MENU** → `GoToMainMenu()`

---

## 11. Maps

Create these maps in `Content/LastBell/Maps/`:

### L_MainMenu
- Game Mode Override: `BP_LastBellGameMode`
- Place a `BP_LastBellPlayerController`-spawning pawn with just a camera
- On `BeginPlay` (Level Blueprint): create and add `WB_MainMenu` to viewport

### L_CharacterSelect
- Similar — show `WB_CharacterSelect`
- Display 3 fighter models in a rotator array

### L_FightArena
- Game Mode: `BP_LastBellGameMode`
- Place `BP_PlayerBoxer` at `(-120, 0, 0)` facing +X
- Place `BP_AIBoxer_Rookie/Counter/Heavyweight` at `(120, 0, 0)` facing -X
- Place `BP_BoxingRing` at origin
- Place `BP_Crowd` at origin (ArenaRadius=1400)
- Directional light for fill, 4 spotlights already on the Ring actor
- In **Level Blueprint BeginPlay**:
  ```
  Get GameMode (cast to LastBellGameMode)
  → StartMatch(GameInstance.GetSelectedOpponent())
  ```
- Bind `OnMatchStateChanged`:
  - `PreMatch` → Show HUD, disable player input briefly
  - `RoundActive` → Ring bell sound
  - `KnockDown` → Show KO counter widget
  - `BetweenRounds` → Show round result briefly
  - `MatchWin/MatchLose` → Open win/lose level

### L_WinScreen
- Show `WB_WinScreen` on BeginPlay

### L_LoseScreen
- Show `WB_LoseScreen` on BeginPlay

---

## 12. Environment Blueprints

### BP_BoxingRing
- Parent: `BoxingRingActor`
- Assign a cube `SM_Cube` (engine content) to each mesh component
- Set material instances with correct colors via PrimaryColor/SecondaryColor params
- The C++ `BuildRing()` / `BuildLighting()` runs on BeginPlay automatically

### BP_Crowd
- Parent: `CrowdActor`
- Assign a simple low-poly human mesh to `CrowdMeshes`
- `SpawnCrowd()` auto-runs on BeginPlay
- Call `AnimateCheering()` from GameMode events for crowd reaction

---

## 13. Animation (Anim BP)

### ABP_Boxer
- Parent: `BoxerAnimInstance`
- Reads: `BoxerState`, `bIsAttacking`, `bIsDucking`, `bIsDodging`,
  `CurrentAttack`, `MoveSpeed`, `HorizontalMovement`

State machine (**LocomotionSM**):
```
Idle ──► Move (MoveSpeed > 10)
Idle ──► Duck (bIsDucking)
Any ──► Attack_Jab (bIsAttacking AND CurrentAttack==Jab)
Any ──► Attack_Hook (bIsAttacking AND CurrentAttack==Hook)
Any ──► Attack_Uppercut (bIsAttacking AND CurrentAttack==Uppercut)
Any ──► HitStun (BoxerState==HitStun)
Any ──► KnockedDown (BoxerState==KnockedDown)
Any ──► KO (BoxerState==KO)
```

Use any humanoid animation set (Mixamo, Paragon assets, or custom).
All animation slots driven by state — no manual Blueprint wiring needed.

---

## 14. Audio

Import audio into `Content/LastBell/Audio/` then assign in `DA_GameSettings`:

| Slot | Description |
|------|-------------|
| `BellSound` | Classic boxing bell ding |
| `CrowdCheerSound` | Audience cheering loop |
| `CrowdBoooSound` | Audience booing |
| `KOAnnounceSound` | "KO!" announcer shout |
| `BackgroundMusic` | Arena ambient music loop |

Per-fighter punch sounds set in each `DA_Fighter*` asset:
`JabSound`, `HookSound`, `UppercutSound`, `HitSound`, `KOSound`

Play sounds in the Boxer Blueprint from `OnAttackStarted` and `OnHitReceived` events.

---

## 15. Placeholder Assets Checklist

If you don't have custom assets yet, use these Engine defaults:

| Need | Engine Asset |
|------|--------------|
| Fighter mesh | `/Engine/EngineMeshes/SM_MatPreviewMesh_02` or Mannequin |
| Boxing gloves | Add as separate StaticMesh actors attached to hand sockets |
| Ring floor | `/Engine/BasicShapes/Plane` |
| Ropes / posts | `/Engine/BasicShapes/Cube` |
| Hit particle | `/Engine/Tutorial/SubEditors/TutorialAssets/TutorialParticle` |
| Camera shake | Subclass `UPerlinNoiseCameraShakePattern` in `BoxingCameraShake.h` |
| Crowd mesh | Any simple humanoid shape |

---

## 16. Final Checklist Before First Play

- [ ] Project compiles with 0 errors
- [ ] `IMC_Default` assigned in `BP_PlayerBoxer`
- [ ] At least one opponent data asset exists
- [ ] `L_MainMenu` is the default map in Project Settings
- [ ] `BP_LastBellGameMode` set as Global Default Game Mode
- [ ] `LastBellGameInstance` set as Game Instance class
- [ ] `BP_BoxingRing` placed in `L_FightArena`
- [ ] `BP_PlayerBoxer` and `BP_AIBoxer` placed in `L_FightArena`
- [ ] Level Blueprint calls `StartMatch()` on BeginPlay
- [ ] HUD widget assigned to `BP_LastBellHUD`

---

## Expanding Later

- **New opponents**: Create a new `DA_FighterXXX` and a BT variant — zero code changes needed
- **New moves**: Add to `EBoxingMove`, add `FAttackData` entry, bind new input action
- **Online multiplayer**: Replace `ABoxingAIController` with a second `ALastBellPlayerController`
- **Career mode**: Extend `ULastBellGameInstance` with win/loss tracking across sessions
- **Cosmetics**: Skin slots via additional material parameters in `FighterDataAsset`
