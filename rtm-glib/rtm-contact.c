/*
 * rtm-contact.c: Represents a contact
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
 * SECTION:rtm-contact
 * @short_description: A class that represents a contact
 *
 * #RtmContact represents a contact from Remeber The Milk.
 */

#include <rtm-contact.h>
#include <rtm-util.h>
#include <rtm-error.h>

#define RTM_CONTACT_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE (        \
                                              (obj), RTM_TYPE_CONTACT, RtmContactPrivate))

struct _RtmContactPrivate {
        gchar *id;
        gchar *username;
        gchar *fullname;
};

enum {
        PROP_0,

        PROP_ID,
        PROP_USERNAME,
        PROP_FULLNAME,
};

G_DEFINE_TYPE (RtmContact, rtm_contact, G_TYPE_OBJECT);

static void
rtm_contact_get_property (GObject *gobject, guint prop_id, GValue *value,
                          GParamSpec *pspec)
{
        RtmContactPrivate *priv = RTM_CONTACT_GET_PRIVATE (RTM_CONTACT (gobject));

        switch (prop_id) {
        case PROP_ID:
                g_value_set_string (value, priv->id);
                break;

        case PROP_USERNAME:
                g_value_set_string (value, priv->username);
                break;

        case PROP_FULLNAME:
                g_value_set_string (value, priv->fullname);
                break;

        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id,
                                                   pspec);
                break;
        }
}

static void
rtm_contact_set_property (GObject *gobject, guint prop_id, const GValue *value,
                          GParamSpec *pspec)
{
        RtmContactPrivate *priv = RTM_CONTACT_GET_PRIVATE (RTM_CONTACT (gobject));

        switch (prop_id) {
        case PROP_ID:
                if (priv->id) {
                        g_free (priv->id);
                }
                priv->id = g_value_dup_string (value);
                break;

        case PROP_USERNAME:
                if (priv->username) {
                        g_free (priv->username);
                }
                priv->username = g_value_dup_string (value);
                break;

        case PROP_FULLNAME:
                if (priv->fullname) {
                        g_free (priv->fullname);
                }
                priv->fullname = g_value_dup_string (value);
                break;

        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id,
                                                   pspec);
                break;
        }
}

static void
rtm_contact_finalize (GObject *gobject)
{
        RtmContactPrivate *priv = RTM_CONTACT_GET_PRIVATE (RTM_CONTACT (gobject));

        if (priv->id) {
                g_free (priv->id);
        }
        if (priv->username) {
                g_free (priv->username);
        }
        if (priv->fullname) {
                g_free (priv->fullname);
        }

        G_OBJECT_CLASS (rtm_contact_parent_class)->finalize (gobject);
}

