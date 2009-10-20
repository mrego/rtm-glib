/*
 * rtm-location.c: Represents a location for a task
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
 * SECTION:rtm-location
 * @short_description: A class that represents a location of tasks
 *
 * #RtmLocation represents a location of tasks from Remeber The Milk.
 */

#include <rtm-location.h>
#include <rtm-util.h>

#define RTM_LOCATION_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE (        \
                                           (obj), RTM_TYPE_LOCATION, RtmLocationPrivate))

struct _RtmLocationPrivate {
        gchar *id;
        gchar *name;
        gchar *longitude;
        gchar *latitude;
        gchar *zoom;
        gchar *address;
        gboolean viewable;
};

enum {
        PROP_0,

        PROP_ID,
        PROP_NAME,
        PROP_LONGITUDE,
        PROP_LATITUDE,
        PROP_ZOOM,
        PROP_ADDRESS,
        PROP_VIEWABLE,
};

G_DEFINE_TYPE (RtmLocation, rtm_location, G_TYPE_OBJECT);

static void
rtm_location_get_property (GObject *gobject, guint prop_id, GValue *value,
                           GParamSpec *pspec)
{
        RtmLocationPrivate *priv = RTM_LOCATION_GET_PRIVATE (RTM_LOCATION (gobject));

        switch (prop_id) {
        case PROP_ID:
                g_value_set_string (value, priv->id);
                break;

        case PROP_NAME:
                g_value_set_string (value, priv->name);
                break;

        case PROP_LONGITUDE:
                g_value_set_string (value, priv->longitude);
                break;

        case PROP_LATITUDE:
                g_value_set_string (value, priv->latitude);
                break;

        case PROP_ZOOM:
                g_value_set_string (value, priv->zoom);
                break;

        case PROP_ADDRESS:
                g_value_set_string (value, priv->address);
                break;

        case PROP_VIEWABLE:
                g_value_set_boolean (value, priv->viewable);

        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id,
                                                   pspec);
                break;
        }
}

static void
rtm_location_set_property (GObject *gobject, guint prop_id, const GValue *value,
                           GParamSpec *pspec)
{
        RtmLocationPrivate *priv = RTM_LOCATION_GET_PRIVATE (RTM_LOCATION (gobject));

        switch (prop_id) {
        case PROP_ID:
                g_free (priv->id);
                priv->id = g_value_dup_string (value);
                break;

        case PROP_NAME:
                g_free (priv->name);
                priv->name = g_value_dup_string (value);
                break;

        case PROP_LONGITUDE:
                g_free (priv->longitude);
                priv->longitude = g_value_dup_string (value);
                break;

        case PROP_LATITUDE:
                g_free (priv->latitude);
                priv->latitude = g_value_dup_string (value);
                break;

        case PROP_ZOOM:
                g_free (priv->zoom);
                priv->zoom = g_value_dup_string (value);
                break;

        case PROP_ADDRESS:
                g_free (priv->address);
                priv->address = g_value_dup_string (value);
                break;

        case PROP_VIEWABLE:
                priv->viewable = g_value_get_boolean (value);
                break;

        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id,
                                                   pspec);
                break;
        }
}

static void
rtm_location_finalize (GObject *gobject)
{
        RtmLocationPrivate *priv = RTM_LOCATION_GET_PRIVATE (RTM_LOCATION (gobject));

        g_free (priv->id);
        g_free (priv->name);
        g_free (priv->longitude);
        g_free (priv->latitude);
        g_free (priv->zoom);
        g_free (priv->address);

        G_OBJECT_CLASS (rtm_location_parent_class)->finalize (gobject);
}

