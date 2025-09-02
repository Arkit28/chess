#include "move.hpp"
#include "utils.hpp"

std::string Move::toString() const {
    std::string s;
    s += fileChar(from);
    s += rankChar(from);
    s += fileChar(to);
    s += rankChar(to);

    if(flags & PROMOTION){
        s += 'q'; //default to queen promotion for now
    }

    return s;
}
