/** \file SMTPlib.c
 *  \brief SMTP wrapper for FreeRTOS
 *
 * \addtogroup net
 * @{
*/

/**
\defgroup SMTPlib SMTPlib stack
@{

The SMTP commands allows to set the parameters for the SMTP connection, and to send emails from the Flyport.

*/

/* **************************************************************************																					
 *                                OpenPicus                 www.openpicus.com
 *                                                            italian concept
 * 
 *            openSource wireless Platform for sensors and Internet of Things	
 * **************************************************************************
 *  FileName:        SMTPlib.c
 *  Dependencies:    Microchip configs files
 *  Module:          FlyPort WI-FI
 *  Compiler:        Microchip C30 v3.12 or higher
 *
 *  Author               Rev.    Date              Comment
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Gabriele Allegria    1.0     1/20/2011		   First release  (core team)
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  Software License Agreement
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  This is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License (version 2) as published by 
 *  the Free Software Foundation AND MODIFIED BY OpenPicus team.
 *  
 *  ***NOTE*** The exception to the GPL is included to allow you to distribute
 *  a combined work that includes OpenPicus code without being obliged to 
 *  provide the source code for proprietary components outside of the OpenPicus
 *  code. 
 *  OpenPicus software is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details. 
 * 
 * 
 * Warranty
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * WE ARE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 **************************************************************************/


#include "SMTPlib.h"



static int ToSend = 0; 

#if defined(STACK_USE_SMTP_CLIENT)
 /// @cond debug
//****************************************************************************
//	Generic function to accomplish: start, send, busy and stop (to save space) 
//****************************************************************************
BOOL GenericSMTP(int fSMTP)
{
 	while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);		//	xSemFrontEnd TAKE
	xErr = 0;
	if (xFrontEndStat == 0)
	{	
		ToSend = fSMTP;
		xFrontEndStatRet = 2;

		xQueueSendToBack(xQueue,&ToSend,0);					//	Send TCPIsConnected command to the stack
		xFrontEndStat = 1;
		xSemaphoreGive(xSemFrontEnd);						//	xSemFrontEnd GIVE	

		while (xFrontEndStat != 2);								//	Waits for stack answer
		while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);		//	xSemFrontEnd TAKE

		BOOL retBool;
		retBool = xBool;
		xFrontEndStat = 0;
		xSemaphoreGive(xSemFrontEnd);							//	xSemFrontEnd GIVE
		taskYIELD();
		return retBool;
	}
	else
	{
		xErr = fSMTP;
		xSemaphoreGive(xSemFrontEnd);
		taskYIELD();
		return FALSE;
	}	
}
 /// @endcond



/**
 * SMTPStart - Initializes the SMTP module. If the client is already in use, the initialization will not be possible.
 * \param None
 * \return TRUE: the SMTP client was correctly initialized.
 * \return FALSE: the SMTP client wasn't initialized. Maybe it's already used
 */
 BOOL SMTPStart()
 {
	return GenericSMTP(26);
 }
 
 /// @cond debug
 int cSMTPStart()
 {
	xBool = SMTPBeginUsage();
	return 0;
 }
 /// @endcond



/**
 * SMTPSetServer - Sets all the parameters for the SMTP remote server (server name, username, password and port).
 * \param servparam the parameter to set. Available list is the following:<BR>
 <UL>
	<LI><B>SERVER_NAME:</B> the name of the SMTP server.</LI>
	<LI><B>SERVER_USER:</B> the username for access to the server.</L1>
	<LI><B>SERVER_PASS:</B> the password associated to the username.</L1>
	<LI><B>SERVER_PORT:</B> the port of the SMTP server.</L1>
 </UL>
 * \param paramfield a string containing the associated value to the parameter (server name, username, password or SMTP port).<BR>
  For example: SMTPSetServer(SERVER_NAME , "smtp.serverxyz.com")
 * \return None.
 */
void SMTPSetServer(int servparam , char * paramfield)
{

 	while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);		//	xSemFrontEnd TAKE
	xErr = 0;
	if (xFrontEndStat == 0)
	{	
		ToSend = 27;
		xFrontEndStatRet = 2;
		xInt = servparam;
		xChar = paramfield;
		xQueueSendToBack(xQueue,&ToSend,0);					//	Send TCPIsConnected command to the stack
		xFrontEndStat = 1;
		xSemaphoreGive(xSemFrontEnd);						//	xSemFrontEnd GIVE	

		while (xFrontEndStat != 2);								//	Waits for stack answer
		while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);		//	xSemFrontEnd TAKE

		BOOL retBool;
		retBool = xBool;
		xFrontEndStat = 0;
		xSemaphoreGive(xSemFrontEnd);							//	xSemFrontEnd GIVE
		taskYIELD();
	}
	else
	{
		xErr = 27;
		xSemaphoreGive(xSemFrontEnd);
		taskYIELD();
	}	
}

 /// @cond debug
