Thomas Britnell
Eitan Yehuda

Nov. 2021

Assignment 3
We used Windows 10 OS and completed this assignment in VSCode.

We compiled the code in MSYS2 MINGW-64 using the Makefile. Simply type make to remove past exe, compile program, and run.

Controlable features:
- Press the arrow keys to move the selected light.
- Press 1 and 2 to change which light is selected.
- Press l to turn the lights on or off.
- Use w a s d to change the direction of the cannon.
- Press f to disable/enable friction mode.
- Press x, y, and z to increase the rotation of the scene around each respective axis.
- Press X, Y, and Z to decrease these rotations.
- Press p to enter particle camera mode, which follows the next particle until it dies.
- Press t to enable/disable particle trails.
- Press space to turn the fountain on or off.
- Press = and - to zoom in and out respectively.
- Press r to reset the particles.
- Press q to exit the program.

Design decision notes:
- particle trails follow the entire path of each particle when setting is toggled on with 't' and only disapear if particle 
dies or setting is toggled off.
- when particles are resert with 'r' the fountain stays on.
- canon rotation has a maximum rotation of 45 degrees in each direction.
- instead of arrow keys, we used x, y and z for scene rotations in order to enable user to rotate about the x, y and z axes.
- arrow keys are used to move the light sources which are represented by teapots. 1 and 2 toggle which source is currently 
seleccted.
- the floor is a grid just for easier evaluation.
