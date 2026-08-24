# physics-sim

This is a simple self-made physics simulation I made with SFML 2.5.0. I made this as a simple project after my first year in CS. 

As of now (24th, August, 2026) the simulation is complete and running. There are a few bugs that I need to fix, namely particles phasing through each other when using the Tree Collision Detection and Brute Force (only at higher particle speeds). 

The only unique aspect of this project was that I used a Binary Tree to Optimize Collision detection instead of a Quad-Tree, since I wanted to test if it actually improves performance, which turns out to be a huge success.

# How to use
Upon Running the code, you will be greeted with 3 buttons in the start menu, clicking on the first leads to the Simulation, the second leads to the Settings of the Simulation, and the third to close the simulation. 

Starting with the settings: 
You can change the X and Y axis accelerations from -25.0 to 25.0.
You can decide which algorithm is used in collision detection.

Upon tinkering with the settings you can return to the menu with the red button on the bottom 

Next the Simulation window:
The Simulation window contains a white box which is the simulation box, where all the particles and physics will be 
simulated
Here you are greeted with 3 buttons on the right:
The first allows you to create a particle:
Here you can change the particles attribuites like radius and mass through the sliders
and create a particle by clicking in the simulation box

The second will allow you to delete one:
Just click on the particle you wish to delete

The third will close the simulation

That is all.

Thank you for using my physics simulation