#ifndef COMPARATOR_H_INCLUDED
#define COMPARATOR_H_INCLUDED

class Comparator{
    public:
    bool operator()(Node* a, Node *b){
        return a -> weight < b -> weight;
    }
};

#endif // COMPARATOR_H_INCLUDED
