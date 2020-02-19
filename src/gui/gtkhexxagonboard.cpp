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
#include "gtkhexxagonboard.h"
#include <iostream>
#include <stdio.h>
#include <math.h>

using namespace std;

GtkHexxagonBoard::GtkHexxagonBoard() : bbBoard(0,0), bbColor(0,0), bbMask(0,0)
{
    set_events(Gdk::EXPOSURE_MASK | Gdk::STRUCTURE_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);

    setPixmaps();
    add_tileChangeListener(sigc::mem_fun(*this, &GtkHexxagonBoard::setPixmaps));
    add_tileChangeListener(sigc::mem_fun(*this, &GtkHexxagonBoard::fake_configure_event));

    clearSelection();
    clearCompHighlight();
    
    /* We will do our own. Since there is no way to
     * control when to flip the screen. */
    set_double_buffered(0);
}

void GtkHexxagonBoard::setPixmaps()
{
    buf_board[0] = open_pixmap_from_file("board_N_0");
    buf_board[1] = open_pixmap_from_file("board_L_0");
    buf_board[2] = open_pixmap_from_file("board_D_0");
    buf_board[3] = open_pixmap_from_file("board_N_1");
    buf_board[4] = open_pixmap_from_file("board_N_2");
    buf_board[5] = open_pixmap_from_file("board_L_1");
    buf_board[6] = open_pixmap_from_file("board_L_2");
}

void GtkHexxagonBoard::operator=(const Board &right)
{
	bbColor = right.getBBColor();
	bbBoard = right.getBBBoard();
}

void GtkHexxagonBoard::setBoardMask(const BitBoard64 &right)
{
    bbMask = right;   
}

void boardSizeInWindow(int &width, int &height)
{
    // For width of hexagon==1, width of board is 7, height is 7.79423 (ie. sqrt(3)/2*9)
    if (width * 7.79423 / 7 > height)
	width = static_cast<int>(height * 7 / 7.79423);
    else
	height = static_cast<int>(width * 7.79423 / 7);
}

static double adjust = 0.03; 
static double offset = 0.02;

void GtkHexxagonBoard::draw()
{
    if(!back)
        return;

    int width = get_allocation().get_width();
    int height = get_allocation().get_height();

    /* clear */
    Glib::RefPtr<Gdk::GC> gc = Gdk::GC::create(get_window());

    Gdk::Rectangle rec(0,0, width, height);
    gc->set_clip_rectangle(rec);

    Gdk::Color cx;
    cx.set_rgb(0, 0, 0);
    get_window()->get_colormap()->alloc_color(cx);
    gc->set_foreground(cx);

    back->draw_rectangle(gc, true, 0, 0, width, height); 
    gc.clear();

    int windowwidth = width, windowheight = height;
    boardSizeInWindow(width, height);

    int xoff = (windowwidth > width ? (windowwidth - width) / 2 : 0);
    int yoff = (windowheight > height ? (windowheight - height) / 2 : 0);

    int cellNo = 0;
    for(int y = 0; y < 9; y++)
    {
        for(int x = 0; x < 9; x++)
        {
            if(x - y >= 5)
                continue;
            if(y - x >= 5)
                continue;

            if(!bbMask.getBit(cellNo))
            {
                cellNo++;
                continue;
            }

            // Width and height
            double w = (width  - width  * offset) / (9 - 8*0.25); 
            double h = (height - height * offset) / 9;   

            // Step size
            double xs = w * .748; 

            double posx = round(xs * x);
            double posy = round(2 * h - (h * x / 2) + h*y);

            posx -= w * adjust / 2;
            posy -= h * adjust / 2;

            posx += width  * offset / 2;
            posy += height * offset / 2;

            int nx = (int) round(posx) + xoff;
            int ny = (int) round(posy) + yoff;

            int type;
            LookUp lookup(bbMask);

            if(bbBoard.getBit(cellNo))
            {
                if(bbColor.getBit(cellNo))
                    type = 4;
                else
                    type = 3;
   
                if(cellNo == compFrom)
                   type += 2; 
            }
            else
            {
                type = 0;

                if(selectedCell != -1)
                {
                    if((~bbBoard & lookup.getClone(selectedCell)).getBit(cellNo))
                        type = 1;
                    else if((~bbBoard & lookup.getJump(selectedCell)).getBit(cellNo))
                        type = 2;
                }
                else if(compTo == cellNo)
                {   
                    type = 2;
                }
            }

            Glib::RefPtr<Gdk::GC> gc = Gdk::GC::create(back);
            gc->set_clip_mask(mask[type]);
            gc->set_clip_origin(nx, ny);

            if(back && map_board[type])
                back->draw_drawable(gc, map_board[type], 0, 0, nx, ny, -1, -1);

            cellNo++;
        }
    }

    queue_draw();
}

