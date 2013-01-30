#include "bayesDecision.h"	
#include "eqcalculator.h"
#include "logger.h"
#include "evaluator.h"
#include <sstream>

bool inPosition(int poz1, int poz2)
{
	if (poz1 == 1) poz1 = -5;
	if (poz1 == 2) poz1 = -4;
	if (poz2 == 1) poz2 = -5;
	if (poz2 == 2) poz2 = -4;

	return poz1 > poz2;
}
bool heroInPosition(CurrentGameInfo& gameInfo)
{
	int myPoz = gameInfo.getHero().getPoz();
	bool inPoz = true;
	for (int i = 0; i < gameInfo.getOpponentsInfo().size(); ++i)
	{
		int poz = gameInfo.getOpponentsInfo()[i].getPoz();
		if (!inPosition(myPoz, poz))
		{
			inPoz = false;
		}
	}

	return inPoz;
}
double maxOpponentStack(CurrentGameInfo& game)
{
	double maxbet = game.getBiggestBet();
	double maxStackSize = 0;
	for (int i = 0; i < game.getOpponentsInfo().size(); ++i)
	{
		CurrentPlayerInfo player = game.getOpponentsInfo()[i];
		double currentbet = maxbet - player.getBetsize();
		double actualStack = player.getActualStacksize() - currentbet;

		if (maxStackSize < actualStack) maxStackSize = actualStack;  
	}

	return maxStackSize;
}
double maxOpponentTotalStack(CurrentGameInfo& game)
{
	double maxbet = game.getBiggestBet();
	double maxStackSize = 0;
	for (int i = 0; i < game.getOpponentsInfo().size(); ++i)
	{
		CurrentPlayerInfo player = game.getOpponentsInfo()[i];
		double actualStack = player.getActualStacksize() + player.getBetsize();
		if (maxStackSize < actualStack) maxStackSize = actualStack;  
	}

	return maxStackSize;
}
double maxOpponentOriginalStack(CurrentGameInfo& game)
{
	double maxbet = game.getBiggestBet();
	double maxStackSize = 0;
	for (int i = 0; i < game.getOpponentsInfo().size(); ++i)
	{
		CurrentPlayerInfo player = game.getOpponentsInfo()[i];
		double actualStack = player.getStacksize();
		if (maxStackSize < actualStack) maxStackSize = actualStack;  
	}

	return maxStackSize;
}
double getMaxRaise(CurrentGameInfo& gameInfo)
{
	double betsize = 0;
	for (int i = 0; i < gameInfo.getOpponentsInfo().size(); ++i)
	{
		CurrentPlayerInfo& player = gameInfo.getOpponentsInfo()[i];
		if (player.getBetsize() > betsize)
			betsize = player.getBetsize();
	}

	return betsize;
}
double BayesDecision::nrOfPlayersWithMaxBetSize(CurrentGameInfo& gameInfo)
{
	double result = 0;

	vector<CurrentPlayerInfo>& players = gameInfo.getOpponentsInfo();

	for (int i = 0; i < players.size(); ++i)
	{
		if (abs(players[i].getBetsize() - gameInfo.getBiggestBet()) < 0.01)
			result ++;
	}

	return result;
}

