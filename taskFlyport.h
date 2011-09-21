
//#include "taskTCPIP.h"


#include "string.h"
#include "TCPlib.h"
#include "UDPlib.h"
#include "WFlib.h"
#include "SMTPlib.h"


//	RTOS components - Semaphore and queues
extern xQueueHandle xQueue;
extern xSemaphoreHandle xSemFrontEnd;
extern xSemaphoreHandle xSemHW;


//	FrontEnd variables
extern BYTE xIPAddress[];
extern WORD xTCPPort;
extern TCP_SOCKET xSocket;
extern int xFrontEndStat;
extern int xErr;
extern BOOL xBool;
extern WORD xWord;
extern char *xChar;
extern BYTE *xByte;
extern BYTE xByte2;
extern BYTE xByte3;
extern SMTP_POINTERS SMTPClient;

void FlyportTask();
