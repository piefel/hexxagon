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


#include "gtkmainmenu.h"
#include "gtkaboutwindow.h"
#include "gtkeditwindow.h"
#include "gtkeditwindow.h"
#include "helper_functions.h"
#include <gtkmm/stock.h>

#include <fstream>

GtkMainMenu::GtkMainMenu(GameModel *g) : game(g)
{
    items().push_back(Gtk::Menu_Helpers::MenuElem("_File", mFile));   
    mFile.items().push_back(Gtk::Menu_Helpers::MenuElem("_New game", sigc::mem_fun(*this, &GtkMainMenu::on_menu_new_game)));
    mFile.items().push_back(Gtk::Menu_Helpers::MenuElem("_Open game", sigc::mem_fun(*this, &GtkMainMenu::on_menu_open_game)));
    mFile.items().push_back(Gtk::Menu_Helpers::MenuElem("_Save game", sigc::mem_fun(*this, &GtkMainMenu::on_menu_save_game)));
    mFile.items().push_back(Gtk::Menu_Helpers::MenuElem("_Save game as ...", sigc::mem_fun(*this, &GtkMainMenu::on_menu_save_as_game)));
    mFile.items().push_back(Gtk::Menu_Helpers::SeparatorElem());
    mFile.items().push_back(Gtk::Menu_Helpers::MenuElem("_Quit", Gtk::AccelKey("<control>q"),
		sigc::mem_fun(*this, &GtkMainMenu::on_menu_quit)));
    
    Gtk::RadioMenuItem *r;
    items().push_back(Gtk::Menu_Helpers::MenuElem("_Options", mLocalGame));
    mLocalGame.items().push_back(Gtk::Menu_Helpers::RadioMenuElem(modeGroup, "1 player mode",
                                 sigc::bind<int>(sigc::mem_fun(*this, &GtkMainMenu::on_menu_noplayers), 1)));
    r = (Gtk::RadioMenuItem*) &mLocalGame.items().back();
    r->set_active();

    mLocalGame.items().push_back(Gtk::Menu_Helpers::RadioMenuElem(modeGroup, "2 player mode",
                                 sigc::bind<int>(sigc::mem_fun(*this, &GtkMainMenu::on_menu_noplayers), 2)));
                
    mLocalGame.items().push_back(Gtk::Menu_Helpers::SeparatorElem());

    mLocalGame.items().push_back(Gtk::Menu_Helpers::MenuElem("_Difficulty", mDifficulty));
    mDifficulty.items().push_back(Gtk::Menu_Helpers::RadioMenuElem(levelGroup, "Very easy",
                                 sigc::bind<int>(sigc::mem_fun(*this, &GtkMainMenu::on_menu_difficulty), 1)));
    mDifficulty.items().push_back(Gtk::Menu_Helpers::RadioMenuElem(levelGroup, "Easy",
                                 sigc::bind<int>(sigc::mem_fun(*this, &GtkMainMenu::on_menu_difficulty), 2)));
    mDifficulty.items().push_back(Gtk::Menu_Helpers::RadioMenuElem(levelGroup, "Normal",
                                 sigc::bind<int>(sigc::mem_fun(*this, &GtkMainMenu::on_menu_difficulty), 3)));
    r = (Gtk::RadioMenuItem*) &mDifficulty.items().back();
    r->set_active();
    mDifficulty.items().push_back(Gtk::Menu_Helpers::RadioMenuElem(levelGroup, "Hard",
                                 sigc::bind<int>(sigc::mem_fun(*this, &GtkMainMenu::on_menu_difficulty), 4)));
    mDifficulty.items().push_back(Gtk::Menu_Helpers::RadioMenuElem(levelGroup, "Very hard",
                                 sigc::bind<int>(sigc::mem_fun(*this, &GtkMainMenu::on_menu_difficulty), 5)));

//    mDifficulty.items().push_back(Gtk::Menu_Helpers::SeparatorElem());
//    mDifficulty.items().push_back(Gtk::Menu_Helpers::RadioMenuElem(levelGroup, "_Custom ... ",
//                                 sigc::bind<int>(sigc::mem_fun(*this, &GtkMainMenu::on_menu_difficulty), 6)));
    
//    mLocalGame.items().push_back(Gtk::Menu_Helpers::SeparatorElem());

    mLocalGame.items().push_back(Gtk::Menu_Helpers::MenuElem("_Tile set", mTileSet));
    mTileSet.items().push_back(Gtk::Menu_Helpers::RadioMenuElem(tileGroup, "Classic (default)",
                                 sigc::bind<const char*>(sigc::mem_fun(*this, &GtkMainMenu::on_menu_tileset), "default/")));
    r = (Gtk::RadioMenuItem*) &mTileSet.items().back();
    r->set_active();
    mTileSet.items().push_back(Gtk::Menu_Helpers::RadioMenuElem(tileGroup, "Debian",
                                 sigc::bind<const char*>(sigc::mem_fun(*this, &GtkMainMenu::on_menu_tileset), "debian/")));
    mTileSet.items().push_back(Gtk::Menu_Helpers::RadioMenuElem(tileGroup, "Marble",
                                 sigc::bind<const char*>(sigc::mem_fun(*this, &GtkMainMenu::on_menu_tileset), "marble/")));

    mLocalGame.items().push_back(Gtk::Menu_Helpers::MenuElem("Edit board layouts ...", sigc::mem_fun(*this, &GtkMainMenu::on_menu_edit_board)));

//    items().push_back(Gtk::Menu_Helpers::MenuElem("_Network game", mNetworkGame));   
    
    items().push_back(Gtk::Menu_Helpers::MenuElem("_Help", mHelp));   
    mHelp.items().push_back(Gtk::Menu_Helpers::MenuElem("_About", sigc::mem_fun(*this, &GtkMainMenu::on_menu_about)));
}

