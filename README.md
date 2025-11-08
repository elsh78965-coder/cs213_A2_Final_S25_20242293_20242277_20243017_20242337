AudioPlayer Project

Faculty of Computers and Artificial Intelligence – Cairo University
Under the supervision of: Dr. Mohamed El-Ramly


---

Course Information

Course Code: CS213 – Object-Oriented Programming

Assignment: #2 — Audio Player (Final Submission)

Project Code: cs213_A2_Final_S25_20242293_20242277_20243017_20242337



---

Team Members

Name	University ID

Mohamed Abd El-Baset Abd El-Bary	20242293
Mohamed Hassan Mousa	20242277
Ezz El-Din Amgad	20243017
Mostafa Hammad Mohamed	20242337



---

Project Overview

The AudioPlayer project is a modular desktop music player built using the JUCE Framework and C++.
It applies Object-Oriented Programming concepts such as Encapsulation, Composition, and Polymorphism.
The system allows users to load, play, pause, and control audio files through an intuitive GUI.
It ensures clean architecture, efficient resource management, and synchronization between audio and GUI modules.


---

Project Objectives

Design and implement an audio player using OOP principles.

Utilize the JUCE framework for real-time audio processing and GUI development.

Build a scalable and maintainable architecture for future extensions.

Gain practical understanding of event-driven multimedia programming.



---

Implemented Features

#	Feature	Description

1	Load Audio File	Supports .wav, .mp3, .aiff, and other formats.
2	Play / Stop / Pause	Controls playback operations smoothly.
3	Mute / Unmute	Toggles sound output instantly.
4	Loop Playback	Repeats track continuously.
5	Forward / Backward (10s)	Skips 10 seconds forward or backward.
6	Gain Control	Adjusts playback volume using a slider.
7	Position Control	Seeks to any part of the audio track.
8	Transport Management	Handles playback states efficiently.
9	File Validation	Prevents invalid file loading and errors.
10	GUI Integration	User-friendly interface built with JUCE components.
11	Resource Management	Uses prepareToPlay and releaseResources correctly.
12	Encapsulated Audio Logic	Separates backend audio from interface logic.
13	Event-Driven Design	Keeps GUI and audio modules synchronized.
14	Modular Structure	Organized, reusable, and extendable codebase.



---

Project Architecture

AudioPlayer/
│
├── Source/
│   ├── PlayerAudio.h / .cpp      → Handles all audio playback and logic
│   ├── PlayerGUI.h / .cpp        → Manages GUI controls and interactions
│   ├── MainComponent.h / .cpp    → Integrates GUI and audio modules
│   └── Main.cpp                  → Application entry point
│
└── AudioPlayer.jucer             → JUCE configuration file


---

Class Descriptions

PlayerAudio

Handles all core audio functionality.
Implements AudioTransportSource for managing playback.
Provides methods to load files, play, stop, loop, and control gain and position.
Responsible for resource preparation and cleanup.

PlayerGUI

Contains buttons, sliders, and labels.
Connects user input to PlayerAudio methods.
Updates interface elements to reflect playback state.

MainComponent

Acts as the main controller of the application.
Initializes JUCE environment and integrates GUI with audio logic.
Handles layout rendering and updates.


---

Build & Run Instructions

1. Open AudioPlayer.jucer in Projucer.


2. Export the project to Visual Studio 2022 or Xcode.


3. Open the exported project in the IDE.


4. Build and run the application.


5. Load an audio file and use the interface to control playback.



Note: Make sure the JUCE framework is correctly installed and linked before building.


---

Testing & Verification

Test Case	Result

Load valid audio files	Passed
Load invalid files	Handled with error message
Play / Stop / Pause	Passed
Mute / Unmute	Passed
Loop playback	Passed
Forward / Backward jump	Passed
Gain and position control	Passed
Application performance	Stable and responsive



---

Future Enhancements

Feature	Description

Playback Speed Control	Adjust playback rate dynamically.
Playlist Management	Support multiple tracks.
Waveform Visualization	Display real-time waveform.
Track Metadata Display	Show title, artist, and duration.
Custom Loop Points	Allow user-defined loop regions.
Enhanced UI Design	Improved layout and themes.



---

Technologies Used

Category	Tool / Library

Programming Language	C++
Framework	JUCE
Paradigm	Object-Oriented Programming
IDE	Visual Studio 2022 / Projucer
Version Control	Git and GitHub



---

Conclusion

The AudioPlayer project demonstrates the application of Object-Oriented Design and audio programming using JUCE.
It combines modular architecture, efficient resource handling, and a simple interface to create a functional and extendable audio player suitable for future improvements.


---

Prepared by Group  – CS213, Cairo University . FCAI
Under the supervision of: Dr. Mohamed El-Ramly `  

