
/******************************************************************************
*    	File name	: KeyPad.c 
*		Description : TEECO Project User Program File
*		Company		: idnics co.,
*		Version		: 1.00
*		Contact		: Naver TEECO Cafe
******************************************************************************/

/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
#include "DataLog.h"
#ifdef EXO_ENABLE
/******************************************************************************
							Constant & Macros
******************************************************************************/
/******************************************************************************
							Private & Local Variables
******************************************************************************/
/******************************************************************************
							Function Prototype
******************************************************************************/
volatile U32  gSerialNoEdit[30];
U32			ExoTimerId=0;
extern volatile U32             EXO_IO_PROCESS;
extern volatile int EXO_Find_CommandChangeFlag[3];
int IOStopTimer(int id);
int	SmartLogDataAddressProcess( void )
{
	if(flash.EXO.item	  ==EXO_PH) 			flash.EXO.itemAdd+=5;
	else if(flash.EXO.item==EXO_TURBIDITY_FNU) 	flash.EXO.itemAdd+=6;
	else if(flash.EXO.item==EXO_COND) 			flash.EXO.itemAdd+=8;
	else if(flash.EXO.item==EXO_CHLOROPHYLL) 	flash.EXO.itemAdd+=9;
	else if(flash.EXO.item==EXO_FDOM) 			flash.EXO.itemAdd+=6;
	else if(flash.EXO.item==EXO_ODO) 			flash.EXO.itemAdd+=6;
    else if(flash.EXO.item==EXO_AMMONINUM) 		flash.EXO.itemAdd+=6;
	else if(flash.EXO.item==EXO_NOSELECT)		flash.EXO.itemAdd=9 ;//센서는 있으나 데이터를 출력하지 않는경우
	return (0);
}
int SmartLogExo_ItemDisplayProcess(int UartSelect) {
	unsigned int		portNo=0;
	int					port=0;
	float				nanPv=0.0;

	flash.EXO.item=EXO_NOSELECT;
	for(port=1;port<9;port++)//포트를 찾는다.
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//		PH , MV		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( flash.EXO.Sensor_PH[UartSelect][0] == port )
		{
			//Dprintf(" pH[%d]",port);
			SmartLogDataAddressProcess();//측정치가 올라오는 순서를 찾는다.
			if(flash.EXO.Sensor_PH_Status[UartSelect][0]==ON)
			{
				nanPv = flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+0];
				if(gv.wiperEnableFlag==OFF) {
					sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","pH");                    
					if(isnan(nanPv)){
						Dprintf("nan\n");
					} else {
						flash.Uart[UartSelect]->SensorPV[portNo] = nanPv;//원본
					}
				}
				portNo++;
			}
			#if defined(TYPE_APPROVAL_ENABLE)
			nanPv = flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+1];
			if(gv.wiperEnableFlag==OFF) {
				if(isnan(nanPv)) {
					Dprintf("nan\n");
				} else {
					gv.tms_pHmV = nanPv;
				}
			}
			#else
			if(flash.EXO.Sensor_PHMV_Status[UartSelect][0]==ON)
			{
				nanPv = flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+1];
				if(gv.wiperEnableFlag==OFF) {
					if(isnan(nanPv)) {
						Dprintf("nan\n");
					} else {
						sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","pHmV");
						flash.Uart[UartSelect]->SensorPV[portNo] = nanPv;
					}
				}
				portNo++;
			}
			#endif
			if(flash.EXO.Sensor_ORP_Status[UartSelect][0]==ON)
			{
				nanPv = flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+2];
				if(gv.wiperEnableFlag==OFF) {
					if(isnan(nanPv)) {
						Dprintf("nan\n");
					} else {
						sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","ORP mV");
						flash.Uart[UartSelect]->SensorPV[portNo] = nanPv;
					}
				}
				portNo++;
			}
			flash.EXO.item=EXO_PH;
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//		ODO SAT, ODO	MG/L	
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( flash.EXO.Sensor_ODO[UartSelect][0] == port )
		{
			//Dprintf(" ODO[%d]",port);
			SmartLogDataAddressProcess();//측정치가 올라오는 순서를 찾는다.
			if(flash.EXO.Sensor_ODO_SAT_Status[UartSelect][0]==ON)
			{
				nanPv = flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd];
				if(gv.wiperEnableFlag==OFF) {
					if(isnan(nanPv)) {
						Dprintf("nan\n");
					} else {
						sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","ODO Sat");
						if(nanPv<0) {
							nanPv=0;
						}
						flash.Uart[UartSelect]->SensorPV[portNo] = nanPv;
					}
				}
				portNo++;
			}
			if(flash.EXO.Sensor_ODO_MGL_Status[UartSelect][0]==ON)
			{
				if(gv.wiperEnableFlag==OFF) {
					nanPv = flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+1];
					if(isnan(nanPv)) {
						Dprintf("nan\n");
					} else {
						sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","ODO mg/L");
						if(nanPv<0) {
							nanPv=0;
						}
						flash.Uart[UartSelect]->SensorPV[portNo] = nanPv;//원본
					}
				}
				portNo++;
			}
			flash.EXO.item=EXO_ODO;
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//		SPCOND,   SAL, TEMP	
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( flash.EXO.Sensor_COND[UartSelect][0] == port )
		{
			SmartLogDataAddressProcess();//측정치가 올라오는 순서를 찾는다.
			nanPv = flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd];
			if(flash.EXO.Sensor_COND_TEMP_Status[UartSelect][0]==ON)
			{
				if(gv.wiperEnableFlag==OFF) {					
					nanPv = flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd];
					if(isnan(nanPv)) {
						Dprintf("nan\n");
					} else {
						sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","TEMP 'C");
                        #if defined(DAECHEONGHO_SUGILGWANLISO)
                        sprintf(flash.Uart[UartSelect]->SetupName[portNo],"[CH%d] %s",(UartSelect+1),"TEMP 'C");
                        #endif
						flash.Uart[UartSelect]->SensorPV[portNo] = nanPv;
                        flash.EXO.temp[UartSelect]=nanPv;
					}
				}
				portNo++;
			}
			if(flash.EXO.Sensor_COND_SP_Status[UartSelect][0]==ON)
			{				
                if(flash.EXO.exoCalMode==0) {					
                    if(flash.EXO.SpcondmgmsEnable[UartSelect]==0) { //단위전환데이터출력
                        if(gv.wiperEnableFlag==OFF) {
                            nanPv = flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+2];
                            if(isnan(nanPv)) {
                                Dprintf("nan\n");
                            } else {
                                sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","SPCOND uS/cm");
                                flash.Uart[UartSelect]->SensorPV[portNo] = nanPv ;			//uS/cm 
                            }
                        }
                    } else {//교정중에는 mg/cm로 고정된다.
                        if(gv.wiperEnableFlag==OFF) {
                            nanPv = flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+2] / 1000.0f;
                            if(isnan(nanPv)) {
                                Dprintf("nan\n");
                            } else {
                                sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","SPCOND mS/cm");
                                flash.Uart[UartSelect]->SensorPV[portNo] = nanPv;	//mS/cm						
                            }
                        }
                    }
                } else {
                    if(gv.wiperEnableFlag==OFF) {
                        nanPv=flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+2] / 1000.0f;
                        if(isnan(nanPv)) {
                            Dprintf("nan\n");
                        } else {
                            sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","SPCOND mS/cm");
                            flash.Uart[UartSelect]->SensorPV[portNo] = nanPv;	//mS/cm
                        }
                    }
                }
				//Dprintf("수신전도도[%d] = %.3f\n",UartSelect,flash.Uart[UartSelect]->SensorPV[portNo]);							
                //gv.tms_SPCondPV=flash.Uart[UartSelect]->SensorPV[portNo];//교정용 데이터 mS/cm
				portNo++;
			}
			if(flash.EXO.Sensor_COND_SAL_Status[UartSelect][0]==ON)
			{
				if(gv.wiperEnableFlag==OFF) {
					nanPv=flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+3];
					if(isnan(nanPv)) {
                        Dprintf("nan\n");
					} else {						
						sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","SAL ppt");
						flash.Uart[UartSelect]->SensorPV[portNo] = nanPv;
						flash.EXO.CalSaltPV[UartSelect]= nanPv;
					}
				}
				portNo++;
			} else { //염분을 선택하지 않아도 교정용 염분값은 측정을 한다.
                nanPv=flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+3];
                if(gv.wiperEnableFlag==OFF) {
                    if(isnan(nanPv)) {
                        Dprintf("nan\n");
                    } else {						
                        flash.EXO.CalSaltPV[UartSelect]= nanPv;
                    }
                }
            }
			flash.EXO.CalSaltPV[UartSelect]= flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+3];//보정용 염소값 무조건 적용한다.
			flash.EXO.item=EXO_COND;
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//		TUB FNU, TSS
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( flash.EXO.Sensor_TUB[UartSelect][0] == port )
		{
			//Dprintf(" TUB[%d]",port);
			SmartLogDataAddressProcess();//측정치가 올라오는 순서를 찾는다.
			if(flash.EXO.Sensor_TUB_FNU_Status[UartSelect][0]==ON)
			{
                if(gv.wiperEnableFlag==OFF) {
                    sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","TURBIDITY NTU");
                    nanPv=flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+0];             
                    if(isnan(nanPv)) {
                        Dprintf("nan\n");
                    } else {
                        //Dprintf("TUBIDITY FNU NO = %d\n", flash.EXO.itemAdd+0);
                        flash.Uart[UartSelect]->SensorPV[portNo] = nanPv;
                    }
                }
				portNo++;				
			}
            if(flash.EXO.Sensor_TSS_MGL_Status[UartSelect][0]==ON) {
                if(gv.wiperEnableFlag==OFF) {
                    sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","TSS mg/L");
                    nanPv=flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+1];             
                    if(isnan(nanPv)) {
                        Dprintf("nan\n");
                    } else {
                        //Dprintf("TSS NO = %d\n", flash.EXO.itemAdd+1);
                        flash.Uart[UartSelect]->SensorPV[portNo] = nanPv;
                    }
                }
				portNo++;				
            }                
			flash.EXO.item=EXO_TURBIDITY_FNU;
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//		CHLOROPHYLL UGL, RFU,  BGAPC UGL , BGAPC RFU, BGAPE UGL, BGAPE RFU 
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( flash.EXO.Sensor_CHLIB[UartSelect][0] == port )
		{
			//Dprintf(" CHLIB/BGAPC[%d]",port);
			SmartLogDataAddressProcess();//측정치가 올라오는 순서를 찾는다.
			
			if(flash.EXO.Sensor_CHLIB_UGL_Status[UartSelect][0]==ON)
			{
				if(gv.wiperEnableFlag==OFF) {
					nanPv=flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+1];
					if(isnan(nanPv)){
                        Dprintf("nan\n");
					} else {
						sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","CHLOROPHYLL ug/L");
						flash.Uart[UartSelect]->SensorPV[portNo] = nanPv;
					}
				}
				portNo++;
			}
			if(flash.EXO.Sensor_CHLIB_RFU_Status[UartSelect][0]==ON)
			{
				if(gv.wiperEnableFlag==OFF) {
					nanPv=flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+0];
					if(isnan(nanPv)) {
                        Dprintf("nan\n");
					} else {
						sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","CHLOROPHYLL RFU");
						flash.Uart[UartSelect]->SensorPV[portNo] = nanPv;
					}
				}
				portNo++;
			}
			if(flash.EXO.Sensor_BGA_PCRFU_Status[UartSelect][0]==ON)
			{
				if(gv.wiperEnableFlag==OFF) {
					nanPv=flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+2];
					if(isnan(nanPv)) {
                        Dprintf("nan\n");
					} else {
						sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","BGAPC RFU");
						flash.Uart[UartSelect]->SensorPV[portNo] = nanPv;
					}
				}
				portNo++;
			}
			if(flash.EXO.Sensor_BGA_PCUGL_Status[UartSelect][0]==ON)
			{
				if(gv.wiperEnableFlag==OFF) {
					nanPv=flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+3];
					if(isnan(nanPv)){
                        Dprintf("nan\n");
					} else {
						sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","BGAPC ug/L");
						flash.Uart[UartSelect]->SensorPV[portNo] = nanPv;
					}
				}
				portNo++;
			}
			if(flash.EXO.Sensor_BGA_PERFU_Status[UartSelect][0]==ON)
			{
				if(gv.wiperEnableFlag==OFF) {					
					nanPv=flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+2];
					if(isnan(nanPv)){
                        Dprintf("nan\n");
					} else {
						sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","BGAPE RFU");
						flash.Uart[UartSelect]->SensorPV[portNo] = nanPv;
					}
				}
				portNo++;
			}
			if(flash.EXO.Sensor_BGA_PEUGL_Status[UartSelect][0]==ON)
			{
				if(gv.wiperEnableFlag==OFF) {
					nanPv=flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+3];
					if(isnan(nanPv)) {
                        Dprintf("nan\n");
					} else {
						sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","BGAPE ug/L");
						flash.Uart[UartSelect]->SensorPV[portNo] = nanPv;
					}
				}
				portNo++;
			}					
			flash.EXO.item=EXO_CHLOROPHYLL;
		}		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//		DEPTH
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( flash.EXO.Sensor_DEPTH[UartSelect][0] == port )
		{
			//Dprintf(" DEPTH = %d \n", port );
			SmartLogDataAddressProcess();
			//Dprintf("USART[%d] = %d\n",UartSelect,flash.EXO.itemAdd);
			if(flash.EXO.Sensor_DEPTH_Status[UartSelect][0]==ON)
			{
				nanPv=flash.EXO.EXO_Data[UartSelect][flash.EXO.DataListCount[UartSelect]-6];
				if(isnan(nanPv)) {
                    Dprintf("nan\n");
				} else {
					sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","DEPTH m");
					flash.Uart[UartSelect]->SensorPV[portNo] = nanPv;
				}
				portNo++;
			}
		}		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//		Ammouninum
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( flash.EXO.Sensor_Ammoninum[UartSelect][0] == port )
		{
			SmartLogDataAddressProcess();//측정치가 올라오는 순서를 찾는다.
			if(flash.EXO.Sensor_Ammoninum_Status[UartSelect][0]==ON)
			{
				if(gv.wiperEnableFlag==OFF) {
					nanPv=flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+1];
					if(isnan(nanPv)) {
                        Dprintf("nan\n");
					} else {
						sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","NH4+_N mg/L");
						flash.Uart[UartSelect]->SensorPV[portNo] = nanPv;
					}
				}
				portNo++;
			}
			flash.EXO.item=EXO_AMMONINUM;
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//		FDOM RFU, FDOM QSU
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( flash.EXO.Sensor_FDOM[UartSelect][0] == port )
		{
			//Dprintf(" FDOM[%d]",port);
			SmartLogDataAddressProcess();//측정치가 올라오는 순서를 찾는다.
			if(flash.EXO.Sensor_FDOM_RFU_Status[UartSelect][0]==ON)
			{
				if(gv.wiperEnableFlag==OFF) {
					nanPv=flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+2];
					if(isnan(nanPv)) {
                        Dprintf("nan\n");
					} else {
						sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","FDOM RFU");
						flash.Uart[UartSelect]->SensorPV[portNo] = nanPv;
					}
				}
				portNo++;
			}
			if(flash.EXO.Sensor_FDOM_QSU_Status[UartSelect][0]==ON)
			{
				if(gv.wiperEnableFlag==OFF) {
					nanPv=flash.EXO.EXO_Data[UartSelect][flash.EXO.itemAdd+3];
					if(isnan(nanPv)){
                        Dprintf("nan\n");
					} else {
						sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","FDOM QSU");					
						flash.Uart[UartSelect]->SensorPV[portNo] = nanPv;
					}
				}
				portNo++;
			}
			flash.EXO.item=EXO_FDOM;
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//		BAT
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if(flash.EXO.Sensor_Battery_Status[UartSelect][0]==ON)
        {
            sprintf(flash.Uart[UartSelect]->SetupName[portNo],"%s","Battery");
            flash.Uart[UartSelect]->SensorPV[portNo] = flash.EXO.EXO_Data[UartSelect][5];
        }
    }
	return (0);
}
int	 SmartLogEXOMessageTimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Graphics 	*Gr   = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.

	if(Gr==NULL) {													// 그래픽 할당을 못 받은 경우  
		Dprintf("SmartLogUSBMessageTimerEvent:: Graphics Error\n");
		return(0);
	}		
	PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);
	SetWidgetText(gSerialNoEdit[18] ,flash.Uart[flash.EXO.UartPort]->SetupName[0]);
	SetWidgetText(gSerialNoEdit[19] ,flash.Uart[flash.EXO.UartPort]->SetupName[1]);
	SetWidgetText(gSerialNoEdit[20] ,flash.Uart[flash.EXO.UartPort]->SetupName[2]);
	SetWidgetText(gSerialNoEdit[21] ,flash.Uart[flash.EXO.UartPort]->SetupName[3]);
	SetWidgetText(gSerialNoEdit[22] ,flash.Uart[flash.EXO.UartPort]->SetupName[4]);
	SetWidgetText(gSerialNoEdit[23] ,flash.Uart[flash.EXO.UartPort]->SetupName[5]);
	SetWidgetText(gSerialNoEdit[24] ,flash.Uart[flash.EXO.UartPort]->SetupName[6]);
	SetWidgetText(gSerialNoEdit[25] ,flash.Uart[flash.EXO.UartPort]->SetupName[7]);
	if(gv.exoPortFindCnt[USART_1]<=3) {
		SetWidgetText(gSerialNoEdit[27] ,"PORT.1 체크 중 입니다.");
	}
	else if(gv.exoPortFindCnt[USART_1]<=5) {
		SetWidgetText(gSerialNoEdit[27] ,"PORT.2 체크 중 입니다.");
	}
	else if(gv.exoPortFindCnt[USART_1]<=7) {
		SetWidgetText(gSerialNoEdit[27] ,"PORT.3 체크 중 입니다.");
	}
	else if(gv.exoPortFindCnt[USART_1]<=9) {
		SetWidgetText(gSerialNoEdit[27] ,"PORT.4 체크 중 입니다.");
	}
	else if(gv.exoPortFindCnt[USART_1]<=11) {
		SetWidgetText(gSerialNoEdit[27] ,"PORT.5 체크 중 입니다.");
	}
	else if(gv.exoPortFindCnt[USART_1]<=13) {
		SetWidgetText(gSerialNoEdit[27] ,"PORT.6 체크 중 입니다.");
	}
	else if(gv.exoPortFindCnt[USART_1]<=15) {
		SetWidgetText(gSerialNoEdit[27] ,"PORT.7 체크 중 입니다.");
	}
	else if(gv.exoPortFindCnt[USART_1]<=17) {
		SetWidgetText(gSerialNoEdit[27] ,"PORT.8 체크 중 입니다.");
	}
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return 0;
}

