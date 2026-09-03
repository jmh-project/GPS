/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
#include "DataLog.h"
volatile int goldCount[6];
int SensorDataErrorCheck(int ch)
{
	if(gv.aItemReceiveBuffer[ch][0]!='2')
	{
		gv.nFlag_ReceiveDataProcess[ch]=OFF;
		gv.nFlag_ReceiveStart[ch]=OFF;		//데이터 수신대기 
		gv.nReceiveBufferCount[ch]=0;		//데이터 버퍼 주소 초기화 
		Dprintf("YSI Time Set ERROR 1[%d]\n",ch);
		return 0;
	}
	if(gv.aItemReceiveBuffer[ch][1]!='0')
	{
		gv.nFlag_ReceiveDataProcess[ch]=OFF;
		gv.nFlag_ReceiveStart[ch]=OFF;		//데이터 수신대기 
		gv.nReceiveBufferCount[ch]=0;		//데이터 버퍼 주소 초기화 
		Dprintf("YSI Time Set ERROR 2[%d]\n",ch);
		return 0;
	}
	if(gv.aItemReceiveBuffer[ch][4]!='/')
	{
		gv.nFlag_ReceiveDataProcess[ch]=OFF;
		gv.nFlag_ReceiveStart[ch]=OFF;		//데이터 수신대기 
		gv.nReceiveBufferCount[ch]=0;		//데이터 버퍼 주소 초기화 
		Dprintf("YSI Time Set ERROR 3[%d]\n",ch);
		return 0;
	}
	if(gv.aItemReceiveBuffer[ch][7]!='/')
	{
		gv.nFlag_ReceiveDataProcess[ch]=OFF;
		gv.nFlag_ReceiveStart[ch]=OFF;		//데이터 수신대기 
		gv.nReceiveBufferCount[ch]=0;		//데이터 버퍼 주소 초기화 
		Dprintf("YSI Time Set ERROR 4[%d]\n",ch);
		return 0;
	}
	if(gv.aItemReceiveBuffer[ch][10]!=' ')
	{
		gv.nFlag_ReceiveDataProcess[ch]=OFF;
		gv.nFlag_ReceiveStart[ch]=OFF;		//데이터 수신대기 
		gv.nReceiveBufferCount[ch]=0;		//데이터 버퍼 주소 초기화 
		Dprintf("YSI Time Set ERROR 5[%d]\n",ch);
		return 0;
	}
	if(gv.aItemReceiveBuffer[ch][13]!=':')
	{
		gv.nFlag_ReceiveDataProcess[ch]=OFF;
		gv.nFlag_ReceiveStart[ch]=OFF;		//데이터 수신대기 
		gv.nReceiveBufferCount[ch]=0;		//데이터 버퍼 주소 초기화 
		Dprintf("YSI Time Set ERROR 6[%d]\n",ch);
		return 0;
	}
	if(gv.aItemReceiveBuffer[ch][16]!=':')
	{
		gv.nFlag_ReceiveDataProcess[ch]=OFF;
		gv.nFlag_ReceiveStart[ch]=OFF;		//데이터 수신대기 
		gv.nReceiveBufferCount[ch]=0;		//데이터 버퍼 주소 초기화 
		Dprintf("YSI Time Set ERROR 7[%d]\n",ch);
		return 0;
	}
	if(gv.ReceiveDataLength[ch] != goldCount[ch])//정확한 데이터 수신시 그때 데이터 길이를 기억하였다. 다음수신시 데이터 길이를 비교한다.
	{
		Dprintf("[YSI LENGTH ERROR]gv.nReceiveBufferCount = %d goldCount = %d\n",gv.ReceiveDataLength[ch],goldCount[ch] );
		goldCount[ch] = gv.ReceiveDataLength[ch];
		
		gv.nFlag_ReceiveDataProcess[ch]=OFF;	//수신데이터삭제
		gv.nFlag_ReceiveStart[ch]=OFF;			//데이터 수신대기 
		gv.nReceiveBufferCount[ch]=0;			//데이터 버퍼 주소 초기화 
		return 0;
	}
	
	return (1);
}
