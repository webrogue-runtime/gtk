/* GDK - The GIMP Drawing Kit
 *
 * Copyright © 2026 Artem Borovik
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include "gdkconfig.h"

#include "gdkcairocontext-webrogue.h"

G_DEFINE_TYPE (GdkWebrogueCairoContext, gdk_webrogue_cairo_context, GDK_TYPE_CAIRO_CONTEXT)

static void
gdk_webrogue_cairo_context_init (GdkWebrogueCairoContext *self)
{
  abort ();
}

static void
_gdk_webrogue_cairo_context_dispose (GObject *object)
{
  G_OBJECT_CLASS (gdk_webrogue_cairo_context_parent_class)->dispose (object);
}

static void
_gdk_webrogue_cairo_context_begin_frame (GdkDrawContext *draw_context,
                                         gpointer context_data,
                                         GdkMemoryDepth depth,
                                         cairo_region_t *region,
                                         GdkColorState **out_color_state,
                                         GdkMemoryDepth *out_depth)
{
  abort ();
}

static void
_gdk_webrogue_cairo_context_end_frame (GdkDrawContext *draw_context,
                                       gpointer context_data,
                                       cairo_region_t *painted)
{
  abort ();
}

static void
_gdk_webrogue_cairo_context_surface_resized (GdkDrawContext *draw_context)
{
  abort ();
}

static cairo_t *
_gdk_webrogue_cairo_context_cairo_create (GdkCairoContext *context)
{
  abort ();
}

static void
gdk_webrogue_cairo_context_class_init (GdkWebrogueCairoContextClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GdkDrawContextClass *draw_context_class = GDK_DRAW_CONTEXT_CLASS (klass);
  GdkCairoContextClass *cairo_context_class = GDK_CAIRO_CONTEXT_CLASS (klass);

  gobject_class->dispose = _gdk_webrogue_cairo_context_dispose;

  draw_context_class->begin_frame = _gdk_webrogue_cairo_context_begin_frame;
  draw_context_class->end_frame = _gdk_webrogue_cairo_context_end_frame;
  draw_context_class->surface_resized = _gdk_webrogue_cairo_context_surface_resized;

  cairo_context_class->cairo_create = _gdk_webrogue_cairo_context_cairo_create;
}
