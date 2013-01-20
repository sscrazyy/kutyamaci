#pragma once

#include "updater.h"
#include "Worker.h"
#include "HandHistory.h"
#include "logger.h"

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/regex.hpp>

#include <set>
#include <string>

#include <conio.h>
#include <windows.h>

using namespace std;

class LiveUpdater : public Updater
{
private:

    boost::mutex updateMutex;
public:

	LiveUpdater(string folder, Database* database)
	{
		this->database = database;
		this->folder = folder;
	}

	void  run()
	{
        set<string> S;
        boost::regex fileName("^Supersonic_[[:digit:]]*\.txt");
        Logger& logger = Logger::getLogger(LOGGER_TYPE::HAND_HISTORY_PARSER);
		while(1)
		{
			HANDLE hFind;
			WIN32_FIND_DATA ffd;

			string s = folder + "\\*";

			if ((hFind = FindFirstFile(s2ws(s).c_str(), &ffd)) == INVALID_HANDLE_VALUE)
			{
				Logger& logger = Logger::getLogger(LOGGER_TYPE::HAND_HISTORY_PARSER);
                logger.logExp("Invalid path specified for the hand history files.", LOGGER_TYPE::HAND_HISTORY_PARSER);
    			return;
			}

            do
			{
				if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					//TODO: Log the files from the directory using the logger.

                    char ch[1000];
				    char DefChar = ' ';
				    WideCharToMultiByte(CP_ACP,0,ffd.cFileName,-1,ch,1000,&DefChar,NULL);
				    if (regex_match(ch, fileName)) {
                        string s = folder + "\\";
				        string s2 = ch;

                        string akt = s + s2;

                        if (S.find(akt) == S.end())
                        {
                            S.insert(akt);
                            Worker w(akt, this, this->updateMutex);
                            boost::thread workerThread(w);
                        }
                    }
				}
			}
			while (FindNextFile(hFind, &ffd) != 0);

		FindClose(hFind);
		Sleep(10000);
		}
	}

    void update(vector<HandHistory>& history)
    {
        Logger& logger = Logger::getLogger(LOGGER_TYPE::HAND_HISTORY_PARSER);

        set<string>* Players = new set<string>();
		map<string, double>* VPIP = new map<string, double>();
		map<string, double>* PFR = new map<string, double>();
		map<string, int>* aggrM = new map<string, int>();
		map<string, int>* passM = new map<string, int>();
		map<string, int>* handnr = new map<string, int>();
        for (int i = 0; i < history.size(); ++i)
		{
			if (database->isHand(history[i].getId())) continue;
            Logger& logger = Logger::getLogger(LOGGER_TYPE::HAND_HISTORY_PARSER);
            logger.logExp("The following hand does not exist in the database: ", history[i].getId().c_str(), LOGGER_TYPE::HAND_HISTORY_PARSER);
            database->insertHand(history[i].getId());

			for (int j = 0; j < history[i].getPlayerHistories().size(); ++j)
			{
				PlayerHistory player = history[i].getPlayerHistories()[j];
				/*
				if (!database->isUser(player.getPlayerName()))
				{
					database->insertUser(player.getPlayerName());
				}
				*/
				if (Players->find(player.getPlayerName()) == Players->end())
				{
					Players->insert(player.getPlayerName());
				}
				bool raised = false;
				bool called = false;
				for (int k = 0; k < player.getPreflopAction().size(); ++k)
				{
					if (player.getPreflopAction()[k].getType() == 'r') raised = true;
					if (player.getPreflopAction()[k].getType() == 'c') called = true;
				}

				if (called || raised)
				{
					//database->setVPIP(100, player.getPlayerName());
					(*VPIP)[player.getPlayerName()] = ((*VPIP)[player.getPlayerName()] * (*handnr)[player.getPlayerName()] + 100) / ((*handnr)[player.getPlayerName()] + 1);
				}
				else
				{
					//database->setVPIP(0, player.getPlayerName());
					(*VPIP)[player.getPlayerName()] = ((*VPIP)[player.getPlayerName()] * (*handnr)[player.getPlayerName()]) / ((*handnr)[player.getPlayerName()] + 1);
				}

				if (raised)
				{
					//database->setPFR(100, player.getPlayerName());
					(*PFR)[player.getPlayerName()] = ((*PFR)[player.getPlayerName()] * (*handnr)[player.getPlayerName()] + 100) / ((*handnr)[player.getPlayerName()] + 1);
				}
				else
				{
					//database->setPFR(0, player.getPlayerName());
					(*PFR)[player.getPlayerName()] = ((*PFR)[player.getPlayerName()] * (*handnr)[player.getPlayerName()]) / ((*handnr)[player.getPlayerName()] + 1);
				}

				int aggr = 0;
				int pass = 0;
				for (int k = 0; k < player.getFlopAction().size(); ++k)
				{
					if (player.getFlopAction()[k].getType() == 'c') pass++;
					if (player.getFlopAction()[k].getType() == 'r') aggr++;
					//if (player.getFlopAction()[k].getType() == 'x') pass++;
				}
				for (int k = 0; k < player.getTurnAction().size(); ++k)
				{
					if (player.getFlopAction()[k].getType() == 'c') pass++;
					if (player.getFlopAction()[k].getType() == 'r') aggr++;
					//if (player.getFlopAction()[k].getType() == 'x') pass++;
				}
				for (int k = 0; k < player.getRiverAction().size(); ++k)
				{
					if (player.getFlopAction()[k].getType() == 'c') pass++;
					if (player.getFlopAction()[k].getType() == 'r') aggr++;
					//if (player.getFlopAction()[k].getType() == 'x') pass++;
				}

				//database->setAGGR(aggr, player.getPlayerName());
				//database->setPASS(pass, player.getPlayerName());
				(*aggrM)[player.getPlayerName()] += aggr;
				(*passM)[player.getPlayerName()] += pass;
				(*handnr)[player.getPlayerName()]++;
			}
		}

		for (set<string>::iterator it = Players->begin(); it != Players->end(); ++it)
		{
			if (!database->isUser(*it))
			{
				database->insertUser(*it);
			}
			database->setAGGR((*aggrM)[*it], *it);
			database->setPASS((*passM)[*it], *it);
			database->setPFR((*PFR)[*it], (*handnr)[*it], *it);
			database->setVPIP((*VPIP)[*it], (*handnr)[*it], *it);
			database->incHandnr((*handnr)[*it], *it);
		}

		delete Players;
		delete aggrM;
		delete passM;
		delete VPIP;
		delete PFR;
		delete handnr;

        cout << "Databases has been updated!" << endl;
	}
};