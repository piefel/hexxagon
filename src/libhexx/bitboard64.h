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


#ifndef _BITBOARD64_H
#define _BITBOARD64_H

#include <netinet/in.h>
#include <iostream>

namespace libhexx 
{
	class BitBoard64
	{
	public:
		BitBoard64(void) {lowbits = 0; highbits = 0;};
		BitBoard64(uint32_t l, uint32_t h) {lowbits = l, highbits = h;};
		
		void setBit(int bit);
		void unSetBit(int bit);
		int  getBit(int bit) const;
		
		BitBoard64 operator&(const BitBoard64 &right) const;
		BitBoard64 operator|(const BitBoard64 &right) const;
		BitBoard64 operator^(const BitBoard64 &right) const;
		BitBoard64 operator~(void) const;

		BitBoard64 operator=(const BitBoard64 &right);
		
		bool notEmpty() const;
		bool empty() const { return !notEmpty(); };
		
		void print(void) const;

        void printInHex()
        {
            printf("0x%X, 0x%X\n", lowbits, highbits);
        };
        
        friend std::ostream& operator<<(std::ostream &output, const class BitBoard64 &b);
		friend std::istream& operator>>(std::istream &input, class BitBoard64 &b);
		
	private:
		
		uint32_t lowbits, highbits;
	};
}

#endif

