#include "helpers.h"
#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>

std::string loadBotToken() {
    std::string token;
    std::ifstream env_file(".env");
    if (env_file.is_open()) {
        std::string line;
        std::getline(env_file, line);
        token = line.substr(10);
        env_file.close();
    }
    return token;
}

void saveBotToken(const std::string &token) {
    std::ofstream env_file(".env");
    if (!env_file.is_open()) {
        std::cout << "Error: Could not create .env file" << std::endl;
        return;
    }
    env_file << "BOT_TOKEN=" << token;
    env_file.close();
    std::cout << "Bot token saved to .env file" << std::endl;
}

std::string toUpperUTF8(const std::string &str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    std::wstring wide = conv.from_bytes(str);
    std::locale loc("");
    for (auto &c : wide) c = std::toupper(c, loc);
    return conv.to_bytes(wide);
}