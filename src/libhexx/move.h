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


#ifndef _HEXXAGONMOVE_H
#define _HEXXAGONMOVE_H

#include "board.h"

#include <list>
#include <vector>

namespace libhexx 
{
    class Move
    {
        public:
            Move() { from = 99; to = 99;};
            Move(int t) { from = t; to = t; };
            Move(int f, int t) { from = f; to = t; };

            inline bool operator<(const Move &r) const
            {
                return (score < r.score);
            };
    
            inline bool operator!=(const Move &r) const
            {
                return (score != r.score);
            };
          
            operator bool() const { return from != 99 && to != 99; };
            
            char from, to;
            int score;
    };
    
    bool scoreMoves(std::vector<Move> &moves, class Board board, const LookUp& lookUp, int depth, bool (*callback)(), int maxtime);
} //namespace libhexx

#endif // _HEXXAGONMOVE_H
