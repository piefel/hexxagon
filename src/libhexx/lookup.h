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


#ifndef _LOOKUP_H
#define _LOOKUP_H

#include "bitboard64.h"

namespace libhexx
{
	int getHexxagonIndex(int x, int y);

	class LookUp
		{
		public:
			LookUp(void) { BitBoard64 b = ~BitBoard64(0,0); init(b); };
			LookUp(BitBoard64& mask) { init(mask); };
			
			void init(BitBoard64& mask);

			void initJumpLookups(BitBoard64& mask);
			void initCloneLookups(BitBoard64& mask);

			inline const BitBoard64& getJump(int nr) const { return jump_lookups[nr]; };
			inline const BitBoard64& getClone(int nr) const { return clone_lookups[nr]; };

		protected:
			BitBoard64 clone_lookups[61]; 
			BitBoard64 jump_lookups[61]; 
 		};
}

#endif
