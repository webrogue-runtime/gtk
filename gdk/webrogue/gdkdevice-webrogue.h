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

#include <gdk/gdkdeviceprivate.h>

G_BEGIN_DECLS

#define GDK_TYPE_WEBROGUE_DEVICE (gdk_webrogue_device_get_type ())
#define GDK_WEBROGUE_DEVICE(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), GDK_TYPE_WEBROGUE_DEVICE, GdkWebrogueDevice))
#define GDK_WEBROGUE_DEVICE_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), GDK_TYPE_WEBROGUE_DEVICE, GdkWebrogueDeviceClass))
#define GDK_IS_WEBROGUE_DEVICE(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), GDK_TYPE_WEBROGUE_DEVICE))
#define GDK_IS_WEBROGUE_DEVICE_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), GDK_TYPE_WEBROGUE_DEVICE))
#define GDK_WEBROGUE_DEVICE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), GDK_TYPE_WEBROGUE_DEVICE, GdkWebrogueDeviceClass))

typedef struct _GdkWebrogueDevice GdkWebrogueDevice;
typedef struct _GdkWebrogueDeviceClass GdkWebrogueDeviceClass;

struct _GdkWebrogueDevice
{
  GdkDevice parent_instance;
};

struct _GdkWebrogueDeviceClass
{
  GdkDeviceClass parent_class;
};

GType gdk_webrogue_device_get_type (void) G_GNUC_CONST;

G_END_DECLS
