#include "header/placeBrickCommand.h"



PlaceBrickCommand::PlaceBrickCommand(Brick brick, Board* board) : brick(brick), board(board){}
    

void PlaceBrickCommand::execute(){
    this->board->placeBrick(this->brick);
}

void PlaceBrickCommand::undo(){
    this->board->undoPlaceBrick();
}
