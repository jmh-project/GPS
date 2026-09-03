/******************************************************************************
*    	File name	: EXO_Process.c 
*		Description : DongMun RH100 Control Program
*		Company		: idnics co.,
*		Version		: 1.00
*		Contact		: 
******************************************************************************/

/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
#include "DataLog.h"
volatile U32 W5500_IO_PROCESS = 0;
int W5500_IOPrepareEvent(HANDLE handle, int command, int param, void *Data)
{
    stIOProcess 	*io_process = FindIOProcess(handle);

    if(io_process==NULL) {
        Dprintf("IOPrepareEvent:: IO Precess not Found\n");
        return(0);
    }
    W5500_IO_PROCESS = IOOpenTimer(handle, 500);
    IOStartTimer(W5500_IO_PROCESS);
    return(1);
}

void IoBoardControl(void); 
void ConnectionStatus(void);
#ifdef JSON_ENABLE
int W5500_JsonServer(int slot);
#endif
int W5500_IOTimerEvent(HANDLE handle, int command, int param, void *Data)  
{
    stIOProcess 	*io_process = FindIOProcess(handle);
    if(io_process==NULL) {
        Dprintf("IOTimerEvent:: IO Precess not Found\n");
        return(0);
    }

//    if(flash.relay[0]==ON) flash.relay[0]=OFF;
//    else flash.relay[0]=ON;
//    if(flash.relay[1]==ON) flash.relay[1]=OFF;
//    else flash.relay[1]=ON;

//    IoBoardControl();


#ifdef JSON_ENABLE    
    W5500_JsonServer(0); 
#endif
    //W5500_Server(0);
    //W5500_Server(1);
    //ConnectionStatus();    
    return(1);
}
int W5500_IOProcess(void)
{
    Dprintf("[ENABLE] W5500_IOProcess\n");
    makeIOProcess("W5500", W5500_IOPrepareEvent, W5500_IOTimerEvent);
    return(1);
}
