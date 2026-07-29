#include "move.hpp"
#include "utils.hpp"

std::string Move::toString() const {
    std::string s;
    s += fileChar(current_square);
    s += rankChar(current_square);
    s += fileChar(target_square);
    s += rankChar(target_square);

    if(flags & PROMOTION){
        s += 'q'; //default to queen promotion for now
    }

    return s;
}
