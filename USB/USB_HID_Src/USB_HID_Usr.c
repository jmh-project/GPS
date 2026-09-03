#include "TEECO_System.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"
#include "Debug_Uart.h"

void USBH_USR_ApplicationSelected(void);
void USBH_USR_Init(void);
void USBH_USR_DeInit(void);
void USBH_USR_DeviceAttached(void);
void USBH_USR_ResetDevice(void);
void USBH_USR_DeviceDisconnected (void);
void USBH_USR_OverCurrentDetected (void);
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed); 
void USBH_USR_Device_DescAvailable(void *);
void USBH_USR_DeviceAddressAssigned(void);
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc);
void USBH_USR_Manufacturer_String(void *);
void USBH_USR_Product_String(void *);
void USBH_USR_SerialNum_String(void *);
void USBH_USR_EnumerationDone(void);
USBH_USR_Status USBH_USR_UserInput(void);
void USBH_USR_DeInit(void);
void USBH_USR_DeviceNotSupported(void);
void USBH_USR_UnrecoveredError(void);



#define KYBRD_FIRST_COLUMN               (uint16_t)319
#define KYBRD_LAST_COLUMN                (uint16_t)7
#define KYBRD_FIRST_LINE                 (uint8_t)120
#define KYBRD_LAST_LINE                  (uint8_t)200


/**
* @}
*/ 
uint8_t  KeybrdCharXpos           = 0;
uint16_t KeybrdCharYpos           = 0;
extern  int16_t  x_loc, y_loc; 
extern __IO int16_t  prev_x, prev_y;


extern USB_OTG_CORE_HANDLE           USB_OTG_Core_dev;
/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USR_Callbacks =
{
  USBH_USR_Init,
  USBH_USR_DeInit,
  USBH_USR_DeviceAttached,
  USBH_USR_ResetDevice,
  USBH_USR_DeviceDisconnected,
  USBH_USR_OverCurrentDetected,
  USBH_USR_DeviceSpeedDetected,
  USBH_USR_Device_DescAvailable,
  USBH_USR_DeviceAddressAssigned,
  USBH_USR_Configuration_DescAvailable,
  USBH_USR_Manufacturer_String,
  USBH_USR_Product_String,
  USBH_USR_SerialNum_String,
  USBH_USR_EnumerationDone,
  USBH_USR_UserInput,
  NULL,
  USBH_USR_DeviceNotSupported,
  USBH_USR_UnrecoveredError
};

void USBH_USR_Init(void)
{
    static uint8_t startup = 0;  
  
    if(startup == 0 )
    {
        startup = 1;
    }
}

void USR_MOUSE_ProcessData(HID_MOUSE_Data_TypeDef *data)
{
  
    uint8_t idx = 1;   
    static uint8_t b_state[3] = { 0, 0, 0};
  
    if ((data->x != 0) && (data->y != 0))
    {
        HID_MOUSE_UpdatePosition(data->x , data->y);
    }
  
    for ( idx = 0 ; idx < 3 ; idx ++)
    {    
        if(data->button & 1 << idx) 
        {
            if(b_state[idx] == 0)
            {
                HID_MOUSE_ButtonPressed (idx);
                b_state[idx] = 1;
            }
        }
        else
        {
            if(b_state[idx] == 1)
            {
                HID_MOUSE_ButtonReleased (idx);
                b_state[idx] = 0;
            }
        }
    }  
}

/**
* @brief  USR_KEYBRD_Init
*         Init Keyboard window
* @param  None
* @retval None
*/
void  USR_KEYBRD_Init (void)
{
    /*
    LCD_UsrLog((void*)USB_HID_KeybrdStatus); 
    LCD_UsrLog("> Use Keyboard to tape characters: \n\n");   
    LCD_UsrLog("\n\n\n\n\n\n");
    LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 42, "                                   ");
    LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 30, "                                   ");  

    LCD_SetTextColor(Green);
    */
    KeybrdCharXpos = KYBRD_FIRST_LINE;
    KeybrdCharYpos = KYBRD_FIRST_COLUMN;
}


/**
* @brief  USR_KEYBRD_ProcessData
*         Process Keyboard data
* @param  data : Keyboard data to be displayed
* @retval None
*/
void  USR_KEYBRD_ProcessData (uint8_t data)
{
    //Dputc(data);
    /*
  if(data == '\n')
  {
    KeybrdCharYpos = KYBRD_FIRST_COLUMN;
    
    //Increment char X position
    KeybrdCharXpos+=SMALL_FONT_LINE_WIDTH;
    
  }
  else if (data == '\r')
  {
    // Manage deletion of charactter and upadte cursor location
    if( KeybrdCharYpos == KYBRD_FIRST_COLUMN) 
    {
      //First character of first line to be deleted
      if(KeybrdCharXpos == KYBRD_FIRST_LINE)
      {  
        KeybrdCharYpos =KYBRD_FIRST_COLUMN; 
      }
      else
      {
        KeybrdCharXpos-=SMALL_FONT_LINE_WIDTH;
        KeybrdCharYpos =(KYBRD_LAST_COLUMN+SMALL_FONT_COLUMN_WIDTH); 
      }
    }
    else
    {
      KeybrdCharYpos +=SMALL_FONT_COLUMN_WIDTH;                  
      
    } 
    LCD_DisplayChar(KeybrdCharXpos,KeybrdCharYpos, ' '); 
  }
  else
  {
    LCD_DisplayChar(KeybrdCharXpos,KeybrdCharYpos, data);    
    // Update the cursor position on LCD 
    
    //Increment char Y position
    KeybrdCharYpos -=SMALL_FONT_COLUMN_WIDTH;
    
    //Check if the Y position has reached the last column
    if(KeybrdCharYpos == KYBRD_LAST_COLUMN)
    {
      KeybrdCharYpos = KYBRD_FIRST_COLUMN;
      
      //Increment char X position
      KeybrdCharXpos+=SMALL_FONT_LINE_WIDTH;
      
    }
  }
  */
}


