#ifndef COMPARATOR_H_INCLUDED
#define COMPARATOR_H_INCLUDED

struct Comparator{

    bool operator()(Node* a, Node* b) const {
        if(a==b)
            return false;
        /*
        if(fabs(a -> fScore - b -> fScore) < std::numeric_limits<double>::epsilon()){
            std::cout << a ->GetCenter().x << " " << a ->GetCenter().y << std::endl;
            std::cout << b ->GetCenter().x << " " << b ->GetCenter().y << std::endl;
            return  a -> goaldist < b -> goaldist;
        }
        */

        return a -> fScore < b -> fScore;
    }
};

#endif // COMPARATOR_H_INCLUDED
