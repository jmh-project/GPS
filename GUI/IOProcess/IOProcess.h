/******************************************************************************
*    	File name	: IOPrecess.h 
*		Description : Configuration File
*		Company		: idnics co.,
*		Version		: 1.00
******************************************************************************/
#ifndef IO_PROCESS
    #define IO_PROCESS

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct __IOProcess
{
	int			Status;				
	HANDLE		Handle;				
	char		Name[32]; 
	int	 		(*TimerEvent)(HANDLE , int , int , void *);
	int	 		(*PrepareEvent)(HANDLE , int , int , void *);
} stIOProcess;
	 
void 	IOTimerInit(void);
void 	IOProcessInit(void);
HANDLE 	GetIOProcessHandle(void);
int 	GetIOProcessCount(void);
stIOProcess *FindIOProcessPointer(HANDLE handle);
stIOProcess *FindIOProcess(HANDLE handle);
stIOProcess *GetIOProcess(HANDLE handle);
stIOProcess *GetIOProcessByName(char *name);
int IOProcessTest(void);

HANDLE makeIOProcess(char *Name, 
		int	(*PrepareEvent)(HANDLE , int , int , void *),
		int	(*TimerEvent)(HANDLE , int , int , void *)
);


int IOOpenTimer(HANDLE handle, int interval);
int IOStartTimer(int id);
int IOStopTimer(int id);
int IOCloseTimer(int id);
int IOSetTimerInterval(int id, int interval);
#ifdef __cplusplus
}
#endif

#endif // IO_PROCESS
