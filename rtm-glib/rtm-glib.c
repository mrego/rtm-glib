/*
 * rtm-glib.c: API Library
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
 * SECTION:rtm-glib
 * @short_description: API library for Remember The Milk
 *
 * API library to acces Remember The Milk web service
 */

#include <glib-object.h>
#include <string.h>
#include <rest/rest-proxy.h>
#include <rest/rest-xml-parser.h>
#include <rtm-glib.h>
#include <rtm-error.h>


#define RTM_URL "http://api.rememberthemilk.com/services/rest/"
#define RTM_URL_AUTH "http://www.rememberthemilk.com/services/auth/"


/* Remember The Milk web service methods */

/* @param (required): api_key=%s, Your API application key */
#define RTM_METHOD_TEST_ECHO "rtm.test.echo"

/* @param (required): api_key=%s, Your API application key */
/* @param (required): auth_token=%s, The authentication token to check */
#define RTM_METHOD_TEST_LOGIN "rtm.test.login"

/* @param (required): api_key=%s, Your API application key */
#define RTM_METHOD_AUTH_GET_FROB "rtm.auth.getFrob"

/* @param (required): api_key=%s, Your API application key */
/* @param (required): frob=%s, The frob to check */
#define RTM_METHOD_AUTH_GET_TOKEN "rtm.auth.getToken"

/* @param (required): api_key=%s, Your API application key */
/* @param (required): auth_token=%s, The authentication token to check */
#define RTM_METHOD_AUTH_CHECK_TOKEN "rtm.auth.checkToken"

/* @param (required): api_key=%s, Your API application key */
/* @param (optional): list_id=%s, The id of the list to perform an action on */
/* @param (optional): filter=%s, If specified, only tasks matching the desired
   criteria are returned */
/* @param (optional): last_sync=%s, An ISO 8601 formatted time value. If
   last_sync is provided, only tasks modified since last_sync will be returned,
   and each element will have an attribute, current, equal to last_sync */
#define RTM_METHOD_TASKS_GET_LIST "rtm.tasks.getList"

/* @param (required): api_key=%s, Your API application key */
#define RTM_METHOD_LISTS_GET_LIST "rtm.lists.getList"

/* @param (required): api_key=%s, Your API application key */
#define RTM_METHOD_TIMELINES_CREATE "rtm.timelines.create"

/* @param (required): api_key=%s, Your API application key */
/* @param (required): timeline=%s, The timeline within which to run a method */
/* @param (optional): list_id=%s, The id of the list to perform an action on */
/* @param (required): name=%s, The desired task name */
/* @param (optional): parse=%s, Specifies whether to process name using Smart
   Add */
#define RTM_METHOD_TASKS_ADD "rtm.tasks.add"

/* @param (required): api_key=%s, Your API application key */
/* @param (required): timeline=%s, The timeline within which to run a method */
/* @param (required): transaction_id=%s, The id of transaction within a
   timeline */
#define RTM_METHOD_TRANSACTIONS_UNDO "rtm.transactions.undo"

/* @param (required): api_key=%s, Your API application key */
/* @param (required): timeline=%s, The timeline within which to run a method */
/* @param (required): list_id=%s, The id of the list to perform an action on */
/* @param (required): taskseries_id=%s, The id of the task series to perform an
   action on */
/* @param (required): task_id=%s, The id of the task to perform an action on */
#define RTM_METHOD_TASKS_DELETE "rtm.tasks.delete"

/* @param (required): api_key=%s, Your API application key */
/* @param (required): timeline=%s, The timeline within which to run a method */
/* @param (required): list_id=%s, The id of the list to perform an action on */
/* @param (required): taskseries_id=%s, The id of the task series to perform an
   action on */
/* @param (required): task_id=%s, The id of the task to perform an action on */
/* @param (required): name=%s, The desired task name */
#define RTM_METHOD_TASKS_SET_NAME "rtm.tasks.setName"

/* @param (required): api_key=%s, Your API application key */
/* @param (required): timeline=%s, The timeline within which to run a method */
/* @param (required): name=%s, The desired list name */
/* @param (optional): filter=%s, If specified, a Smart List is created with the
   desired criteria */
#define RTM_METHOD_LISTS_ADD "rtm.lists.add"

/* @param (required): api_key=%s, Your API application key */
/* @param (required): timeline=%s, The timeline within which to run a method */
/* @param (required): list_id=%s, The id of the list to perform an action on */
#define RTM_METHOD_LISTS_DELETE "rtm.lists.delete"

/* @param (required): api_key=%s, Your API application key */
/* @param (required): timeline=%s, The timeline within which to run a method */
/* @param (required): list_id=%s, The id of the list to perform an action on */
/* @param (required): name=%s, The desired list name. Cannot be Inbox or Sent */
#define RTM_METHOD_LISTS_SET_NAME "rtm.lists.setName"


#define RTM_GLIB_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE (        \
                                           (obj), RTM_TYPE_GLIB, RtmGlibPrivate))

