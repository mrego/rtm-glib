/*
 * rtm-task.h: Represents a task
 *
 * Copyright (C) 2009 Manuel Rego Casasnovas <mrego@igalia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */

#ifndef __RTM_TASK_H__
#define __RTM_TASK_H__

#include <glib-object.h>
#include <rest/rest-xml-parser.h>

G_BEGIN_DECLS

#define RTM_TYPE_TASK (rtm_task_get_type ())
#define RTM_TASK(obj)                                                   \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), RTM_TYPE_TASK, RtmTask))
#define RTM_IS_TASK(obj)                                        \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), RTM_TYPE_TASK))
#define RTM_TASK_CLASS(klass)                                           \
        (G_TYPE_CHECK_CLASS_CAST ((klass), RTM_TYPE_TASK, RtmTaskClass))
#define RTM_IS_TASK_CLASS(klass)                                \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), RTM_TYPE_TASK))
#define RTM_TASK_GET_CLASS(obj)                                         \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), RTM_TYPE_TASK, RtmTaskClass))

typedef struct _RtmTask RtmTask;
typedef struct _RtmTaskClass RtmTaskClass;
typedef struct _RtmTaskPrivate RtmTaskPrivate;

struct _RtmTask {
        GObject parent_instance;

        /*< private >*/
        RtmTaskPrivate *priv;
};

struct _RtmTaskClass {
        GObjectClass parent_class;
};

GType
rtm_task_get_type (void) G_GNUC_CONST;

RtmTask *
rtm_task_new (void);

gchar *
rtm_task_get_id (RtmTask *task);

gboolean
rtm_task_set_id (RtmTask *task, gchar* id);

gchar *
rtm_task_get_taskseries_id (RtmTask *task);

gboolean
rtm_task_set_taskseries_id (RtmTask *task, gchar* taskseries_id);

gchar *
rtm_task_get_list_id (RtmTask *task);

gboolean
rtm_task_set_list_id (RtmTask *task, gchar* list_id);

gchar *
rtm_task_get_name (RtmTask *task);

gboolean
rtm_task_set_name (RtmTask *task, gchar* name);

gchar *
rtm_task_get_priority (RtmTask *task);

gboolean
rtm_task_set_priority (RtmTask *task, gchar* priority);

void
rtm_task_load_data (RtmTask *task, RestXmlNode *node, const gchar *list_id);

gchar *
rtm_task_to_string (RtmTask *task);

gchar *
rtm_task_get_url (RtmTask *task);

gboolean
rtm_task_set_url (RtmTask *task, gchar* url);

GList *
rtm_task_get_tags (RtmTask *task);

gchar *
rtm_task_find_tag (RtmTask *task, gchar* tag);

gboolean
rtm_task_add_tag (RtmTask *task, gchar* tag, GError **error);

gboolean
rtm_task_remove_tag (RtmTask *task, gchar* tag, GError **error);

gchar *
rtm_task_get_location_id (RtmTask *task);

gboolean
rtm_task_set_location_id (RtmTask *task, gchar* location_id);

GTimeVal *
rtm_task_get_due_date (RtmTask *task);

gboolean
rtm_task_set_due_date (RtmTask *task, GTimeVal *due_date);

GTimeVal *
rtm_task_get_added_date (RtmTask *task);

gboolean
rtm_task_set_added_date (RtmTask *task, GTimeVal *added_date);

GTimeVal *
rtm_task_get_completed_date (RtmTask *task);

gboolean
rtm_task_set_completed_date (RtmTask *task, GTimeVal *completed_date);

GTimeVal *
rtm_task_get_deleted_date (RtmTask *task);

gboolean
rtm_task_set_deleted_date (RtmTask *task, GTimeVal *deleted_date);

gboolean
rtm_task_has_due_time (RtmTask *task);

gboolean
rtm_task_set_has_due_time (RtmTask *task, gboolean has_due_time);

gchar *
rtm_task_get_estimate (RtmTask *task);

gboolean
rtm_task_set_estimate (RtmTask *task, gchar* estimate);

guint
rtm_task_get_postponed (RtmTask *task);

gboolean
rtm_task_set_postponed (RtmTask *task, guint postponed);

#endif /* __RTM_TASK_H__ */
