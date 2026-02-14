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

#define VK_USE_PLATFORM_WEBROGUE

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "gdkcairocontext-webrogue.h"
#include "gdkdeviceprivate.h"
#include "gdkdisplay-webrogue.h"
#include "gdkdisplay.h"
#include "gdkeventsource-webrogue.h"
#include "gdkprivate-webrogue.h"
#include "gdkprofilerprivate.h"
#include "gdksurface-webrogue.h"
#include "gdksurfaceprivate.h"
#include "gdkvulkancontext-webrogue.h"
#include <gio/gio.h>
#include <glib.h>

#include "gdk/gdkprivate.h"

G_DEFINE_TYPE (GdkWebrogueDisplay, gdk_webrogue_display, GDK_TYPE_DISPLAY)

GdkDisplay *
_gdk_webrogue_display_open (const char *display_name)
{
  GdkDisplay *display;
  GdkWebrogueDisplay *display_webrogue;

  GDK_DEBUG (MISC, "opening display %s", display_name ? display_name : "");

  display = g_object_new (GDK_TYPE_WEBROGUE_DISPLAY, NULL);
  display_webrogue = GDK_WEBROGUE_DISPLAY (display);
  display_webrogue->event_source = _gdk_webrogue_event_source_new (display);

  gdk_display_emit_opened (display);

  return display;
}

static void
_gdk_webrogue_display_dispose (GObject *object)
{
  GdkWebrogueDisplay *display_webrogue = GDK_WEBROGUE_DISPLAY (object);

  g_list_store_remove_all (display_webrogue->monitors);
  if (display_webrogue->event_source)
    {
      g_source_destroy (display_webrogue->event_source);
      g_source_unref (display_webrogue->event_source);
      display_webrogue->event_source = NULL;
    }

  G_OBJECT_CLASS (gdk_webrogue_display_parent_class)->dispose (object);
}

static void
_gdk_webrogue_display_finalize (GObject *object)
{
  GdkWebrogueDisplay *display_webrogue = GDK_WEBROGUE_DISPLAY (object);

  g_object_unref (display_webrogue->monitors);

  G_OBJECT_CLASS (gdk_webrogue_display_parent_class)->finalize (object);
}

static const char *
_gdk_webrogue_display_get_name (GdkDisplay *display)
{
  return "webrogue-0";
}

static void
_gdk_webrogue_display_beep (GdkDisplay *display)
{
  abort ();
}

static void
_gdk_webrogue_display_sync (GdkDisplay *display)
{
  abort ();
}

static void
_gdk_webrogue_display_flush (GdkDisplay *display)
{
  g_return_if_fail (GDK_IS_DISPLAY (display));
}

static void
_gdk_webrogue_display_make_default (GdkDisplay *display)
{
}

static gulong
_gdk_webrogue_display_get_next_serial (GdkDisplay *display)
{
  static gulong serial = 0;
  return ++serial;
}

static void
_gdk_webrogue_display_notify_startup_complete (GdkDisplay *display,
                                               const char *startup_id)
{
}

static GdkKeymap *
_gdk_webrogue_display_get_keymap (GdkDisplay *display)
{
  abort ();
}

static GListModel *
_gdk_webrogue_display_get_monitors (GdkDisplay *display)
{
  GdkWebrogueDisplay *self = GDK_WEBROGUE_DISPLAY (display);

  return G_LIST_MODEL (self->monitors);
}

static GdkMonitor *
_gdk_webrogue_display_get_monitor_at_surface (GdkDisplay *display,
                                              GdkSurface *window)
{
  return NULL;
}

static void
_gdk_webrogue_display_set_cursor_theme (GdkDisplay *display,
                                        const char *name,
                                        int size)
{
  abort ();
}

static gboolean
_gdk_webrogue_display_get_setting (GdkDisplay *display,
                                   const char *name,
                                   GValue *value)
{
  return FALSE;
}

static void
gdk_webrogue_display_system_bell (GdkDisplay *display,
                                  GdkSurface *surface)
{
  abort ();
}

static void
gdk_webrogue_display_class_init (GdkWebrogueDisplayClass *class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);
  GdkDisplayClass *display_class = GDK_DISPLAY_CLASS (class);

  object_class->dispose = _gdk_webrogue_display_dispose;
  object_class->finalize = _gdk_webrogue_display_finalize;

  display_class->toplevel_type = GDK_TYPE_WEBROGUE_TOPLEVEL;
  display_class->popup_type = GDK_TYPE_WEBROGUE_POPUP;
  display_class->cairo_context_type = GDK_TYPE_WEBROGUE_CAIRO_CONTEXT;

#ifdef GDK_RENDERING_VULKAN
    display_class->vk_context_type = GDK_TYPE_WEBROGUE_VULKAN_CONTEXT;
  if(webroguegfx_vulkan_check()) {
    display_class->vk_extension_name = VK_WEBROGUE_SURFACE_EXTENSION_NAME;
  } else {
    display_class->vk_extension_name = NULL;
  }
#endif

  display_class->get_name = _gdk_webrogue_display_get_name;
  display_class->beep = _gdk_webrogue_display_beep;
  display_class->sync = _gdk_webrogue_display_sync;
  display_class->flush = _gdk_webrogue_display_flush;
  display_class->make_default = _gdk_webrogue_display_make_default;
  display_class->queue_events = _gdk_webrogue_display_queue_events;
  display_class->get_next_serial = _gdk_webrogue_display_get_next_serial;
  display_class->notify_startup_complete = _gdk_webrogue_display_notify_startup_complete;
  display_class->get_keymap = _gdk_webrogue_display_get_keymap;

  // display_class->init_gl = gdk_webrogue_display_init_gl;

  display_class->get_monitors = _gdk_webrogue_display_get_monitors;
  display_class->get_monitor_at_surface = _gdk_webrogue_display_get_monitor_at_surface;
  display_class->get_setting = _gdk_webrogue_display_get_setting;
  display_class->set_cursor_theme = _gdk_webrogue_display_set_cursor_theme;
}

static void
gdk_webrogue_display_init (GdkWebrogueDisplay *display)
{
  GdkWebrogueDisplay *display_webrogue = GDK_WEBROGUE_DISPLAY (display);

  display->monitors = g_list_store_new (GDK_TYPE_MONITOR);
}
