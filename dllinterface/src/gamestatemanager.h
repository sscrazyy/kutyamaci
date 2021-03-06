#pragma once

#include "Action.h"
#include "Hand.h"
#include <vector>
#include <string>
#include "currentPlayerInfo.h"
#include "currentGameInfo.h"
#include "database.h"

using namespace std;

class GameStateManager
{
private:

	double handNumber;
	double myStackSize;

    int dealerPosition;
    int bettingRound;
    Hand lastHand;
	int dealtbits;

	bool firstHand;

    //vector<double> currentBets;
    vector<string> players;

    vector<CurrentPlayerInfo> currentPlayerInfos;
    vector<bool> isCurrentPlayerInfosSet;

    vector<double> balance;
    double maxRaiseSize;
    CurrentGameInfo* currentGameInfo;

	Action actionCache;
	bool cacheAvalaible;
	bool bluff;

	Database* database;


    int nextPosition(int position);

    GameStateManager();
    ~GameStateManager();

    static GameStateManager* gameStateManager;

public:

    static GameStateManager& getGameStateManager();

    bool IsHandReset(Hand hand);
	bool IsHandReset(double handNumber);

    void resetCurrentBets();

    void resetState(int dealerPos, Hand hand);
    void resetBettingRound();

    void nextBettingRound();
    int getBettingRound();

    void setDealerPosition(int dealerPosition);
    int  getDealerPosition();

    void setHand(Hand hand);
    Hand getHand();

    double getCurrentBetByPos(int idx);
    void setCurrentBet(int idx, double currentBet);

    void setPlayer(string player, int idx);
    string getPlayerNameByPos(int idx);

    CurrentPlayerInfo& getCurrentPlayerInfo(int pos);
    void setCurrentPlayerInfo(int pos, CurrentPlayerInfo currentPlayerInfo);
    bool isCurrentPlayerInfoSet(int pos);

    void setInitialBalance(int pos, double balance);
    double getInitialBalanceByPos(int pos);

    double getMaxRaise();
    void setMaxRaise(double);

    void setCurrentGameInfo(CurrentGameInfo*);
    CurrentGameInfo* getCurrentGameInfo();

	void setAction(Action action);
	Action getAction();

	void setCache(bool);
	bool isCacheAvalaible();

	void setHandNumber(double);
	double getHandNumber();

	void setMyStackSize(double);
	double getMyStackSize();

	void setBluff(bool);
	bool isBluff();

	void setFirstHand(bool);
	bool isFirstHand();

	void setBits(int);
	bool isBitSet(int);
	void clearBits();

	Database* getDatabase();
};