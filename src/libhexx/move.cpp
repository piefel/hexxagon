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

#include <sys/timeb.h>
#include <stdlib.h>
#include <string.h>

#include <ctime>
#include <iostream>
#include <algorithm>

using namespace libhexx;

char global_count;
int alphaBeta(const Board &board, const LookUp &lookup, bool turn, int level, int alpha, int beta, bool (*callback)())
{
	if(!level)
		return board.evaluate(lookup, turn);

    std::vector<Move> moves;
    if(!board.generateMoveList(moves, lookup))
    {
        if(board.isGameOver(lookup))
            return board.evaluate(lookup, turn);

        Board newboard = board;
        newboard.invertColors();
    	return -alphaBeta(newboard, lookup, turn, level - 1, -beta, -alpha, callback);
    }

	global_count++;
    
	if(!global_count && callback)
    {
        /* Terminate? */
		if(!callback())
            return SCR_BREAK;   
    }
    
	int best = -SCR_INFINITY;
	for(std::vector<Move>::iterator i = moves.begin(); i != moves.end() && (best < beta); i++)
	{
		if(best > alpha)
			alpha = best;

		Board newboard = Board(board);
		newboard.applyMove(*i, lookup);
		int value = -alphaBeta(newboard, lookup, !turn, level - 1, -beta, -alpha, callback);

        if(value == SCR_BREAK)
            return SCR_BREAK;
        
		if(value > best)
			best = value;
	}     

	return best;
}


bool libhexx::scoreMoves(std::vector<Move> &moves, const Board board, const LookUp& lookUp, int depth, bool (*callback)(), int maxtime)
{
	time_t t = time(NULL);

	for(int i = 1; (i < depth) && (time(NULL) - t <= maxtime); i++)
	{
		int best  = -SCR_INFINITY;
		int alpha = -SCR_INFINITY;
		int beta  = SCR_INFINITY;

		for(std::vector<Move>::iterator j = moves.begin(); (j != moves.end()) && (time(NULL) - t <= maxtime) && (best < beta); j++)
		{
			if(best > alpha)
				alpha = best;

			Board newboard = Board(board);
			newboard.applyMove(*j, lookUp);
			int value = -alphaBeta(newboard, lookUp, false, i, -beta, -alpha, callback);
		
            if(value == SCR_BREAK)
                return false;
            
			j->score = value;

			if(value > best)
				best = value;
		}
		
		stable_sort(moves.rbegin(), moves.rend());
	}

    return true;
}


