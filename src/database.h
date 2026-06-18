#ifndef DATABASE_H
#define DATABASE_H

#include "types.h"
#include <string>
#include <vector>

std::vector<CountryData> getCountries();
void addMentionCounter(const int &id);
void addUserMentionCounter(const int &userId, const int &countryId);
void addGuildMentionCounter(const int &guildId, const int &countryId);
std::vector<TopCountryData> getTopThreeMentionedCountries();
std::vector<TopCountryData> getUserTopThreeMentionedCountries();
void initDatabase();

#endif // DATABASE_H