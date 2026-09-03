#include "TEECO_System.h"
#include "DataLog.h"

int ReceiveQuescanGPS( int ascii, int comPort ) {
    
	if(ascii == '$') {
		gv.nFlag_ReceiveStart[comPort]=1;
		gv.nReceiveBufferCount[comPort]=0;
		gv.aItemReceiveBuffer[comPort][gv.nReceiveBufferCount[comPort]++] = ascii;
	}
	else if(gv.nFlag_ReceiveStart[comPort]==1) {
		gv.aItemReceiveBuffer[comPort][gv.nReceiveBufferCount[comPort]++] = ascii;
        
		if(ascii==0x0A) {
            if((gv.aItemReceiveBuffer[comPort][1]) == 'G') && (gv.aItemReceiveBuffer[comPort][2] == 'P') && 
            (gv.aItemReceiveBuffer[comPort][3] == 'R') && (gv.aItemReceiveBuffer[comPort][4] == 'M') && (gv.aItemReceiveBuffer[comPort][5] == 'C'))
            {
                if(gv.aItemReceiveBuffer[comPort][18] == 'V') //데이터송신대기
                {
                    gv.nFlag_ReceiveStart[comPort]=0;
                    gv.nReceiveBufferCount[comPort]=0;
                }
                else if(gv.aItemReceiveBuffer[comPort][18] == 'A' && gv.aItemReceiveBuffer[comPort][30] == 'N')
                {
                    //gv.nFlag_ReceiveDataProcess[comPort] = ENABLE;
                    io.gpgamcAvalue[0]=io.buf[uart][20];
                    io.gpgamcAvalue[1]=io.buf[uart][21];
                    io.gpgamcAvalue[2]=io.buf[uart][22];
                    io.gpgamcAvalue[3]=io.buf[uart][23];
                    io.gpgamcAvalue[4]=io.buf[uart][24];
                    io.gpgamcAvalue[5]=io.buf[uart][25];
                    io.gpgamcAvalue[6]=io.buf[uart][26];
                    io.gpgamcAvalue[7]=io.buf[uart][27];
                    io.gpgamcAvalue[8]=io.buf[uart][28];
                    io.gpgamcAvalue[9]=0;
                    io.gpgamcNvalue[0]=io.buf[uart][32];
                    io.gpgamcNvalue[1]=io.buf[uart][33];
                    io.gpgamcNvalue[2]=io.buf[uart][34];
                    io.gpgamcNvalue[3]=io.buf[uart][35];
                    io.gpgamcNvalue[4]=io.buf[uart][36];
                    io.gpgamcNvalue[5]=io.buf[uart][37];
                    io.gpgamcNvalue[6]=io.buf[uart][38];
                    io.gpgamcNvalue[7]=io.buf[uart][39];
                    io.gpgamcNvalue[8]=io.buf[uart][40];
                    io.gpgamcNvalue[9]=io.buf[uart][41];
                    io.gpgamcNvalue[10]=0;
                    
                    gv.nFlag_ReceiveStart[comPort]=0;
                    gv.nReceiveBufferCount[comPort]=0;
                    
                }
            }
            if(gv.nReceiveBufferCount[comPort] > 256) {
                gv.nFlag_ReceiveStart[comPort]=0;
                gv.nReceiveBufferCount[comPort]=0;
                gv.nFlag_ReceiveDataProcess[comPort]=DISABLE;
            }
        }
	}
	return 1;
}