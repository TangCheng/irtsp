#include "iRTSPServer.hh"

#ifdef __cplusplus
extern "C"
{
#endif

void launch_irtsp_server()
{
    IRTSPServer *server = new IRTSPServer();
    server->startServer();
}

#ifdef __cplusplus
}
#endif
