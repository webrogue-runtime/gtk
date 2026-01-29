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

#include "config.h"

#include <stdint.h>

#include <gdk/gdk.h>
#include <gdk/gdkdisplayprivate.h>
#include <gdk/gdkkeys.h>
#include <gdk/gdksurface.h>
#include <glib.h>

#include <epoxy/egl.h>

G_BEGIN_DECLS

struct _GdkWebrogueDisplay
{
  GdkDisplay parent_instance;
  GListStore *monitors;
  GSource *event_source;
};

struct _GdkWebrogueDisplayClass
{
  GdkDisplayClass parent_class;
};

GdkDisplay *_gdk_webrogue_display_open (const char *display_name);

GType gdk_webrogue_display_get_type (void);
#define GDK_TYPE_WEBROGUE_DISPLAY (gdk_webrogue_display_get_type ())

typedef struct _GdkWebrogueDisplay GdkWebrogueDisplay;
typedef struct _GdkWebrogueDisplayClass GdkWebrogueDisplayClass;

#define GDK_WEBROGUE_DISPLAY(object) (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_WEBROGUE_DISPLAY, GdkWebrogueDisplay))
#define GDK_IS_WEBROGUE_DISPLAY(object) (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_WEBROGUE_DISPLAY))

G_END_DECLS
