#include "embeds.h"

dpp::embed buildInfoEmbed(const std::vector<TopCountryData>& topCountries) {
    dpp::embed infoEmbed =
        dpp::embed()
            .set_author("Civer_mau", "https://github.com/Civermau",
                        "https://avatars.githubusercontent.com/u/66493296")
            .set_title("Country Mentioned Bot!")
            .set_url("https://github.com/Civermau/Country-Mentioned-Bot-Cpp-Edition")
            .set_description("Let Yui inform you that you've just mentioned a country!")
            .set_color(dpp::colors::pink)
            .set_thumbnail(
                "https://github.com/Civermau/Country-Mentioned-Bot-Cpp-Edition/"
                "blob/master/Images/Mini.jpg?raw=true")
            .set_image(
                "https://github.com/Civermau/Country-Mentioned-Bot-Cpp-Edition/"
                "blob/master/Images/Mega.jpg?raw=true");

    if (topCountries.size() >= 1)
        infoEmbed.add_field("Most mentioned country:",
                            "**" + topCountries[0].name + "** with " + topCountries[0].mentions + " mentions!");
    if (topCountries.size() >= 2)
        infoEmbed.add_field("Second place:",
                            "**" + topCountries[1].name + "** (" + topCountries[1].mentions + ")", true);
    if (topCountries.size() >= 3)
        infoEmbed.add_field("Third place:",
                            "**" + topCountries[2].name + "** (" + topCountries[2].mentions + ")", true);
    if (topCountries.empty())
        infoEmbed.add_field("No countries mentioned yet!",
                            "Start mentioning countries to see the leaderboard!");

    infoEmbed.set_footer(dpp::embed_footer().set_text("Bot created by Civer_mau"));
    return infoEmbed;
}

dpp::embed buildUserInfoEmbed(const std::vector<TopCountryData>& countries) {
    dpp::embed userInfoEmbed =
        dpp::embed()
            .set_author("Civer_mau", "https://github.com/Civermau",
                        "https://avatars.githubusercontent.com/u/66493296")
            .set_title("Country Mentioned Bot!")
            .set_url("https://github.com/Civermau/Country-Mentioned-Bot-Cpp-Edition")
            .set_color(dpp::colors::pink)
            .set_thumbnail(
                "https://github.com/Civermau/Country-Mentioned-Bot-Cpp-Edition/"
                "blob/master/Images/Mini.jpg?raw=true")
            .set_image(
                "https://github.com/Civermau/Country-Mentioned-Bot-Cpp-Edition/"
                "blob/master/Images/Mega.jpg?raw=true");

    if (countries.size() >= 1)
        userInfoEmbed.add_field("Most mentioned country:",
                            "**" + countries[0].name + "** with " + countries[0].mentions + " mentions!");
    if (countries.size() >= 2)
        userInfoEmbed.add_field("Second place:",
                            "**" + countries[1].name + "** (" + countries[1].mentions + ")", true);
    if (countries.size() >= 3)
        userInfoEmbed.add_field("Third place:",
                            "**" + countries[2].name + "** (" + countries[2].mentions + ")", true);
    if (countries.empty())
        userInfoEmbed.add_field("No countries mentioned yet!",
                            "Start mentioning countries to see the leaderboard!");

    userInfoEmbed.set_footer(
        dpp::embed_footer().set_text("Bot created by Civer_mau")
    );

    return userInfoEmbed;
}