vector<double> BayesDecision::getRaiseSizes(CurrentGameInfo& gameInfo)
{
	vector<double> result;

	int str = Evaluator::cardStrength(gameInfo.getHand().getCard1(), gameInfo.getHand().getCard2(), gameInfo.getBoard());
	int maxOpponentStack = maxOpponentOriginalStack(gameInfo);	

	if (gameInfo.getStreet() == 0)
	{
		if (gameInfo.getBiggestBet() >= 10)
		{
			if (heroInPosition(gameInfo))
			{
				result.push_back( (gameInfo.getBiggestBet() * (2.2 + nrOfPlayersWithMaxBetSize(gameInfo) - 1)) * gameInfo.getBblind() );
			}
			else
			{
				result.push_back( (gameInfo.getBiggestBet() * (2.7 + nrOfPlayersWithMaxBetSize(gameInfo) - 1)) * gameInfo.getBblind() );
			}
		}
		else
		{
			if (heroInPosition(gameInfo))
				result.push_back( (gameInfo.getBiggestBet() * (2.5 + nrOfPlayersWithMaxBetSize(gameInfo) - 1)) * gameInfo.getBblind() );
			result.push_back( (gameInfo.getBiggestBet() * (3 + nrOfPlayersWithMaxBetSize(gameInfo) - 1)) * gameInfo.getBblind() );
			result.push_back( (gameInfo.getBiggestBet() * (3.5 + nrOfPlayersWithMaxBetSize(gameInfo) - 1)) * gameInfo.getBblind() );
		}
	}
	else if (gameInfo.getStreet() == 3 && gameInfo.getPotcommon() >= 35 && str >= 1 && maxOpponentStack > 80)
	{
		result.push_back( ( gameInfo.getPotcommon() * 1/2 ) * gameInfo.getBblind() );
	}
	else
	{
		if ( gameInfo.getBiggestBet() < gameInfo.getPotcommon() / 5 )
		{
			int boardType = Evaluator::boardType(gameInfo.getBoard());
			double potcommon = gameInfo.getPotcommon();

			if ((boardType == 0 && potcommon >= 17 && gameInfo.getStreet() == 1) || (boardType == 0 && potcommon >= 32 && gameInfo.getStreet() == 2))
			{
				result.push_back( ( gameInfo.getPotcommon() * 1/2 ) * gameInfo.getBblind() );
			}
			else if ((potcommon >= 17 && gameInfo.getStreet() == 1) || (potcommon >= 32 && gameInfo.getStreet() == 2))
			{
				result.push_back( ( gameInfo.getPotcommon() * 1/2 ) * gameInfo.getBblind() );
				result.push_back( ( gameInfo.getPotcommon() * 2/3 ) * gameInfo.getBblind() );
			}
			else
			{
				if (heroInPosition(gameInfo))
					result.push_back( ( gameInfo.getPotcommon() * 1/2 ) * gameInfo.getBblind() );
			
				result.push_back( ( gameInfo.getPotcommon() * 2/3 ) * gameInfo.getBblind() );
				result.push_back( ( gameInfo.getPotcommon() * 3/4 ) * gameInfo.getBblind() );
				if (boardType != 0) result.push_back( ( gameInfo.getPotcommon() ) * gameInfo.getBblind() );
			}
		}
		else
		{

				double correction = 0;
				if ( gameInfo.getBiggestBet() < gameInfo.getPotcommon() / 3 )
					correction = gameInfo.getBiggestBet();

				if (heroInPosition(gameInfo))
					result.push_back( (gameInfo.getBiggestBet() * (2.5 + nrOfPlayersWithMaxBetSize(gameInfo) - 1) + correction ) * gameInfo.getBblind() );

				result.push_back( (gameInfo.getBiggestBet() * (3 + nrOfPlayersWithMaxBetSize(gameInfo) - 1) + correction ) * gameInfo.getBblind() );
				result.push_back( (gameInfo.getBiggestBet() * (3.5 + nrOfPlayersWithMaxBetSize(gameInfo) - 1) + correction ) * gameInfo.getBblind() );
		
		}
		
		if (gameInfo.getStreet() == 3)
				result.push_back( gameInfo.getHero().getActualStacksize() * gameInfo.getBblind() );
	}

	return result;
}
PlayerRange BayesDecision::getCallRaiseRange(double betsize, PlayerRange& range, CurrentGameInfo& game, BayesUserPreflop& preflop, BayesUserFlop& flop, BayesUserTurn& turn, BayesUserRiver& river)
{
	Logger& logger = Logger::getLogger(BOT_LOGIC);
	PlayerRange totalRange;

	CurrentPlayerInfo player = game.getPlayerbyId(range.getId());
	double actualStackSize = player.getActualStacksize() + player.getBetsize();
	actualStackSize *= game.getBblind();
	if (betsize > actualStackSize) betsize = actualStackSize;

	if (game.getStreet() == 0)
	{
		totalRange = preflop.getCallRaiseRange(player.getVPIP(), player.getPFR(), player.getStacksize() * game.getBblind(), player.getPoz(), 0, betsize, game.getBblind(), 0, patternsNeeded);
	}

	if (game.getStreet() == 1)
	{
		totalRange = flop.getCallRaiseRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(), 0, betsize, game.getBblind(),
			game.getPotcommon() * game.getBblind(), game.getBoard(), game.getHand(), patternsNeeded);
	}

	if (game.getStreet() == 2)
	{
		totalRange = turn.getCallRaiseRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(), 0, betsize, game.getBblind(),
			game.getPotcommon() * game.getBblind(),  game.getFlopPotSize() * game.getBblind(), game.getBoard(), game.getHand(), patternsNeeded);
	}

	if (game.getStreet() == 3)
	{
		totalRange = river.getCallRaiseRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(), 0, betsize, game.getBblind(),
			game.getPotcommon() * game.getBblind(),  game.getFlopPotSize() * game.getBblind(), game.getBoard(), game.getHand(), patternsNeeded);
	}

	ostringstream s;
	s << "Betsize: " << betsize << " ";
	s << "VPIP: " << player.getVPIP() << " ";
	s << "PFR: " << player.getPFR() << " ";
	s << "AF: " << player.getAF() << " ";
	s << "StackSize: " << player.getStacksize() << " ";
	s << "bblind: " << game.getBblind() << " ";
	s << "potcommon: " << game.getPotcommon() << endl;

	s << "board: ";
	vector<Card> board = game.getBoard();
	for (int i = 0; i < board.size(); ++i)
	{
		s << board[i].toString();
	}

	s << endl << "hand: ";
	s << game.getHand().toString() << endl;

	logger.logExp(s.str(), BOT_LOGIC);

	totalRange.setId(range.getId());
	if (totalRange.range.size() == 0)
	{
		logger.logExp("Call raise range is 0", LOGGER_TYPE::BOT_LOGIC);
		return totalRange;
	}

	if (game.getStreet() != 0)
		totalRange = RangeUtils::mergeRange(range, totalRange, game.getBoard(), game.getHand());

	totalRange.setId(range.getId());
	if (totalRange.range.size() == 0)
	{
		logger.logExp("Merged range is 0", LOGGER_TYPE::BOT_LOGIC);
		return totalRange;
	}

	//s << "-----------------------------------------MERGED RANGE----------------------------------------" << endl << totalRange.toString() << endl << endl;

	totalRange.setId(range.getId());
	return totalRange;
}

