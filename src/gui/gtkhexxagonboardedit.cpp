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


#include "gtkhexxagonboardedit.h"

GtkHexxagonBoardEdit::GtkHexxagonBoardEdit() 
{
    bbMask = BitBoard64(0, 0);
}

void GtkHexxagonBoardEdit::setLayout(const libhexx::Layout &l)
{
    bbMask = l.getMask();
    bbColor = l.getColor();
    bbBoard = l.getBoard();
    draw();
}

libhexx::Layout GtkHexxagonBoardEdit::getLayout()
{
    return libhexx::Layout(bbMask, bbBoard, bbColor); 
}

bool GtkHexxagonBoardEdit::on_cell_press_event(GdkEventButton* event, int cell)
{
    /* change color */
    if(event->button == 1)
    {
        BitBoard64 bb(0,0);
        bb.setBit(cell);
   
        bbColor = (~bbColor & bb) | (bbColor & ~bb);       
        bbBoard = bbBoard | bb;
                
        draw();
    }

    /* change empty/invalid */
    if(event->button == 3)
    {
        BitBoard64 bb(0,0);
        bb.setBit(cell);
      
        if((bbBoard & bbMask & bb | (~bbMask & bb)).empty())
        {
            bbMask = bbMask & ~bb;
        }
        else
        {
            bbMask = bbMask | bb;
            bbBoard = bbBoard & ~bb;
        }
        
        draw();
    }

    //bbMask.printInHex(); bbBoard.printInHex(); bbColor.printInHex();
    
    return true;
}

bool GtkHexxagonBoardEdit::on_cell_release_event(GdkEventButton* event, int cell)
{
    
    return true;
}

