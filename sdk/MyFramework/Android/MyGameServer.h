
#pragma once

#ifdef __OBJC__
#include <objc/NSObjCRuntime.h>
#endif

#include "gpg/achievement.h"
#include "gpg/achievement_manager.h"
#include "gpg/builder.h"
#include "gpg/debug.h"
#include "gpg/default_callbacks.h"
#include "gpg/game_services.h"
#include "gpg/leaderboard.h"
#include "gpg/leaderboard_manager.h"
#include "gpg/platform_configuration.h"
#include "gpg/player_manager.h"
#include "gpg/score_page.h"
#include "gpg/types.h"

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