double manipulateEQRaise(CurrentGameInfo& game)
{
}
double manipulateEQCall(CurrentGameInfo& game)
{
}
double manipluateFE(CurrentGameInfo& game)
{
}

double modifyValue(double x, double value)
{
	x += value;
	if (x < 0) x = 0;
	if (x > 1) x = 1;

	return x;
}
double BayesDecision::calculateEQ(vector<PlayerRange>& ranges, vector<Card>& board, Hand& hand, CurrentGameInfo& game)
{
	Logger& logger = Logger::getLogger(BOT_LOGIC);

	vector<PlayerRange> result;
	EqCalculator calc;
	PlayerRange myRange;
	myRange.range.insert(make_pair(hand, 1));

	result.push_back(myRange);

	logger.logExp("=============BEGIN EQ CALCULATION========================", RANGE_LOGGER);

	for (int i = 0; i < ranges.size(); ++i)
	{
		result.push_back(ranges[i]);
		logger.logExp("PLAYER NAME: " + game.getPlayerbyId(ranges[i].getId()).getName(), RANGE_LOGGER);
		logger.logExp("BOARD: ", RANGE_LOGGER);
		for (int j = 0; j < board.size(); ++j)
		{
			logger.logExp(board[i].toString(), RANGE_LOGGER);
		}

		logger.logExp("Total percentage of ranges: ", result[i + 1].totalPercentage(), BOT_LOGIC);
		logger.logExp(result[i + 1].toString(), RANGE_LOGGER);
		logger.logExp("=====================================================================", RANGE_LOGGER);
	}

	logger.logExp("Calculating EQ...", BOT_LOGIC);
	double eq = calc.calculate(result, board, 10000) / 100;
	logger.logExp("Calculated original EQ", eq, BOT_LOGIC);
	logger.logExp("=============END EQ CALCULATION========================", RANGE_LOGGER);
	eq = modifyValue(eq, -0.05);
	return eq;
}
vector<double> BayesDecision::getFoldEquities(double betsize, CurrentGameInfo& game, vector<PlayerRange>& ranges, BayesUserPreflop& preflop, BayesUserFlop& flop, BayesUserTurn& turn , BayesUserRiver& river)
{
	Logger& logger = Logger::getLogger(BOT_LOGIC);
	logger.logExp("============================CALCULATING FE's=====================", BOT_LOGIC);
	vector<double> result;

	if (game.getStreet() == 0)
	{
		for (int j = 0; j < ranges.size(); ++j)
		{
			CurrentPlayerInfo player = game.getPlayerbyId(ranges[j].getId());
			double akt = preflop.getProbabilityFE(player.getVPIP(), player.getPFR(), player.getStacksize() * game.getBblind(), player.getPoz(),
				player.getLine(), player.getBetsize() * game.getBblind(), game.getBblind(), game.getPotcommon() * game.getBblind(), patternsNeeded);

			result.push_back(akt);
		}
	}
	else if (game.getStreet() == 1)
	{
		for (int j = 0; j < ranges.size(); ++j)
		{
			CurrentPlayerInfo player = game.getPlayerbyId(ranges[j].getId());
			
			double akt = flop.getProbabilityFE(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(),
				player.getLine(), player.getBetsize() * game.getBblind(), game.getBblind(), game.getPotcommon() * game.getBblind(), patternsNeeded);

			result.push_back(akt);
		}
	}
	else if (game.getStreet() == 2)
	{
		for (int j = 0; j < ranges.size(); ++j)
		{
			CurrentPlayerInfo player = game.getPlayerbyId(ranges[j].getId());
			double akt = turn.getProbabilityFE(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(),
				player.getLine(), player.getBetsize() * game.getBblind(), game.getBblind(), game.getPotcommon() * game.getBblind(), game.getFlopPotSize() * game.getBblind(), patternsNeeded);

			result.push_back(akt);
		}
	}
	else if (game.getStreet() == 3)
	{
		for (int j = 0; j < ranges.size(); ++j)
		{
			CurrentPlayerInfo player = game.getPlayerbyId(ranges[j].getId());
			double akt = river.getProbabilityFE(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(),
				player.getLine(), player.getBetsize() * game.getBblind(), game.getBblind(), game.getPotcommon() * game.getBblind(), game.getFlopPotSize() * game.getBblind(), patternsNeeded);

			result.push_back(akt);
		}
	}

	logger.logExp("============================CALCULATED FE's=====================", BOT_LOGIC);
	for (int i = 0; i < result.size(); ++i)
	{
		logger.logExp("FE= ", result[i], BOT_LOGIC);
	}

	return result;
}
double BayesDecision::calculateEVRaise(CurrentGameInfo& gameInfo, vector<PlayerRange>& callRaiseRanges, vector<double>& foldEquities, double betsize, bool allIn = false)
{ 
	Logger& logger = Logger::getLogger(BOT_LOGIC);
	logger.logExp("===================Calculating EV of raise with betsize ", betsize, BOT_LOGIC);

	//STACK functions are FUCKED UP
	double stacksize = gameInfo.getHero().getActualStacksize() + gameInfo.getHero().getBetsize();
	double maxOpponentS = maxOpponentTotalStack(gameInfo);
	double effectiv = (stacksize < maxOpponentS ? stacksize : maxOpponentS);

	logger.logExp("Hero actualStackSize: ", gameInfo.getHero().getActualStacksize(), BOT_LOGIC);
	logger.logExp("HeroCurrentBet: ", gameInfo.getHero().getBetsize(), BOT_LOGIC);
	logger.logExp("Opponent maxStack: ", maxOpponentS, BOT_LOGIC);
	logger.logExp("effective stack ", effectiv, BOT_LOGIC);

	effectiv *= gameInfo.getBblind();
	if (betsize >= 0.4 * effectiv)
	{
		betsize = effectiv;
		allIn = true;
	}

	logger.logExp("Calculating EV of raise with betsize", betsize, BOT_LOGIC);


	logger.logExp("Hero's stacksize : ", stacksize, BOT_LOGIC);

	double ev = 0;

	int n = callRaiseRanges.size();

	double commonpot = gameInfo.getPotcommon() * gameInfo.getBblind();
	double heroCurrentBet = gameInfo.getHero().getBetsize() * gameInfo.getBblind();
	logger.logExp("Commonpot : ", commonpot, BOT_LOGIC);
	logger.logExp("Hero currentbet : ", heroCurrentBet, BOT_LOGIC);

	for (int bits = 0; bits < (1 << n); ++bits)
	{
		vector<PlayerRange> actualRanges;
		vector<CurrentPlayerInfo> playersPlaying;
		double chanceOfThis = 1;
		double nr = 0;
		// Includes common pot + currentbet of players whose actions are fold
		double pot = commonpot;

		double totalCalls = 0;

		for (int i = 0; i < n; ++i)
		{
			if (bits & (1 << i))
			{
				chanceOfThis *= 1 - foldEquities[i];
				actualRanges.push_back(callRaiseRanges[i]);

				CurrentPlayerInfo player  = gameInfo.getPlayerbyId(callRaiseRanges[i].getId());
				double stacksize = player.getActualStacksize() + player.getBetsize();
				stacksize *= gameInfo.getBblind();
				if (stacksize < betsize)
					totalCalls += stacksize;
				else
					totalCalls += betsize;
				playersPlaying.push_back(player);
				nr++;
			}
			else
			{
				chanceOfThis *= foldEquities[i];
				CurrentPlayerInfo player  = gameInfo.getPlayerbyId(callRaiseRanges[i].getId());

				pot += player.getBetsize() * gameInfo.getBblind();
			}
		}

		double eq = 1;
		
		//MANIPULATE EQ RAISE

		logger.logExp("Final EQ : ", eq, BOT_LOGIC);
		double thisEV = eq * (pot + totalCalls + heroCurrentBet) - (1 - eq) * (betsize - heroCurrentBet);
		thisEV -= (pot + totalCalls + betsize) * 0.05;

		logger.logExp("Total calls : ", totalCalls, BOT_LOGIC);
		logger.logExp("Chance of this situation : ", chanceOfThis, BOT_LOGIC);
		logger.logExp("EV of this situation : ", thisEV, BOT_LOGIC);

		ev += chanceOfThis * thisEV;
	}

	logger.logExp("===============Calculated EV : ", ev, BOT_LOGIC);

	return ev;
}

