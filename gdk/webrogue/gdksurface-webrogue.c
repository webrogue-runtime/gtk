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

#include "gdksurface-webrogue.h"

#include "gdkdevice-webrogue.h"
#include "gdkdeviceprivate.h"
#include "gdkdisplay-webrogue.h"
#include "gdkdisplay.h"
#include "gdkdragsurfaceprivate.h"
#include "gdkeventsource-webrogue.h"
#include "gdkframeclockidleprivate.h"
#include "gdkpopupprivate.h"
#include "gdkprivate-webrogue.h"
#include "gdkseatprivate.h"
#include "gdksurfaceprivate.h"
#include "gdktextureprivate.h"
#include "gdktoplevelprivate.h"

#include <graphene.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

G_DEFINE_TYPE (GdkWebrogueSurface, gdk_webrogue_surface, GDK_TYPE_SURFACE)

static void
gdk_webrogue_surface_toplevel_resize (GdkSurface *surface,
                                      int width,
                                      int height);

static void
gdk_webrogue_surface_move_resize_internal (GdkSurface *surface,
                                           gboolean with_move,
                                           int x,
                                           int y,
                                           int width,
                                           int height);

static gboolean
compute_size_idle (gpointer user_data)
{
  abort ();
}

static void
on_frame_clock_after_update (GdkFrameClock *clock,
                             GdkSurface *surface)
{
}

static void
gdk_webrogue_surface_init (GdkWebrogueSurface *impl)
{
  webroguegfx_make_window (&impl->wr_window);
  // int width, height;
  int gl_width, gl_height;
  // webroguegfx_window_size (impl->wr_window, &width, &height);
  webroguegfx_gl_size (impl->wr_window, &gl_width, &gl_height);
  // double scale = gl_width / width;

  GdkSurface *surface = GDK_SURFACE (impl);

  surface->width = gl_width;
  surface->height = gl_height;

  gdk_surface_invalidate_rect (surface, NULL);

  g_object_notify (G_OBJECT (surface), "width");
  g_object_notify (G_OBJECT (surface), "height");
  // g_object_notify (G_OBJECT (surface), "scale");
  _gdk_surface_update_size (surface);
}

static void
on_frame_clock_after_paint (GdkFrameClock *clock,
                            GdkSurface *surface)
{
}

static void
on_frame_clock_before_paint (GdkFrameClock *clock,
                             GdkSurface *surface)
{
  GdkFrameTimings *timings = gdk_frame_clock_get_current_timings (clock);
  gint64 presentation_time;
  gint64 refresh_interval;

  if (surface->update_freeze_count > 0)
    return;

  gdk_frame_clock_get_refresh_info (clock,
                                    timings->frame_time,
                                    &refresh_interval, &presentation_time);
  if (presentation_time != 0)
    {
      timings->predicted_presentation_time = presentation_time + refresh_interval;
    }
  else
    {
      timings->predicted_presentation_time = timings->frame_time + refresh_interval / 2 + refresh_interval;
    }
}

static void
connect_frame_clock (GdkSurface *surface)
{
  GdkFrameClock *frame_clock = gdk_surface_get_frame_clock (surface);

  g_signal_connect (frame_clock, "before-paint",
                    G_CALLBACK (on_frame_clock_before_paint), surface);
  g_signal_connect_after (frame_clock, "update",
                          G_CALLBACK (on_frame_clock_after_update), surface);
  g_signal_connect (frame_clock, "after-paint",
                    G_CALLBACK (on_frame_clock_after_paint), surface);
}

static void
disconnect_frame_clock (GdkSurface *surface)
{
  GdkFrameClock *frame_clock = gdk_surface_get_frame_clock (surface);

  g_signal_handlers_disconnect_by_func (frame_clock,
                                        on_frame_clock_before_paint, surface);
  g_signal_handlers_disconnect_by_func (frame_clock,
                                        on_frame_clock_after_update, surface);
  g_signal_handlers_disconnect_by_func (frame_clock,
                                        on_frame_clock_after_paint, surface);
}

