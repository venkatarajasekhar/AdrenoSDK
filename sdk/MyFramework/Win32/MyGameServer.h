
#pragma once

class GameServer 
{
public:

	GameServer();

	void init();

	void beginUserInitiatedSignIn();
	void signOut();
	void unlockAchievement(const char *achievementId);
	void submitHighScore(const char *leaderboardId, int score);
	void showAchievements();
	void showLeaderboard(const char *leaderboardId);

private:
};