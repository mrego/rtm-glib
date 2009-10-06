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

START_TEST (test_load_data)
{
        RestXmlParser *parser;
        RestXmlNode *node;
        gchar xml[] =
                "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                "<taskseries id=\"987654\" name=\"Test\">"
                "<task id=\"123456\" priority=\"2\" />"
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

        rest_xml_node_unref (node);
        g_object_unref (parser);
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

        TCase * tcase_load_data = tcase_create ("Load data");
        tcase_add_checked_fixture (tcase_load_data, setup, teardown);
        tcase_add_test (tcase_load_data, test_load_data);
        suite_add_tcase (suite, tcase_load_data);

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
