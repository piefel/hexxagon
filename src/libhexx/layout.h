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


#ifndef _LAYOUT_H
#define _LAYOUT_H

#include "bitboard64.h"
#include <iostream>

namespace libhexx
{
    class Layout 
    {
        public:
            Layout()
            {
                board = BitBoard64(0,0);
                color = BitBoard64(0,0);
                
                board.setBit( 0); color.setBit(0);
                board.setBit(34); color.setBit(34);
                board.setBit(56); color.setBit(56);
                board.setBit( 4);
                board.setBit(26);
                board.setBit(60);
                
                mask = BitBoard64(0xFFFFFFFF, 0x1FFFFFFF);
                mask.unSetBit(22);
                mask.unSetBit(39);
                mask.unSetBit(29);
            };
            
            Layout(libhexx::BitBoard64 m, libhexx::BitBoard64 b, libhexx::BitBoard64 c) : mask(m), board(b), color(c)
            {
                /* mask away the last 3 unused bits. Just to be safe */
                BitBoard64 x(0xFFFFFFFF, 0x1FFFFFFF); 
                mask = mask & x;
                board = board & x;
            };

            libhexx::BitBoard64 getMask() const
            {
                return mask;
            };
            
            libhexx::BitBoard64 getBoard() const
            {
                return board;
            };
            
            libhexx::BitBoard64 getColor() const
            {
                return color;
            };
      
            bool isValid();
            
            friend std::istream& operator>>(std::istream &input, class Layout &l);
            friend std::ostream& operator<<(std::ostream &output, const class Layout &l);
        
        protected:

            libhexx::BitBoard64 mask;
            libhexx::BitBoard64 board;
            libhexx::BitBoard64 color;
    };
}

#endif // _LAYOUT_H

