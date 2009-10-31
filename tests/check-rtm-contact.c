/*
 * check-rtm-contact.c: Test RtmContact class
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
#include <rtm-glib/rtm-contact.h>
#include <rest/rest-xml-parser.h>

RtmContact * contact;

void
setup (void)
{
        g_type_init();

        contact = rtm_contact_new ();
}

void
teardown (void)
{
        g_object_unref (contact);
}

START_TEST (test_id)
{
        rtm_contact_set_id (contact, "123456");
        fail_unless (g_strcmp0 (rtm_contact_get_id (contact), "123456") == 0,
                     "Contact ID not set properly");
}
END_TEST

START_TEST (test_username)
{
        rtm_contact_set_username (contact, "username");
        fail_unless (g_strcmp0 (rtm_contact_get_username (contact), "username") == 0,
                     "Contact username not set properly");
}
END_TEST

START_TEST (test_fullname)
{
        rtm_contact_set_fullname (contact, "fullname");
        fail_unless (g_strcmp0 (rtm_contact_get_fullname (contact), "fullname") == 0,
                     "Contact fullname not set properly");
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
                "<contact id=\"123456\" username=\"username\" "
                "fullname=\"fullname\" />";

        parser = rest_xml_parser_new ();
        node = rest_xml_parser_parse_from_data (parser, xml, sizeof (xml) - 1);

        rtm_contact_load_data (contact, node);

        fail_unless (g_strcmp0 (rtm_contact_get_id (contact), "123456") == 0,
                     "Contact ID not set properly");
        fail_unless (g_strcmp0 (rtm_contact_get_username (contact), "username") == 0,
                     "Contact username not load properly");
        fail_unless (g_strcmp0 (rtm_contact_get_fullname (contact), "fullname") == 0,
                     "Contact fullname not load properly");

        rest_xml_node_unref (node);
        g_object_unref (parser);
}
END_TEST

Suite *
check_rtm_contact_suite (void)
{
        Suite * suite = suite_create ("RtmContact");

        TCase * tcase_id = tcase_create ("ID");
        tcase_add_checked_fixture (tcase_id, setup, teardown);
        tcase_add_test (tcase_id, test_id);
        suite_add_tcase (suite, tcase_id);

        TCase * tcase_username = tcase_create ("Username");
        tcase_add_checked_fixture (tcase_username, setup, teardown);
        tcase_add_test (tcase_username, test_username);
        suite_add_tcase (suite, tcase_username);

        TCase * tcase_fullname = tcase_create ("Fullname");
        tcase_add_checked_fixture (tcase_fullname, setup, teardown);
        tcase_add_test (tcase_fullname, test_fullname);
        suite_add_tcase (suite, tcase_fullname);

        return suite;
}

int
main ()
{
        int number_failed;

        SRunner *sr = srunner_create (NULL);

        srunner_add_suite (sr, (Suite *) check_rtm_contact_suite ());

        srunner_run_all (sr, CK_VERBOSE);
        number_failed = srunner_ntests_failed (sr);
        srunner_free (sr);
        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
