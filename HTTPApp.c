 
/****************************************************************************
  SECTION 	Include
****************************************************************************/

#include "TCPIP Stack/TCPIP.h"
#if defined(STACK_USE_HTTP2_SERVER)

#include "stdlib.h"
#include "string.h"
#include "Sensor.h"
#include "Config.h"

/****************************************************************************
  SECTION	Define
****************************************************************************/
#define __HTTPAPP_C


extern struct Sensor_t sensors[];

/****************************************************************************
  SECTION 	Authorization Handlers
****************************************************************************/
 

/*****************************************************************************
  FUNCTION	BYTE HTTPNeedsAuth(BYTE* cFile)

  This function is used by the stack to decide if a page is access protected.
  If the function returns 0x00, the page is protected, if returns 0x80, no 
  authentication is required
*****************************************************************************/
#if defined(HTTP_USE_AUTHENTICATION)
BYTE HTTPNeedsAuth(BYTE* cFile)
{
	//	If you want to restrict the access to some page, include it in the folder "protect"
	//	here you can change the folder, or add others
	if(memcmppgm2ram(cFile, (ROM void*)"protect", 7) == 0)
		return 0x00;		// Authentication will be needed later

	// You can match additional strings here to password protect other files.
	// You could switch this and exclude files from authentication.
	// You could also always return 0x00 to require auth for all files.
	// You can return different values (0x00 to 0x79) to track "realms" for below.

	return 0x80;			// No authentication required
}
#endif

/*****************************************************************************
  FUNCTION	BYTE HTTPCheckAuth(BYTE* cUser, BYTE* cPass)
	
  This function checks if username and password inserted are acceptable

  ***************************************************************************/
#if defined(HTTP_USE_AUTHENTICATION)
BYTE HTTPCheckAuth(BYTE* cUser, BYTE* cPass)
{
	if(strcmppgm2ram((char *)cUser,(ROM char *)"admin") == 0
		&& strcmppgm2ram((char *)cPass, (ROM char *)"flyport") == 0)
		return 0x80;		// We accept this combination
	
	// You can add additional user/pass combos here.
	// If you return specific "realm" values above, you can base this 
	//   decision on what specific file or folder is being accessed.
	// You could return different values (0x80 to 0xff) to indicate 
	//   various users or groups, and base future processing decisions
	//   in HTTPExecuteGet/Post or HTTPPrint callbacks on this value.
	
	return 0x00;			// Provided user/pass is invalid
}
#endif

/****************************************************************************
  SECTION	GET/POST Form Handlers
****************************************************************************/
  
#define API_SET_SENSOR "set_sensor.cgi"


#define getHttpParam(KEY) HTTPGetROMArg(curHTTP.data, (ROM BYTE *)KEY)


/****************************************************************************
  FUNCTION	HTTP_IO_RESULT HTTPExecuteGet(void)
	
  This function processes every GET request from the pages.
*****************************************************************************/
HTTP_IO_RESULT HTTPExecuteGet(void)
{	
	BYTE filename[20];

	// Load the file name
	// Make sure BYTE filename[] above is large enough for your longest name
	MPFSGetFilename(curHTTP.file, filename, 20);
	//LCDWrite(filename);
	// If it's the LED updater file
	if(!memcmppgm2ram(filename, API_SET_SENSOR, strlen(API_SET_SENSOR)+1)) {
		int id                    = atoi(getHttpParam("address"));
		sensors[id].type          = atoi(getHttpParam("type"));
		sensors[id].enabled       = atoi(getHttpParam("enabled"));
		sensors[id].address       = id; /* CUIDADO: address is assumed equal to id */
		sensors[id].threshold     = atoi(getHttpParam("threshold"));
		sensors[id].triggerType   = atoi(getHttpParam("triggerType"));
		strcpy(getHttpParam("name"), sensors[id].name);

		saveConfig();
	}

	return HTTP_IO_DONE;
}



#ifdef HTTP_USE_POST
/****************************************************************************
  FUNCTION	HTTP_IO_RESULT HTTPExecutePost(void)
	
  This function processes every POST request from the pages. 
*****************************************************************************/
HTTP_IO_RESULT HTTPExecutePost(void)
{

	
	return HTTP_IO_DONE;
}
#endif


static void sendRespStr(const char* key, const char* value) {
	BYTE resp[30];
	sprintf(resp, "%s=%s;", key, value);
	TCPPutString(sktHTTP, resp);	
}

static void sendRespInt(const char* key, int value) {
	BYTE resp[30];
	sprintf(resp, "%s=%d;", key, value);
	TCPPutString(sktHTTP, resp);	
}

void HTTPPrint_sensor(WORD id) {

	Sensor* s = &sensors[id];
	
	sendRespInt("type", s->type);
	sendRespStr("name", s->name);
	sendRespInt("enabled", s->enabled);
	sendRespInt("address", s->address);
	sendRespInt("threshold", s->threshold);
	sendRespInt("triggerType", s->triggerType);
	sendRespInt("triggered", s->triggered);
	sendRespInt("timeTriggered", s->timeTriggered);
	sendRespInt("value", s->value);
	//LCDWrite("Read\n");
}



#endif
