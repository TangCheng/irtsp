#include "iRTSPServer.hh"

IRTSPServer::IRTSPServer(Boolean reuseFirstSource, Boolean iFramesOnly)
     : m_reuseFirstSource(reuseFirstSource),
       m_iFramesOnly(iFramesOnly),
       m_authDB(NULL),
       m_rtspServer(NULL)
{
    // Begin by setting up our usage environment:
    m_scheduler = BasicTaskScheduler::createNew();
    m_env = BasicUsageEnvironment::createNew(*m_scheduler);
}

IRTSPServer::~IRTSPServer()
{
}

void IRTSPServer::setAuthUser()
{
}

void IRTSPServer::startServer()
{
    m_rtspServer = RTSPServer::createNew(*m_env, 8554, m_authDB);
    if (m_rtspServer == NULL) {
        *m_env << "Failed to create RTSP server: " << m_env->getResultMsg() << "\n";
        return;
    }
    char const* streamName = "liveStream";
    char const* descriptionString = "Session streamed by \"iRTSP\"";
    ServerMediaSession* sms =
        ServerMediaSession::createNew(*m_env, streamName, streamName, descriptionString);
    sms->addSubsession(H264LiveStreamServerMediaSubsession::createNew(*m_env, m_reuseFirstSource));
    m_rtspServer->addServerMediaSession(sms);

    m_env->taskScheduler().doEventLoop(); // does not return
}
