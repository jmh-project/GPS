/**
  ******************************************************************************
  * @file       Enum_Example.c
  * @author  (주)자이언텍 기술개발지원팀
  * @version V0.1
  * @date    2015년 4월21일
  * @brief    
                
  *          
  * @attention
  * 이파일은 자이언텍 지적재산권이므로 타사에 소스를 제공해서는 안됩니다. 
  * <h2><center>&copy; COPYRIGHT 2014 ZionTech Inc.</center></h2>
  * @warning  
  * @verbatim
  *  Edit History
  * @endverbatim  
  ******************************************************************************  
  */
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "TEECO_System.h"
#if 0
#define DRIVER_NAME "tsc2003"
	 
enum tsc2003_pd {
	PD_POWERDOWN = 0, /* penirq */
	PD_IREFOFF_ADCON = 1, /* no penirq */
	PD_IREFON_ADCOFF = 2, /* penirq */
	PD_IREFON_ADCON = 3, /* no penirq */
	PD_PENIRQ_ARM = PD_IREFON_ADCOFF,
	PD_PENIRQ_DISARM = PD_IREFON_ADCON,
};

enum tsc2003_m {
	M_12BIT = 0,
	M_8BIT = 1
};

enum tsc2003_cmd {
	MEAS_TEMP0 = 0,
	MEAS_VBAT1 = 1,
	MEAS_IN1 = 2,
	MEAS_TEMP1 = 4,
	MEAS_VBAT2 = 5,
	MEAS_IN2 = 6,
	ACTIVATE_NX_DRIVERS = 8,
	ACTIVATE_NY_DRIVERS = 9,
	ACTIVATE_YNX_DRIVERS = 10,
	MEAS_XPOS = 12,
	MEAS_YPOS = 13,
	MEAS_Z1POS = 14,
	MEAS_Z2POS = 15
};

struct tsc2003_data 
{
	enum tsc2003_pd pd;
	enum tsc2003_m m;
	int vbat1;
	int vbat2;
	int temp0;
	int temp1;
	int in1;
	int in2;
};

#define TSC2003_CMD(cn,pdn,m) (((cn) << 4) | ((pdn) << 2) | ((m) << 1))

static inline int tsc2003_command (struct tsc2003_data *data, enum tsc2003_cmd cmd, 	enum tsc2003_pd pd)
{
	char c;
	c = TSC2003_CMD(cmd, pd, data->m);
	return c;
}	 

void EnumTest(void)
{
	struct tsc2003_data data;
	enum tsc2003_cmd cmd;
	enum tsc2003_pd pd;
	int a = tsc2003_command(&data, cmd, pd);
	Dprintf("Enum data=%d\n", a);
}
#endif	 
#ifdef __cplusplus
}
#endif


