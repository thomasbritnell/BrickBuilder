
#ifndef BOARD_H
#define BOARD_H

#include "mathLib3D.h"
#include "brick.h"
#include "vector"


class Board{
    public:
        Board();
        Board(Point3D center,int cellSize, int length, int width, std::vector<Brick> bricks);

        Point3D center;
        int cellSize;
        int length;   // half the number of cells in z axis
        int width;    // half the number of cells in x axis
        std::vector<Brick> bricks;
        int cursorPos[3];

        void placeBrick(Brick brick);
        void undoPlaceBrick();
        void deleteBrick(Brick brick);
};

#endif