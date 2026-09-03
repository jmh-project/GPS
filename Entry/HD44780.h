

#define HD44780_CMD_RESET            	0x30     	/*!< Resets display - used in init 3x */
#define HD44780_CMD_CLEAR            	0x01     	/*!< Clears display */
#define HD44780_CMD_RETURN_HOME      	0x02     	/*!< Sets DDRAM pointer to 0 */
#define HD44780_CMD_ENTRY_MODE       	0x04     	/*!< Sets how the pointer is updated after a character write */
#define HD44780_CMD_DISPLAY          	0x08     	/*!< Display settings */
#define HD44780_CMD_SHIFT            	0x10     	/*!< Cursor and display movement */
#define HD44780_CMD_FUNCTION         	0x20     	/*!< Screen type setup */
#define HD44780_CMD_CGRAM_ADDR       	0x40     	/*!< Sets CGRAM address */
#define HD44780_CMD_DDRAM_ADDR       	0x80     	/*!< Sets DDRAM address */

/* ENTRY_MODE Command parameters */
#define HD44780_ENTRY_SHIFT_DISP 			0x01	 		/*!< Shift display */
#define HD44780_ENTRY_SHIFT_CURS 			0x00	 		/*!< Shift cursor */
#define HD44780_ENTRY_ADDR_INC   			0x02     	/*!< Increments pointer */
#define HD44780_ENTRY_ADDR_DEC   			0x00	 		/*!< Decrements pointer */

/* DISPLAY Command parameters */
#define HD44780_DISP_ON       				0x04      /*!< Enables the display */
#define HD44780_DISP_OFF      				0x00      /*!< Disables the display */
#define HD44780_DISP_CURS_ON  				0x02      /*!< Enables cursor */
#define HD44780_DISP_CURS_OFF 				0x00      /*!< Disables cursor */
#define HD44780_DISP_BLINK_ON					0x01      /*!< Enables cursor blinking */
#define HD44780_DISP_BLINK_OFF  			0x00      /*!< Disables cursor blinking */

/* SHIFT Command parameters */
#define HD44780_SHIFT_DISPLAY    			0x08      /*!< Shifts the display or shifts the cursor if not set */
#define HD44780_SHIFT_CURSOR    			0x00      /*!< Shifts the display or shifts the cursor if not set */
#define HD44780_SHIFT_RIGHT      			0x04      /*!< Shift to the right */
#define HD44780_SHIFT_LEFT      			0x00      /*!< Shift to the left  */

/* FUNCTION Command parameters */
#define HD44780_FUNC_BUS_8BIT  				0x10      /*!< 8 bit bus */
#define HD44780_FUNC_BUS_4BIT  				0x00      /*!< 4 bit bus */
#define HD44780_FUNC_LINES_2   				0x08      /*!< 2 lines */
#define HD44780_FUNC_LINES_1   				0x00      /*!< 1 line */
#define HD44780_FUNC_FONT_5x10 				0x04      /*!< 5x10 font */
#define HD44780_FUNC_FONT_5x8  				0x00      /*!< 5x8 font */

void LCD_Display( char *str1, char *str2, char *str3, char *str4 );
void LCD_Init( void );


