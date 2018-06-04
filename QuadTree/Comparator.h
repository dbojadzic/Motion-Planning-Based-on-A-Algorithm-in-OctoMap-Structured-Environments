#ifndef COMPARATOR_H_INCLUDED
#define COMPARATOR_H_INCLUDED

struct Comparator{

    bool operator()(Node* a, Node* b) const {
        if(a==b)
            return false;
        else
            return a -> fScore < b -> fScore;
    }

    /*
    bool operator()(Node* a, Node* b) const {
        return a -> fScore < b -> fScore;
    }
    */
};

#endif // COMPARATOR_H_INCLUDED
