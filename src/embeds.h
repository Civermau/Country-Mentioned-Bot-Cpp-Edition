#ifndef EMBEDS_H
#define EMBEDS_H

#include "types.h"
#include <dpp/dpp.h>
#include <vector>

dpp::embed buildInfoEmbed(const std::vector<TopCountryData>& topCountries);
dpp::embed buildUserInfoEmbed(const std::vector<TopCountryData>& countries);

#endif