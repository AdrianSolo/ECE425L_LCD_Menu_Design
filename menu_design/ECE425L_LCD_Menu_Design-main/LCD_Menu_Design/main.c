/*
 * @file main.c
 *
 * @brief Main source code for the LCD Menu Design program.
 *
 * This file contains the main entry point and function definitions for the LCD Menu Design program.
 * It interfaces with the following:
 *	- EduBase Board LEDs (LED0 - LED3)
 *	- EduBase Board 16x2 Liquid Crystal Display (LCD)
 *  - PMOD ENC Module (Rotary Encoder)
 *
 * @note For more information regarding the LCD, refer to the HD44780 LCD Controller Datasheet.
 * Link: https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
 *
 * @author Adrian Solorzano 
 */

#include "TM4C123GH6PM.h"

#include "SysTick_Delay.h"
#include "EduBase_LCD.h"

#include "PMOD_ENC.h"
#include "Timer_0A_Interrupt.h"

#include "GPIO.h"

#define MAX_COUNT 7

static uint8_t state = 0;
static uint8_t last_state = 0;
static uint8_t pmod_enc_btn_pressed = 0;
static int prev_main_menu_counter = 0xFF;
static int main_menu_counter = 0;

/**
 * @brief Reads the state of the PMOD ENC module every 1 ms.
 *
 * The PMOD_ENC_Task function is called when the Timer 0A module triggers a periodic interrupt
 * every 1 ms. It reads the state of the PMOD ENC module. It sets the pmod_enc_btn_pressed flag
 * if the button is pressed. In addition, it increments or decrements the global variable, main_menu_counter,
 * depending on the direction of the rotary encoder's rotation. This is used to indicate the active item on the LCD menu.
 *
 * @param None
 *
 * @return None
 */
void PMOD_ENC_Task(void);

/**
 * @brief Displays the main menu items on the LCD based on the value of main_menu_state.
 *
 * @param main_menu_state Determines the menu item that is currently selected
 *        based on the global variable, main_menu_counter.
 *
 * @return None
 */
void Display_Main_Menu(int main_menu_state);

/**
 * @brief Handles main menu selection whenever the PMOD ENC button is pressed.
 *
 * This function calls corresponding functions based on the menu item selected when
 * the button on the PMOD ENC module is pressed.
 *
 * @param None
 *
 * @return None
 */
void Process_Main_Menu_Selection(void);

void Heart_Animation_Sequence(void);


int main(void)
{
    // Initialize the SysTick timer used to provide blocking delay functions
    SysTick_Delay_Init();
    
    // Initialize the 16x2 LCD on the EduBase Board
    EduBase_LCD_Init();
    
		// Create custom characters on the LCD
		EduBase_LCD_Create_Custom_Character(HEART_SHAPE_LOCATION, heart_shape);
		EduBase_LCD_Create_Custom_Character(RIGHT_ARROW_LOCATION, right_arrow);
	
		// Initialize the LEDs on the EduBase board (Port B)
		EduBase_LEDs_Init();
	
		// Initialize the PMOD ENC (Rotary Encoder) module
		PMOD_ENC_Init();
	
		// Initialize Timer 0A to generate periodic interrupts every 1ms
		// and read the state of the PMOD ENC module
		Timer_0A_Interrupt_Init(&PMOD_ENC_Task);
	
		// Read the state of the PMOD ENC Module and assign the value to last_state
		last_state = PMOD_ENC_Get_State();
	
	while(1)
	{
    if (prev_main_menu_counter != main_menu_counter)
    {
        EduBase_LCD_Clear_Display();
				Display_Main_Menu(main_menu_counter);
        prev_main_menu_counter = main_menu_counter;
    }

		Process_Main_Menu_Selection();
    SysTick_Delay1ms(100);
	}	
}

