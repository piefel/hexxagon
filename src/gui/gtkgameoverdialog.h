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


#ifndef _GTKGAMEOVERDIALOG_H
#define _GTKGAMEOVERDIALOG_H
 
#include <gtkmm.h>
#include <libhexx/libhexx.h>
#include "gamemodel.h"

class GtkGameOverDialog : public Gtk::Dialog, public libhexx::View
{
    public:
        GtkGameOverDialog(GameModel *g);

        virtual void updateView();
        void setGame(GameModel *g) { game = g; };
        
        void close();
        
    private:
        GtkGameOverDialog();

	void setPixmaps();
        void hideInvisibles();
        
        Gtk::VBox vbox;
            Gtk::Image imgComputerWins;
            Gtk::Image imgYouWin;
            Gtk::Image imgDraw; 
            
            Gtk::Label text;
        
        GameModel *game;
};

#endif //_GAMEOVERDIALOG_H
