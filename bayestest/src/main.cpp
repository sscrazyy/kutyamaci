//#include "plusEVBotLogic.h"
#include <iostream>
#include <vector>
#include "bayesUserFunctions.h"
#include "eqcalculator.h"

using namespace std;

BayesUserPreflop preflop;
BayesUserFlop flop;
BayesUserTurn turn;
BayesUserRiver river;

void testRiverFE()
{
	printf("\n--------------------------------river tests----------------------------------\n\n");

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
	double FE = river.getProbabilityFE(  20,       18,       3,         125*0.04,            5,              0,             0.04,             2.75,                 0.75,            35);
	printf("river 3bet pot reg ellen triple barrel pozin kivul              = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
		   FE = river.getProbabilityFE(  20,       18,       3,         125*0.04,            2,              0,             0.04,             2.75,                 0.75,            35);
	printf("river 3bet pot reg ellen triple barrel pozibol                  = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
		   FE = river.getProbabilityFE(  40,       10,       1,         125*0.04,            5,              0,             0.04,             2.75,                 0.75,            35);
	printf("river 3bet pot fish ellen triple barrel pozin kivul             = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
		   FE = river.getProbabilityFE(  40,       10,       1,         125*0.04,            2,              0,             0.04,             2.75,                 0.75,            35);
	printf("river 3bet pot fish ellen triple barrel pozibol                 = %lf\n", FE);

	printf("--------------------------------end of river tests------------------------------\n");
}

void testTurnFE()
{
	printf("\n--------------------------------turn tests----------------------------------\n\n");

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
	double FE = turn.getProbabilityFE(   40,       5,         1,           4,                 2,             0,             0.04,               1,                   0.4,              35);
	printf("turn emelt pot fish ellen pozibol barrel                        = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
		   FE = turn.getProbabilityFE(   40,       5,         1,           4,                 5,             0,             0.04,               1,                   0.4,              35);
	printf("turn emelt pot fish ellen pozin kivul barrel                    = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
		   FE = turn.getProbabilityFE(   20,       18,        3,           4,                 2,             0,             0.04,               1,                   0.4,              35);
	printf("turn emelt pot regular ellen pozibol barrel                     = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
		   FE = turn.getProbabilityFE(   20,       18,        3,           4,                 5,             0,             0.04,               1,                   0.4,              35);
	printf("turn emelt pot regular ellen pozin kivul barrel                 = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
		   FE = turn.getProbabilityFE(   50,       40,        5,           4,                 2,             0,             0.04,               1,                   0.4,              35);
	printf("turn emelt pot maniac ellen pozibol barrel                      = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
		   FE = turn.getProbabilityFE(   50,       40,        5,           4,                 5,             0,             0.04,               1,                   0.4,              35);
	printf("turn emelt pot maniac ellen pozin kivul barrel                  = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
		   FE = turn.getProbabilityFE(   20,       18,        3,           4,                 1,             0.66,          0.04,               1,                   0.4,              35);
	printf("turn emelt pot regular ellen raise barrel                       = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
		   FE = turn.getProbabilityFE(   20,       18,        3,           4,                 1,             1.6,           0.04,               1,                   0.4,              10);
	printf("turn emelt pot regular ellen 3bet								= %lf\n", FE);

	printf("-----------------------------end of turn tests------------------------------\n");
}

void testFlopFE()
{
	printf("\n--------------------------------flop tests----------------------------------\n\n");

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
	double FE = flop.getProbabilityFE(  40,        5,         1,           4,                 2,             0,             0.04,              0.4,                  35);
	printf("flop emelt pot fish ellen pozibol cbet                          = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
		   FE = flop.getProbabilityFE(  40,        5,         1,           4,                 5,             0,             0.04,              0.4,                  35);
	printf("flop emelt pot fish ellen pozin kivul cbet                      = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
		   FE = flop.getProbabilityFE(  20,        18,        3,           4,                 2,             0,             0.04,              0.4,                  35);
	printf("flop emelt pot regular ellen pozibol cbet                       = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
		   FE = flop.getProbabilityFE(  20,        18,        3,           4,                 5,             0,             0.04,              0.4,                  35);
	printf("flop emelt pot regular ellen pozin kivul cbet                   = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
		   FE = flop.getProbabilityFE(  50,        40,        5,           4,                 2,             0,             0.04,              0.4,                  35);
	printf("flop emelt pot maniac ellen pozibol cbet                        = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
		   FE = flop.getProbabilityFE(  50,        40,        5,           4,                 5,             0,             0.04,              0.4,                  35);
	printf("flop emelt pot maniac ellen pozin kivul cbet                    = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
		   FE = flop.getProbabilityFE(  40,        10,        1,           2,                 2,             0,             0.04,              0.4,                  35);
	printf("flop emelt pot midstack fish ellen pozibol cbet                 = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
		   FE = flop.getProbabilityFE(  40,        10,        1,           2,                 5,             0,             0.04,              0.4,                  35);
	printf("flop emelt pot midstack fish pozin kivul cbet                   = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
		   FE = flop.getProbabilityFE(  40,        10,        1,           1,                 2,             0,             0.04,              0.4,                  35);
	printf("flop emelt pot shortstack fish ellen pozibol cbet               = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
		   FE = flop.getProbabilityFE(  40,        10,        1,           1,                 5,             0,             0.04,              0.4,                  35);
	printf("flop emelt pot shortstack fish pozin kivul cbet                 = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  20,        18,        3,           4,                  1,             0.2,           0.04,             0.3,                 35);
	printf("flop emelt pot regular raise cbet                               = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  20,        18,        3,           4,                  1,             0.3,           0.04,             0.3,                 35);
	printf("flop emelt pot regular raise cbet  (pot meretu cbet)            = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  40,        10,        1,           4,                  1,             0.2,           0.04,             0.3,                 35);
	printf("flop emelt pot fish    raise cbet                               = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  40,        10,        1,           4,                  1,             0.3,           0.04,             0.3,                 35);
	printf("flop emelt pot fish    raise cbet  (pot meretu cbet)            = %lf\n", FE);


	// --------------------------------------------------------------------------- 3bet pot ----------------------------------------------------------------------------


	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  20,        18,        3,           4,                  1,             0.4,          0.04,              0.75,                 35);
	printf("flop 3bet pot regular raise cbet                                = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  20,        18,        3,           4,                  1,             0.75,         0.04,              0.75,                 35);
	printf("flop 3bet pot regular raise cbet   (pot meretu cbet)            = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  20,        18,        3,           4,                  2,             0,            0.04,              0.75,                 35);
	printf("flop 3bet pot regular ellen pozibol cbet                        = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  20,        18,        3,           4,                  5,             0,            0.04,              0.75,                 35);
	printf("flop 3bet pot regular ellen pozin  kivul                        = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  40,        5,         3,           4,                  2,             0,            0.04,              0.75,                 35);
	printf("flop 3bet pot fish ellen pozibol cbet                           = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  40,        5,         1,           4,                  5,             0,            0.04,              0.75,                 35);
	printf("flop 3bet pot fish ellen pozin  kivul                           = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  40,        5,         1,           4,                  1,             1.2,            0.04,            0.75,                 35);
	printf("flop 3bet pot fish ellen 3bet                                   = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  20,        18,         3,          4,                  1,             1.2,            0.04,            0.75,                 35);
	printf("flop 3bet pot regular ellen 3bet                                = %lf\n", FE);

	printf("-----------------------------end of flop tests------------------------------\n");

}

void testPreflopFE()
{
	printf("\n-----------------------------preflop tests----------------------------------\n\n");

	//                                   VPIP      PFR     stacksize      poz          line          betsize          bblind         
	double FE = preflop.getProbabilityFE(20,       18,        4,          -1,            1,           0.12,           0.04,            0,             35);
	printf("preflop 3bet CO regular ellen                                   = %lf\n", FE);

	//                                   VPIP      PFR     stacksize      poz          line          betsize          bblind         
		   FE = preflop.getProbabilityFE(40,       15,        4,          -1,            1,           0.12,           0.04,            0,             35);
	printf("preflop 3bet CO fish    ellen                                   = %lf\n", FE);

	printf("--------------------------end of preflop tests------------------------------\n");
}

// 8/5 pre shove 100bb
void testPreflopRange1()
{
	printf("\n--------------------------------------8/5 pre shove range--------------------------------------------\n");

	PlayerRange range = preflop.getRange(8, 5, 4, -3, 1, 4, 0.04, 0, 0);

	range.printRange();

	printf("--------------------------------------end of 8/5 pre shove range---------------------------------------\n");
}

// 8/5 pre call 100bb
void testPreflopRange2()
{
	printf("\n--------------------------------------8/5 pre call range--------------------------------------------\n");

	PlayerRange range = preflop.getRange(8, 5, 4, -3, 0, 4, 0.04, 0, 0);

	range.printRange();

	printf("--------------------------------------end of 8/5 pre call range---------------------------------------\n");
}

void testBoardType()
{
	printf("\n--------------------------------------board type testing--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('8','c'));
	board.push_back(Card('A','s'));
	board.push_back(Card('T','c'));
	board.push_back(Card('8','s'));
	board.push_back(Card('5','d'));

	int boardType = Evaluator::boardType(board);

	printf("8c As Tc 8s 5d board type                        = %d\n", boardType);

	printf("--------------------------------------end of board type testing---------------------------------------\n");
}

void testBoardType2()
{
	printf("\n--------------------------------------board type testing--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('2','c'));
	board.push_back(Card('T','c'));
	board.push_back(Card('3','d'));
	board.push_back(Card('2','d'));
	board.push_back(Card('Q','d'));

	int boardType = Evaluator::boardType(board);

	printf("2c TC 3d 2d Qd board type                        = %d\n", boardType);

	printf("--------------------------------------end of board type testing---------------------------------------\n");
}

void testBoardType3()
{
	printf("\n--------------------------------------board type testing--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('6','c'));
	board.push_back(Card('Q','d'));
	board.push_back(Card('A','h'));
	board.push_back(Card('9','h'));
	board.push_back(Card('T','h'));

	int boardType = Evaluator::boardType(board);

	printf("6c Qd Ah 9h Th board type                        = %d\n", boardType);

	printf("--------------------------------------end of board type testing---------------------------------------\n");
}

void testRiverRange1()
{
	printf("\n--------------------------------------River regular AI call range 3bet pot on 8s3c8c4s3h--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('8','s'));
	board.push_back(Card('3','c'));
	board.push_back(Card('8','c'));
	board.push_back(Card('4','s'));
	board.push_back(Card('3','h'));

	Hand hand;
	hand.setHand(Card('Q','c'),Card('Q','s'));

	//PlayerRange range = river.getRange(20, 15, 2.5, 111.5*0.04, 5, 0, 0.04, 68.5*0.04, 18.25*0.04, board, hand, 35); 
	PlayerRange range = river.getCallRaiseRange(20, 15, 2.5, 111.5*0.04, 0, 1.82, 0.04, 2.74, 0.73, board, hand, 35); 

	range.printRange();

	printf("--------------------------------------end of River regular AI call range 3bet pot on 8s3c8c4s3h---------------------------------------\n");
}

void testEvaluator1()
{
	printf("\n--------------------------------------River JJ on 8s3c8c4s3h--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('8','s'));
	board.push_back(Card('3','c'));
	board.push_back(Card('8','c'));
	board.push_back(Card('4','s'));
	board.push_back(Card('3','h'));

	Hand hand;
	hand.setHand(Card('J','c'),Card('J','s'));

	printf("handstrength : %d", Evaluator::cardStrength(hand.getCard1(), hand.getCard2(), board));

	printf("--------------------------------------end of River JJ on 8s3c8c4s3h---------------------------------------\n");
}

void testEvaluator2()
{
	printf("\n--------------------------------------River AT on 7d4s4hTs8c--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('7','d'));
	board.push_back(Card('4','s'));
	board.push_back(Card('4','h'));
	board.push_back(Card('T','s'));
	board.push_back(Card('8','c'));

	Hand hand;
	hand.setHand(Card('2','c'),Card('T','c'));

	printf("handstrength : %d", Evaluator::cardStrength(hand.getCard1(), hand.getCard2(), board));

	printf("--------------------------------------end of River AT on 7d4s4hTs8c---------------------------------------\n");
}

void testEQCalculator()
{
	printf("\n--------------------------------------EQ calculation --------------------------------------------\n");

	PlayerRange range1, range2;
	range1.range.insert(make_pair(Hand(Card('A','d'),Card('J','h')), 1));
	range2.range.insert(make_pair(Hand(Card('A','c'),Card('T','s')), 1));

	vector<PlayerRange> ranges;
	ranges.push_back(range1);
	ranges.push_back(range2);

	vector<Card> board;
	board.push_back(Card('8','d'));
	board.push_back(Card('T','d'));
	board.push_back(Card('2','d'));
	board.push_back(Card('6','d'));

	EqCalculator calculator;

	double eq = calculator.calculate(ranges, board, 10000);

	printf("EQ of AdJh vs QdJd on 8dTd2d6d                  = %lf\n", eq);

	printf("--------------------------------------end of EQ calculation---------------------------------------\n");
}

int main()
{
    try
    {
		EqCalculator calc;
		//PlusEVBotLogic botlogic;
		//BayesDecision decision;

		/*preflop.read("preflopBayes");
		flop.read("flopBayes");
		turn.read("turnBayes");
		river.read("riverBayes");

		testPreflopFE();
		testFlopFE();
		testTurnFE();
		testRiverFE();

		testBoardType();
		testBoardType2();
		testBoardType3();

		testEvaluator1();*/

		testEvaluator2();

		//testEQCalculator();

		//testRiverRange1();

		//testPreflopRange1();
		//testPreflopRange2();
    }
    catch (std::exception& e)
    {
        cout << "exception thrown: " << endl;
        cout << e.what() << endl;
        cout << "hit enter to terminate" << endl;
        cin.get();
    }
}