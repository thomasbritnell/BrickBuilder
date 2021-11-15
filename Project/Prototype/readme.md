Thomas Britnell 400129897
Eitan Yehuda 400117972

Developed on windows 10

simply type make to run the program. 


Program Usage:

Press 1 and 2 to increase/decrease the grid width.
Press 3 and 4 to increase/decrease the grid length.
Use the scroll wheel to zoom in and out.
Press a to add a brick at a random location.
Press u to undo a brick placement.
Right click and drag with the mouse to rotate the scene.
Press q to quit.

Design Decisions:

Right now the .obj file being used is from blender, but isn't modeled with the lego brick studs on top yet.
Also, there are normals included with the obj file but they are not used yet since there is no lighting.
The resize feature doesnt' currently take into account the bricks present on the grid. Later on we plan to make it so you cannot lower the width or length of the grid past where existing bricks are. 
Also, the random bricks will overlap, meaning that a brick can be placed at the same poistion as another. Later we will add an array to the board object which keeps track of which grid cells are occupied, and you won't be able to add bricks there. The add random bricks feature is temporary and only to show that the bricks are being added and drawn correctly. 




