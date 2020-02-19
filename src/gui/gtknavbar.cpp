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


#include "gtknavbar.h"

GtkNavBar::GtkNavBar(GameModel *g) : game(g), 
                                     bfirst(Gtk::Stock::GOTO_FIRST), 
                                     bprev(Gtk::Stock::GO_BACK), 
                                     bhelp(Gtk::Stock::HELP), 
                                     bexec(Gtk::Stock::EXECUTE), 
                                     bnext(Gtk::Stock::GO_FORWARD), 
                                     blast(Gtk::Stock::GOTO_LAST)
{
    bfirst.signal_clicked().connect(sigc::mem_fun(*this, &GtkNavBar::on_first));
    append(bfirst);
    
    bprev.signal_clicked().connect(sigc::mem_fun(*this, &GtkNavBar::on_prev));
    append(bprev);
    
    bhelp.signal_clicked().connect(sigc::mem_fun(*this, &GtkNavBar::on_hint));
    append(bhelp);
    
    bexec.signal_clicked().connect(sigc::mem_fun(*this, &GtkNavBar::on_exec));
    append(bexec);
    
    bnext.signal_clicked().connect(sigc::mem_fun(*this, &GtkNavBar::on_next));
    append(bnext);
    
    blast.signal_clicked().connect(sigc::mem_fun(*this, &GtkNavBar::on_last));
    append(blast);

    set_show_arrow(false);
}

void GtkNavBar::updateView()
{
    bnext.set_sensitive(game->isNext());
    blast.set_sensitive(game->isNext());
    bprev.set_sensitive(game->isPrev());
    bfirst.set_sensitive(game->isPrev());
}

void GtkNavBar::on_first()
{
    int change = 0;
    
    while(game->prev() != -1)
        change = 1;

    if(change)
    {
        game->breakMove();
        game->updateView();    
    }
}

void GtkNavBar::on_prev()
{
    if(game->prev() != -1)
    {
        game->breakMove();
        game->updateView();
    }
}

void GtkNavBar::on_exec()
{
    if(!game->isPlaying() || game->isThinking())
        game->computerMove();
}

void GtkNavBar::on_hint()
{
    if(!game->isPlaying() || game->isThinking())
        game->hint();
}

void GtkNavBar::on_next()
{
    if(game->next() != -1)
    {
        game->breakMove();
        game->updateView();
    }
}

void GtkNavBar::on_last()
{
    int change = 0;
    
    while(game->next() != -1)
        change = 1;

    if(change)
    {
        game->breakMove();
        game->updateView();    
    }
}
