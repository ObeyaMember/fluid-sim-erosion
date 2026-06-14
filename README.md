




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
making use of **compute shaders** on the **GPU** (while keeping the partitioning). The consecutive perofrmance improvements lead to the program going from a
300 particles real-time simulation capacity to around 30 to 50 thousand.  

All the graphics are rendered using th OpenGL pipeline.

## 🚀 In action
<p align="center">
  <img src="https://github.com/user-attachments/assets/27d4085f-a8e7-4317-bde6-6215ca3d197b" width="350">
  <img src="https://github.com/user-attachments/assets/8116d5aa-707f-4254-87f3-86f0f9bdae9b" width="350">
  <img src="https://github.com/user-attachments/assets/e53cecf9-d8c0-425a-8e2a-d250e080ac80" width="350">
</p>


<p align="center">
  <img src="https://github.com/user-attachments/assets/37c8a740-3d87-4cb2-a091-2e5c33130b91" width="360">
  <img src="https://github.com/user-attachments/assets/40533b9a-831c-4a5b-aff5-86aacf2df71b" width="370">
</p>

