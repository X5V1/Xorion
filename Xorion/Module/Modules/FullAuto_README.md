# FullAuto Module

## Overview
FullAuto is a comprehensive automation module that controls multiple hacks and provides AI-driven gameplay with configurable perfection levels.

## Features

### Section 1: Perfection Mode
Controls whether the automation behaves perfectly or with human-like imperfections.

**Settings:**
- `On` (true): All modules work perfectly like a robot
- `Off` (false): Introduces human-like imperfections:
  - Scaffold: 1/18 chance to fail block placement when bridging >12 blocks
  - Aimbot: 1/6 chance for crosshair to be slightly off
  - Fucker: Only destroys blocks 70% of the time (30% ignored)

**Integration with Other Modules:**
To enable perfection mode imperfections, the following modules need to be modified to check `FullAuto::isPerfectionModeEnabled()`:

1. **Scaffold Module**: Check `shouldPlaceFail()` before placing blocks
2. **Aimbot Module**: Check `shouldAimbotMiss()` and add slight offset to aim
3. **Fucker Module**: Check `shouldFuckerIgnore()` and skip block destruction

Example integration in Scaffold:
```cpp
if (auto fullAuto = moduleMgr->getModule<FullAuto>()) {
    if (fullAuto->isEnabled() && !fullAuto->isPerfectionModeEnabled()) {
        if (fullAuto->shouldPlaceFail()) {
            return; // Skip block placement
        }
    }
}
```

### Section 2: Hacks
Controls which hacks are enabled based on risk level.

**Settings (0-2):**
- `0 - Off`: Only enables Fullbright and Autosprint (minimal detection risk)
- `1 - Low Risk`: Adds limited use of risky hacks:
  - Fucker: Used only 2/10 times
  - Autoclicker: 2.5 second bursts only
  - Aimbot: Only when opponent is sidestepping
  - AirJump: Only to prevent fall damage or void death
  - NoKnockback (Velocity): Only when health > 8/20 hearts
- `2 - On`: Full access to all hacks:
  - Continuous autoclicker and aimbot during combat
  - Jetpack for movement
  - Scaffold for bridging
  - Fucker works continuously
  - Movement enhancement: Spider, Speed, Bhop, Step, HighJump
  - All modules enabled when convenient

### Section 3: Mode
Controls the automation's primary objective.

**Settings (0-1):**
- `0 - Kill`: Finds and attacks the nearest player
  - Enables appropriate combat modules based on hack level
  - Uses movement hacks to reach target
  - Manages Fucker to clear obstacles
  
- `1 - Run`: Evades threats
  - Identifies top 3 players with best armor within 50 block range
  - Calculates escape direction away from threat center
  - Enables Scaffold for bridging during escape
  - Uses Jetpack in full hack mode

## Usage

1. Enable the FullAuto module
2. Configure Perfection Mode (default: Off)
3. Set Hacks level (default: Off - 0)
4. Choose Mode (default: Kill - 0)

The module will automatically manage other modules based on your configuration.

## Technical Notes

- Runs at 20 ticks per second (standard Minecraft tick rate)
- Autoclicker bursts in low-risk mode: 50 ticks (2.5 seconds)
- Player detection range: 50 blocks (for Run mode)
- Health threshold for NoKnockback: 8 hearts (40% of max health)
- Fall damage threshold: Y < 10 or falling speed > 0.5 blocks/tick

## Category
COMBAT - Appears in the combat section of the module list
