// #include "command.h"
#include "mathLib3D.h"
#include "brick.h"
#include "board.h"

class PlaceBrickCommand{
    public:
        Brick brick;
        Board* board;
        
        PlaceBrickCommand(Brick brick, Board* board);

        void execute();
        void undo();

};