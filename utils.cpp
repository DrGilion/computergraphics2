#include "utils.h"

#include <algorithm>

int Utils::clamp(int value, int lowerbound, int upperbound){
    return std::max(lowerbound,std::min(value,upperbound));
}