static void
gdk_webrogue_surface_constructed (GObject *object)
{

  GdkWebrogueSurface *self = GDK_WEBROGUE_SURFACE (object);
  GdkSurface *surface = GDK_SURFACE (object);
  GdkWebrogueDisplay *webrogue_display = GDK_WEBROGUE_DISPLAY (gdk_surface_get_display (surface));

  g_object_ref (self);

  G_OBJECT_CLASS (gdk_webrogue_surface_parent_class)->constructed (object);

  connect_frame_clock (surface);
}

static void
gdk_webrogue_surface_finalize (GObject *object)
{
  _gdk_webrogue_events_remove_surface (GDK_WEBROGUE_SURFACE (object));
}

static gboolean
thaw_updates_cb (GdkSurface *surface)
{
  if (!GDK_SURFACE_DESTROYED (surface))
    gdk_surface_thaw_updates (surface);
  g_object_unref (surface);
  return G_SOURCE_REMOVE;
}

static void
_gdk_webrogue_surface_destroy (GdkSurface *surface,
                               gboolean foreign_destroy)
{
}

static void
gdk_webrogue_surface_hide (GdkSurface *surface)
{
  abort ();
}

static double
gdk_webrogue_surface_get_scale (GdkSurface *surface)
{
  return 1;
}

static void
gdk_webrogue_surface_move_resize_internal (GdkSurface *surface,
                                           gboolean with_move,
                                           int x,
                                           int y,
                                           int width,
                                           int height)
{
}

static void
gdk_webrogue_surface_toplevel_resize (GdkSurface *surface,
                                      int width,
                                      int height)
{
  gdk_webrogue_surface_move_resize_internal (surface, FALSE,
                                             0, 0,
                                             width, height);
}

static void
gdk_webrogue_surface_move (GdkSurface *surface,
                           int x,
                           int y)
{
  gdk_webrogue_surface_move_resize_internal (surface, TRUE, x, y, -1, -1);
}

static void
gdk_webrogue_surface_layout_popup (GdkSurface *surface,
                                   int width,
                                   int height,
                                   GdkPopupLayout *layout)
{
  abort ();
}

static void
show_popup (GdkSurface *surface)
{
  gdk_surface_set_is_mapped (surface, TRUE);
  gdk_surface_invalidate_rect (surface, NULL);
}

static void
show_grabbing_popup (GdkSeat *seat,
                     GdkSurface *surface,
                     gpointer user_data)
{
  show_popup (surface);
}

static gboolean
gdk_webrogue_surface_present_popup (GdkSurface *surface,
                                    int width,
                                    int height,
                                    GdkPopupLayout *layout)
{
  gdk_webrogue_surface_layout_popup (surface, width, height, layout);

  if (GDK_SURFACE_IS_MAPPED (surface))
    return TRUE;

  if (surface->autohide)
    {
      gdk_seat_grab (gdk_display_get_default_seat (surface->display),
                     surface,
                     GDK_SEAT_CAPABILITY_ALL,
                     TRUE,
                     NULL, NULL,
                     show_grabbing_popup, NULL);
    }
  else
    {
      show_popup (surface);
    }

  return GDK_SURFACE_IS_MAPPED (surface);
}

static void
gdk_webrogue_surface_set_title (GdkSurface *surface,
                                const char *title)
{
}

static void
gdk_webrogue_surface_set_startup_id (GdkSurface *surface,
                                     const char *startup_id)
{
}

static void
gdk_webrogue_surface_set_transient_for (GdkSurface *surface,
                                        GdkSurface *parent)
{
  abort ();
}

static void
gdk_webrogue_surface_set_modal_hint (GdkSurface *surface,
                                     gboolean modal)
{
}

static void
gdk_webrogue_surface_get_geometry (GdkSurface *surface,
                                   int *x,
                                   int *y,
                                   int *width,
                                   int *height)
{
  g_return_if_fail (GDK_IS_SURFACE (surface));

  int new_width, new_height;
  GdkWebrogueSurface *impl;
  impl = GDK_WEBROGUE_SURFACE (surface);
  webroguegfx_gl_size (impl->wr_window, &new_width, &new_height);

  if (x)
    *x = surface->x;
  if (y)
    *y = surface->y;
  if (width)
    *width = new_width;
  if (height)
    *height = new_height;
}

