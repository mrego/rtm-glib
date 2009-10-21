/*
 * rtm-task.c: Represents a task
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
 * SECTION:rtm-task
 * @short_description: A class that represents a task
 *
 * #RtmTask represents a task from Remeber The Milk.
 */

#include <rtm-task.h>
#include <rtm-util.h>
#include <rtm-error.h>

#define RTM_TASK_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE (        \
                                           (obj), RTM_TYPE_TASK, RtmTaskPrivate))

struct _RtmTaskPrivate {
        gchar *id;
        gchar *taskseries_id;
        gchar *list_id;
        gchar *name;
        gchar *priority;
        gchar *url;
        gchar *location_id;
        GTimeVal *due_date;
        gboolean has_due_time;
        GTimeVal *added_date;
        GTimeVal *completed_date;
        GTimeVal *deleted_date;
        GList *tags;
};

enum {
        PROP_0,

        PROP_ID,
        PROP_TASKSERIES_ID,
        PROP_LIST_ID,
        PROP_NAME,
        PROP_PRIORITY,
        PROP_URL,
        PROP_LOCATION_ID,
        PROP_HAS_DUE_TIME,
};

G_DEFINE_TYPE (RtmTask, rtm_task, G_TYPE_OBJECT);

static void
rtm_task_get_property (GObject *gobject, guint prop_id, GValue *value,
                       GParamSpec *pspec)
{
        RtmTaskPrivate *priv = RTM_TASK_GET_PRIVATE (RTM_TASK (gobject));

        switch (prop_id) {
        case PROP_ID:
                g_value_set_string (value, priv->id);
                break;

        case PROP_TASKSERIES_ID:
                g_value_set_string (value, priv->taskseries_id);
                break;

        case PROP_LIST_ID:
                g_value_set_string (value, priv->list_id);
                break;

        case PROP_NAME:
                g_value_set_string (value, priv->name);
                break;

        case PROP_PRIORITY:
                g_value_set_string (value, priv->priority);
                break;

        case PROP_URL:
                g_value_set_string (value, priv->url);
                break;

        case PROP_LOCATION_ID:
                g_value_set_string (value, priv->location_id);
                break;

        case PROP_HAS_DUE_TIME:
                g_value_set_boolean (value, priv->has_due_time);
                break;

        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id,
                                                   pspec);
                break;
        }
}

static void
rtm_task_set_property (GObject *gobject, guint prop_id, const GValue *value,
                       GParamSpec *pspec)
{
        RtmTaskPrivate *priv = RTM_TASK_GET_PRIVATE (RTM_TASK (gobject));

        switch (prop_id) {
        case PROP_ID:
                g_free (priv->id);
                priv->id = g_value_dup_string (value);
                break;

        case PROP_TASKSERIES_ID:
                g_free (priv->taskseries_id);
                priv->taskseries_id = g_value_dup_string (value);
                break;

        case PROP_LIST_ID:
                g_free (priv->list_id);
                priv->list_id = g_value_dup_string (value);
                break;

        case PROP_NAME:
                g_free (priv->name);
                priv->name = g_value_dup_string (value);
                break;

        case PROP_PRIORITY:
                g_free (priv->priority);
                priv->priority = g_value_dup_string (value);
                break;

        case PROP_URL:
                g_free (priv->url);
                priv->url = g_value_dup_string (value);
                break;

        case PROP_LOCATION_ID:
                g_free (priv->location_id);
                priv->location_id = g_value_dup_string (value);
                break;

        case PROP_HAS_DUE_TIME:
                priv->has_due_time = g_value_get_boolean (value);
                break;

        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id,
                                                   pspec);
                break;
        }
}

static void
rtm_task_dispose (GObject *gobject)
{
        RtmTaskPrivate *priv = RTM_TASK_GET_PRIVATE (RTM_TASK (gobject));

        g_list_free (priv->tags);

        G_OBJECT_CLASS (rtm_task_parent_class)->dispose (gobject);
}

static void
rtm_task_finalize (GObject *gobject)
{
        RtmTaskPrivate *priv = RTM_TASK_GET_PRIVATE (RTM_TASK (gobject));

        g_free (priv->id);
        g_free (priv->taskseries_id);
        g_free (priv->list_id);
        g_free (priv->name);
        g_free (priv->priority);
        g_free (priv->url);
        g_free (priv->location_id);
        if (priv->due_date) {
                g_free (priv->due_date);
        }
        if (priv->added_date) {
                g_free (priv->added_date);
        }
        if (priv->completed_date) {
                g_free (priv->completed_date);
        }
        if (priv->deleted_date) {
                g_free (priv->deleted_date);
        }

        G_OBJECT_CLASS (rtm_task_parent_class)->finalize (gobject);
}

