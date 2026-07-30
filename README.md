# RewindKit
**RewindKit** is a high-performance C++ runtime plugin that enables position and rotation recording and seamless time rewinding for any Actor or Character in Unreal Engine.

## 🌟 Key Features
### ⏳ Frame-Accurate Recording & Rewind
- **Universal Actor Component (`URewindTimeComponent`)**: Attach to Players, Enemies, Projectiles, Vehicles, or Physics Props.
- **Continuous Trajectory Buffering**: Automatically records transform history (`FVector` positions and `FRotator` rotations) into optimized memory buffers.
- **Configurable Rewind Window**: Easily clamp and set recording durations (1.0s to 5.0s) for low memory footprint and high FPS.
---
### 🎨 Visual & Audio Event Sync
- **State Phase Delegate (`FOnTimePhaseChanged`)**: Broadcasts current phase state (`ETP_Record` vs `ETP_Rewind`).
- **Post-Process & Audio Integration**: Use state delegates to trigger post-process visual distortions, rewind audio reversal, particle effects, or play Animation Blueprints in reverse.
- **Real-Time Visual Debugging**: Optional visual debug mode (`bShowDebugRecording`) that draws 3D coordinate axes along the recorded motion path in real-time.
---
## 🚀 Blueprint & C++ Usage
### Simple Blueprint Calls:
1. Attach `RewindTimeComponent` to your Actor.
2. Call `StartRecordMovement()` when recording should begin.
3. Call `StartRewindMovement()` (e.g., on keypress or skill trigger) to replay historical positions backward frame-by-frame.
4. Call `StopRewindMovement()` to return to normal gameplay.
```cpp
--- Test Actor Example come with the plugin 
<img width="478" height="204" alt="w1" src="https://github.com/user-attachments/assets/de9ba61b-ae5f-4b55-af0a-b9f580cfcf96" />

simple blueprint code within the Actor 
<img width="1917" height="937" alt="w2" src="https://github.com/user-attachments/assets/f4b1409b-7565-45f7-a278-e7a4c0fb5d61" />


// C++ Execution Example:
RewindComponent->StartRewindMovement();

🛠️ Technical Details
Module Type: Runtime
Primary Component: URewindTimeComponent
Supported Platforms: Win64, Mac, Linux, iOS, Android, Consoles


