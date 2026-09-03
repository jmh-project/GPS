/******************************************************************************
*       File name	: DataLog_Init.c 
*		Description 	: 7인치 데이터로거 하드웨어 구조체 초기화
*		Company	: ziontech co.,
*		Version		: 0.10 (2015년 4월23일 Start)
*		Contact		: www.ziontek.co.kr
******************************************************************************/
/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
#include "DataLog.h"

/******************************************************************************
							Constant & Macros
******************************************************************************/
/******************************************************************************
							Private & Local Variables
******************************************************************************/

/******************************************************************************
							Function Prototype
******************************************************************************/

/******************************************************************************
							Example Program
******************************************************************************/


int fnSensorReceiveValueProcess(int ch)//ch는 USART 채널을 의미한다.
{
    #ifdef YSI_ENABLE
	S8 		abuf[400];
	int		nFlag_DispItems[50];
	int		nBufferCount=0;
	int		nCnt=0;
	//int		hCount;
	int		nFontCount=0;
	//int  	nDispItemsCount=0;
	int 	    cnt2;
	//int		cnt = 0;
	int		i=0;
	int		valueCnt=0;
	//static	int	waitCnt=0;
	//SCREEN_MODE 에서 수신한 데이터가 아니면 문제가 발생한다.
	for(i=0;i<20;i++)   abuf[i]=0;
	for(i=0;i<50;i++)   nFlag_DispItems[i]=0;
//*******************************************************************************************************	
//		통신 에러발생을 막기 위해 추가 
//		공정된 데이터를 확인하여 문제가 있는지 체크한다.
//*******************************************************************************************************
	gv.comportConnectionError[ch]=0;//공통으로 사용한다.
	if(SensorDataErrorCheck(ch)==0)
	{
		return 0;
	}
    //Dprintf("gv.nFlag_ReceiveDataProcess = %d gv.nFlag_Sensor_DataRead = %d\n",gv.nFlag_ReceiveDataProcess[ch],gv.nFlag_Sensor_DataRead[ch] );
	if((gv.nFlag_ReceiveDataProcess[ch]==ON)&&(gv.nFlag_Sensor_DataRead[ch]==SCREEN_MODE))
	{ 
		
		gv.nFlag_ReceiveDataProcess[ch]=OFF;
		//여기서 저장된 데이터는 시간 출력에 적용된다.
		
		gv.timeBuf[0]=gv.aItemReceiveBuffer[USART_1][nBufferCount++];// 2
		gv.timeBuf[1]=gv.aItemReceiveBuffer[USART_1][nBufferCount++];// 0
		gv.timeBuf[2]=gv.aItemReceiveBuffer[USART_1][nBufferCount++];// 0
		gv.timeBuf[3]=gv.aItemReceiveBuffer[USART_1][nBufferCount++];// 0
		gv.timeBuf[4]=gv.aItemReceiveBuffer[USART_1][nBufferCount++];// /
		gv.timeBuf[5]=gv.aItemReceiveBuffer[USART_1][nBufferCount++];// 1			
		gv.timeBuf[6]=gv.aItemReceiveBuffer[USART_1][nBufferCount++];// 1
		gv.timeBuf[7]=gv.aItemReceiveBuffer[USART_1][nBufferCount++];// /
		gv.timeBuf[8]=gv.aItemReceiveBuffer[USART_1][nBufferCount++];// 0
		gv.timeBuf[9]=gv.aItemReceiveBuffer[USART_1][nBufferCount++];// 1
		gv.timeBuf[10]=gv.aItemReceiveBuffer[USART_1][nBufferCount++];//뛰어쓰기
		gv.timeBuf[11]=gv.aItemReceiveBuffer[USART_1][nBufferCount++];// 1
		gv.timeBuf[12]=gv.aItemReceiveBuffer[USART_1][nBufferCount++];// 1
		gv.timeBuf[13]=gv.aItemReceiveBuffer[USART_1][nBufferCount++];// : 
		gv.timeBuf[14]=gv.aItemReceiveBuffer[USART_1][nBufferCount++];// 1
		gv.timeBuf[15]=gv.aItemReceiveBuffer[USART_1][nBufferCount++];// 1
		gv.timeBuf[16]=gv.aItemReceiveBuffer[USART_1][nBufferCount++];// :
		gv.timeBuf[17]=gv.aItemReceiveBuffer[USART_1][nBufferCount++];// 1
		gv.timeBuf[18]=gv.aItemReceiveBuffer[USART_1][nBufferCount++];// 1
		gv.timeBuf[19]=0;
		// 	0x20 0x20 0x30 0x31 0x32 0x20 0x20 사이에 있는 문자를 골라낸다. 
		//	0x20 이면 시작 ~ 문자 ~ 0x20 종료 
		//	nFlag_0x20=1;  ~      ~ nFlag_0x20=0 // 문자출력 

		nFontCount=0;
		valueCnt = 0;		
		//int	fn420mAOutput( float lowlimit, float highlimit, float fValue, int channel )
		for(nCnt=0; nCnt < 400; nCnt++) 
		{
			if((gv.aItemReceiveBuffer[ch][nBufferCount] != 0x20)&&(gv.aItemReceiveBuffer[ch][nBufferCount] != 0x0d)&&(gv.aItemReceiveBuffer[ch][nBufferCount]!=0x0a)) //0x20이 아닌 데이터는 모두 abuf 에 저장한다.
			{			
				abuf[nFontCount++]=gv.aItemReceiveBuffer[ch][nBufferCount];
				//Dprintf("%c",gv.aItemReceiveBuffer[nBufferCount]);
			}
			else
			{
				//Dprintf("gv.nFlag_SelectItem_Temp = %d[ %s ]\n",gv.nFlag_SelectItem_Temp,abuf);
				if(nFontCount>0)
				{
					//TEMPERATURE
					if((flash.nFlag_SelectItem_Temp[ch]==ON)&&(nFlag_DispItems[0]==0))
					{
						nFlag_DispItems[0]=1;//출력 실행 완료 Flag
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					//SPCOND
					else if((flash.nFlag_SelectItem_SpCond[ch]==ON)&&(nFlag_DispItems[1]==0))
					{
						nFlag_DispItems[1]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					//COND
					else if((flash.nFlag_SelectItem_Cond[ch]==ON)&&(nFlag_DispItems[2]==0))
					{
						nFlag_DispItems[2]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					//RESIST
					else if((flash.nFlag_SelectItem_Resist[ch]==ON)&&(nFlag_DispItems[3]==0))
					{
						nFlag_DispItems[3]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					//TDS
					else if((flash.nFlag_SelectItem_TDS[ch]==ON)&&(nFlag_DispItems[4]==0))
					{					
						nFlag_DispItems[4]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}	
					//SAL
					else if((flash.nFlag_SelectItem_Sal[ch]==ON)&&(nFlag_DispItems[5]==0))	
					{
						nFlag_DispItems[5]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					//DOSAT
					else if((flash.nFlag_SelectItem_DOSat[ch]==ON)&&(nFlag_DispItems[6]==0))	
					{
						nFlag_DispItems[6]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					//DO
					else if((flash.nFlag_SelectItem_DO[ch]==ON)&&(nFlag_DispItems[7]==0))	
					{

						nFlag_DispItems[7]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					//DOCHARG
					else if((flash.nFlag_SelectItem_DOChrg[ch]==ON)&&(nFlag_DispItems[8]==0))	
					{
						nFlag_DispItems[8]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}

					//DOsat %Local
					else if((flash.nFlag_SelectItem_DOSatLocal[ch]==ON)&&(nFlag_DispItems[9]==0))	
					{
						nFlag_DispItems[9]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}

					//PressureAbs psia
					else if((flash.nFlag_SelectItem_PressAbs[ch]==ON)&&(nFlag_DispItems[10]==0))
					{					
						nFlag_DispItems[10]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}	
					//Pressure psir
					else if((flash.nFlag_SelectItem_PressPsir[ch]==ON)&&(nFlag_DispItems[11]==0))
					{					
						nFlag_DispItems[11]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}	
					//PressureGau psig
					else if((flash.nFlag_SelectItem_PressGau[ch]==ON)&&(nFlag_DispItems[12]==0))
					{					
						nFlag_DispItems[12]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}	
					//Pressure psi
					else if((flash.nFlag_SelectItem_PressPsi[ch]==ON)&&(nFlag_DispItems[13]==0))
					{					
						nFlag_DispItems[13]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}	
					//DEPTH
					else if((flash.nFlag_SelectItem_Depth[ch]==ON)&&(nFlag_DispItems[14]==0))
					{
						nFlag_DispItems[14]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}	
					//PH
					else if((flash.nFlag_SelectItem_pH[ch]==ON)&&(nFlag_DispItems[15]==0))
					{
						nFlag_DispItems[15]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					else if((flash.nFlag_SelectItem_pHmV[ch]==ON)&&(nFlag_DispItems[16]==0))
					{
						nFlag_DispItems[16]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					else if((flash.nFlag_SelectItem_ORP[ch]==ON)&&(nFlag_DispItems[17]==0))
					{
						nFlag_DispItems[17]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					else if((flash.nFlag_SelectItem_Ammonium[ch]==ON)&&(nFlag_DispItems[18]==0))
					{
						nFlag_DispItems[18]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					else if((flash.nFlag_SelectItem_Ammonia[ch]==ON)&&(nFlag_DispItems[19]==0))
					{
						nFlag_DispItems[19]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					else if((flash.nFlag_SelectItem_Nitrate[ch]==ON)&&(nFlag_DispItems[20]==0))
					{
						nFlag_DispItems[20]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					else if((flash.nFlag_SelectItem_Chloride[ch]==ON)&&(nFlag_DispItems[21]==0))
					{
						nFlag_DispItems[21]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					else if((flash.nFlag_SelectItem_PAR1[ch]==ON)&&(nFlag_DispItems[22]==0))
					{
						nFlag_DispItems[22]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					else if((flash.nFlag_SelectItem_PAR2[ch]==ON)&&(nFlag_DispItems[23]==0))
					{
						nFlag_DispItems[23]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					else if((flash.nFlag_SelectItem_Turbidity6026[ch]==ON)&&(nFlag_DispItems[24]==0))
					{
						nFlag_DispItems[24]=1;
						abuf[nFontCount++]='0';
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
                    /*
					else if((flash.nFlag_SelectItem_Fluorescence[ch]==ON)&&(nFlag_DispItems[26]==0))
					{
						nFlag_DispItems[26]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
                    */
					else if((flash.nFlag_SelectItem_Rhodamine[ch]==ON)&&(nFlag_DispItems[27]==0))
					{
						nFlag_DispItems[27]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					else if((flash.nFlag_SelectItem_Turbidity6136[ch]==ON)&&(nFlag_DispItems[28]==0))
					{
						nFlag_DispItems[28]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					else if((flash.nFlag_SelectItem_Chlorophyll[ch]==ON)&&(nFlag_DispItems[25]==0))
					{
						nFlag_DispItems[25]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}					
                    //ChlorophyllRFU 대치
					else if((flash.nFlag_SelectItem_Fluorescence[ch]==ON)&&(nFlag_DispItems[37]==0))
					{
						nFlag_DispItems[37]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}					
					else if((flash.nFlag_SelectItem_ODO[ch]==ON)&&(nFlag_DispItems[33]==0))
					{
                        
						nFlag_DispItems[33]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						//Dprintf("S-ODO = %7.2f\n",flash.Uart[ch]->SensorPV[valueCnt]);
                        valueCnt++;
						if(valueCnt>=8) return 0;
					}
                    
					else if((flash.nFlag_SelectItem_Ground[ch]==ON)&&(nFlag_DispItems[30]==0))
					{
						nFlag_DispItems[30]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					else if((flash.nFlag_SelectItem_FullScale[ch]==ON)&&(nFlag_DispItems[31]==0))
					{
						nFlag_DispItems[31]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					else if((flash.nFlag_SelectItem_BGAPC[ch]==ON)&&(nFlag_DispItems[34]==0))
					{
						nFlag_DispItems[34]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					else if((flash.nFlag_SelectItem_BGAPCRFU[ch]==ON)&&(nFlag_DispItems[36]==0))
					{
						nFlag_DispItems[36]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					else if((flash.nFlag_SelectItem_BGAPE[ch]==ON)&&(nFlag_DispItems[35]==0))
					{
						nFlag_DispItems[35]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					else if((flash.nFlag_SelectItem_ODOSet[ch]==ON)&&(nFlag_DispItems[32]==0))
					{
						nFlag_DispItems[32]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
					else if((flash.nFlag_SelectItem_Bat[ch]==ON)&&(nFlag_DispItems[29]==0))
					{
						nFlag_DispItems[29]=1;
						abuf[nFontCount]=0;
						flash.Uart[ch]->SensorPV[valueCnt]=atof(abuf);
						valueCnt++;
						if(valueCnt>=8) return 0;
					}
				}
				nFontCount=0;	
				for(cnt2=0;cnt2<20;cnt2++)
				{
					abuf[cnt2]=0;
				}
			}
			nBufferCount++;
		}
	}
	#endif
	return 0;
}