double BayesDecision::calculateEVCall(CurrentGameInfo& game, vector<PlayerRange>& ranges)
{
	Logger& logger = Logger::getLogger(LOGGER_TYPE::BOT_LOGIC);

	double EVCALL = 0;

	if (game.getStreet() == 0)
	{
		double totalpot = game.getTotalPot() * game.getBblind();
		double call = game.getAmountToCall() * game.getBblind();
		
		logger.logExp("START CALL EV CALCULATION\n", LOGGER_TYPE::BOT_LOGIC);

		double eq = calculateEQ(ranges, game.getBoard(), game.getHand(), game);
		
		//MANIPULATE EQ CALL/CHECK

		EVCALL = eq * totalpot - (1 - eq) * (call);
		EVCALL -= (totalpot + call) * 0.05;

		stringstream stream;
		stream << "TOTALPOT= " << totalpot << endl;
		stream << "CALL= " << call << endl;
		stream << "EQ= " << eq << endl;
		stream << "EVCALL= " << EVCALL << endl;

		logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);
	}
	else if (game.getStreet() == 1)
	{
		logger.logExp("START CALL EV CALCULATION\n", LOGGER_TYPE::BOT_LOGIC);

		double totalpot = game.getTotalPot() * game.getBblind();
		double call = game.getAmountToCall() * game.getBblind();
		double potcommon = game.getPotcommon() * game.getBblind();
		double eq = calculateEQ(ranges, game.getBoard(), game.getHand(), game);

		//MANIPULATE EQ CALL/CHECK

		EVCALL = eq * totalpot - (1 - eq) * (call);
		EVCALL -= (totalpot + call) * 0.05;

		stringstream stream;
		stream << "TOTALPOT= " << totalpot << endl;
		stream << "CALL= " << call << endl;
		stream << "EQ= " << eq << endl;
		stream << "EVCALL= " << EVCALL << endl;

		logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);
	}
	else if (game.getStreet() == 2)
	{
		logger.logExp("START CALL EV CALCULATION\n", LOGGER_TYPE::BOT_LOGIC);

		double totalpot = game.getTotalPot() * game.getBblind();
		double call = game.getAmountToCall() * game.getBblind();
		double potcommon = game.getPotcommon() * game.getBblind();
		double eq = calculateEQ(ranges, game.getBoard(), game.getHand(), game);

		//MANIPULATE EQ CALL/CHECK

		EVCALL = eq * totalpot - (1 - eq) * (call);
		EVCALL -= (totalpot + call) * 0.05;

		stringstream stream;
		stream << "TOTALPOT= " << totalpot << endl;
		stream << "CALL= " << call << endl;
		stream << "EQ= " << eq << endl;
		stream << "EVCALL= " << EVCALL << endl;

		logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);
	}
	else if (game.getStreet() == 3)
	{
		logger.logExp("START CALL EV CALCULATION\n", LOGGER_TYPE::BOT_LOGIC);
		double totalpot = game.getTotalPot() * game.getBblind();
		double call = game.getAmountToCall() * game.getBblind();
		double commonpot = game.getPotcommon() * game.getBblind();
		double eq = calculateEQ(ranges, game.getBoard(), game.getHand(), game);

		//MANIPULATE EQ CALL/CHECK

		EVCALL = eq * totalpot - (1 - eq) * (call);
		EVCALL -= (totalpot + call) * 0.05;

		stringstream stream;
		stream << "TOTALPOT= " << totalpot << endl;
		stream << "CALL= " << call << endl;
		stream << "EQ= " << eq << endl;
		stream << "EVCALL= " << EVCALL << endl;

		logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);
	}

	return EVCALL;
}

