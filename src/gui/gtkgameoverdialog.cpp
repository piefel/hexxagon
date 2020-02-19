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


#include "gtkgameoverdialog.h"
#include "helper_functions.h"

#include <sstream>

GtkGameOverDialog::GtkGameOverDialog(GameModel *g) : Dialog("Game over", true, true), game(g)
{
    setPixmaps();
    add_tileChangeListener(sigc::mem_fun(*this, &GtkGameOverDialog::setPixmaps));

    get_vbox()->pack_start(vbox);
    {
        vbox.set_border_width(8);
        vbox.set_spacing(8);

        vbox.pack_start(imgComputerWins);
        vbox.pack_start(imgYouWin);
        vbox.pack_start(imgDraw);
        vbox.pack_start(text);
    }

    set_default_size(5, 5);
    set_resizable(false);
    
    show_all();
    hideInvisibles();

    add_button("Ok", 1)->signal_clicked().connect(sigc::mem_fun(*this, &GtkGameOverDialog::close));
}

void GtkGameOverDialog::setPixmaps()
{
    imgComputerWins.set(open_pixmap_from_file("computer_wins"));
    imgYouWin.set(open_pixmap_from_file("you_win"));
    imgDraw.set(open_pixmap_from_file("draw"));
}

void GtkGameOverDialog::hideInvisibles()
{
    imgComputerWins.hide();
    imgYouWin.hide();
    imgDraw.hide();
    hide();
}

void GtkGameOverDialog::close()
{
    hideInvisibles();
}

void GtkGameOverDialog::updateView()
{
    if(game->endOfGame())
    {
        /* Set up the dialog and show it */
        if(game->getBrickCount(0) == game->getBrickCount(1))
        {
            imgDraw.show();     
            text.set_text("Congratulations, no one had to loose!");        
        }
        else
        {
            int winner;

            if(game->getBrickCount(0) > game->getBrickCount(1))
                winner = 0;
            else
                winner = 1;

            if(game->getNoOfPlayers() == 1)
            {
                if(winner != game->getComputerPlayer())
                    imgYouWin.show();
                else
                    imgComputerWins.show();
            }
            else
            {
                imgYouWin.show();
            }
            
            
            std::ostringstream buf;

            buf << "Player " << winner + 1 << " won with " << game->getBrickCount(winner);
            buf << " pieces against\nplayer " << !winner + 1 << " who had " <<  game->getBrickCount(!winner) << " pieces.";
            text.set_text(buf.str()); 
        }
        
        present();
    }
}

