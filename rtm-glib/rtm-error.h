/*
 * rtm-error.h: Library errors
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


#ifndef RTM_ERROR_H
#define RTM_ERROR_H

#include <glib.h>
#include <glib-object.h>

#define RTM_ERROR_DOMAIN (rtm_get_error_quark())

/**
 * _RtmError:
 *
 * A #GError error code.
 */
enum _RtmError
{
        RTM_UNKNOWN_ERROR,
        RTM_ERROR_RESPONSE_FAIL,
        RTM_TASK_ALREADY_EXISTS,
        RTM_TASK_NOT_FOUND,
        RTM_TAG_ALREADY_ASSIGNED,
        RTM_TAG_NOT_FOUND,
};

typedef enum _RtmError RtmError;

const gchar*
rtm_error_get_message (GError *err);

gint
rtm_error_get_code (GError *err);

GQuark
rtm_get_error_quark (void);


#endif
