/*
 * AR8SystemHw.cpp
 *
 * Created: 18.06.2014 14:12:55
 * Author: viktor.pankraz
 */

#include "AR8SystemHw.h"

#include <Security/Checksum.h>
#include <Security/ModuleId.h>
#include <HwUnitBoards/DimmerHw.h>
#include <HwUnits/Dimmer.h>
#include <HwUnits/DS1820.h>
#include <HwUnits/DigitalOutputUnit.h>
#include <HwUnits/RollerShutter.h>
#include <Tracing/Logger.h>
#include <LogicalButton.h>
#include <Protocols/IpStack/UdpConnection.h>
#include <Protocols/Ethernet/MAC.h>
#include <Gateway.h>
#include <HbcIpStackManager.h>
#include <HbcConfiguration.h>

#include <iostream>

void putc( char c )
{
   std::cout << c;
}

AR8SystemHw::AR8SystemHw()
{
   configure();
}

void AR8SystemHw::configure()
{
   // configure Logger
   Logger::instance().setStream( putc );

   //new DigitalOutputUnit( redLed );

   DEBUG_H1( FSTR( "configure" ) );
   configureSlots();
   configureLogicalButtons();
   configureEthernet();
   configureRs485();
   enableInterrupts();
}

void AR8SystemHw::configureEthernet()
{
   uint16_t deviceId = HbcConfiguration::instance().getDeviceId();
   MAC::local.set( 0xAE, 0xDE, 0x48, 0, HBYTE( deviceId ), LBYTE( deviceId ) );
/*
   if ( enc28j60.init() == Enc28j60::OK )
   {
      new HbcIpStackManager( enc28j60 );
      new Gateway( UdpConnection::connect( IP::broadcast(), 9, 9, NULL ), Gateway::UDP_9 );
   }
   */
}

void AR8SystemHw::configureRs485()
{
}

void AR8SystemHw::configureLogicalButtons()
{
   DEBUG_M1( FSTR( "LButtons" ) );

   uint8_t i = 0;
   uint8_t mask = HbcConfiguration::instance().getLogicalButtonMask();
   while ( mask )
   {
      if ( mask & 1 )
      {
         new LogicalButton( i + 1 );
      }
      mask >>= 1;
      i++;
   }
}

void AR8SystemHw::configureSlots()
{
   DEBUG_M1( FSTR( "Slots" ) );

   uint8_t slot = MAX_SLOTS;
   uint8_t pinNumber = 0;
   Object* object;
   while ( slot-- )
   {
      uint8_t portNumber = ( slot < ( MAX_SLOTS / 2 ) ) ? PortA : PortB;

      if ( pinNumber > 3 )
      {
         pinNumber = 0;
      }

      SlotType slotType = (SlotType)HbcConfiguration::instance().getSlotType( slot );
      DEBUG_M4( "slot", slot, " = ", (uint8_t)slotType );

      switch ( slotType )
      {
         case DIMMER:
         case DIMMER_30:
         case DIMMER_31:
         {
            new Dimmer( slot + 1, new DimmerHw( PortPin( portNumber + 2, pinNumber ),
                                                PortPin( portNumber, pinNumber ),
                                                slotType == DIMMER_31 ) );
            break;
         }

         case POWER_SOCKET:
         {
            object = new DigitalOutputUnit( PortPin( portNumber, pinNumber ) );
            object->setInstanceId( slot + 1 );
            break;
         }

         case ROLLER_SHUTTER:
         {
            new RollerShutter( slot + 1, new RollerShutterHw( PortPin( portNumber + 2, pinNumber ), PortPin( portNumber, pinNumber ) ) );
            break;
         }

         case DOUBLE_SWITCH:
         {
            object = new DigitalOutputUnit( PortPin( portNumber + 2, pinNumber ) );
            object->setInstanceId( slot + 1 );
            object = new DigitalOutputUnit( PortPin( portNumber, pinNumber ) );
            object->setInstanceId( slot + 1 + MAX_SLOTS );
            break;
         }
         case UNUSED_SLOT:
         case NOT_CONFIGURED:
         {
            break;
         }

         default:
         {
            WARN_4( "not supported slotType: ", (uint8_t)slotType, " for slot", slot );
         }
      }
      pinNumber++;
   }
}

