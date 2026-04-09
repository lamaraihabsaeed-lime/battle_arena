#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>
#include <vector>

struct ScoreEntry {
    std::string playerName;
    int score;
    int level;
};

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager() = default;

    // Score management
    bool saveScore(const std::string& playerName, int score, int level);
    std::vector<ScoreEntry> getTopScores(int limit = 10);
    bool initializeDatabase();

private:
    // For demo purposes, we'll use in-memory storage
    std::vector<ScoreEntry> scores;
};

#endif // DATABASEMANAGER_H
