/*
 * check-rtm-task.c: Test RtmTask class
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

#include <stdlib.h>
#include <check.h>
#include <rtm-glib/rtm-task.h>
#include <rest/rest-xml-parser.h>

RtmTask * task;

void
setup (void)
{
        g_type_init();

        task = rtm_task_new ();
}

void
teardown (void)
{
        g_object_unref (task);
}

START_TEST (test_id)
{
        rtm_task_set_id (task, "123456");
        fail_unless (g_strcmp0 (rtm_task_get_id (task), "123456") == 0,
                     "Task ID not set properly");
}
END_TEST

START_TEST (test_taskseries_id)
{
        rtm_task_set_taskseries_id (task, "987654");
        fail_unless (g_strcmp0 (rtm_task_get_taskseries_id (task), "987654") == 0,
                     "Taskseries ID not set properly");
}
END_TEST

START_TEST (test_list_id)
{
        rtm_task_set_list_id (task, "102030");
        fail_unless (g_strcmp0 (rtm_task_get_list_id (task), "102030") == 0,
                     "List ID not set properly");
}
END_TEST

START_TEST (test_name)
{
        rtm_task_set_name (task, "test");
        fail_unless (g_strcmp0 (rtm_task_get_name (task), "test") == 0,
                     "Task name not set properly");
}
END_TEST

START_TEST (test_priority)
{
        rtm_task_set_priority (task, "2");
        fail_unless (g_strcmp0 (rtm_task_get_priority (task), "2") == 0,
                     "Task priority not set properly");
}
END_TEST

START_TEST (test_url)
{
        rtm_task_set_url (task, "http://gitorious.org/rtm-glib/");
        fail_unless (g_strcmp0 (rtm_task_get_url (task),
                                "http://gitorious.org/rtm-glib/") == 0,
                     "Task URL not set properly");
}
END_TEST

START_TEST (test_location_id)
{
        rtm_task_set_location_id (task, "111222");
        fail_unless (g_strcmp0 (rtm_task_get_location_id (task),
                                "111222") == 0,
                     "Task location ID not set properly");
}
END_TEST

START_TEST (test_due_date)
{
        gchar * due = "2006-05-07T10:19:54Z";
        GTimeVal due_date;
        g_time_val_from_iso8601 (due, &due_date);

        rtm_task_set_due_date (task, &due_date);
        fail_unless (g_strcmp0 (g_time_val_to_iso8601 (rtm_task_get_due_date (task)), "2006-05-07T10:19:54Z") == 0,
                     "Task due_date not set properly");
}
END_TEST

START_TEST (test_added_date)
{
        gchar * added = "2006-05-07T10:19:54Z";
        GTimeVal added_date;
        g_time_val_from_iso8601 (added, &added_date);

        rtm_task_set_added_date (task, &added_date);
        fail_unless (g_strcmp0 (g_time_val_to_iso8601 (rtm_task_get_added_date (task)), "2006-05-07T10:19:54Z") == 0,
                     "Task added_date not set properly");
}
END_TEST

START_TEST (test_completed_date)
{
        gchar * completed = "2006-05-07T10:19:54Z";
        GTimeVal completed_date;
        g_time_val_from_iso8601 (completed, &completed_date);

        rtm_task_set_completed_date (task, &completed_date);
        fail_unless (g_strcmp0 (g_time_val_to_iso8601 (rtm_task_get_completed_date (task)), "2006-05-07T10:19:54Z") == 0,
                     "Task completed_date not set properly");
}
END_TEST

START_TEST (test_deleted_date)
{
        gchar * deleted = "2006-05-07T10:19:54Z";
        GTimeVal deleted_date;
        g_time_val_from_iso8601 (deleted, &deleted_date);

        rtm_task_set_deleted_date (task, &deleted_date);
        fail_unless (g_strcmp0 (g_time_val_to_iso8601 (rtm_task_get_deleted_date (task)), "2006-05-07T10:19:54Z") == 0,
                     "Task deleted_date not set properly");
}
END_TEST

START_TEST (test_has_due_time)
{
        rtm_task_set_has_due_time (task, TRUE);
        fail_unless (rtm_task_has_due_time (task),
                     "Task has_due_time not set properly");
}
END_TEST

START_TEST (test_estimate)
{
        rtm_task_set_estimate (task, "2 hours");
        fail_unless (g_strcmp0 (rtm_task_get_estimate (task), "2 hours") == 0,
                     "Task estimate not set properly");
}
END_TEST

START_TEST (test_load_data)
{
        RestXmlParser *parser;
        RestXmlNode *node;
        GList *tags, *item;
        gchar *tag;
        gchar xml[] =
                "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                "<taskseries id=\"987654\" name=\"Test\" "
                "url=\"http://gitorious.org/rtm-glib/\" "
                "location_id=\"111222\">"
                "<tags>"
                "<tag>rtm</tag>"
                "<tag>glib</tag>"
                "</tags>"
                "<task id=\"123456\" priority=\"2\" "
                "due=\"2006-05-07T10:19:54Z\" has_due_time=\"1\" "
                "added=\"2006-04-07T10:19:54Z\" completed=\"2006-06-07T10:19:54Z\" "
                "deleted=\"2006-07-07T10:19:54Z\" estimate=\"2 hours\" />"
                "</taskseries>";

        parser = rest_xml_parser_new ();
        node = rest_xml_parser_parse_from_data (parser, xml, sizeof (xml) - 1);

        rtm_task_load_data (task, node, "102030");

        fail_unless (g_strcmp0 (rtm_task_get_id (task), "123456") == 0,
                     "Task ID not set properly");
        fail_unless (g_strcmp0 (rtm_task_get_taskseries_id (task), "987654") == 0,
                     "Taskseries ID not set properly");
        fail_unless (g_strcmp0 (rtm_task_get_list_id (task), "102030") == 0,
                     "List ID not set properly");
        fail_unless (g_strcmp0 (rtm_task_get_name (task), "Test") == 0,
                     "Task name not load properly");
        fail_unless (g_strcmp0 (rtm_task_get_priority (task), "2") == 0,
                     "Task priority not load properly");
        fail_unless (g_strcmp0 (rtm_task_get_url (task),
                                "http://gitorious.org/rtm-glib/") == 0,
                     "Task URL not load properly");
        fail_unless (g_strcmp0 (rtm_task_get_location_id (task),
                                "111222") == 0,
                     "Task location ID not load properly");
        fail_unless (g_strcmp0 (g_time_val_to_iso8601 (rtm_task_get_due_date (task)),
                                "2006-05-07T10:19:54Z") == 0,
                     "Task due date not load properly");
        fail_unless (g_strcmp0 (g_time_val_to_iso8601 (rtm_task_get_added_date (task)),
                                "2006-04-07T10:19:54Z") == 0,
                     "Task added date not load properly");
        fail_unless (g_strcmp0 (g_time_val_to_iso8601 (rtm_task_get_completed_date (task)),
                                "2006-06-07T10:19:54Z") == 0,
                     "Task completed date not load properly");
        fail_unless (g_strcmp0 (g_time_val_to_iso8601 (rtm_task_get_deleted_date (task)),
                                "2006-07-07T10:19:54Z") == 0,
                     "Task deleted date not load properly");
        fail_unless (rtm_task_has_due_time (task),
                     "Task has_due_time not set properly");
        fail_unless (g_strcmp0 (rtm_task_get_estimate (task), "2 hours") == 0,
                     "Task estimate not load properly");

        tags = rtm_task_get_tags (task);
        fail_unless (g_list_length (tags) == 2,
                     "Task tags not load properly");

        item = g_list_first (tags);
        tag = (gchar *) item->data;
        fail_unless (g_strcmp0 (tag, "rtm") == 0,
                     "Task first tag not load properly");

        item = g_list_last (tags);
        tag = (gchar *) item->data;
        fail_unless (g_strcmp0 (tag, "glib") == 0,
                     "Task last tag not load properly");

        rest_xml_node_unref (node);
        g_object_unref (parser);
}
END_TEST

START_TEST (test_find_tag)
{
        gchar *found_tag;

        rtm_task_add_tag (task, "rtm", NULL);

        found_tag = rtm_task_find_tag (task, "rtm");
        fail_unless (found_tag != NULL, "Tag was not found");

        found_tag = rtm_task_find_tag (task, "glib");
        fail_unless (found_tag == NULL, "Tag found that does not exists");
}
END_TEST

START_TEST (test_add_tag)
{
        GList *tags;
        gchar *tag;
        GError *error = NULL;
        gboolean result = FALSE;

        result = rtm_task_add_tag (task, "rtm", &error);
        fail_unless (error == NULL, "Error must not be filled");
        fail_unless (result, "Method should return TRUE");

        tags = rtm_task_get_tags (task);

        fail_unless (g_list_length (tags) == 1, "Tag was not added properly");

        tag = (gchar *) tags->data;
        fail_unless (g_strcmp0 (tag, "rtm") == 0,
                     "Tag added is not rigth");
}
END_TEST

START_TEST (test_add_two_times_the_same_tag)
{
        GError *error = NULL;
        gboolean result = TRUE;

        rtm_task_add_tag (task, "rtm", NULL);
        result = rtm_task_add_tag (task, "rtm", &error);

        fail_unless (error != NULL, "Error not detected");
        fail_if (result, "Method should return FALSE");
        fail_unless (g_list_length (rtm_task_get_tags (task)) == 1,
                     "Second tag was added to the task");
}
END_TEST

START_TEST (test_remove_tag)
{
        gchar *tag;
        GError *error = NULL;
        gboolean result = FALSE;

        rtm_task_add_tag (task, "rtm", NULL);

        fail_unless (g_list_length (rtm_task_get_tags (task)) == 1,
                     "Tag was not added properly");

        result = rtm_task_remove_tag (task, "rtm", &error);
        fail_unless (error == NULL, "Error must not be filled");
        fail_unless (result, "Method should return TRUE");

        fail_unless (g_list_length (rtm_task_get_tags (task)) == 0,
                     "Tag was not removed properly");

        result = rtm_task_remove_tag (task, "glib", &error);
        fail_unless (error != NULL, "Error not detected properly");
        fail_if (result, "Method should return FALSE");
}
END_TEST

Suite *
check_rtm_task_suite (void)
{
        Suite * suite = suite_create ("RtmTask");

        TCase * tcase_id = tcase_create ("ID");
        tcase_add_checked_fixture (tcase_id, setup, teardown);
        tcase_add_test (tcase_id, test_id);
        suite_add_tcase (suite, tcase_id);

        TCase * tcase_taskseries_id = tcase_create ("Taskseries ID");
        tcase_add_checked_fixture (tcase_taskseries_id, setup, teardown);
        tcase_add_test (tcase_taskseries_id, test_taskseries_id);
        suite_add_tcase (suite, tcase_taskseries_id);

        TCase * tcase_list_id = tcase_create ("List ID");
        tcase_add_checked_fixture (tcase_list_id, setup, teardown);
        tcase_add_test (tcase_list_id, test_list_id);
        suite_add_tcase (suite, tcase_list_id);

        TCase * tcase_name = tcase_create ("Name");
        tcase_add_checked_fixture (tcase_name, setup, teardown);
        tcase_add_test (tcase_name, test_name);
        suite_add_tcase (suite, tcase_name);

        TCase * tcase_priority = tcase_create ("Priority");
        tcase_add_checked_fixture (tcase_priority, setup, teardown);
        tcase_add_test (tcase_priority, test_priority);
        suite_add_tcase (suite, tcase_priority);

        TCase * tcase_url = tcase_create ("URL");
        tcase_add_checked_fixture (tcase_url, setup, teardown);
        tcase_add_test (tcase_url, test_url);
        suite_add_tcase (suite, tcase_url);

        TCase * tcase_location_id = tcase_create ("Location ID");
        tcase_add_checked_fixture (tcase_location_id, setup, teardown);
        tcase_add_test (tcase_location_id, test_location_id);
        suite_add_tcase (suite, tcase_location_id);

        TCase * tcase_due_date = tcase_create ("Due date");
        tcase_add_checked_fixture (tcase_due_date, setup, teardown);
        tcase_add_test (tcase_due_date, test_due_date);
        suite_add_tcase (suite, tcase_due_date);

        TCase * tcase_added_date = tcase_create ("Added date");
        tcase_add_checked_fixture (tcase_added_date, setup, teardown);
        tcase_add_test (tcase_added_date, test_added_date);
        suite_add_tcase (suite, tcase_added_date);

        TCase * tcase_completed_date = tcase_create ("Completed date");
        tcase_add_checked_fixture (tcase_completed_date, setup, teardown);
        tcase_add_test (tcase_completed_date, test_completed_date);
        suite_add_tcase (suite, tcase_completed_date);

        TCase * tcase_deleted_date = tcase_create ("Deleted date");
        tcase_add_checked_fixture (tcase_deleted_date, setup, teardown);
        tcase_add_test (tcase_deleted_date, test_deleted_date);
        suite_add_tcase (suite, tcase_deleted_date);

        TCase * tcase_has_due_time = tcase_create ("Has due time");
        tcase_add_checked_fixture (tcase_has_due_time, setup, teardown);
        tcase_add_test (tcase_has_due_time, test_has_due_time);
        suite_add_tcase (suite, tcase_has_due_time);

        TCase * tcase_load_data = tcase_create ("Load data");
        tcase_add_checked_fixture (tcase_load_data, setup, teardown);
        tcase_add_test (tcase_load_data, test_load_data);
        suite_add_tcase (suite, tcase_load_data);

        TCase * tcase_find_tag = tcase_create ("Find tag");
        tcase_add_checked_fixture (tcase_find_tag, setup, teardown);
        tcase_add_test (tcase_find_tag, test_find_tag);
        suite_add_tcase (suite, tcase_find_tag);

        TCase * tcase_add_tag = tcase_create ("Add tag");
        tcase_add_checked_fixture (tcase_add_tag, setup, teardown);
        tcase_add_test (tcase_add_tag, test_add_tag);
        suite_add_tcase (suite, tcase_add_tag);

        TCase * tcase_add_two_times_the_same_tag = tcase_create ("Add two times the same tag");
        tcase_add_checked_fixture (tcase_add_two_times_the_same_tag, setup, teardown);
        tcase_add_test (tcase_add_two_times_the_same_tag, test_add_two_times_the_same_tag);
        suite_add_tcase (suite, tcase_add_two_times_the_same_tag);

        TCase * tcase_remove_tag = tcase_create ("Remove tag");
        tcase_add_checked_fixture (tcase_remove_tag, setup, teardown);
        tcase_add_test (tcase_remove_tag, test_remove_tag);
        suite_add_tcase (suite, tcase_remove_tag);

        return suite;
}

int
main ()
{
        int number_failed;

        SRunner *sr = srunner_create (NULL);

        srunner_add_suite (sr, (Suite *) check_rtm_task_suite ());

        srunner_run_all (sr, CK_VERBOSE);
        number_failed = srunner_ntests_failed (sr);
        srunner_free (sr);
        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
