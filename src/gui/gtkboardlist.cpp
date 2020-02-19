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


#include "gtkboardlist.h"

#include <assert.h>
#include <fstream>
#include <cstdlib>

using namespace libhexx;

GtkBoardList::GtkBoardList()
{
    lstore = Gtk::ListStore::create(colRec);   
    set_model(lstore);
    append_column("Name:", colRec.col_name);

    /* Initialize list */
    if(!read_list_from_file())
        init_default_list();

    selection = get_selection();
    selection->signal_changed().connect(sigc::mem_fun(*this, &GtkBoardList::on_selection_changed));
}

void GtkBoardList::init_default_list()
{
    addBoard("Original", Layout());
    addBoard("Full board", Layout(BitBoard64(0xFFFFFFFF, 0xFFFFFFFF), BitBoard64(0x4000011, 0xF1000004), BitBoard64(0x89DC5, 0x100301E)));
    addBoard("Jing and Jang", Layout(BitBoard64(0xEFDFEFF, 0xFFEFF7EE), BitBoard64(0x1, 0xF0000000), BitBoard64(0x89D6D, 0x738701E)));
    addBoard("North and south", Layout(BitBoard64(0xBFEFF7FF, 0xFFFDFEFF), BitBoard64(0x4000011, 0xF1000004), BitBoard64(0x4089D7C, 0x1638701A)));
    addBoard("Snowflake", Layout(BitBoard64(0xA3FFDFBE, 0xEFBF7FF8), BitBoard64(0x10100, 0xE0101000), BitBoard64(0x44099F7C, 0x1620401A)));
    addBoard("Center capture", Layout(BitBoard64(0xEF6F1FFF, 0xFFFF1EDE), BitBoard64(0x4000001, 0xF0000004), BitBoard64(0x44099F6C, 0x720401E)));
    addBoard("Discus", Layout(BitBoard64(0xFEF6EE7F, 0xFFCEEDEF), BitBoard64(0x8000240, 0xE0480002), BitBoard64(0x4C099D2C, 0x768401C)));
    addBoard("Tight center", Layout(BitBoard64(0xE7FF5DBF, 0xFFB75FFC), BitBoard64(0x20804, 0xE4020800), BitBoard64(0x4C09952C, 0x36A481C)));
}

std::string nameOfSaveFile()
{
    return std::string(getenv("HOME")) + "/.hexxagon";
}

bool GtkBoardList::read_list_from_file()
{
    std::ifstream file(nameOfSaveFile().c_str());
   
    if(!file)
        return false;

    std::list<std::string> names;
    std::list<Layout> layouts;

    while(file)
    {
        std::string str;
        Layout l;
        
        if(!getline(file, str))
        {
            file.close();
            return false;
        }

        if(str == "###END-OF-LAYOUT-LIST###")
            break;
        
        if(!(file >> l))
        {
            file.close();
            return false;
        }

        std::string newln;
        if(!getline(file, newln))
        {
            file.close();
            return false;
        }
      
        names.push_back(str);
        layouts.push_back(l);
    }
  
    while(!names.empty())
    {
        addBoard(names.front(), layouts.front());
        names.pop_front();
        layouts.pop_front();
    }
    
    file.close();

    return true;
}

bool GtkBoardList::write_list_to_file()
{
    std::ofstream file(nameOfSaveFile().c_str());

    if(!file)
        return false;

    /* Save all rows */
    Gtk::TreeModel::Children::iterator iter = lstore->children().begin();
    while(iter != lstore->children().end())
    {
        if(!(file << (*iter)[colRec.col_name] << "\n" << data[(*iter)[colRec.col_name]]))
        {
            file.close();
            return false;
        }
        
        ++iter;
    }
    
    if(!(file << "###END-OF-LAYOUT-LIST###\n"))
    {
        file.close();
        return false;
    }
    
    file.close();
    
    return true;
}

void GtkBoardList::on_selection_changed()
{
    Gtk::TreeModel::iterator iter = selection->get_selected();
    if(iter) //If anything is selected
    {
        Gtk::TreeModel::Row row = *iter;
        callback(row[colRec.col_name], (data.find(row[colRec.col_name]))->second);
    }
}

Glib::ustring GtkBoardList::getSelected()
{
    Gtk::TreeModel::iterator iter = selection->get_selected();
    if(iter) //If anything is selected
    {
        Gtk::TreeModel::Row row = *iter;
        return row[colRec.col_name];
    }
    else
        return Glib::ustring("");
}

bool GtkBoardList::addBoard(const Glib::ustring &name, const libhexx::Layout &l)
{
    /* No duplicate names */
    if(data.find(name) != data.end())
        return true;

    /* Insert in sorted order */
    Gtk::TreeModel::Children::iterator iter = lstore->children().begin();
    while(iter != lstore->children().end())
    {
        if(name < (*iter)[colRec.col_name])
            break;

        ++iter;
    }
    
    Gtk::TreeModel::Row row;
    
    if(iter == lstore->children().end())
        row = *(lstore->append());
    else
        row = *(lstore->insert(iter));
    
    row[colRec.col_name] = name;

    data[name] = l;

    return false;
}

bool GtkBoardList::removeBoard(const Glib::ustring &name)
{
    /* Is there a name in the list? */
    if(data.find(name) == data.end())
        return true;
    
    /* Find the row */
    Gtk::TreeModel::Children::iterator iter = lstore->children().begin();
    while(iter != lstore->children().end())
    {
        if(name == (*iter)[colRec.col_name])
            break;

        ++iter;
    }
    
    assert(iter != lstore->children().end());

    lstore->erase(iter);
   
    data.erase(data.find(name));
    
    return false;
}

void GtkBoardList::selectFirst()
{
    selection->select(lstore->children().begin());
    on_selection_changed();
}
