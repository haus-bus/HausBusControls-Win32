/*
 * AR8SystemHw.cpp
 *
 * Created: 18.06.2014 14:12:55
 * Author: viktor.pankraz
 */

#include "AR8SystemHw.h"

#include <Security/Checksum.h>
#include <Security/ModuleId.h>
#include <HwUnits/DigitalOutputUnit.h>
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
      if ( slot < ( MAX_SLOTS / 2 ) )
      {
         slotHw[slot].getDigitalOutput1()->setPortNumber( PortA );
         slotHw[slot].getDigitalOutput0()->setPortNumber( PortC );
         //slotHw[slot].setTimerCounter0( &TimerCounter::instance( PortC, 0 ) );
      }
      else
      {
         slotHw[slot].getDigitalOutput1()->setPortNumber( PortB );
         slotHw[slot].getDigitalOutput0()->setPortNumber( PortD );
         //slotHw[slot].setTimerCounter0( &TimerCounter::instance( PortD, 0 ) );
      }
      if ( pinNumber > 3 )
      {
         pinNumber = 0;
      }
      slotHw[slot].getDigitalOutput0()->setPinNumber( pinNumber );
      slotHw[slot].getDigitalOutput1()->setPinNumber( pinNumber );
      slotHw[slot].configure( HbcConfiguration::instance().getSlotType( slot ) );

      if ( slotHw[slot].isDimmerHw() )
      {
         // new Dimmer( slot + 1, &slotHw[slot] );
      }
      else if ( slotHw[slot].isPowerSocketHw() )
      {
         object = new DigitalOutputUnit( *slotHw[slot].getDigitalOutput1() );
         object->setInstanceId( slot + 1 );
      }
      else if ( slotHw[slot].isRollerShutterHw() )
      {
         // new RollerShutter( slot + 1, reinterpret_cast<RollerShutterHw*>( &slotHw[slot] ) );
      }
      else if ( slotHw[slot].isDoubleSwitchHw() )
      {
         object = new DigitalOutputUnit( *slotHw[slot].getDigitalOutput0() );
         object->setInstanceId( slot + 1 );
         object = new DigitalOutputUnit( *slotHw[slot].getDigitalOutput1() );
         object->setInstanceId( slot + 9 );
      }
      DEBUG_M4( "slot", slot, " = ", slotHw[slot].getType() );

      pinNumber++;
   }
}


