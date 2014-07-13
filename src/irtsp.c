#include "irtsp.h"
#include "messages.h"
#include <json-glib/json-glib.h>

/*
enum
{
    PROP_0,
    PROP_XX,
    N_PROPERTIES
};
*/

typedef struct _IpcamIRtspPrivate
{
    gboolean param_changed;
    guint rtsp_port;
    GHashTable *users_hash;
    GThread *live555_thread;
    gchar watch_variable;
} IpcamIRtspPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(IpcamIRtsp, ipcam_irtsp, IPCAM_BASE_APP_TYPE)

//static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };
extern void launch_irtsp_server(unsigned int port, char *watchVariable);
static void ipcam_irtsp_before_start(IpcamIRtsp *irtsp);
static void ipcam_irtsp_in_loop(IpcamIRtsp *irtsp);
static void message_handler(GObject *obj, IpcamMessage* msg, gboolean timeout);
static void ipcam_irtsp_request_users(IpcamIRtsp *irtsp);
static void ipcam_irtsp_request_rtsp_port(IpcamIRtsp *irtsp);
static void ipcam_irtsp_set_rtsp_port(IpcamIRtsp *irtsp, JsonNode *body);
static void ipcam_irtsp_set_users(IpcamIRtsp *irtsp, JsonNode *body);

static void ipcam_irtsp_finalize(GObject *self)
{
    IpcamIRtspPrivate *priv = ipcam_irtsp_get_instance_private(IPCAM_IRTSP(self));
    priv->watch_variable = 1;
    g_thread_join(priv->live555_thread);
    g_hash_table_destroy(priv->users_hash);
    G_OBJECT_CLASS(ipcam_irtsp_parent_class)->finalize(self);
}

static void
destroy_notify(gpointer data)
{
    g_free(data);
}

static void
ipcam_irtsp_init(IpcamIRtsp *self)
{
	IpcamIRtspPrivate *priv = ipcam_irtsp_get_instance_private(self);
    priv->param_changed = FALSE;
    priv->rtsp_port = 8554;
    priv->users_hash = g_hash_table_new_full(g_str_hash, g_str_equal, destroy_notify, destroy_notify);
    priv->watch_variable = 0;
}
/*
static void ipcam_irtsp_get_property(GObject    *object,
                                           guint       property_id,
                                           GValue     *value,
                                           GParamSpec *pspec)
{
    IpcamIrtsp *self = IPCAM_IRTSP(object);
    IpcamIRtspPrivate *priv = ipcam_irtsp_get_instance_private(self);
    switch(property_id)
    {
    case PROP_XX:
        {
            g_value_set_string(value, priv->xx);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_irtsp_set_property(GObject      *object,
                                           guint         property_id,
                                           const GValue *value,
                                           GParamSpec   *pspec)
{
    IpcamIrtsp *self = IPCAM_IRTSP(object);
    IpcamIRtspPrivate *priv = ipcam_irtsp_get_instance_private(self);
    switch(property_id)
    {
    case PROP_XX:
        {
            g_free(priv->xx);
            priv->xx = g_value_dup_string(value);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
*/
static void ipcam_irtsp_class_init(IpcamIRtspClass *klass)
{

    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->finalize = &ipcam_irtsp_finalize;
 /*
    object_class->get_property = &ipcam_irtsp_get_property;
    object_class->set_property = &ipcam_irtsp_set_property;

    obj_properties[PROP_XX] =
        g_param_spec_string("xx",
                            "xxx",
                            "xxx.",
                            NULL, // default value
                            G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

    g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);
*/
    IpcamBaseServiceClass *base_service_class = IPCAM_BASE_SERVICE_CLASS(klass);
    base_service_class->before = &ipcam_irtsp_before_start;
    base_service_class->in_loop = &ipcam_irtsp_in_loop;
}

static gpointer live555_thread_proc(gpointer data)
{
    IpcamIRtspPrivate *priv = (IpcamIRtspPrivate *)data;
    launch_irtsp_server(priv->rtsp_port, &priv->watch_variable);
    g_thread_exit(0);
    return NULL;
}

static void
ipcam_irtsp_before_start(IpcamIRtsp *irtsp)
{
    IpcamIRtspPrivate *priv = ipcam_irtsp_get_instance_private(irtsp);
    ipcam_irtsp_request_users(irtsp);
    ipcam_irtsp_request_rtsp_port(irtsp);
    priv->live555_thread = g_thread_new("live555", live555_thread_proc, priv);
}

static void
ipcam_irtsp_in_loop(IpcamIRtsp *irtsp)
{
    IpcamIRtspPrivate *priv = ipcam_irtsp_get_instance_private(irtsp);
    if (priv->param_changed)
    {
        priv->param_changed = FALSE;
        // do something
    }
}

