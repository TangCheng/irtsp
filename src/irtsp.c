#include "irtsp.h"

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
    gchar *xx;
} IpcamIRtspPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(IpcamIRtsp, ipcam_irtsp, IPCAM_BASE_APP_TYPE)

//static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };

static void ipcam_irtsp_init(IpcamIRtsp *self)
{
	IpcamIRtspPrivate *priv = ipcam_irtsp_get_instance_private(self);
    priv->xx = NULL;
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
/*
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
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
}
