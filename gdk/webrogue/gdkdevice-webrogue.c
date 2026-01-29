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
#include <stdlib.h>

#include "gdkdevice-webrogue.h"

#include "gdkprivate-webrogue.h"
#include "gdksurfaceprivate.h"

G_DEFINE_TYPE (GdkWebrogueDevice, gdk_webrogue_device, GDK_TYPE_DEVICE)

static void
gdk_webrogue_device_init (GdkWebrogueDevice *device_core)
{
  abort ();
}

static void
_gdk_webrogue_device_set_surface_cursor (GdkDevice *device,
                                         GdkSurface *surface,
                                         GdkCursor *cursor)
{
  abort ();
}

static GdkGrabStatus
_gdk_webrogue_device_grab (GdkDevice *device,
                           GdkSurface *surface,
                           gboolean owner_events,
                           GdkEventMask event_mask,
                           GdkSurface *confine_to,
                           GdkCursor *cursor,
                           guint32 time_)
{
  abort ();
}

static void
_gdk_webrogue_device_ungrab (GdkDevice *device,
                             guint32 time_)
{
  abort ();
}

static GdkSurface *
_gdk_webrogue_device_surface_at_position (GdkDevice *device,
                                          double *win_x,
                                          double *win_y,
                                          GdkModifierType *mask)
{
  abort ();
}

static void
gdk_webrogue_device_class_init (GdkWebrogueDeviceClass *klass)
{
  GdkDeviceClass *device_class = GDK_DEVICE_CLASS (klass);

  device_class->set_surface_cursor = _gdk_webrogue_device_set_surface_cursor;
  device_class->grab = _gdk_webrogue_device_grab;
  device_class->ungrab = _gdk_webrogue_device_ungrab;
  device_class->surface_at_position = _gdk_webrogue_device_surface_at_position;
}
