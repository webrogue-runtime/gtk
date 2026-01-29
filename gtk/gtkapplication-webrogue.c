/*
 * Copyright © 2026 Artem Borovik
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the licence, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Florian "sp1rit" <sp1rit@disroot.org>
 */

#include "config.h"

#include "gtkapplicationprivate.h"

typedef GtkApplicationImplClass GtkApplicationImplWebrogueClass;

typedef struct
{
  GtkApplicationImpl impl;
} GtkApplicationImplWebrogue;

G_DEFINE_TYPE (GtkApplicationImplWebrogue, gtk_application_impl_webrogue, GTK_TYPE_APPLICATION_IMPL)

static guint
gtk_application_impl_webrogue_inhibit (GtkApplicationImpl *impl,
                                       GtkWindow *window,
                                       GtkApplicationInhibitFlags flags,
                                       const char *reason)
{
  // if (flags & GTK_APPLICATION_INHIBIT_SUSPEND)
  // TODO: iterate over all active surfaces and call toplevel_inhibit_suspend for all toplevels
  // potentionally for GTK_APPLICATION_INHIBIT_SWITCH lockTask mode?
  return 0;
}

static void
gtk_application_impl_webrogue_uninhibit (GtkApplicationImpl *impl, guint cookie)
{
}

static void
gtk_application_impl_webrogue_init (GtkApplicationImplWebrogue *self)
{
}

static void
gtk_application_impl_webrogue_startup (GtkApplicationImpl *impl,
                                       gboolean support_save)
{
}

static void
gtk_application_impl_webrogue_before_emit (GtkApplicationImpl *impl,
                                           GVariant *platform_data)
{
}
static void
gtk_application_impl_webrogue_window_added (GtkApplicationImpl *impl,
                                            GtkWindow *window,
                                            GVariant *state)
{
}

static void
gtk_application_impl_webrogue_active_window_changed (GtkApplicationImpl *impl,
                                                     GtkWindow *window)
{
}

static void
gtk_application_impl_webrogue_handle_window_realize (GtkApplicationImpl *impl,
                                                     GtkWindow *window)
{
}

static void
gtk_application_impl_webrogue_handle_window_map (GtkApplicationImpl *impl,
                                                 GtkWindow *window)
{
}

static void
gtk_application_impl_webrogue_class_init (GtkApplicationImplClass *klass)
{
  klass->startup = gtk_application_impl_webrogue_startup;
  klass->inhibit = gtk_application_impl_webrogue_inhibit;
  klass->uninhibit = gtk_application_impl_webrogue_uninhibit;
  klass->before_emit = gtk_application_impl_webrogue_before_emit;
  klass->window_added = gtk_application_impl_webrogue_window_added;
  klass->active_window_changed = gtk_application_impl_webrogue_active_window_changed;
  klass->handle_window_realize = gtk_application_impl_webrogue_handle_window_realize;
  klass->handle_window_map = gtk_application_impl_webrogue_handle_window_map;
}
