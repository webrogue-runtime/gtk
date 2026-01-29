/* GDK - The GIMP Drawing Kit
 * Copyright (C) 2026 Artem Borovik
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "gdkdisplay-webrogue.h"
#include "gdksurface-webrogue.h"
#include <gdk/gdkcursor.h>

GdkDrag *_gdk_webrogue_surface_drag_begin (GdkSurface *surface,
                                           GdkDevice *device,
                                           GdkContentProvider *content,
                                           GdkDragAction actions,
                                           double dx,
                                           double dy);
GdkDisplay *_gdk_webrogue_display_open (const char *display_name);
wr_window gdk_webrogue_surface_get_wr_window (GdkSurface *surface);
