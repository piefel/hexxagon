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
#include <libhexx/libhexx.h>

using namespace libhexx;

class GtkNavBar : public Gtk::Toolbar, public View
{
    public:
        GtkNavBar(GameModel *g);
        
        virtual void updateView();

        void setGameModel(GameModel *g) { game = g; };
    
        void on_first();
        void on_prev();
        void on_hint();
        void on_exec();
        void on_next();
        void on_last();

    protected:
        GameModel *game;        

        Gtk::ToolButton bfirst;
        Gtk::ToolButton bprev;
        Gtk::ToolButton bhelp;
        Gtk::ToolButton bexec;
        Gtk::ToolButton bnext;
        Gtk::ToolButton blast;

        GtkNavBar();
};
