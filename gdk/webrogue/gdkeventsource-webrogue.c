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

#include "config.h"

#include "gdkeventsource-webrogue.h"

#include "gdkeventsprivate.h"
#include "gdkframeclockprivate.h"
#include "gdksurfaceprivate.h"

#include <webroguegfx/webroguegfx.h>
#include <stdlib.h>


static GList *registered_surfaces = NULL;

static void resize_window(GdkWebrogueSurface *webrogue_surface, gpointer userdata) {
  GdkSurface *surface = GDK_SURFACE (webrogue_surface);
  int w, h;
  webroguegfx_gl_size(webrogue_surface->wr_window, &w, &h);
  if (surface->width != w ||
      surface->height != h)
    {
      surface->width = w;
      surface->height = h;

      _gdk_surface_update_size (surface);
      gdk_surface_invalidate_rect (surface, NULL);
  
      gdk_surface_request_layout (surface);
      gdk_surface_queue_render (surface);
    }
}

static void resize_all_windows(void) {
  g_list_foreach (registered_surfaces, (GFunc)resize_window, NULL);
}

void _gdk_webrogue_events_add_surface (GdkWebrogueSurface *webrogue_surface) {
  registered_surfaces = g_list_append (registered_surfaces, webrogue_surface);

}
void _gdk_webrogue_events_remove_surface (GdkWebrogueSurface *webrogue_surface) {
  registered_surfaces = g_list_remove (registered_surfaces, webrogue_surface);
}

typedef struct GdkEventSource
{
  GSource source;

  webrogue_event current_wr_event;
  GdkDisplay *display;
} GdkEventSource;

static gboolean
_gdk_event_source_check (GSource *source)
{
  GdkEventSource *event_source = (GdkEventSource *) source;
  // if(event_source->current_wr_event.type != WEBROGUE_EVENT_TYPE_INVALID) {
  //   return TRUE;
  // }
  // event_source->current_wr_event = webroguegfx_poll();
  while(true) {
    webrogue_event current_wr_event = webroguegfx_poll();

    switch (current_wr_event.type) {
      case WEBROGUE_EVENT_TYPE_WINDOW_RESIZED:
        resize_all_windows();
        break;
      case WEBROGUE_EVENT_TYPE_GL_RESIZED:
        resize_all_windows();
        break;
      default:
        break;
    }
    if (current_wr_event.type == WEBROGUE_EVENT_TYPE_INVALID) {
      break;
    }
  }


  // return event_source->current_wr_event.type != WEBROGUE_EVENT_TYPE_INVALID;
  return FALSE;
}

static gboolean
_gdk_event_source_prepare (GSource *source,
                           int *timeout)
{
  *timeout = 10;
  return _gdk_event_source_check (source);
}

static gboolean
_gdk_event_source_dispatch (GSource *source,
                            GSourceFunc callback,
                            gpointer user_data)
{
  // GdkEvent *event;
  // if (event)
  //   {
  //     _gdk_event_emit (event);
  //     gdk_event_unref (event);
  //   }

  return G_SOURCE_CONTINUE;
}

static void
_gdk_event_source_finalize (GSource *source)
{
}

static GSourceFuncs event_funcs = {
  _gdk_event_source_prepare,
  _gdk_event_source_check,
  _gdk_event_source_dispatch,
  _gdk_event_source_finalize
};

void
_gdk_webrogue_display_queue_events (GdkDisplay *display)
{
}

GSource *
_gdk_webrogue_event_source_new (GdkDisplay *display)
{
  GSource *source;
  GdkEventSource *event_source;
  char *name;

  source = g_source_new (&event_funcs, sizeof (GdkEventSource));
  name = g_strdup_printf ("GDK Webrogue Event source (%s)",
                          gdk_display_get_name (display));
  g_source_set_name (source, name);
  g_free (name);
  event_source = (GdkEventSource *) source;
  event_source->current_wr_event.type = WEBROGUE_EVENT_TYPE_INVALID;
  event_source->display = display;

  g_source_set_priority (source, GDK_PRIORITY_EVENTS);
  g_source_set_can_recurse (source, TRUE);
  g_source_attach (source, NULL);

  return source;
}
