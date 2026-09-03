#include "TEECO_System.h"

int AD420mAInput(void) {
    int ch=0;
    for(ch = 0; ch < INPUT_MAX_CHANNEL; ch++ ) {
        if(flash.Input[ch]->CalHighmV > 0.0F && flash.Input[ch]->CalHighLimit > 0.0F) {
			if(flash.Input[ch]->mV > 300.0f) {
				flash.Input[ch]->PV = flash.Input[ch]->CalLowLimit + ((flash.Input[ch]->mV - flash.Input[ch]->CalLowmV) / 
				(flash.Input[ch]->CalHighmV - flash.Input[ch]->CalLowmV)) * (flash.Input[ch]->CalHighLimit - flash.Input[ch]->CalLowLimit);
			}
            //Dprintf("PV[%d] =%7.2f mV = %7.1f\n",ch,flash.Input[ch]->PV,flash.Input[ch]->mV);
        }
        else flash.Input[ch]->PV = 0.0f;
        //레인지 오버인경우 최대최소값을 적용한다.
        //if(flash.Input[ch]->PV <= flash.Input[ch]->CalLowLimit) flash.Input[ch]->PV =  flash.Input[ch]->CalLowLimit;
        //if(flash.Input[ch]->PV >= flash.Input[ch]->CalHighLimit) flash.Input[ch]->PV =  flash.Input[ch]->CalHighLimit;
    }
	return 0;
}
int AD420mAOutput(void) {
    int port = 0;
	int UartChannel = 0;
    
	for(UartChannel=0;UartChannel<USART_MAX;UartChannel++) {		//USART 5채널 검사하여 출력한다. 
        for(int ch=0;ch<OUTPUT_MAX_CHANNEL;ch++) {                    //OUTPUT_MAX_CHANNEL=10
            for(int UartItem=0;UartItem<USART_SENSOR_MAX;UartItem++) {//UART 채널당 15개(USART_SENSOR_MAX)의 항목을 사용한다.
				if(strcmp(flash.Uart[UartChannel]->SetupName[UartItem],flash.Output[ch]->Name)==0) 
				{	
					if(flash.Output[ch]->Name[0]=='N'&& flash.Output[ch]->Name[1]=='O'&& flash.Output[ch]->Name[2]=='N'&& flash.Output[ch]->Name[3]=='E') {
                        //데이터를 출력하지 않는다.
					}
                    else {
                        gv.value420Output[port++] = flash.Uart[UartChannel]->SensorPV[UartItem]; //출력되는 데이터
                    }
				}
			} 
		}
	}
    if(gv.cal420Toggle==OFF)//보정시에는 데이터를 출력못하게 하기위함이다.
    {
       //Dprintf("[%d] 420 = %f, 420 = %f, 420 = %f, 420 = %f,\n",port, gv.value420Output[0], gv.value420Output[1], gv.value420Output[2], gv.value420Output[3]);
       Int420mAOutput();//4채널 데이터를 한번에 출력한다.
    }
	return 0;
}
int	Int420mAOutput( void ) {
	float	fProcessValue 	= 0.0F;
	float	fValue = 0.0F;
	unsigned int ch = 0;
	unsigned short value[20];
    
	for( ch = 0; ch < OUTPUT_MAX_CHANNEL; ch++ )//gv.value420Output
    {		
        if(gv.value420Output[ch] < flash.Output[ch]->CalLowLimit) fValue = flash.Output[ch]->CalLowLimit;
        else fValue = gv.value420Output[ch];
        if(gv.value420Output[ch] > flash.Output[ch]->CalHighLimit ) fValue = flash.Output[ch]->CalHighLimit;
        else fValue = gv.value420Output[ch];
        
        if(flash.Output[ch]->CalHighLimit > 0.0F ) 
        {
            fProcessValue = flash.Output[ch]->CalHighLimit - flash.Output[ch]->CalLowLimit;
            fProcessValue = 65535.0F / fProcessValue;
            fProcessValue = (fProcessValue * fValue) - (fProcessValue * flash.Output[ch]->CalLowLimit);
            value[ch] = (unsigned short)fProcessValue;
        }
		else
		{			
			value[ch] = 0;			
		}
    }
	
	gv.ad420Ch1mA=value[0];
	gv.ad420Ch2mA=value[1];
	
	#if 0 //8채널 AD 보드 사용시 사용한다.
    UART_AD420_Control(value[0],value[1]);//이함수를 사용하면 데이터가 깨지는 현상이 발생한다.
	SENSOR_UART2( 0x02 );
	SENSOR_UART2( 0x30 );//PORT
	
	SENSOR_UART2((value[2]>>8) & 0x00FF);
	SENSOR_UART2(value[2] & 0x00FF);

	SENSOR_UART2((value[3]>>8) & 0x00FF);
	SENSOR_UART2(value[3] & 0x00FF);

	SENSOR_UART2((value[4]>>8) & 0x00FF);
	SENSOR_UART2(value[4] & 0x00FF);

	SENSOR_UART2((value[5]>>8) & 0x00FF);
	SENSOR_UART2(value[5] & 0x00FF);

	SENSOR_UART2((value[6]>>8) & 0x00FF);
	SENSOR_UART2(value[6] & 0x00FF);

	SENSOR_UART2((value[7]>>8) & 0x00FF);
	SENSOR_UART2(value[7] & 0x00FF);

	SENSOR_UART2((value[8]>>8) & 0x00FF);
	SENSOR_UART2(value[8] & 0x00FF);

	SENSOR_UART2((value[9]>>8) & 0x00FF);
	SENSOR_UART2(value[9] & 0x00FF);
	
	SENSOR_UART2( 0x03 );
	#endif
	return (1);	
}
