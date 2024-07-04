# PlayerStateSubsystem
This plugin is an extension of Unreal Engine's native Subsystem class.
If you're not familiar with subsystems, this is a good explainer from Epic Games.
https://dev.epicgames.com/documentation/en-us/unreal-engine/programming-subsystems-in-unreal-engine
TL;DR, Subsystems are singleton-like constructs that share their lifecycle with a specific object. Native examples in Engine include the Local Player Subsystem and the Game Instance Subsystem (which are attached to the Local Player and Game Instance, respectively).
Subsystems provide a convenient way to tie functionality to a given object with a known lifecycle.
I figured since the Player State persists through the entire session and is replicated, it would be handy to have a way of attaching stuff to it without cluttering up the main class too much.
<b>Be mindful that subsystems introduce some overhead. Always weigh the pros of decoupling your code from the Player State class with the cons of having an additional object hanging around</b>

# Quickstart guide
1. Download the repo
2. Move the PlayerStateSubsystem folder into [YourProject]/Plugins folder
3. Launch editor
4. In your <b>Game Mode</b>, make sure the <b>Player State</b> is set to <b>PlayerState_WithSubsystem</b>, or optionally subclass this class to implement your own functionality.
5. To create a Player State Subsystem, simply create a new class that inherits from <b>PlayerStateSubsystem</b>.
