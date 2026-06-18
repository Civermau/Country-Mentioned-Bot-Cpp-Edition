#ifndef TYPES_H
#define TYPES_H

#include <string>

struct CountryData {
    int id;
    std::string en_name;
    std::string es_name;
};

struct TopCountryData {
    std::string name;
    std::string mentions;
};

#endif // TYPES_H