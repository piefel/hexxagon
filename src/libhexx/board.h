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


#ifndef _BOARD_H
#define _BOARD_H

#include "bitboard64.h"
#include "lookup.h"
#include "move.h"
#include "layout.h"

#include <iostream>
#include <vector>

namespace libhexx 
{
    class Move;
    class MoveList;

    enum 
    {
        SCR_WIN = 20000,
        SCR_INFINITY = 32000,
        SCR_BREAK = 999999
    };

    /* Hexxagon type */
    enum 
    {
        pl1 = 1,
        pl2,
        empty,
        removed
    };

    int getHexxagonIndex(int x, int y);
    void initCloneLookups();
    void initJumpLookups();

    class Board
    {
        public:
            Board();
            Board(const Layout &l);
            Board(const Board &copy);

            void setToLayout(const Layout &l);
            int evaluate(const LookUp &l, bool invert) const;
            int countBricks(int player, BitBoard64 mask) const;
            int getHexxagon(int i) const;
            int applyMove(const Move &move, const LookUp &l);

            bool generateMoveList(std::vector<Move> &moves, const LookUp& l) const;

            Move computerMove(const LookUp &l, int depth, bool (*callback)(), int maxtime);

            bool moveExists(const LookUp& l) const;
            bool isGameOver(const LookUp &l) const;
            bool isMoveValid(const Move &move, const LookUp &lookUp) const;

            BitBoard64 getBBBoard() const { return board; };
            BitBoard64 getBBColor() const { return color; };

            void invertColors() { color = ~color; };
            
            Board operator=(const Board &right);

            friend std::istream& operator>>(std::istream &input, class Board &b);
            friend std::ostream& operator<<(std::ostream &output, const class Board &b);

            void displayBoardText(const LookUp& l, int turn) const;

        protected:
            BitBoard64 board;
            BitBoard64 color;
    };
} // namespace libhexx

#endif // _BOARD_H

