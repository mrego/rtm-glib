/*
 * rtm-time-zone.c: Represents a time zone of Rememeber The Milk
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

/**
 * SECTION:rtm-time-zone
 * @short_description: A class that represents a time zone
 *
 * #RtmTimeZone represents a time zone of Remeber The Milk.
 */

#include <rtm-time-zone.h>
#include <rtm-util.h>

#define RTM_TIME_ZONE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE (        \
                                           (obj), RTM_TYPE_TIME_ZONE, RtmTimeZonePrivate))

struct _RtmTimeZonePrivate {
        gchar *id;
        gchar *name;
        gboolean dst;
        gchar *offset;
        gchar *current_offset;
};

enum {
        PROP_0,

        PROP_ID,
        PROP_NAME,
        PROP_DST,
        PROP_OFFSET,
        PROP_CURRENT_OFFSET,
};

G_DEFINE_TYPE (RtmTimeZone, rtm_time_zone, G_TYPE_OBJECT);

static void
rtm_time_zone_get_property (GObject *gobject, guint prop_id, GValue *value,
                           GParamSpec *pspec)
{
        RtmTimeZonePrivate *priv = RTM_TIME_ZONE_GET_PRIVATE (RTM_TIME_ZONE (gobject));

        switch (prop_id) {
        case PROP_ID:
                g_value_set_string (value, priv->id);
                break;

        case PROP_NAME:
                g_value_set_string (value, priv->name);
                break;

        case PROP_DST:
                g_value_set_boolean (value, priv->dst);

        case PROP_OFFSET:
                g_value_set_string (value, priv->offset);
                break;

        case PROP_CURRENT_OFFSET:
                g_value_set_string (value, priv->current_offset);
                break;

        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id,
                                                   pspec);
                break;
        }
}

static void
rtm_time_zone_set_property (GObject *gobject, guint prop_id, const GValue *value,
                           GParamSpec *pspec)
{
        RtmTimeZonePrivate *priv = RTM_TIME_ZONE_GET_PRIVATE (RTM_TIME_ZONE (gobject));

        switch (prop_id) {
        case PROP_ID:
                g_free (priv->id);
                priv->id = g_value_dup_string (value);
                break;

        case PROP_NAME:
                g_free (priv->name);
                priv->name = g_value_dup_string (value);
                break;

        case PROP_DST:
                priv->dst = g_value_get_boolean (value);
                break;

        case PROP_OFFSET:
                g_free (priv->offset);
                priv->offset = g_value_dup_string (value);
                break;

        case PROP_CURRENT_OFFSET:
                g_free (priv->current_offset);
                priv->current_offset = g_value_dup_string (value);
                break;

        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id,
                                                   pspec);
                break;
        }
}

static void
rtm_time_zone_finalize (GObject *gobject)
{
        RtmTimeZonePrivate *priv = RTM_TIME_ZONE_GET_PRIVATE (RTM_TIME_ZONE (gobject));

        g_free (priv->id);
        g_free (priv->name);
        g_free (priv->offset);
        g_free (priv->current_offset);

        G_OBJECT_CLASS (rtm_time_zone_parent_class)->finalize (gobject);
}

