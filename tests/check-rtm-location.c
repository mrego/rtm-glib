/*
 * check-rtm-location.c: Test RtmLocation class
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
#include <rtm-glib/rtm-location.h>
#include <rest/rest-xml-parser.h>

RtmLocation * location;

void
setup (void)
{
        g_type_init();

        location = rtm_location_new ();
}

void
teardown (void)
{
        g_object_unref (location);
}

START_TEST (test_id)
{
        rtm_location_set_id (location, "123456");
        fail_unless (g_strcmp0 (rtm_location_get_id (location), "123456") == 0,
                     "Location ID not set properly");
}
END_TEST

START_TEST (test_name)
{
        rtm_location_set_name (location, "Berlin");
        fail_unless (g_strcmp0 (rtm_location_get_name (location), "Berlin") == 0,
                     "Location name not set properly");
}
END_TEST

START_TEST (test_longitude)
{
        rtm_location_set_longitude (location, "13.411508");
        fail_unless (g_strcmp0 (rtm_location_get_longitude (location), "13.411508") == 0,
                     "Location longitude not set properly");
}
END_TEST

START_TEST (test_latitude)
{
        rtm_location_set_latitude (location, "52.524008");
        fail_unless (g_strcmp0 (rtm_location_get_latitude (location), "52.524008") == 0,
                     "Location latitude not set properly");
}
END_TEST

START_TEST (test_zoom)
{
        rtm_location_set_zoom (location, "9");
        fail_unless (g_strcmp0 (rtm_location_get_zoom (location), "9") == 0,
                     "Location zoom not set properly");
}
END_TEST

START_TEST (test_address)
{
        rtm_location_set_address (location, "Berlin, Germany");
        fail_unless (g_strcmp0 (rtm_location_get_address (location), "Berlin, Germany") == 0,
                     "Location address not set properly");
}
END_TEST

START_TEST (test_viewable)
{
        rtm_location_set_viewable (location, TRUE);
        fail_unless (rtm_location_is_viewable (location),
                     "Location viewable not set properly");
}
END_TEST

START_TEST (test_load_data)
{
        RestXmlParser *parser;
        RestXmlNode *node;
        gchar xml[] =
                "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                "<location id=\"987654321\" name=\"Berlin\" "
                "longitude=\"13.411508\" latitude=\"52.524008\" zoom=\"9\" "
                "address=\"Berlin, Germany\" viewable=\"1\" />";

        parser = rest_xml_parser_new ();
        node = rest_xml_parser_parse_from_data (parser, xml, sizeof (xml) - 1);

        rtm_location_load_data (location, node);

        fail_unless (g_strcmp0 (rtm_location_get_id (location), "987654321") == 0,
                     "Location ID not set properly");
        fail_unless (g_strcmp0 (rtm_location_get_name (location), "Berlin") == 0,
                     "Location name not load properly");
        fail_unless (g_strcmp0 (rtm_location_get_longitude (location), "13.411508") == 0,
                     "Location longitude not set properly");
        fail_unless (g_strcmp0 (rtm_location_get_latitude (location), "52.524008") == 0,
                     "Location latitude not set properly");
        fail_unless (g_strcmp0 (rtm_location_get_zoom (location), "9") == 0,
                     "Location zoom not set properly");
        fail_unless (g_strcmp0 (rtm_location_get_address (location), "Berlin, Germany") == 0,
                     "Location address not set properly");
        fail_unless (rtm_location_is_viewable (location),
                     "Location viewable not load properly");

        rest_xml_node_unref (node);
        g_object_unref (parser);
}
END_TEST

Suite *
check_rtm_location_suite (void)
{
        Suite * suite = suite_create ("RtmLocation");

        TCase * tcase_id = tcase_create ("Id");
        tcase_add_checked_fixture (tcase_id, setup, teardown);
        tcase_add_test (tcase_id, test_id);
        suite_add_tcase (suite, tcase_id);

        TCase * tcase_name = tcase_create ("Name");
        tcase_add_checked_fixture (tcase_name, setup, teardown);
        tcase_add_test (tcase_name, test_name);
        suite_add_tcase (suite, tcase_name);

        TCase * tcase_longitude = tcase_create ("Longitude");
        tcase_add_checked_fixture (tcase_longitude, setup, teardown);
        tcase_add_test (tcase_longitude, test_longitude);
        suite_add_tcase (suite, tcase_longitude);

        TCase * tcase_latitude = tcase_create ("Latitude");
        tcase_add_checked_fixture (tcase_latitude, setup, teardown);
        tcase_add_test (tcase_latitude, test_latitude);
        suite_add_tcase (suite, tcase_latitude);

        TCase * tcase_zoom = tcase_create ("Zoom");
        tcase_add_checked_fixture (tcase_zoom, setup, teardown);
        tcase_add_test (tcase_zoom, test_zoom);
        suite_add_tcase (suite, tcase_zoom);

        TCase * tcase_address = tcase_create ("Address");
        tcase_add_checked_fixture (tcase_address, setup, teardown);
        tcase_add_test (tcase_address, test_address);
        suite_add_tcase (suite, tcase_address);

        TCase * tcase_viewable = tcase_create ("Viewable");
        tcase_add_checked_fixture (tcase_viewable, setup, teardown);
        tcase_add_test (tcase_viewable, test_viewable);
        suite_add_tcase (suite, tcase_viewable);

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

        srunner_add_suite (sr, (Suite *) check_rtm_location_suite ());

        srunner_run_all (sr, CK_VERBOSE);
        number_failed = srunner_ntests_failed (sr);
        srunner_free (sr);
        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
