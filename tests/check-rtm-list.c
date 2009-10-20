/*
 * check-rtm-list.c: Test RtmList class
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
#include <rtm-glib/rtm-list.h>
#include <rtm-glib/rtm-task.h>
#include <rest/rest-xml-parser.h>

RtmList * list;

void
setup (void)
{
        g_type_init();

        list = rtm_list_new ();
}

void
teardown (void)
{
        g_object_unref (list);
}

START_TEST (test_id)
{
        rtm_list_set_id (list, "123456");
        fail_unless (g_strcmp0 (rtm_list_get_id (list), "123456") == 0,
                     "List ID not set properly");
}
END_TEST

START_TEST (test_name)
{
        rtm_list_set_name (list, "test");
        fail_unless (g_strcmp0 (rtm_list_get_name (list), "test") == 0,
                     "List name not set properly");
}
END_TEST

START_TEST (test_deleted)
{
        rtm_list_set_deleted (list, TRUE);
        fail_unless (rtm_list_is_deleted (list),
                     "List deleted not set properly");
}
END_TEST

START_TEST (test_locked)
{
        rtm_list_set_locked (list, TRUE);
        fail_unless (rtm_list_is_locked (list),
                     "List locked not set properly");
}
END_TEST

START_TEST (test_archived)
{
        rtm_list_set_archived (list, TRUE);
        fail_unless (rtm_list_is_archived (list),
                     "List archived not set properly");
}
END_TEST

START_TEST (test_position)
{
        rtm_list_set_position (list, "0");
        fail_unless (g_strcmp0 (rtm_list_get_position (list), "0") == 0,
                     "List position not set properly");
}
END_TEST

START_TEST (test_smart)
{
        rtm_list_set_smart (list, TRUE);
        fail_unless (rtm_list_is_smart (list),
                     "List smart not set properly");
}
END_TEST

START_TEST (test_sort_order)
{
        rtm_list_set_sort_order (list, "2");
        fail_unless (g_strcmp0 (rtm_list_get_sort_order (list), "2") == 0,
                     "List sort_order not set properly");
}
END_TEST

START_TEST (test_filter)
{
        rtm_list_set_smart (list, TRUE);
        rtm_list_set_filter (list, "test");
        fail_unless (g_strcmp0 (rtm_list_get_filter (list), "test") == 0,
                     "List filter not set properly");
}
END_TEST

START_TEST (test_load_data)
{
        RestXmlParser *parser;
        RestXmlNode *node;
        gchar xml[] =
                "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                "<list id=\"123456\" name=\"Test\" deleted=\"0\" locked=\"0\" "
                "archived=\"0\" position=\"0\" smart=\"1\" sort_order=\"2\">"
                "<filter>(priority:1)</filter>"
                "</list>";

        parser = rest_xml_parser_new ();
        node = rest_xml_parser_parse_from_data (parser, xml, sizeof (xml) - 1);

        rtm_list_load_data (list, node);

        fail_unless (g_strcmp0 (rtm_list_get_id (list), "123456") == 0,
                     "List ID not set properly");
        fail_unless (g_strcmp0 (rtm_list_get_name (list), "Test") == 0,
                     "List name not load properly");
        fail_unless (!rtm_list_is_deleted (list),
                     "List deleted not load properly");
        fail_unless (!rtm_list_is_locked (list),
                     "List locked not load properly");
        fail_unless (!rtm_list_is_archived (list),
                     "List archived not load properly");
        fail_unless (g_strcmp0 (rtm_list_get_position (list), "0") == 0,
                     "List position not load properly");
        fail_unless (rtm_list_is_smart (list),
                     "List smart not load properly");
        fail_unless (g_strcmp0 (rtm_list_get_sort_order (list), "2") == 0,
                     "List sort_order not load properly");
        fail_unless (g_strcmp0 (rtm_list_get_filter (list), "(priority:1)") == 0,
                     "List filter not load properly");

        rest_xml_node_unref (node);
        g_object_unref (parser);
}
END_TEST

START_TEST (test_find_task)
{
        RtmTask *task, *found_task;

        rtm_list_set_id (list, "987654");

        task = rtm_task_new ();
        rtm_task_set_id (task, "123456");

        rtm_list_add_task (list, task, NULL);

        found_task = rtm_list_find_task (list, "123456");
        fail_unless (found_task != NULL, "Task was not found");

        found_task = rtm_list_find_task (list, "987654");
        fail_unless (found_task == NULL, "Task found with wrong ID");
}
END_TEST

START_TEST (test_add_task)
{
        RtmTask *task;
        GList *glist, *item;
        GError *error = NULL;
        gboolean result = FALSE;

        rtm_list_set_id (list, "987654");

        task = rtm_task_new ();
        rtm_task_set_id (task, "123456");
        rtm_task_set_list_id (task, "102030");
        rtm_task_set_name (task, "My task");

        result = rtm_list_add_task (list, task, &error);
        fail_unless (error == NULL, "Error must not be filled");
        fail_unless (result, "Method should return TRUE");

        glist = rtm_list_get_tasks (list);

        fail_unless (g_list_length (glist) == 1, "Task was not added properly");

        item = glist;
        task = (RtmTask *) item->data;
        fail_unless (g_strcmp0 (rtm_task_get_id (task), "123456") == 0,
                     "Task added is not rigth");

        fail_unless (g_strcmp0 (rtm_task_get_list_id (task), "987654") == 0,
                     "List ID of task was not updated properly");
}
END_TEST

START_TEST (test_add_two_tasks_with_the_same_id)
{
        RtmTask *task, *task2;
        GError *error = NULL;
        gboolean result = TRUE;

        rtm_list_set_id (list, "987654");

        task = rtm_task_new ();
        rtm_task_set_id (task, "123456");

        task2 = rtm_task_new ();
        rtm_task_set_id (task2, "123456");

        rtm_list_add_task (list, task, NULL);
        result = rtm_list_add_task (list, task2, &error);

        fail_unless (error != NULL, "Error not detected");
        fail_if (result, "Method should return FALSE");
        fail_unless (g_list_length (rtm_list_get_tasks (list)) == 1,
                     "Second task was added to the list");
}
END_TEST

START_TEST (test_remove_task)
{
        RtmTask *task;
        GError *error = NULL;
        gboolean result = FALSE;

        rtm_list_set_id (list, "987654");

        task = rtm_task_new ();
        rtm_task_set_id (task, "123456");

        rtm_list_add_task (list, task, NULL);

        fail_unless (g_list_length (rtm_list_get_tasks (list)) == 1,
                     "Task was not added properly");

        result = rtm_list_remove_task (list, "123456", &error);
        fail_unless (error == NULL, "Error must not be filled");
        fail_unless (result, "Method should return TRUE");

        fail_unless (g_list_length (rtm_list_get_tasks (list)) == 0,
                     "Task was not removed properly");

        result = rtm_list_remove_task (list, "123456", &error);
        fail_unless (error != NULL, "Error not detected properly");
        fail_if (result, "Method should return FALSE");
}
END_TEST

START_TEST (test_list_of_tasks_is_unmodifiable)
{
        RtmTask *task;
        GList *glist;

        task = rtm_task_new ();
        rtm_task_set_id (task, "123456");

        glist = rtm_list_get_tasks (list);

        glist = g_list_append (glist, task);

        fail_unless (g_list_length (rtm_list_get_tasks (list)) == 0,
                     "Internal list of tasks attribute was modified");
}
END_TEST


Suite *
check_rtm_list_suite (void)
{
        Suite * suite = suite_create ("RtmList");

        TCase * tcase_id = tcase_create ("Id");
        tcase_add_checked_fixture (tcase_id, setup, teardown);
        tcase_add_test (tcase_id, test_id);
        suite_add_tcase (suite, tcase_id);

        TCase * tcase_name = tcase_create ("Name");
        tcase_add_checked_fixture (tcase_name, setup, teardown);
        tcase_add_test (tcase_name, test_name);
        suite_add_tcase (suite, tcase_name);

        TCase * tcase_deleted = tcase_create ("Deleted");
        tcase_add_checked_fixture (tcase_deleted, setup, teardown);
        tcase_add_test (tcase_deleted, test_deleted);
        suite_add_tcase (suite, tcase_deleted);

        TCase * tcase_locked = tcase_create ("Locked");
        tcase_add_checked_fixture (tcase_locked, setup, teardown);
        tcase_add_test (tcase_locked, test_locked);
        suite_add_tcase (suite, tcase_locked);

        TCase * tcase_archived = tcase_create ("Archived");
        tcase_add_checked_fixture (tcase_archived, setup, teardown);
        tcase_add_test (tcase_archived, test_archived);
        suite_add_tcase (suite, tcase_archived);

        TCase * tcase_position = tcase_create ("Position");
        tcase_add_checked_fixture (tcase_position, setup, teardown);
        tcase_add_test (tcase_position, test_position);
        suite_add_tcase (suite, tcase_position);

        TCase * tcase_smart = tcase_create ("Smart");
        tcase_add_checked_fixture (tcase_smart, setup, teardown);
        tcase_add_test (tcase_smart, test_smart);
        suite_add_tcase (suite, tcase_smart);

        TCase * tcase_sort_order = tcase_create ("Sort order");
        tcase_add_checked_fixture (tcase_sort_order, setup, teardown);
        tcase_add_test (tcase_sort_order, test_sort_order);
        suite_add_tcase (suite, tcase_sort_order);

        TCase * tcase_filter = tcase_create ("Filter");
        tcase_add_checked_fixture (tcase_filter, setup, teardown);
        tcase_add_test (tcase_filter, test_filter);
        suite_add_tcase (suite, tcase_filter);

        TCase * tcase_load_data = tcase_create ("Load data");
        tcase_add_checked_fixture (tcase_load_data, setup, teardown);
        tcase_add_test (tcase_load_data, test_load_data);
        suite_add_tcase (suite, tcase_load_data);

        TCase * tcase_find_task = tcase_create ("Find task");
        tcase_add_checked_fixture (tcase_find_task, setup, teardown);
        tcase_add_test (tcase_find_task, test_find_task);
        suite_add_tcase (suite, tcase_find_task);

        TCase * tcase_add_task = tcase_create ("Add task");
        tcase_add_checked_fixture (tcase_add_task, setup, teardown);
        tcase_add_test (tcase_add_task, test_add_task);
        suite_add_tcase (suite, tcase_add_task);

        TCase * tcase_add_two_tasks_with_the_same_id = tcase_create ("Add two tasks with the same ID");
        tcase_add_checked_fixture (tcase_add_two_tasks_with_the_same_id, setup, teardown);
        tcase_add_test (tcase_add_two_tasks_with_the_same_id, test_add_two_tasks_with_the_same_id);
        suite_add_tcase (suite, tcase_add_two_tasks_with_the_same_id);

        TCase * tcase_remove_task = tcase_create ("Remove task");
        tcase_add_checked_fixture (tcase_remove_task, setup, teardown);
        tcase_add_test (tcase_remove_task, test_remove_task);
        suite_add_tcase (suite, tcase_remove_task);

        TCase * tcase_list_of_tasks_is_unmodifiable =
                tcase_create ("List of tasks is unmodifiable");
        tcase_add_checked_fixture (
                tcase_list_of_tasks_is_unmodifiable, setup, teardown);
        tcase_add_test (
                tcase_list_of_tasks_is_unmodifiable,
                test_list_of_tasks_is_unmodifiable);
        suite_add_tcase (suite, tcase_list_of_tasks_is_unmodifiable);

        return suite;
}

int
main ()
{
        int number_failed;

        SRunner *sr = srunner_create (NULL);

        srunner_add_suite (sr, (Suite *) check_rtm_list_suite ());

        srunner_run_all (sr, CK_VERBOSE);
        number_failed = srunner_ntests_failed (sr);
        srunner_free (sr);
        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
