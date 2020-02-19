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


#ifndef _GTKBOARDLIST_H
#define _GTKBOARDLIST_H

#include <gtkmm.h>
#include <libhexx/libhexx.h>

#include <map>

class GtkBoardList : public Gtk::TreeView
{
    public:
        GtkBoardList();
       
        void init_default_list();
        bool read_list_from_file();
        bool write_list_to_file();

        Glib::ustring getSelected();
        bool addBoard(const Glib::ustring &name, const libhexx::Layout &l);
        bool removeBoard(const Glib::ustring &name);
        void selectFirst();

        void on_selection_changed();

        sigc::signal<void, const Glib::ustring&, const libhexx::Layout&> callback; 
        
    private:
        class ModelColumns : public Gtk::TreeModel::ColumnRecord
        {
            public:

                ModelColumns()
                { 
                    add(col_name); 
                };

                Gtk::TreeModelColumn<Glib::ustring> col_name;
        };

        Glib::RefPtr<Gtk::ListStore> lstore;
        ModelColumns colRec;

        Glib::RefPtr<Gtk::TreeSelection> selection;
        
        std::map<Glib::ustring, libhexx::Layout> data;
};

#endif //_GTKBOARDLIST_H

