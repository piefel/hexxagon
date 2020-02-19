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


#ifndef _GAME_H
#define _GAME_H

#include "board.h"
#include "view.h"

#include <list>

namespace libhexx
{
	class Game
	{
	public:
        class ioException
        {
        };

		Game();
		Game(Layout l);

		virtual ~Game() { boardList.clear(); };
		
		virtual int getTurn() const { return pos->second; };
		
		int next();
		int prev();
		
        bool isNext();
		bool isPrev();
		
        void setToLayout(Layout l);
		
        virtual void reset();
		virtual void destroyRest();
		
		virtual int applyMove(const Move &move);
		
		int nrBoards() const;
		
		const Board& getBoard() const { return pos->first; };
        
        BitBoard64 getMask() const { return boardMask; };
        int getBrickCount(int player) const { return getBoard().countBricks((getTurn()+!player) % 2 + 1, boardMask); };
        
		int endOfGame() const { return getBoard().isGameOver(lookUp); };
        int moveExists() const { return getBoard().moveExists(lookUp); };
        int isMoveValid(Move &move) const { return getBoard().isMoveValid(move, lookUp);};
		
        Move computerMove(int depth, bool (*callback)(), int maxtime);
        Move computerHint(int depth, bool (*callback)(), int maxtime);

		void displayText() const { getBoard().displayBoardText(lookUp, getTurn()); };
		
		friend std::ostream& operator<<(std::ostream &output, const Game &g);
		friend std::istream& operator>>(std::istream &input, Game &g);
		
	protected:

        void setMask(const BitBoard64 &b) 
        { 
            boardMask = b; 
            lookUp.init(boardMask);
        };
        
		void loadV1(std::istream &input, Game &g);
		void loadV2(std::istream &input, Game &g);

		BitBoard64 boardMask; //Which cells are masked away?
		LookUp lookUp;
		
        std::list<std::pair<Board, uint8_t> > boardList;
        std::list<std::pair<Board, uint8_t> >::iterator pos;
	};
}

bool isValidHexxagonStream(std::istream &in);

#endif // _GAME_H

