# Country Mentioned Bot - C++ Edition

Made one in C#, why not make another one in C++?

Again, I found really funny having a bot on servers that could automatically detect countries in messages and send the respective image. But this time using sqlite to keep track of the mentions and display them in a cute way!

<div align="center">
   <img width="551" height="527" alt="image" src="https://github.com/user-attachments/assets/1252320a-a70b-4755-98b2-7a5a58479c75" />
</div>

## 🌟 Features

- **Automatic Country Detection**: Detects mentions of countries in both English and Spanish
- **Flag Image Responses**: Automatically sends corresponding country flag images when countries are mentioned
- **Mention Tracking**: Keeps track of how many times each country has been mentioned
- **Leaderboard System**: `/info` command shows the top 3 most mentioned countries
- **Multi-language Support**: Currently supports English and Spanish country names
- **SQLite Database**: Persistent storage for country data and mention statistics

## 🚀 Want to compile it yourself?

### Prerequisites

- C++ compiler with C++17 support
- [DPP (Discord++ Library)](https://github.com/brainboxdotcc/DPP)
- SQLite3 development libraries

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/Civermau/Country-Mentioned-Bot-Cpp-Edition.git
   cd Country-Mentioned-Bot-Cpp-Edition
   ```

2. **Install Dependencies**
   
   **On Arch Linux:**
   ```bash
   sudo pacman -S sqlite3 dpp 
   ```
   (Installing dpp gave me a LOT of troubles on [nyarch](https://github.com/Nyarchlinux)  [please don't ask, that's actually the distro I use], had to use `makepkg -si --noconfirm --skippgpcheck` to make it work)
   
   Too lazy to explain the others, just check [official installing guides](https://dpp.dev/installing.html)

3. **Create a Discord Bot**
   - Go to [Discord Developer Portal](https://discord.com/developers/applications)
   - Create a new application
   - Go to the "Bot" section and click reset token
   - Copy the bot token and place it on your .env file

4. **Build the Project**
   
   Ngl, I hate Cmake and makefiles, i just 
   ```bash
   clear ; g++ -std=c++23 -o ./build/bot-uwu main.cpp -ldpp -lsqlite3 && ./build/bot-uwu
   ```
   (You should probably make a proper makefile... and cpp 23 was to use contains() on strings)
5. **Run the Bot**
   ```bash
   #Give it a proper name too, don't use bot uwu
   ./CountryMentionedBot
   ```

### Or if you are lazy, you can just invite it to your own server!
[click here, come on, it's not a virus](https://discord.com/oauth2/authorize?client_id=1311875601640194240&permissions=101440&integration_type=0&scope=bot)
   
## 🤖 Bot Commands

- **Automatic Responses**: The bot automatically detects country mentions and responds with flag images
- **`/info`**: Shows an embed with the top 3 most mentioned countries and their mention counts

## 📊 Database Schema

The bot uses SQLite with two main tables:

### `countries` table
- `id` (INTEGER PRIMARY KEY): Unique country identifier
- `en` (TEXT): English country name
- `es` (TEXT): Spanish country name

### `mentions` table
- `id` (INTEGER): Foreign key to countries table
- `mentions` (INTEGER): Number of times the country has been mentioned

There's already a .db file in the repo, it has the countries loaded and an empty mentions table
## 🖼️ Image Requirements

The bot expects country flag images in the `Images/` directory with the following naming convention:
- Filename: `COUNTRY_NAME.jpg` (e.g., `MEXICO.jpg`, `FRANCE.jpg`)
- Format: JPG
- The bot includes 200+ country flag images covering most recognized countries

# IMPORTANT
All of the country mentioned pictures, every single one of them, where made by [Tangermusic](https://linktr.ee/Tangermusic) (I still need to ask permission to upload them, if there are no images in the repo, I didn't got the permission unu)

<img width="375" height="5000" alt="image" src="https://github.com/user-attachments/assets/bda51e10-1610-4f74-8d1d-ed514e91983d" />

https://x.com/tangermusic/status/1652837806484385795

## 🔧 Configuration

### Environment Variables
- `BOT_TOKEN`: Your Discord bot token (automatically managed by the bot)

### Bot Permissions
The bot requires the following permissions:
- View channels
- Send Messages
- Attach files
- Read Message History
- Add reactions (MAYBE, I'll use this later)

Don't worry, I won't spy your server's conversations... or I will...?

## 📄 License
As all of my works, this project goes by [this license](https://creativecommons.org/licenses/by-nc-sa/4.0/), check [LICENSE](LICENSE) for more details

You are free to download, modify and distribute this code, ALWAYS FOR FREE, WITH THE SAME LICENSE

## 🙏 Acknowledgments

- [DPP (Discord++)](https://github.com/brainboxdotcc/DPP) - C++ Discord API library
- Discord for providing the API
- [Tangermusic](https://linktr.ee/Tangermusic) (cool music!)
- And you, for using this (Yeah, I wanted to add the cliche gratitude)
