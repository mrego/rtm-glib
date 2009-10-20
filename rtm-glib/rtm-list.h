/*
 * rtm-list.h: Represents a list of tasks
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

#ifndef __RTM_LIST_H__
#define __RTM_LIST_H__

#include <glib-object.h>
#include <rest/rest-xml-parser.h>
#include <rtm-glib/rtm-task.h>


G_BEGIN_DECLS

#define RTM_TYPE_LIST (rtm_list_get_type ())
#define RTM_LIST(obj)                                                   \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), RTM_TYPE_LIST, RtmList))
#define RTM_IS_LIST(obj)                                        \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), RTM_TYPE_LIST))
#define RTM_LIST_CLASS(klass)                                           \
        (G_TYPE_CHECK_CLASS_CAST ((klass), RTM_TYPE_LIST, RtmListClass))
#define RTM_IS_LIST_CLASS(klass)                                \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), RTM_TYPE_LIST))
#define RTM_LIST_GET_CLASS(obj)                                         \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), RTM_TYPE_LIST, RtmListClass))

typedef struct _RtmList RtmList;
typedef struct _RtmListClass RtmListClass;
typedef struct _RtmListPrivate RtmListPrivate;

struct _RtmList {
        GObject parent_instance;

        /*< private >*/
        RtmListPrivate *priv;
};

struct _RtmListClass {
        GObjectClass parent_class;
};

GType
rtm_list_get_type (void) G_GNUC_CONST;

RtmList *
rtm_list_new (void);

gchar *
rtm_list_get_id (RtmList *list);

gboolean
rtm_list_set_id (RtmList *list, gchar* id);

gchar *
rtm_list_get_name (RtmList *list);

gboolean
rtm_list_set_name (RtmList *list, gchar* name);

gboolean
rtm_list_is_deleted (RtmList *list);

gboolean
rtm_list_set_deleted (RtmList *list, gboolean deleted);

gboolean
rtm_list_is_locked (RtmList *list);

gboolean
rtm_list_set_locked (RtmList *list, gboolean locked);

gboolean
rtm_list_is_archived (RtmList *list);

gboolean
rtm_list_set_archived (RtmList *list, gboolean archived);

gchar *
rtm_list_get_position (RtmList *list);

gboolean
rtm_list_set_position (RtmList *list, gchar* position);

gboolean
rtm_list_is_smart (RtmList *list);

gboolean
rtm_list_set_smart (RtmList *list, gboolean smart);

gchar *
rtm_list_get_sort_order (RtmList *list);

gboolean
rtm_list_set_sort_order (RtmList *list, gchar* sort_order);

gchar *
rtm_list_get_filter (RtmList *list);

gboolean
rtm_list_set_filter (RtmList *list, gchar* filter);

void
rtm_list_load_data (RtmList *list, RestXmlNode *node);

gchar *
rtm_list_to_string (RtmList *list);

GList *
rtm_list_get_tasks (RtmList *list);

RtmTask *
rtm_list_find_task (RtmList *list, gchar *task_id);

gboolean
rtm_list_add_task (RtmList *list, RtmTask *task, GError **error);

gboolean
rtm_list_remove_task (RtmList *list, gchar *task_id, GError **error);

#endif /* __RTM_LIST_H__ */
