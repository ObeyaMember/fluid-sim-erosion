




## 🌟 Highlights

- Fluid simulation through a Smoothed Particle Dynamics Approach
- Spatial partitionning
- CPU and GPU implementations
- Fluid/Ground collisions and erosion
- Visualization using OpenGL


## ℹ️ Overview

This C project was developed in the context of the french Classes Préparatoires' TIPE presentation, with the yearly theme being **Cycles, boucles** (Cycles, loops). 

In its current state, the program can simulate fluids and their effect on ground through erosion. There are three different implementations inside the code. One on the **CPU**,
with no real algorithmic regard for performance (in O(n_particles²)), a second CPU-side implementation, this time featuring **spatial grid partitioning**, and a third one 
making use of **compute shaders** on the **GPU** (while keeping the partitioning).

All the graphics arerendered using th OpenGL pipeline.

## 🚀 In action
<img width="727" height="465" alt="Screenshot From 2026-06-04 02-32-44" src="https://github.com/user-attachments/assets/37c8a740-3d87-4cb2-a091-2e5c33130b91" />
<img width="740" height="710" alt="Screenshot From 2026-06-04 02-36-46" src="https://github.com/user-attachments/assets/40533b9a-831c-4a5b-aff5-86aacf2df71b" />
<img width="686" height="573" alt="Screenshot From 2026-06-08 09-53-41" src="https://github.com/user-attachments/assets/27d4085f-a8e7-4317-bde6-6215ca3d197b" />
<img width="686" height="573" alt="Screenshot From 2026-06-08 09-53-59" src="https://github.com/user-attachments/assets/8116d5aa-707f-4254-87f3-86f0f9bdae9b" />
<img width="686" height="573" alt="Screenshot From 2026-06-08 09-54-13" src="https://github.com/user-attachments/assets/e53cecf9-d8c0-425a-8e2a-d250e080ac80" />