static void
rtm_task_class_init (RtmTaskClass *klass)
{
        GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

        g_type_class_add_private (klass, sizeof (RtmTaskPrivate));

        gobject_class->get_property = rtm_task_get_property;
        gobject_class->set_property = rtm_task_set_property;
        gobject_class->finalize = rtm_task_finalize;
        gobject_class->dispose = rtm_task_dispose;

        g_object_class_install_property (
                gobject_class,
                PROP_ID,
                g_param_spec_string (
                        "id",
                        "ID",
                        "The identifier of the task",
                        NULL,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_TASKSERIES_ID,
                g_param_spec_string (
                        "taskseries_id",
                        "Taskseries ID",
                        "The identifier of the Taskseries, an ID used \
                                by Remember The Milk which identifies a Task together with the task ID",
                        NULL,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_LIST_ID,
                g_param_spec_string (
                        "list_id",
                        "List ID",
                        "The identifier of the List to which the task \
                                belongs",
                        NULL,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_NAME,
                g_param_spec_string (
                        "name",
                        "Name",
                        "The name of the task",
                        NULL,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_PRIORITY,
                g_param_spec_string (
                        "priority",
                        "Priority",
                        "The priority of the task",
                        NULL,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_URL,
                g_param_spec_string (
                        "url",
                        "URL",
                        "The URL associated with a task",
                        NULL,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_LOCATION_ID,
                g_param_spec_string (
                        "location_id",
                        "Location ID",
                        "The location identifier of the task",
                        NULL,
                        G_PARAM_READWRITE));

        g_object_class_install_property (
                gobject_class,
                PROP_HAS_DUE_TIME,
                g_param_spec_boolean (
                        "has_due_time",
                        "Has due time",
                        "    Specifies whether the due date has a due time",
                        FALSE,
                        G_PARAM_READWRITE));

}

static void
rtm_task_init (RtmTask *task)
{
        task->priv = RTM_TASK_GET_PRIVATE (task);
        task->priv->tags = NULL;
}

/**
 * rtm_task_new:
 *
 * Creates a new instance of this class.
 *
 * Returns: a new #RtmTask object.
 */
RtmTask *
rtm_task_new ()
{
        return g_object_new (RTM_TYPE_TASK, NULL);
}

/**
 * rtm_task_get_id:
 * @task: a #RtmTask.
 *
 * Gets the #RtmTask:id property of the object.
 *
 * Returns: the ID of the task.
 */
gchar *
rtm_task_get_id (RtmTask *task)
{
        g_return_val_if_fail (task != NULL, NULL);

        return task->priv->id;
}

/**
 * rtm_task_set_id:
 * @task: a #RtmTask.
 * @id: an ID for the #RtmTask.
 *
 * Sets the #RtmTask:id property of the object.
 *
 * Returns: %TRUE if ID is set.
 */
gboolean
rtm_task_set_id (RtmTask *task, gchar* id)
{
        g_return_val_if_fail (task != NULL, FALSE);
        g_return_val_if_fail (id != NULL, FALSE);

        task->priv->id = g_strdup (id);
        return TRUE;
}

/**
 * rtm_task_get_taskseries_id:
 * @task: a #RtmTask.
 *
 * Gets the #RtmTask:taskseries_id property of the object.
 *
 * Returns: the Taskseries ID of the task.
 */
gchar *
rtm_task_get_taskseries_id (RtmTask *task)
{
        g_return_val_if_fail (task != NULL, NULL);

        return task->priv->taskseries_id;
}

/**
 * rtm_task_set_taskseries_id:
 * @task: a #RtmTask.
 * @taskseries_id: an Taskseries ID for the #RtmTask.
 *
 * Sets the #RtmTask:taskseries_id property of the object.
 *
 * Returns: %TRUE if Taskseries ID is set.
 */
gboolean
rtm_task_set_taskseries_id (RtmTask *task, gchar* taskseries_id)
{
        g_return_val_if_fail (task != NULL, FALSE);
        g_return_val_if_fail (taskseries_id != NULL, FALSE);

        task->priv->taskseries_id = g_strdup(taskseries_id);
        return TRUE;
}

/**
 * rtm_task_get_list_id:
 * @task: a #RtmTask.
 *
 * Gets the #RtmTask:list_id property of the object.
 *
 * Returns: the List ID of the task.
 */
gchar *
rtm_task_get_list_id (RtmTask *task)
{
        g_return_val_if_fail (task != NULL, NULL);

        return task->priv->list_id;
}

/**
 * rtm_task_set_list_id:
 * @task: a #RtmTask.
 * @list_id: an List ID for the #RtmTask.
 *
 * Sets the #RtmTask:list_id property of the object.
 *
 * Returns: %TRUE if List ID is set.
 */
gboolean
rtm_task_set_list_id (RtmTask *task, gchar* list_id)
{
        g_return_val_if_fail (task != NULL, FALSE);
        g_return_val_if_fail (list_id != NULL, FALSE);

        task->priv->list_id = g_strdup (list_id);
        return TRUE;
}

/**
 * rtm_task_get_name:
 * @task: a #RtmTask.
 *
 * Gets the #RtmTask:name property of the object.
 *
 * Returns: the name of the task.
 */
gchar *
rtm_task_get_name (RtmTask *task)
{
        g_return_val_if_fail (task != NULL, NULL);

        return task->priv->name;
}

/**
 * rtm_task_set_name:
 * @task: a #RtmTask.
 * @name: a name for the #RtmTask.
 *
 * Sets the #RtmTask:name property of the object.
 *
 * Returns: %TRUE if name is set.
 */
gboolean
rtm_task_set_name (RtmTask *task, gchar* name)
{
        g_return_val_if_fail (task != NULL, FALSE);
        g_return_val_if_fail (name != NULL, FALSE);

        task->priv->name = g_strdup (name);
        return TRUE;
}

/**
 * rtm_task_get_priority:
 * @task: a #RtmTask.
 *
 * Gets the #RtmTask:priority property of the object.
 *
 * Returns: the priority of the task.
 */
gchar *
rtm_task_get_priority (RtmTask *task)
{
        g_return_val_if_fail (task != NULL, NULL);

        return task->priv->priority;
}

/**
 * rtm_task_set_priority:
 * @task: a #RtmTask.
 * @priority: a priority for the #RtmTask.
 *
 * Sets the #RtmTask:priority property of the object. The possible valid
 * values are %N, %1, %2 and %3.
 *
 * Returns: %TRUE if priority is set.
 */
gboolean
rtm_task_set_priority (RtmTask *task, gchar* priority)
{
        g_return_val_if_fail (task != NULL, FALSE);
        g_return_val_if_fail (
                (g_strcmp0 (priority, "N") == 0) ||
                (g_strcmp0 (priority, "1") == 0) ||
                (g_strcmp0 (priority, "2") == 0) ||
                (g_strcmp0 (priority, "3") == 0),
                FALSE);

        task->priv->priority = g_strdup (priority);
        return TRUE;
}

/**
 * rtm_task_load_data:
 * @task: a #RtmTask.
 * @node: a #RestXmlNode with the #RtmTask data.
 * @list_id: the list ID which belongs the #RtmTask.
 *
 * Sets the data of the #RtmTask object from the #RestXmlNode.
 */
void
rtm_task_load_data (RtmTask *task, RestXmlNode *node, const gchar *list_id)
{
        g_return_if_fail (task != NULL);
        g_return_if_fail (node != NULL);
        g_return_if_fail (list_id != NULL);

        RestXmlNode *node_tags, *node_tmp;
        gchar *tag;
        const gchar *due, *added, *completed, *deleted;
        GTimeVal due_date, added_date, completed_date, deleted_date;

        task->priv->taskseries_id = g_strdup (rest_xml_node_get_attr (node, "id"));
        task->priv->name = g_strdup (rest_xml_node_get_attr (node, "name"));
        task->priv->url = g_strdup (rest_xml_node_get_attr (node, "url"));
        task->priv->location_id = g_strdup (rest_xml_node_get_attr (node, "location_id"));

        node_tags = rest_xml_node_find (node, "tags");
        for (node_tmp = rest_xml_node_find (node_tags, "tag"); node_tmp;
             node_tmp = node_tmp->next) {
                tag = g_strdup (node_tmp->content);
                rtm_task_add_tag (task, tag, NULL);
        }

        node_tmp = rest_xml_node_find (node, "task");
        task->priv->id = g_strdup (rest_xml_node_get_attr (node_tmp, "id"));
        task->priv->priority = g_strdup (rest_xml_node_get_attr (node_tmp, "priority"));

        due = rest_xml_node_get_attr (node_tmp, "due");
        if (due && (g_strcmp0 (due, "") != 0)) {
                g_time_val_from_iso8601 (due, &due_date);
                task->priv->due_date = rtm_util_g_time_val_dup (&due_date);
        }

        task->priv->has_due_time = (g_strcmp0 (rest_xml_node_get_attr (node_tmp, "has_due_time"), "1") == 0);

        added = rest_xml_node_get_attr (node_tmp, "added");
        if (added && (g_strcmp0 (added, "") != 0)) {
                g_time_val_from_iso8601 (added, &added_date);
                task->priv->added_date = rtm_util_g_time_val_dup (&added_date);
        }

        completed = rest_xml_node_get_attr (node_tmp, "completed");
        if (completed && (g_strcmp0 (completed, "") != 0)) {
                g_time_val_from_iso8601 (completed, &completed_date);
                task->priv->completed_date = rtm_util_g_time_val_dup (&completed_date);
        }

        deleted = rest_xml_node_get_attr (node_tmp, "deleted");
        if (deleted && (g_strcmp0 (deleted, "") != 0)) {
                g_time_val_from_iso8601 (deleted, &deleted_date);
                task->priv->deleted_date = rtm_util_g_time_val_dup (&deleted_date);
        }

        task->priv->list_id = g_strdup (list_id);
}

/**
 * rtm_task_to_string:
 * @task: a #RtmTask.
 *
 * Generates a printable string which represents a #RtmTask object.
 *
 * Returns: A string representing the #RtmTask.
 */
gchar *
rtm_task_to_string (RtmTask *task)
{
        g_return_val_if_fail (task != NULL, NULL);

        GList *item;
        gchar **tags;
        gint i = 0;

        tags = g_new0 (gchar*, g_list_length (task->priv->tags) + 1);
        for (item = task->priv->tags; item; item = g_list_next (item)) {
                tags[i] = (gchar *) item->data;
                i++;
        }
        tags[i] = NULL;

        gchar *string = g_strconcat (
                "RtmTask: [\n",
                "  ID: ", rtm_util_string_or_null (task->priv->id), "\n",
                "  Taskseries ID: ",
                rtm_util_string_or_null (task->priv->taskseries_id), "\n",
                "  List ID: ",
                rtm_util_string_or_null (task->priv->list_id), "\n",
                "  Name: ", rtm_util_string_or_null (task->priv->name), "\n",
                "  Priority: ", rtm_util_string_or_null (task->priv->priority), "\n",
                "  URL: ", rtm_util_string_or_null (task->priv->url), "\n",
                "  Tags: ", g_strjoinv (", ", tags), "\n",
                "  Location ID: ", rtm_util_string_or_null (task->priv->location_id), "\n",
                "  Due date: ", rtm_util_g_time_val_to_string (task->priv->due_date), "\n",
                "  Has due time: ", rtm_util_gboolean_to_string (task->priv->has_due_time), "\n",
                "  Added date: ", rtm_util_g_time_val_to_string (task->priv->added_date), "\n",
                "  Completed date: ", rtm_util_g_time_val_to_string (task->priv->completed_date), "\n",
                "  Deleted date: ", rtm_util_g_time_val_to_string (task->priv->deleted_date), "\n",
                "]\n",
                NULL);

        g_strfreev (tags);

        return string;
}

/**
 * rtm_task_get_url:
 * @task: a #RtmTask.
 *
 * Gets the #RtmTask:url property of the object.
 *
 * Returns: the url of the task.
 */
gchar *
rtm_task_get_url (RtmTask *task)
{
        g_return_val_if_fail (task != NULL, NULL);

        return task->priv->url;
}

/**
 * rtm_task_set_url:
 * @task: a #RtmTask.
 * @url: a url for the #RtmTask.
 *
 * Sets the #RtmTask:url property of the object.
 *
 * Returns: %TRUE if url is set.
 */
gboolean
rtm_task_set_url (RtmTask *task, gchar* url)
{
        g_return_val_if_fail (task != NULL, FALSE);
        g_return_val_if_fail (url != NULL, FALSE);

        task->priv->url = g_strdup (url);
        return TRUE;
}

/**
 * rtm_task_get_location_id:
 * @task: a #RtmTask.
 *
 * Gets the #RtmTask:location_id property of the object.
 *
 * Returns: the location ID of the task.
 */
gchar *
rtm_task_get_location_id (RtmTask *task)
{
        g_return_val_if_fail (task != NULL, NULL);

        return task->priv->location_id;
}

/**
 * rtm_task_set_location_id:
 * @task: a #RtmTask.
 * @location_id: a location ID for the #RtmTask.
 *
 * Sets the #RtmTask:location_id property of the object.
 *
 * Returns: %TRUE if location ID is set.
 */
gboolean
rtm_task_set_location_id (RtmTask *task, gchar* location_id)
{
        g_return_val_if_fail (task != NULL, FALSE);
        g_return_val_if_fail (location_id != NULL, FALSE);

        task->priv->location_id = g_strdup (location_id);
        return TRUE;
}

/**
 * rtm_task_get_due_date:
 * @task: a #RtmTask.
 *
 * Gets the #RtmTask:due_date property of the object.
 *
 * Returns: the due date of the task.
 */
GTimeVal *
rtm_task_get_due_date (RtmTask *task)
{
        g_return_val_if_fail (task != NULL, NULL);

        return task->priv->due_date;
}

/**
 * rtm_task_set_due_date:
 * @task: a #RtmTask.
 * @due_date: a due date for the #RtmTask.
 *
 * Sets the #RtmTask:due_date property of the object.
 *
 * Returns: %TRUE if due_date is set.
 */
gboolean
rtm_task_set_due_date (RtmTask *task, GTimeVal *due_date)
{
        g_return_val_if_fail (task != NULL, FALSE);
        g_return_val_if_fail (due_date != NULL, FALSE);

        task->priv->due_date = rtm_util_g_time_val_dup (due_date);
        return TRUE;
}

/**
 * rtm_task_get_tags:
 * @list: a #RtmList.
 *
 * Gets the tags of a #RtmTask. It returns a copy of the list, use
 * rtm_task_add_tag() and rtm_task_remove_tag() to manipulate this list.
 *
 * Returns: A #GList of #gchar*.
 */
GList *
rtm_task_get_tags (RtmTask *task)
{
        g_return_val_if_fail (task != NULL, NULL);

        return g_list_copy (task->priv->tags);
}

/**
 * rtm_task_find_tag:
 * @task: a #RtmTask.
 * @tag: a tag.
 *
 * Finds a tag in the #RtmTask.
 *
 * Returns: The tag found or %NULL.
 */
gchar *
rtm_task_find_tag (RtmTask *task, gchar *tag)
{
        g_return_val_if_fail (task != NULL, NULL);
        g_return_val_if_fail (tag != NULL, NULL);

        GList *item;
        gchar *temp_tag;

        for (item = task->priv->tags; item; item = g_list_next (item)) {
                temp_tag = (gchar *) item->data;
                if (g_strcmp0 (temp_tag, tag) == 0) {
                        return tag;
                }
        }

        return NULL;
}

/**
 * rtm_task_add_tag:
 * @task: a #RtmTask.
 * @tag: a tag.
 * @error: location to store #GError or %NULL.
 *
 * Adds a tag to the current #RtmTask, if is not already assigned.
 *
 * Returns: %TRUE if success.
 */
gboolean
rtm_task_add_tag (RtmTask *task, gchar *tag, GError **error)
{
        g_return_val_if_fail (task != NULL, FALSE);
        g_return_val_if_fail (tag != NULL, FALSE);

        gchar *existent_tag;

        existent_tag = rtm_task_find_tag (task, tag);

        if (existent_tag != NULL) {
                g_set_error (
                        error,
                        RTM_ERROR_DOMAIN,
                        RTM_TAG_ALREADY_ASSIGNED,
                        "Tag \"%s\" already assigned to this task",
                        tag);
                return FALSE;
        }

        task->priv->tags = g_list_append (task->priv->tags, tag);
        return TRUE;
}

/**
 * rtm_task_remove_tag:
 * @task: a task.
 * @tag: a tag to be removed.
 * @error: location to store #GError or %NULL.
 *
 * Removes a tag from the current #RtmTask.
 *
 * Returns: %TRUE if success.
 */
gboolean
rtm_task_remove_tag (RtmTask *task, gchar *tag, GError **error)
{
        g_return_val_if_fail (task != NULL, FALSE);
        g_return_val_if_fail (tag != NULL, FALSE);

        gchar *existent_tag;

        existent_tag = rtm_task_find_tag (task, tag);

        if (existent_tag == NULL) {
                g_set_error (
                        error,
                        RTM_ERROR_DOMAIN,
                        RTM_TAG_NOT_FOUND,
                        "Tag \"%s\" was not found on this task", tag);
                return FALSE;
        }

        task->priv->tags = g_list_remove (task->priv->tags, existent_tag);
        return TRUE;
}

/**
 * rtm_task_get_added_date:
 * @task: a #RtmTask.
 *
 * Gets the #RtmTask:added_date property of the object.
 *
 * Returns: the added date of the task.
 */
GTimeVal *
rtm_task_get_added_date (RtmTask *task)
{
        g_return_val_if_fail (task != NULL, NULL);

        return task->priv->added_date;
}

/**
 * rtm_task_set_added_date:
 * @task: a #RtmTask.
 * @added_date: a added date for the #RtmTask.
 *
 * Sets the #RtmTask:added_date property of the object.
 *
 * Returns: %TRUE if added_date is set.
 */
gboolean
rtm_task_set_added_date (RtmTask *task, GTimeVal *added_date)
{
        g_return_val_if_fail (task != NULL, FALSE);
        g_return_val_if_fail (added_date != NULL, FALSE);

        task->priv->added_date = rtm_util_g_time_val_dup (added_date);
        return TRUE;
}

/**
 * rtm_task_get_completed_date:
 * @task: a #RtmTask.
 *
 * Gets the #RtmTask:completed_date property of the object.
 *
 * Returns: the completed date of the task.
 */
GTimeVal *
rtm_task_get_completed_date (RtmTask *task)
{
        g_return_val_if_fail (task != NULL, NULL);

        return task->priv->completed_date;
}

/**
 * rtm_task_set_completed_date:
 * @task: a #RtmTask.
 * @completed_date: a completed date for the #RtmTask.
 *
 * Sets the #RtmTask:completed_date property of the object.
 *
 * Returns: %TRUE if completed_date is set.
 */
gboolean
rtm_task_set_completed_date (RtmTask *task, GTimeVal *completed_date)
{
        g_return_val_if_fail (task != NULL, FALSE);
        g_return_val_if_fail (completed_date != NULL, FALSE);

        task->priv->completed_date = rtm_util_g_time_val_dup (completed_date);
        return TRUE;
}

/**
 * rtm_task_get_deleted_date:
 * @task: a #RtmTask.
 *
 * Gets the #RtmTask:deleted_date property of the object.
 *
 * Returns: the deleted date of the task.
 */
GTimeVal *
rtm_task_get_deleted_date (RtmTask *task)
{
        g_return_val_if_fail (task != NULL, NULL);

        return task->priv->deleted_date;
}

/**
 * rtm_task_set_deleted_date:
 * @task: a #RtmTask.
 * @deleted_date: a deleted date for the #RtmTask.
 *
 * Sets the #RtmTask:deleted_date property of the object.
 *
 * Returns: %TRUE if deleted_date is set.
 */
gboolean
rtm_task_set_deleted_date (RtmTask *task, GTimeVal *deleted_date)
{
        g_return_val_if_fail (task != NULL, FALSE);
        g_return_val_if_fail (deleted_date != NULL, FALSE);

        task->priv->deleted_date = rtm_util_g_time_val_dup (deleted_date);
        return TRUE;
}

/**
 * rtm_task_has_due_time:
 * @task: a #RtmTask.
 *
 * Checks the #RtmTask:has_due_time property of the object.
 *
 * Returns: %TRUE if the task due date has a due time.
 */
gboolean
rtm_task_has_due_time (RtmTask *task)
{
        g_return_val_if_fail (task != NULL, FALSE);

        return task->priv->has_due_time;
}

/**
 * rtm_task_set_has_due_time:
 * @task: a #RtmTask.
 * @has_due_time: %TRUE to specify that the task due date has a due time.
 *
 * Sets the #RtmTask:has_due_time property of the object.
 *
 * Returns: %TRUE if has_due_time is set.
 */
gboolean
rtm_task_set_has_due_time (RtmTask *task, gboolean has_due_time)
{
        g_return_val_if_fail (task != NULL, FALSE);

        task->priv->has_due_time = has_due_time;
        return TRUE;
}
