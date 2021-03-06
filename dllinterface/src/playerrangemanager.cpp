#include "playerrangemanager.h"

#include "logger.h"

PlayerRangeManager* PlayerRangeManager::playerRangeManager = 0;

PlayerRangeManager::PlayerRangeManager()
    : playerRanges() 
{
}

PlayerRangeManager::~PlayerRangeManager()
{
}

PlayerRangeManager& PlayerRangeManager::getPlayerRangeManager()
{
    if (!playerRangeManager)
    {
        playerRangeManager = new PlayerRangeManager();
    }

    return *playerRangeManager;
}

void PlayerRangeManager::resetRanges(GameStateManager& gameState)
{
	Logger& logger = Logger::getLogger(LOGGER_TYPE::DLL_INTERFACE_LOGGER);
	logger.logExp("[Resetting player ranges]", LOGGER_TYPE::DLL_INTERFACE_LOGGER);

    playerRanges.clear();

    PlayerRange pr;
	pr = pr.create100();
    for (int idx = 0; idx < 6; ++idx)
    {
		pr.setId(idx);
		pr.setValid(true);
        playerRanges.push_back(pr);
    }

}

PlayerRange PlayerRangeManager::getPlayerRange(int pos)
{
    return playerRanges[pos];
}

void PlayerRangeManager::setPlayerRange(int pos, PlayerRange& pr)
{
    playerRanges[pos] = pr;
}

vector<PlayerRange> PlayerRangeManager::getPlayerRanges()
{
    return playerRanges;
}

vector<PlayerRange> PlayerRangeManager::getOpponentRanges()
{
	vector<PlayerRange> res;
	for (int idx = 1; idx < 6; ++idx)
	{
		res.push_back(playerRanges[idx]);
	}
	return res;
}