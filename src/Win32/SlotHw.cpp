/*
 * SlotHw.cpp
 *
 * Created: 18.06.2014 14:12:55
 * Author: viktor.pankraz
 */

#include "SlotHw.h"
#include <Tracing/Logger.h>

const uint8_t SlotHw::debugLevel( DEBUG_LEVEL_OFF );

SlotHw::SlotHw() : pwmEnabled( 0 ), digitalOutput0( 0, 0 ), digitalOutput1( 0, 0 )
{
}

void SlotHw::configure( uint8_t targetType )
{
   if ( targetType && ( targetType < MAX_SLOT_TYPES ) )
   {
      type = targetType;
      configureOutput( digitalOutput0, ( type & 1 ) );
      configureOutput( digitalOutput1, ( type & 2 ) );
   }
   else
   {
      type = UNUSED_SLOT;
   }
}

void SlotHw::configureOutput( DigitalOutput& output, uint8_t isOutput )
{
   if ( isOutput )
   {
      output.clear();
      output.getIoPort().setPinsAsOutput( output.getPin() );
   }
   else
   {
      output.enablePullup();
      output.configInput();
   }
}

uint16_t SlotHw::getPwmDuty()
{
   uint8_t pin = digitalOutput0.getPin();

   uint16_t compareValue = 0;
   if ( pin == Pin0 )
   {
      compareValue = timerCounter0->getCaptureA();
   }
   else if ( pin == Pin1 )
   {
      compareValue = timerCounter0->getCaptureB();
   }
   else if ( pin == Pin2 )
   {
      compareValue = timerCounter0->getCaptureC();
   }
   else if ( pin == Pin3 )
   {
      compareValue = timerCounter0->getCaptureD();
   }

   if ( compareValue < timerCounter0->getPeriod() )
   {
      return compareValue / ( timerCounter0->getPeriod() / 1000 );
   }
   return 1000;
}

uint8_t SlotHw::hasError()
{
   if ( uint8_t version = isDimmerHw() )
   {
      if ( ( version == V30 ) && digitalOutput1.isSet() )
      {
         return ErrorCode::DEFECT;
      }
      if ( ( version == V31 ) && isOn() && !digitalOutput1.isSet() )
      {
         return ErrorCode::DEFECT;
      }
      if ( timerCounter0->getPeriod() == 0xFFFF )
      {
         return ErrorCode::INVALID_PERIOD;
      }
      if ( !timerCounter0->isRunning() && pwmEnabled )
      {
         return ErrorCode::NO_ZERO_CROSS_DETECTED;
      }
   }
   return ErrorCode::NO_FAILTURE;
}

uint16_t SlotHw::isOn()
{
   if ( isDimmerHw() )
   {
      if ( pwmEnabled && timerCounter0->isRunning() )
      {
         return getPwmDuty();
      }
      return digitalOutput0.isSet();
   }
   else if ( isRollerShutterHw() )
   {
      return digitalOutput0.isSet() || digitalOutput1.isSet();
   }
   else if ( isPowerSocketHw() )
   {
      return digitalOutput1.isSet();
   }
   return 0;
}

void SlotHw::on( uint16_t value )
{
   setPwmDuty( value );
}

uint8_t SlotHw::setMode( uint8_t mode )
{
   if ( mode == DIMM_L )
   {
      digitalOutput0.setInverted( true );
   }
   else
   {
      digitalOutput0.setInverted( false );
   }
   pwmEnabled = ( mode < SWITCH );
   return ( mode > SWITCH ) ? ErrorCode::INVALID_MODE : ErrorCode::NO_FAILTURE;
}

void SlotHw::setPwmDuty( uint16_t duty )
{
   if ( pwmEnabled )
   {
      if ( digitalOutput0.isInverted() )
      {
         if ( duty > 1000 )
         {
            duty = 0;
         }
         else
         {
            duty = 1000 - duty;
         }
      }

      if ( duty )
      {
         digitalOutput0.set();
      }
      else
      {
         digitalOutput0.clear();
      }
   }
}

void SlotHw::stop()
{
   digitalOutput0.clear();
   digitalOutput1.clear();
}

uint8_t SlotHw::suspend( uint8_t mode )
{
   if ( isDimmerHw() == V31 )
   {
      if ( mode == ENABLE )
      {
         digitalOutput1.configOutput();
         digitalOutput1.clear();
      }
      else if ( mode == DISABLE )
      {
         digitalOutput1.configOutput();
         digitalOutput1.set();
      }
      else
      {
         digitalOutput1.configInput();
      }
   }
   return true;

}

DigitalOutput* SlotHw::getDigitalOutput0() const
{
   return (DigitalOutput*) &digitalOutput0;
}

DigitalOutput* SlotHw::getDigitalOutput1() const
{
   return (DigitalOutput*) &digitalOutput1;
}
