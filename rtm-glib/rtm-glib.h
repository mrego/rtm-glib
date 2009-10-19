/*
 * rtm-glib.h: API Library
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

#ifndef __RTM_GLIB_H__
#define __RTM_GLIB_H__

#include <glib-object.h>
#include <rest/rest-xml-parser.h>
#include <rtm-glib/rtm-task.h>
#include <rtm-glib/rtm-list.h>


G_BEGIN_DECLS

#define RTM_TYPE_GLIB (rtm_glib_get_type ())
#define RTM_GLIB(obj) \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), RTM_TYPE_GLIB, RtmGlib))
#define RTM_IS_GLIB(obj) \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), RTM_TYPE_GLIB))
#define RTM_GLIB_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_CAST ((klass), RTM_TYPE_GLIB, RtmGlibClass))
#define RTM_IS_GLIB_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), RTM_TYPE_GLIB))
#define RTM_GLIB_GET_CLASS(obj)        \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), RTM_TYPE_GLIB, RtmGlibClass))

typedef struct _RtmGlib RtmGlib;
typedef struct _RtmGlibClass RtmGlibClass;
typedef struct _RtmGlibPrivate RtmGlibPrivate;

struct _RtmGlib {
        GObject parent_instance;

        /*< private >*/
        RtmGlibPrivate *priv;
};

struct _RtmGlibClass {
        GObjectClass parent_class;
};

GType
rtm_glib_get_type (void) G_GNUC_CONST;

RtmGlib *
rtm_glib_new (gchar *api_key, gchar *shared_secret);

gboolean
rtm_glib_test_echo (RtmGlib *rtm, GError **error);

gchar *
rtm_glib_test_login (RtmGlib *rtm, gchar *auth_token, GError **error);

gchar *
rtm_glib_auth_get_frob (RtmGlib *rtm, GError **error);

gchar *
rtm_glib_auth_get_token (RtmGlib *rtm, gchar *frob, GError **error);

gboolean
rtm_glib_auth_check_token (RtmGlib *rtm, gchar *auth_token, GError **error);

gchar *
rtm_glib_auth_get_login_url (RtmGlib *rtm, gchar *frob);

GList *
rtm_glib_tasks_get_list (RtmGlib *rtm, gchar *list_id, gchar *filter,
                         gchar *last_sync, GError **error);

GList *
rtm_glib_lists_get_list (RtmGlib *rtm, GError **error);

gchar *
rtm_glib_timelines_create (RtmGlib *rtm, GError **error);

RtmTask *
rtm_glib_tasks_add (RtmGlib *rtm, gchar* timeline, gchar *task_name,
                    gchar *list_id, gboolean parse, GError **error);

gboolean
rtm_glib_transactions_undo (RtmGlib *rtm, gchar *timeline,
                            gchar* transaction_id, GError **error);

gchar *
rtm_glib_tasks_delete (RtmGlib *rtm, gchar* timeline, RtmTask *task,
                       GError **error);

gchar *
rtm_glib_tasks_set_name (RtmGlib *rtm, gchar* timeline, RtmTask *task,
                         gchar *name, GError **error);

RtmList *
rtm_glib_lists_add (RtmGlib *rtm, gchar* timeline, gchar *list_name,
                    GError **error);

gchar *
rtm_glib_lists_delete (RtmGlib *rtm, gchar* timeline, RtmList *list,
                       GError **error);

gchar *
rtm_glib_lists_set_name (RtmGlib *rtm, gchar* timeline, RtmList *list,
                         gchar *name, GError **error);

gboolean
rtm_glib_lists_set_default (RtmGlib *rtm, gchar* timeline, RtmList *list,
                            GError **error);

gchar *
rtm_glib_lists_archive (RtmGlib *rtm, gchar* timeline, RtmList *list,
                        GError **error);

gchar *
rtm_glib_lists_unarchive (RtmGlib *rtm, gchar* timeline, RtmList *list,
                          GError **error);

gchar *
rtm_glib_tasks_set_url (RtmGlib *rtm, gchar* timeline, RtmTask *task,
                        gchar *url, GError **error);

gchar *
rtm_glib_tasks_set_tags (RtmGlib *rtm, gchar* timeline, RtmTask *task,
                         gchar *tags, GError **error);

gchar *
rtm_glib_tasks_add_tags (RtmGlib *rtm, gchar* timeline, RtmTask *task,
                         gchar *tags, GError **error);

gchar *
rtm_glib_tasks_remove_tags (RtmGlib *rtm, gchar* timeline, RtmTask *task,
                            gchar *tags, GError **error);

gchar *
rtm_glib_tasks_set_location (RtmGlib *rtm, gchar* timeline, RtmTask *task,
                             gchar *location_id, GError **error);

#endif /* __RTM_GLIB_H__ */