int	 SmartLogEXOItemPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Windows 	*win = GetWindow(parent, handle, "SmartLogEXOItemPaintEvent");
	Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.

	if(win==NULL) 
	{
		Dprintf("SmartLogEXOItemPaintEvent:: Error\n");
		return(0);
	}
	if(Gr==NULL)
	{
		Dprintf("SmartLogEXOItemPaintEvent::GetGraphics Error\n");
		return(0);
	}	
	PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);
	PaintDrawControl(ON);		
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}


int	 SmartLogEXOItemClickEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Windows 	*win = GetWindow(parent, handle, "SmartLogEXOItemClickEvent");
	Windows		*parent_win = FindContainerWindow(parent,  "SmartLogEXOItemClickEvent");
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	
	volatile unsigned int		color=0;
	volatile U32					id=0;
	volatile unsigned int		i=0;
	volatile unsigned int		itemCnt=0;
	//int a, c;
	//unsigned char			data[20];
	//Dprintf("KeyPad Button parent=%d Handle=%d Click = %s Mode=%d parentname=%s\n",win->Parent, win->Handle, win->NormalName, parent_win->Mode, parent_win->Text);
	if(win==NULL) 
	{
		Dprintf("SmartLogEXOItemPaintEvent:: Error\n");
		return(0);
	}
	if(parent_win == NULL)
	{
		Dprintf("SmartLogKeypadClickEvent:: Windows Error\n");
	}
	//SetWidgetText(parent_win->CallWidget_ID, win->Text);//종료시에 콜한 버튼에 데이터를 출력한다.
	
	//Dprintf("win->ID = %d\n",win->ID);
	/*
	if(flash.Uart[flash.EXO.UartPort]->ItemCnt==8)//최대항목수를 넘지 않게한다.
	{
		if(win->ID != 20) return(1);//WINDOW CLOSE가 아니면 리턴한다.
	}
	*/	
	if(win->ID != 19) {
		Dprintf("EXO ITEM PORT CLEAR\n");
		strcpy(flash.Uart[flash.EXO.UartPort]->SetupName[0],"NONE");
		strcpy(flash.Uart[flash.EXO.UartPort]->SetupName[1],"NONE");
		strcpy(flash.Uart[flash.EXO.UartPort]->SetupName[2],"NONE");
		strcpy(flash.Uart[flash.EXO.UartPort]->SetupName[3],"NONE");
		strcpy(flash.Uart[flash.EXO.UartPort]->SetupName[4],"NONE");
		strcpy(flash.Uart[flash.EXO.UartPort]->SetupName[5],"NONE");
		strcpy(flash.Uart[flash.EXO.UartPort]->SetupName[6],"NONE");
		strcpy(flash.Uart[flash.EXO.UartPort]->SetupName[7],"NONE");
	}
	
    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy
	switch(win->ID)
	{
		case 0: 	if(flash.EXO.Sensor_PH_Status[flash.EXO.UartPort][0]==ON)
					{
						flash.EXO.Sensor_PH_Status[flash.EXO.UartPort][0]=OFF;
						 SetWidgetText(gSerialNoEdit[0],"OFF");
						SetWidgetColor(gSerialNoEdit[0],clWhite,clBlack);

					}
					else
					{
						flash.EXO.Sensor_PH_Status[flash.EXO.UartPort][0]=ON;
						 SetWidgetText(gSerialNoEdit[0],"ON");
						SetWidgetColor(gSerialNoEdit[0],clWhite,clRed);
					}
		break;
		case 1: 	if(flash.EXO.Sensor_PHMV_Status[flash.EXO.UartPort][0]==ON)
					{
						flash.EXO.Sensor_PHMV_Status[flash.EXO.UartPort][0]=OFF;
						 SetWidgetText(gSerialNoEdit[1],"OFF");
						SetWidgetColor(gSerialNoEdit[1],clWhite,clBlack);
					}
					else
					{
						flash.EXO.Sensor_PHMV_Status[flash.EXO.UartPort][0]=ON;
						 SetWidgetText(gSerialNoEdit[1],"ON");
						SetWidgetColor(gSerialNoEdit[1],clWhite,clRed);
					}
		break;
		case 2: 	if(flash.EXO.Sensor_ORP_Status[flash.EXO.UartPort][0]==ON)
					{
						flash.EXO.Sensor_ORP_Status[flash.EXO.UartPort][0]=OFF;
						 SetWidgetText(gSerialNoEdit[2],"OFF");
						SetWidgetColor(gSerialNoEdit[2],clWhite,clBlack);
					}
					else
					{
						flash.EXO.Sensor_ORP_Status[flash.EXO.UartPort][0]=ON;
						 SetWidgetText(gSerialNoEdit[2],"ON");
						SetWidgetColor(gSerialNoEdit[2],clWhite,clRed);
					}
		break;
		case 3: 	if(flash.EXO.Sensor_ODO_SAT_Status[flash.EXO.UartPort][0]==ON)
					{
						flash.EXO.Sensor_ODO_SAT_Status[flash.EXO.UartPort][0]=OFF;
						 SetWidgetText(gSerialNoEdit[3],"OFF");
						SetWidgetColor(gSerialNoEdit[3],clWhite,clBlack);
					}
					else
					{
						flash.EXO.Sensor_ODO_SAT_Status[flash.EXO.UartPort][0]=ON;
						 SetWidgetText(gSerialNoEdit[3],"ON");
						SetWidgetColor(gSerialNoEdit[3],clWhite,clRed);
					}
		break;
		case 4: 	if(flash.EXO.Sensor_ODO_MGL_Status[flash.EXO.UartPort][0]==ON)
					{
						flash.EXO.Sensor_ODO_MGL_Status[flash.EXO.UartPort][0]=OFF;
						 SetWidgetText(gSerialNoEdit[4],"OFF");
						SetWidgetColor(gSerialNoEdit[4],clWhite,clBlack);
					}
					else
					{
						flash.EXO.Sensor_ODO_MGL_Status[flash.EXO.UartPort][0]=ON;
						 SetWidgetText(gSerialNoEdit[4],"ON");
						SetWidgetColor(gSerialNoEdit[4],clWhite,clRed);
					}
		break;
		case 5: 	if(flash.EXO.Sensor_COND_SP_Status[flash.EXO.UartPort][0]==ON)
					{
						flash.EXO.Sensor_COND_SP_Status[flash.EXO.UartPort][0]=OFF;
						 SetWidgetText(gSerialNoEdit[5],"OFF");
						SetWidgetColor(gSerialNoEdit[5],clWhite,clBlack);
					}
					else
					{
						flash.EXO.Sensor_COND_SP_Status[flash.EXO.UartPort][0]=ON;
						 SetWidgetText(gSerialNoEdit[5],"ON");
						SetWidgetColor(gSerialNoEdit[5],clWhite,clRed);
					}
		break;
		case 6: 	if(flash.EXO.Sensor_COND_SAL_Status[flash.EXO.UartPort][0]==ON)
					{
						flash.EXO.Sensor_COND_SAL_Status[flash.EXO.UartPort][0]=OFF;
						 SetWidgetText(gSerialNoEdit[6],"OFF");
						SetWidgetColor(gSerialNoEdit[6],clWhite,clBlack);
					}
					else
					{
						flash.EXO.Sensor_COND_SAL_Status[flash.EXO.UartPort][0]=ON;
						 SetWidgetText(gSerialNoEdit[6],"ON");
						SetWidgetColor(gSerialNoEdit[6],clWhite,clRed);
					}
		break;
		case 7: 	if(flash.EXO.Sensor_COND_TEMP_Status[flash.EXO.UartPort][0]==ON)
					{
						flash.EXO.Sensor_COND_TEMP_Status[flash.EXO.UartPort][0]=OFF;
						 SetWidgetText(gSerialNoEdit[7],"OFF");
						SetWidgetColor(gSerialNoEdit[7],clWhite,clBlack);
					}
					else
					{
						flash.EXO.Sensor_COND_TEMP_Status[flash.EXO.UartPort][0]=ON;
						 SetWidgetText(gSerialNoEdit[7],"ON");
						SetWidgetColor(gSerialNoEdit[7],clWhite,clRed);
					}
		break;
                            
		case 8: 	if(flash.EXO.Sensor_TUB_FNU_Status[flash.EXO.UartPort][0]==ON)
					{
						flash.EXO.Sensor_TUB_FNU_Status[flash.EXO.UartPort][0]=OFF;
						 SetWidgetText(gSerialNoEdit[8],"OFF");
						SetWidgetColor(gSerialNoEdit[8],clWhite,clBlack);
					}
					else
					{
						flash.EXO.Sensor_TUB_FNU_Status[flash.EXO.UartPort][0]=ON;
						 SetWidgetText(gSerialNoEdit[8],"ON");
						SetWidgetColor(gSerialNoEdit[8],clWhite,clRed);
					}
		break;
		case 9: 	if(flash.EXO.Sensor_Battery_Status[flash.EXO.UartPort][0]==ON)
					{
						flash.EXO.Sensor_Battery_Status[flash.EXO.UartPort][0]=OFF;
						 SetWidgetText(gSerialNoEdit[9],"OFF");
						SetWidgetColor(gSerialNoEdit[9],clWhite,clBlack);
					}
					else
					{
						flash.EXO.Sensor_Battery_Status[flash.EXO.UartPort][0]=ON;
						 SetWidgetText(gSerialNoEdit[9],"ON");
						SetWidgetColor(gSerialNoEdit[9],clWhite,clRed);
					}
		break;
		case 10: 	if(flash.EXO.Sensor_CHLIB_UGL_Status[flash.EXO.UartPort][0]==ON)
					{
						flash.EXO.Sensor_CHLIB_UGL_Status[flash.EXO.UartPort][0]=OFF;
						 SetWidgetText(gSerialNoEdit[10],"OFF");
						SetWidgetColor(gSerialNoEdit[10],clWhite,clBlack);
					}
					else
					{
						flash.EXO.Sensor_CHLIB_UGL_Status[flash.EXO.UartPort][0]=ON;
						 SetWidgetText(gSerialNoEdit[10],"ON");
						SetWidgetColor(gSerialNoEdit[10],clWhite,clRed);
					}
		break;
		case 11: 	if(flash.EXO.Sensor_CHLIB_RFU_Status[flash.EXO.UartPort][0]==ON)
					{
						flash.EXO.Sensor_CHLIB_RFU_Status[flash.EXO.UartPort][0]=OFF;
						 SetWidgetText(gSerialNoEdit[11],"OFF");
						SetWidgetColor(gSerialNoEdit[11],clWhite,clBlack);
						
					}
					else
					{
						flash.EXO.Sensor_CHLIB_RFU_Status[flash.EXO.UartPort][0]=ON;
						 SetWidgetText(gSerialNoEdit[11],"ON");
						SetWidgetColor(gSerialNoEdit[11],clWhite,clRed);
					}
		break;
		case 12: 	if(flash.EXO.Sensor_BGA_PCRFU_Status[flash.EXO.UartPort][0]==ON)
					{
						flash.EXO.Sensor_BGA_PCRFU_Status[flash.EXO.UartPort][0]=OFF;
						 SetWidgetText(gSerialNoEdit[12],"OFF");
						SetWidgetColor(gSerialNoEdit[12],clWhite,clBlack);
					}
					else
					{
						flash.EXO.Sensor_BGA_PCRFU_Status[flash.EXO.UartPort][0]=ON;
						 SetWidgetText(gSerialNoEdit[12],"ON");
						SetWidgetColor(gSerialNoEdit[12],clWhite,clRed);
					}
		break;
		case 13: 	if(flash.EXO.Sensor_BGA_PCUGL_Status[flash.EXO.UartPort][0]==ON)
					{
						flash.EXO.Sensor_BGA_PCUGL_Status[flash.EXO.UartPort][0]=OFF;
						 SetWidgetText(gSerialNoEdit[13],"OFF");
						SetWidgetColor(gSerialNoEdit[13],clWhite,clBlack);
					}
					else
					{
						flash.EXO.Sensor_BGA_PCUGL_Status[flash.EXO.UartPort][0]=ON;
						 SetWidgetText(gSerialNoEdit[13],"ON");
						SetWidgetColor(gSerialNoEdit[13],clWhite,clRed);
					}
		break;
		case 14: 	if(flash.EXO.Sensor_BGA_PERFU_Status[flash.EXO.UartPort][0]==ON)
					{
						flash.EXO.Sensor_BGA_PERFU_Status[flash.EXO.UartPort][0]=OFF;
						 SetWidgetText(gSerialNoEdit[14],"OFF");
						SetWidgetColor(gSerialNoEdit[14],clWhite,clBlack);
					}
					else
					{
						flash.EXO.Sensor_BGA_PERFU_Status[flash.EXO.UartPort][0]=ON;
						 SetWidgetText(gSerialNoEdit[14],"ON");
						SetWidgetColor(gSerialNoEdit[14],clWhite,clRed);
					}
		break;
		case 15: 	if(flash.EXO.Sensor_BGA_PEUGL_Status[flash.EXO.UartPort][0]==ON)
					{
						flash.EXO.Sensor_BGA_PEUGL_Status[flash.EXO.UartPort][0]=OFF;
						 SetWidgetText(gSerialNoEdit[15],"OFF");
						SetWidgetColor(gSerialNoEdit[15],clWhite,clBlack);
					}
					else
					{
						flash.EXO.Sensor_BGA_PEUGL_Status[flash.EXO.UartPort][0]=ON;
						 SetWidgetText(gSerialNoEdit[15],"ON");
						SetWidgetColor(gSerialNoEdit[15],clWhite,clRed);
					}
		break;
		case 16: 	if(flash.EXO.Sensor_DEPTH_Status[flash.EXO.UartPort][0]==ON)
					{
						flash.EXO.Sensor_DEPTH_Status[flash.EXO.UartPort][0]=OFF;
						 SetWidgetText(gSerialNoEdit[16],"OFF");
						SetWidgetColor(gSerialNoEdit[16],clWhite,clBlack);
					}
					else
					{
						flash.EXO.Sensor_DEPTH_Status[flash.EXO.UartPort][0]=ON;
						 SetWidgetText(gSerialNoEdit[16],"ON");
						SetWidgetColor(gSerialNoEdit[16],clWhite,clRed);
					}
		break;
		case 17: 	
        {
                    #ifdef _Ammoninum
                    if(flash.EXO.Sensor_Ammoninum_Status[flash.EXO.UartPort][0]==ON)
					{
 						flash.EXO.Sensor_Ammoninum_Status[flash.EXO.UartPort][0]=OFF;
						
                        SetWidgetText(gSerialNoEdit[17],"OFF");
						SetWidgetColor(gSerialNoEdit[17],clWhite,clBlack);
					}
					else
					{
						flash.EXO.Sensor_Ammoninum_Status[flash.EXO.UartPort][0]=ON;

                        SetWidgetText(gSerialNoEdit[17],"ON");
						SetWidgetColor(gSerialNoEdit[17],clWhite,clRed);
					}
                    #endif
                    #ifdef _FDOM_RFU
                    if(flash.EXO.Sensor_FDOM_RFU_Status[flash.EXO.UartPort][0]==ON)
					{
 						flash.EXO.Sensor_FDOM_RFU_Status[flash.EXO.UartPort][0]=OFF;
						
                        SetWidgetText(gSerialNoEdit[17],"OFF");
						SetWidgetColor(gSerialNoEdit[17],clWhite,clBlack);
					}
					else
					{
						flash.EXO.Sensor_FDOM_RFU_Status[flash.EXO.UartPort][0]=ON;

                        SetWidgetText(gSerialNoEdit[17],"ON");
						SetWidgetColor(gSerialNoEdit[17],clWhite,clRed);
					}
                    #endif
                    #ifdef _FDOM_QSU
                    if(flash.EXO.Sensor_FDOM_QSU_Status[flash.EXO.UartPort][0]==ON)
					{
 						flash.EXO.Sensor_FDOM_QSU_Status[flash.EXO.UartPort][0]=OFF;
						
                        SetWidgetText(gSerialNoEdit[17],"OFF");
						SetWidgetColor(gSerialNoEdit[17],clWhite,clBlack);
					}
					else
					{
						flash.EXO.Sensor_FDOM_QSU_Status[flash.EXO.UartPort][0]=ON;

                        SetWidgetText(gSerialNoEdit[17],"ON");
						SetWidgetColor(gSerialNoEdit[17],clWhite,clRed);
					}
                    #endif
        }
		break;
		case 19:		
			{
				/*
				flash.EXO.Sensor_PH[flash.EXO.UartPort][0]		=0;
				flash.EXO.Sensor_ODO[flash.EXO.UartPort][0]	=0;
				flash.EXO.Sensor_COND[flash.EXO.UartPort][0]	=0;
				flash.EXO.Sensor_TUB[flash.EXO.UartPort][0]		=0;
				flash.EXO.Sensor_SOND[flash.EXO.UartPort][0]	=0;
				flash.EXO.Sensor_CHLIB[flash.EXO.UartPort][0]	=0;
				flash.EXO.Sensor_DEPTH[flash.EXO.UartPort][0]	=0;
				flash.EXO.Sensor_WIPER[flash.EXO.UartPort][0]	=0;
				flash.EXO.Sensor_FDOM[flash.EXO.UartPort][0]	=0;
				*/
				/*
				for(i=0;i<12;i++)
				{
					flash.Uart[flash.EXO.UartPort]->Name[i][0]='N';
					flash.Uart[flash.EXO.UartPort]->Name[i][1]='O';
					flash.Uart[flash.EXO.UartPort]->Name[i][2]='N';
					flash.Uart[flash.EXO.UartPort]->Name[i][3]='E';
					flash.Uart[flash.EXO.UartPort]->Name[i][4]=0;
			
				}
				*/				
				flash.Uart[flash.EXO.UartPort]->ItemCnt=0;
				/*
				for(i=0;i<12;i++)
				{
					flash.Uart[flash.EXO.UartPort]->Name[i][0]='N';
					flash.Uart[flash.EXO.UartPort]->Name[i][1]='O';
					flash.Uart[flash.EXO.UartPort]->Name[i][2]='N';
					flash.Uart[flash.EXO.UartPort]->Name[i][3]='E';
					flash.Uart[flash.EXO.UartPort]->Name[i][4]=0;

					flash.Uart[flash.EXO.UartPort]->SetupName[i][0]='N';
					flash.Uart[flash.EXO.UartPort]->SetupName[i][1]='O';
					flash.Uart[flash.EXO.UartPort]->SetupName[i][2]='N';
					flash.Uart[flash.EXO.UartPort]->SetupName[i][3]='E';
					flash.Uart[flash.EXO.UartPort]->SetupName[i][4]=0;
				}
				*/
				if(flash.EXO.Sensor_PH_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = PH\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}
				if(flash.EXO.Sensor_PHMV_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = PHMV\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}
				if(flash.EXO.Sensor_ORP_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = ORP\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}
				if(flash.EXO.Sensor_ODO_SAT_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = ODO SAT\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}
				if(flash.EXO.Sensor_ODO_MGL_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = ODO MGL\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}
				if(flash.EXO.Sensor_COND_SP_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = COND SP\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}
				if(flash.EXO.Sensor_COND_SAL_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = COND SAL\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}
				if(flash.EXO.Sensor_COND_TEMP_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = COND TEMP\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}
				if(flash.EXO.Sensor_TUB_FNU_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = TURBIDITY FNU\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}
                
//				if(flash.EXO.Sensor_TUB_NTU_Status[flash.EXO.UartPort][0]==ON)
//				{
//					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
//					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
//					Dprintf("EXOITEM[%d] = TUBIDITY NTU\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
//				}

				if(flash.EXO.Sensor_TSS_MGL_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = TURBIDITY TSS\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}
                
				if(flash.EXO.Sensor_CHLIB_UGL_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = CHLIB UGL\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}
				if(flash.EXO.Sensor_CHLIB_RFU_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = CHLIB RFU\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}
				if(flash.EXO.Sensor_BGA_PCRFU_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = BGA PCRFU\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}
				if(flash.EXO.Sensor_BGA_PCUGL_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = BGA PCUGL\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}
				if(flash.EXO.Sensor_BGA_PERFU_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = BGA PERFU\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}
				if(flash.EXO.Sensor_BGA_PEUGL_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = BGA PEUGL\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}
				if(flash.EXO.Sensor_DEPTH_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = DEPTH\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}
				if(flash.EXO.Sensor_Ammoninum_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = Ammoninum\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}
				if(flash.EXO.Sensor_FDOM_RFU_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = FDOM RFU\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}
				if(flash.EXO.Sensor_FDOM_QSU_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = FDOM QSU\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}
				if(flash.EXO.Sensor_CABLEPOWER_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = CABLEPOWER\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}
				if(flash.EXO.Sensor_Battery_Status[flash.EXO.UartPort][0]==ON)
				{
					flash.Uart[flash.EXO.UartPort]->ItemCnt++;
					flash.Uart[flash.EXO.UartPort]->SensorStatus=1;
					Dprintf("EXOITEM[%d] = CABLEPOWER\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				}

				Dprintf("ItemCnt = %d\n",flash.Uart[flash.EXO.UartPort]->ItemCnt);
				
				for(i=0;i<8;i++)//측정값을 초기화한다.
				{
					flash.Uart[flash.EXO.UartPort]->SensorPV[i]=0;
				}
				//선택된 항목이 8개 이상이면 8개만 출력한다.
				if(flash.Uart[flash.EXO.UartPort]->ItemCnt >8) flash.Uart[flash.EXO.UartPort]->ItemCnt=8;
				
				//****************************************************************************************************************
				EXO_Find_CommandChangeFlag[0]=OFF;
				EXO_Find_CommandChangeFlag[1]=OFF;
				EXO_Find_CommandChangeFlag[2]=OFF;
                //EXO 포트 변경    
				//저장시에 포트가 변경되는 현상이 발생하여 아래 for문을 추가한다.
                for(int i=0; i<8; i++) {
                    flash.EXO.SensorAddress[flash.EXO.UartPort][i][0]=flash.EXO.imsiSensorAddress[flash.EXO.UartPort][i][0];
                    flash.EXO.SensorAddress[flash.EXO.UartPort][i][1]=flash.EXO.imsiSensorAddress[flash.EXO.UartPort][i][1];
                    flash.EXO.SensorAddress[flash.EXO.UartPort][i][2]=flash.EXO.imsiSensorAddress[flash.EXO.UartPort][i][2];
                    flash.EXO.SensorAddress[flash.EXO.UartPort][i][3]=flash.EXO.imsiSensorAddress[flash.EXO.UartPort][i][3];
                    Dprintf("UART%d PORT[%d] ADDRESS %02x %02x %02x %02x\n"
                    ,i
                    ,flash.EXO.UartPort+1
                    ,flash.EXO.SensorAddress[flash.EXO.UartPort][i][0]
                    ,flash.EXO.SensorAddress[flash.EXO.UartPort][i][1]
                    ,flash.EXO.SensorAddress[flash.EXO.UartPort][i][2]
                    ,flash.EXO.SensorAddress[flash.EXO.UartPort][i][3]);
                }
				DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
				
				//IOStopTimer(EXO_IO_PROCESS);//센서검지를 위한 별도의 IO Process 를 동작시킨다.
				
                StopTimer(ExoTimerId);
				
                //****************************************************************************************************************			
				CloseWindow(NULL, parent);
			}
        break;
        //TSS추가 2023 05 25
        case 26: 	if(flash.EXO.Sensor_TSS_MGL_Status[flash.EXO.UartPort][0]==ON)
            {
                flash.EXO.Sensor_TSS_MGL_Status[flash.EXO.UartPort][0]=OFF;
                 SetWidgetText(gSerialNoEdit[win->ID],"OFF");
                SetWidgetColor(gSerialNoEdit[win->ID],clWhite,clBlack);
            }
            else
            {
                flash.EXO.Sensor_TSS_MGL_Status[flash.EXO.UartPort][0]=ON;
                SetWidgetText(gSerialNoEdit[win->ID],"ON");
                SetWidgetColor(gSerialNoEdit[win->ID],clWhite,clRed);
            }
		break;
	}
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}
int	 SmartLogEXOItemCreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	//char	buf[64];
	int		sx = 10;
	int		sy = 10;
    int     width = 200 +20;
    int     width2 = 210 +20;
    int     height = 40;
    int     height2= 43;
	int		color=clWhite;
	//int		textColor=clBlack;
	char data[20];
	U32	id=0;
	char fbuf[30];
	int			i=0,a=0,c=0;
	
	Windows 	*win = GetWindow(parent, handle,"SmartLogEXOItemCreateEvent");
	Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	if(win==NULL)
	{
		Dprintf("SmartLogEXOItemCreateEvent:: Windows Error\n");
		return(0);
	}
	if(Gr==NULL)
	{
		Dprintf("SmartLogEXOItemCreateEvent::GetGraphics Error\n");
		return(0);
	}	
	PaintDrawControl(OFF);
	Gr->FillRect(0, 0, 0, 0);//Dummy
	Gr->SetColor(flash.systemBackGroundColor);
	Gr->FillRect(0, 0, win->client_width+1, win->client_height);	
	
	gv.exoPortFindCnt[USART_1]=1;
	gv.exoPortFindCnt[USART_2]=1;
	gv.exoPortFindCnt[USART_3]=1;
	
	
	EventMessage("EXO 센서 FIND 기능실행");
    
	//GetWidgetText(win->CallWidget_ID, buf);//제어문에 문제가 있어 삭제한다.

    Dprintf("SmartLogEXOItemCreateEvent :: flash.EXO.UartPort = %d\n", flash.EXO.UartPort);    
	//****************************************************************************************************************
	EXO_Find_CommandChangeFlag[flash.EXO.UartPort]=ON;
	
    //IOStartTimer(EXO_IO_PROCESS);//센서검지를 위한 별도의 IO Process 를 동작시킨다.
	
	//flash.EXO_IO_Process_ChangeFlag[0]=OFF; //EXO 수신 루틴을 변경한다.	
	//flash.EXO_IO_Process_ChangeFlag[1]=OFF; //EXO 수신 루틴을 변경한다.	
	//flash.EXO_IO_Process_ChangeFlag[2]=OFF; //EXO 수신 루틴을 변경한다.	
	//IOStopTimer(EXO_IO_PROCESS);//센서검지를 위한 별도의 IO Process 를 동작시킨다.

	//****************************************************************************************************************
	for(i=0;i<USART_SENSOR_MAX;i++) strcpy(flash.Uart[flash.EXO.UartPort]->SetupName[i],"NONE");
	
	//센서 포트 넘버 초기화
	flash.EXO.Sensor_PH[flash.EXO.UartPort][0]		=0;
	flash.EXO.Sensor_ODO[flash.EXO.UartPort][0]		=0;
	flash.EXO.Sensor_COND[flash.EXO.UartPort][0]	=0;
	flash.EXO.Sensor_TUB[flash.EXO.UartPort][0]		=0;
	flash.EXO.Sensor_SOND[flash.EXO.UartPort][0]	=0;
	flash.EXO.Sensor_CHLIB[flash.EXO.UartPort][0]	=0;
	flash.EXO.Sensor_DEPTH[flash.EXO.UartPort][0]	=0;
	flash.EXO.Sensor_WIPER[flash.EXO.UartPort][0]	=0;
	flash.EXO.Sensor_FDOM[flash.EXO.UartPort][0]	=0;
    flash.EXO.Sensor_Ammoninum[flash.EXO.UartPort][0]	=0;

	//센서 ID를 삭제한다.
     for(a=0;a<8;a++)
      for(c=0;c<4;c++)
    	flash.EXO.SensorAddress[flash.EXO.UartPort][a][c] = 0;
    //PH
	//id = Button(handle, sx, sy, width, height, TEXT_BUTTON, NULL, NULL, "", FONT16, "pH", PUSH_BUTTON, 0, SmartLogEXOItemClickEvent);
    id = SButton(handle, sx, sy, width, height, 0, "pH",  PUSH_BUTTON, 0, SmartLogEXOItemClickEvent);
	SetWidgetColor(id,clBlue,color);	
	if(flash.EXO.Sensor_PH_Status[flash.EXO.UartPort][0]==ON) 
	{
		strcpy(fbuf,"ON");
	}
	else 
	{
		strcpy(fbuf,"OFF");
	}
    
	gSerialNoEdit[0] = TextBox(handle, sx+width2, sy, width, height, 2, fbuf, RGB565CONVERT(191, 191, 191), clBlack, CENTER_ALIGN, READ_ONLY, NULL);
	if(flash.EXO.Sensor_PH_Status[flash.EXO.UartPort][0]==ON)
	{
		SetWidgetColor(gSerialNoEdit[0],clWhite,clRed);
	}
	else 
	{
		SetWidgetColor(gSerialNoEdit[0],clWhite,OFF);
	}			   
    
    //pHmV
	//id = Button(handle, sx+width2+width2, sy, width, height, TEXT_BUTTON, NULL, NULL, "",  FONT16, "pHmV", PUSH_BUTTON, 1, SmartLogEXOItemClickEvent);
    id = SButton(handle, sx+width2+width2, sy, width, height, 0, "pHmV",  PUSH_BUTTON, 1, SmartLogEXOItemClickEvent);
    SetWidgetColor(id,clBlue,color);	
	if(flash.EXO.Sensor_PHMV_Status[flash.EXO.UartPort][0]==ON)sprintf(fbuf,"ON");
	else strcpy(fbuf,"OFF");
	gSerialNoEdit[1] = TextBox(handle, sx+width2+width2+width2, sy, width, height, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
			   SetWidgetColor(gSerialNoEdit[1],clWhite,clBlack);
	if(flash.EXO.Sensor_PHMV_Status[flash.EXO.UartPort][0]==ON)
	{
		SetWidgetColor(gSerialNoEdit[1],clWhite,clRed);
	}
	else 
	{
		SetWidgetColor(gSerialNoEdit[1],clWhite,OFF);
	}

	sy+=height2;
	//id= Button(handle, sx, sy, width, height, TEXT_BUTTON, NULL, NULL, "",  FONT16, "ORP", PUSH_BUTTON, 2, SmartLogEXOItemClickEvent);
    id = SButton(handle, sx, sy, width, height, 0, "ORP",  PUSH_BUTTON, 2, SmartLogEXOItemClickEvent);
    SetWidgetColor(id,clBlue,color);	
	if(flash.EXO.Sensor_ORP_Status[flash.EXO.UartPort][0]==ON)sprintf(fbuf,"ON");
	else strcpy(fbuf,"OFF");
	gSerialNoEdit[2] = TextBox(handle, sx+width2, sy, width, height, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
			   SetWidgetColor(gSerialNoEdit[2],clWhite,clBlack);
	if(flash.EXO.Sensor_ORP_Status[flash.EXO.UartPort][0]==ON)
	{ 
		SetWidgetColor(gSerialNoEdit[2],clWhite,clRed);
	}
	else 
	{
		SetWidgetColor(gSerialNoEdit[2],clWhite,OFF);
	}


	//id = Button(handle, sx+width2+width2, sy, width, height, TEXT_BUTTON, NULL, NULL, "", FONT16, "ODO sat", PUSH_BUTTON, 3, SmartLogEXOItemClickEvent);
    id = SButton(handle, sx+width2+width2, sy, width, height, 0, "ODO sat",  PUSH_BUTTON, 3, SmartLogEXOItemClickEvent);
    SetWidgetColor(id,clBlue,color);	
	if(flash.EXO.Sensor_ODO_SAT_Status[flash.EXO.UartPort][0]==ON)sprintf(fbuf,"ON");
	else strcpy(fbuf,"OFF");
	gSerialNoEdit[3] = TextBox(handle, sx+width2+width2+width2, sy, width, height, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
			   SetWidgetColor(gSerialNoEdit[3],clWhite,clBlack);
	if(flash.EXO.Sensor_ODO_SAT_Status[flash.EXO.UartPort][0]==ON)
	{
		SetWidgetColor(gSerialNoEdit[3],clWhite,clRed);
	}
	else 
	{
		SetWidgetColor(gSerialNoEdit[3],clWhite,OFF);
	}
	
	
	sy+=height2;
	//id = Button(handle, sx, sy, width, height, TEXT_BUTTON, NULL, NULL, "",  FONT16, "ODO mg/L", PUSH_BUTTON, 4, SmartLogEXOItemClickEvent);
    id = SButton(handle, sx, sy, width, height, 0, "ODO mg/L",  PUSH_BUTTON, 4, SmartLogEXOItemClickEvent);
    SetWidgetColor(id,clBlue,color);	
	if(flash.EXO.Sensor_ODO_MGL_Status[flash.EXO.UartPort][0]==ON)sprintf(fbuf,"ON");
	else strcpy(fbuf,"OFF");
	gSerialNoEdit[4] = TextBox(handle, sx+width2, sy, width, height, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
			   SetWidgetColor(gSerialNoEdit[4],clWhite,clBlack);
	if(flash.EXO.Sensor_ODO_MGL_Status[flash.EXO.UartPort][0]==ON)
	{
		SetWidgetColor(gSerialNoEdit[4],clWhite,clRed);
	}
	else 
	{
		SetWidgetColor(gSerialNoEdit[4],clWhite,OFF);
	}
	
	//id= Button(handle, sx+width2+width2, sy, width, height, TEXT_BUTTON, NULL, NULL, "",  FONT16, "SPCOND mS/cm", PUSH_BUTTON, 5, SmartLogEXOItemClickEvent);
    id = SButton(handle, sx+width2+width2, sy, width, height, 0, "SPCOND mS/cm",  PUSH_BUTTON, 5, SmartLogEXOItemClickEvent);
    SetWidgetColor(id,clBlue,color);	
	if(flash.EXO.Sensor_COND_SP_Status[flash.EXO.UartPort][0]==ON)sprintf(fbuf,"ON");
	else strcpy(fbuf,"OFF");
	gSerialNoEdit[5] = TextBox(handle, sx+width2+width2+width2, sy, width, height, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
			   SetWidgetColor(gSerialNoEdit[5],clWhite,clBlack);
	if(flash.EXO.Sensor_COND_SP_Status[flash.EXO.UartPort][0]==ON)
	{
		SetWidgetColor(gSerialNoEdit[5],clWhite,clRed);
	}
	else 
	{
		SetWidgetColor(gSerialNoEdit[5],clWhite,OFF);
	}

	
	
	sy+=height2;
	//id = Button(handle, sx, sy, width, height, TEXT_BUTTON, NULL, NULL, "", FONT16, "SAL ppt", PUSH_BUTTON, 6, SmartLogEXOItemClickEvent);
    id = SButton(handle, sx, sy, width, height, 0, "SAL ppt",  PUSH_BUTTON, 6, SmartLogEXOItemClickEvent);
    SetWidgetColor(id,clBlue,color);	
	if(flash.EXO.Sensor_COND_SAL_Status[flash.EXO.UartPort][0]==ON)sprintf(fbuf,"ON");
	else strcpy(fbuf,"OFF");
	gSerialNoEdit[6] = TextBox(handle, sx+width2, sy, width, height, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
			   SetWidgetColor(gSerialNoEdit[6],clWhite,clBlack);
	if(flash.EXO.Sensor_COND_SAL_Status[flash.EXO.UartPort][0]==ON)
	{
		SetWidgetColor(gSerialNoEdit[6],clWhite,clRed);
	}
	else 
	{
		SetWidgetColor(gSerialNoEdit[6],clWhite,OFF);
	}

	
	//id = Button(handle, sx+width2+width2, sy, width, height, TEXT_BUTTON, NULL, NULL, "",  FONT16, "TEMP", PUSH_BUTTON, 7, SmartLogEXOItemClickEvent);
    id = SButton(handle, sx+width2+width2, sy, width, height, 0, "TEMP 'C",  PUSH_BUTTON, 7, SmartLogEXOItemClickEvent);
    SetWidgetColor(id,clBlue,color);	
	if(flash.EXO.Sensor_COND_TEMP_Status[flash.EXO.UartPort][0]==ON)sprintf(fbuf,"ON");
	else strcpy(fbuf,"OFF");
	gSerialNoEdit[7] = TextBox(handle, sx+width2+width2+width2, sy, width, height, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
			   SetWidgetColor(gSerialNoEdit[7],clWhite,clBlack);
	if(flash.EXO.Sensor_COND_TEMP_Status[flash.EXO.UartPort][0]==ON)
	{
		SetWidgetColor(gSerialNoEdit[7],clWhite,clRed);
	}
	else 
	{
		SetWidgetColor(gSerialNoEdit[7],clWhite,OFF);
	}

	
	sy+=height2;
	//id= Button(handle, sx, sy, width, height, TEXT_BUTTON, NULL, NULL, "",  FONT16, "TUBIDITY NTU", PUSH_BUTTON, 8, SmartLogEXOItemClickEvent);
    id = SButton(handle, sx, sy, width, height, 0, "TURBIDITY NTU",  PUSH_BUTTON, 8, SmartLogEXOItemClickEvent);
    SetWidgetColor(id,clBlue,color);	
	if(flash.EXO.Sensor_TUB_FNU_Status[flash.EXO.UartPort][0]==ON)sprintf(fbuf,"ON");
	else strcpy(fbuf,"OFF");
	gSerialNoEdit[8] = TextBox(handle, sx+width2, sy, width, height, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
			   SetWidgetColor(gSerialNoEdit[8],clWhite,clBlack);
	if(flash.EXO.Sensor_TUB_FNU_Status[flash.EXO.UartPort][0]==ON)
	{
		SetWidgetColor(gSerialNoEdit[8],clWhite,clRed);
	}
	else 
	{
		SetWidgetColor(gSerialNoEdit[8],clWhite,OFF);
	}

    strcpy(data,"Battery");
	//id = Button(handle, sx+width2+width2, sy, width, height, TEXT_BUTTON, NULL, NULL, "", FONT16, data, PUSH_BUTTON, 9, SmartLogEXOItemClickEvent);
    id = SButton(handle, sx+width2+width2, sy, width, height, 0, "Battery V",  PUSH_BUTTON, 9, SmartLogEXOItemClickEvent);
    SetWidgetColor(id,clBlue,color);	
	if(flash.EXO.Sensor_Battery_Status[flash.EXO.UartPort][0]==ON)sprintf(fbuf,"ON");
	else sprintf(fbuf,"OFF");
	gSerialNoEdit[9] = TextBox(handle, sx+width2+width2+width2, sy, width, height, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
			   SetWidgetColor(gSerialNoEdit[9],clWhite,clBlack);
	if(flash.EXO.Sensor_Battery_Status[flash.EXO.UartPort][0]==ON)
	{
		SetWidgetColor(gSerialNoEdit[9],clWhite,clRed);
	}
	else 
	{
		SetWidgetColor(gSerialNoEdit[9],clWhite,OFF);
	}

	sy+=height2;		   
	//id = Button(handle, sx, sy, width, height, TEXT_BUTTON, NULL, NULL, "",  FONT16, "CHLOROPHYLL ug/L", PUSH_BUTTON, 10, SmartLogEXOItemClickEvent);
    id = SButton(handle, sx, sy, width, height, 0, "CHLOROPHYLL ug/L",  PUSH_BUTTON, 10, SmartLogEXOItemClickEvent);
    SetWidgetColor(id,clBlue,color);	
	if(flash.EXO.Sensor_CHLIB_UGL_Status[flash.EXO.UartPort][0]==ON)sprintf(fbuf,"ON");
	else strcpy(fbuf,"OFF");
	gSerialNoEdit[10] = TextBox(handle, sx+width2, sy, width, height, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
			   SetWidgetColor(gSerialNoEdit[10],clWhite,clBlack);
	if(flash.EXO.Sensor_CHLIB_UGL_Status[flash.EXO.UartPort][0]==ON)
	{
		SetWidgetColor(gSerialNoEdit[10],clWhite,clRed);
	}
	else 
	{
		SetWidgetColor(gSerialNoEdit[10],clWhite,OFF);
	}

	//id= Button(handle, sx+width2+width2, sy, width, height, TEXT_BUTTON, NULL, NULL, "",  FONT16, "CHLOROPHYLL RFU", PUSH_BUTTON, 11, SmartLogEXOItemClickEvent);
    id = SButton(handle, sx+width2+width2, sy, width, height, 0, "CHLOROPHYLL RFU",  PUSH_BUTTON, 11, SmartLogEXOItemClickEvent);
    SetWidgetColor(id,clBlue,color);	
	if(flash.EXO.Sensor_CHLIB_RFU_Status[flash.EXO.UartPort][0]==ON)sprintf(fbuf,"ON");
	else strcpy(fbuf,"OFF");
	gSerialNoEdit[11] = TextBox(handle, sx+width2+width2+width2, sy, width, height, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
			   SetWidgetColor(gSerialNoEdit[11],clWhite,clBlack);
	if(flash.EXO.Sensor_CHLIB_RFU_Status[flash.EXO.UartPort][0]==ON)
	{
		SetWidgetColor(gSerialNoEdit[11],clWhite,clRed);
	}
	else 
	{
		SetWidgetColor(gSerialNoEdit[11],clWhite,OFF);
	}

	sy+=height2;		   
	//id = Button(handle, sx, sy, width, height, TEXT_BUTTON, NULL, NULL, "",  FONT16, "BGAPC RFU", PUSH_BUTTON, 12, SmartLogEXOItemClickEvent);
    id = SButton(handle, sx, sy, width, height, 0, "BGAPC RFU",  PUSH_BUTTON, 12, SmartLogEXOItemClickEvent);
    SetWidgetColor(id,clBlue,color);	
	if(flash.EXO.Sensor_BGA_PCRFU_Status[flash.EXO.UartPort][0]==ON)sprintf(fbuf,"ON");
	else strcpy(fbuf,"OFF");
	gSerialNoEdit[12] = TextBox(handle, sx+width2, sy, width, height, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
			   SetWidgetColor(gSerialNoEdit[12],clWhite,clBlack);
	if(flash.EXO.Sensor_BGA_PCRFU_Status[flash.EXO.UartPort][0]==ON)
	{
		SetWidgetColor(gSerialNoEdit[12],clWhite,clRed);
	}
	else 
	{
		SetWidgetColor(gSerialNoEdit[12],clWhite,OFF);
	}

	//id= Button(handle, sx+width2+width2, sy, width, height, TEXT_BUTTON, NULL, NULL, "",  FONT16, "BGAPC ug/L", PUSH_BUTTON, 13, SmartLogEXOItemClickEvent);
    id = SButton(handle, sx+width2+width2, sy, width, height, 0, "BGAPC ug/L",  PUSH_BUTTON, 13, SmartLogEXOItemClickEvent);
    SetWidgetColor(id,clBlue,color);	
	if(flash.EXO.Sensor_BGA_PCUGL_Status[flash.EXO.UartPort][0]==ON)sprintf(fbuf,"ON");
	else strcpy(fbuf,"OFF");
	gSerialNoEdit[13] = TextBox(handle, sx+width2+width2+width2, sy, width, height, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
			   SetWidgetColor(gSerialNoEdit[13],clWhite,clBlack);
	if(flash.EXO.Sensor_BGA_PCUGL_Status[flash.EXO.UartPort][0]==ON)
	{
		SetWidgetColor(gSerialNoEdit[13],clWhite,clRed);
	}
	else 
	{
		SetWidgetColor(gSerialNoEdit[13],clWhite,OFF);
	}

	
	sy+=height2;		   
	//id = Button(handle, sx, sy, width, height, TEXT_BUTTON, NULL, NULL, "",  FONT16, "BGAPE RFU", PUSH_BUTTON, 14, SmartLogEXOItemClickEvent);
    id = SButton(handle, sx, sy, width, height, 0, "BGAPE RFU",  PUSH_BUTTON, 14, SmartLogEXOItemClickEvent);
    SetWidgetColor(id,clBlue,color);	
	if(flash.EXO.Sensor_BGA_PERFU_Status[flash.EXO.UartPort][0]==ON)sprintf(fbuf,"ON");
	else strcpy(fbuf,"OFF");
	gSerialNoEdit[14] = TextBox(handle, sx+width2, sy, width, height, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
			   SetWidgetColor(gSerialNoEdit[14],clWhite,clBlack);
	if(flash.EXO.Sensor_BGA_PERFU_Status[flash.EXO.UartPort][0]==ON)
	{
		SetWidgetColor(gSerialNoEdit[14],clWhite,clRed);
	}
	else 
	{
		SetWidgetColor(gSerialNoEdit[14],clWhite,OFF);
	}
	
			   
	//id= Button(handle, sx+width2+width2, sy, width, height, TEXT_BUTTON, NULL, NULL, "",  FONT16, "BGAPE ug/L", PUSH_BUTTON, 15, SmartLogEXOItemClickEvent);
    id = SButton(handle, sx+width2+width2, sy, width, height, 0, "BGAPE ug/L",  PUSH_BUTTON, 15, SmartLogEXOItemClickEvent);
    SetWidgetColor(id,clBlue,color);	
	if(flash.EXO.Sensor_BGA_PEUGL_Status[flash.EXO.UartPort][0]==ON)sprintf(fbuf,"ON");
	else strcpy(fbuf,"OFF");
	gSerialNoEdit[15] = TextBox(handle, sx+width2+width2+width2, sy, width, height, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
			   SetWidgetColor(gSerialNoEdit[15],clWhite,clBlack);
	if(flash.EXO.Sensor_BGA_PEUGL_Status[flash.EXO.UartPort][0]==ON)
	{
		SetWidgetColor(gSerialNoEdit[15],clWhite,clRed);
	}
	else 
	{
		SetWidgetColor(gSerialNoEdit[15],clWhite,OFF);
	}

	
	sy+=height2;		   
	//id = Button(handle, sx, sy, width, height, TEXT_BUTTON, NULL, NULL, "",  FONT16, "DEPTH m", PUSH_BUTTON, 16, SmartLogEXOItemClickEvent);
    id = SButton(handle, sx, sy, width, height, 0, "DEPTH m",  PUSH_BUTTON, 16, SmartLogEXOItemClickEvent);
    SetWidgetColor(id,clBlue,color);	
	if(flash.EXO.Sensor_DEPTH_Status[flash.EXO.UartPort][0]==ON)sprintf(fbuf,"ON");
	else strcpy(fbuf,"OFF");
	gSerialNoEdit[16] = TextBox(handle, sx+width2, sy, width, height, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
			   SetWidgetColor(gSerialNoEdit[16],clWhite,clBlack);
	if(flash.EXO.Sensor_DEPTH_Status[flash.EXO.UartPort][0]==ON)
	{
		SetWidgetColor(gSerialNoEdit[16],clWhite,clRed);
	}
	else 
	{
		SetWidgetColor(gSerialNoEdit[16],clWhite,OFF);
	}

    #ifdef _Ammoninum
	//id= Button(handle, sx+width2+width2, sy, width, height, TEXT_BUTTON, NULL, NULL, "",  FONT16, "Ammoninum", PUSH_BUTTON, 17, SmartLogEXOItemClickEvent);
    id = SButton(handle, sx+width2+width2, sy, width, height, 0, "Ammoninum",  PUSH_BUTTON, 17, SmartLogEXOItemClickEvent);
    SetWidgetColor(id,clBlue,color);	
	if(flash.EXO.Sensor_Ammoninum_Status[flash.EXO.UartPort][0]==ON)sprintf(fbuf,"ON");
	else strcpy(fbuf,"OFF");
	gSerialNoEdit[17] = TextBox(handle, sx+width2+width2+width2, sy, width, height, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
			   SetWidgetColor(gSerialNoEdit[17],clWhite,clBlack);
	if(flash.EXO.Sensor_Ammoninum_Status[flash.EXO.UartPort][0]==ON)
	{
		SetWidgetColor(gSerialNoEdit[17],clWhite,clRed);
	}
	else 
	{
		SetWidgetColor(gSerialNoEdit[17],clWhite,OFF);
	}
    #endif

	sy+=height2;		   
	//id = Button(handle, sx, sy, width, height, TEXT_BUTTON, NULL, NULL, "",  FONT16, "TSS", PUSH_BUTTON, 26, SmartLogEXOItemClickEvent);
    id = SButton(handle, sx, sy, width, height, 0, "TSS",  PUSH_BUTTON, 26, SmartLogEXOItemClickEvent);
    SetWidgetColor(id,clBlue,color);	
	if(flash.EXO.Sensor_TSS_MGL_Status[flash.EXO.UartPort][0]==ON)sprintf(fbuf,"ON");
	else strcpy(fbuf,"OFF");
	gSerialNoEdit[26] = TextBox(handle, sx+width2, sy, width, height, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
			   SetWidgetColor(gSerialNoEdit[26],clWhite,clBlack);
	if(flash.EXO.Sensor_TSS_MGL_Status[flash.EXO.UartPort][0]==ON)
	{
		SetWidgetColor(gSerialNoEdit[26],clWhite,clRed);
	}
	else 
	{
		SetWidgetColor(gSerialNoEdit[26],clWhite,OFF);
	}

    #ifdef _FDOM_RFU
	//id= Button(handle, sx+width2+width2, sy, width, height, TEXT_BUTTON, NULL, NULL, "",  FONT16, "FDOM RFU", PUSH_BUTTON, 18, SmartLogEXOItemClickEvent);
	id = SButton(handle, sx+width2+width2, sy, width, height, 0, "FDOM RFU",  PUSH_BUTTON, 17, SmartLogEXOItemClickEvent);
				  SetWidgetColor(id,clBlue,color);	
	if(flash.EXO.Sensor_FDOM_RFU_Status[flash.EXO.UartPort][0]==ON)sprintf(fbuf,"ON");
	else strcpy(fbuf,"OFF");
	gSerialNoEdit[17] = TextBox(handle, sx+width2+width2+width2, sy, width, height, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
			   SetWidgetColor(gSerialNoEdit[17],clWhite,clBlack);
	if(flash.EXO.Sensor_FDOM_RFU_Status[flash.EXO.UartPort][0]==ON)
	{
		SetWidgetColor(gSerialNoEdit[17],clWhite,clRed);
	}
	else 
	{
		SetWidgetColor(gSerialNoEdit[17],clWhite,OFF);
	}
    #endif
    #ifdef _FDOM_QSU
	//id= Button(handle, sx+width2+width2, sy, width, height, TEXT_BUTTON, NULL, NULL, "",  FONT16, "FDOM QSU", PUSH_BUTTON, 18, SmartLogEXOItemClickEvent);
	id = SButton(handle, sx+width2+width2, sy, width, height, 0, "FDOM RFU",  PUSH_BUTTON, 17, SmartLogEXOItemClickEvent);	
		SetWidgetColor(id,clBlue,color);	
	if(flash.EXO.Sensor_FDOM_QSU_Status[flash.EXO.UartPort][0]==ON)sprintf(fbuf,"ON");
	else strcpy(fbuf,"OFF");
	gSerialNoEdit[17] = TextBox(handle, sx+width2+width2+width2, sy, width, height, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
			   SetWidgetColor(gSerialNoEdit[17],clWhite,clBlack);
	if(flash.EXO.Sensor_FDOM_QSU_Status[flash.EXO.UartPort][0]==ON)
	{
		SetWidgetColor(gSerialNoEdit[17],clWhite,clRed);
	}
	else 
	{
		SetWidgetColor(gSerialNoEdit[17],clWhite,OFF);
	}
    #endif
//***************************************************************************************************************************************
//		센서를 검지하여 항목이름을 출력한다.
//***************************************************************************************************************************************
	
	sy+=height2;
	sy+=height2;		   
	/*
	gSerialNoEdit[18] = 	TextBox(handle, sx, sy, width, height, 2, "PORT.1", RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
								SetWidgetColor(gSerialNoEdit[18],clWhite,clBlack);
	gSerialNoEdit[19] = 	TextBox(handle, sx+width2, sy, width, height, 2, "PORT.2", RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
								SetWidgetColor(gSerialNoEdit[19],clWhite,clBlack);
	gSerialNoEdit[20] = 	TextBox(handle, sx+width2+width2, sy, width, height, 2, "PORT.3", RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
								SetWidgetColor(gSerialNoEdit[20],clWhite,clBlack);
	gSerialNoEdit[21] = 	TextBox(handle, sx+width2+width2+width2, sy, width, height, 2, "PORT.4", RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
								SetWidgetColor(gSerialNoEdit[21],clWhite,clBlack);


    sy+=height2;		   
	gSerialNoEdit[22] = 	TextBox(handle, sx, sy, width, height, 2, "PORT.5", RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
								SetWidgetColor(gSerialNoEdit[22],clWhite,clBlack);
	gSerialNoEdit[23] = 	TextBox(handle, sx+width2, sy, width, height, 2, "PORT.6", RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
								SetWidgetColor(gSerialNoEdit[23],clWhite,clBlack);
	gSerialNoEdit[24] = 	TextBox(handle, sx+width2+width2, sy, width, height, 2, "PORT.7", RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
								SetWidgetColor(gSerialNoEdit[24],clWhite,clBlack);
	gSerialNoEdit[25] = 	TextBox(handle, sx+width2+width2+width2, sy, width, height, 2, "PORT.8", RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
								SetWidgetColor(gSerialNoEdit[25],clWhite,clBlack);
	*/
	//SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, dataBuf,  PUSH_BUTTON, 34, SystemMain_KeyEvent);				
	gSerialNoEdit[18] = 	SButton(handle, sx, sy, width, height, 0, "PORT.1",  PUSH_BUTTON, 18, NULL);			
								SetWidgetColor(gSerialNoEdit[18],clWhite,clBlack);
	gSerialNoEdit[19] = 	SButton(handle, sx+width2, sy, width, height, 0, "PORT.2",  PUSH_BUTTON, 19, NULL);
								SetWidgetColor(gSerialNoEdit[19],clWhite,clBlack);
	gSerialNoEdit[20] = 	SButton(handle, sx+width2+width2, sy, width, height, 0, "PORT.3",  PUSH_BUTTON, 20, NULL);
								SetWidgetColor(gSerialNoEdit[20],clWhite,clBlack);
	gSerialNoEdit[21] = 	SButton(handle, sx+width2+width2+width2, sy, width, height, 0, "PORT.4",  PUSH_BUTTON, 21, NULL);
								SetWidgetColor(gSerialNoEdit[21],clWhite,clBlack);


    sy+=height2;		   
	gSerialNoEdit[22] = 	SButton(handle, sx, sy, width, height, 0, "PORT.5",  PUSH_BUTTON, 22, NULL);
								SetWidgetColor(gSerialNoEdit[22],clWhite,clBlack);
	gSerialNoEdit[23] = 	SButton(handle, sx+width2, sy, width, height, 0, "PORT.6",  PUSH_BUTTON, 23, NULL);
								SetWidgetColor(gSerialNoEdit[23],clWhite,clBlack);
	gSerialNoEdit[24] = 	SButton(handle, sx+width2+width2, sy, width, height, 0, "PORT.7",  PUSH_BUTTON, 24, NULL);
								SetWidgetColor(gSerialNoEdit[24],clWhite,clBlack);
	gSerialNoEdit[25] = 	SButton(handle, sx+width2+width2+width2, sy, width, height, 0, "PORT.8",  PUSH_BUTTON, 25, NULL);
								SetWidgetColor(gSerialNoEdit[25],clWhite,clBlack);

	
	sy+=height2;
    //id 	= Button(handle, sx+width2+width2+width2, sy, width, height, TEXT_BUTTON, NULL, NULL, "",  FONT16, "EXIT", PUSH_BUTTON, 19, SmartLogEXOItemClickEvent);
    id = NButton(handle, sx+width2+width2+width2, sy+10, width, height+10, 0, "EXIT",  PUSH_BUTTON, 19, SmartLogEXOItemClickEvent);
    SetWidgetColor(id,clBlue,clWhite);	

	gSerialNoEdit[27] = NButton(handle, sx, sy+10, width*3, height+10, 0, " ",  PUSH_BUTTON, 27, SmartLogEXOItemClickEvent);
    SetWidgetColor(id,clBlue,clWhite);	

    PaintDrawControl(ON);	
    Gr->Draw(parent, handle, command, param, Data);

	ExoTimerId=OpenTimer(parent, handle, 1000);		// 1초간격으로 메시지를 출력한다.
	StartTimer(ExoTimerId);
	
	return(1);
}

void SmartLogEXOItemKeypad(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title)
{
	WidgetStruct wid_struct;	

	wid_struct.FontNo = font_no;
	wid_struct.CallWidget_ID = id;

	makeWindow(
				LCD,
				"EXOItemKeypad",				// Windows Name
				DESKTOP_WINDOW,			// Parent Window Handle
				CONTAINER_WINDOW,		// Windows Kind
				XP_STYLE,				// Windows Style
				sx, sy, 				// Start Position
				940, 700,			// Size
				SHOW,					// Create after Show
				SmartLogEXOItemCreateEvent,		// CreateEvent
				SmartLogEXOItemPaintEvent,		// PaintEvent
				NULL,					// MouseOnEvent
				NULL,					// MouseOffEvent
				NULL,					// MouseMoveEvent
				NULL,					// GestureEvent
				NULL,					// ClickEvent
				SmartLogEXOMessageTimerEvent,					// TimerEvent
				NULL,  					// ControlEvent
				NULL,  					// ChangeEvent
				NULL,
				NULL,
				NULL,
				NULL,
				GRAY7,					// PARAMETER
				value,					// Normal Touch Screen
				0,						// Touch Screen
				title,					// Windows Text
				MODALLESS_WINDOW,
				&wid_struct
				);
				
	//ShowWindow(NULL, mainHandle);
}
#endif
