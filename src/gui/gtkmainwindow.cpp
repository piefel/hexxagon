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


#include <libhexx/libhexx.h>
#include "gtkmainwindow.h"

using namespace libhexx;

GtkMainWindow::GtkMainWindow() : score(&game), hexBoard(&game), navBar(&game), menu(&game), gameoverdialog(&game)
{
    /* Vertical box top: Menu, Navbar */
    add(vbox);
    vbox.pack_start(topBox, Gtk::PACK_SHRINK);

    topBox.pack_start(menu, Gtk::PACK_EXPAND_WIDGET);
    topBox.pack_start(navBar, Gtk::PACK_SHRINK);

    /* Vertical box middle */
    vbox.pack_start(lowBox, Gtk::PACK_EXPAND_WIDGET);
    lowBox.pack_start(hexBoard, Gtk::PACK_EXPAND_WIDGET);
    lowBox.pack_start(score, Gtk::PACK_SHRINK);
    
    /* Set up game */
    game.addView(&hexBoard);
    game.addView(&score);
    game.addView(&gameoverdialog);
    game.addView(&navBar);
    
    set_default_size(750, 650);
    
    /* Make main window visible */
    show_all_children();
}

bool GtkMainWindow::on_configure_event(GdkEventConfigure *event)
{
    Gtk::Window::on_configure_event(event);
    
    return true;
}

