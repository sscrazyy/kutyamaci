#pragma once

#include "AbstractBotLogic.h"
#include "bayesUserFunctions.H"
#include "database.h"
#include "bayesDecision.h"
#include "simpleDecision.h"
#include "globalConst.h"

class PlusEVBotLogic : public AbstractBotLogic //implements interface
{
private:
	BayesUserPreflop preflop;
	BayesUserFlop flop;
	BayesUserTurn turn;
	BayesUserRiver river;
	Database* database;

	CurrentPlayerInfo setStandardPlayerType(CurrentPlayerInfo&, double);
	void extendGameInfo(CurrentGameInfo&);
	bool preflopOpenRaise(CurrentGameInfo&);

public:
	PlusEVBotLogic();
	virtual ~PlusEVBotLogic();

    virtual PlayerRange calculateRange(int id, CurrentGameInfo&, PlayerRange&);
    virtual Action makeDecision(CurrentGameInfo&, vector<PlayerRange>&);
};