static void
gdk_webrogue_surface_get_root_coords (GdkSurface *surface,
                                      int x,
                                      int y,
                                      int *root_x,
                                      int *root_y)
{
  abort ();
}

static gboolean
gdk_webrogue_surface_get_device_state (GdkSurface *surface,
                                       GdkDevice *device,
                                       double *x,
                                       double *y,
                                       GdkModifierType *mask)
{
  g_return_val_if_fail (surface == NULL || GDK_IS_SURFACE (surface), FALSE);

  if (GDK_SURFACE_DESTROYED (surface))
    return FALSE;

  return TRUE;
}

static void
gdk_webrogue_surface_set_input_region (GdkSurface *surface,
                                       cairo_region_t *shape_region)
{
}

static GdkSurface *
gdk_webrogue_drag_surface_new (GdkDisplay *display)
{
  return g_object_new (GDK_TYPE_WEBROGUE_DRAG_SURFACE,
                       "display", display,
                       NULL);
}

static void
gdk_webrogue_toplevel_begin_resize (GdkToplevel *toplevel,
                                    GdkSurfaceEdge edge,
                                    GdkDevice *device,
                                    int button,
                                    double x,
                                    double y,
                                    guint32 timestamp)
{
}

static void
gdk_webrogue_toplevel_begin_move (GdkToplevel *toplevel,
                                  GdkDevice *device,
                                  int button,
                                  double x,
                                  double y,
                                  guint32 timestamp)
{
}

static gboolean
gdk_webrogue_surface_beep (GdkSurface *surface)
{
  return FALSE;
}

static void
gdk_webrogue_surface_request_layout (GdkSurface *surface)
{
}

static void
gdk_webrogue_surface_destroy_notify (GdkSurface *surface)
{
  if (!GDK_SURFACE_DESTROYED (surface))
    _gdk_surface_destroy (surface, TRUE);

  g_object_unref (surface);
}

static gboolean
gdk_webrogue_surface_compute_size (GdkSurface *surface)
{
  return FALSE;
}

static void
gdk_webrogue_surface_class_init (GdkWebrogueSurfaceClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GdkSurfaceClass *impl_class = GDK_SURFACE_CLASS (klass);

  object_class->constructed = gdk_webrogue_surface_constructed;
  object_class->finalize = gdk_webrogue_surface_finalize;

  impl_class->hide = gdk_webrogue_surface_hide;
  impl_class->get_geometry = gdk_webrogue_surface_get_geometry;
  impl_class->get_root_coords = gdk_webrogue_surface_get_root_coords;
  impl_class->get_device_state = gdk_webrogue_surface_get_device_state;
  impl_class->set_input_region = gdk_webrogue_surface_set_input_region;
  impl_class->destroy = _gdk_webrogue_surface_destroy;
  impl_class->beep = gdk_webrogue_surface_beep;
  impl_class->destroy_notify = gdk_webrogue_surface_destroy_notify;
  impl_class->drag_begin = _gdk_webrogue_surface_drag_begin;
  impl_class->get_scale = gdk_webrogue_surface_get_scale;
  impl_class->request_layout = gdk_webrogue_surface_request_layout;
  impl_class->compute_size = gdk_webrogue_surface_compute_size;
}

#define LAST_PROP 1

typedef struct
{
  GdkWebrogueSurface parent_instance;
} GdkWebroguePopup;

typedef struct
{
  GdkWebrogueSurfaceClass parent_class;
} GdkWebroguePopupClass;

static void gdk_webrogue_popup_iface_init (GdkPopupInterface *iface);

G_DEFINE_TYPE_WITH_CODE (GdkWebroguePopup, gdk_webrogue_popup, GDK_TYPE_WEBROGUE_SURFACE, G_IMPLEMENT_INTERFACE (GDK_TYPE_POPUP, gdk_webrogue_popup_iface_init))

