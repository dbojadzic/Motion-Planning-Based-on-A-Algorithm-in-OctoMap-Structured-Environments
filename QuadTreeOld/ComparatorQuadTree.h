#ifndef COMPARATORQUADTREE_H_INCLUDED
#define COMPARATORQUADTREE_H_INCLUDED

class ComparatorQuadTree{
    public:
    bool operator()(Node* a, Node *b) const {
        return a -> topLeft.x * 10000 + a -> topLeft.y < b -> topLeft.x * 10000 + b -> topLeft.y;
    }
};

#endif // COMPARATORQUADTREE_H_INCLUDED
