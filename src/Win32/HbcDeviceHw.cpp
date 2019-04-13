/*
 * HbcDeviceHw.cpp
 *
 *  Created on: 28.08.2014
 *      Author: Viktor Pankraz
 */

#include "HbcDeviceHw.h"
#include <Security/Checksum.h>
#include <FlashString.h>
#include <Tracing/Logger.h>
#include <Security/ModuleId.h>

const uint8_t HbcDeviceHw::debugLevel( DEBUG_LEVEL_OFF );

bool HbcDeviceHw::getModuleId( uint8_t index, ModuleId* moduleId )
{
   return false;
}

void HbcDeviceHw::enableInterrupts()
{
}