void GtkMainMenu::on_menu_noplayers(int n)
{
    if(!game)
        return;

    game->setNoOfPlayers(n);
}

void GtkMainMenu::on_menu_difficulty(int n)
{
    if(!game)
        return;
    
    switch(n)
    {
        case 1:
            game->setAILevel(2);
            game->setAITimeout(1000);
            break;
        case 2:
            game->setAILevel(3);
            game->setAITimeout(1000);
            break;
        case 3:
            game->setAILevel(4);
            game->setAITimeout(10000);
            break;
        case 4:
            game->setAILevel(5);
            game->setAITimeout(30000);
            break;
        case 5:
            game->setAILevel(6);
            game->setAITimeout(30000);
            break;
        default:
            break;
    }
}

void GtkMainMenu::on_menu_tileset(const char *tilesetname)
{
    set_tile_set(tilesetname);
}

void GtkMainMenu::on_menu_edit_board()
{
    game->breakMove();
    raiseEditWindow("Edit board layouts.");
}

void GtkMainMenu::on_layout_select_no_action(const libhexx::Layout &l)
{
}

void GtkMainMenu::on_menu_new_game()
{
    game->breakMove();
    GtkEditWindow *w = raiseEditWindow("Select layout for new game.");
    w->signal_on_close.connect(mem_fun(*this, &GtkMainMenu::on_layout_select));
}

void GtkMainMenu::on_layout_select(const libhexx::Layout &l)
{
    game->setToLayout(l);
    game->updateView();
    lastSaveFilename = "";
}

void GtkMainMenu::on_menu_about()
{
    game->breakMove();
    raiseAboutWindow();
}

void GtkMainMenu::on_menu_quit()
{
    game->breakMove();
    Gtk::Main::quit();   
}

bool is_hexxagon_file(const Gtk::FileFilter::Info &filter_info)
{
    std::ifstream f(filter_info.filename.c_str());

    return isValidHexxagonStream(f);
}

void GtkMainMenu::on_menu_open_game()
{
    game->breakMove();

    Gtk::FileChooserDialog dialog("Open hexxagon game from file.");

    // Hexxagon filefilter
    {
        Gtk::FileFilter f;
        f.set_name("Hexxagon games");
        f.add_custom(Gtk::FILE_FILTER_FILENAME, sigc::ptr_fun(&is_hexxagon_file));
        dialog.add_filter(f);
    }
    
    // No filter
    {
        Gtk::FileFilter a;
        a.set_name("All");
        a.add_pattern("*");
        dialog.add_filter(a);
    }

    dialog.set_select_multiple(false);
    dialog.add_button(Gtk::Stock::OK, 1);
    dialog.add_button(Gtk::Stock::CANCEL, 0);
    dialog.present();
   
    int response = dialog.run();

    /* ok was pressed */
    if(response)
    {
        std::ifstream f(dialog.get_filename().c_str());
        
        try
        {
            if(f >> *game)
            {
                game->updateView();

                lastSaveFilename = "";
            }
        }

        catch(Game::ioException e)
        {
            error_dialog("The game could not be loaded. Please make sure that the file i readable and is a valid Hexxagon save file.", Gtk::MESSAGE_ERROR);
        }
    }
}

void GtkMainMenu::on_menu_save_game()
{
    if(lastSaveFilename.length() > 0)
    {
        try
        {
            std::ofstream f(lastSaveFilename.c_str());
            
            if(!f)
                throw Game::ioException();

            if(!(f << *game))
                throw Game::ioException();
        }

        catch(Game::ioException e)
        {
            error_dialog("The game could not be saved. Please make sure that you have the right write privileges and that the disk isn't full.", Gtk::MESSAGE_ERROR);
        }
        
    }
    else
        on_menu_save_as_game();
}

void GtkMainMenu::on_menu_save_as_game()
{
    game->breakMove();
    
    Gtk::FileChooserDialog dialog("Save hexxagon game to file.", Gtk::FILE_CHOOSER_ACTION_SAVE);

    // Hexxagon filefilter
    {
        Gtk::FileFilter f;
        f.set_name("Hexxagon games");
        f.add_custom(Gtk::FILE_FILTER_FILENAME, sigc::ptr_fun(&is_hexxagon_file));
        dialog.add_filter(f);
    }
    
    // No filter
    {
        Gtk::FileFilter a;
        a.set_name("All");
        a.add_pattern("*");
        dialog.add_filter(a);
    }

    dialog.add_button(Gtk::Stock::OK, 1);
    dialog.add_button(Gtk::Stock::CANCEL, 0);
    dialog.present();
   
    int response = dialog.run();

    /* ok was pressed */
    if(response)
    {
        try
        {
            std::ofstream f(dialog.get_filename().c_str());
            
            if(!f)
                throw Game::ioException();

            if(!(f << *game))
                throw Game::ioException();
            
            lastSaveFilename = dialog.get_filename();
        }

        catch(Game::ioException e)
        {
            error_dialog("The game could not be saved. Please make sure that you have the right write privileges and that the disk isn't full.", Gtk::MESSAGE_ERROR);
        }
    }
}


