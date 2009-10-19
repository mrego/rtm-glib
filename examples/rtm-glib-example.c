/*
 * rtm-glib-example.c: Example
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

#include <stdio.h>
#include <rtm-glib/rtm-glib.h>
#include <rtm-glib/rtm-task.h>
#include <rtm-glib/rtm-error.h>
#include <rtm-glib/rtm-list.h>


#define RTM_API_KEY "de4eacaea750baff02b70e6ec57fcf22"
#define RTM_SHARED_SECRET "6aedd71e2f4f9c94"


gint
main (gint argc, gchar **argv)
{
        RtmGlib *rtm;
        gchar *frob;
        gchar *url;
        gchar *auth_token;
        gchar *username;
        GList *glist;
        GList *item;
        RtmTask *task;
        GError *error = NULL;
        RtmList *rtm_list;
        gchar *timeline;
        gchar *transaction_id;

        g_thread_init (NULL);
        g_type_init();

        rtm = rtm_glib_new (RTM_API_KEY, RTM_SHARED_SECRET);

        if (rtm_glib_test_echo (rtm, &error)) {
                g_print ("Test echo OK!\n");
        } else {
                g_print ("Test echo FAIL!\n");
        }
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }

        frob = rtm_glib_auth_get_frob (rtm, &error);
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }
        g_print ("Frob: %s\n", frob);

        url = rtm_glib_auth_get_login_url (rtm, frob);
        g_print ("URL: %s\n", url);

        getchar ();

        auth_token = rtm_glib_auth_get_token (rtm, frob, &error);
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }
        g_print ("auth_token: %s\n", auth_token);

        if (rtm_glib_auth_check_token (rtm, auth_token, NULL)) {
                g_print ("auth_token valid!\n");
        } else {
                g_print ("auth_token not valid!\n");
        }
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }

        username = rtm_glib_test_login (rtm, auth_token, &error);

        g_free (auth_token);

        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }
        g_print ("username: %s\n", username);

        glist = rtm_glib_tasks_get_list (rtm, NULL, NULL, NULL, &error);
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }
        for (item = glist; item; item = g_list_next (item)) {
                task = (RtmTask *) item->data;
                g_print ("%s", rtm_task_to_string (task));
        }
        g_list_free (glist);

        glist = rtm_glib_lists_get_list (rtm, &error);
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }
        for (item = glist; item; item = g_list_next (item)) {
                rtm_list = (RtmList *) item->data;
                g_print ("%s", rtm_list_to_string (rtm_list));
        }
        g_list_free (glist);

        timeline = rtm_glib_timelines_create (rtm, &error);
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }
        g_print ("timeline: %s", timeline);

        task = rtm_glib_tasks_add (rtm, timeline, "test-rtm-glib", NULL, FALSE, &error);
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }
        if (task != NULL) {
                g_print ("First task added! task_id: %s\n", rtm_task_get_id (task));
        } else {
                g_print ("First task NOT added!\n");
        }

        transaction_id = rtm_glib_tasks_delete (rtm, timeline, task, &error);
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }
        if (transaction_id != NULL) {
                g_print ("Task deleted! transaction_id: %s\n", transaction_id);
        } else {
                g_print ("Task NOT deleted!\n");
        }

        if (rtm_glib_transactions_undo (rtm, timeline, transaction_id, &error)) {
                g_print ("Undo!\n");
        } else {
                g_print ("NOT undo!\n");
        }
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }

        transaction_id = rtm_glib_tasks_set_name (rtm, timeline, task, "test-rtm-glib-renamed", &error);
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }
        if (transaction_id != NULL) {
                g_print ("Task renamed! transaction_id: %s\n", transaction_id);
        } else {
                g_print ("Task NOT renamed!\n");
        }

        transaction_id = rtm_glib_tasks_set_url (rtm, timeline, task, "http://http://gitorious.org/rtm-glib/", &error);
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }
        if (transaction_id != NULL) {
                g_print ("Task URL set! transaction_id: %s\n", transaction_id);
        } else {
                g_print ("Task URL NOT set!\n");
        }

        transaction_id = rtm_glib_tasks_set_location (rtm, timeline, task, "111222", &error);
        if (error != NULL) {
                g_warning ("%s", rtm_error_get_message (error));
                error = NULL;
        }
        if (transaction_id != NULL) {
                g_print ("Task location set! transaction_id: %s\n", transaction_id);
        } else {
                g_print ("Task location NOT set!\n");
        }

        transaction_id = rtm_glib_tasks_set_tags (rtm, timeline, task, "rtm,glib", &error);
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }
        if (transaction_id != NULL) {
                g_print ("Task tags set! transaction_id: %s\n", transaction_id);
        } else {
                g_print ("Task tags NOT set!\n");
        }

        transaction_id = rtm_glib_tasks_add_tags (rtm, timeline, task, "gnome,library", &error);
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }
        if (transaction_id != NULL) {
                g_print ("Task tags added! transaction_id: %s\n", transaction_id);
        } else {
                g_print ("Task tags NOT added!\n");
        }

        transaction_id = rtm_glib_tasks_remove_tags (rtm, timeline, task, "glib,gnome", &error);
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }
        if (transaction_id != NULL) {
                g_print ("Task tags removed! transaction_id: %s\n", transaction_id);
        } else {
                g_print ("Task tags NOT removed!\n");
        }

        task = rtm_glib_tasks_add (rtm, timeline, "test-rtm-glib2", NULL, FALSE, &error);
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }
        if (task != NULL) {
                g_print ("Second task added! task_id: %s\n", rtm_task_get_id (task));
        } else {
                g_print ("Second task NOT added!\n");
        }

        transaction_id = rtm_glib_tasks_delete (rtm, timeline, task, &error);
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }
        if (transaction_id != NULL) {
                g_print ("Task deleted! transaction_id: %s\n", transaction_id);
        } else {
                g_print ("Task NOT deleted!\n");
        }

        rtm_list = rtm_glib_lists_add (rtm, timeline, "test-list-glib", &error);
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }
        if (task != NULL) {
                g_print ("List added! list_id: %s\n", rtm_list_get_id (rtm_list));
        } else {
                g_print ("List NOT added!\n");
        }

        transaction_id = rtm_glib_lists_set_name (rtm, timeline, rtm_list, "test-rtm-glib-renamed", &error);
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }
        if (transaction_id != NULL) {
                g_print ("List renamed! transaction_id: %s\n", transaction_id);
        } else {
                g_print ("List NOT renamed!\n");
        }

        transaction_id = rtm_glib_lists_archive (rtm, timeline, rtm_list, &error);
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }
        if (transaction_id != NULL) {
                g_print ("List archived! transaction_id: %s\n", transaction_id);
        } else {
                g_print ("List NOT archived!\n");
        }

        transaction_id = rtm_glib_lists_unarchive (rtm, timeline, rtm_list, &error);
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }
        if (transaction_id != NULL) {
                g_print ("List unarchived! transaction_id: %s\n", transaction_id);
        } else {
                g_print ("List NOT unarchived!\n");
        }

        if (rtm_glib_lists_set_default (rtm, timeline, rtm_list, &error)) {
                g_print ("List \"%s\" set as default!\n", rtm_list_get_id (rtm_list));
        } else {
                g_print ("List \"%s\" NOT set as default!\n", rtm_list_get_id (rtm_list));
        }
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }

        transaction_id = rtm_glib_lists_delete (rtm, timeline, rtm_list, &error);
        if (error != NULL) {
                g_error ("%s", rtm_error_get_message (error));
        }
        if (transaction_id != NULL) {
                g_print ("List deleted! transaction_id: %s\n", transaction_id);
        } else {
                g_print ("List NOT deleted!\n");
        }

        g_free (frob);
        g_free (url);
        g_free (username);
        g_free (timeline);
        g_free (transaction_id);

        g_object_unref (task);
        g_object_unref (rtm_list);
        g_object_unref (rtm);

        return 0;
}
