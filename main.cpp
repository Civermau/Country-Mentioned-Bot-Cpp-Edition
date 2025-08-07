#include "main.h"
#include <sqlite3.h>
#include <iostream>
#include <map>
#include <fstream>
#include <cstdlib>

int main() {
    
    std::string BOT_TOKEN = loadBotToken();
    
    while (BOT_TOKEN.empty()) {
        char buffer[100];
        std::cout << "BOT_TOKEN is empty, please provide a token" << std::endl;
        std::cout << "If you write wrong the token the program won't close (I'll fix this later), so be careful" << std::endl;
        std::cout << "Token will be automatically saved to the .env file" << std::endl;
        std::cout << "To get a token, you can follow this guide: https://discord.com/developers/docs/quick-start/getting-started#step-1-creating-an-app" << std::endl;
        std::cout << "Token: ";
        std::cin.getline(buffer, 100);
        BOT_TOKEN = buffer;
        saveBotToken(BOT_TOKEN);
    }

    std::cout << "Loading countries from db..." << std::endl;
    std::vector<CountryData> countries = getCountries();    

    std::cout << "Countries loaded" << std::endl;
    std::cout << "[TODO] Find a way to load contries dinamically instead of hardcoded (If I want to add more languages)" << std::endl;

    dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);
 
    bot.on_log(dpp::utility::cout_logger());


    bot.on_message_create([&bot, &countries](const dpp::message_create_t& event) {
        if (event.msg.author.id == bot.me.id) {
            return;
        }

        std::string uppercaseMessage = event.msg.content;
        std::transform(uppercaseMessage.begin(), uppercaseMessage.end(), uppercaseMessage.begin(), ::toupper);

        dpp::message m(event.msg.channel_id, "");
        dpp::message emptyMessage(event.msg.channel_id, "");
        int filesAdded = 0;
        for (const auto& country : countries) {
            if (uppercaseMessage.contains(country.en_name) || uppercaseMessage.contains(country.es_name)) {
                m.add_file(country.en_name + ".jpg", dpp::utility::read_file("Images/" + country.en_name + ".jpg"));
                addMentionCounter(std::to_string(country.id));
                filesAdded++;

                // bot.message_add_reaction(event.msg.id, event.msg.channel_id, EMOJI_FLAG_MEXICO);

                if (filesAdded == 10) {
                    event.reply(m);
                    m = emptyMessage;
                    filesAdded = 0;
                }
            }
        }
        if (filesAdded > 0) {
            event.reply(m);
        }
    });
 
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {  
        if (event.command.get_command_name() == "info") {
            std::vector<TopCountryData> topCountries = getTopThreeMentionedCountries();
            dpp::message m(event.command.channel_id, buildInfoEmbed(topCountries));
            event.reply(m);
        }
    });
 
    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("info", "Get info about the bot", bot.me.id));
            std::cout << "Commands registered" << std::endl;
        }

        for (const auto& guild : event.guilds) {
            std::cout << "Guild: " << guild << std::endl;
        }
        
        std::cout << "[TODO] Add a way to get the guild name" << std::endl;
    });
 
    bot.start(dpp::st_wait);
}




// -------------------------------------DB MANAGEMENT
void addMentionCounter(const std::string& id) {
    sqlite3* db;
    sqlite3_open("CountryMentionedBot.db", &db);
    
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO mentions (id, mentions) VALUES (?, 1) ON CONFLICT(id) DO UPDATE SET mentions = mentions + 1";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(db);
}

std::vector<TopCountryData> getTopThreeMentionedCountries() {
    std::vector<TopCountryData> topCountries;
    sqlite3* db;
    sqlite3_open("CountryMentionedBot.db", &db);
    
    sqlite3_stmt* stmt;
    const char* sql = "SELECT c.en, m.mentions FROM countries c INNER JOIN mentions m ON c.id = m.id ORDER BY m.mentions DESC LIMIT 3";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        int rank = 1;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            TopCountryData country;
            
            const unsigned char* name_result = sqlite3_column_text(stmt, 0);
            if (name_result) {
                country.name = reinterpret_cast<const char*>(name_result);
            }
            
            const unsigned char* mentions_result = sqlite3_column_text(stmt, 1);
            if (mentions_result) {
                country.mentions = reinterpret_cast<const char*>(mentions_result);
            }
            
            country.rank = rank;
            topCountries.push_back(country);
            rank++;
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(db);
    
    return topCountries;
}

std::vector<CountryData> getCountries() {
    std::vector<CountryData> countries;
    sqlite3* db;
    sqlite3_open("CountryMentionedBot.db", &db);
    
    sqlite3_stmt* stmt;
    const char* sql = "SELECT id, en, es FROM countries";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            CountryData country;
            country.id = sqlite3_column_int(stmt, 0);
            
            const unsigned char* en_result = sqlite3_column_text(stmt, 1);
            if (en_result) {
                country.en_name = reinterpret_cast<const char*>(en_result);
            }
            
            const unsigned char* es_result = sqlite3_column_text(stmt, 2);
            if (es_result) {
                country.es_name = reinterpret_cast<const char*>(es_result);
            }
            
            countries.push_back(country);
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(db);
    return countries;
}


// ------------------ HELPERS
dpp::embed buildInfoEmbed(std::vector<TopCountryData> topCountries) {
    dpp::embed infoEmbed = dpp::embed()
        .set_author("Civer_mau", "https://github.com/Civermau", "https://avatars.githubusercontent.com/u/66493296")
        .set_title("Country Mentioned Bot!")
        .set_url("https://github.com/Civermau/Country-Mentioned-Bot-Cpp-Edition")
        .set_description("Let Yui inform you that you've just mentioned a country!")
        .set_color(dpp::colors::pink)
        .set_thumbnail("https://github.com/Civermau/Country-Mentioned-Bot-Cpp-Edition/blob/master/Images/Mini.jpg?raw=true")
        .set_image("https://github.com/Civermau/Country-Mentioned-Bot-Cpp-Edition/blob/master/Images/Mega.jpg?raw=true");
    
    // Add fields for each top country
    if (topCountries.size() >= 1) {
        infoEmbed.add_field("Most mentioned country:", "**" + topCountries[0].name + "** with " + topCountries[0].mentions + " mentions!");
    }
    
    if (topCountries.size() >= 2) {
        infoEmbed.add_field("Second place:", "**" + topCountries[1].name + "** (" + topCountries[1].mentions + ")", true);
    }
    
    if (topCountries.size() >= 3) {
        infoEmbed.add_field("Third place:", "**" + topCountries[2].name + "** (" + topCountries[2].mentions + ")", true);
    }
    
    // If no countries have been mentioned yet
    if (topCountries.empty()) {
        infoEmbed.add_field("No countries mentioned yet!", "Start mentioning countries to see the leaderboard!");
    }
    
    infoEmbed.set_footer(dpp::embed_footer().set_text("Bot created by Civer_mau"));
    return infoEmbed;
}

std::string loadBotToken() {
    std::string token;
    std::ifstream env_file(".env");
    if (env_file.is_open()) {
        std::string line;
        std::getline(env_file, line);
        token = line.substr(10);
        env_file.close();
        return token;
    }

    env_file.close();
    return token;
}

void saveBotToken(const std::string& token) {
    std::ofstream env_file(".env");
    if (!env_file.is_open()) {
        std::cout << "Error: Could not create .env file" << std::endl;
        return;
    }

    env_file.clear();
    env_file.seekp(0);

    env_file << "BOT_TOKEN=" << token;
    env_file.close();
    std::cout << "Bot token saved to .env file" << std::endl;
}