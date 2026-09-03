/******************************************************************************
							Include File
******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif

#include "TEECO_System.h"
#ifdef W5500
#include "BSP_W5500.h"
#include "T_socket.h"
#include "T_dhcp.h"


#include "T_loopback.h"


///////////////////////////////////////////////////////////////
// Loopback Test Example Code using ioLibrary_BSD			 //
///////////////////////////////////////////////////////////////
int32_t loopback_tcps(uint8_t sn, uint8_t* buf, uint16_t port)
{
	int32_t ret;
	uint16_t size = 0, sentsize=0;
	switch(getSn_SR(sn))
	{
		case SOCK_ESTABLISHED :
			if(getSn_IR(sn) & Sn_IR_CON)
			{
				Dprintf("loopback_tcps::%d:Connected\r\n",sn);
				setSn_IR(sn,Sn_IR_CON);
			}
			if((size = getSn_RX_RSR(sn)) > 0)
			{
				if(size > DATA_BUF_SIZE) size = DATA_BUF_SIZE;
				ret = recv(sn,buf,size);
				if(ret <= 0) return ret;
				sentsize = 0;
				while(size != sentsize)
				{
					ret = send(sn,buf+sentsize,size-sentsize);
					if(ret < 0)
					{
						close(sn);
						return ret;
					}
					sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
				}
			}
        break;
			
		case SOCK_CLOSE_WAIT :
			Dprintf("loopback_tcps::%d:CloseWait\r\n",sn);
			if((ret=disconnect(sn)) != SOCK_OK) return ret;
			Dprintf("loopback_tcps::%d:Closed\r\n",sn);
        break;
		case SOCK_INIT :
			Dprintf("loopback_tcps::%d:Listen, port [%d]\r\n",sn, port);
			if( (ret = listen(sn)) != SOCK_OK) return ret;
        break;
		case SOCK_CLOSED:
			Dprintf("loopback_tcps::%d:LBTStart\r\n",sn);
			if((ret=socket(sn,Sn_MR_TCP,port,0x00)) != sn)
				return ret;
			Dprintf("loopback_tcps::%d:Opened\r\n",sn);
		break;
		
		default:
        break;
   }
   return 1;
}
#endif
#ifdef __cplusplus
}
#endif
