#include "header/board.h"


Board::Board(Point3D center,int cellSize, int length, int width, std::vector<Brick> bricks): 
    center(center), cellSize(cellSize),length(length),width(width),bricks(bricks){
        this->cursorPos[0] = 0;
        this->cursorPos[1] = 0;
        this->cursorPos[2] = 0;
    }

Board::Board() : Board(Point3D(),1,5,5,std::vector<Brick>()){
    this->cursorPos[0] = 0;
    this->cursorPos[1] = 0;
    this->cursorPos[2] = 0;
}

void Board::placeBrick(Brick brick){
    this->bricks.push_back(brick);
}

void Board::undoPlaceBrick(){
    this->bricks.pop_back();
}

void Board::deleteBrick(Brick brick){
    for(std::vector<Brick>::iterator b = this->bricks.begin(); b != this->bricks.end(); ++b){ // parse through actual addresses of bricks in the array
        if(b->position.isEqualTo(brick.position)){ // check with brick position
            this->bricks.erase(b);
            break;
        }
    }
}