//======================================AI FUNCTIONS====================================================
bool BayesDecision::committed(CurrentGameInfo& game)
{
	Logger& logger = Logger::getLogger(BOT_LOGIC);
	logger.logExp("ARE WE COMMITTED?", BOT_LOGIC);

	double totalpot = game.getTotalPot();
	double call = game.getAmountToCall();

	totalpot += call;

	double effectiv = game.getHero().getActualStacksize() - call;
	if (effectiv < 0) effectiv = 0;

	double opponentMaxStackSize = maxOpponentStack(game);

	logger.logExp("Hero actualStack: ",  game.getHero().getActualStacksize(), BOT_LOGIC);
	logger.logExp("Hero call", call, BOT_LOGIC);
	logger.logExp("Hero effectiv stack: ", effectiv, BOT_LOGIC);
	logger.logExp("Opponent max stacksize: ", opponentMaxStackSize, BOT_LOGIC);

	if (effectiv > opponentMaxStackSize) effectiv = opponentMaxStackSize;

	double EQ = effectiv / (2 * effectiv + totalpot);

	logger.logExp("EQUITY NEEDED : ", EQ, BOT_LOGIC);
	
	if (EQ < 0.25)
	{
		return true;
	}

	return false;

}
bool BayesDecision::canCallAfterRaise(CurrentGameInfo& gameInfo, PlayerRange& range, double betsize, BayesUserPreflop& preflop, BayesUserFlop& flop, BayesUserTurn& turn , BayesUserRiver& river)
{
	Logger& logger = Logger::getLogger(BOT_LOGIC);

	logger.logExp("===============BEGIN canCallAfterRaise================= ", BOT_LOGIC);

	PlayerRange allinRange;
	CurrentPlayerInfo& player = gameInfo.getPlayerbyId(range.getId());

	if (gameInfo.getStreet() == 1)
	{
		allinRange = flop.getRaiseRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * gameInfo.getBblind(), betsize, gameInfo.getBblind(), 
										gameInfo.getPotcommon() * gameInfo.getBblind(), gameInfo.getBoard(), gameInfo.getHand(), patternsNeeded);
	}
	else if (gameInfo.getStreet() == 2)
	{
		allinRange = turn.getRaiseRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * gameInfo.getBblind(), betsize, gameInfo.getBblind(), 
										gameInfo.getPotcommon() * gameInfo.getBblind(), gameInfo.getFlopPotSize() * gameInfo.getBblind(), gameInfo.getBoard(), gameInfo.getHand(), patternsNeeded);
	}
	else if (gameInfo.getStreet() == 3)
	{
		allinRange = river.getRaiseRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * gameInfo.getBblind(), betsize, gameInfo.getBblind(), 
										 gameInfo.getPotcommon() * gameInfo.getBblind(), gameInfo.getFlopPotSize() * gameInfo.getBblind(), gameInfo.getBoard(), gameInfo.getHand(), patternsNeeded);
	}

	logger.logExp("===============END of getting allinRange================= ", BOT_LOGIC);

	// If we have not enough patterns, we can let the bot go
	if (allinRange.range.size() == 0)
	{
		logger.logExp("ERROR IN OPPONENT ALLIN RANGE : RETURNING TRUE", BOT_LOGIC);
		return true;
	}

	allinRange = RangeUtils::mergeRange(range, allinRange, gameInfo.getBoard(), gameInfo.getHand());
	allinRange.setId(range.getId());

	logger.logExp("===============END of merging allinRange================= ", BOT_LOGIC);

	vector<PlayerRange> ranges;
	ranges.push_back(allinRange);

	double EQ = calculateEQ(ranges, gameInfo.getBoard(), gameInfo.getHand(), gameInfo);
	//EQ = modifyValue(EQ, 0.05);

	logger.logExp("EQUITY VS ALLIN RANGE : ", EQ, BOT_LOGIC);

	double originalpot = gameInfo.getTotalPot() * gameInfo.getBblind();
	double herocurrentbet = gameInfo.getHero().getBetsize() * gameInfo.getBblind();
	double opponentstack = gameInfo.getPlayerbyId(range.getId()).getActualStacksize() * gameInfo.getBblind();
	double opponentcurrentbet = gameInfo.getPlayerbyId(range.getId()).getBetsize() * gameInfo.getBblind();

	double effectivestack = (opponentstack + opponentcurrentbet);
	double herostack = (gameInfo.getHero().getActualStacksize() + gameInfo.getHero().getBetsize()) * gameInfo.getBblind();
	if (effectivestack > herostack)
		effectivestack = herostack;

	double totalpot = originalpot + betsize - herocurrentbet + opponentstack;
	double call = effectivestack - betsize;
	if (call < 0) call = 0;

	double evcall = EQ * totalpot - (1 - EQ) * call;
	evcall -= ( 2*effectivestack + gameInfo.getPotcommon() * gameInfo.getBblind() ) * 0.05;

	logger.logExp("ALLIN CALL EV AFTER RAISING : ", evcall, BOT_LOGIC);

	return evcall > 0;
}


