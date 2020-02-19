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


#include "lookup.h"

using namespace libhexx;

void LookUp::init(BitBoard64& mask)
{
	initJumpLookups(mask);
	initCloneLookups(mask);
}

int libhexx::getHexxagonIndex(int x, int y)
{
	int no;

	if(x > 0 && x < 10 && y > 0 && y < 10)
	{
		if(y < 5)
			if(x > (9 - (5 - y)))
				return -1;

		if(y > 5)
			if(x <= (y - 5))
				return -1;
	  
		no = x+y*9 - 10;
	  
		if(y > 1) no -= 4;
		if(y > 2) no -= 3;
		if(y > 3) no -= 2;
		if(y > 4) no -= 1;
		if(y > 5) no -= 1;
		if(y > 6) no -= 2;
		if(y > 7) no -= 3;
		if(y > 8) no -= 4;
	  
		return no;
	}

	return -1;
}

void LookUp::initCloneLookups(BitBoard64& mask)
{
	int no = 0;

	for(int i = 0; i < 61; i++)
		clone_lookups[i] = BitBoard64(0,0);

	// Clone. 
	for(int y = 1; y <= 9; y++)
	{
		for(int x = 1; x <= 9; x++)
		{
			int out = 0;
			
			if(x > 0 && x < 10 && y > 0 && y < 10)
			{
				if(y < 5)
					if(x > (9 - (5 - y)))
						out = 1;
				
				if(y > 5)
					if(x <= (y - 5))
						out = 1;
			}
			
			if(!out)
			{
                if(mask.getBit(getHexxagonIndex(x, y)))
                {
                    clone_lookups[no].setBit(getHexxagonIndex(x-1, y-1));
                    clone_lookups[no].setBit(getHexxagonIndex(x  , y-1));
                    clone_lookups[no].setBit(getHexxagonIndex(x-1, y  ));
                    clone_lookups[no].setBit(getHexxagonIndex(x+1, y  ));
                    clone_lookups[no].setBit(getHexxagonIndex(x  , y+1));
                    clone_lookups[no].setBit(getHexxagonIndex(x+1, y+1));

                    /* remove moves that are not on the board. */
                    clone_lookups[no] = clone_lookups[no] & mask; 
                }

                no++;
            }
        } 
    }
}

void LookUp::initJumpLookups(BitBoard64& mask)
{
    int no = 0;

	for(int i = 0; i < 61; i++)
		jump_lookups[i] = BitBoard64(0,0);

	// Jump.
	for(int y = 1; y <= 9; y++)
	{
		for(int x = 1; x <= 9; x++)
		{
			int out = 0;
			
			if(x > 0 && x < 10 && y > 0 && y < 10)
			{
				if(y < 5)
					if(x > (9 - (5 - y)))
						out = 1;
				
				if(y > 5)
					if(x <= (y - 5))
						out = 1;
			}
			
			if(!out)
			{
                if(mask.getBit(getHexxagonIndex(x, y)))
                {
                    jump_lookups[no].setBit(getHexxagonIndex(x-2, y-2));
                    jump_lookups[no].setBit(getHexxagonIndex(x-1, y-2));
                    jump_lookups[no].setBit(getHexxagonIndex(  x, y-2));
                    jump_lookups[no].setBit(getHexxagonIndex(x-2, y-1));
                    jump_lookups[no].setBit(getHexxagonIndex(x+1, y-1));
                    jump_lookups[no].setBit(getHexxagonIndex(x-2, y  ));
                    jump_lookups[no].setBit(getHexxagonIndex(x+2, y  ));
                    jump_lookups[no].setBit(getHexxagonIndex(x-1, y+1));
                    jump_lookups[no].setBit(getHexxagonIndex(x+2, y+1));
                    jump_lookups[no].setBit(getHexxagonIndex(x  , y+2));
                    jump_lookups[no].setBit(getHexxagonIndex(x+1, y+2));
                    jump_lookups[no].setBit(getHexxagonIndex(x+2, y+2));
                    /* remove moves that are not on the board. */
                    clone_lookups[no] = clone_lookups[no] & mask; 
                }
                
                no++;
            }
        }
	}
}

