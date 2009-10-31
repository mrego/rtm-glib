/*
 * rtm-contact.h: Represents a contact
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

#ifndef __RTM_CONTACT_H__
#define __RTM_CONTACT_H__

#include <glib-object.h>
#include <rest/rest-xml-parser.h>

G_BEGIN_DECLS

#define RTM_TYPE_CONTACT (rtm_contact_get_type ())
#define RTM_CONTACT(obj)                                                   \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), RTM_TYPE_CONTACT, RtmContact))
#define RTM_IS_CONTACT(obj)                                        \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), RTM_TYPE_CONTACT))
#define RTM_CONTACT_CLASS(klass)                                           \
        (G_TYPE_CHECK_CLASS_CAST ((klass), RTM_TYPE_CONTACT, RtmContactClass))
#define RTM_IS_CONTACT_CLASS(klass)                                \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), RTM_TYPE_CONTACT))
#define RTM_CONTACT_GET_CLASS(obj)                                         \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), RTM_TYPE_CONTACT, RtmContactClass))

typedef struct _RtmContact RtmContact;
typedef struct _RtmContactClass RtmContactClass;
typedef struct _RtmContactPrivate RtmContactPrivate;

struct _RtmContact {
        GObject parent_instance;

        /*< private >*/
        RtmContactPrivate *priv;
};

struct _RtmContactClass {
        GObjectClass parent_class;
};

GType
rtm_contact_get_type (void) G_GNUC_CONST;

RtmContact *
rtm_contact_new (void);

gchar *
rtm_contact_get_id (RtmContact *contact);

gboolean
rtm_contact_set_id (RtmContact *contact, const gchar* id);

gchar *
rtm_contact_get_username (RtmContact *contact);

gboolean
rtm_contact_set_username (RtmContact *contact, const gchar* username);

gchar *
rtm_contact_get_fullname (RtmContact *contact);

gboolean
rtm_contact_set_fullname (RtmContact *contact, const gchar* fullname);

void
rtm_contact_load_data (RtmContact *contact, RestXmlNode *node);

gchar *
rtm_contact_to_string (RtmContact *contact);

#endif /* __RTM_CONTACT_H__ */