struct _RtmGlibPrivate {
        gchar *api_key;
        gchar *shared_secret;
        gchar *auth_token;
};

enum {
        PROP_0,

        PROP_API_KEY,
        PROP_SHARED_SECRET,
        PROP_AUTH_TOKEN,
};

G_DEFINE_TYPE (RtmGlib, rtm_glib, G_TYPE_OBJECT);



/* Private methods headers */
gchar *
rtm_glib_caculate_md5 (RtmGlib *rtm, GHashTable * params);

gboolean
rtm_glib_check_response (RtmGlib *rtm, RestXmlNode *root, GError **error);

void
rtm_glib_sign_call (RtmGlib *rtm, RestProxyCall **call);

RestXmlNode *
rtm_glib_call_method (RtmGlib *rtm, gchar *method, GError **error, ...);



static void
rtm_glib_get_property (GObject *gobject, guint prop_id, GValue *value,
                       GParamSpec *pspec)
{
        RtmGlibPrivate *priv = RTM_GLIB_GET_PRIVATE (RTM_GLIB (gobject));

        switch (prop_id) {
        case PROP_API_KEY:
                g_value_set_string (value, priv->api_key);
                break;

        case PROP_SHARED_SECRET:
                g_value_set_string (value, priv->shared_secret);
                break;

        case PROP_AUTH_TOKEN:
                g_value_set_string (value, priv->auth_token);
                break;

        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id,
                                                   pspec);
                break;
        }
}

static void
rtm_glib_set_property (GObject *gobject, guint prop_id, const GValue *value,
                       GParamSpec *pspec)
{
        RtmGlibPrivate *priv = RTM_GLIB_GET_PRIVATE (RTM_GLIB (gobject));

        switch (prop_id) {
        case PROP_API_KEY:
                g_free (priv->api_key);
                priv->api_key = g_value_dup_string (value);
                break;

        case PROP_SHARED_SECRET:
                g_free (priv->shared_secret);
                priv->shared_secret = g_value_dup_string (value);
                break;

        case PROP_AUTH_TOKEN:
                g_free (priv->auth_token);
                priv->auth_token = g_value_dup_string (value);
                break;

        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id,
                                                   pspec);
                break;
        }
}

static void
rtm_glib_finalize (GObject *gobject)
{
        RtmGlibPrivate *priv = RTM_GLIB_GET_PRIVATE (RTM_GLIB (gobject));

        g_free (priv->api_key);
        g_free (priv->shared_secret);
        g_free (priv->auth_token);

        G_OBJECT_CLASS (rtm_glib_parent_class)->finalize (gobject);
}

