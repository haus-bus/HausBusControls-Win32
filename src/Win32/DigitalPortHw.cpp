/*
 * DigitalPortHw.cpp
 *
 *  Created on: 06.08.2014
 *      Author: Viktor Pankraz
 */

#include "DigitalPortHw.h"


DigitalPortHw::DigitalPortHw( uint8_t portNumber ) : port( &IoPort::instance( portNumber ) )
{
}

void DigitalPortHw::configure( uint8_t outputPins, uint8_t inputPins, uint8_t pwmPins )
{
   port->setPinsAsOutput( outputPins | pwmPins );
   port->clearPins( outputPins | pwmPins );
   port->setPinsAsInput( inputPins );
}

void DigitalPortHw::on( uint8_t pin, uint8_t value )
{
   if ( value )
   {
      port->setPins( pin );
   }
   else
   {
      port->clearPins( pin );
   }
}