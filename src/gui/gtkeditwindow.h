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


#ifndef _GTKEDITWINDOW_H
#define _GTKEDITWINDOW_H

#include <gtkmm.h>
#include "gtkhexxagonboardedit.h"
#include "gtkboardlist.h"

class GtkEditWindow : public Gtk::Window
{
    public:
        GtkEditWindow();

        virtual bool on_configure_event(GdkEventConfigure *event);

        void on_ok_button();
        void on_cancel_button();
        void on_save_button();
        void on_delete_button();
        
        void on_close();
        void activate();
        
        void on_selection_changed(const Glib::ustring &name, const libhexx::Layout &l);
        
        void set_title(const Glib::ustring &title);

        virtual ~GtkEditWindow();
            
        sigc::signal<void, const libhexx::Layout&> signal_on_close;
    
    private:

        Gtk::VBox vbox;
            Gtk::Frame frame;
                Gtk::VBox fvbox;
                    Gtk::HPaned hpaned;
                        Gtk::ScrolledWindow scrollWin;
                            GtkBoardList list;
                        GtkHexxagonBoardEdit board;
                    Gtk::HBox fhbox;
                        Gtk::Button delBtn;
                        Gtk::Button saveBtn;
                        Gtk::Label nameLbl;
                        Gtk::Entry nameEntry;
            Gtk::HBox lowBox;
                Gtk::Button okBtn;
                Gtk::Button cancelBtn;
};

GtkEditWindow* raiseEditWindow(const Glib::ustring &title);

#endif //_GTKEDITWINDOW_H
