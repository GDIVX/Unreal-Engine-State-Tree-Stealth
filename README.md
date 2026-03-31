# UE5 Stealth AI System: State Tree Implementation

## Project Overview
A technical demonstration of a modular stealth AI system built in Unreal Engine 5. This project focuses on utilizing the State Tree system to create reactive, designer-friendly AI agents with a clean C++/Blueprint hybrid architecture.

## Key Features
* **State Tree Architecture:** Implements a data-driven, event-based approach for AI behavior, providing a performant and scalable alternative to traditional Behavior Trees.
* **Component-Based Detection:**
    * DetectionComponent.cpp: Acts as a mediator for Pawn Sensing and manages AI-side perception logic.
    * DetectableComponent.cpp: Attached to the player to track observers, manage detection scores, and record breadcrumb locations.
* **Spline-Based Patrols:** Decouples patrol data from the agent. Level designers can place spline actors in the world, which AI agents automatically identify and follow based on proximity.
* **Dynamic Searching:** Implements breadcrumb tracking and ghost point generation to simulate realistic environmental searching after the agent loses line of sight.
* **Editor Utility Widget:** A custom tool developed to automate outliner organization, ensuring project hierarchy remains clean during rapid level iteration.

## Technical Implementation
* **Logic Split:** Core systems, heavy math, and base components are written in C++. Blueprints are utilized for high-level orchestration, visual effects, and state-tree task configurations.
* **Evaluators:** Custom C++ evaluators act as event dispatchers between the sensing components and the State Tree, ensuring the agent reacts immediately to environmental stimuli.
* **Modularity:** Systems are built to be extensible. Adding new behaviors involves creating a new state and hooking it into the existing event-driven framework.

## Setup
1. Clone the repository.
2. Right-click the .uproject file and select "Generate Visual Studio project files."
3. Open the .sln file and build the project in the Development Editor configuration.
4. Open the project in Unreal Engine 5.

## Future Considerations
While the current suspicion logic is handled via custom components, migrating detection levels to the Gameplay Ability System (GAS) attributes would allow for more complex interactions, such as environmental modifiers affecting detection rates through standard Gameplay Effects.
