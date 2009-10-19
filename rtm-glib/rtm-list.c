/*
 * rtm-list.c: Represents a list of tasks
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
 * SECTION:rtm-list
 * @short_description: A class that represents a list of tasks
 *
 * #RtmList represents a list of tasks from Remeber The Milk.
 */

#include <rtm-list.h>
#include <rtm-util.h>
#include <rtm-error.h>

#define RTM_LIST_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE (        \
                                           (obj), RTM_TYPE_LIST, RtmListPrivate))

struct _RtmListPrivate {
        gchar *id;
        gchar *name;
        GList *tasks;
};

enum {
        PROP_0,

        PROP_ID,
        PROP_NAME,
};

G_DEFINE_TYPE (RtmList, rtm_list, G_TYPE_OBJECT);

static void
rtm_list_get_property (GObject *gobject, guint prop_id, GValue *value,
                       GParamSpec *pspec)
{
        RtmListPrivate *priv = RTM_LIST_GET_PRIVATE (RTM_LIST (gobject));

        switch (prop_id) {
        case PROP_ID:
                g_value_set_string (value, priv->id);
                break;

        case PROP_NAME:
                g_value_set_string (value, priv->name);
                break;

        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id,
                                                   pspec);
                break;
        }
}

static void
rtm_list_set_property (GObject *gobject, guint prop_id, const GValue *value,
                       GParamSpec *pspec)
{
        RtmListPrivate *priv = RTM_LIST_GET_PRIVATE (RTM_LIST (gobject));

        switch (prop_id) {
        case PROP_ID:
                g_free (priv->id);
                priv->id = g_value_dup_string (value);
                break;

        case PROP_NAME:
                g_free (priv->name);
                priv->name = g_value_dup_string (value);
                break;

        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id,
                                                   pspec);
                break;
        }
}

static void
rtm_list_dispose (GObject *gobject)
{
        RtmListPrivate *priv = RTM_LIST_GET_PRIVATE (RTM_LIST (gobject));

        g_list_free (priv->tasks);

        G_OBJECT_CLASS (rtm_list_parent_class)->dispose (gobject);
}

static void
rtm_list_finalize (GObject *gobject)
{
        RtmListPrivate *priv = RTM_LIST_GET_PRIVATE (RTM_LIST (gobject));

        g_free (priv->id);
        g_free (priv->name);

        G_OBJECT_CLASS (rtm_list_parent_class)->finalize (gobject);
}

