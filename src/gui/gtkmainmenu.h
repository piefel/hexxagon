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


#include <gtkmm/menu.h>
#include <gtkmm/menubar.h>

#include "gamemodel.h"
#include "gtkhexxagonboardplay.h"

class GtkMainMenu : public Gtk::MenuBar
{
    public:
        GtkMainMenu(GameModel *g);
        
        void setGame(GameModel *g) { game = g; };
        
        void on_menu_noplayers(int n);
        void on_menu_difficulty(int n);
        void on_menu_tileset(const char* tilesetname);
        
    protected:
        GameModel *game;
        
        void on_layout_select_no_action(const libhexx::Layout &l);
        void on_layout_select(const libhexx::Layout &l);
        void on_menu_edit_board();
        void on_menu_about();
        void on_menu_quit();
        void on_menu_new_game();
        void on_menu_open_game();
        void on_menu_save_game();
        void on_menu_save_as_game();
        
        Gtk::Menu mFile;
        Gtk::Menu mLocalGame;
        Gtk::Menu mNetworkGame;
        Gtk::Menu mHelp;

        Gtk::Menu mDifficulty;
        Gtk::Menu mTileSet;
        
        Gtk::RadioMenuItem::Group modeGroup;
        Gtk::RadioMenuItem::Group levelGroup;
        Gtk::RadioMenuItem::Group tileGroup;

        Glib::ustring lastSaveFilename;
        
    private:
        GtkMainMenu();
};

