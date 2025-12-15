# How to Find Signatures with x64dbg

Simple guide for finding function addresses and memory offsets in Minecraft 1.21.130.

## Setup
1. Download x64dbg from https://x64dbg.com
2. Launch Minecraft 1.21.130
3. Inject Xorion.dll
4. Open x64dbg → File → Attach → Minecraft.Windows.exe

---

## Finding Functions

### Method 1: Search for Strings

**Steps:**
1. Press `Ctrl+F` → All modules → String references
2. Type keywords (e.g., "swing", "particle", "attack")
3. Double-click a result to jump to that code
4. Scroll up to find the function start (look for `push rbp` or `sub rsp,`)
5. **Right-click the bytes column (left side) → Copy** - that's your signature!

**Example:** Finding swingArm
- Search "swing"
- Find animation code
- Copy bytes from start of function

### Method 2: Breakpoint When It Happens

**Steps:**
1. Search for text related to what you want
2. Right-click instruction → Breakpoint → Set Breakpoint
3. Press F9 to continue
4. Do the action in-game (attack, break block, etc.)
5. x64dbg pauses
6. Press `Ctrl+K` to see call stack
7. Copy bytes from the function that called this

---

## Finding Memory Offsets

**Example:** Finding player velocity offset

**Steps:**
1. Jump in-game so your Y velocity is ~0.4
2. In x64dbg: `Ctrl+F` → Find Pattern → Type `0.4` (as float)
3. When found, **right-click → Breakpoint → Hardware, Access → DWORD**
4. Press F9, then move in-game
5. x64dbg pauses showing: `movss xmm0, [rcx+1B0]`
6. The `+1B0` is your offset! (0x1B0 = 432 decimal)

**Yes, you can copy directly from the main view!** Just right-click the bytes column when x64dbg stops at your breakpoint.

---

## Making Signatures

A signature is the first 10-20 bytes with wildcards for changing parts.

**Rules:**
- Copy 10-20 bytes from function start
- Replace addresses with `?` (e.g., `E8 12 34 56 78` → `E8 ? ? ? ?`)
- Keep instruction codes the same

**Example:**
```
Original: 40 53 48 83 EC 20 48 8B D9 E8 45 67 89 AB
Make it:  40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ?
                                        ^^^^^^^^^^
                                        (call address changes)
```

**Test:** Press `Ctrl+F` → Binary → Pattern, paste signature. Should match only once.

---

## Quick Example: LocalPlayer::swingArm()

**Goal:** Find arm swing function

1. Press `Ctrl+F` → Search "swing" or "attack"
2. Double-click result
3. Right-click → Breakpoint → Set Breakpoint
4. Press F9
5. Attack in Minecraft
6. x64dbg pauses
7. Scroll up to function start
8. **Right-click bytes column → Copy**
9. Example: `40 53 48 83 EC 20 48 8B D9 E8 12 34 56 78`
10. Replace call address: `40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ?`

Add to `Signatures.h`:
```cpp
FUNC("LocalPlayer::swingArm", "40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ?");
```

---

## Adding to Xorion

### For Functions
Edit `SDK/Signatures.h`:
```cpp
FUNC("FunctionName", "48 89 5C 24 ? 57 48 83 EC 20");
```

### For Memory Offsets
Edit class file (`SDK/Entity.h`, `SDK/LocalPlayer.h`):
```cpp
BUILD_ACCESS(this, Vec3, velocity, 0x1B0);  // offset you found
```

### Rebuild
```powershell
cd C:\Xorion\build
cmake --build . --config Release
```

---

## Quick Reference

### What to Search
- **swingArm**: "swing", "attack"
- **addParticle**: "particle"
- **Position/Velocity**: Your XYZ coords as floats
- **Health**: 20.0

### Common Assembly
```assembly
Function Start:
  40 53              push rbx
  48 83 EC 20       sub rsp, 20

Reading Memory:
  movss xmm0, [rcx+1B0]  <- offset is 0x1B0
  
Function Call:
  E8 12 34 56 78    <- replace with E8 ? ? ? ? in signature
```

### Tips
- Use **Hardware breakpoints** (right-click → Breakpoint → Hardware)
- **Call stack** (Ctrl+K) shows you what called what
- **Copy bytes** from the main disassembly view
- Test signatures with Ctrl+F → Pattern before adding

Done! Just attach, search, copy bytes, replace addresses with `?`, and add to Xorion.
