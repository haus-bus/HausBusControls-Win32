/*
 * AR8SystemHw.h
 *
 *  Created on: 06.08.2014
 *      Author: Viktor Pankraz
 */

#ifndef Electronics_SystemBoards_AR8_AR8_Boards_AR8SystemHw_H
#define Electronics_SystemBoards_AR8_AR8_Boards_AR8SystemHw_H

#include <HbcDeviceHw.h>

class AR8SystemHw : public HbcDeviceHw
{
   public:

      static const uint8_t MAX_SLOTS = 8;

      ////    Constructors and destructors    ////

      AR8SystemHw();

      ////    Operations    ////

   private:

      void configure();

      void configureEthernet();

      void configureRs485();

      void configureLogicalButtons();

      void configureSlots();

      inline static const char* getId()
      {
         return "SystemHw:: ";
      }      

      ////    Additional operations    ////

   public:

      ////    Attributes    ////

      ////    Relations and components    ////

   protected:

};

#endif