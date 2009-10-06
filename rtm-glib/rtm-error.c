/*
 * rtm-error.c: Library errors
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

#include <rtm-error.h>

/**
 * rtm_error_get_message:
 * @err: a #GError.
 *
 * Get the error message.
 *
 * Returns: An error message.
 */
const gchar*
rtm_error_get_message (GError *err)
{
        return err->message;
}

/**
 * rtm_error_get_code:
 * @err: a #GError.
 *
 * Get the error's code
 *
 * Returns: An error code.
 */
gint
rtm_error_get_code (GError *err)
{
        return err->code;
}

/**
 * rtm_get_error_quark:
 *
 * The implementation of #RTM_ERROR_DOMAIN error domain. See documentation
 * for #GError in GLib reference manual.
 *
 * Returns: The error domain quark for use with #GError.
 */
GQuark
rtm_get_error_quark (void)
{
        static GQuark quark = 0;

        if (quark == 0)
        {
                quark = g_quark_from_static_string ("rtm-error-quark");
        }

        return quark;
}
