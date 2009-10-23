/*
 * check-rtm-time-zone.c: Test RtmTimeZone class
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
#include <rtm-glib/rtm-time-zone.h>
#include <rest/rest-xml-parser.h>

RtmTimeZone * time_zone;

void
setup (void)
{
        g_type_init();

        time_zone = rtm_time_zone_new ();
}

void
teardown (void)
{
        g_object_unref (time_zone);
}

START_TEST (test_id)
{
        rtm_time_zone_set_id (time_zone, "217");
        fail_unless (g_strcmp0 (rtm_time_zone_get_id (time_zone), "217") == 0,
                     "Time zone ID not set properly");
}
END_TEST

START_TEST (test_name)
{
        rtm_time_zone_set_name (time_zone, "Asia/Hovd");
        fail_unless (g_strcmp0 (rtm_time_zone_get_name (time_zone), "Asia/Hovd") == 0,
                     "Time zone name not set properly");
}
END_TEST

START_TEST (test_dst)
{
        rtm_time_zone_set_dst (time_zone, TRUE);
        fail_unless (rtm_time_zone_is_dst (time_zone),
                     "Time zone DST not set properly");
}
END_TEST

START_TEST (test_offset)
{
        rtm_time_zone_set_offset (time_zone, "28800");
        fail_unless (g_strcmp0 (rtm_time_zone_get_offset (time_zone), "28800") == 0,
                     "Time zone offset not set properly");
}
END_TEST

START_TEST (test_current_offset)
{
        rtm_time_zone_set_current_offset (time_zone, "25200");
        fail_unless (g_strcmp0 (rtm_time_zone_get_current_offset (time_zone), "25200") == 0,
                     "Time zone current offset not set properly");
}
END_TEST

START_TEST (test_load_data)
{
        RestXmlParser *parser;
        RestXmlNode *node;
        gchar xml[] =
                "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                "<timezone id=\"217\" name=\"Asia/Hovd\" dst=\"1\" "
                "offset=\"28800\" current_offset=\"25200\" />";

        parser = rest_xml_parser_new ();
        node = rest_xml_parser_parse_from_data (parser, xml, sizeof (xml) - 1);

        rtm_time_zone_load_data (time_zone, node);

        fail_unless (g_strcmp0 (rtm_time_zone_get_id (time_zone), "217") == 0,
                     "Time zone ID not set properly");
        fail_unless (g_strcmp0 (rtm_time_zone_get_name (time_zone), "Asia/Hovd") == 0,
                     "Time zone name not set properly");
        fail_unless (rtm_time_zone_is_dst (time_zone),
                     "Time zone DST not set properly");
        fail_unless (g_strcmp0 (rtm_time_zone_get_offset (time_zone), "28800") == 0,
                     "Time zone offset not set properly");
        fail_unless (g_strcmp0 (rtm_time_zone_get_current_offset (time_zone), "25200") == 0,
                     "Time zone current offset not set properly");

        rest_xml_node_unref (node);
        g_object_unref (parser);
}
END_TEST

Suite *
check_rtm_time_zone_suite (void)
{
        Suite * suite = suite_create ("RtmTimeZone");

        TCase * tcase_id = tcase_create ("Id");
        tcase_add_checked_fixture (tcase_id, setup, teardown);
        tcase_add_test (tcase_id, test_id);
        suite_add_tcase (suite, tcase_id);

        TCase * tcase_name = tcase_create ("Name");
        tcase_add_checked_fixture (tcase_name, setup, teardown);
        tcase_add_test (tcase_name, test_name);
        suite_add_tcase (suite, tcase_name);

        TCase * tcase_dst = tcase_create ("DST");
        tcase_add_checked_fixture (tcase_dst, setup, teardown);
        tcase_add_test (tcase_dst, test_dst);
        suite_add_tcase (suite, tcase_dst);

        TCase * tcase_offset = tcase_create ("Offset");
        tcase_add_checked_fixture (tcase_offset, setup, teardown);
        tcase_add_test (tcase_offset, test_offset);
        suite_add_tcase (suite, tcase_offset);

        TCase * tcase_current_offset = tcase_create ("Current offset");
        tcase_add_checked_fixture (tcase_current_offset, setup, teardown);
        tcase_add_test (tcase_current_offset, test_current_offset);
        suite_add_tcase (suite, tcase_current_offset);

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

        srunner_add_suite (sr, (Suite *) check_rtm_time_zone_suite ());

        srunner_run_all (sr, CK_VERBOSE);
        number_failed = srunner_ntests_failed (sr);
        srunner_free (sr);
        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
