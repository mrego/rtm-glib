/*
 * rtm-time-zone.h: Represents a time zone of Rememeber The Milk
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

#ifndef __RTM_TIME_ZONE_H__
#define __RTM_TIME_ZONE_H__

#include <glib-object.h>
#include <rest/rest-xml-parser.h>


G_BEGIN_DECLS

#define RTM_TYPE_TIME_ZONE (rtm_time_zone_get_type ())
#define RTM_TIME_ZONE(obj)                                                   \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), RTM_TYPE_TIME_ZONE, RtmTimeZone))
#define RTM_IS_TIME_ZONE(obj)                                        \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), RTM_TYPE_TIME_ZONE))
#define RTM_TIME_ZONE_CLASS(klass)                                           \
        (G_TYPE_CHECK_CLASS_CAST ((klass), RTM_TYPE_TIME_ZONE, RtmTimeZoneClass))
#define RTM_IS_TIME_ZONE_CLASS(klass)                                \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), RTM_TYPE_TIME_ZONE))
#define RTM_TIME_ZONE_GET_CLASS(obj)                                         \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), RTM_TYPE_TIME_ZONE, RtmTimeZoneClass))

typedef struct _RtmTimeZone RtmTimeZone;
typedef struct _RtmTimeZoneClass RtmTimeZoneClass;
typedef struct _RtmTimeZonePrivate RtmTimeZonePrivate;

struct _RtmTimeZone {
        GObject parent_instance;

        /*< private >*/
        RtmTimeZonePrivate *priv;
};

struct _RtmTimeZoneClass {
        GObjectClass parent_class;
};

GType
rtm_time_zone_get_type (void) G_GNUC_CONST;

RtmTimeZone *
rtm_time_zone_new (void);

gchar *
rtm_time_zone_get_id (RtmTimeZone *time_zone);

gboolean
rtm_time_zone_set_id (RtmTimeZone *time_zone, gchar* id);

gchar *
rtm_time_zone_get_name (RtmTimeZone *time_zone);

gboolean
rtm_time_zone_set_name (RtmTimeZone *time_zone, gchar* name);

gchar *
rtm_time_zone_get_offset (RtmTimeZone *time_zone);

gboolean
rtm_time_zone_is_dst (RtmTimeZone *time_zone);

gboolean
        rtm_time_zone_set_dst (RtmTimeZone *time_zone, gboolean dst);

gboolean
rtm_time_zone_set_offset (RtmTimeZone *time_zone, gchar* offset);

gchar *
rtm_time_zone_get_current_offset (RtmTimeZone *time_zone);

gboolean
rtm_time_zone_set_current_offset (RtmTimeZone *time_zone, gchar* current_offset);

void
rtm_time_zone_load_data (RtmTimeZone *time_zone, RestXmlNode *node);

gchar *
rtm_time_zone_to_string (RtmTimeZone *time_zone);

#endif /* __RTM_TIME_ZONE_H__ */
