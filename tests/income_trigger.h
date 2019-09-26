/**
 *  @file    income_trigger.h
 *
 *  @author  Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 *  @copyright MIT License
 */

#ifndef INCOME_TRIGGER_H
#define INCOME_TRIGGER_H

#include <connection_trigger.h>

namespace Kitsune
{
namespace Network
{
class AbstractSocket;

class IncomeTrigger : public ConnectionTrigger
{
public:
    IncomeTrigger();
    ~IncomeTrigger();
    void handleConnection(AbstractSocket* socket);
};

} // namespace Network
} // namespace Kitsune

#endif // INCOME_TRIGGER_H
