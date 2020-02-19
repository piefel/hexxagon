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


#include "helper_functions.h"
#include "gtkscore.h"
#include <libhexx/libhexx.h>

#include <sstream>

Gtk::ProgressBar *progress;

GtkScore::GtkScore(GameModel *g) : game(g)
{
    set_shadow_type(Gtk::SHADOW_OUT);

    add(vbox);
    {
        //vbox.set_border_width(8);
        vbox.set_spacing(20);

        vbox.pack_start(turnFrame, Gtk::PACK_SHRINK);
        {
            turnFrame.set_label("Turn");

            pl1timg.set_image( 80, 70, open_pixmap_from_file("board_N_2"));
            pl2timg.set_image( 80, 70, open_pixmap_from_file("board_N_1"));
            turnFrame.add(vboxturn);
            {
                vboxturn.set_border_width(4);
                vboxturn.set_spacing(4); 
                
                vboxturn.pack_start(turnlabel);
                pl1timg.show();
                pl2timg.show();
 
                turn1active = false;
                turn2active = false;
            }
        }
        
        vbox.pack_start(computerFrame, Gtk::PACK_SHRINK);
        {
            computerFrame.set_label("Thinking");

            computerFrame.add(vboxcomputer);
            {
                vboxcomputer.set_border_width(4);
                vboxcomputer.set_spacing(4); 
                
                vboxcomputer.pack_start(thinkprogress);
                thinkprogress.set_pulse_step(0.01f);
                thinkprogress.pulse();
                progress = &thinkprogress;
            }
        }
        
        vbox.pack_start(pl1Frame, Gtk::PACK_SHRINK);
        {
            
            pl1img.set_image( 80, 70, open_pixmap_from_file("board_N_2"));
            
            pl1Frame.set_label("Player 1");
            //pl1Frame.set_shadow_type(Gtk::SHADOW_NONE);

            pl1Frame.add(vboxf1);
            {
                vboxf1.set_border_width(4);
                vboxf1.set_spacing(4); 

                vboxf1.pack_start(pl1img);
                vboxf1.pack_start(pl1Bar);
            }
        }

        vbox.pack_start(pl2Frame, Gtk::PACK_SHRINK);
        {
            pl2Frame.set_label("Player 2");

            pl2img.set_image( 80, 70, open_pixmap_from_file("board_N_1"));
            pl2Frame.add(vboxf2);
            {
                vboxf2.set_border_width(4);
                vboxf2.set_spacing(4); 

                vboxf2.pack_start(pl2img);
                vboxf2.pack_start(pl2Bar);
            }
        }
    }
    add_tileChangeListener(sigc::mem_fun(*this, &GtkScore::setPixmaps));
}

void GtkScore::setPixmaps()
{
    pl1timg.set_image( 80, 70, open_pixmap_from_file("board_N_2"));
    pl2timg.set_image( 80, 70, open_pixmap_from_file("board_N_1"));
    pl1img.set_image( 80, 70, open_pixmap_from_file("board_N_2"));
    pl2img.set_image( 80, 70, open_pixmap_from_file("board_N_1"));
}

void GtkScore::updateView()
{
    /* Turn */
    if(game->getTurn())
    {
        if(!turn1active)
        {
            turn1active = true;
            vboxturn.pack_start(pl1timg);
        }
        
        if(turn2active)
        {
            turn2active = false;
            vboxturn.remove(pl2timg);
        }

        turnlabel.set_text("Player 1");
    } 
    else 
    {
        if(turn1active)
        {
            turn1active = false;
            vboxturn.remove(pl1timg);
        }

        if(!turn2active)
        {
            turn2active = true;
            vboxturn.pack_start(pl2timg);
        }
        
        turnlabel.set_text("Player 2");
    }
    
    /* player stats */
    libhexx::BitBoard64 mask = game->getMask();

    int total = 0;
    for(int i = 0; i < 61; i++)
    {
        if(mask.getBit(i))
            total++;
    } 
    
    int pl1 = game->getBrickCount(0);
    int pl2 = game->getBrickCount(1);
    
    std::ostringstream buf;

    double fr1 = (double)pl1 / total;
    buf << pl1;
    pl1Bar.set_text(buf.str());
    
    double fr2 = (double)pl2 / total;
    buf.str("");
    buf << pl2;
    pl2Bar.set_text(buf.str());
    
    pl1Bar.set_fraction(fr1);
    pl2Bar.set_fraction(fr2);
}

