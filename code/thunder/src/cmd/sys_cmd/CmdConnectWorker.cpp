/*******************************************************************************
 * Project:  AsyncServer
 * @file     CmdConnectWorker.cpp
 * @brief 
 * @author   cjy
 * @date:    2017年8月6日
 * @note
 * Modify history:
 ******************************************************************************/
#include "CmdConnectWorker.hpp"

namespace thunder
{

CmdConnectWorker::CmdConnectWorker()
    : pStepConnectWorker(NULL)
{
}

CmdConnectWorker::~CmdConnectWorker()
{
}

bool CmdConnectWorker::AnyMessage(
                const MsgShell& stMsgShell,
                const MsgHead& oInMsgHead,
                const MsgBody& oInMsgBody)
{
    return(true);
}

bool CmdConnectWorker::Start(const MsgShell& stMsgShell, int iWorkerIndex)
{
    MsgHead oMsgHead;
    MsgBody oMsgBody;
    ConnectWorker oConnWorker;
    oConnWorker.set_worker_index(iWorkerIndex);
    oMsgBody.set_body(oConnWorker.SerializeAsString());
    oMsgHead.set_cmd(CMD_REQ_CONNECT_TO_WORKER);
//    oMsgHead.set_seq(pStepConnectWorker->GetSequence());
    oMsgHead.set_msgbody_len(oMsgBody.ByteSize());
    LOG4_DEBUG("send cmd %d.", oMsgHead.cmd());
    for (int i = 0; i < 3; ++i)
    {
        pStepConnectWorker = new StepConnectWorker(stMsgShell, oMsgHead, oMsgBody);
        if (pStepConnectWorker == NULL)
        {
            LOG4_ERROR("error %d: new StepConnectWorker() error!", ERR_NEW);
            return(false);
        }

        if (RegisterCallback(pStepConnectWorker))
        {
            pStepConnectWorker->Emit(ERR_OK);
            return(true);
        }
        else
        {
            delete pStepConnectWorker;
        }
    }
    return(false);
}

} /* namespace thunder */
