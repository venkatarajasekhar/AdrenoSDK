
#pragma once

#ifdef __OBJC__
#include <objc/NSObjCRuntime.h>
#endif

#include "gpg/gpg.h"

class GameServer 
{
public:

	GameServer();

	void init();

	void beginUserInitiatedSignIn();
	void signOut();
	void unlockAchievement(const char *achievementId);
	void submitHighScore(const char *leaderboardId, uint64_t score);
	void showAchievements();
	void showLeaderboard(const char *leaderboardId);

	// Getter

	gpg::GameServices* getGameServices();
	bool isAuthInProgress();

private:
	void initServices(
		gpg::PlatformConfiguration const &pc,
		gpg::GameServices::Builder::OnAuthActionStartedCallback started_callback,
		gpg::GameServices::Builder::OnAuthActionFinishedCallback finished_callback);

private:
	bool m_is_auth_in_progress;
	std::unique_ptr<gpg::GameServices> m_game_services;
};