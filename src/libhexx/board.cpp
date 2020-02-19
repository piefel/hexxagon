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


#include "board.h"
#include "move.h"
#include <iostream>
#include <stdlib.h>

using namespace libhexx;

Board::Board()
{
    setToLayout(Layout());
}

Board::Board(const Board &copy)
{
	board = copy.board;
	color = copy.color;
}

Board::Board(const Layout &l)
{
    setToLayout(l);
}

Board Board::operator=(const Board &right)
{
	color = right.color;
	board = right.board;

	return *this;
}

void Board::setToLayout(const Layout &l)
{
	board = l.getBoard();
	color = l.getColor();
}

int Board::countBricks(int player, BitBoard64 mask) const 
{
	int good = 0, bad = 0;

	for(int i = 0; i < 61; i++)
	{
		if((board & mask).getBit(i))
		{
			if(color.getBit(i))
				good++;
			else
				bad++;
		}
	}

	if(player == 0)
		return good - bad;
	else if(player == 1)
		return good;
	else if(player == 2)
		return bad;
	
	return 0;
}

int Board::evaluate(const LookUp &l, bool invert) const
{
    int good = 0; 
    int bad = 0;
    int count = 0;

    for(int i = 0; i < 61; i++)
    {
        if(board.getBit(i))
        {
            count++;
            if(color.getBit(i))
                good++;
            else
                bad++;
        }
    }

    int score = good - bad;
    
    if(!good && bad)
        return score - SCR_WIN;

    if(good && !bad)
        return score + SCR_WIN;

    if(isGameOver(l))
    {   
        if(invert)
        {
            if(good - bad > 0)
                return score + SCR_WIN;
        }
        else
        {
            if(good - bad < 0)
                return score - SCR_WIN;
        }
    }
    
    return score;
}

int Board::getHexxagon(int i) const
{
    if(board.getBit(i))
    {
        if(color.getBit(i))
            return pl1;
        else
            return pl2;
    } else
        return empty;
}

int Board::applyMove(const Move &move, const LookUp &l)
{
    board.setBit(move.to);
    color.setBit(move.to);

    color = color | l.getClone(move.to);

    if(move.from != move.to) // Jump.
        board.unSetBit(move.from);

    color = ~color;

    return 0;
}

bool Board::isMoveValid(const Move &move, const LookUp &l) const
{
    BitBoard64 mine = color & board;

    if(!board.getBit(move.to))
    {
        if(move.from == move.to) // clone
        {
            if((mine & l.getClone(move.to)).notEmpty())
                return true;
        }
        else // Jump
        {
            BitBoard64 tmp(0,0);
            tmp.setBit(move.from);

            if((tmp & l.getJump(move.to) & mine).notEmpty())
                return true;
        }
    }

    return false;
}

bool Board::generateMoveList(std::vector<Move> &movelist, const LookUp &l) const
{
	for(int i = 0; i < 61; i++)
	{
		if(!board.getBit(i)) // Found place to clone / jump.
		{
			if((color & board & l.getClone(i)).notEmpty()) // Clone.
			{
				Move move = Move(i);
				movelist.push_back(move);
			}
			
			BitBoard64 moves;
			if((moves = ((board & l.getJump(i)) & color)).notEmpty()) // Jump.
			{
				for(int j = 0; j < 61; j++)
				{
					if(moves.getBit(j))
					{
						Move move = Move(j, i);
						movelist.push_back(move);
					}
				}
			}
		}
	}

	if(movelist.empty())
		return false;

	return true;
}

bool Board::moveExists(const LookUp& l) const
{
	for(int i = 0; i < 61; i++)
	{
		if(!board.getBit(i)) // Found place to clone / jump.
		{
			if((color & board & l.getClone(i)).notEmpty()) // Clone.
		        return true;
            
			BitBoard64 moves;
			if((moves = ((board & l.getJump(i)) & color)).notEmpty()) // Jump.
			{
				for(int j = 0; j < 61; j++)
				{
					if(moves.getBit(j))
                        return true;
				}
			}
		}
	}

	return false;
}

bool Board::isGameOver(const LookUp &l) const
{
    if(!moveExists(l))  // No move for current player.
    {
        Board b(*this);
        b.invertColors();

        if(!b.moveExists(l)) // No move for other player.
        {
            return true;
        }
    }

    return false;
}

Move Board::computerMove(const LookUp &l, int depth, bool (*callback)(), int maxtime)
{
    std::vector<Move> movelist;

	if(!generateMoveList(movelist, l))
		return Move();

    bool ret = scoreMoves(movelist, *this, l, depth, callback, maxtime);
    
    if(ret)
    {
        Move m = movelist[0];

        /* calculate a position to clone from */
        if(m.to == m.from)
        {
            BitBoard64 b = l.getClone(m.to) & board & color;

            for(m.from = 0; !b.getBit(m.from) && m.from < 61; m.from++)
                ;
        }

        applyMove(movelist[0], l);

        return m;
    } 
    else
        return Move();
}

std::istream& libhexx::operator>>(std::istream &input, class Board &b)
{
	input >> b.color;
	input >> b.board;

	return input;
}

std::ostream& libhexx::operator<<(std::ostream &output, const class Board &b)
{
    output << b.color;
    output << b.board;

	return output;
}

void Board::displayBoardText(const LookUp& l, int turn) const
{
	BitBoard64 c(color);

	if(!turn)
		c = ~c;

    std::cout << "         A B C D E F G H I\n";
    std::cout << "        / / / / / / / / /\n";

	for(int y = 1; y < 10; y++)
	{
        std::cout << y << "- ";

		if(y < 5)
		{
			for(int off = 0; off < (5 - y); off++)
				std::cout << " ";
		}

		for(int x = 1; x < 10; x++)
		{
			int no;

			if((no = getHexxagonIndex(x, y)) == -1)
				std::cout << " ";
			else 
			{
				if(board.getBit(no))
				{
					if(c.getBit(getHexxagonIndex(x, y)))
						std::cout << "x ";
					else
						std::cout << "o ";
				} else
					std::cout << ". ";
			}
		}
        
        std::cout << "\n";
	}
	
	int empty = 61 - ((turn ? countBricks(1, ~BitBoard64(0,0)) : countBricks(2, ~BitBoard64(0,0))) + 
					  (turn ? countBricks(2, ~BitBoard64(0,0)) : countBricks(1, ~BitBoard64(0,0))));
	
    std::cout << "\nBricks: x " << (turn ? countBricks(1, ~BitBoard64(0,0)) : countBricks(2, ~BitBoard64(0,0)));
    std::cout << ", o " << (turn ? countBricks(2, ~BitBoard64(0,0)) : countBricks(1, ~BitBoard64(0,0)));
    std::cout <<	". Empty " << empty << ".\n";
    std::cout << "Next to move: " << (turn ? "x" : "o");
    std::cout << (isGameOver(l) ? ", Game over." : "") << "\n";
}
