#pragma once

#include <string>

namespace ariel
{
#define LEN 9     // length of array --string--
#define EPSILON 0 // precision

enum class Unit
{
    CM = 1, // length units
    M,
    KM,
    SEC = 10, // time units
    MIN,
    HOUR,
    G = 20, // mass units
    KG,
    TON
};

const std::string units[LEN] = {"[cm]", "[m]", "[km]", "[sec]", "[min]", "[hour]", "[g]", "[kg]", "[ton]"};
} // namespace ariel