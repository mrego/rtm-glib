/*
 * rtm-util.h: Common util functions
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

#include <glib.h>


#define DEBUG_PRINT(format, ...) g_debug ("%s (%s) " format, G_STRLOC, G_STRFUNC, ##__VA_ARGS__)


gchar *
rtm_util_string_or_null (gchar *string);

gchar *
rtm_util_gboolean_to_string (gboolean boolean);

GTimeVal *
rtm_util_g_time_val_dup (GTimeVal *time_val);

gchar *
rtm_util_g_time_val_to_string (GTimeVal *time_val);
