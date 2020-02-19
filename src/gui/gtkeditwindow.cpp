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
#include "gtkeditwindow.h"
#include <iostream>

#include "gtkhexxagonboardplay.h"

GtkEditWindow *globalEditWindow = NULL;

GtkEditWindow::GtkEditWindow() : delBtn(Gtk::Stock::DELETE),
                                 saveBtn(Gtk::Stock::SAVE),
                                 okBtn(Gtk::Stock::OK),
                                 cancelBtn(Gtk::Stock::CANCEL)
{
    /* Set up gui */
    add(vbox);
    {
        vbox.set_border_width(8);
        vbox.set_spacing(8);

        vbox.pack_start(frame, Gtk::PACK_EXPAND_WIDGET);
        {
            frame.set_label("Select and edit boards:");

            frame.add(fvbox);
            {
                fvbox.set_border_width(8);
                fvbox.set_spacing(4);

                fvbox.pack_start(hpaned, Gtk::PACK_EXPAND_WIDGET);
                {
                    hpaned.pack1(scrollWin, Gtk::SHRINK);
                    {
                        scrollWin.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
                        scrollWin.add(list);
                    }

                    hpaned.pack2(board);
                }
                
                fvbox.pack_start(fhbox, Gtk::PACK_SHRINK);
                {
                    fhbox.pack_start(delBtn, Gtk::PACK_SHRINK);
                    fhbox.pack_start(saveBtn, Gtk::PACK_SHRINK);
                    fhbox.pack_start(nameLbl, Gtk::PACK_SHRINK);
                    fhbox.pack_start(nameEntry, Gtk::PACK_EXPAND_WIDGET);
                    nameEntry.set_max_length(28);
                }
            }
        }
        
        vbox.pack_start(lowBox, Gtk::PACK_SHRINK);
        {
            lowBox.pack_start(okBtn, Gtk::PACK_EXPAND_WIDGET);
            lowBox.pack_start(cancelBtn, Gtk::PACK_EXPAND_WIDGET);
        }
    }
   
    /* connect signals */
    okBtn.signal_clicked().connect(sigc::mem_fun(*this, &GtkEditWindow::on_ok_button));
    cancelBtn.signal_clicked().connect(sigc::mem_fun(*this, &GtkEditWindow::on_cancel_button));
    delBtn.signal_clicked().connect(sigc::mem_fun(*this, &GtkEditWindow::on_delete_button));
    saveBtn.signal_clicked().connect(sigc::mem_fun(*this, &GtkEditWindow::on_save_button));
    
    /* Set up window */
    set_default_size(650, 600);
    set_modal(true);
    show_all();

    present();

    list.callback.connect(sigc::mem_fun(*this, &GtkEditWindow::on_selection_changed));

    list.selectFirst();
}

GtkEditWindow* raiseEditWindow(const Glib::ustring &title)
{
    if(globalEditWindow)
        globalEditWindow->present();
    else
        globalEditWindow = new GtkEditWindow();
    
    globalEditWindow->set_title(title);  

    return globalEditWindow;
}

void GtkEditWindow::set_title(const Glib::ustring &title)
{
    Gtk::Window::set_title(title);
}

void GtkEditWindow::on_selection_changed(const Glib::ustring &name, const libhexx::Layout &l)
{
    board.setLayout(l); 
}

bool GtkEditWindow::on_configure_event(GdkEventConfigure *event)
{
    Gtk::Window::on_configure_event(event);

    return false;
}

void GtkEditWindow::on_ok_button()
{
    if(board.getLayout().isValid())
    {
        hide();
        signal_on_close(board.getLayout());    
        signal_on_close.clear();
    }
    else
        error_dialog("This is not a valid board. There has to "\
                     "be at least one piece of each color and "\
                     "player 1 must be able to make a move.", Gtk::MESSAGE_INFO);
}

void GtkEditWindow::on_cancel_button()
{
    hide();
    signal_on_close.clear();
}


void GtkEditWindow::on_save_button()
{
    if(nameEntry.get_text().length() > 0 && !board.getLayout().isValid())
    {
        error_dialog("This is not a valid board. There has to "\
                     "be at least one piece of each color and "\
                     "player 1 must be able to make a move. "\
                     "Changes to the list will be saved but this "\
                     "board will not be added.", Gtk::MESSAGE_INFO);
    }
    else if(nameEntry.get_text().length() > 0)
    {
        if(list.addBoard(nameEntry.get_text(), board.getLayout()))
        {
            error_dialog("There exists a layout with this name already. "\
                         "Change the name or delete the old one.", Gtk::MESSAGE_INFO);
        }
        
        nameEntry.set_text("");
    }
       
    if(!list.write_list_to_file())
    {
        error_dialog("For some reason the changes you have made could not be saved. "\
                     "Make sure that the file ~/.hexxagon is writable.", Gtk::MESSAGE_ERROR);
    }
}

void GtkEditWindow::on_delete_button()
{
    list.removeBoard(list.getSelected());
}

void GtkEditWindow::on_close()
{
    delete this;
}

GtkEditWindow::~GtkEditWindow()
{
    globalEditWindow = NULL;
}