int cSMTPSetServer()
{
	switch (xInt)
	{
	//	Setting server name
	case SERVER_NAME:
		SMTPClient.Server.szRAM = (BYTE *) xChar;
		break;
	
	//	Setting username
	case SERVER_USER:
		SMTPClient.Username.szRAM = (BYTE *) xChar;
		break;
		
	//	Setting password
	case SERVER_PASS:
		SMTPClient.Password.szRAM = (BYTE *) xChar;
		break;
		
	//	Port setting
	case SERVER_PORT:
		SMTPClient.ServerPort = (WORD)atol(xChar);
		break;		
	}
	return 0;
}
 /// @endcond
 
 
//****************************************************************************
//	28 - SMTPSetMsg - Sets all the parameter for the email message to send
//****************************************************************************
/**
 * SMTPSetMsg - Sets all the parameters for the email message to send (the sender, the destination address, the cc and bcc addresses, email subject and body). 
 * \param msgparam the parameter to set. Available list is the following:<BR>
 <UL>
	<LI><B>MSG_TO:</B> the destination address.</LI>
	<LI><B>MSG_CC:</B> the cc address.</L1>
	<LI><B>MSG_BCC:</B> the bcc address.</L1>
	<LI><B>MSG_FROM:</B> the sender of the email message.</L1>
	<LI><B>MSG_SUBJECT:</B> the object of the email message.</L1>
	<LI><B>MSG_BODY:</B> the body of the email message.</L1>
 </UL>
 * \param mparamfield a string containing the associated value to the parameter.<BR>
  For example: if we want to send a message to example@openpicus.com. SMTPSetMsg(MSG_TO , "example@openpicus.com")
 * \return None.
 */
void SMTPSetMsg(int msgparam , char * mparamfield)
{

 	while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);		//	xSemFrontEnd TAKE
	xErr = 0;
	if (xFrontEndStat == 0)
	{	
		ToSend = 28;
		xFrontEndStatRet = 2;
		xInt = msgparam;
		xChar = mparamfield;
		xQueueSendToBack(xQueue,&ToSend,0);					//	Send TCPIsConnected command to the stack
		xFrontEndStat = 1;
		xSemaphoreGive(xSemFrontEnd);						//	xSemFrontEnd GIVE	

		while (xFrontEndStat != 2);								//	Waits for stack answer
		while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);		//	xSemFrontEnd TAKE

		BOOL retBool;
		retBool = xBool;
		xFrontEndStat = 0;
		xSemaphoreGive(xSemFrontEnd);							//	xSemFrontEnd GIVE
		taskYIELD();
	}
	else
	{
		xErr = 28;
		xSemaphoreGive(xSemFrontEnd);
		taskYIELD();
	}	
}


 /// @cond debug
int cSMTPSetMsg()
{
	switch (xInt)
	{
	//	Setting destination email address
	case MSG_TO:
		SMTPClient.To.szRAM = (BYTE *) xChar;
		break;
		
	//	Setting cc for the message
	case MSG_CC:
		SMTPClient.CC.szRAM = (BYTE *) xChar;
		break;
	
	//	Setting bcc for the message
	case MSG_BCC:
		SMTPClient.BCC.szRAM = (BYTE *) xChar;
		break;
	
	//	Setting sender of the message
	case MSG_FROM:
		SMTPClient.From.szRAM = (BYTE *) xChar;
		break;
		
	//	Setting subject of the message
	case MSG_SUBJECT:
		SMTPClient.Subject.szRAM = (BYTE *) xChar;
		break;
	
	//	Setting the body of the message
	case MSG_BODY:
		SMTPClient.Body.szRAM = (BYTE *) xChar;
		break;
	}
	return 0;
}
 /// @endcond
 
 
 
/**
 * SMTPSend - Once all the connection and message settings are done, it's possible to send the desired email message with this command
 * \param None
 * \return TRUE: the message was correctly sent.
 * \return FALSE: there was an error. Maybe the SMTP client is still busy in sending another message, or it's not initialized.
 */
BOOL SMTPSend()
{
	return GenericSMTP(29);
}
 
 /// @cond debug
//****************************************************************************
//	Only internal use
//	SMTPSend callback function
//**************************************************************************** 
int cSMTPSend()
{
	SMTPSendMail();
	xBool = TRUE;
	return 0;
}
 /// @endcond


/**
 * SMTPBusy - Verifies if the SMTP client is busy perfoming some action. Until the client is busy is not possible to other SMTP commands.
 * \param None
 * \return TRUE: the client is busy.
 * \return FALSE: the client is not performing any action, and can be used by the firmware.
 */
BOOL SMTPBusy()
{
	return GenericSMTP(30);
}
 
 /// @cond debug
//****************************************************************************
//	Only internal use
//	SMTPBusy callback funtion
//****************************************************************************
int cSMTPBusy()
{
	xBool = SMTPIsBusy();
	return 0;
}
 /// @endcond


/**
 * SMTPStop - Closes the SMTP client. To use it again must restart it.
 * \param None
 * \return TRUE: the client correctly closed.
 * \return FALSE: an error occured closing the client.
 */
BOOL SMTPStop()
{
	return GenericSMTP(31);
}
 
 /// @cond debug
//****************************************************************************
//	Only internal use
//	SMTPStop callback function
//****************************************************************************
int cSMTPStop()
{
	if (SMTPEndUsage() == SMTP_SUCCESS)
		xBool = TRUE;
	else
		xBool = FALSE;		
	return 0;
}
 /// @endcond
#endif
