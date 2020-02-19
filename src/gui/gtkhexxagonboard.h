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


#ifndef _GTKHEXXAGON_BOARD_H
#define _GTKHEXXAGON_BOARD_H
 
#include <gtkmm.h>
#include <libhexx/libhexx.h>

using namespace libhexx;

class GtkHexxagonBoard : public Gtk::DrawingArea
{
    public:
        GtkHexxagonBoard();

	void operator=(const Board &right);
        void setBoardMask(const BitBoard64 &right);
        
        int getSelection() const { return selectedCell; };
        void clearSelection() { selectedCell = -1; };
        void setSelection(int cell) { selectedCell = cell; };

        void setCompHighlight(int from, int to) { compFrom = from; compTo = to; };
        void clearCompHighlight() { compFrom = -1; compTo = -1; };
        
        int getCellFromPos(int inx, int iny);

    private:
        Glib::RefPtr<Gdk::Pixbuf> buf_board[7];
        Glib::RefPtr<Gdk::Pixmap> map_board[7];
        Glib::RefPtr<Gdk::Bitmap> mask[7];
        
        Glib::RefPtr<Gdk::Pixmap> back;

	void setPixmaps();
	void fake_configure_event();
        
    protected:
        virtual bool on_button_press_event(GdkEventButton *event);
        virtual bool on_button_release_event(GdkEventButton *event);
        
        virtual bool on_configure_event(GdkEventConfigure *event);

        virtual bool on_cell_press_event(GdkEventButton *event, int cell);
        virtual bool on_cell_release_event(GdkEventButton *event, int cell);

        bool on_expose_event(GdkEventExpose *event);
        void rescalePixmaps();

        void draw();

        BitBoard64 bbBoard;
		BitBoard64 bbColor;
        BitBoard64 bbMask;
       
        int selectedCell;

        int compFrom;
        int compTo;
};

#endif //_GTKHEXXAGON_BOARD_H