Action BayesDecision::makeDecision(CurrentGameInfo& game, vector<PlayerRange>& ranges, BayesUserPreflop& preflop, BayesUserFlop& flop, BayesUserTurn& turn, BayesUserRiver& river)
{
	Logger& logger = Logger::getLogger(LOGGER_TYPE::BOT_LOGIC);

	logger.logExp("Hand: " + game.getHand().toString(), LOGGER_TYPE::BOT_LOGIC );
	logger.logExp("Opponent number: ", (int)ranges.size(), BOT_LOGIC);
	if (ranges.size() == 0)
	{
		logger.logExp("Cannot decide action 1", LOGGER_TYPE::BOT_LOGIC);
		return Action('n', 0);
	}

	for (int i = 0; i < ranges.size(); ++i)
	{
		if (!ranges[i].getValid())
		{
			logger.logExp("Cannot decide action 2", LOGGER_TYPE::BOT_LOGIC);
			logger.logExp("Range id: ", ranges[i].getId(), BOT_LOGIC);
			return Action('n', 0);
		}
	}

	Action res;

	double EVRAISE = -100000;
	double maxRaiseSize = 0;
	double EVCALL = 0;

	if (committed(game))
	{
		double betsize = game.getHero().getActualStacksize() + game.getHero().getBetsize();
		double maxOpponent = maxOpponentStack(game) + game.getBiggestBet();
		if (betsize > maxOpponent) betsize = maxOpponent;
		betsize *= game.getBblind();

		vector<PlayerRange> aktRanges;
		
		for (int j = 0; j < ranges.size(); ++j)
		{
			PlayerRange akt;

			if (game.getPlayerbyId(ranges[j].getId()).getActualStacksize() > 0.01)
				akt = getCallRaiseRange(betsize, ranges[j], game, preflop, flop, turn, river);
			else
				akt = ranges[j];

			if (akt.range.size() == 0) break;
			aktRanges.push_back(akt);
		}

		if (aktRanges.size() != ranges.size()) return Action('n', 0);
		
		vector<double> foldEquities = getFoldEquities(betsize, game, ranges, preflop, flop, turn, river);

		for (int i = 0; i < foldEquities.size(); ++i)
			foldEquities[i] *= 0.2;

		double EVAI = calculateEVRaise(game, aktRanges, foldEquities, betsize, true);
		
		logger.logExp("EV of allin : ", EVAI, BOT_LOGIC);

		if (EVAI > 0)
		{
			return Action('r', 1000);
		}
		
		return Action('f', 0);
	}
	
	vector<double> betSizes = getRaiseSizes(game);

	if (game.getStreet() == 0)
	{
		logger.logExp("PREFLOP CALCULATION STARTED", LOGGER_TYPE::BOT_LOGIC);

		for (int i = ranges.size() > 2 ? betSizes.size() - 1 : 0; i < betSizes.size(); ++i)
		{

			double betsize = betSizes[i];

			logger.logExp("actualBetSize: ", betSizes[i], LOGGER_TYPE::BOT_LOGIC);

			vector<PlayerRange> aktRanges;
			for (int j = 0; j < ranges.size(); ++j)
			{
				PlayerRange akt = getCallRaiseRange(betsize, ranges[j], game, preflop, flop, turn, river);
				if (akt.range.size() == 0) break;
				aktRanges.push_back(akt);
			}

			if (aktRanges.size() != ranges.size()) break;
								
			vector<double> foldEquities = getFoldEquities(betsize, game, ranges, preflop, flop, turn, river);
			double evraise = calculateEVRaise(game, aktRanges, foldEquities, betsize);

			if (evraise > EVRAISE)
			{
				EVRAISE = evraise;
				maxRaiseSize = betsize;
			}
		}

		EVCALL = calculateEVCall(game, ranges);

		if (EVRAISE < 0 && game.getHand().isPocket())
		{	
			logger.logExp("Calculate set call", BOT_LOGIC);
			double maxstacksize = 0;
			for (int i = 0; i < game.getOpponentsInfo().size(); ++i)
			{
				CurrentPlayerInfo& player = game.getOpponentsInfo()[i];
				if (abs(player.getBetsize()-game.getBiggestBet() < 0.01) && player.getStacksize() > maxstacksize)
				{
					maxstacksize = player.getStacksize();
				}
			}
			
			logger.logExp("MaxStackSize= ", maxstacksize, BOT_LOGIC);
			if (maxstacksize > game.getHero().getStacksize())
			{
				maxstacksize = game.getHero().getStacksize();
			}
			logger.logExp("MaxStackSizeHero= ", maxstacksize, BOT_LOGIC);
			maxstacksize *= game.getBblind();

			if (maxstacksize > game.getAmountToCall() * 24)
			{
				EVCALL = 100000;
			}
		}

		if (EVRAISE == -100000 && EVCALL < 0)
		{
			logger.logExp("Cannot decide action EVRAISE == -10000", LOGGER_TYPE::BOT_LOGIC);
			return Action('n', 0);
		}
	}

	if (game.getStreet() == 1)
	{
		logger.logExp("FLOP CALCULATION STARTED", LOGGER_TYPE::BOT_LOGIC);

		int str = Evaluator::cardStrength(game.getHand().getCard1(), game.getHand().getCard2(), game.getBoard());

		for (int i = ranges.size() > 2 ? betSizes.size() - 1 : 0; i < betSizes.size(); ++i)
		{

			double betsize = betSizes[i];
			double potcommon = game.getPotcommon() * game.getBblind();

			logger.logExp("actualBetSize: ", betSizes[i], LOGGER_TYPE::BOT_LOGIC);
			vector<PlayerRange> aktRanges;
			for (int j = 0; j < ranges.size(); ++j)
			{
				PlayerRange akt = getCallRaiseRange(betsize, ranges[j], game, preflop, flop, turn, river);
				if (akt.range.size() == 0)
				{
					logger.logExp("Couldn't get opponent's calling+raising range.", LOGGER_TYPE::BOT_LOGIC);
					break;
				}
				aktRanges.push_back(akt);
			}
			
			if (aktRanges.size() != ranges.size()) break;

			vector<double> foldEquities = getFoldEquities(betsize, game, ranges, preflop, flop, turn, river);
			double evraise = calculateEVRaise(game, aktRanges, foldEquities, betsize);

			if (ranges.size() == 1 && betsize > potcommon && str <= 2 && str != 0 && !canCallAfterRaise(game, ranges[0], betsize, preflop, flop, turn, river))
				evraise = -100000;

			if (evraise > EVRAISE)
			{
				EVRAISE = evraise;
				maxRaiseSize = betsize;
			}
		}

		EVCALL = calculateEVCall(game, ranges);
		
		if (EVRAISE == -100000 && EVCALL < 0)
		{
			logger.logExp("Cannot decide action EVRAISE == -10000", LOGGER_TYPE::BOT_LOGIC);
			return Action('n', 0);
		}
	}

	if (game.getStreet() == 2)
	{
		logger.logExp("TURN CALCULATION STARTED", LOGGER_TYPE::BOT_LOGIC);

		int str = Evaluator::cardStrength(game.getHand().getCard1(), game.getHand().getCard2(), game.getBoard());

		for (int i = ranges.size() > 2 ? betSizes.size() - 1 : 0; i < betSizes.size(); ++i)
		{

			double betsize = betSizes[i];

			double potcommon = game.getPotcommon() * game.getBblind();

			logger.logExp("actualBetSize: ", betSizes[i], LOGGER_TYPE::BOT_LOGIC);
			vector<PlayerRange> aktRanges;
			for (int j = 0; j < ranges.size(); ++j)
			{
				PlayerRange akt = getCallRaiseRange(betsize, ranges[j], game, preflop, flop, turn, river);
				if (akt.range.size() == 0) break;
				aktRanges.push_back(akt);
			}
			
			if (aktRanges.size() != ranges.size()) break;
		
			vector<double> foldEquities = getFoldEquities(betsize, game, ranges, preflop, flop, turn, river);
			double evraise = calculateEVRaise(game, aktRanges, foldEquities, betsize);

			if (ranges.size() == 1 && betsize > potcommon && str <= 2 && !canCallAfterRaise(game, ranges[0], betsize, preflop, flop, turn, river))
				evraise = -100000;

			if (evraise > EVRAISE)
			{
				EVRAISE = evraise;
				maxRaiseSize = betsize;
			}
		}

		EVCALL = calculateEVCall(game, ranges);

		if (EVRAISE == -100000 && EVCALL < 0) return Action('n', 0);
	}

	if (game.getStreet() == 3)
	{
		logger.logExp("RIVER CALCULATION STARTED", LOGGER_TYPE::BOT_LOGIC);

		//RIVER VALUEBET
		int str = Evaluator::cardStrength(game.getHand().getCard1(), game.getHand().getCard2(), game.getBoard());

		for (int i = ranges.size() > 2 ? betSizes.size() - 2 : 0; i < betSizes.size(); ++i)
		{
			double betsize = betSizes[i];
			double potcommon = game.getPotcommon() * game.getBblind();

			logger.logExp("actualBetSize: ", betSizes[i], LOGGER_TYPE::BOT_LOGIC);
			vector<PlayerRange> aktRanges;
			for (int j = 0; j < ranges.size(); ++j)
			{
				PlayerRange akt = getCallRaiseRange(betsize, ranges[j], game, preflop, flop, turn, river);
				if (akt.range.size() == 0) break;
				aktRanges.push_back(akt);
			}
			
			if (aktRanges.size() != ranges.size()) break;
			
			vector<double> foldEquities = getFoldEquities(betsize, game, ranges, preflop, flop, turn, river);
			double evraise = calculateEVRaise(game, aktRanges, foldEquities, betsize);

			if (ranges.size() == 1 && betsize > potcommon && str <= 2 && !canCallAfterRaise(game, ranges[0], betsize, preflop, flop, turn, river))
				evraise = -100000;

			if (evraise > EVRAISE)
			{
				EVRAISE = evraise;
				maxRaiseSize = betsize;
			}
		}

		EVCALL = calculateEVCall(game, ranges);
	
		if (EVRAISE == -100000 && EVCALL < 0) return Action('n', 0);
	}

	double totalpot = game.getTotalPot() * game.getBblind();

	logger.logExp("DECIDED ACTION\n", LOGGER_TYPE::BOT_LOGIC);
	if (EVRAISE > 0 || EVCALL > 0)
	{
		if (abs(EVRAISE - EVCALL) <= game.getBblind() && EVRAISE > 0)
		{
			EVRAISE = 100000;
		}

		if (EVRAISE > EVCALL)
		{
			if (EVRAISE == -100000)
			{
				res.setAction('n', 0);
			}
			else
			{
				res.setAction('r', maxRaiseSize);
			}
		}
		else
		{
			if (EVRAISE == -100000)
			{
				logger.logExp("ACTION is hack\n", BOT_LOGIC);
				res.setAction('h', 0);
			}
			else if (abs(game.getAmountToCall() - 0) < 0.001)
			{
				res.setAction('x', 0);
			}
			else
			{
				res.setAction('c', 0);
			}
		}
	}
	else
	{
		if (game.getStreet() == 0 && game.getHand().isAK())
		{
			res.setAction('n', 0);
		}
		else
		{
			res.setAction('f', 0);
		}
	}

	return res;
};