static void
rtm_contact_class_init (RtmContactClass *klass)
{
        GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

        g_type_class_add_private (klass, sizeof (RtmContactPrivate));

        gobject_class->get_property = rtm_contact_get_property;
        gobject_class->set_property = rtm_contact_set_property;
        gobject_class->finalize = rtm_contact_finalize;

        g_object_class_install_property (
                gobject_class,
                PROP_ID,
                g_param_spec_string (
                        "id",
                        "ID",
                        "The identifier of the contact",
                        NULL,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_USERNAME,
                g_param_spec_string (
                        "username",
                        "Username",
                        "The username of the contact",
                        NULL,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_FULLNAME,
                g_param_spec_string (
                        "fullname",
                        "Fullname",
                        "The fullname of the contact",
                        NULL,
                        G_PARAM_READWRITE));

}

static void
rtm_contact_init (RtmContact *contact)
{
        contact->priv = RTM_CONTACT_GET_PRIVATE (contact);

        contact->priv->id = NULL;
        contact->priv->username = NULL;
        contact->priv->fullname = NULL;
}

/**
 * rtm_contact_new:
 *
 * Creates a new instance of this class.
 *
 * Returns: a new #RtmContact object.
 */
RtmContact *
rtm_contact_new ()
{
        return g_object_new (RTM_TYPE_CONTACT, NULL);
}

/**
 * rtm_contact_get_id:
 * @contact: a #RtmContact.
 *
 * Gets the #RtmContact:id property of the object.
 *
 * Returns: the ID of the contact.
 */
gchar *
rtm_contact_get_id (RtmContact *contact)
{
        g_return_val_if_fail (contact != NULL, NULL);

        return contact->priv->id;
}

/**
 * rtm_contact_set_id:
 * @contact: a #RtmContact.
 * @id: an ID for the #RtmContact.
 *
 * Sets the #RtmContact:id property of the object.
 *
 * Returns: %TRUE if ID is set.
 */
gboolean
rtm_contact_set_id (RtmContact *contact, const gchar* id)
{
        g_return_val_if_fail (contact != NULL, FALSE);
        g_return_val_if_fail (id != NULL, FALSE);

        g_object_set (contact, "id", id, NULL);

        return TRUE;
}

/**
 * rtm_contact_get_username:
 * @contact: a #RtmContact.
 *
 * Gets the #RtmContact:username property of the object.
 *
 * Returns: the username of the contact.
 */
gchar *
rtm_contact_get_username (RtmContact *contact)
{
        g_return_val_if_fail (contact != NULL, NULL);

        return contact->priv->username;
}

/**
 * rtm_contact_set_username:
 * @contact: a #RtmContact.
 * @username: a username for the #RtmContact.
 *
 * Sets the #RtmContact:username property of the object.
 *
 * Returns: %TRUE if username is set.
 */
gboolean
rtm_contact_set_username (RtmContact *contact, const gchar* username)
{
        g_return_val_if_fail (contact != NULL, FALSE);
        g_return_val_if_fail (username != NULL, FALSE);

        g_object_set (contact, "username", username, NULL);

        return TRUE;
}

/**
 * rtm_contact_get_fullname:
 * @contact: a #RtmContact.
 *
 * Gets the #RtmContact:fullname property of the object.
 *
 * Returns: the fullname of the contact.
 */
gchar *
rtm_contact_get_fullname (RtmContact *contact)
{
        g_return_val_if_fail (contact != NULL, NULL);

        return contact->priv->fullname;
}

/**
 * rtm_contact_set_fullname:
 * @contact: a #RtmContact.
 * @fullname: a fullname for the #RtmContact.
 *
 * Sets the #RtmContact:fullname property of the object.
 *
 * Returns: %TRUE if fullname is set.
 */
gboolean
rtm_contact_set_fullname (RtmContact *contact, const gchar* fullname)
{
        g_return_val_if_fail (contact != NULL, FALSE);
        g_return_val_if_fail (fullname != NULL, FALSE);

        g_object_set (contact, "fullname", fullname, NULL);

        return TRUE;
}

/**
 * rtm_contact_load_data:
 * @contact: a #RtmContact.
 * @node: a #RestXmlNode with the #RtmContact data.
 * @list_id: the list ID which belongs the #RtmContact.
 *
 * Sets the data of the #RtmContact object from the #RestXmlNode.
 */
void
rtm_contact_load_data (RtmContact *contact, RestXmlNode *node)
{
        g_return_if_fail (contact != NULL);
        g_return_if_fail (node != NULL);

        rtm_contact_set_id (contact, rest_xml_node_get_attr (node, "id"));
        rtm_contact_set_username (contact, rest_xml_node_get_attr (node, "username"));
        rtm_contact_set_fullname (contact, rest_xml_node_get_attr (node, "fullname"));
}

/**
 * rtm_contact_to_string:
 * @contact: a #RtmContact.
 *
 * Generates a printable string which represents a #RtmContact object.
 *
 * Returns: A string representing the #RtmContact.
 */
gchar *
rtm_contact_to_string (RtmContact *contact)
{
        g_return_val_if_fail (contact != NULL, NULL);

        gchar *string = g_strconcat (
                "RtmContact: [\n",
                "  ID: ", rtm_util_string_or_null (contact->priv->id), "\n",
                "  Username: ", rtm_util_string_or_null (contact->priv->username), "\n",
                "  Fullname: ", rtm_util_string_or_null (contact->priv->fullname), "\n",
                "]\n",
                NULL);

        return string;
}
