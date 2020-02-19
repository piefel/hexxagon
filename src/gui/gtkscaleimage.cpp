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


#include "gtkscaleimage.h"

GtkScaleImage::GtkScaleImage()
{

}

GtkScaleImage::GtkScaleImage(int width, int height, Glib::RefPtr<Gdk::Pixbuf> picture)
{
    set_image(width, height, picture);
}

void GtkScaleImage::set_image(int width, int height, Glib::RefPtr<Gdk::Pixbuf> picture)
{
    Glib::RefPtr<Gdk::Pixbuf> resizebuf = picture->scale_simple(width, height, Gdk::INTERP_BILINEAR);
    resizebuf->render_pixmap_and_mask(picmap, bitmask, 228);

    Image::set(picmap, bitmask);
}
