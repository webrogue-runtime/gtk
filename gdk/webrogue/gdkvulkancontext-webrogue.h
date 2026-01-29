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

#include "gdkconfig.h"

#ifdef GDK_RENDERING_VULKAN

#define VK_USE_PLATFORM_WEBROGUE

#include "gdkvulkancontextprivate.h"

G_BEGIN_DECLS

#define GDK_TYPE_WEBROGUE_VULKAN_CONTEXT (gdk_webrogue_vulkan_context_get_type ())
#define GDK_WEBROGUE_VULKAN_CONTEXT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GDK_TYPE_WEBROGUE_VULKAN_CONTEXT, GdkWebrogueVulkanContext))
#define GDK_IS_WEBROGUE_VULKAN_CONTEXT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GDK_TYPE_WEBROGUE_VULKAN_CONTEXT))
#define GDK_WEBROGUE_VULKAN_CONTEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_WEBROGUE_VULKAN_CONTEXT, GdkWebrogueVulkanContextClass))
#define GDK_IS_WEBROGUE_VULKAN_CONTEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_WEBROGUE_VULKAN_CONTEXT))
#define GDK_WEBROGUE_VULKAN_CONTEXT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_WEBROGUE_VULKAN_CONTEXT, GdkWebrogueVulkanContextClass))

typedef struct _GdkWebrogueVulkanContext GdkWebrogueVulkanContext;
typedef struct _GdkWebrogueVulkanContextClass GdkWebrogueVulkanContextClass;

struct _GdkWebrogueVulkanContext
{
  GdkVulkanContext parent_instance;
};

struct _GdkWebrogueVulkanContextClass
{
  GdkVulkanContextClass parent_class;
};

GType gdk_webrogue_vulkan_context_get_type (void) G_GNUC_CONST;

G_END_DECLS

#endif /* !GDK_RENDERING_VULKAN */