static void
rtm_location_class_init (RtmLocationClass *klass)
{
        GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

        g_type_class_add_private (klass, sizeof (RtmLocationPrivate));

        gobject_class->get_property = rtm_location_get_property;
        gobject_class->set_property = rtm_location_set_property;
        gobject_class->finalize = rtm_location_finalize;

        g_object_class_install_property (
                gobject_class,
                PROP_ID,
                g_param_spec_string (
                        "id",
                        "ID",
                        "The identifier of the location",
                        NULL,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_NAME,
                g_param_spec_string (
                        "name",
                        "Name",
                        "The name of the location",
                        NULL,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_LONGITUDE,
                g_param_spec_string (
                        "longitude",
                        "Longitude",
                        "The longitude of the location",
                        NULL,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_LATITUDE,
                g_param_spec_string (
                        "latitude",
                        "Latitude",
                        "The latitude of the location",
                        NULL,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_ZOOM,
                g_param_spec_string (
                        "zoom",
                        "Zoom",
                        "The zoom of the location",
                        NULL,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_ADDRESS,
                g_param_spec_string (
                        "address",
                        "Address",
                        "The address of the location",
                        NULL,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_VIEWABLE,
                g_param_spec_boolean (
                        "viewable",
                        "Viewable",
                        "If the location is viewable or not",
                        FALSE,
                        G_PARAM_READWRITE));

}

static void
rtm_location_init (RtmLocation *location)
{
        location->priv = RTM_LOCATION_GET_PRIVATE (location);
}

/**
 * rtm_location_new:
 *
 * Creates a new instance of this class.
 *
 * Returns: a new #RtmLocation object.
 */
RtmLocation *
rtm_location_new ()
{
        return g_object_new (RTM_TYPE_LOCATION, NULL);
}

/**
 * rtm_location_get_id:
 * @location: a #RtmLocation.
 *
 * Gets the #RtmLocation:id property of the object.
 *
 * Returns: the ID of the location.
 */
gchar *
rtm_location_get_id (RtmLocation *location)
{
        g_return_val_if_fail (location != NULL, NULL);

        return location->priv->id;
}

/**
 * rtm_location_set_id:
 * @location: a #RtmLocation.
 * @id: an ID for the #RtmLocation.
 *
 * Sets the #RtmLocation:id property of the object.
 *
 * Returns: %TRUE if ID is set.
 */
gboolean
rtm_location_set_id (RtmLocation *location, gchar* id)
{
        g_return_val_if_fail (location != NULL, FALSE);
        g_return_val_if_fail (id != NULL, FALSE);

        location->priv->id = g_strdup(id);
        return TRUE;
}

/**
 * rtm_location_get_name:
 * @location: a #RtmLocation.
 *
 * Gets the #RtmLocation:name property of the object.
 *
 * Returns: the name of the location.
 */
gchar *
rtm_location_get_name (RtmLocation *location)
{
        g_return_val_if_fail (location != NULL, NULL);

        return location->priv->name;
}

/**
 * rtm_location_set_name:
 * @location: a #RtmLocation.
 * @name: a name for the #RtmLocation.
 *
 * Sets the #RtmLocation:name property of the object.
 *
 * Returns: %TRUE if name is set.
 */
gboolean
rtm_location_set_name (RtmLocation *location, gchar* name)
{
        g_return_val_if_fail (location != NULL, FALSE);
        g_return_val_if_fail (name != NULL, FALSE);

        location->priv->name = g_strdup(name);
        return TRUE;
}

/**
 * rtm_location_get_longitude:
 * @location: a #RtmLocation.
 *
 * Gets the #RtmLocation:longitude property of the object.
 *
 * Returns: the longitude of the location.
 */
gchar *
rtm_location_get_longitude (RtmLocation *location)
{
        g_return_val_if_fail (location != NULL, NULL);

        return location->priv->longitude;
}

/**
 * rtm_location_set_longitude:
 * @location: a #RtmLocation.
 * @longitude: a longitude for the #RtmLocation.
 *
 * Sets the #RtmLocation:longitude property of the object.
 *
 * Returns: %TRUE if longitude is set.
 */
gboolean
rtm_location_set_longitude (RtmLocation *location, gchar* longitude)
{
        g_return_val_if_fail (location != NULL, FALSE);
        g_return_val_if_fail (longitude != NULL, FALSE);

        location->priv->longitude = g_strdup(longitude);
        return TRUE;
}

/**
 * rtm_location_get_latitude:
 * @location: a #RtmLocation.
 *
 * Gets the #RtmLocation:latitude property of the object.
 *
 * Returns: the latitude of the location.
 */
gchar *
rtm_location_get_latitude (RtmLocation *location)
{
        g_return_val_if_fail (location != NULL, NULL);

        return location->priv->latitude;
}

/**
 * rtm_location_set_latitude:
 * @location: a #RtmLocation.
 * @latitude: a latitude for the #RtmLocation.
 *
 * Sets the #RtmLocation:latitude property of the object.
 *
 * Returns: %TRUE if latitude is set.
 */
gboolean
rtm_location_set_latitude (RtmLocation *location, gchar* latitude)
{
        g_return_val_if_fail (location != NULL, FALSE);
        g_return_val_if_fail (latitude != NULL, FALSE);

        location->priv->latitude = g_strdup(latitude);
        return TRUE;
}

/**
 * rtm_location_get_zoom:
 * @location: a #RtmLocation.
 *
 * Gets the #RtmLocation:zoom property of the object.
 *
 * Returns: the zoom of the location.
 */
gchar *
rtm_location_get_zoom (RtmLocation *location)
{
        g_return_val_if_fail (location != NULL, NULL);

        return location->priv->zoom;
}

/**
 * rtm_location_set_zoom:
 * @location: a #RtmLocation.
 * @zoom: a zoom for the #RtmLocation.
 *
 * Sets the #RtmLocation:zoom property of the object.
 *
 * Returns: %TRUE if zoom is set.
 */
gboolean
rtm_location_set_zoom (RtmLocation *location, gchar* zoom)
{
        g_return_val_if_fail (location != NULL, FALSE);
        g_return_val_if_fail (zoom != NULL, FALSE);

        location->priv->zoom = g_strdup(zoom);
        return TRUE;
}

/**
 * rtm_location_get_address:
 * @location: a #RtmLocation.
 *
 * Gets the #RtmLocation:address property of the object.
 *
 * Returns: the address of the location.
 */
gchar *
rtm_location_get_address (RtmLocation *location)
{
        g_return_val_if_fail (location != NULL, NULL);

        return location->priv->address;
}

/**
 * rtm_location_set_address:
 * @location: a #RtmLocation.
 * @address: a address for the #RtmLocation.
 *
 * Sets the #RtmLocation:address property of the object.
 *
 * Returns: %TRUE if address is set.
 */
gboolean
rtm_location_set_address (RtmLocation *location, gchar* address)
{
        g_return_val_if_fail (location != NULL, FALSE);
        g_return_val_if_fail (address != NULL, FALSE);

        location->priv->address = g_strdup(address);
        return TRUE;
}

/**
 * rtm_location_is_viewable:
 * @location: a #RtmLocation.
 *
 * Checks the #RtmLocation:viewable property of the object.
 *
 * Returns: %TRUE if the location is viewable.
 */
gboolean
rtm_location_is_viewable (RtmLocation *location)
{
        g_return_val_if_fail (location != NULL, NULL);

        return location->priv->viewable;
}

/**
 * rtm_location_set_viewable:
 * @location: a #RtmLocation.
 * @viewable: a viewable for the #RtmLocation.
 *
 * Sets the #RtmLocation:viewable property of the object.
 *
 * Returns: %TRUE if viewable is set.
 */
gboolean
rtm_location_set_viewable (RtmLocation *location, gboolean viewable)
{
        g_return_val_if_fail (location != NULL, FALSE);

        location->priv->viewable = viewable;
        return TRUE;
}

/**
 * rtm_location_load_data:
 * @location: a #RtmLocation.
 * @node: a #RestXmlNode with the #RtmLocation data.
 *
 * Sets the data of the #RtmLocation object from the #RestXmlNode.
 */
void
rtm_location_load_data (RtmLocation *location, RestXmlNode *node)
{
        g_return_if_fail (location != NULL);
        g_return_if_fail (node != NULL);

        gchar *viewable;

        location->priv->id = g_strdup (rest_xml_node_get_attr (node, "id"));
        location->priv->name = g_strdup (rest_xml_node_get_attr (node, "name"));
        location->priv->longitude = g_strdup (rest_xml_node_get_attr (node, "longitude"));
        location->priv->latitude = g_strdup (rest_xml_node_get_attr (node, "latitude"));
        location->priv->zoom = g_strdup (rest_xml_node_get_attr (node, "zoom"));
        location->priv->address = g_strdup (rest_xml_node_get_attr (node, "address"));
        location->priv->viewable = (g_strcmp0 (rest_xml_node_get_attr (node, "viewable"), "1") == 0);
}

/**
 * rtm_location_to_string:
 * @location: a #RtmLocation.
 *
 * Generates a printable string which represents a #RtmLocation object.
 *
 * Returns: A string representing the #RtmLocation.
 */
gchar *
rtm_location_to_string (RtmLocation *location)
{
        g_return_val_if_fail (location != NULL, NULL);

        gchar *string;

        string = g_strconcat (
                "RtmLocation: [\n",
                "  ID: ", rtm_util_string_or_null (location->priv->id), "\n",
                "  Name: ", rtm_util_string_or_null (location->priv->name), "\n",
                "  Longitude: ", rtm_util_string_or_null (location->priv->longitude), "\n",
                "  Latitude: ", rtm_util_string_or_null (location->priv->latitude), "\n",
                "  Zoom: ", rtm_util_string_or_null (location->priv->zoom), "\n",
                "  Address: ", rtm_util_string_or_null (location->priv->address), "\n",
                "  Viewable: ", rtm_util_gboolean_to_string (location->priv->viewable), "\n",
                "]\n",
                NULL);

        return string;
}