static void
rtm_glib_class_init (RtmGlibClass *klass)
{
        GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

        g_type_class_add_private (klass, sizeof (RtmGlibPrivate));

        gobject_class->get_property = rtm_glib_get_property;
        gobject_class->set_property = rtm_glib_set_property;
        gobject_class->finalize = rtm_glib_finalize;

        g_object_class_install_property (
                gobject_class,
                PROP_API_KEY,
                g_param_spec_string (
                        "api_key",
                        "API Key",
                        "API Key for Remember The Milk service",
                        NULL,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_SHARED_SECRET,
                g_param_spec_string (
                        "shared_secret",
                        "Shared Secret",
                        "Shared Secret key for Remember The Milk service",
                        NULL,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_SHARED_SECRET,
                g_param_spec_string (
                        "auth_token",
                        "Authentication Token",
                        "Authentication Token for Remember The Milk service",
                        NULL,
                        G_PARAM_READWRITE));

}

static void
rtm_glib_init (RtmGlib *rtm)
{
        rtm->priv = RTM_GLIB_GET_PRIVATE (rtm);
}

/**
 * rtm_glib_new:
 * @api_key: API Key for Remember The Milk service
 * @shared_secret: Shared Secret key for Remember The Milk service
 *
 * Creates a new instance of this class.
 *
 * Returns: a new #RtmGlib object.
 */
RtmGlib *
rtm_glib_new (gchar *api_key, gchar *shared_secret)
{
        g_return_val_if_fail (api_key != NULL, NULL);
        g_return_val_if_fail (shared_secret != NULL, NULL);

        return g_object_new (RTM_TYPE_GLIB,
                             "api_key", api_key,
                             "shared_secret", shared_secret,
                             NULL);
}

/**
 * rtm_glib_caculate_md5:
 * @rtm: a #RtmGlib object.
 * @params: a #GHashTable with the params for a HTTP request.
 *
 * Calculates the md5 value for the params in order to sign the HTTP request
 * following the algorithm described at:
 * http://www.rememberthemilk.com/services/api/authentication.rtm
 *
 * Returns: the md5 value.
 */
gchar *
rtm_glib_caculate_md5 (RtmGlib *rtm, GHashTable * params)
{
        g_assert (rtm != NULL);
        g_assert (params != NULL);

        GList *keys;
        gchar *key;
        gchar *value;
        gchar *params_string;
        gchar *md5;

        g_debug ("rtm_calculate_md5");

        /* First concat the SHARED SECRET key */
        params_string = rtm->priv->shared_secret;

        /* Sort the param hash */
        keys = g_hash_table_get_keys (params);
        keys = g_list_sort (keys, (GCompareFunc) g_strcmp0);

        /* Iterate over the params hash */
        while (keys) {
                key = keys->data;
                value = g_hash_table_lookup (params, key);

                // Concat each key and value
                params_string = g_strconcat (params_string, key, value, NULL);

                keys = keys->next;
        }

        g_debug ("params_string: %s", params_string);

        /* Generates the md5 value */
        md5 = g_compute_checksum_for_string (G_CHECKSUM_MD5,
                                             params_string,
                                             strlen (params_string));

        g_debug ("md5: %s", md5);

        g_free (params_string);

        return md5;
}

/**
 * rtm_glib_sign_call:
 * @rtm: a #RtmGlib object.
 * @call: a #RestProxyCall to be signed.
 * @see_also: rtm_caculate_md5()
 *
 * Signs a call adding the api_sig param.
 */
void
rtm_glib_sign_call (RtmGlib *rtm, RestProxyCall **call)
{
        g_assert (rtm != NULL);
        g_assert (call != NULL);

        GHashTable *params;
        gchar *md5;

        g_debug ("rtm_sign_call");

        /* Gets the call params */
        params = rest_proxy_call_get_params (*call);

        /* Calculate the md5 */
        md5 = rtm_glib_caculate_md5 (rtm, params);

        /* Adds the new api_sig param */
        rest_proxy_call_add_param (*call, "api_sig", md5);

        g_hash_table_unref (params);
        g_free (md5);
}

/**
 * rtm_glib_check_response:
 * @rtm: a #RtmGlib object.
 * @root: the root #RestXmlNode of Remember The Milk response.
 * @error: a #GError to be filled if response is not successful.
 *
 * Checks if response is or not successful.
 *
 * Returns: %TRUE if the response is successful.
 */
gboolean
rtm_glib_check_response (RtmGlib *rtm, RestXmlNode *root, GError **error)
{
        g_assert (rtm != NULL);
        g_assert (root != NULL);

        RestXmlNode *error_node;
        const gchar *status, *error_code, *error_msg;

        if (g_strcmp0 (root->name, "rsp") == 0) {
                status = rest_xml_node_get_attr (root, "stat");
                if (g_strcmp0 (status, "ok") != 0) {
                        error_node = rest_xml_node_find (root, "err");

                        error_code = rest_xml_node_get_attr (error_node, "code");
                        error_msg = rest_xml_node_get_attr (error_node, "msg");

                        g_set_error (
                                error,
                                RTM_ERROR_DOMAIN,
                                RTM_ERROR_RESPONSE_FAIL,
                                "%s: %s",
                                error_code,
                                error_msg);
                        return FALSE;
                }
        } else {
                g_set_error (
                        error,
                        RTM_ERROR_DOMAIN,
                        RTM_UNKNOWN_ERROR,
                        "Unknown response from Remember The Milk");
                return FALSE;
        }

        return TRUE;
}

/**
 * rtm_glib_call_method:
 * @rtm: a #RtmGlib object.
 * @method: the method name to be called.
 * @Varargs: list of parameters (pairs of name and value) terminated by %NULL.
 *
 * Calls a method of Remember The Milk API with the arguments passed.
 *
 * Returns: A #RestXmlNode object with the method response. Or %NULL if call
 * fails.
 */
RestXmlNode *
rtm_glib_call_method (RtmGlib *rtm, gchar *method, GError **error, ...)
{
        g_assert (rtm != NULL);
        g_assert (method != NULL);

        RestProxy *proxy;
        RestProxyCall *call;
        RestXmlParser *parser;
        RestXmlNode *root;
        va_list params;
        GError *tmp_error = NULL;

        g_debug ("rtm_call_method: %s", method);

        proxy = rest_proxy_new (RTM_URL, FALSE);
        call = rest_proxy_new_call (proxy);

        rest_proxy_call_add_param (call, "method", method);
        rest_proxy_call_add_param (call, "api_key", rtm->priv->api_key);

        va_start (params, error);
        rest_proxy_call_add_params_from_valist (call, params);
        va_end (params);

        rtm_glib_sign_call (rtm, &call);
        rest_proxy_call_run (call, NULL, &tmp_error);
        if (tmp_error != NULL) {
                g_object_unref (call);
                g_object_unref (proxy);

                g_set_error (
                        error,
                        RTM_ERROR_DOMAIN,
                        RTM_UNKNOWN_ERROR,
                        "%s",
                        tmp_error->message);

                return NULL;
        }
        g_debug ("payload: %s", rest_proxy_call_get_payload (call));

        parser = rest_xml_parser_new ();
        root = rest_xml_parser_parse_from_data (
                parser,
                rest_proxy_call_get_payload (call),
                rest_proxy_call_get_payload_length (call));

        if (!rtm_glib_check_response (rtm, root, &tmp_error)) {
                g_propagate_error (error, tmp_error);
                rest_xml_node_unref (root);
                root = NULL;
        }

        g_object_unref (call);
        g_object_unref (proxy);
        g_object_unref (parser);

        return root;
}

/**
 * rtm_glib_test_echo:
 * @rtm: a #RtmGlib object.
 * @error: location to store #GError or %NULL.
 *
 * Just checks if the webservice is working properly.
 *
 * Returns: %TRUE if the service is available.
 */
gboolean
rtm_glib_test_echo (RtmGlib *rtm, GError **error)
{
        g_return_val_if_fail (rtm != NULL, FALSE);

        RestXmlNode *root, *node;
        gboolean valid = FALSE;
        GError *tmp_error = NULL;

        g_debug ("rtm_glib_test_echo");

        root = rtm_glib_call_method (rtm,
                RTM_METHOD_TEST_ECHO, &tmp_error,
                NULL);
        if (tmp_error != NULL) {
                g_propagate_error (error, tmp_error);
                return FALSE;
        }

        node = rest_xml_node_find (root, "api_key");
        valid = (g_strcmp0 (node->content, rtm->priv->api_key) == 0);

        node = rest_xml_node_find (root, "method");
        g_debug ("method: %s", node->content);

        rest_xml_node_unref (root);

        return valid;
}

/**
 * rtm_glib_test_login:
 * @rtm: a #RtmGlib object.
 * @auth_token: the authentication token.
 * @error: location to store #GError or %NULL.
 *
 * Checks if the user is already logged. In that case sets the
 * #RtmGlib:auth_token property.
 *
 * Returns: The username if is already logged.
 */
gchar *
rtm_glib_test_login (RtmGlib *rtm, gchar *auth_token, GError **error)
{
        g_return_val_if_fail (rtm != NULL, NULL);
        g_return_val_if_fail (auth_token != NULL, NULL);

        RestXmlNode *root, *node;
        gchar *username;
        GError *tmp_error = NULL;

        g_debug ("rtm_test_login");

        root = rtm_glib_call_method (rtm,
                RTM_METHOD_TEST_LOGIN, &tmp_error,
                "auth_token", auth_token,
                NULL);
        if (tmp_error != NULL) {
                g_propagate_error (error, tmp_error);
                return NULL;
        }

        node = rest_xml_node_find (root, "user");
        g_debug ("user_id: %s", rest_xml_node_get_attr (node, "id"));

        node = rest_xml_node_find (root, "username");
        username = g_strdup (node->content);
        g_debug ("username: %s", username);

        rest_xml_node_unref (root);

        rtm->priv->auth_token = g_strdup (auth_token);

        return username;
}

/**
 * rtm_glib_auth_get_frob:
 * @rtm: a #RtmGlib object.
 * @error: location to store #GError or %NULL.
 *
 * Gets the authentication frob.
 *
 * Returns: The authentication frob.
 **/
gchar *
rtm_glib_auth_get_frob (RtmGlib *rtm, GError **error)
{
        g_return_val_if_fail (rtm != NULL, NULL);

        RestXmlNode *root, *node;
        gchar *frob;
        GError *tmp_error = NULL;

        g_debug ("rtm_glib_auth_get_frob");

        root = rtm_glib_call_method (rtm,
                RTM_METHOD_AUTH_GET_FROB, &tmp_error,
                NULL);
        if (tmp_error != NULL) {
                g_propagate_error (error, tmp_error);
                return NULL;
        }

        node = rest_xml_node_find (root, "frob");

        frob = g_strdup (node->content);
        g_debug ("frob: %s", frob);

        rest_xml_node_unref (root);

        return frob;
}

/**
 * rtm_glib_auth_get_token:
 * @rtm: a #RtmGlib object.
 * @frob: the authentication frob.
 * @error: location to store #GError or %NULL.
 *
 * Gets the authentication token and sets the #RtmGlib:auth_token property.
 *
 * Returns: The authentication token.
 **/
gchar *
rtm_glib_auth_get_token (RtmGlib *rtm, gchar *frob, GError **error)
{
        g_return_val_if_fail (rtm != NULL, NULL);
        g_return_val_if_fail (frob != NULL, NULL);

        RestXmlNode *root, *node;
        gchar *auth_token;
        GError *tmp_error = NULL;

        g_debug ("rtm_glib_auth_get_token");

        root = rtm_glib_call_method (rtm,
                RTM_METHOD_AUTH_GET_TOKEN, &tmp_error,
                "frob", frob,
                NULL);
        if (tmp_error != NULL) {
                g_propagate_error (error, tmp_error);
                return NULL;
        }

        node = rest_xml_node_find (root, "token");

        auth_token = g_strdup (node->content);
        g_debug ("auth_token: %s", auth_token);

        rest_xml_node_unref (root);

        rtm->priv->auth_token = g_strdup (auth_token);

        return auth_token;
}

/**
 * rtm_glib_auth_check_token:
 * @rtm: a #RtmGlib object.
 * @auth_token: the authentication token.
 * @error: location to store #GError or %NULL.
 *
 * Checks if authentication token is or not valid. If it is valid sets the
 * #RtmGlib:auth_token property.
 *
 * Returns: %TRUE if authentication token is valid.
 **/
gboolean
rtm_glib_auth_check_token (RtmGlib *rtm, gchar *auth_token, GError **error)
{
        g_return_val_if_fail (rtm != NULL, FALSE);
        g_return_val_if_fail (auth_token != NULL, FALSE);

        RestXmlNode *root, *node;
        gboolean valid = FALSE;
        GError *tmp_error = NULL;

        g_debug ("rtm_glib_auth_check_token");

        root = rtm_glib_call_method (rtm,
                RTM_METHOD_AUTH_CHECK_TOKEN, &tmp_error,
                "auth_token", auth_token,
                NULL);
        if (tmp_error != NULL) {
                g_propagate_error (error, tmp_error);
                return FALSE;
        }

        node = rest_xml_node_find (root, "token");

        valid = (g_strcmp0 (node->content, auth_token) == 0);

        rest_xml_node_unref (root);

        if (valid) {
                rtm->priv->auth_token = g_strdup (auth_token);
        }

        return valid;
}

/**
 * rtm_glib_auth_get_login_url:
 * @rtm: a #RtmGlib object.
 * @frob: the authentication frob.
 *
 * Gets the URL to login in Remember The Milk.
 *
 * Returns: The URL to login.
 **/
gchar *
rtm_glib_auth_get_login_url (RtmGlib *rtm, gchar *frob)
{
        g_return_val_if_fail (rtm != NULL, NULL);
        g_return_val_if_fail (frob != NULL, NULL);

        gchar *url;
        GHashTable *params;
        gchar *md5;

        g_debug ("rtm_glib_auth_get_login_url");

        params = g_hash_table_new (g_str_hash, g_str_equal);

        g_hash_table_insert (params, "api_key", rtm->priv->api_key);
        g_hash_table_insert (params, "perms", "delete");
        g_hash_table_insert (params, "frob", frob);

        md5 = rtm_glib_caculate_md5 (rtm, params);

        url = g_strconcat (RTM_URL_AUTH, "?",
                           "api_key=", rtm->priv->api_key, "&",
                           "perms=", "delete", "&",
                           "frob=", frob, "&",
                           "api_sig=", md5,
                           NULL);

        g_debug ("url: %s", url);

        g_hash_table_unref (params);
        g_free (md5);

        return url;
}

/**
 * rtm_glib_tasks_get_list:
 * @rtm: a #RtmGlib object already authenticated.
 * @list_id: %NULL or the id of the list to perform an action on.
 * @filter: %NULL or if specified, only tasks matching the desired criteria
 * are returned.
 * @last_sync: %NULL or an ISO 8601 formatted time value. If @last_sync is
 * provided, only tasks modified since last_sync will be returned, and each
 * element will have an attribute, current, equal to @last_sync.
 * @error: location to store #GError or %NULL.
 *
 * Gets the list of tasks.
 *
 * Returns: A #GList of #RtmTask objects.
 **/
GList *
rtm_glib_tasks_get_list (RtmGlib *rtm, gchar *list_id, gchar *filter,
                         gchar *last_sync, GError **error)
{
        g_return_val_if_fail (rtm != NULL, NULL);
        g_return_val_if_fail (rtm->priv->auth_token != NULL, NULL);

        if (filter == NULL) {
                filter = "";
        }
        if (last_sync == NULL) {
                last_sync = "";
        }

        RestXmlNode *root, *node, *node2;
        GList *list = NULL;
        RtmTask *task;
        const gchar *task_list_id;
        GError *tmp_error = NULL;

        g_debug ("rtm_glib_tasks_get_list");

        if (list_id == NULL) {
                root = rtm_glib_call_method (
                        rtm,
                        RTM_METHOD_TASKS_GET_LIST, &tmp_error,
                        "auth_token", rtm->priv->auth_token,
                        "filter", filter,
                        "last_sync", last_sync,
                        NULL);
        } else {
                root = rtm_glib_call_method (
                        rtm,
                        RTM_METHOD_TASKS_GET_LIST, &tmp_error,
                        "auth_token", rtm->priv->auth_token,
                        "list_id", list_id,
                        "filter", filter,
                        "last_sync", last_sync,
                        NULL);
        }
        if (tmp_error != NULL) {
                g_propagate_error (error, tmp_error);
                return NULL;
        }

        for (node = rest_xml_node_find (root, "list"); node; node = node->next) {
                task_list_id = rest_xml_node_get_attr (node, "id");
                for (node2 = rest_xml_node_find (node, "taskseries"); node2; node2 = node2->next) {
                        task = rtm_task_new ();
                        rtm_task_load_data (task, node2, task_list_id);
                        list = g_list_append (list, task);
                }
        }

        rest_xml_node_unref (root);

        return list;
}

/**
 * rtm_glib_lists_get_list:
 * @rtm: a #RtmGlib object already authenticated.
 * @error: location to store #GError or %NULL.
 *
 * Gets the list of lists.
 *
 * Returns: A #GList of #RtmList objects.
 **/
GList *
rtm_glib_lists_get_list (RtmGlib *rtm, GError **error)
{
        g_return_val_if_fail (rtm != NULL, NULL);
        g_return_val_if_fail (rtm->priv->auth_token != NULL, NULL);

        RestXmlNode *root, *node;
        GList *glist = NULL;
        RtmList *rtmlist;
        GError *tmp_error = NULL;

        g_debug ("rtm_glib_lists_get_list");

        root = rtm_glib_call_method (rtm,
                RTM_METHOD_LISTS_GET_LIST, &tmp_error,
                "auth_token", rtm->priv->auth_token,
                NULL);
        if (tmp_error != NULL) {
                g_propagate_error (error, tmp_error);
                return NULL;
        }

        for (node = rest_xml_node_find (root, "list"); node; node = node->next) {
                rtmlist = rtm_list_new ();
                rtm_list_load_data (rtmlist, node);
                glist = g_list_append (glist, rtmlist);
        }

        rest_xml_node_unref (root);

        return glist;
}

/**
 * rtm_glib_timelines_create:
 * @rtm: a #RtmGlib object already authenticated.
 * @error: location to store #GError or %NULL.
 *
 * Gets a new timeline.
 *
 * Returns: A timeline.
 **/
gchar *
rtm_glib_timelines_create (RtmGlib *rtm, GError **error)
{
        g_return_val_if_fail (rtm != NULL, NULL);
        g_return_val_if_fail (rtm->priv->auth_token != NULL, NULL);

        RestXmlNode *root, *node;
        gchar *timeline;
        GError *tmp_error = NULL;

        g_debug ("rtm_glib_timelines_create");

        root = rtm_glib_call_method (rtm,
                RTM_METHOD_TIMELINES_CREATE, &tmp_error,
                "auth_token", rtm->priv->auth_token,
                NULL);
        if (tmp_error != NULL) {
                g_propagate_error (error, tmp_error);
                return NULL;
        }

        node = rest_xml_node_find (root, "timeline");
        timeline = g_strdup (node->content);
        g_debug ("timeline: %s", timeline);

        rest_xml_node_unref (root);

        return timeline;
}

/**
 * rtm_glib_tasks_add:
 * @rtm: a #RtmGlib object already authenticated.
 * @timeline: the timeline within which to run a method.
 * @task_name: the desired task name.
 * @list_id: %NULL or the id of the list to perform an action on.
 * @parse: %FALSE or %TRUE to specify whether to process name using Smart Add.
 * @error: location to store #GError or %NULL.
 *
 * Adds a task to the list specified by @list_id or the default list if
 * @list_id is %NULL.
 *
 * Returns: A #RtmTask with the data of the task added.
 **/
RtmTask *
rtm_glib_tasks_add (RtmGlib *rtm, gchar* timeline, gchar *task_name,
                    gchar *list_id, gboolean parse, GError **error)
{
        g_return_val_if_fail (rtm != NULL, NULL);
        g_return_val_if_fail (rtm->priv->auth_token != NULL, NULL);
        g_return_val_if_fail (timeline != NULL, NULL);
        g_return_val_if_fail (task_name != NULL, NULL);

        RestXmlNode *root, *node;
        const gchar *task_list_id;
        RtmTask *task;
        GError *tmp_error = NULL;
        gchar *parse_smart_add = "0";

        g_debug ("rtm_glib_tasks_add");

        if (parse) {
                parse_smart_add = "1";
        }

        if (list_id == NULL) {
                root = rtm_glib_call_method (
                        rtm,
                        RTM_METHOD_TASKS_ADD, &tmp_error,
                        "auth_token", rtm->priv->auth_token,
                        "timeline", timeline,
                        "name", task_name,
                        "parse", parse_smart_add,
                        NULL);
        } else {
                root = rtm_glib_call_method (
                        rtm,
                        RTM_METHOD_TASKS_ADD, &tmp_error,
                        "auth_token", rtm->priv->auth_token,
                        "timeline", timeline,
                        "name", task_name,
                        "list_id", list_id,
                        "parse", parse_smart_add,
                        NULL);
        }
        if (tmp_error != NULL) {
                g_propagate_error (error, tmp_error);
                return NULL;
        }

        node = rest_xml_node_find (root, "list");
        task_list_id = rest_xml_node_get_attr (node, "id");

        node = rest_xml_node_find (node, "taskseries");
        task = rtm_task_new ();
        rtm_task_load_data (task, node, task_list_id);

        rest_xml_node_unref (root);

        return task;
}

/**
 * rtm_glib_transactions_undo:
 * @rtm: a #RtmGlib object already authenticated.
 * @timeline: the timeline within which to run a method.
 * @transaction_id: the id of transaction within a timeline.
 * @error: location to store #GError or %NULL.
 *
 * Reverts the affects of an action.
 *
 * Returns: %TRUE if the transcation is undone successfuly.
 **/
gboolean
rtm_glib_transactions_undo (RtmGlib *rtm, gchar *timeline,
                            gchar* transaction_id, GError **error)
{
        g_return_val_if_fail (rtm != NULL, FALSE);
        g_return_val_if_fail (rtm->priv->auth_token != NULL, FALSE);
        g_return_val_if_fail (timeline != NULL, FALSE);
        g_return_val_if_fail (transaction_id != NULL, FALSE);

        RestXmlNode *root;
        GError *tmp_error = NULL;

        g_debug ("rtm_glib_transactions_undo");

        root = rtm_glib_call_method (rtm,
                RTM_METHOD_TRANSACTIONS_UNDO, &tmp_error,
                "auth_token", rtm->priv->auth_token,
                "timeline", timeline,
                "transaction_id", transaction_id,
                NULL);
        if (tmp_error != NULL) {
                g_propagate_error (error, tmp_error);
                return FALSE;
        }

        rest_xml_node_unref (root);

        return TRUE;
}

/**
 * rtm_glib_tasks_delete:
 * @rtm: a #RtmGlib object already authenticated.
 * @timeline: the timeline within which to run a method.
 * @task: a #RtmTask to be deleted.
 * @error: location to store #GError or %NULL.
 *
 * Removes a task.
 *
 * Returns: The transaction identifier or %NULL if it fails.
 **/
gchar *
rtm_glib_tasks_delete (RtmGlib *rtm, gchar* timeline, RtmTask *task,
                       GError **error)
{
        g_return_val_if_fail (rtm != NULL, NULL);
        g_return_val_if_fail (rtm->priv->auth_token != NULL, NULL);
        g_return_val_if_fail (timeline != NULL, NULL);
        g_return_val_if_fail (task != NULL, NULL);

        RestXmlNode *root, *node;
        gchar *transaction_id;
        GError *tmp_error = NULL;

        g_debug ("rtm_glib_tasks_delete");

        root = rtm_glib_call_method (
                rtm,
                RTM_METHOD_TASKS_DELETE, &tmp_error,
                "auth_token", rtm->priv->auth_token,
                "timeline", timeline,
                "list_id", rtm_task_get_list_id (task),
                "taskseries_id", rtm_task_get_taskseries_id (task),
                "task_id", rtm_task_get_id (task),
                NULL);
        if (tmp_error != NULL) {
                g_propagate_error (error, tmp_error);
                return NULL;
        }

        node = rest_xml_node_find (root, "transaction");
        transaction_id = g_strdup (rest_xml_node_get_attr (node, "id"));
        g_debug ("transaction_id: %s", transaction_id);

        rest_xml_node_unref (root);

        return transaction_id;
}

/**
 * rtm_glib_tasks_set_name:
 * @rtm: a #RtmGlib object already authenticated.
 * @timeline: the timeline within which to run a method.
 * @task: a #RtmTask to be modified with the new name.
 * @name: the desired name.
 * @error: location to store #GError or %NULL.
 *
 * Renames a task.
 *
 * Returns: The transaction identifier or %NULL if it fails.
 **/
gchar *
rtm_glib_tasks_set_name (RtmGlib *rtm, gchar* timeline, RtmTask *task,
                         gchar *name, GError **error)
{
        g_return_val_if_fail (rtm != NULL, NULL);
        g_return_val_if_fail (rtm->priv->auth_token != NULL, NULL);
        g_return_val_if_fail (timeline != NULL, NULL);
        g_return_val_if_fail (task != NULL, NULL);
        g_return_val_if_fail (name != NULL, NULL);

        RestXmlNode *root, *node;
        gchar *transaction_id;
        GError *tmp_error = NULL;

        g_debug ("rtm_glib_tasks_set_name");

        root = rtm_glib_call_method (
                rtm,
                RTM_METHOD_TASKS_SET_NAME, &tmp_error,
                "auth_token", rtm->priv->auth_token,
                "timeline", timeline,
                "list_id", rtm_task_get_list_id (task),
                "taskseries_id", rtm_task_get_taskseries_id (task),
                "task_id", rtm_task_get_id (task),
                "name", name,
                NULL);
        if (tmp_error != NULL) {
                g_propagate_error (error, tmp_error);
                return NULL;
        }

        node = rest_xml_node_find (root, "transaction");
        transaction_id = g_strdup (rest_xml_node_get_attr (node, "id"));
        g_debug ("transaction_id: %s", transaction_id);

        rest_xml_node_unref (root);

        return transaction_id;
}

/**
 * rtm_glib_lists_add:
 * @rtm: a #RtmGlib object already authenticated.
 * @timeline: the timeline within which to run a method.
 * @list_name: the desired list name.
 * @error: location to store #GError or %NULL.
 *
 * Adds a new list.
 *
 * Returns: A #RtmList with the data of the new list added.
 **/
RtmList *
rtm_glib_lists_add (RtmGlib *rtm, gchar* timeline, gchar *list_name,
                    GError **error)
{
        g_return_val_if_fail (rtm != NULL, NULL);
        g_return_val_if_fail (rtm->priv->auth_token != NULL, NULL);
        g_return_val_if_fail (timeline != NULL, NULL);
        g_return_val_if_fail (list_name != NULL, NULL);

        RestXmlNode *root, *node;
        RtmList *list;
        GError *tmp_error = NULL;

        g_debug ("rtm_glib_lists_add");

        root = rtm_glib_call_method (
                rtm,
                RTM_METHOD_LISTS_ADD, &tmp_error,
                "auth_token", rtm->priv->auth_token,
                "timeline", timeline,
                "name", list_name,
                NULL);
        if (tmp_error != NULL) {
                g_propagate_error (error, tmp_error);
                return NULL;
        }

        node = rest_xml_node_find (root, "list");

        list = rtm_list_new ();
        rtm_list_load_data (list, node);

        rest_xml_node_unref (root);

        return list;
}

/**
 * rtm_glib_lists_delete:
 * @rtm: a #RtmGlib object already authenticated.
 * @timeline: the timeline within which to run a method.
 * @list: a #RtmList to be deleted.
 * @error: location to store #GError or %NULL.
 *
 * Removes a list.
 *
 * Returns: The transaction identifier or %NULL if it fails.
 **/
gchar *
rtm_glib_lists_delete (RtmGlib *rtm, gchar* timeline, RtmList *list,
                       GError **error)
{
        g_return_val_if_fail (rtm != NULL, NULL);
        g_return_val_if_fail (rtm->priv->auth_token != NULL, NULL);
        g_return_val_if_fail (timeline != NULL, NULL);
        g_return_val_if_fail (list != NULL, NULL);

        RestXmlNode *root, *node;
        gchar *transaction_id;
        GError *tmp_error = NULL;

        g_debug ("rtm_glib_lists_delete");

        root = rtm_glib_call_method (
                rtm,
                RTM_METHOD_LISTS_DELETE, &tmp_error,
                "auth_token", rtm->priv->auth_token,
                "timeline", timeline,
                "list_id", rtm_list_get_id (list),
                NULL);
        if (tmp_error != NULL) {
                g_propagate_error (error, tmp_error);
                return NULL;
        }

        node = rest_xml_node_find (root, "transaction");
        transaction_id = g_strdup (rest_xml_node_get_attr (node, "id"));
        g_debug ("transaction_id: %s", transaction_id);

        rest_xml_node_unref (root);

        return transaction_id;
}

/**
 * rtm_glib_lists_set_name:
 * @rtm: a #RtmGlib object already authenticated.
 * @timeline: the timeline within which to run a method.
 * @list: a #RtmList to be modified with the new name.
 * @name: the desired name.
 * @error: location to store #GError or %NULL.
 *
 * Renames a list. The new name can not be %Inbox or %Sent.
 *
 * Returns: The transaction identifier or %NULL if it fails.
 **/
gchar *
rtm_glib_lists_set_name (RtmGlib *rtm, gchar* timeline, RtmList *list,
                         gchar *name, GError **error)
{
        g_return_val_if_fail (rtm != NULL, NULL);
        g_return_val_if_fail (rtm->priv->auth_token != NULL, NULL);
        g_return_val_if_fail (timeline != NULL, NULL);
        g_return_val_if_fail (list != NULL, NULL);
        g_return_val_if_fail (name != NULL, NULL);
        g_return_val_if_fail (
                (g_strcmp0 (name, "Inbox") != 0) ||
                (g_strcmp0 (name, "Sent") != 0),
                NULL);

        RestXmlNode *root, *node;
        gchar *transaction_id;
        GError *tmp_error = NULL;

        g_debug ("rtm_glib_lists_set_name");

        root = rtm_glib_call_method (
                rtm,
                RTM_METHOD_LISTS_SET_NAME, &tmp_error,
                "auth_token", rtm->priv->auth_token,
                "timeline", timeline,
                "list_id", rtm_list_get_id (list),
                "name", name,
                NULL);
        if (tmp_error != NULL) {
                g_propagate_error (error, tmp_error);
                return NULL;
        }

        node = rest_xml_node_find (root, "transaction");
        transaction_id = g_strdup (rest_xml_node_get_attr (node, "id"));
        g_debug ("transaction_id: %s", transaction_id);

        rest_xml_node_unref (root);

        return transaction_id;
}