static void
rtm_list_class_init (RtmListClass *klass)
{
        GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

        g_type_class_add_private (klass, sizeof (RtmListPrivate));

        gobject_class->get_property = rtm_list_get_property;
        gobject_class->set_property = rtm_list_set_property;
        gobject_class->finalize = rtm_list_finalize;
        gobject_class->dispose = rtm_list_dispose;

        g_object_class_install_property (
                gobject_class,
                PROP_ID,
                g_param_spec_string (
                        "id",
                        "ID",
                        "The identifier of the list",
                        NULL,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_NAME,
                g_param_spec_string (
                        "name",
                        "Name",
                        "The name of the list",
                        NULL,
                        G_PARAM_READWRITE));

}

static void
rtm_list_init (RtmList *list)
{
        list->priv = RTM_LIST_GET_PRIVATE (list);
        list->priv->tasks = NULL;
}

/**
 * rtm_list_new:
 *
 * Creates a new instance of this class.
 *
 * Returns: a new #RtmList object.
 */
RtmList *
rtm_list_new ()
{
        return g_object_new (RTM_TYPE_LIST, NULL);
}

/**
 * rtm_list_get_id:
 * @list: a #RtmList.
 *
 * Gets the #RtmList:id property of the object.
 *
 * Returns: the ID of the list.
 */
gchar *
rtm_list_get_id (RtmList *list)
{
        g_return_val_if_fail (list != NULL, NULL);

        return list->priv->id;
}

/**
 * rtm_list_set_id:
 * @list: a #RtmList.
 * @id: an ID for the #RtmList.
 *
 * Sets the #RtmList:id property of the object.
 *
 * Returns: %TRUE if ID is set.
 */
gboolean
rtm_list_set_id (RtmList *list, gchar* id)
{
        g_return_val_if_fail (list != NULL, FALSE);
        g_return_val_if_fail (id != NULL, FALSE);

        list->priv->id = g_strdup(id);
        return TRUE;
}

/**
 * rtm_list_get_name:
 * @list: a #RtmList.
 *
 * Gets the #RtmList:name property of the object.
 *
 * Returns: the name of the list.
 */
gchar *
rtm_list_get_name (RtmList *list)
{
        g_return_val_if_fail (list != NULL, NULL);

        return list->priv->name;
}

/**
 * rtm_list_set_name:
 * @list: a #RtmList.
 * @name: a name for the #RtmList.
 *
 * Sets the #RtmList:name property of the object.
 *
 * Returns: %TRUE if name is set.
 */
gboolean
rtm_list_set_name (RtmList *list, gchar* name)
{
        g_return_val_if_fail (list != NULL, FALSE);
        g_return_val_if_fail (name != NULL, FALSE);

        list->priv->name = g_strdup(name);
        return TRUE;
}

/**
 * rtm_list_load_data:
 * @list: a #RtmList.
 * @node: a #RestXmlNode with the #RtmList data.
 *
 * Sets the data of the #RtmList object from the #RestXmlNode.
 */
void
rtm_list_load_data (RtmList *list, RestXmlNode *node)
{
        g_return_if_fail (list != NULL);
        g_return_if_fail (node != NULL);

        list->priv->id = g_strdup (rest_xml_node_get_attr (node, "id"));
        list->priv->name = g_strdup (rest_xml_node_get_attr (node, "name"));
}

/**
 * rtm_list_to_string:
 * @list: a #RtmList.
 *
 * Generates a printable string which represents a #RtmList object.
 *
 * Returns: A string representing the #RtmList.
 */
gchar *
rtm_list_to_string (RtmList *list)
{
        g_return_val_if_fail (list != NULL, NULL);

        GList *item;
        RtmTask *task;
        gchar *string;

        string = g_strconcat (
                "RtmList: [\n",
                "  ID: ", rtm_util_string_or_null (list->priv->id), "\n",
                "  Name: ", rtm_util_string_or_null (list->priv->name), "\n",
                "]\n",
                NULL);

        for (item = list->priv->tasks; item; item = g_list_next (item)) {
                task = (RtmTask *) item->data;
                string = g_strconcat (string, rtm_task_to_string (task), NULL);
        }

        return string;
}

/**
 * rtm_list_get_tasks:
 * @list: a #RtmList.
 *
 * Gets the tasks of a #RtmList. It returns a copy of the list, use
 * rtm_list_add_task() and rtm_list_remove_task() to manipulate this list.
 *
 * Returns: A #GList of #RtmTask.
 */
GList *
rtm_list_get_tasks (RtmList *list)
{
        g_return_val_if_fail (list != NULL, NULL);

        return g_list_copy (list->priv->tasks);
}

/**
 * rtm_list_find_task:
 * @list: a #RtmList.
 * @task_id: a task ID.
 *
 * Finds a task in the #RtmList.
 *
 * Returns: The #RtmTask found or %NULL.
 */
RtmTask *
rtm_list_find_task (RtmList *list, gchar *task_id)
{
        g_return_val_if_fail (list != NULL, NULL);
        g_return_val_if_fail (task_id != NULL, NULL);

        GList *item;
        RtmTask *task;

        for (item = list->priv->tasks; item; item = g_list_next (item)) {
                task = (RtmTask *) item->data;
                if (g_strcmp0 (rtm_task_get_id (task), task_id) == 0) {
                        return task;
                }
        }

        return NULL;
}

/**
 * rtm_list_add_task:
 * @list: a #RtmList.
 * @task: a #RtmTask.
 * @error: location to store #GError or %NULL.
 *
 * Adds a task to the current #RtmList. It also sets the #RtmTask:list_id
 * property to match with the #RtmList:id of the current list.
 *
 * Returns: %TRUE if success.
 */
gboolean
rtm_list_add_task (RtmList *list, RtmTask *task, GError **error)
{
        g_return_val_if_fail (list != NULL, FALSE);
        g_return_val_if_fail (task != NULL, FALSE);

        RtmTask *existent_task;
        gchar *task_id;

        task_id = rtm_task_get_id (task);
        existent_task = rtm_list_find_task (list, task_id);

        if (existent_task != NULL) {
                g_set_error (
                        error,
                        RTM_ERROR_DOMAIN,
                        RTM_TASK_ALREADY_EXISTS,
                        "Task with ID \"%s\" already exists on this list",
                        task_id);
                return FALSE;
        }

        rtm_task_set_list_id (task, list->priv->id);
        list->priv->tasks = g_list_append (list->priv->tasks, task);
        return TRUE;
}

/**
 * rtm_list_remove_task:
 * @list: a #RtmList.
 * @task_id: a task ID.
 * @error: location to store #GError or %NULL.
 *
 * Removes a task from the current #RtmList.
 *
 * Returns: %TRUE if success.
 */
gboolean
rtm_list_remove_task (RtmList *list, gchar *task_id, GError **error)
{
        g_return_val_if_fail (list != NULL, FALSE);
        g_return_val_if_fail (task_id != NULL, FALSE);

        RtmTask *existent_task;

        existent_task = rtm_list_find_task (list, task_id);

        if (existent_task == NULL) {
                g_set_error (
                        error,
                        RTM_ERROR_DOMAIN,
                        RTM_TASK_NOT_FOUND,
                        "Task with ID \"%s\" was not found on this list",
                        task_id);
                return FALSE;
        }

        list->priv->tasks = g_list_remove (list->priv->tasks, existent_task);
        return TRUE;
}
