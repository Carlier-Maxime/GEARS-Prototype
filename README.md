# ⚙️ G.E.A.R.S. (Gameplay Effect Automation Resources System)

**G.E.A.R.S.** is a top-down Factory Game developed with **Unreal Engine 5**.
The ultimate goal is to design the "Perfect Gear" through a complex, automated production system.

This project is a personal deep-dive into the **Gameplay Ability System (GAS)**
within an industrial simulation context, focusing on high-performance **Native C++** systems.

---

## 🛠️ Tech Stack & Architecture

- **Engine:** Unreal Engine 5.7+
- **IDE:** JetBrains Rider
- **Key Systems:**
  - **Gameplay Ability System (GAS):** Manages machine states, power consumption, and production buffs/debuffs.
  - **High-Performance Conveyors:**
    - **Native Logic:** Pure C++ movement math to handle thousands of items with zero Blueprint overhead.
    - **Instanced Rendering:** Massive optimization using **HISM** (Hierarchical Instanced Static Meshes) for item visualization.
  - **Data-Driven Design:** `UPrimaryDataAsset` for lightweight item definitions and asynchronous loading via the **Asset Manager**.

---

## 🚀 Technical Roadmap

### Phase 1: Foundations & World Exploration (Completed ✅)

<details>
<summary><b>🛠️ Project Initialization</b></summary>

- [x] Initial setup with **Unreal Engine 5.7+** (C++ Blank Project).
- [x] Git **Ignore** configuration for UE5 & Rider.
- [x] Git **Attributes** configuration for LFS binary assets.
</details>

<details>
<summary><b>🕹️ RTS-Style 3C</b></summary>

- [x] **Advanced Camera:** Pan/Zoom via SpringArm with Spring-based smoothing.
- [x] **Camera Tweaker:** Pitch lock, 90° auto-align, and dynamic FOV scaling.
- [x] **Input System:** ZQSD movement & Mouse Picking for grid interaction.
</details>

<details>
<summary><b>🎨 UI Framework (Stylizer)</b></summary>

- [x] **C++ Base Widgets:** Core logic implemented in C++ to minimize Blueprint overhead.
- [x] **Modular Components:** UIComponent system for styling (Border, Buttons, Checkboxes, …).
- [x] **Data-Driven Style:** Centralized style configuration via DataAssets.
</details>

<details>
<summary><b>🌍 World Subsystem</b></summary>

- [x] **GridSubsystem:** Implementation of specialized types (`FWorldGridPos`, `FChunkIndex`).
- [x] **Data-Driven Config:** Centralized settings via `DeveloperSettings`.
- [x] **Material Integration:** Real-time updates via `Material Parameter Collections`.
</details>

<details>
<summary><b>🎲 Procedural Generation Engine</b></summary>

- [x] **Noise System:** Multi-octave Fractal Brownian Motion (fBm) implementation.
- [x] **Biome Distribution:** Voronoi-based climate points (Temperature/Humidity).
- [x] **Resource Spawning:** Tag-based rules with biome-specific weighting.
- [x] **Noise Previews:** Preview generation for biome and resource distributions in the editor.
</details>

<details>
<summary><b>⚡ Optimized Rendering & Data</b></summary>

- [x] **WorldBatcher:** Task-based generation using `ParallelFor` and scoped locking.
- [x] **HISM Management:** Procedural chunk-based floor tiling for high performance.
- [x] **DataRegistry:** Decoupled management of Biomes and Resources.
</details>

### Phase 2: Survival & Manual Interaction (In Progress 🏗️)
- [ ] **Data Architecture:** Base implementation of `UItemDataAsset` and `UMachineDataAsset` (Data-Driven design).
- [ ] **Interaction System:** Native C++ Line-trace interaction for manual resource gathering (woodcutting/mining).
- [ ] **Inventory System:** Lightweight C++ container system for item management.

### Phase 3: The Logistics Core (The "C++ Powerhouse")
- [ ] **HISM Manager:** Global Singleton for handling thousands of instanced meshes for world items.
- [ ] **Native Conveyors:** Performance-first movement logic for belts with item-ahead collision detection.
- [ ] **GAS Integration:** Setup of `AttributeSets` (Energy, Throughput) and core Gameplay Abilities for machines.

### Phase 4: Industrial Automation
- [ ] **Grid Building:** Ghost-mesh placement system with snapping and validity checks.
- [ ] **Power Grid:** A distribution graph system connecting power generators to consumers.
- [ ] **Machine Logic:** Fully automated Miners and Crafters integrated with the GAS production cycle.
- [ ] **Minimalist HUD:** Building selector and resource throughput monitor.

### Phase 5: Advanced Systems & Optimization
- [ ] **Persistence:** Robust Save/Load system for the grid state and machine data.
- [ ] **Advanced GAS Effects:** Overclocking, regional buffs (heat/cold), and visual feedback via Gameplay Cues.
- [ ] **Logic Control:** "Redstone-like" signal system to enable/disable factory segments based on conditions.
- [ ] **Factory Analytics:** Resource manager viewer to track global production efficiency.

---

## 💻 Setup & Contribution

> **Note:** This project requires Git LFS for binary assets.

1. Clone the repository: `git clone [REPO_URL]`
2. Right-click `GEARS.uproject` > `Generate Visual Studio project files`.
3. Open the project with **JetBrains Rider** or VS2022.
