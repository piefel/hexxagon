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


#include "bitboard64.h"
#include <stdio.h>
#include <iostream>
#include <netinet/in.h>

using namespace libhexx;

BitBoard64 BitBoard64::operator~(void) const
{
	BitBoard64 ret(~lowbits, ~highbits);
	return ret;
}

BitBoard64 BitBoard64::operator&(const BitBoard64 &right) const
{
	BitBoard64 ret(lowbits & right.lowbits, highbits & right.highbits);
	return ret;
}

BitBoard64 BitBoard64::operator|(const BitBoard64 &right) const
{
	BitBoard64 ret(lowbits | right.lowbits, highbits | right.highbits);
	return ret;
}

BitBoard64 BitBoard64::operator=(const BitBoard64 &right)
{
	lowbits = right.lowbits;
	highbits = right.highbits;
	return *this;
}

BitBoard64 BitBoard64::operator^(const BitBoard64 &right) const
{
	BitBoard64 ret(lowbits ^ right.lowbits, highbits ^ right.highbits);
	return ret;
}

bool BitBoard64::notEmpty() const
{
	return (bool)(lowbits || highbits);
}

void BitBoard64::setBit(int bit)
{
	if((bit < 0) || (bit > 63))
		return;

	if(bit < 32)
		lowbits |= (1 << bit);
	else
		highbits |= (1 << (bit-32));
}

void BitBoard64::unSetBit(int bit)
{
	if((bit < 0) || (bit > 63))
		return;

	if(bit < 32)
		lowbits &= ~(1 << bit);
	else
		highbits &= ~(1 << (bit-32));
}

int BitBoard64::getBit(int bit) const
{
	if((bit < 0) || (bit > 63))
		return -1;

	if(bit < 32)
		return lowbits & (1 << bit);
	else
		return highbits & (1 << (bit-32));
}

std::istream& libhexx::operator>>(std::istream &input, class BitBoard64 &b)
{
	 /* InStream will take care of network order. */
	input >> b.lowbits >> b.highbits;
	return input;
}

std::ostream& libhexx::operator<<(std::ostream& output, const class BitBoard64 &b)
{
	/* Just to get the same byte order. */
	output << b.lowbits << "\n" << b.highbits << "\n";

	return output;
}


/*********************/
/* Debug struff      */
int getBFP(int x, int y)
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

void BitBoard64::print() const
{
	int x, y, no, off;

	for(y = 1; y < 10; y++)
	{
		if(y < 5)
		{
			for(off = 0; off < (5 - y); off++)
				std::cout << " ";
		}
		for(x = 1; x < 10; x++)
		{
			if((no = getBFP(x, y)) == -1)
				std::cout << " ";
			else
			{
				if(getBit(no))
					std::cout << "x ";
				else
					std::cout << ". ";
			}
		}
        std::cout << "\n";
	}
}



