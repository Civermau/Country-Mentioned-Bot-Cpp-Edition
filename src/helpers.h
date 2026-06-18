#ifndef HELPERS_H
#define HELPERS_H

#include <string>

std::string loadBotToken();
void saveBotToken(const std::string& token);
std::string toUpperUTF8(const std::string& str);

#endif // HELPERS_H