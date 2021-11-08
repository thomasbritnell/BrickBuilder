Thomas Britnell
Eitan Yehuda

Nov. 2021

Assignment 3
Developed on Windows 10


Design Decisions:


Our 3 special features are 
    -particle trails, which follow every particle when enabled
    -particle camera mode, which follows a particle when p is pressed
    -the ability to change the direction of the particle stream (using WASD) via a particle "cannon"

In order for the particles to fall and bounce realistically, the "gravity" had to be tweaked. The gravitational constant we settled on was (g/200), which can be found in the update function of particle.cpp. 

The lights are modeled with solid teapots so the user can see where they are, in order to better manipulate them (using the arrow keys).

The cannon arm is limited to go no further than a 45 degree angle, so the user can't shoot the particle right into the ground.



Program Usage:

Press the arrow keys to move the selected light
Press 1 and 2 to change which light is selected
Press L to turn the lights on or off
Use w a s d to change the direction of the cannon
Press f to enable friction mode
Press x, y, and z to increase the rotation of the scene in each respective direction
Press X, Y, and Z to decrease these rotations
Press p to enter particle camera mode, which follows the next particle until it dies
Press space to turn the fountain on or off
Press = and - to zoom in and out respectively
Press t to add particle trails
Press r to reset the particles
Press q to exit the program