static void
message_handler(GObject *obj, IpcamMessage* msg, gboolean timeout)
{
    g_return_if_fail(IPCAM_IS_IRTSP(obj));

    if (!timeout && msg)
    {
        gchar *str = ipcam_message_to_string(msg);
        g_print("result=\n%s\n", str);
        g_free(str);

        gchar *action;
        g_object_get(msg, "action", &action, NULL);
        if (g_str_equal("get_network", action))
        {
            JsonNode *body = NULL;
            g_object_get(msg, "body", &body, NULL);
            ipcam_irtsp_set_rtsp_port(IPCAM_IRTSP(obj), body);
        }
        else if (g_str_equal("get_users", action))
        {
            JsonNode *body = NULL;
            g_object_get(msg, "body", &body, NULL);
            ipcam_irtsp_set_users(IPCAM_IRTSP(obj), body);
        }
        else
        {
            g_print("unknown action: %s\n", action);
        }
        g_free(action);
    }
}

static void
ipcam_irtsp_request_users(IpcamIRtsp *irtsp)
{
    IpcamRequestMessage *rq_msg = g_object_new(IPCAM_REQUEST_MESSAGE_TYPE,
                                              "action", "get_users",
                                              NULL);
    gchar *token = ipcam_base_app_get_config(IPCAM_BASE_APP(irtsp), "token");
    JsonBuilder *builder = json_builder_new();
    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "items");
    json_builder_begin_array(builder);
    json_builder_add_string_value(builder, "username");
    json_builder_add_string_value(builder, "password");
    json_builder_end_array(builder);
    json_builder_end_object(builder);
    JsonNode *body = json_builder_get_root(builder);
    g_object_set(G_OBJECT(rq_msg), "body", body, NULL);
    ipcam_base_app_send_message(IPCAM_BASE_APP(irtsp),
                                IPCAM_MESSAGE(rq_msg),
                                "iconfig",
                                token,
                                message_handler,
                                5);
    g_object_unref(rq_msg);
    g_object_unref(builder);
}

static void
ipcam_irtsp_request_rtsp_port(IpcamIRtsp *irtsp)
{
    IpcamRequestMessage *rq_msg = g_object_new(IPCAM_REQUEST_MESSAGE_TYPE,
                                              "action", "get_network",
                                              NULL);
    gchar *token = ipcam_base_app_get_config(IPCAM_BASE_APP(irtsp), "token");
    JsonBuilder *builder = json_builder_new();
    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "items");
    json_builder_begin_array(builder);
    json_builder_add_string_value(builder, "server_port");
    json_builder_end_array(builder);
    json_builder_end_object(builder);
    JsonNode *body = json_builder_get_root(builder);
    g_object_set(G_OBJECT(rq_msg), "body", body, NULL);
    ipcam_base_app_send_message(IPCAM_BASE_APP(irtsp),
                                IPCAM_MESSAGE(rq_msg),
                                "iconfig",
                                token,
                                message_handler,
                                5);
    g_object_unref(rq_msg);
    g_object_unref(builder);
}

static void
ipcam_irtsp_set_rtsp_port(IpcamIRtsp *irtsp, JsonNode *body)
{
    JsonObject *items = json_object_get_object_member(json_node_get_object(body), "items");
    JsonObject *server_port = json_object_get_object_member(items, "server_port");
    const guint rtsp_port = json_object_get_int_member(server_port, "rtsp");
    g_print("rtsp_port = %u\n", rtsp_port);
    IpcamIRtspPrivate *priv = ipcam_irtsp_get_instance_private(irtsp);
    priv->param_changed = (priv->rtsp_port == rtsp_port);
    priv->rtsp_port = rtsp_port;
}

static void
proc_each_user(JsonArray *array, guint index_, JsonNode *element_node, gpointer user_data)
{
    const gchar *username = json_object_get_string_member(json_node_get_object(element_node), "username");
    const gchar *password = json_object_get_string_member(json_node_get_object(element_node), "password");
    g_print("username = %s\npassword = %s\n", username, password);
    IpcamIRtspPrivate *priv = ipcam_irtsp_get_instance_private(IPCAM_IRTSP(user_data));
    g_hash_table_insert(priv->users_hash, g_strdup(username), g_strdup(password));
}

static void
ipcam_irtsp_set_users(IpcamIRtsp *irtsp, JsonNode *body)
{
    JsonArray *users = json_object_get_array_member(json_node_get_object(body), "items");
    json_array_foreach_element(users, proc_each_user, irtsp);
    IpcamIRtspPrivate *priv = ipcam_irtsp_get_instance_private(irtsp);
    priv->param_changed = TRUE;
}
