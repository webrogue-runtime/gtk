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

#include "gdkprivate-webrogue.h"

G_BEGIN_DECLS

GSource *_gdk_webrogue_event_source_new (GdkDisplay *display);
void _gdk_webrogue_display_queue_events (GdkDisplay *display);
void _gdk_webrogue_events_add_surface (GdkWebrogueSurface *webrogue_surface);
void _gdk_webrogue_events_remove_surface (GdkWebrogueSurface *webrogue_surface);

G_END_DECLS
