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
#include <iostream>
#include <cstdlib>
#include <vector>

namespace {
    std::string tileset = "default/";
    std::vector<sigc::slot<void> > tileChangeListeners;
}

Glib::RefPtr<Gdk::Pixbuf> open_pixmap_from_file(const std::string& pixmapname)
{
    std::string filename = std::string(IMAGE_INSTALL_DIR) + tileset + pixmapname + ".png";
    std::string defaultfilename = std::string(IMAGE_INSTALL_DIR) + pixmapname + ".png";
    Glib::RefPtr<Gdk::Pixbuf> ret;
   
    try
    {
	if (!Glib::file_test(filename, Glib::FILE_TEST_EXISTS) &&
		Glib::file_test(defaultfilename, Glib::FILE_TEST_EXISTS))
	    ret = Gdk::Pixbuf::create_from_file(defaultfilename);
	else
	    ret = Gdk::Pixbuf::create_from_file(filename);
    }
    
    catch(Glib::FileError e)
    {
        std::cout << "Could not open image: " << filename << ", aborting!\n";
        exit(-1);
    }

    catch(Gdk::PixbufError)
    {
        std::cout << "Could not parse image: " << filename << ", aborting!\n";
        exit(-1);
    }

    return ret;
}

void set_tile_set(const std::string& tilesetname)
{
    if (tileset != tilesetname) {
	tileset = tilesetname;
	for(std::vector<sigc::slot<void> >::iterator i = tileChangeListeners.begin();
		i != tileChangeListeners.end(); i++)
	    (*i)(); 
    }
}

void add_tileChangeListener(sigc::slot<void> listener) {
    tileChangeListeners.push_back(listener);
}

void error_dialog(const Glib::ustring &text, Gtk::MessageType mt)
{
    Gtk::MessageDialog dialog(text, false, mt);

    dialog.run();
}
