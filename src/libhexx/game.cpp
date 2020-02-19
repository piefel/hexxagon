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


#include "game.h"
#include <netinet/in.h>
#include <string.h>
#include <sstream>

using namespace libhexx;
using namespace std;

Game::Game()
{
	boardList.push_front(make_pair(Board(Layout()), 1));
    setMask(Layout().getMask());
	pos = boardList.begin();
}

Game::Game(Layout l)
{
	boardList.push_front(make_pair(Board(), 1));

	pos = boardList.begin();

    setToLayout(l);
}

bool Game::isNext()
{
	/* never go past the last one. */
	if(pos != (--boardList.end()))
        return true;
    else
        return false;
}

bool Game::isPrev()
{
	if(pos != boardList.begin())
        return true;
    else
        return false;
}

int Game::next()
{
	/* never go past the last one. */
	if(isNext())
	{
		pos++;
		return 0;
	} 
	
	return -1;
}

int Game::prev()
{
	if(isPrev())
	{
		pos--;
		return 0;
	}
	
	return -1;
}

void Game::setToLayout(Layout l)
{
    setMask(l.getMask());
    
    reset();
    
    boardList.front().first.setToLayout(l);
}

void Game::reset()
{
	pos = boardList.begin();
	
	destroyRest();
}

void Game::destroyRest()
{
	list<pair<Board, uint8_t> >::iterator next = pos;
	next++;
    boardList.erase(next, boardList.end());
}

int Game::applyMove(const Move &move)
{
	destroyRest(); // No forward history thanx! =)
	
	Board b(pos->first);

	int ret = b.applyMove(move, lookUp);

    BitBoard64 low61(0xFFFFFFFF, 0x1FFFFFFF);
    if(!(b.getBBBoard() & ~boardMask & low61).empty())
    {
        std::cout << "VAFAN!!!!!\n";
    }
    
    if(!ret)
	{
		boardList.push_back(make_pair(b, !getTurn()));
		pos++;

        /* If the other player cant move it's our turn again */
        if(!moveExists())
        {   
            pos->second = !pos->second;
            pos->first.invertColors();
        }
	}

	return ret;
}

Move Game::computerMove(int depth, bool (*callback)(), int maxtime)
{
	destroyRest(); // No forward history thanx! =)
	
	Board b(pos->first);

	Move m = b.computerMove(lookUp, depth, callback, maxtime);
		
	if(m)
	{
		boardList.push_back(make_pair(b, !getTurn()));
		pos++;

        /* If the other player cant move it's our turn again */
        if(!moveExists())
        {
            pos->second = !pos->second;
            pos->first.invertColors();
        }
    }

	return m;
}

Move Game::computerHint(int depth, bool (*callback)(), int maxtime)
{
	Board b(pos->first);

	return b.computerMove(lookUp, depth, callback, maxtime);
}

int Game::nrBoards() const
{
	return boardList.size();
}

//  File format
//----------------
//  
//  Hex2agon 1.0\n
//  32bit networkorder integer : boards
//  datablocks....
//  EOF

#define FILE_HDR_V1 "Hex2agon 1.1\n"
#define FILE_HDR_V2 "Hex2agon 1.2\n"

void Game::loadV1(std::istream& input, Game& g)
{
	uint32_t nr;
	input >> nr;

	g.boardList.clear();
	int turn = 1;

	class Board bTmp;
	while(nr)
	{
		/* Old format is backwards. 
		 * oh my this is ugly... I think it works though
		 * so why change it? */

		uint32_t *data = new uint32_t[4];
		data[0] = 0; data[1] = 0; data[2] = 0; data[3] = 0;
		for(int j = 0; j < 2; j++)
		{
			for(int i = 0; i < 4; i++)
			{
				uint8_t byte = 0;
		 
				//input.read(&byte,1);

				data[j*2] |=  byte << (i * 8);
			}
			data[j*2] = htonl(data[j*2]);
			for(int i = 0; i < 4; i++)
			{
				uint8_t byte = 0;
		 
				//input.read(&byte,1);

				data[j*2+1] |=  byte << (i * 8);
			}
			data[j*2+1] = htonl(data[j*2+1]);
		}

		//libsock::BufferInStream bis((char*)data, 16);
		//bis >> bTmp;
		
		g.boardList.push_back(make_pair(bTmp, turn));
        turn = !turn;
		nr--;
	}
	
	g.pos = g.boardList.begin();
	while(g.next() == 0)
		;
}

void Game::loadV2(std::istream &input, Game &g)
{
    BitBoard64 mask;
    input >> mask;
    g.setMask(mask);
    
	uint32_t nr;
	input >> nr;

	g.boardList.clear();

	while(nr)
	{
        Board tmp;
        int turn;
        
        input >> tmp;
        input >> turn;

		g.boardList.push_back(make_pair(tmp, turn));

		nr--;
	}
	
	g.pos = g.boardList.begin();
	while(g.next() == 0)
		;
}

bool isValidHexxagonStream(std::istream &in)
{
	char str[64];
	in.read(str, strlen(FILE_HDR_V1));
	
	if(!memcmp(str, FILE_HDR_V1, strlen(FILE_HDR_V1)))
        return true;
	if(!memcmp(str, FILE_HDR_V2, strlen(FILE_HDR_V2)))
        return true;    

    return false;
}

std::istream& libhexx::operator>>(std::istream &input, Game &g)
{
	char str[64];
	input.read(str, strlen(FILE_HDR_V1));
	
	if(!memcmp(str, FILE_HDR_V1, strlen(FILE_HDR_V1)))
		g.loadV1(input, g);
	else if(!memcmp(str, FILE_HDR_V2, strlen(FILE_HDR_V2)))
		g.loadV2(input, g);
	else
		throw Game::ioException();
	
	return input;
}

std::ostream& libhexx::operator<<(std::ostream &output, const Game &g)
{
	output << FILE_HDR_V2;

    output << g.getMask();
    
	output << (uint32_t)g.nrBoards() << "\n";
    
	for(list<pair<Board, uint8_t> >::const_iterator step = g.boardList.begin(); step != g.boardList.end(); step++)
    {
		output << step->first;
        output << (int)step->second << "\n";
    }
    
	return output;
}