static void
gdk_webrogue_popup_init (GdkWebroguePopup *popup)
{
}

static void
gdk_webrogue_popup_constructed (GObject *object)
{
  abort ();
}

static void
gdk_webrogue_popup_get_property (GObject *object,
                                 guint prop_id,
                                 GValue *value,
                                 GParamSpec *pspec)
{
  GdkSurface *surface = GDK_SURFACE (object);

  switch (prop_id)
    {
    case LAST_PROP + GDK_POPUP_PROP_PARENT:
      g_value_set_object (value, surface->parent);
      break;

    case LAST_PROP + GDK_POPUP_PROP_AUTOHIDE:
      g_value_set_boolean (value, surface->autohide);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gdk_webrogue_popup_set_property (GObject *object,
                                 guint prop_id,
                                 const GValue *value,
                                 GParamSpec *pspec)
{
  GdkSurface *surface = GDK_SURFACE (object);

  switch (prop_id)
    {
    case LAST_PROP + GDK_POPUP_PROP_PARENT:
      surface->parent = g_value_dup_object (value);
      if (surface->parent != NULL)
        surface->parent->children = g_list_prepend (surface->parent->children, surface);
      break;

    case LAST_PROP + GDK_POPUP_PROP_AUTOHIDE:
      surface->autohide = g_value_get_boolean (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gdk_webrogue_popup_class_init (GdkWebroguePopupClass *class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);

  object_class->constructed = gdk_webrogue_popup_constructed;
  object_class->get_property = gdk_webrogue_popup_get_property;
  object_class->set_property = gdk_webrogue_popup_set_property;

  gdk_popup_install_properties (object_class, 1);
}

static gboolean
gdk_webrogue_popup_present (GdkPopup *popup,
                            int width,
                            int height,
                            GdkPopupLayout *layout)
{
  return gdk_webrogue_surface_present_popup (GDK_SURFACE (popup), width, height, layout);
}

static GdkGravity
gdk_webrogue_popup_get_surface_anchor (GdkPopup *popup)
{
  return GDK_SURFACE (popup)->popup.surface_anchor;
}

static GdkGravity
gdk_webrogue_popup_get_rect_anchor (GdkPopup *popup)
{
  return GDK_SURFACE (popup)->popup.rect_anchor;
}

static int
gdk_webrogue_popup_get_position_x (GdkPopup *popup)
{
  return GDK_SURFACE (popup)->x;
}

static int
gdk_webrogue_popup_get_position_y (GdkPopup *popup)
{
  return GDK_SURFACE (popup)->y;
}

static void
gdk_webrogue_popup_iface_init (GdkPopupInterface *iface)
{
  iface->present = gdk_webrogue_popup_present;
  iface->get_surface_anchor = gdk_webrogue_popup_get_surface_anchor;
  iface->get_rect_anchor = gdk_webrogue_popup_get_rect_anchor;
  iface->get_position_x = gdk_webrogue_popup_get_position_x;
  iface->get_position_y = gdk_webrogue_popup_get_position_y;
}

typedef struct
{
  GdkWebrogueSurface parent_instance;
} GdkWebrogueToplevel;

typedef struct
{
  GdkWebrogueSurfaceClass parent_class;
} GdkWebrogueToplevelClass;

static void gdk_webrogue_toplevel_iface_init (GdkToplevelInterface *iface);

G_DEFINE_TYPE_WITH_CODE (GdkWebrogueToplevel, gdk_webrogue_toplevel, GDK_TYPE_WEBROGUE_SURFACE, G_IMPLEMENT_INTERFACE (GDK_TYPE_TOPLEVEL, gdk_webrogue_toplevel_iface_init))

static void
gdk_webrogue_toplevel_init (GdkWebrogueToplevel *toplevel)
{
  _gdk_webrogue_events_add_surface (GDK_WEBROGUE_SURFACE (toplevel));
}

static void
gdk_webrogue_toplevel_constructed (GObject *object)
{
  GdkSurface *surface = GDK_SURFACE (object);
  GdkFrameClock *frame_clock;

  frame_clock = _gdk_frame_clock_idle_new ();
  gdk_surface_set_frame_clock (surface, frame_clock);
  g_object_unref (frame_clock);

  G_OBJECT_CLASS (gdk_webrogue_toplevel_parent_class)->constructed (object);
}

static void
gdk_webrogue_toplevel_set_property (GObject *object,
                                    guint prop_id,
                                    const GValue *value,
                                    GParamSpec *pspec)
{
  GdkSurface *surface = GDK_SURFACE (object);

  switch (prop_id)
    {
    case LAST_PROP + GDK_TOPLEVEL_PROP_TITLE:
      gdk_webrogue_surface_set_title (surface, g_value_get_string (value));
      g_object_notify_by_pspec (G_OBJECT (surface), pspec);
      break;

    case LAST_PROP + GDK_TOPLEVEL_PROP_STARTUP_ID:
      gdk_webrogue_surface_set_startup_id (surface, g_value_get_string (value));
      g_object_notify_by_pspec (G_OBJECT (surface), pspec);
      break;

    case LAST_PROP + GDK_TOPLEVEL_PROP_TRANSIENT_FOR:
      gdk_webrogue_surface_set_transient_for (surface, g_value_get_object (value));
      g_object_notify_by_pspec (G_OBJECT (surface), pspec);
      break;

    case LAST_PROP + GDK_TOPLEVEL_PROP_MODAL:
      gdk_webrogue_surface_set_modal_hint (surface, g_value_get_boolean (value));
      g_object_notify_by_pspec (G_OBJECT (surface), pspec);
      break;

    case LAST_PROP + GDK_TOPLEVEL_PROP_ICON_LIST:
      break;

    case LAST_PROP + GDK_TOPLEVEL_PROP_DECORATED:
      break;

    case LAST_PROP + GDK_TOPLEVEL_PROP_DELETABLE:
      break;

    case LAST_PROP + GDK_TOPLEVEL_PROP_SHORTCUTS_INHIBITED:
      break;

    case LAST_PROP + GDK_TOPLEVEL_PROP_GRAVITY:
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gdk_webrogue_toplevel_get_property (GObject *object,
                                    guint prop_id,
                                    GValue *value,
                                    GParamSpec *pspec)
{
  GdkSurface *surface = GDK_SURFACE (object);

  switch (prop_id)
    {
    case LAST_PROP + GDK_TOPLEVEL_PROP_STATE:
      g_value_set_flags (value, surface->state);
      break;

    case LAST_PROP + GDK_TOPLEVEL_PROP_TITLE:
      g_value_set_string (value, "");
      break;

    case LAST_PROP + GDK_TOPLEVEL_PROP_STARTUP_ID:
      g_value_set_string (value, "");
      break;

    case LAST_PROP + GDK_TOPLEVEL_PROP_TRANSIENT_FOR:
      g_value_set_object (value, surface->transient_for);
      break;

    case LAST_PROP + GDK_TOPLEVEL_PROP_MODAL:
      g_value_set_boolean (value, surface->modal_hint);
      break;

    case LAST_PROP + GDK_TOPLEVEL_PROP_ICON_LIST:
      g_value_set_pointer (value, NULL);
      break;

    case LAST_PROP + GDK_TOPLEVEL_PROP_DECORATED:
      break;

    case LAST_PROP + GDK_TOPLEVEL_PROP_DELETABLE:
      break;

    case LAST_PROP + GDK_TOPLEVEL_PROP_SHORTCUTS_INHIBITED:
      g_value_set_boolean (value, surface->shortcuts_inhibited);
      break;

    case LAST_PROP + GDK_TOPLEVEL_PROP_CAPABILITIES:
      g_value_set_flags (value, GDK_TOPLEVEL_CAPABILITIES_MAXIMIZE |
                                    GDK_TOPLEVEL_CAPABILITIES_FULLSCREEN |
                                    GDK_TOPLEVEL_CAPABILITIES_MINIMIZE);
      break;

    case LAST_PROP + GDK_TOPLEVEL_PROP_GRAVITY:
      g_value_set_boolean (value, GDK_GRAVITY_NORTH_EAST);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gdk_webrogue_toplevel_class_init (GdkWebrogueToplevelClass *class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);

  object_class->constructed = gdk_webrogue_toplevel_constructed;
  object_class->get_property = gdk_webrogue_toplevel_get_property;
  object_class->set_property = gdk_webrogue_toplevel_set_property;

  gdk_toplevel_install_properties (object_class, 1);
}

static void
show_surface (GdkSurface *surface)
{
  gboolean was_mapped;

  if (surface->destroyed)
    return;

  was_mapped = GDK_SURFACE_IS_MAPPED (surface);

  if (!was_mapped)
    gdk_surface_set_is_mapped (surface, TRUE);

  if (!was_mapped)
    gdk_surface_invalidate_rect (surface, NULL);
}

static void
gdk_webrogue_toplevel_present (GdkToplevel *toplevel,
                               GdkToplevelLayout *layout)
{
  GdkSurface *surface = GDK_SURFACE (toplevel);

  gdk_surface_request_layout (surface);
  show_surface (surface);
}

static gboolean
gdk_webrogue_toplevel_minimize (GdkToplevel *toplevel)
{
  return TRUE;
}

static void
gdk_webrogue_toplevel_focus (GdkToplevel *toplevel,
                             guint32 timestamp)
{
}

static void
gdk_webrogue_toplevel_iface_init (GdkToplevelInterface *iface)
{
  iface->present = gdk_webrogue_toplevel_present;
  iface->minimize = gdk_webrogue_toplevel_minimize;
  iface->focus = gdk_webrogue_toplevel_focus;
  iface->begin_resize = gdk_webrogue_toplevel_begin_resize;
  iface->begin_move = gdk_webrogue_toplevel_begin_move;
}

typedef struct
{
  GdkWebrogueSurface parent_instance;
} GdkWebrogueDragSurface;

typedef struct
{
  GdkWebrogueSurfaceClass parent_class;
} GdkWebrogueDragSurfaceClass;

static void gdk_webrogue_drag_surface_iface_init (GdkDragSurfaceInterface *iface);

G_DEFINE_TYPE_WITH_CODE (GdkWebrogueDragSurface, gdk_webrogue_drag_surface, GDK_TYPE_WEBROGUE_SURFACE, G_IMPLEMENT_INTERFACE (GDK_TYPE_DRAG_SURFACE, gdk_webrogue_drag_surface_iface_init))

static void
gdk_webrogue_drag_surface_init (GdkWebrogueDragSurface *surface)
{
}

static void
gdk_webrogue_drag_surface_constructed (GObject *object)
{
  GdkSurface *surface = GDK_SURFACE (object);
  GdkFrameClock *frame_clock;

  frame_clock = _gdk_frame_clock_idle_new ();
  gdk_surface_set_frame_clock (surface, frame_clock);
  g_object_unref (frame_clock);

  G_OBJECT_CLASS (gdk_webrogue_drag_surface_parent_class)->constructed (object);
}

static void
gdk_webrogue_drag_surface_class_init (GdkWebrogueDragSurfaceClass *class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);

  object_class->constructed = gdk_webrogue_drag_surface_constructed;
}

static gboolean
gdk_webrogue_drag_surface_present (GdkDragSurface *drag_surface,
                                   int width,
                                   int height)
{
  GdkSurface *surface = GDK_SURFACE (drag_surface);

  gdk_webrogue_surface_toplevel_resize (surface, width, height);
  show_surface (surface);

  return TRUE;
}

static void
gdk_webrogue_drag_surface_iface_init (GdkDragSurfaceInterface *iface)
{
  iface->present = gdk_webrogue_drag_surface_present;
}

wr_window
gdk_webrogue_surface_get_wr_window (GdkSurface *surface)
{
  GdkWebrogueSurface *impl;

  g_return_val_if_fail (GDK_IS_WEBROGUE_SURFACE (surface), NULL);

  impl = GDK_WEBROGUE_SURFACE (surface);
  return impl->wr_window;
}
