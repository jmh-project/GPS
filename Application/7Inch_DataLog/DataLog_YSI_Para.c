/******************************************************************************
*    	File name	: SmartLogStart.c 
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

/******************************************************************************
							Constant & Macros
******************************************************************************/
/******************************************************************************
							Private & Local Variables
******************************************************************************/
/******************************************************************************
							Function Prototype
******************************************************************************/
int fnSensor_PARAX_Command_Process( int ch )
{
	int		i=0;
#ifdef YSI_ENABLE
	int		idCnt=0;
	int		nItemSelect=0;
	char	cItmeCodeBuffer[10];
	char	cItemCodeBufferAddress=0;
	for(int nCnt=0;nCnt<10;nCnt++)	
		cItmeCodeBuffer[nCnt]=0;
#endif
	for(i=0;i<USART_SENSOR_MAX;i++)
	{
		flash.Uart[ch]->SetupName[i][0]='N';
		flash.Uart[ch]->SetupName[i][1]='O';
		flash.Uart[ch]->SetupName[i][2]='N';
		flash.Uart[ch]->SetupName[i][3]='E';
		flash.Uart[ch]->SetupName[i][4]=0;
		flash.Uart[ch]->SetupName[i][5]=0;
		flash.Uart[ch]->SetupName[i][6]=0;
		flash.Uart[ch]->SetupName[i][7]=0;
		flash.Uart[ch]->SetupName[i][8]=0;
		flash.Uart[ch]->SetupName[i][9]=0;
		flash.Uart[ch]->SetupName[i][10]=0;
		flash.Uart[ch]->SetupName[i][11]=0;
		flash.Uart[ch]->SetupName[i][12]=0;
		flash.Uart[ch]->SetupName[i][13]=0;
		flash.Uart[ch]->SetupName[i][14]=0;
		flash.Uart[ch]->SetupName[i][15]=0;
		flash.Uart[ch]->SetupName[i][16]=0;
		flash.Uart[ch]->SetupName[i][17]=0;
		flash.Uart[ch]->SetupName[i][18]=0;
		flash.Uart[ch]->SetupName[i][19]=0;

		flash.Uart[ch]->SetupName[i][0]='N';
		flash.Uart[ch]->SetupName[i][1]='O';
		flash.Uart[ch]->SetupName[i][2]='N';
		flash.Uart[ch]->SetupName[i][3]='E';
		flash.Uart[ch]->SetupName[i][4]=0;
		flash.Uart[ch]->SetupName[i][5]=0;
		flash.Uart[ch]->SetupName[i][6]=0;
		flash.Uart[ch]->SetupName[i][7]=0;
		flash.Uart[ch]->SetupName[i][8]=0;
		flash.Uart[ch]->SetupName[i][9]=0;
		flash.Uart[ch]->SetupName[i][10]=0;
		flash.Uart[ch]->SetupName[i][11]=0;
		flash.Uart[ch]->SetupName[i][12]=0;
		flash.Uart[ch]->SetupName[i][13]=0;
		flash.Uart[ch]->SetupName[i][14]=0;
		flash.Uart[ch]->SetupName[i][15]=0;
		flash.Uart[ch]->SetupName[i][16]=0;
		flash.Uart[ch]->SetupName[i][17]=0;
		flash.Uart[ch]->SetupName[i][18]=0;
		flash.Uart[ch]->SetupName[i][19]=0;
	}
#ifdef YSI_ENABLE
	flash.nFlag_SelectItem_Temp[ch]=0;
	flash.nSelectItem_Temp_Unit[ch]=0;
	
	flash.nFlag_SelectItem_SpCond[ch]=0;
	flash.nFlag_SelectItem_SpCond_Unit[ch]=0;
	
	flash.nFlag_SelectItem_Cond[ch]=0;
	flash.nFlag_SelectItem_Cond_Unit[ch]=0;
	
	flash.nFlag_SelectItem_Resist[ch]=0;
	flash.nFlag_SelectItem_Resist_Unit[ch]=0;
	
	flash.nFlag_SelectItem_TDS[ch]=0;
	flash.nFlag_SelectItem_TDS_Unit[ch]=0;
	
	flash.nFlag_SelectItem_Sal[ch]=0;
	flash.nFlag_SelectItem_Sal_Unit[ch]=0;
	
	flash.nFlag_SelectItem_DOSat[ch]=0;
	flash.nFlag_SelectItem_DOSat_Unit[ch]=0;
	
	flash.nFlag_SelectItem_DO[ch]=0;
	flash.nFlag_SelectItem_DO_Unit[ch]=0;
	
	flash.nFlag_SelectItem_DOChrg[ch]=0;
	flash.nFlag_SelectItem_DOChrg_Unit[ch]=0;
	
	flash.nFlag_SelectItem_DOSatLocal[ch]=0;
	flash.nFlag_SelectItem_DOSatLocal_Unit[ch]=0;

	flash.nFlag_SelectItem_PressAbs[ch]=0;
	flash.nSelectItem_PressAbs_Unit[ch]=0;

	flash.nFlag_SelectItem_PressPsir[ch]=0;
	flash.nSelectItem_PressPsir_Unit[ch]=0;

	flash.nFlag_SelectItem_PressGau[ch]=0;
	flash.nSelectItem_PressGau_Unit[ch]=0;

	flash.nFlag_SelectItem_PressPsi[ch]=0;
	flash.nSelectItem_PressPsi_Unit[ch]=0;
	
	flash.nFlag_SelectItem_Depth[ch]=0;
	flash.nSelectItem_Depth_Unit[ch]=0;
	
	flash.nFlag_SelectItem_pH[ch]=0;
	flash.nFlag_SelectItem_pH_Unit[ch]=0;
	
	flash.nFlag_SelectItem_pHmV[ch]=0;

	flash.nFlag_SelectItem_ORP[ch]=0;
	
	flash.nFlag_SelectItem_Ammonium[ch]=0;
	flash.nFlag_SelectItem_Ammonium_Uint[ch]=0;

	flash.nFlag_SelectItem_Ammonia[ch]=0;
	flash.nFlag_SelectItem_Ammonia_Uint[ch]=0;

	flash.nFlag_SelectItem_Nitrate[ch]=0;
	flash.nFlag_SelectItem_Nitrate_Uint[ch]=0;

	flash.nFlag_SelectItem_Chloride[ch]=0;
	flash.nFlag_SelectItem_Chloride_Uint[ch]=0;

	flash.nFlag_SelectItem_PAR1[ch]=0;
	flash.nFlag_SelectItem_PAR1_Uint[ch]=0;

	flash.nFlag_SelectItem_PAR2[ch]=0;
	flash.nFlag_SelectItem_PAR2_Uint[ch]=0;

	flash.nFlag_SelectItem_Turbidity6026[ch]=0;
	flash.nFlag_SelectItem_Turbidity6026_Uint[ch]=0;
	
	flash.nFlag_SelectItem_Chlorophyll[ch]=0;
	flash.nFlag_SelectItem_Chlorophyll_Uint[ch]=0;


	flash.nFlag_SelectItem_Fluorescence[ch]=0;
	flash.nFlag_SelectItem_Fluorescence_Uint[ch]=0;


	flash.nFlag_SelectItem_Rhodamine[ch]=0;
	flash.nFlag_SelectItem_Rhodamine_Uint[ch]=0;

	flash.nFlag_SelectItem_Turbidity6136[ch]=0;
	flash.nFlag_SelectItem_Turbidity6136_Uint[ch]=0;

	flash.nFlag_SelectItem_RhodaminWT[ch]=0;

	flash.nFlag_SelectItem_ODOSet[ch]=0;

	flash.nFlag_SelectItem_ODO[ch]=0;
	
	flash.nFlag_SelectItem_BGAPC[ch]=0;
	flash.nFlag_SelectItem_BGAPC_Uint[ch]=0;


	flash.nFlag_SelectItem_BGAPE[ch]=0;
	flash.nFlag_SelectItem_BGAPE_Uint[ch]=0;

	flash.nFlag_SelectItem_BGAPCRFU[ch]=0;
	flash.nFlag_SelectItem_BGAPCRFU_Uint[ch]=0;


	flash.nFlag_SelectItem_CHL[ch]=0;
	flash.nFlag_SelectItem_CHL_Uint[ch]=0;

	flash.nFlag_SelectItem_Bat[ch]=0;
	
	flash.nFlag_SelectItem_Ground[ch]=0;

	flash.nFlag_SelectItem_FullScale[ch]=0;	
	//Dprintf("gv.nReceiveItemNameBufCount = %d",gv.nReceiveItemNameBufCount);		
	//Dprintf("fnSensor_PARAX_Command_Process :: UsartPort = %d\n",ch);
	flash.Uart[ch]->ItemCnt = 0;
	for(int nCnt=0;nCnt < gv.nReceiveItemNameBufCount; nCnt++ )
	{
		if(gv.aItemReceiveBuffer[ch][nCnt]==0x20)
		{
			if((cItmeCodeBuffer[0]>=0x30)&&(cItmeCodeBuffer[0]<=0x39))
			{
				if(cItmeCodeBuffer[1]==0x20)
				{
					switch(cItmeCodeBuffer[0])
					{
						case '0': nItemSelect=0;break;
						case '1': nItemSelect=1;break;
						case '2': nItemSelect=2;break;
						case '3': nItemSelect=3;break;
						case '4': nItemSelect=4;break;
						case '5': nItemSelect=5;break;
						case '6': nItemSelect=6;break;
						case '7': nItemSelect=7;break;
						case '8': nItemSelect=8;break;
						case '9': nItemSelect=9;break;
					}
				}
				else
				{
					if(cItmeCodeBuffer[2]==0x20)
						cItmeCodeBuffer[2]=0;
					else
						cItmeCodeBuffer[3]=0;
						
					nItemSelect=atoi(cItmeCodeBuffer);					
				}
				for(i=0;i<10;i++)	
					cItmeCodeBuffer[i]=0;
				cItemCodeBufferAddress=0;		
                Dprintf("%d ",nItemSelect);                
				switch(nItemSelect)
				{
                    
                   
					//Time ==========================================================================================================
					case 51: 
					{
						//Dprintf("Time, ");
					}
					break;
					case 52: 
					{
						//Dprintf("Data, ");
					}
					break;
					case 53: 
					{
						//Dprintf("Data, ");
					}
					break;
					case 54: 
					{
						//Dprintf("Time, ");
					}
					break;
					//Temperature.c ==========================================================================================================
					case 1: 
					{
						flash.nFlag_SelectItem_Temp[ch]=ON;
						flash.nSelectItem_Temp_Unit[ch]=2;
						idCnt=0;	
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='T';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='E';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='M';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='P';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='C';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;

						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Temperature.f
					case 2: 
					{
						flash.nFlag_SelectItem_Temp[ch]=ON;
						flash.nSelectItem_Temp_Unit[ch]=3;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='T';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='E';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='M';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='P';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='F';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Temperature.k
					case 3: 
					{
						flash.nFlag_SelectItem_Temp[ch]=ON;
						flash.nSelectItem_Temp_Unit[ch]=4;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='T';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='E';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='M';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='P';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='K';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//spcond mS/cm ==========================================================================================================
					case 6:
					{
						flash.nFlag_SelectItem_SpCond[ch]=ON;
						flash.nFlag_SelectItem_SpCond_Unit[ch]=2;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='S';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='p';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='C';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='o';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='n';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='d';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='S';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='/';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='c';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//spcond uS/cm
					case 7:
					{
						flash.nFlag_SelectItem_SpCond[ch]=ON;
						flash.nFlag_SelectItem_SpCond_Unit[ch]=3;
						idCnt=0;

						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='S';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='p';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='C';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='o';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='n';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='d';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='u';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='S';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='/';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='c';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;

					}
					break;
					//cond mS/cm ==========================================================================================================
					case 4:
					{
						flash.nFlag_SelectItem_Cond[ch]=ON;
						flash.nFlag_SelectItem_Cond_Unit[ch]=2;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='C';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='o';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='n';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='d';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='S';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='/';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='c';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
 						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//cond uS/cm
					case 5:
					{
						flash.nFlag_SelectItem_Cond[ch]=ON;
						flash.nFlag_SelectItem_Cond_Unit[ch]=3;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='C';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='o';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='n';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='d';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='u';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='S';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='/';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='c';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
 						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Resist MOhm.cm ==========================================================================================================
					case 9:
					{
						flash.nFlag_SelectItem_Resist[ch]=ON;
						flash.nFlag_SelectItem_Resist_Unit[ch]=2;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='R';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='i';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='t';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='M';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='O';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='h';
 						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
 						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='*';
 						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='c';
 						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Resist KOhm.cm
					case 8:
					{
						flash.nFlag_SelectItem_Resist[ch]=ON;
						flash.nFlag_SelectItem_Resist_Unit[ch]=3;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='R';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='i';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='t';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='K';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='O';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='h';
 						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
 						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='*';
 						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='c';
 						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Resist Ohm.cm
					case 94:
					{
						flash.nFlag_SelectItem_Resist[ch]=ON;
						flash.nFlag_SelectItem_Resist_Unit[ch]=4;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='R';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='i';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='t';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='O';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='h';
 						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
 						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='*';
 						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='c';
 						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//TDS g/L ==========================================================================================================
					case 10:
					{
						flash.nFlag_SelectItem_TDS[ch]=ON;
						flash.nFlag_SelectItem_TDS_Unit[ch]=2;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='T';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='D';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='S';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='g';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='/';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='L';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//TDS Kg/L
					case 95:
					{
						flash.nFlag_SelectItem_TDS[ch]=ON;
						flash.nFlag_SelectItem_TDS_Unit[ch]=3;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='T';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='D';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='S';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='K';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='g';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='/';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='L';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Salinity PPT ==========================================================================================================
					case 12:
					{
						flash.nFlag_SelectItem_Sal[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='S';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='a';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='l';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='i';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='n';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='i';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='t';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='y';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='p';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='p';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='t';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//DOsat % ==========================================================================================================
					case 14:
					{
						flash.nFlag_SelectItem_DOSat[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='D';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='O';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='a';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='t';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=' ';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='%';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=' ';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//DO mg/L
					case 15:
					{
						flash.nFlag_SelectItem_DO[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='D';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='O';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='g';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='/';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='L';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//DO Charge DOchrg
					case 96:
					{
						flash.nFlag_SelectItem_DOChrg[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='D';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='O';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='C';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='h';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='a';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='g';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//DOsat %Local
					case 200:
					{
						flash.nFlag_SelectItem_DOSatLocal[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='D';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='O';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='a';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='t';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='%';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='L';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='o';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='c';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='a';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='l';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//PressureAbs psia ==========================================================================================================
					case 20:
					{
						flash.nFlag_SelectItem_PressAbs[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='P';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='u';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='-';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='A';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='b';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='p';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='i';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='a';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Pressure psir
					case 104:
					{
						flash.nFlag_SelectItem_PressPsir[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='P';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='u';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='p';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='i';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//PressureGau psig
					case 21:
					{
						flash.nFlag_SelectItem_PressGau[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='P';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='u';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='-';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='G';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='a';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='u';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='p';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='i';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='g';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Pressure psi
					case 111:
					{
						flash.nFlag_SelectItem_PressPsi[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='P';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='u';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='p';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='i';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Depth meters ==========================================================================================================
					case 22:
					{
						flash.nFlag_SelectItem_Depth[ch]=ON;
						flash.nSelectItem_Depth_Unit[ch]=2;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='D';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='p';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='t';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='h';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='t';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Depth ft
					case 23:
					{
						flash.nFlag_SelectItem_Depth[ch]=ON;
						flash.nSelectItem_Depth_Unit[ch]=3;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='D';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='p';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='t';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='h';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='f';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='t';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//pH pH ==========================================================================================================
					case 18:
					{
						flash.nFlag_SelectItem_pH[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='p';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='H';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//pH mV
					case 17:
					{
						flash.nFlag_SelectItem_pHmV[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='p';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='H';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='V';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//ORP mV ==========================================================================================================
					case 19:
					{
						flash.nFlag_SelectItem_ORP[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='O';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='R';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='P';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='V';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Ammonium mg N/L ==========================================================================================================
					case 48:
					{
						flash.nFlag_SelectItem_Ammonium[ch]=ON;
						flash.nFlag_SelectItem_Ammonium_Uint[ch]=2;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='A';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='o';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='n';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='i';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='u';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='g';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='N';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='/';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='L';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Ammonium mV
					case 108:
					{
						flash.nFlag_SelectItem_Ammonium[ch]=ON;
						flash.nFlag_SelectItem_Ammonium_Uint[ch]=3;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='A';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='o';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='n';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='i';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='u';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='V';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Ammonia mg N/L==========================================================================================================
					case 47:
					{
						flash.nFlag_SelectItem_Ammonia[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='A';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='o';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='n';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='i';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='a';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='g';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='N';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='/';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='L';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Nitrate mg N/L ==========================================================================================================
					case 106:
					{
						flash.nFlag_SelectItem_Nitrate[ch]=ON;
						flash.nFlag_SelectItem_Nitrate_Uint[ch]=2;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='N';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='i';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='t';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='a';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='t';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='g';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='N';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='/';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='L';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Nitrate
					case 101:
					{
						flash.nFlag_SelectItem_Nitrate[ch]=ON;
						flash.nFlag_SelectItem_Nitrate_Uint[ch]=3;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='N';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='i';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='t';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='a';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='t';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='V';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Chloride mg/L ==========================================================================================================
					case 112:
					{
						flash.nFlag_SelectItem_Chloride[ch]=ON;
						flash.nFlag_SelectItem_Chloride_Uint[ch]=2;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='C';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='h';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='o';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='i';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='d';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='g';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='/';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='L';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Chloride mV
					case 145:
					{
						flash.nFlag_SelectItem_Chloride[ch]=ON;
						flash.nFlag_SelectItem_Chloride_Uint[ch]=3;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='C';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='h';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='o';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='i';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='d';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='V';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//PAR1 ==========================================================================================================
					case 201:
					{
						flash.nFlag_SelectItem_PAR1[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='P';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='A';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='R';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='1';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//PAR2
					case 202:
					{
						flash.nFlag_SelectItem_PAR2[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='P';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='A';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='R';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='2';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Turbidity 6026 NTU ==========================================================================================================
					case 37:
					{
						flash.nFlag_SelectItem_Turbidity6026[ch]=ON;
						idCnt=0;
						if(flash.ysiSensorNameChange==OFF)
						{
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='T';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='u';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='b';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='i';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='d';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='i';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='t';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='6';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='0';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='2';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='6';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						}
						else
						{
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='S';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='S';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='g';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='/';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='L';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=NULL;
						}
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Chlorophyll ug/L ==========================================================================================================
					case 193:
					{
						flash.nFlag_SelectItem_Chlorophyll[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='C';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='h';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='l';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='o';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='o';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='p';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='h';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='y';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='l';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='l';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='u';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='g';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='/';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='L';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Fluorescence %FS ==========================================================================================================
					case 194:
					{
						flash.nFlag_SelectItem_Fluorescence[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='F';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='l';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='u';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='o';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='c';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='n';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='c';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						//flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='%';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='F';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='S';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=0;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Rhodamine ug/L ==========================================================================================================
					case 204:
					{
						flash.nFlag_SelectItem_Rhodamine[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='R';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='h';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='o';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='d';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='a';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='i';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='n';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='u';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='g';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='/';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='L';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Turbidity6136 NTU ==========================================================================================================
					case 203:
					{
						flash.nFlag_SelectItem_Turbidity6136[ch]=ON;
						idCnt=0;

						if(flash.ysiSensorNameChange==OFF)
						{
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='T';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='u';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='b';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='i';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='d';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='i';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='t';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='6';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='1';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='3';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='6';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						}
						else
						{
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='S';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='S';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='g';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='/';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='L';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
							flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=NULL;
						}
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Battery volts ==========================================================================================================
					case 28:
					{
						flash.nFlag_SelectItem_Bat[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='B';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='a';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='t';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='t';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='y';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Ground Hertz ==========================================================================================================
					case 98:
					{
						flash.nFlag_SelectItem_Ground[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='G';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='o';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='u';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='n';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='d';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='H';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='t';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='z';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					//Full Scale Hertz ==========================================================================================================
					case 99:
					{
						flash.nFlag_SelectItem_FullScale[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='F';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='u';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='l';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='l';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='S';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='c';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='a';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='l';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='H';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='r';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='z';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}
					break;
					case 211:
					{
						flash.nFlag_SelectItem_ODOSet[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='O';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='D';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='O';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='S';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='a';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='t';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=' ';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='%';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=' ';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}break;
					case 212:
					{
						flash.nFlag_SelectItem_ODO[ch]=ON;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='O';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='D';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='O';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='g';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='/';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='L';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}break;
					case 215:
					{
						flash.nFlag_SelectItem_BGAPC[ch]=ON;
						flash.nFlag_SelectItem_BGAPC_Uint[ch]=2;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='B';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='G';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='A';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='-';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='P';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='C';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='C';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='l';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='l';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='/';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='L';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}break;
					case 216:
					{
						flash.nFlag_SelectItem_BGAPCRFU[ch]=ON;
						flash.nFlag_SelectItem_BGAPCRFU_Uint[ch]=3;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='B';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='G';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='A';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='-';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='P';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='C';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='R';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='F';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='U';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}break;
					case 217:
					{
					flash.nFlag_SelectItem_BGAPE[ch]=ON;
						flash.nFlag_SelectItem_BGAPE_Uint[ch]=2;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='B';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='G';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='A';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='-';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='P';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='E';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='C';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='e';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='l';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='l';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='s';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='/';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='m';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='L';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}break;
					case 218:
					{
						flash.nFlag_SelectItem_BGAPE[ch]=ON;
						flash.nFlag_SelectItem_BGAPE_Uint[ch]=3;
						idCnt=0;
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='B';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='G';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='A';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='-';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='P';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='E';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='(';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='R';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='F';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]='U';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt++]=')';
						flash.Uart[ch]->SetupName[flash.Uart[ch]->ItemCnt][idCnt]=NULL;
						flash.Uart[ch]->ItemCnt++;
					}break;
				}
				if(flash.Uart[ch]->ItemCnt>=8)//8개보다 더들오면 나머지는 버린다.
				{
					for(i=0;i<USART_SENSOR_MAX;i++)
					{
						for(idCnt=0;idCnt<20;idCnt++)
						{
							flash.Uart[ch]->SetupName[i][idCnt]=flash.Uart[ch]->SetupName[i][idCnt];
						}
					}
                    //Dprintf("%s ",flash.Uart[ch]->SetupName[i]);
					return 0;
				}
			}
		}
		else
		{
			cItmeCodeBuffer[cItemCodeBufferAddress++]=gv.aItemReceiveBuffer[ch][nCnt];
		}			
	}
	for(i=0;i<USART_SENSOR_MAX;i++)
	{
		for(idCnt=0;idCnt<20;idCnt++)
		{
			flash.Uart[ch]->SetupName[i][idCnt]=flash.Uart[ch]->SetupName[i][idCnt];
		}
	}
    
    #endif
	return 1;
}
