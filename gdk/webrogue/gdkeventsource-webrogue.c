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

#include <stdlib.h>

static gboolean
_gdk_event_source_check (GSource *source)
{
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
  char *name;

  source = g_source_new (&event_funcs, sizeof (GSource));
  name = g_strdup_printf ("GDK Webrogue Event source (%s)",
                          gdk_display_get_name (display));
  g_source_set_name (source, name);
  g_free (name);

  g_source_set_priority (source, GDK_PRIORITY_EVENTS);
  g_source_set_can_recurse (source, TRUE);
  g_source_attach (source, NULL);

  return source;
}
