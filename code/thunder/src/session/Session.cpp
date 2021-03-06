/*******************************************************************************
 * Project:  AsyncServer
 * @file     Session.cpp
 * @brief 
 * @author   cjy
 * @date:    2017年7月28日
 * @note
 * Modify history:
 ******************************************************************************/
#include "step/Step.hpp"
#include "Session.hpp"
#include "../step/StepNodeAccess.hpp"

namespace thunder
{

Session::Session(uint64 ulSessionId, ev_tstamp dSessionTimeout, const std::string& strSessionClass)
    : m_bRegistered(false), m_dSessionTimeout(dSessionTimeout), m_activeTime(0),
      m_strSessionClassName(strSessionClass), m_pLabor(0), m_pLogger(0), m_pTimeoutWatcher(0)
{
    char szSessionId[32] = {0};
    snprintf(szSessionId, sizeof(szSessionId), "%llu", ulSessionId);
    m_strSessionId = szSessionId;
}

Session::Session(const std::string& strSessionId, ev_tstamp dSessionTimeout, const std::string& strSessionClass)
    : m_bRegistered(false), m_dSessionTimeout(dSessionTimeout), m_activeTime(0),
      m_strSessionId(strSessionId), m_strSessionClassName(strSessionClass), m_pLabor(0), m_pLogger(0), m_pTimeoutWatcher(0)
{
}

Session::~Session()
{
    if (m_pTimeoutWatcher != 0)
    {
        free(m_pTimeoutWatcher);
        m_pTimeoutWatcher = 0;
    }
}

bool Session::RegisterCallback(Session* pSession)
{
    return(m_pLabor->RegisterCallback(pSession));
}

void Session::DeleteCallback(Session* pSession)
{
    m_pLabor->DeleteCallback(pSession);
}

bool Session::RegisterCallback(Step* pStep)
{
    return(m_pLabor->RegisterCallback(pStep));
}

void Session::DeleteCallback(Step* pStep)
{
    m_pLabor->DeleteCallback(pStep);
}

uint32 Session::GetNodeId()
{
    return(m_pLabor->GetNodeId());
}

uint32 Session::GetWorkerIndex()
{
    return(m_pLabor->GetWorkerIndex());
}

const std::string& Session::GetNodeType() const
{
    return(m_pLabor->GetNodeType());
}

const llib::CJsonObject& Session::GetCustomConf() const
{
    return(m_pLabor->GetCustomConf());
}

time_t Session::GetNowTime() const
{
    return(m_pLabor->GetNowTime());
}

bool Session::Pretreat(Step* pStep)
{
    return(m_pLabor->Pretreat(pStep));
}

bool Session::AsyncStep(Step* pStep,ev_tstamp dTimeout)
{
    if (pStep == NULL)
    {
        LOG4CPLUS_ERROR_FMT(GetLogger(),"pStep == NULL!");
        return(false);
    }
    if (!m_pLabor->RegisterCallback(pStep,dTimeout))
    {
        LOG4CPLUS_ERROR_FMT(GetLogger(),"RegisterCallback(pStep) error!");
        delete pStep;
        pStep = NULL;
        return(false);
    }
    if (STATUS_CMD_RUNNING != pStep->Emit(ERR_OK))
    {
        DeleteCallback(pStep);
        return(false);
    }
    return true;
}

bool Session::EmitStorageAccess(const std::string &strMsgSerial,
		StorageCallbackSession callback,bool boPermanentSession,
		const std::string &nodeType,uint32 uiCmd)
{
    return GetLabor()->EmitStorageAccess(this,strMsgSerial,callback,boPermanentSession,nodeType,uiCmd);
}

bool Session::EmitStandardAccess(const std::string &strMsgSerial,
		StandardCallbackSession callback,bool boPermanentSession,
		const std::string &nodeType,uint32 uiCmd)
{
    return GetLabor()->EmitStandardAccess(this,strMsgSerial,callback,boPermanentSession,nodeType,uiCmd);
}

} /* namespace thunder */
