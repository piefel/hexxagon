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
#include <libhexx/libhexx.h>
#include "gamemodel.h"
#include "gtkscaleimage.h"

class GtkScore : public Gtk::Frame, public libhexx::View
{
    public:
        GtkScore(GameModel *g);

        virtual void updateView();
        void setGame(GameModel *g) { game = g; };

    private:
        GtkScore();

	void setPixmaps();

        Gtk::VBox vbox;
            Gtk::Frame turnFrame;
                Gtk::VBox vboxturn;
                    GtkScaleImage pl1timg;
                    GtkScaleImage pl2timg;
                    Gtk::Label turnlabel; 
        
            Gtk::Frame computerFrame;
                Gtk::VBox vboxcomputer;
                    Gtk::ProgressBar thinkprogress;
                
            Gtk::Frame pl1Frame;
                Gtk::VBox vboxf1;
                    GtkScaleImage pl1img;
                    Gtk::ProgressBar pl1Bar;

            Gtk::Frame pl2Frame;
                Gtk::VBox vboxf2;
                    GtkScaleImage pl2img;
                    Gtk::ProgressBar pl2Bar;
        

        bool turn1active;
        bool turn2active;
        
    protected:
        GameModel *game;
};
