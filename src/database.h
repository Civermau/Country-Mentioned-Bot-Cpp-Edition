#ifndef DATABASE_H
#define DATABASE_H

#include "types.h"
#include <vector>

std::vector<CountryData> getCountries();
void addMentionCounter(const int &id);
void addUserMentionCounter(const int &userId, const int &countryId);
void addGuildMentionCounter(const int &guildId, const int &countryId);
std::vector<TopCountryData> getTopThreeMentionedCountries();
std::vector<TopCountryData> getUserTopThreeMentionedCountries(const int &userID);
std::vector<TopCountryData> getGuildTopThreeMentionedCountries(const int &guildID);
void initDatabase();

#endif // DATABASE_H