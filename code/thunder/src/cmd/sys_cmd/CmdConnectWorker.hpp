/*******************************************************************************
 * Project:  AsyncServer
 * @file     CmdConnectWorker.hpp
 * @brief    连接Worker进程
 * @author   cjy
 * @date:    2017年8月6日
 * @note     CmdConnectWorker有别于其他绝大部分Cmd的是，他是命令发起者而并非命令
 * 接收者，所以一开始他的AnyMessage()的后两个参数并无输入，为简化实现，不调用
 * AnyMessage，而调用Start()。
 * Modify history:
 ******************************************************************************/
#ifndef CMDCONNECTWORKER_HPP_
#define CMDCONNECTWORKER_HPP_

#include "protocol/oss_sys.pb.h"
#include "cmd/Cmd.hpp"
#include "step/sys_step/StepConnectWorker.hpp"

namespace thunder
{

class CmdConnectWorker : public Cmd
{
public:
    CmdConnectWorker();
    virtual ~CmdConnectWorker();
    virtual bool AnyMessage(
                    const MsgShell& stMsgShell,
                    const MsgHead& oInMsgHead,
                    const MsgBody& oInMsgBody);
    bool Start(const MsgShell& stMsgShell, int iWorkerIndex);

public:
    StepConnectWorker* pStepConnectWorker;        ///< 仅为了生成可读性高的类图，构造函数中不分配空间，析构函数中也不回收空间
};

} /* namespace thunder */

#endif /* CMDCONNECTWORKER_HPP_ */
