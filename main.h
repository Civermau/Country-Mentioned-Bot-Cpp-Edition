#ifndef MAIN_H
#define MAIN_H

#include <dpp/dpp.h>
#include <string>
#include <vector>

struct CountryData {
    int id;
    std::string en_name;
    std::string es_name;
};

struct TopCountryData {
    std::string name;
    std::string mentions;
    int rank;
};

std::vector<CountryData> getCountries();
dpp::embed buildInfoEmbed(std::vector<TopCountryData> topCountries);
void addMentionCounter(const std::string& id);
std::string getMostMentionedCountry();
std::vector<TopCountryData> getTopThreeMentionedCountries();
std::string loadBotToken();
void saveBotToken(const std::string& token);

#endif // MAIN_H