void PMOD_ENC_Task(void)
{
    state = PMOD_ENC_Get_State();
    
    if (PMOD_ENC_Button_Read(state) && !PMOD_ENC_Button_Read(last_state))
    {
        pmod_enc_btn_pressed = 1;
    }
    
    if (main_menu_counter < 0)
    {
        main_menu_counter = 0;
    }
    else if (main_menu_counter > MAX_COUNT)
    {
        main_menu_counter = MAX_COUNT;
    }
    else
    {
        main_menu_counter = main_menu_counter + PMOD_ENC_Get_Rotation(state, last_state);
    }
    
    last_state = state;
}
		
		
		
		
void Display_Main_Menu(int main_menu_state)
{
	  switch(main_menu_state)
    {
        case 0x00:
        {
            EduBase_LCD_Set_Cursor(0, 0);
            EduBase_LCD_Send_Data(0x03);
            EduBase_LCD_Set_Cursor(1, 0);
            EduBase_LCD_Display_String("TURN OFF LEDS");
            EduBase_LCD_Set_Cursor(1, 1);
            EduBase_LCD_Display_String("TURN ON LEDS");
            break;
        }
        
        case 0x01:
        {
            EduBase_LCD_Set_Cursor(0, 1);
            EduBase_LCD_Send_Data(0x03);
            EduBase_LCD_Set_Cursor(1, 0);
            EduBase_LCD_Display_String("TURN OFF LEDS");
            EduBase_LCD_Set_Cursor(1, 1);
            EduBase_LCD_Display_String("TURN ON LEDS");
            break;
        }
        
        case 0x02:
        {
            EduBase_LCD_Set_Cursor(0, 0);
            EduBase_LCD_Send_Data(0x03);
            EduBase_LCD_Set_Cursor(1, 0);
            EduBase_LCD_Display_String("TURN ON LEDS");
            EduBase_LCD_Set_Cursor(1, 1);
					EduBase_LCD_Display_String("FLASH LEDS");
            break;
        }
        
        case 0x03:
        {
            EduBase_LCD_Set_Cursor(0, 1);
            EduBase_LCD_Send_Data(0x03);
            EduBase_LCD_Set_Cursor(1, 0);
            EduBase_LCD_Display_String("TURN ON LEDS");
            EduBase_LCD_Set_Cursor(1, 1);
            EduBase_LCD_Display_String("FLASH LEDS");
            break;
        }
				
				case 0x04:
        {
            EduBase_LCD_Set_Cursor(0, 0);
            EduBase_LCD_Send_Data(0x03);
            EduBase_LCD_Set_Cursor(1, 0);
            EduBase_LCD_Display_String("FLASH LEDS");
            EduBase_LCD_Set_Cursor(1, 1);
            EduBase_LCD_Display_String("HEART SEQUENCE");
            break;
        }
				
				case 0x05:
        {
            EduBase_LCD_Set_Cursor(0, 1);
            EduBase_LCD_Send_Data(0x03);
            EduBase_LCD_Set_Cursor(1, 0);
            EduBase_LCD_Display_String("FLASH LEDS");
            EduBase_LCD_Set_Cursor(1, 1);
            EduBase_LCD_Display_String("HEART SEQUENCE");
            break;
        }
				
			  case 0x06:
        {
            EduBase_LCD_Set_Cursor(0, 0);
            EduBase_LCD_Send_Data(0x03);
            EduBase_LCD_Set_Cursor(1, 0);
            EduBase_LCD_Display_String("HEART SEQUENCE");
            EduBase_LCD_Set_Cursor(1, 1);
            EduBase_LCD_Display_String("DISPLAY INFO");
            break;
        }
				
			  case 0x07:
        {
            EduBase_LCD_Set_Cursor(0, 1);
            EduBase_LCD_Send_Data(0x03);
            EduBase_LCD_Set_Cursor(1, 0);
            EduBase_LCD_Display_String("HEART SEQUENCE");
            EduBase_LCD_Set_Cursor(1, 1);
            EduBase_LCD_Display_String("DISPLAY INFO");
            break;
        }
    }
}

void Process_Main_Menu_Selection(void)
{
	    if (pmod_enc_btn_pressed == 1)
    {
        pmod_enc_btn_pressed = 0;
        prev_main_menu_counter = 0xFF;
        
        switch(main_menu_counter)
        {
            case 0x00:
            {
                EduBase_LEDs_Output(EDUBASE_LED_ALL_OFF);
                break;
            }
            
            case 0x01:
            {
                EduBase_LEDs_Output(EDUBASE_LED_ALL_ON);
                break;
            }
            
            case 0x02:
            {
                EduBase_LEDs_Output(EDUBASE_LED_ALL_ON);
                break;
            }
            
            case 0x03:
						{
								for (int i = 0; i < 5; i++) 
									{
										EduBase_LEDs_Output(EDUBASE_LED_ALL_ON);
										SysTick_Delay1ms(500); // Adjust delay as needed
										EduBase_LEDs_Output(EDUBASE_LED_ALL_OFF);
										SysTick_Delay1ms(500);
									}
									
								break;
						}
						
						case 0x04:
						{
								for (int i = 0; i < 5; i++) 
									{
										EduBase_LEDs_Output(EDUBASE_LED_ALL_ON);
										SysTick_Delay1ms(500); // Adjust delay as needed
										EduBase_LEDs_Output(EDUBASE_LED_ALL_OFF);
										SysTick_Delay1ms(500);
									}
									
								break;
						}
						
						case 0x05:
            {
							Heart_Animation_Sequence(); // Call the animation function
							break;
            }
						
						case 0x06:
            {
							Heart_Animation_Sequence(); // Call the animation function
							break;
            }
						
						case 0x07:
            {
							EduBase_LCD_Enable_Display();
							EduBase_LCD_Clear_Display();
							
							EduBase_LCD_Set_Cursor(0, 0);
							EduBase_LCD_Display_String("ECE 425");
							
							EduBase_LCD_Set_Cursor(0, 1);
							EduBase_LCD_Display_String("MICROPROCESSOR");
							
							SysTick_Delay1ms(3000);
							EduBase_LCD_Clear_Display();
							break;
            }
        }
    }
}

void Heart_Animation_Sequence(void)
{
    EduBase_LCD_Clear_Display(); // Clear the display at the beginning

    EduBase_LCD_Set_Cursor(0, 0);
    EduBase_LCD_Send_Data(0x04); // Display the heart character (use the correct code)
		SysTick_Delay1ms(300);
    // Scroll the display to the right
    for (int i = 0; i < 15; i++) // Scroll 15 times for a 16-column display
    {
        EduBase_LCD_Scroll_Display_Right();
        SysTick_Delay1ms(300); // Adjust the delay as needed for the animation speed
    }
		
		SysTick_Delay1ms(300);
		EduBase_LCD_Clear_Display(); 
    EduBase_LCD_Set_Cursor(15,1);
    EduBase_LCD_Send_Data(0x04); // Display the heart character (use the correct code)
		SysTick_Delay1ms(300);
    for (int i = 0; i < 16; i++)// Scroll 15 times for a 16-column display
    {
        EduBase_LCD_Scroll_Display_Left();
        SysTick_Delay1ms(300); // Adjust the delay as needed for the animation speed
    }

    EduBase_LCD_Clear_Display();
}
