#include "random_gen.h"

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 random_gen(seed);
