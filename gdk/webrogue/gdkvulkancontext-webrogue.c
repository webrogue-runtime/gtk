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

#include "gdkconfig.h"

#ifdef GDK_RENDERING_VULKAN

#include "gdkvulkancontext-webrogue.h"

#include "gdkdisplay-webrogue.h"
#include "gdkprivate-webrogue.h"
#include "gdksurface-webrogue.h"

G_DEFINE_TYPE (GdkWebrogueVulkanContext, gdk_webrogue_vulkan_context, GDK_TYPE_VULKAN_CONTEXT)

static VkResult
gdk_webrogue_vulkan_context_create_surface (GdkVulkanContext *context,
                                            VkSurfaceKHR *vr_surface)
{
  GdkSurface *surface = gdk_draw_context_get_surface (GDK_DRAW_CONTEXT (context));

  return GDK_VK_CHECK (vkCreateSurfaceWEBROGUE, gdk_vulkan_context_get_instance (context),
                       &(VkSurfaceCreateInfoWEBROGUE) {
                           VK_STRUCTURE_TYPE_SURFACE_CREATE_INFO_WEBROGUE,
                           NULL,
                           0,
                           gdk_webrogue_surface_get_wr_window (surface) },
                       NULL,
                       vr_surface);
}

static void
gdk_vulkan_context_webrogue_end_frame (GdkDrawContext *context,
                                       gpointer context_data,
                                       cairo_region_t *painted)
{
  GdkSurface *surface = gdk_draw_context_get_surface (GDK_DRAW_CONTEXT (context));
  GdkWebrogueSurface *impl = GDK_WEBROGUE_SURFACE (surface);
  GDK_DRAW_CONTEXT_CLASS (gdk_webrogue_vulkan_context_parent_class)->end_frame (context, context_data, painted);
}

static void
gdk_vulkan_context_webrogue_empty_frame (GdkDrawContext *context)
{
  // GdkSurface *surface = gdk_draw_context_get_surface (GDK_DRAW_CONTEXT (context));

  // gdk_webrogue_surface_handle_empty_frame (surface);
  // abort ();
}

static void
gdk_webrogue_vulkan_context_class_init (GdkWebrogueVulkanContextClass *klass)
{
  GdkVulkanContextClass *vulkan_context_class = GDK_VULKAN_CONTEXT_CLASS (klass);
  GdkDrawContextClass *draw_context_class = GDK_DRAW_CONTEXT_CLASS (klass);

  vulkan_context_class->create_surface = gdk_webrogue_vulkan_context_create_surface;

  draw_context_class->end_frame = gdk_vulkan_context_webrogue_end_frame;
  draw_context_class->empty_frame = gdk_vulkan_context_webrogue_empty_frame;
}

static void
gdk_webrogue_vulkan_context_init (GdkWebrogueVulkanContext *self)
{
}

#endif /* GDK_RENDERING_VULKAN */
