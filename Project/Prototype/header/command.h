#ifndef COMMAND_H
#define COMMAND_H
class Command{
    public:
        virtual void execute();
        virtual void undo();
};

#endif

// place
// colourChange
// rotate
// move
// delete