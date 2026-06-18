#include "database.h"
#include <iostream>
#include <sqlite3.h>

void initDatabase() {
  sqlite3 *db;
  sqlite3_open("CountryMentionedBot.db", &db);

  const char *sql = "CREATE TABLE IF NOT EXISTS countries ("
                    "   id INTEGER PRIMARY KEY,"
                    "   en TEXT NOT NULL,"
                    "   es TEXT NOT NULL"
                    ");"

                    "CREATE TABLE IF NOT EXISTS mentions ("
                    "   id INTEGER PRIMARY KEY,"
                    "   mentions INTEGER NOT NULL DEFAULT 0,"
                    "   FOREIGN KEY (id) REFERENCES countries(id)"
                    ");"


                    "CREATE TABLE IF NOT EXISTS guild_mentions ("
                    "   id INTEGER NOT NULL,"
                    "   guild INTEGER NOT NULL,"
                    "   mentions INTEGER NOT NULL DEFAULT 0,"
                    "   PRIMARY KEY (id, guild),"
                    "   FOREIGN KEY (id) REFERENCES countries(id)"
                    ");"

                    "CREATE TABLE IF NOT EXISTS user_mentions ("
                    "   id INTEGER NOT NULL,"
                    "   user_id INTEGER NOT NULL,"
                    "   mentions INTEGER NOT NULL DEFAULT 0,"
                    "   PRIMARY KEY (id, user_id),"
                    "   FOREIGN KEY (id) REFERENCES countries(id)"
                    ");";

  char *errMsg = nullptr;
  if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
    std::cerr << "[DB] Error initializing database: " << errMsg << std::endl;
    sqlite3_free(errMsg);
  } else {
    std::cout << "[DB] Database initialized" << std::endl;
  }

  sqlite3_close(db);
}

  // ! ----------------------------------------
  // ! ---------------------------------------- GLOBAL MENTIONS
  // ! ----------------------------------------
void addMentionCounter(const int &id) {
  sqlite3 *db;
  sqlite3_open("CountryMentionedBot.db", &db);
  sqlite3_stmt *stmt;
  const char *sql = "INSERT INTO mentions (id, mentions) VALUES (?, 1) ON "
                    "CONFLICT(id) DO UPDATE SET mentions = mentions + 1";
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  }
  sqlite3_close(db);
}

  // ! ----------------------------------------
  // ! ---------------------------------------- USER MENTIONS
  // ! ----------------------------------------
void addUserMentionCounter(const int &userId, const int &countryId){
  sqlite3 *db;
  sqlite3_open("CountryMentionedBot.db", &db);
  sqlite3_stmt *stmt;
  const char *sql = "INSERT INTO user_mentions (id, user_id, mentions) VALUES (?, ?, 1) ON "
                    "CONFLICT(id, user_id) DO UPDATE SET mentions = mentions + 1";

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
    sqlite3_bind_int(stmt, 1, countryId);
    sqlite3_bind_int(stmt, 2, userId);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  }
  sqlite3_close(db);
}

  // ! ----------------------------------------
  // ! ---------------------------------------- GUILD MENTIONS
  // ! ----------------------------------------
void addGuildMentionCounter(const int &guildId, const int &countryId){
  sqlite3 *db;
  sqlite3_open("CountryMentionedBot.db", &db);
  sqlite3_stmt *stmt;
  const char *sql = "INSERT INTO guild_mentions (id, guild, mentions) VALUES (?, ?, 1) ON "
                    "CONFLICT(id, guild) DO UPDATE SET mentions = mentions + 1";

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
    sqlite3_bind_int(stmt, 1, countryId);
    sqlite3_bind_int(stmt, 2, guildId);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  }
  sqlite3_close(db);
}



std::vector<TopCountryData> getTopThreeMentionedCountries() {
  std::vector<TopCountryData> topCountries;
  sqlite3 *db;
  sqlite3_open("CountryMentionedBot.db", &db);
  sqlite3_stmt *stmt;
  const char *sql =
      "SELECT c.en, m.mentions FROM countries c INNER JOIN mentions m ON c.id "
      "= m.id ORDER BY m.mentions DESC LIMIT 3";
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      TopCountryData country;
      const unsigned char *name_result = sqlite3_column_text(stmt, 0);
      if (name_result)
        country.name = reinterpret_cast<const char *>(name_result);
      const unsigned char *mentions_result = sqlite3_column_text(stmt, 1);
      if (mentions_result)
        country.mentions = reinterpret_cast<const char *>(mentions_result);
      topCountries.push_back(country);
    }
    sqlite3_finalize(stmt);
  }
  sqlite3_close(db);
  return topCountries;
}



std::vector<TopCountryData> getUserTopThreeMentionedCountries() { //   ULTRA MEGA PENDING, FIX THIS I DON'T HAVE A WHERE YET
  std::vector<TopCountryData> topUserCountries;
  sqlite3 *db;
  sqlite3_open("CountryMentionedBot.db", &db);
  sqlite3_stmt *stmt;
  const char *sql = "SELECT c.en, um.mentions FROM countries c INNER JOIN user_mentions um ON c.id = um.id ORDER BY um.mentions DESC LIMIT 3";

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      TopCountryData country;
      const unsigned char *name_result = sqlite3_column_text(stmt, 0);
      if (name_result)
        country.name = reinterpret_cast<const char *>(name_result);
      const unsigned char *mentions_result = sqlite3_column_text(stmt, 1);
      if (mentions_result)
        country.mentions = reinterpret_cast<const char *>(mentions_result);
      topUserCountries.push_back(country);
    }
    sqlite3_finalize(stmt);
  }
  sqlite3_close(db);
  return topUserCountries;
}

// // std::vector<TopCountryData> getGuildTopThreeMentionedCountries() 







std::vector<CountryData> getCountries() {
  std::vector<CountryData> countries;
  sqlite3 *db;
  sqlite3_open("CountryMentionedBot.db", &db);
  sqlite3_stmt *stmt;
  const char *sql = "SELECT id, en, es FROM countries";
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      CountryData country;
      country.id = sqlite3_column_int(stmt, 0);
      const unsigned char *en_result = sqlite3_column_text(stmt, 1);
      if (en_result)
        country.en_name = reinterpret_cast<const char *>(en_result);
      const unsigned char *es_result = sqlite3_column_text(stmt, 2);
      if (es_result)
        country.es_name = reinterpret_cast<const char *>(es_result);
      countries.push_back(country);
    }
    sqlite3_finalize(stmt);
  }
  sqlite3_close(db);
  return countries;
}