int GtkHexxagonBoard::getCellFromPos(int inx, int iny)
{
    int width = get_allocation().get_width();
    int height = get_allocation().get_height();

    int windowwidth = width, windowheight = height;
    boardSizeInWindow(width, height);

    int xoff = (windowwidth > width ? (windowwidth - width) / 2 : 0);
    int yoff = (windowheight > height ? (windowheight - height) / 2 : 0);

    // What cell is this in?
    int hit = -1;
    int cellNo = 0;
    for(int y = 0; y < 9; y++)
    {
        for(int x = 0; x < 9; x++)
        {
            if(x - y >= 5)
                continue;
            if(y - x >= 5)
                continue;
           
            // Width and hight
            double w = (width  - width  * offset) / (9 - 8*0.25);
            double h = (height - height * offset) / 9;
            
            // Step size
            double xs = w * .748; 
            
            double posx = round(xs * x);
            double posy = round(2 * h - (h * x / 2) + h*y);

            posx -= w * adjust / 2;
            posy -= h * adjust / 2;
           
            posx += width  * offset / 2;
            posy += height * offset / 2;
            
            int nx = (int) round(posx + w / 2) + xoff;
            int ny = (int) round(posy + h / 2) + yoff;
        
            // Now we got the place for cell cellNo
            double dx = inx - nx;
            double dy = iny - ny;
            double a = (w/2)*(w/2);
            double b  = (h/2)*(h/2);
            
            if(dx*dx/a + dy*dy/b < 1)
            {
                hit = cellNo;
            }

            cellNo++;
        }
    }

    return hit; 
}

bool GtkHexxagonBoard::on_button_press_event(GdkEventButton* event)
{
    int hit = getCellFromPos((int) event->x, (int)event->y);
    
    return on_cell_press_event(event, hit);
}

bool GtkHexxagonBoard::on_button_release_event(GdkEventButton* event)
{
    int hit = getCellFromPos((int)event->x, (int)event->y);

    return on_cell_release_event(event, hit);
}

bool GtkHexxagonBoard::on_cell_press_event(GdkEventButton* event, int cell)
{
    return true;
}

bool GtkHexxagonBoard::on_cell_release_event(GdkEventButton* event, int cell)
{
    return true;
}

void GtkHexxagonBoard::fake_configure_event()
{
    on_configure_event(0);
}

bool GtkHexxagonBoard::on_configure_event(GdkEventConfigure *event)
{
    int width = get_allocation().get_width();
    int height = get_allocation().get_height();

    if(!(width > 20 && height > 20))
        return true;

    if(back)
        back.clear();

    back = Gdk::Pixmap::create(get_window(), width, height);

    // clear...
    Glib::RefPtr<Gdk::GC> gc = Gdk::GC::create(get_window());
    
    Gdk::Rectangle rec(0,0, width, height);
    gc->set_clip_rectangle(rec);
    
    Gdk::Color cx;
    cx.set_rgb(0, 0, 0);
    get_window()->get_colormap()->alloc_color(cx);
    gc->set_foreground(cx);
    
    back->draw_rectangle(gc, true, 0, 0, width, height); 
    gc.clear();
    
    boardSizeInWindow(width, height);
    
    // There is a horizontal overlap of 0.25 of the tile's width
    double w = (width  - width  * offset) / (9 - 8*0.25); 
    double h = (height - height * offset) / 9;   

    w *= adjust + 1;
    h *= adjust + 1;

    for(int i = 0; i < 7; i++)
    {
        Glib::RefPtr<Gdk::Pixbuf> resizebuf = buf_board[i]->scale_simple((int) round(w), (int) round(h), Gdk::INTERP_BILINEAR);

        resizebuf->render_pixmap_and_mask(map_board[i], mask[i], 228);
    }

    draw();
    
    return false;
}

bool GtkHexxagonBoard::on_expose_event(GdkEventExpose *event)
{
    int width = get_allocation().get_width();
    int height = get_allocation().get_height();

    if(back)
    {
        Glib::RefPtr<Gdk::GC> gc = Gdk::GC::create(get_window());
        Gdk::Rectangle rec(0,0, width, height);
        gc->set_clip_rectangle(rec);
        get_window()->draw_drawable(gc, back, 0, 0, 0, 0);
        gc.clear();
    }

    return false;
}
