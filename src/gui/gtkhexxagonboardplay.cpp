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


#include "gtkhexxagonboardplay.h"
#include <unistd.h>
#include "microtime.h"

GtkHexxagonBoardPlay::GtkHexxagonBoardPlay(GameModel *g) : game(g) 
{
    blockInput = 0;
    game->setNoOfPlayers(1);
}

void GtkHexxagonBoardPlay::updateView()
{
    clearSelection();
    
    GtkHexxagonBoard::operator=(game->getBoard());

    if(!game->getTurn())
        bbColor = ~bbColor;

    setBoardMask(game->getMask());

    draw();
}

bool GtkHexxagonBoardPlay::on_cell_press_event(GdkEventButton* event, int cell)
{
    if(blockInput)
        return true;

    blockInput = 1;

    if(event->button == 1)
    {
        if((bbBoard & (game->getTurn() ? bbColor : ~bbColor)).getBit(cell))
        {
            setSelection(cell);
            draw();
        }
        else if(selectedCell != -1)
        {
            LookUp lookup(bbMask);
            int move = 0;
            int playerTurn = game->getTurn();
            
            if(lookup.getClone(selectedCell).getBit(cell))
            {
                Move m(cell);

                if(game->isMoveValid(m))
                {
                    game->applyMove(m);
                    game->next();

                    clearSelection();

                    move = 1;
                }
            }
            else if(lookup.getJump(selectedCell).getBit(cell))
            {
                Move m(selectedCell, cell);

                if(game->isMoveValid(m))
                {
                    game->applyMove(m);
                    game->next();

                    clearSelection();

                    move = 1;
                }
            }

            if(move)
            {
                game->updateView();

                while(Gtk::Main::events_pending())
                    Gtk::Main::iteration(false);

                //Computer or two player mode?
                if(game->getNoOfPlayers() == 1 && game->getTurn() != playerTurn)
                {
                    // Let the computer do his move...
                    game->computerMove();
                }
            }
        }
        else
        {
            clearSelection();
        }
    }

    blockInput = 0;
    return true;
}

void GtkHexxagonBoardPlay::breakMove()
{
    animate = false;
}

void GtkHexxagonBoardPlay::animateMove(const libhexx::Move &m)
{
    setCompHighlight(m.from, m.to); 

    draw(); 
    
    MicroTime start;

    animate = true;
    while(MicroTime() - start < 1000000 && animate)
    {
        while(Gtk::Main::events_pending())
            Gtk::Main::iteration(false);

        usleep(10000);
    }    

    clearCompHighlight();
}

bool GtkHexxagonBoardPlay::on_cell_release_event(GdkEventButton* event, int cell)
{
    if(blockInput)
        return true;
    
    if(event->button == 3)
    {
        clearSelection(); 
        draw(); 
    }
    
    return true;
}

