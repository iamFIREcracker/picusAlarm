/** \file FTPlib.c
 *  \brief FTP wrapper for FreeRTOS
 */

/**
\addtogroup net
@{
*/

/**
\defgroup FTPlib FTPlib stack
@{

FTP provides Internet communication abilities.

\section FTP library
*/

/* **************************************************************************																					
 *                                OpenPicus                 www.openpicus.com
 *                                                            italian concept
 * 
 *            openSource wireless Platform for sensors and Internet of Things	
 * **************************************************************************
 *  FileName:        FTPlib.c
 *  Dependencies:    Microchip configs files
 *  Module:          FlyPort WI-FI
 *  Compiler:        Microchip C30 v3.12 or higher
 *
 *  Author               Rev.    Date              Comment
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Gabriele Allegria    1.0     1/20/2011		   First release  (core team)
 *  
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

#include "FTPlib.h"




/**
 * FTPClientOpen - Creates a FTP client on specified IP address and port
 * \param ftpaddr - IP address of the remote server. Example: "192.168.1.100" (the char array must be NULL terminated).
 * \param ftpport - Port of the remote server to connect. Example: "1234" (the char array must be NULL terminated).
 * \return - INVALID_SOCKET: the operation was failed. Maybe there are not available sockets.
 * \return - A TCP_SOCKET handle to the created socket. It must be used to access the socket in the program (read/write operations and close socket).
 */
TCP_SOCKET FTPClientOpen (char ftpaddr[] , char ftpport[])
{
	return TCPGenericOpen(ftpaddr , 1 , ftpport , 3);
}


/**
 * FTPRead - Reads the specified number of characters from a FTP socket and puts them into the specified char array
 * \param ftpsockread - The handle of the socket to read (the handle returned by the command FTPClientOpen).
 * \param ftpreadch - The char array to fill with the read characters.
 * \param ftprlen - The number of characters to read. 
 * \warning The length of the array must be AT LEAST = ftprlen+1, because at the end of the operation the array it's automatically NULL terminated (is added the '\0' character).
 * \return None.
 */
void FTPRead(TCP_SOCKET ftpsockread , char ftpreadch[] , int ftprlen)
{
	TCPRead(ftpsockread , ftpreadch , ftprlen);
}

/**
 * FTPWrite - Writes an array of characters on the specified socket.
 * \param ftpsockwr - The socket to which data is to be written (it's the handle returned by the command TCPClientOpen or TCPServerOpen).
 * \param ftpstrtowr - Pointer to the array of characters to be written.
 * \param ftpwlen - The number of characters to write.
 * \return The number of bytes written to the socket. If less than ftpwlen, the buffer became full or the socket is not conected.
 */
WORD FTPWrite(TCP_SOCKET ftpsockwr , BYTE* ftpstrtowr , int ftpwlen)
{
	return TCPWrite(ftpsockwr , (char *)ftpstrtowr , ftpwlen);
}


/**
 * FTPClose - Closes the client socket specified by the handle.
 * \param Sockclose - The handle of the socket to close (the handle returned by the command FTPClientOpen).
 * \return None.
 */
void FTPClose (TCP_SOCKET Sockclose)
{
	TCPGenericClose(Sockclose);
}


/**
 * FTPisConn - Verifies the connection of a remote FTP device with the socket.
 * \param sockcon - The handle of the socket to control (the handle returned by the command FTPOpen).
 * \return TRUE - The remote connection is established.
 * \return FALSE - The remote connection is not established.
 */
BOOL FTPisConn(TCP_SOCKET sockcon)
{
	return TCPisConn(sockcon);
}


/**
 * FTPRxLen - Verifies how many bytes can be read from the specified FTP socket.
 * \param ftpsocklen - The handle of the socket to control (the handle returned by the command FTPOpen).
 * \return The number of bytes available to be read.
 */
WORD FTPRxLen(TCP_SOCKET ftpsocklen)
{
	return TCPRxLen(ftpsocklen);
}
