/*
 * rtm-util.c: Common util functions
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
 * SECTION:rtm-util
 * @short_description: Common util functions
 */

#include "rtm-util.h"

/**
 * rtm_util_string_or_null:
 * @string: a string.
 *
 * Returns the same string if it is not %NULL, otherwise returns the text
 * "NULL".
 *
 * Returns: The string or the text "NULL".
 */
gchar *
rtm_util_string_or_null (gchar *string)
{
	if (string) {
		return string;
	}

	return "NULL";
}

/**
 * rtm_util_gboolean_to_string:
 * @boolean: a boolean.
 *
 * Returns the strings "TRUE" or "FALSE" depending on the parameter.
 *
 * Returns: The string representing a boolean value.
 */
gchar *
rtm_util_gboolean_to_string (gboolean boolean)
{
        if (boolean) {
                return "TRUE";
        }

        return "FALSE";
}

/**
 * rtm_util_g_time_val_dup:
 * @time_val: a #GTimeVal pointer.
 *
 * Returns pointer to a copy of the #GTimeVal.
 *
 * Returns: A copy of the #GTimeVal.
 */
GTimeVal *
rtm_util_g_time_val_dup (GTimeVal *time_val)
{
        g_return_val_if_fail (time_val != NULL, NULL);

        GTimeVal *new_time_val = g_new0 (GTimeVal, 1);
        new_time_val->tv_sec = time_val->tv_sec;
        new_time_val->tv_usec = time_val->tv_usec;

        return new_time_val;
}

/**
 * rtm_util_g_time_val_to_string:
 * @time_val: a #GTimeVal.
 *
 * Returns an string representing the #GTimeVal if it is not %NULL, otherwise
 * returns "NULL".
 *
 * Returns: The string represeting the #GTimeVal or "NULL".
 */
gchar *
rtm_util_g_time_val_to_string (GTimeVal *time_val)
{
        if (time_val) {
                return g_time_val_to_iso8601 (time_val);
        } else {
                return "NULL";
        }
}
