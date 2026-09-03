#ifndef __DATALOG_HEADER
#define __DATALOG_HEADER

#ifdef __cplusplus
 extern "C" {
#endif

     
#define ULIK_ENABLE
     
     
#ifdef ULIK_ENABLE
     
extern int ULIK_Request(int comPort, int id, int function, int addr1, int addr2, int reg1, int reg2);     
extern int Ulik_HoldingRegisterTable_Reading(int comPort, int id);     
    
typedef struct __ULIK {
    int id;
} UlikStruct;
extern UlikStruct ulik;

#endif
     
 
#ifdef __cplusplus
}
#endif

#endif