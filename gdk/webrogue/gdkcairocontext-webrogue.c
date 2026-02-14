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
#include "gdksurface-webrogue.h"

G_DEFINE_TYPE (GdkWebrogueCairoContext, gdk_webrogue_cairo_context, GDK_TYPE_CAIRO_CONTEXT)

static void
gdk_webrogue_cairo_context_init (GdkWebrogueCairoContext *self)
{
  self->data = NULL;
}

static void
_gdk_webrogue_cairo_context_dispose (GObject *object)
{
  // TODO free(self->data)
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
  *out_color_state = GDK_COLOR_STATE_SRGB;
  *out_depth = gdk_color_state_get_depth (GDK_COLOR_STATE_SRGB);
}

static void
_gdk_webrogue_cairo_context_end_frame (GdkDrawContext *draw_context,
                                       gpointer context_data,
                                       cairo_region_t *painted)
{
  GdkWebrogueCairoContext *self = (GdkWebrogueCairoContext *)draw_context;
  webroguegfx_present_pixels(self->wr_window, self->data, self->data_size);
}

static void
_gdk_webrogue_cairo_context_surface_resized (GdkDrawContext *draw_context)
{
  GdkWebrogueCairoContext *self = (GdkWebrogueCairoContext *)draw_context;
  guint width, height;
  gdk_draw_context_get_buffer_size (draw_context, &width, &height);
  self->data_size = 4 * width * height;
  self->data = realloc(self->data, self->data_size);
}

static cairo_t *
_gdk_webrogue_cairo_context_cairo_create (GdkCairoContext *cairo_context)
{
  GdkWebrogueCairoContext *self = (GdkWebrogueCairoContext *)cairo_context;
  GdkSurface *surface = gdk_draw_context_get_surface (GDK_DRAW_CONTEXT (self));
  GdkWebrogueSurface *webrogue_surface = GDK_WEBROGUE_SURFACE (surface);
  
  self->wr_window = webrogue_surface->wr_window;
  int width, height;
  webroguegfx_gl_size(self->wr_window, &width, &height);
  self->data_size = 4 * width * height;
  self->data = self->data ? realloc(self->data, self->data_size) : malloc(self->data_size);
  int stride = 4 * width;
  

  cairo_surface_t *image_surface = cairo_image_surface_create_for_data (self->data,
                                                       CAIRO_FORMAT_ARGB32,
                                                       width,
                                                       height,
                                                       stride);

  cairo_t *cr = cairo_create (image_surface);
  return cr;
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
