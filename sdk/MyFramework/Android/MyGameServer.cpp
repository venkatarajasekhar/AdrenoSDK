
#include "MyGameServer.h"
#include "MyUtils.h"

#include <android_native_app_glue.h>

//========================================================================================================================
//
// Global
//
//========================================================================================================================

extern android_app* g_androidApp;

//========================================================================================================================
//
// GameServer helpers
//
//========================================================================================================================

void GameServer::initServices(
	gpg::PlatformConfiguration const &pc,
	gpg::GameServices::Builder::OnAuthActionStartedCallback started_callback,
	gpg::GameServices::Builder::OnAuthActionFinishedCallback finished_callback)
{
	if (!m_game_services)
	{
		m_game_services = gpg::GameServices::Builder()
			.SetLogging(gpg::DEFAULT_ON_LOG, gpg::LogLevel::VERBOSE)
			.SetOnAuthActionStarted([this, started_callback](gpg::AuthOperation op)
		{
			m_is_auth_in_progress = true;
			if (started_callback != nullptr)
				started_callback(op);
		})
			.SetOnAuthActionFinished([this, finished_callback](gpg::AuthOperation op, gpg::AuthStatus status)
		{
			smartLog("Sign in finished with a result of " + toString((int)status));
		
			m_is_auth_in_progress = false;
			if (finished_callback != nullptr)
				finished_callback(op, status);

			// Fetching all blocking
			gpg::AchievementManager::FetchAllResponse fetchResponse =
				m_game_services->Achievements().FetchAllBlocking(std::chrono::milliseconds(1000));

			// Fetching all nonblocking
			m_game_services->Achievements().FetchAll(
				gpg::DataSource::CACHE_OR_NETWORK,
				[](gpg::AchievementManager::FetchAllResponse response)
			{
				// After fetching all nonblocking, do something ...
			});
		})
			.Create(pc);
	}
}

//========================================================================================================================
//
// GameServer class
//
//========================================================================================================================

GameServer::GameServer()
	: m_is_auth_in_progress(false)
{
}

void GameServer::init()
{
	throwIfFailed((g_androidApp != nullptr), "Please assign g_androidApp in android_main function.");

	auto callback = [](gpg::AuthOperation op, gpg::AuthStatus status)
	{
		// After successfully authorizing, do something ...
	};

	// gpg-cpp: Set platform intiialization
	gpg::AndroidInitialization::android_main(g_androidApp);

	// Get the platform configuration.
	gpg::AndroidPlatformConfiguration platform_configuration;
	platform_configuration.SetActivity(g_androidApp->activity->clazz);

	initServices(platform_configuration, NULL, callback);
}

void GameServer::beginUserInitiatedSignIn()
{
	if (!m_game_services->IsAuthorized())
	{
		m_game_services->StartAuthorizationUI();
	}
}

void GameServer::signOut() 
{
	if (m_game_services->IsAuthorized())
	{
		m_game_services->SignOut();
	}
}

void GameServer::unlockAchievement(char const *achievement_id)
{
	if (m_game_services->IsAuthorized())
	{
		m_game_services->Achievements().Unlock(achievement_id);
	}
}

void GameServer::submitHighScore(char const *leaderboard_id, uint64_t score)
{
	if (m_game_services->IsAuthorized())
	{
		m_game_services->Leaderboards().SubmitScore(leaderboard_id, score);
	}
}

void GameServer::showAchievements()
{
	if (m_game_services->IsAuthorized())
	{
		m_game_services->Achievements().ShowAllUI(
			[](gpg::UIStatus status)
			{}
		);
	}
}

void GameServer::showLeaderboard(char const *leaderboard_id)
{
	if (m_game_services->IsAuthorized())
	{
		m_game_services->Leaderboards().ShowUI(
			leaderboard_id,
			[](gpg::UIStatus status)
			{}
		);
	}
}

// Getter

gpg::GameServices* GameServer::getGameServices()
{
	return m_game_services.get();
}

bool GameServer::isAuthInProgress()
{
	return m_is_auth_in_progress;
}