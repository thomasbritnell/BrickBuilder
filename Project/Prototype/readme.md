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

Design Decisions/Write up:

Right now the .obj file being used is from blender, but isn't modeled with the lego brick studs on top yet. This is a simple thing to add to the final submission, it just needs to be remodelled. Also, we will have several more meshes than the current 1x1 brick that is added. 

Also, there are normals included with the obj file but they are not used yet since there is no lighting. This will be added to the final, with each brick having a colour (material) that is selectable by the user. 
The resize feature doesnt' currently take into account the bricks present on the grid. Later on we plan to make it so you cannot lower the width or length of the grid past where existing bricks are. 

Also, the random bricks currently overlap, meaning that a brick can be placed at the same poistion as another. Later we will add an array to the board object which keeps track of which grid cells are occupied, to prevent this. The add random bricks feature is temporary and only to show that the bricks are being added, drawn correctly, and can be undone. This feature will not be present in the final submission. Instead, the user will be able to choose which size/type of brick they want, its colour, orientation, and they will be able to choose its position with the mouse. This positioning will likely invove ray tracing to calculate which grid position is closest to the mouse, in order to place a brick there. 

In the final submission there will be some sort of visual preview to show the user where the brick they place will land on the board, which will be helpful for stacking bricks on top of one another (also not currently possible, but will likely be present in the final submission).

In the final submission bricks will not be able to clip through eachother; meaning that brick placements, translations, and rotations will be limited by the bricks surrounding. 






