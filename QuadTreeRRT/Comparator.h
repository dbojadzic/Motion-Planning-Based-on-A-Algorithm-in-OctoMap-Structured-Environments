#ifndef COMPARATOR_H_INCLUDED
#define COMPARATOR_H_INCLUDED

struct Comparator{

    bool operator()(Node* a, Node* b) const {
        if(a==b)
            return false;
        return a -> fScore < b -> fScore;
    }
};

#endif // COMPARATOR_H_INCLUDED
