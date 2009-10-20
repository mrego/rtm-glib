/*
 * rtm-location.h: Represents a location for a task
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

#ifndef __RTM_LOCATION_H__
#define __RTM_LOCATION_H__

#include <glib-object.h>
#include <rest/rest-xml-parser.h>
#include <rtm-glib/rtm-task.h>


G_BEGIN_DECLS

#define RTM_TYPE_LOCATION (rtm_location_get_type ())
#define RTM_LOCATION(obj)                                                   \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), RTM_TYPE_LOCATION, RtmLocation))
#define RTM_IS_LOCATION(obj)                                        \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), RTM_TYPE_LOCATION))
#define RTM_LOCATION_CLASS(klass)                                           \
        (G_TYPE_CHECK_CLASS_CAST ((klass), RTM_TYPE_LOCATION, RtmLocationClass))
#define RTM_IS_LOCATION_CLASS(klass)                                \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), RTM_TYPE_LOCATION))
#define RTM_LOCATION_GET_CLASS(obj)                                         \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), RTM_TYPE_LOCATION, RtmLocationClass))

typedef struct _RtmLocation RtmLocation;
typedef struct _RtmLocationClass RtmLocationClass;
typedef struct _RtmLocationPrivate RtmLocationPrivate;

struct _RtmLocation {
        GObject parent_instance;

        /*< private >*/
        RtmLocationPrivate *priv;
};

struct _RtmLocationClass {
        GObjectClass parent_class;
};

GType
rtm_location_get_type (void) G_GNUC_CONST;

RtmLocation *
rtm_location_new (void);

gchar *
rtm_location_get_id (RtmLocation *location);

gboolean
rtm_location_set_id (RtmLocation *location, gchar* id);

gchar *
rtm_location_get_name (RtmLocation *location);

gboolean
rtm_location_set_name (RtmLocation *location, gchar* name);

gchar *
rtm_location_get_longitude (RtmLocation *location);

gboolean
rtm_location_set_longitude (RtmLocation *location, gchar* longitude);

gchar *
rtm_location_get_latitude (RtmLocation *location);

gboolean
rtm_location_set_latitude (RtmLocation *location, gchar* latitude);

gchar *
rtm_location_get_zoom (RtmLocation *location);

gboolean
rtm_location_set_zoom (RtmLocation *location, gchar* zoom);

gchar *
rtm_location_get_address (RtmLocation *location);

gboolean
rtm_location_set_address (RtmLocation *location, gchar* address);

gboolean
rtm_location_is_viewable (RtmLocation *location);

gboolean
rtm_location_set_viewable (RtmLocation *location, gboolean viewable);

void
rtm_location_load_data (RtmLocation *location, RestXmlNode *node);

gchar *
rtm_location_to_string (RtmLocation *location);

#endif /* __RTM_LOCATION_H__ */
