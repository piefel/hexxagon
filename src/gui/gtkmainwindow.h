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


#include <gtkmm.h>
#include "gamemodel.h"
#include "gtkmainmenu.h"
#include "gtknavbar.h"
#include "gtkhexxagonboard.h"
#include "gtkhexxagonboardplay.h"
#include "gtkscore.h"
#include "gtkgameoverdialog.h"

class GtkMainWindow : public Gtk::Window
{
    public:
        GtkMainWindow();

        virtual bool on_configure_event(GdkEventConfigure *event);
    private:
        Gtk::VBox vbox;
        Gtk::HBox topBox; 
        
        GtkScore score;
        GtkHexxagonBoardPlay hexBoard;
        GtkNavBar   navBar;
        GtkMainMenu menu;
        GtkGameOverDialog gameoverdialog;

        Gtk::HBox lowBox;
    
        
        GameModel game;
};
