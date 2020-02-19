/* Hexxagon board game.
 * Copyright (C) 2001 Erik Jonsson.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 * Email hexxagon@nesqi.se 
 * 
 */


#ifndef _GAMEMODEL_H
#define _GAMEMODEL_H

#include <libhexx/libhexx.h>

#include <list>

class GameModel : public libhexx::Game
{
    public:
        GameModel();
        
        void addView(libhexx::View *view);
        void updateView();
        void animateMove(const libhexx::Move &m);

        void hint();
        
        void setAILevel(int i) { aiLevel = i; };
        void setAITimeout(int i) { maxtimeout = i; };
        void setCallback(bool (*c)()) { callback = c; };
        void setNoOfPlayers(int n) { noOfPlayers = n; };

        bool getComputerPlayer() { return computerPlayer; }; 
        int getNoOfPlayers() { return noOfPlayers; };
        
        void computerMove();
        libhexx::Move computerSingleMove();
		virtual int applyMove(const libhexx::Move &move);

        void breakMove();
        bool isThinking() const { return thinking; };
        bool isPlaying() const { return playing; };
        
    protected:
        bool thinking;
        bool playing;

        int aiLevel;
        int maxtimeout;
        bool (*callback)(); 
        
        int noOfPlayers;
        int computerPlayer; 

        // The views to notify if we update something.
        std::list<libhexx::View*> views;
};

#endif //_GAMEMODEL_H
