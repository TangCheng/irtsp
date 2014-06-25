#ifndef __IRTSP_H__
#define __IRTSP_H__

#include <base_app.h>

#define IPCAM_TYPE_IRTSP (ipcam_irtsp_get_type())
#define IPCAM_IRTSP(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPCAM_TYPE_IRTSP, IpcamIRtsp))
#define IPCAM_IRTSP_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPCAM_TYPE_IRTSP, IpcamIRtspClass))
#define IPCAM_IS_IRTSP(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPCAM_TYPE_IRTSP))
#define IPCAM_IS_IRTSP_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPCAM_TYPE_IRTSP))
#define IPCAM_IRTSP_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), IPCAM_TYPE_IRTSP, IpcamIRtspClass))

typedef struct _IpcamIRtsp IpcamIRtsp;
typedef struct _IpcamIRtspClass IpcamIRtspClass;

struct _IpcamIRtsp
{
    IpcamBaseApp parent;
};

struct _IpcamIRtspClass
{
    IpcamBaseAppClass parent_class;
};

GType ipcam_irtsp_get_type(void);

#endif /* __IRTSP_H__ */
