/*
 * Enc28j60.cpp
 *
 *  Created on: 28.08.2014
 *      Author: Viktor Pankraz
 */

#include <Enc28j60.h>


bool Enc28j60::isLinkedUp()
{
   return true;
}


uint16_t Enc28j60::read( void* pData, uint16_t length )
{
   return 0;
}

void Enc28j60::setUdpPort9Filter()
{
   // configure Pattern Match Filter to destination IP=0xFFFFFFFF and MagicNumber = 0xEFEF
}

uint16_t Enc28j60::write( void* pData, uint16_t length )
{
   return 0;
}