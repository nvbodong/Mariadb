/* -*- c-basic-offset: 2 -*- */
/*
  Copyright(C) 2017 Kouhei Sutou <kou@clear-code.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1335  USA
*/

#include "mrn_smart_bitmap.hpp"

namespace mrn {
  SmartBitmap::SmartBitmap(MY_BITMAP *bitmap)
    : bitmap_(bitmap) {
  }

  SmartBitmap::~SmartBitmap() {
    if (bitmap_) {
      my_bitmap_free(bitmap_);
    }
  }

  MY_BITMAP *SmartBitmap::get() {
    return bitmap_;
  }

  MY_BITMAP *SmartBitmap::release() {
    MY_BITMAP *bitmap = bitmap_;
    bitmap_ = NULL;
    return bitmap;
  }
}
