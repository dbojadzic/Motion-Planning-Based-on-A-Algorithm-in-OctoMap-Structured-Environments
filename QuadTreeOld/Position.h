#ifndef POSITION_H_INCLUDED
#define POSITION_H_INCLUDED

struct Position{
    bool top;
    bool left;
    Position(bool top, bool left) : top(top), left(left) {}
    Position operator!(){
        return Position(!top, !left);
    }
    /*
    bool operator ==(Position position){
        return top == position.top && left == position.left;
    }
    bool operator !=(Position position){
        return top != position.top || left != position.left;
    }
    */
};

#endif // POSITION_H_INCLUDED
