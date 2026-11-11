# OpenBOR Build 6391 - Testing Plan & Verification Matrix

This testing plan outlines verification for OpenBOR v3.0 Build 6391 on both Windows development hosts and physical iOS devices (iPhone & iPad).

---

## 1. Test Environment Separation

| Test Area | Can Run on Windows? | Requires Physical iPhone/iPad? | Primary Verification Method |
|---|:---:|:---:|---|
| **CMake Build Configuration** | **YES** | NO | `scripts/verify-ios-cmake.bat` |
| **Engine Core Logic & Compilation** | **YES** | NO | Windows CMake target / GitHub Actions CI |
| **PAK Archive Header & Extraction** | **YES** | NO | Local PAK parser execution |
| **Touchscreen Virtual Controls** | NO | **YES** | Multi-finger touch inputs on iPhone / iPad |
| **Retina / Safe Area Insets (Notch / Island)** | NO | **YES** | Portrait / Landscape notch avoidance |
| **iOS Sandbox Storage (Files App Integration)** | NO | **YES** | Copying `.pak` to `On My iPhone/OpenBOR/Paks` |
| **Audio Backgrounding / Interruption** | NO | **YES** | Incoming phone call / lock screen / app switch |
| **Bluetooth MFi / Xbox / PS Controller** | Simulated | **YES** | Wireless gamepad pairing via iOS Bluetooth |

---

## 2. Verification Test Cases

### TC-01: Engine Initialization & Version Check
* **Objective:** Ensure the engine boots, initializes SDL2 video/audio subsystems, and reports version `OpenBOR v3.0 Build 6391`.
* **Procedure:** Launch application. Inspect initial log output in `Documents/Logs/OpenBoR.log`.
* **Pass Criteria:** Log states `OpenBoR v3.0 Build 6391, Compile Date: <date>`. No crash or memory corruption during `initSDL()`.

### TC-02: Game Module (.pak) Loading & Menu
* **Objective:** Ensure game modules located in `<Documents>/Paks` or embedded in `<Bundle>/Paks` are detected and selectable.
* **Procedure:** 
  1. Transfer a Build 6391 compatible `.pak` file into `<App Documents>/Paks/` via the iOS Files App.
  2. Launch OpenBOR.
* **Pass Criteria:** Mod selection menu lists the `.pak` file. Selecting it transitions smoothly into game startup.

### TC-03: Gameplay Systems (Spawning, Movement, Jump, Attack, Collision)
* **Objective:** Verify preservation of Build 6391 gameplay systems without engine regression.
* **Procedure:**
  1. Start Stage 1.
  2. Spawn player character.
  3. Walk in 8 directions (Up, Down, Left, Right, Diagonals).
  4. Perform standard attack string (Attack 1).
  5. Perform jump (Jump) and jump-attack combo.
  6. Perform special move (Special).
  7. Engage first enemy wave and verify collision boxes, knockdown, damage calculation, and death animations.
* **Pass Criteria:** Animation timing, collision boxes, and damage behavior match standard Build 6391 PC reference gameplay exactly.

### TC-04: Audio Playback (SFX & Ogg Vorbis BGM)
* **Objective:** Verify that sound effects and streaming background music play clearly without latency or distortion.
* **Procedure:** Listen to title screen BGM, stage BGM, punch/hit sound effects, and voice clips.
* **Pass Criteria:** Stereo audio plays cleanly via iOS CoreAudio driver without crackling or stuttering.

### TC-05: Touchscreen Virtual Gamepad
* **Objective:** Verify responsive touch control input and multi-touch handling.
* **Procedure:**
  1. Use virtual D-pad with left thumb.
  2. Simultaneously tap Attack and Jump with right thumb.
  3. Tap Start and Pause (Esc).
* **Pass Criteria:** Multi-touch correctly registers simultaneous directional input and button actuations without dropped frames or input latency.

### TC-06: Bluetooth Controller Priority
* **Objective:** Verify seamless handoff between touch controls and physical controllers.
* **Procedure:** Pair an Xbox Wireless Controller or DualSense gamepad via iOS Bluetooth Settings. Press any button on the physical controller during gameplay.
* **Pass Criteria:** Physical controller inputs immediately control the player character; on-screen virtual touch buttons fade/hide to prevent visual obstruction. Touching the screen restores touch controls.

### TC-07: Lifecycle Suspension, Resumption & Memory Management
* **Objective:** Ensure the app handles being backgrounded without crashing or audio looping.
* **Procedure:**
  1. During active stage gameplay, swipe up to return to iOS Home screen.
  2. Wait 10 seconds.
  3. Re-open OpenBOR from the App Switcher.
* **Pass Criteria:** Audio pauses while backgrounded; when resumed, audio resumes and frame delta timing resets cleanly without game fast-forwarding or audio desync.

### TC-08: Save Data Persistence
* **Objective:** Verify high scores, game saves, and control settings persist across app termination.
* **Procedure:** Complete a stage, record a high score, force-close OpenBOR from the iOS App Switcher, and restart.
* **Pass Criteria:** High score and stage progression remain intact in `<Documents>/Saves/`.
