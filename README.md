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

### Phase 1: Foundations & World Exploration (In Progress 🏗️)
- [x] **Project Initialization:** UE 5.7+ C++ Blank Project & Git LFS configuration.
- [ ] **RTS-Style 3C:** Top-down camera system (Pan/Zoom) with Mouse Picking for grid interaction.
- [ ] **World Subsystem:** Implementation of a C++ `WorldSubsystem` for high-performance grid math (World-to-Grid coordinates).
- [ ] **Environment:** Procedural generation for trees (wood) and mineral veins using Noise-based sampling.

### Phase 2: Survival & Manual Interaction
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
