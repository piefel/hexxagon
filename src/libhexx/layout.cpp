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


#include "layout.h"
#include "game.h"

using namespace libhexx;

bool Layout::isValid()
{
    if((mask & board & color).empty())
        return false;
   
    if((mask & board & ~color).empty())
        return false;

    Game g(*this);
    if(!g.moveExists())
        return false;
    
    return true;
}

std::istream& libhexx::operator>>(std::istream &input, class Layout &l)
{
	input >> l.mask;
	input >> l.board;
    input >> l.color;

	return input;
}

std::ostream& libhexx::operator<<(std::ostream &output, const class Layout &l)
{
    output << l.mask;
    output << l.board;
    output << l.color;

	return output;
}

