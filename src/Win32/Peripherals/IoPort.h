/*
 * IoPort.h
 *
 *  Created on: 17.07.2017
 *      Author: Viktor Pankraz
 */

#ifndef Peripherals_IoPort_H
#define Peripherals_IoPort_H

#include <SwFramework.h>

class IoPort
{
   public:

      ////    Constructors and destructors    ////

   protected:

      uint8_t pinDirections;
      uint8_t pinStates;
      uint8_t intFlags;
      uint8_t int0Mask;
      uint8_t int1Mask;
      uint8_t invertedPins;
      uint8_t pulledUpPins;

      inline IoPort()
      {
         pinDirections = 0;
         pinStates = 0xFF;
         intFlags = 0;
         int0Mask = 0;
         int1Mask = 0;
         invertedPins = 0;
         pulledUpPins = 0xFF;
      }

      ////    Operations    ////

   public:

      inline void clearInterruptFlags( uint8_t bitMask )
      {
         intFlags &= ~bitMask;
      }

      inline void clearPins( uint8_t pins )
      {
         pinStates &= ~pins;
      }

      inline void disableInterrupt0()
      {
      }

      inline void disableInterrupt0Source( uint8_t pins )
      {
         int0Mask &= ~pins;
      }

      inline void disableInterrupt1()
      {
      }

      inline void disableInterrupt1Source( uint8_t pins )
      {
         int1Mask &= ~pins;
      }

      inline void enableInterrupt0()
      {
      }

      inline void enableInterrupt0Source( uint8_t pins )
      {
         int0Mask |= pins;
      }

      inline uint8_t isInterrupt0SourceEnabled( uint8_t pins )
      {
         return int0Mask & pins;
      }

      inline void enableInterrupt1()
      {
      }

      inline void enableInterrupt1Source( uint8_t pins )
      {
         int1Mask |= pins;
      }

      inline uint8_t isInterrupt1SourceEnabled( uint8_t pins )
      {
         return int1Mask & pins;
      }

      inline uint8_t get() const
      {
         return pinStates;
      }

      inline uint8_t getInputPins()
      {
         return ~getOutputPins();
      }

      inline uint8_t getInterruptFlags( uint8_t bitMask )
      {
         return intFlags & bitMask;
      }

      inline uint8_t getOutputPins()
      {
         return pinDirections;
      }

      static IoPort& instance( uint8_t portNumber );

      template<uint8_t portNumber>
      static inline IoPort& instance()
      {
         if ( portNumber < PortMax )
         {
            return ports[portNumber];
         }
         return dummyPort;
      }

      inline uint8_t isPinInverted( uint8_t pinNumber )
      {
         if ( pinNumber < 8 )
         {
            return invertedPins & (1<<pinNumber);
         }
         return 0;
      }

      inline uint8_t isPinSet( uint8_t pin )
      {
         return get() & pin;
      }

      inline void set( uint8_t value )
      {
         pinStates = value;
      }

      inline void setdirectionection( uint8_t pins )
      {
         pinDirections = pins;
      }

      inline void setPinInverted( uint8_t pinNumber, bool inverted )
      {
         if ( pinNumber < 8 )
         {
            if ( inverted )
            {
               invertedPins |= (1 << pinNumber);
            }
            else
            {
               invertedPins &= ~(1 << pinNumber);
            }
         }
      }

      inline void enablePullup( uint8_t pinNumber )
      {
         if ( pinNumber < 8 )
         {
            pulledUpPins |= (1 << pinNumber);
         }
      }

      inline void enablePulldown( uint8_t pinNumber )
      {
         if ( pinNumber < 8 )
         {
            pulledUpPins &= ~(1 << pinNumber);
         }
      }      

      inline void setPins( uint8_t pins )
      {
         pinStates |= pins;
      }

      inline void setPinsAsInput( uint8_t pins )
      {
         pinDirections &= ~pins;
      }

      inline void setPinsAsOutput( uint8_t pins )
      {
         pinDirections |= pins;
      }

      inline void toggledirectionection( uint8_t pins )
      {
         pinDirections = invert<uint8_t>( pinDirections, pins );
      }

      inline void togglePins( uint8_t pins )
      {
         pinStates = invert<uint8_t>( pinStates, pins );
      }

      ////    Attributes    ////

   protected:

      static IoPort dummyPort;

      static IoPort ports[PortMax];
};

#endif
