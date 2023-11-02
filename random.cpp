#include "random.h"
#include <ctime>

uint32_t Random::seed = 0;

void Random::init(int offset)
{
    seed += time(nullptr) + offset * 16588;
}
