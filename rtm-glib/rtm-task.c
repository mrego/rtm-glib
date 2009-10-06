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

#define RTM_TASK_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE (        \
                                           (obj), RTM_TYPE_TASK, RtmTaskPrivate))

struct _RtmTaskPrivate {
        gchar *id;
        gchar *taskseries_id;
        gchar *list_id;
        gchar *name;
        gchar *priority;
};

enum {
        PROP_0,

        PROP_ID,
        PROP_TASKSERIES_ID,
        PROP_LIST_ID,
        PROP_NAME,
        PROP_PRIORITY,
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

        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id,
                                                   pspec);
                break;
        }
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

}

static void
rtm_task_init (RtmTask *task)
{
        task->priv = RTM_TASK_GET_PRIVATE (task);
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

        RestXmlNode *node_tmp;

        task->priv->taskseries_id = g_strdup (rest_xml_node_get_attr (node, "id"));
        task->priv->name = g_strdup (rest_xml_node_get_attr (node, "name"));

        node_tmp = rest_xml_node_find (node, "task");
        task->priv->id = g_strdup (rest_xml_node_get_attr (node_tmp, "id"));
        task->priv->priority = g_strdup (rest_xml_node_get_attr (node_tmp, "priority"));

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

        gchar *string = g_strconcat (
                "RtmTask: [\n",
                "  ID: ", rtm_util_string_or_null (task->priv->id), "\n",
                "  Taskseries ID: ",
                rtm_util_string_or_null (task->priv->taskseries_id), "\n",
                "  List ID: ",
                rtm_util_string_or_null (task->priv->list_id), "\n",
                "  Name: ", rtm_util_string_or_null (task->priv->name), "\n",
                "  Priority: ", rtm_util_string_or_null (task->priv->priority), "\n",
                "]\n",
                NULL);
        return string;
}