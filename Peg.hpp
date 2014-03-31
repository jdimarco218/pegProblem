#ifndef PEG_HPP
#define PEG_HPP

#include <vector>

class Peg{
    public:

    int position;
    int row;
    int col;
    bool hasPeg;
    std::vector<int> neighborList;

    Peg(int, int, int);

};

#endif