static void
rtm_time_zone_class_init (RtmTimeZoneClass *klass)
{
        GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

        g_type_class_add_private (klass, sizeof (RtmTimeZonePrivate));

        gobject_class->get_property = rtm_time_zone_get_property;
        gobject_class->set_property = rtm_time_zone_set_property;
        gobject_class->finalize = rtm_time_zone_finalize;

        g_object_class_install_property (
                gobject_class,
                PROP_ID,
                g_param_spec_string (
                        "id",
                        "ID",
                        "The identifier of the time zone",
                        NULL,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_NAME,
                g_param_spec_string (
                        "name",
                        "Name",
                        "The name of the time zone",
                        NULL,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_DST,
                g_param_spec_boolean (
                        "DST",
                        "Daylight Saving Time",
                        "Daylight Saving Time is set or not for the time zone",
                        FALSE,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_OFFSET,
                g_param_spec_string (
                        "offset",
                        "Offset",
                        "The offset of the time zone",
                        NULL,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_CURRENT_OFFSET,
                g_param_spec_string (
                        "current_offset",
                        "Current_Offset",
                        "The current offset of the time zone",
                        NULL,
                        G_PARAM_READWRITE));

}

static void
rtm_time_zone_init (RtmTimeZone *time_zone)
{
        time_zone->priv = RTM_TIME_ZONE_GET_PRIVATE (time_zone);
}

/**
 * rtm_time_zone_new:
 *
 * Creates a new instance of this class.
 *
 * Returns: a new #RtmTimeZone object.
 */
RtmTimeZone *
rtm_time_zone_new ()
{
        return g_object_new (RTM_TYPE_TIME_ZONE, NULL);
}

/**
 * rtm_time_zone_get_id:
 * @time_zone: a #RtmTimeZone.
 *
 * Gets the #RtmTimeZone:id property of the object.
 *
 * Returns: the ID of the time_zone.
 */
gchar *
rtm_time_zone_get_id (RtmTimeZone *time_zone)
{
        g_return_val_if_fail (time_zone != NULL, NULL);

        return time_zone->priv->id;
}

/**
 * rtm_time_zone_set_id:
 * @time_zone: a #RtmTimeZone.
 * @id: an ID for the #RtmTimeZone.
 *
 * Sets the #RtmTimeZone:id property of the object.
 *
 * Returns: %TRUE if ID is set.
 */
gboolean
rtm_time_zone_set_id (RtmTimeZone *time_zone, gchar* id)
{
        g_return_val_if_fail (time_zone != NULL, FALSE);
        g_return_val_if_fail (id != NULL, FALSE);

        time_zone->priv->id = g_strdup(id);
        return TRUE;
}

/**
 * rtm_time_zone_get_name:
 * @time_zone: a #RtmTimeZone.
 *
 * Gets the #RtmTimeZone:name property of the object.
 *
 * Returns: the name of the time_zone.
 */
gchar *
rtm_time_zone_get_name (RtmTimeZone *time_zone)
{
        g_return_val_if_fail (time_zone != NULL, NULL);

        return time_zone->priv->name;
}

/**
 * rtm_time_zone_set_name:
 * @time_zone: a #RtmTimeZone.
 * @name: a name for the #RtmTimeZone.
 *
 * Sets the #RtmTimeZone:name property of the object.
 *
 * Returns: %TRUE if name is set.
 */
gboolean
rtm_time_zone_set_name (RtmTimeZone *time_zone, gchar* name)
{
        g_return_val_if_fail (time_zone != NULL, FALSE);
        g_return_val_if_fail (name != NULL, FALSE);

        time_zone->priv->name = g_strdup(name);
        return TRUE;
}

/**
 * rtm_time_zone_is_dst:
 * @time_zone: a #RtmTimeZone.
 *
 * Gets the #RtmTimeZone:dst property of the object.
 *
 * Returns: %TRUE if the time zone is using the Daylight Saving Time.
 */
gboolean
rtm_time_zone_is_dst (RtmTimeZone *time_zone)
{
        g_return_val_if_fail (time_zone != NULL, FALSE);

        return time_zone->priv->dst;
}

/**
 * rtm_time_zone_set_dst:
 * @time_zone: a #RtmTimeZone.
 * @dst: %TRUE to set the Daylight Time Saving for the #RtmTimeZone.
 *
 * Sets the #RtmTimeZone:dst property of the object.
 *
 * Returns: %TRUE if dst is set.
 */
gboolean
rtm_time_zone_set_dst (RtmTimeZone *time_zone, gboolean dst)
{
        g_return_val_if_fail (time_zone != NULL, FALSE);

        time_zone->priv->dst = dst;
        return TRUE;
}

/**
 * rtm_time_zone_get_offset:
 * @time_zone: a #RtmTimeZone.
 *
 * Gets the #RtmTimeZone:offset property of the object.
 *
 * Returns: the offset of the time_zone.
 */
gchar *
rtm_time_zone_get_offset (RtmTimeZone *time_zone)
{
        g_return_val_if_fail (time_zone != NULL, NULL);

        return time_zone->priv->offset;
}

/**
 * rtm_time_zone_set_offset:
 * @time_zone: a #RtmTimeZone.
 * @offset: an offset for the #RtmTimeZone.
 *
 * Sets the #RtmTimeZone:offset property of the object.
 *
 * Returns: %TRUE if offset is set.
 */
gboolean
rtm_time_zone_set_offset (RtmTimeZone *time_zone, gchar* offset)
{
        g_return_val_if_fail (time_zone != NULL, FALSE);
        g_return_val_if_fail (offset != NULL, FALSE);

        time_zone->priv->offset = g_strdup(offset);
        return TRUE;
}

/**
 * rtm_time_zone_get_current_offset:
 * @time_zone: a #RtmTimeZone.
 *
 * Gets the #RtmTimeZone:current_offset property of the object.
 *
 * Returns: the current_offset of the time_zone.
 */
gchar *
rtm_time_zone_get_current_offset (RtmTimeZone *time_zone)
{
        g_return_val_if_fail (time_zone != NULL, NULL);

        return time_zone->priv->current_offset;
}

/**
 * rtm_time_zone_set_current_offset:
 * @time_zone: a #RtmTimeZone.
 * @current_offset: a current_offset for the #RtmTimeZone.
 *
 * Sets the #RtmTimeZone:current_offset property of the object.
 *
 * Returns: %TRUE if current_offset is set.
 */
gboolean
rtm_time_zone_set_current_offset (RtmTimeZone *time_zone, gchar* current_offset)
{
        g_return_val_if_fail (time_zone != NULL, FALSE);
        g_return_val_if_fail (current_offset != NULL, FALSE);

        time_zone->priv->current_offset = g_strdup(current_offset);
        return TRUE;
}

/**
 * rtm_time_zone_load_data:
 * @time_zone: a #RtmTimeZone.
 * @node: a #RestXmlNode with the #RtmTimeZone data.
 *
 * Sets the data of the #RtmTimeZone object from the #RestXmlNode.
 */
void
rtm_time_zone_load_data (RtmTimeZone *time_zone, RestXmlNode *node)
{
        g_return_if_fail (time_zone != NULL);
        g_return_if_fail (node != NULL);

        time_zone->priv->id = g_strdup (rest_xml_node_get_attr (node, "id"));
        time_zone->priv->name = g_strdup (rest_xml_node_get_attr (node, "name"));
        time_zone->priv->dst = (g_strcmp0 (rest_xml_node_get_attr (node, "dst"), "1") == 0);
        time_zone->priv->offset = g_strdup (rest_xml_node_get_attr (node, "offset"));
        time_zone->priv->current_offset = g_strdup (rest_xml_node_get_attr (node, "current_offset"));
}

/**
 * rtm_time_zone_to_string:
 * @time_zone: a #RtmTimeZone.
 *
 * Generates a printable string which represents a #RtmTimeZone object.
 *
 * Returns: A string representing the #RtmTimeZone.
 */
gchar *
rtm_time_zone_to_string (RtmTimeZone *time_zone)
{
        g_return_val_if_fail (time_zone != NULL, NULL);

        gchar *string;

        string = g_strconcat (
                "RtmTimeZone: [\n",
                "  ID: ", rtm_util_string_or_null (time_zone->priv->id), "\n",
                "  Name: ", rtm_util_string_or_null (time_zone->priv->name), "\n",
                "  Daylight Time Saving: ", rtm_util_gboolean_to_string (time_zone->priv->dst), "\n",
                "  Offset: ", rtm_util_string_or_null (time_zone->priv->offset), "\n",
                "  Current offset: ", rtm_util_string_or_null (time_zone->priv->current_offset), "\n",
                "]\n",
                NULL);

        return string;
}
