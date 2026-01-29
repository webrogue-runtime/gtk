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

#include "gdksurface-webrogue.h"
#include <gdk/gdksurfaceprivate.h>
#include <webroguegfx/webroguegfx.h>

G_BEGIN_DECLS

GType gdk_webrogue_toplevel_get_type (void) G_GNUC_CONST;
GType gdk_webrogue_popup_get_type (void) G_GNUC_CONST;
GType gdk_webrogue_drag_surface_get_type (void) G_GNUC_CONST;

#define GDK_TYPE_WEBROGUE_TOPLEVEL (gdk_webrogue_toplevel_get_type ())
#define GDK_TYPE_WEBROGUE_POPUP (gdk_webrogue_popup_get_type ())
#define GDK_TYPE_WEBROGUE_DRAG_SURFACE (gdk_webrogue_drag_surface_get_type ())

struct _GdkWebrogueSurface
{
  GdkSurface parent_instance;
  wr_window wr_window;
};

struct _GdkWebrogueSurfaceClass
{
  GdkSurfaceClass parent_class;
};

GType gdk_webrogue_surface_get_type (void);

typedef struct _GdkWebrogueSurface GdkWebrogueSurface;
typedef struct _GdkWebrogueSurfaceClass GdkWebrogueSurfaceClass;

#define GDK_TYPE_WEBROGUE_SURFACE (gdk_webrogue_surface_get_type ())
#define GDK_WEBROGUE_SURFACE(object) (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_WEBROGUE_SURFACE, GdkWebrogueSurface))
#define GDK_WEBROGUE_SURFACE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_WEBROGUE_SURFACE, GdkWebrogueSurfaceClass))
#define GDK_IS_WEBROGUE_SURFACE(object) (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_WEBROGUE_SURFACE))
#define GDK_IS_WEBROGUE_SURFACE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_WEBROGUE_SURFACE))
#define GDK_WEBROGUE_SURFACE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_WEBROGUE_SURFACE, GdkWebrogueSurfaceClass))

G_END_DECLS
