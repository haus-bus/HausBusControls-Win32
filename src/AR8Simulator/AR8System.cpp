/*
 * AR8System.cpp
 *
 * Created: 18.06.2014 14:12:55
 * Author: viktor.pankraz
 */

#include "AR8System.h"
#include <Scheduler.h>

int main( void )
{
   static const uint8_t MAX_JOBS = 64;
   static const uint8_t MAX_EVENTS = 64;
   static const uint16_t EEPROM_SIZE = 4096;
   static const uint16_t USER_SIGNATURE_SIZE = 64;
   static const uint16_t APPLICATION_TABLE_SIZE = 8192;
   Scheduler::setup( MAX_JOBS, MAX_EVENTS );

   // setup the persistent memories
   ConfigurationManager::setup( Eeprom::setup( EEPROM_SIZE ), EEPROM_SIZE );
   UserSignature::setup( USER_SIGNATURE_SIZE );
   ApplicationTable::setup( APPLICATION_TABLE_SIZE );

   static AR8System ar8;

   while ( 1 )
   {
      Scheduler::runJobs();
      SystemTime::waitMs(2);
   }
}

AR8System::AR8System() :
   digitalPortE( PortE ), digitalPortF( PortF )
{
   if ( hardware.getFckE() < FCKE_V4_0 )
   {
      digitalPortE.setNotUseablePins( Pin0 | Pin1 | Pin4 | Pin5 );
   }
   else
   {
      digitalPortE.setNotUseablePins( Pin0 | Pin1 | Pin2 | Pin3 );
   }

#ifdef _TRACE_PORT_
   digitalPortF.terminate();
#endif
}

