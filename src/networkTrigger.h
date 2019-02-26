/**
 *  @file    networkTrigger.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#ifndef NETWORKTRIGGER_H
#define NETWORKTRIGGER_H

#include <cstdio>
#include <cinttypes>

namespace Kitsune
{
namespace Network
{

class NetworkTrigger
{
public:
    NetworkTrigger();
    virtual ~NetworkTrigger();
    virtual void runTask(uint8_t* buffer, const long bufferSize) = 0;
};

}
}

#endif // NETWORKTRIGGER_H
