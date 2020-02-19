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


#include "gtkaboutwindow.h"
#include "helper_functions.h"

#include <iostream>

const char *about_text =  "Hexxagon v1.0\n"\
                          "Copyright 2004 Erik Jonsson\n"\
                          "\n"\
                          "Contact me on:\n"\
                          "hexxagon@nesqi.se\n"\
                          "\n"\
                          "The latest version can always be found on:\n"\
                          "http://www.nesqi.se/hexxagon/\n"\
                          "\n"\
                          "This program is free software; you can redistribute it and/or\n"\
                          "modify it under the terms of the GNU General Public License\n"\
                          "as published by the Free Software Foundation; either version 2\n"\
                          "of the License, or (at your option) any later version.\n"\
                          "\n"\
                          "This program is distributed in the hope that it will be useful,\n"\
                          "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"\
                          "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"\
                          "See the GNU General Public License for more details.\n"\
                          "\n"\
                          "You should have received a copy of the GNU General Public License\n"\
                          "along with this program; if not, write to the Free Software\n"\
                          "Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.";


GtkAboutWindow *globalAboutWindow = NULL;

GtkAboutWindow::GtkAboutWindow() : text(about_text, Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER), close(Gtk::Stock::CLOSE)
{
    /* Vertical box top: Menu, Navbar */
    add(vbox);
    vbox.add(frame);
    frame.add(fbox);
    fbox.add(image);
    fbox.add(text);
    vbox.pack_end(close, Gtk::PACK_SHRINK);
    fbox.set_border_width(8);
    vbox.set_border_width(8);
    set_default_size(59, 60);
    fbox.set_spacing(8); 
    vbox.set_spacing(8);
    close.signal_clicked().connect(sigc::mem_fun(*this, &GtkAboutWindow::on_close));
    image.set(open_pixmap_from_file("about_top"));
    set_resizable(false);

    show_all();

    present();
}

void raiseAboutWindow()
{
    if(globalAboutWindow)
        globalAboutWindow->present();
    else
        globalAboutWindow = new GtkAboutWindow();
}


bool GtkAboutWindow::on_configure_event(GdkEventConfigure *event)
{
    Gtk::Window::on_configure_event(event);

    return false;
}

void GtkAboutWindow::on_close()
{
    delete this;
}

GtkAboutWindow::~GtkAboutWindow()
{
    globalAboutWindow = NULL;
}

