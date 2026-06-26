#include "database.h"
#include "embeds.h"
#include "helpers.h"
#include "types.h"
#include <dpp/dpp.h>
#include <iostream>

int main() {
  std::string BOT_TOKEN = loadBotToken();

  while (BOT_TOKEN.empty()) {
    char buffer[100];
    std::cout << "BOT_TOKEN is empty, please provide a token" << std::endl;
    std::cout << "Token: ";
    std::cin.getline(buffer, 100);
    BOT_TOKEN = buffer;
    saveBotToken(BOT_TOKEN);
  }

  std::cout << "[DB]  Initializing database..." << std::endl;
  initDatabase();

  std::cout << "[DB]  Loading countries from db..." << std::endl;
  std::vector<CountryData> countries = getCountries();
  std::cout << "[DB]  Countries loaded" << std::endl;

  dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);
  bot.on_log(dpp::utility::cout_logger());

  // ! ----------------------------------------
  // ! ---------------------------------------- ON MESSAGE
  // ! ----------------------------------------

  bot.on_message_create([&bot, &countries](const dpp::message_create_t &event) {
    if (event.msg.author.id == bot.me.id)
      return;

    // std::cout << event.msg.content << std::endl;

    std::string uppercaseMessage = toUpperUTF8(event.msg.content);

    dpp::message m(event.msg.channel_id, "");
    dpp::message emptyMessage(event.msg.channel_id, "");
    int filesAdded = 0;

    for (const auto &country : countries) {
      if (uppercaseMessage.contains(country.en_name) ||
          uppercaseMessage.contains(country.es_name)) {
        m.add_file(
            country.en_name + ".jpg",
            dpp::utility::read_file("Images/" + country.en_name + ".jpg"));

        addMentionCounter(country.id);
        addUserMentionCounter(event.msg.author.id, country.id);
        addGuildMentionCounter(event.msg.guild_id, country.id);

        if (++filesAdded == 10) {
          event.reply(m);
          m = emptyMessage;
          filesAdded = 0;
        }
      }
    }
    if (filesAdded > 0)
      event.reply(m);
  });

  // ! ----------------------------------------
  // ! ---------------------------------------- ON SLASH COMMAND
  // ! ----------------------------------------
  bot.on_slashcommand([&bot](const dpp::slashcommand_t &event) {
    // * --------------------------------------
    // * -------------------------------------- GLOBAL INFO COMMAND
    // * --------------------------------------
    if (event.command.get_command_name() == "info") {
      std::vector<TopCountryData> topCountries =
          getTopThreeMentionedCountries();
      event.reply(dpp::message(event.command.channel_id, buildInfoEmbed(topCountries)));
    }

    // * --------------------------------------
    // * -------------------------------------- GUILD INFO COMMAND
    // * --------------------------------------
    else if (event.command.get_command_name() == "server_info") {
      if (event.command.guild_id == 0) {
        event.reply(dpp::message("This command can only be used in a server!"));
        return;
      }
      dpp::snowflake guild_id = event.command.guild_id;
      std::vector<TopCountryData> topGuildCountries = getGuildTopThreeMentionedCountries(guild_id);
      event.reply(dpp::message(event.command.channel_id, buildUserInfoEmbed(topGuildCountries)));
    }

    // * --------------------------------------
    // * -------------------------------------- USER INFO COMMAND
    // * --------------------------------------
    else if (event.command.get_command_name() == "user_info") {
      auto user_param = event.get_parameter("user");

      if (std::holds_alternative<dpp::snowflake>(user_param)) {
        int user_id = std::get<dpp::snowflake>(user_param);

        if (std::get<dpp::snowflake>(user_param) == bot.me.id) {
          event.reply(
              dpp::message("I cannot mention countries you silly potato"));
          return;
        }

        std::vector<TopCountryData> topUserCountries =
            getUserTopThreeMentionedCountries(user_id);
        event.reply(dpp::message(event.command.channel_id, buildUserInfoEmbed(topUserCountries)));

      } else {
        std::vector<TopCountryData> topUserCountries =
            getUserTopThreeMentionedCountries(event.command.usr.id);
        event.reply(dpp::message(event.command.channel_id,
                                 buildUserInfoEmbed(topUserCountries)));
      }
    }
  });

  // ! ----------------------------------------
  // ! ----------------------------------------
  // ! ----------------------------------------

  bot.on_guild_create([](const dpp::guild_create_t &event) {
    std::cout << "[DEBUG]  Guild: " << event.created.name
              << " (ID: " << event.created.id << ")\n";
  });

  // ! ----------------------------------------
  // ! ---------------------------------------- ON READY
  // ! ----------------------------------------
  bot.on_ready([&bot](const dpp::ready_t &event) {
    if (dpp::run_once<struct register_bot_commands>()) {

      bot.global_command_create(
          dpp::slashcommand("info", "Get info about the bot", bot.me.id));

      dpp::slashcommand user_cmd("user_info", "Get information about a user",
                                 bot.me.id);

      user_cmd.add_option(dpp::command_option(dpp::co_user, "user",
                                              "User to inspect",
                                              false)); // optional

      bot.global_command_create(user_cmd);

      bot.global_command_create(dpp::slashcommand(
          "server_info", "Get information about this server", bot.me.id));

      std::cout << "[DEBUG] Commands registered\n";
    }
  });
  bot.start(dpp::st_wait);
}