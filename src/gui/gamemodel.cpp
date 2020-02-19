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


#include "gamemodel.h"
#include <gtkmm.h>

using namespace libhexx;

bool breakThinking;

extern Gtk::ProgressBar *progress;

bool callGtk()
{
    progress->pulse();
    
    while(Gtk::Main::events_pending())
        Gtk::Main::iteration(false);

    if(breakThinking)
    {
        breakThinking = false;
        return true;
    }

    return false;
}

GameModel::GameModel()
{
    callback = callGtk;
    thinking = false;
    playing = false; 

    computerPlayer = !getTurn();
}

void GameModel::addView(View *view)
{ 
    views.push_back(view);
    view->updateView();
}

void GameModel::breakMove()
{
    breakThinking = true;
    playing = false;

    for(std::list<View*>::iterator i = views.begin(); i != views.end(); i++)
        (*i)->breakMove(); 
}

                    
void GameModel::computerMove()
{
    if(playing)
        return;

    playing = true;
    int playerTurn = !getTurn();
    while(getTurn() != playerTurn && !endOfGame() && playing)
    {
        animateMove(computerSingleMove());               
        
        if(playing)
            next();
        else
            destroyRest();            
        
        updateView();

        while(Gtk::Main::events_pending())
            Gtk::Main::iteration(false);
    }
    playing = false;
}

Move GameModel::computerSingleMove()
{
    Move m;
    
    if(thinking)
        return m;
    
    int turn = !getTurn();

    thinking = 1;
    breakThinking = false;
    
    m = Game::computerMove(aiLevel, callback, maxtimeout); 

    thinking = 0;

    if(m)
        computerPlayer = turn;
    
    return m; 
}

int GameModel::applyMove(const libhexx::Move &move)
{
    computerPlayer = getTurn();
    return libhexx::Game::applyMove(move);
}

void GameModel::updateView() 
{
    for(std::list<View*>::iterator i = views.begin(); i != views.end(); i++)
        (*i)->updateView(); 
}

void GameModel::animateMove(const Move &m) 
{
    for(std::list<View*>::iterator i = views.begin(); i != views.end(); i++)
        (*i)->animateMove(m); 
}

void GameModel::hint()
{
    if(playing)
        return;
    
    Move m;
    
    thinking = true;

    m = computerHint(aiLevel, callback, maxtimeout);
   
    if(m)
    {
        animateMove(m); 
    }

    thinking = false;

    updateView();
}

