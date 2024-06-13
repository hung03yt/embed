#include "main.h"
#include "config_clock.h"
#include "i2c.h"
#include "i2c-lcd.h"
#include "mma8452.h"


#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_4
#define LED1_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_2

#define LED2_GPIO_Port GPIOA
#define BTN1_Pin GPIO_PIN_0
#define BTN1_GPIO_Port GPIOB
#define BTN2_Pin GPIO_PIN_1
#define BTN2_GPIO_Port GPIOB

#define LED_RUN_RED_INTERVAL	500		// ms
#define LED_RUN_GREEN_INTERVAL	1000 // ms
#define LCD_REFRESH_INTERVAL	200
typedef enum {
	STOP = 0,
	START = 1,
} state_e;

typedef enum {
	MODE_STATE = 0,
	MODE_AXIS = 1,
} mode_e;

/*--------------------------- global variable -----------------------------*/
state_e g_state = START;
mode_e g_mode = MODE_STATE;

uint8_t nga = 0;
float Compas_Value;

uint32_t ticksRed, ticksGreen;
uint32_t ticks_lcd;

static void gpio_init(void){
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // Enable IO port A Clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // Enable IO port B Clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // Enable IO port C Clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPDEN; // Enable IO port D Clock
	
	/* Config Led Pin PC13*/
	GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13); // clear bit
	GPIOC->CRH |= (GPIO_CRH_MODE13_1 | GPIO_CRH_MODE13_0); //output 50 MHz
	GPIOC->CRH |= GPIO_CRH_CNF13_0; // push-pull (CNF13[1:0] = 00)

	/* Config LED1 Pin PA2 */
	GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2); // Clear bit cho PA3
	GPIOA->CRL |= (GPIO_CRL_MODE2_1 | GPIO_CRL_MODE2_0); // Output mode, max speed 50 MHz
	GPIOA->CRL |= GPIO_CRL_CNF2_0; // General purpose output push-pull

	/* Config LED2 Pin PA4 */
	GPIOA->CRL &= ~(GPIO_CRL_MODE4 | GPIO_CRL_CNF4); // Clear bit cho PA4
	GPIOA->CRL |= (GPIO_CRL_MODE4_1 | GPIO_CRL_MODE4_0); // Output mode, max speed 50 MHz
	GPIOA->CRL |= GPIO_CRL_CNF4_0; // General purpose output push-pull
	
	 /* Config PB0 and PB1 as input pull-up */
	GPIOB->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0); // Clear bits for PB0
	GPIOB->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1); // Clear bits for PB1
	GPIOB->CRL |= (GPIO_CRL_CNF0_1 | GPIO_CRL_CNF1_1); // Input mode, pull-up/pull-down

	/* Enable pull-up for PB0 and PB1 */
	GPIOB->ODR |= (GPIO_ODR_ODR0 | GPIO_ODR_ODR1); // Set bits for pull-up
}

int16_t x_raw, y_raw, z_raw;
int16_t x, y, z;
int main(void)
{
  HAL_Init();

  SystemClock_Config();
  gpio_init();

	i2c_init();
//	i2c1_scan_bus();
	LED1_GPIO_Port->BSRR = LED1_Pin; // set LED1
	LED2_GPIO_Port->BSRR = LED2_Pin; // set LED1
	MMA8452Q_Init();
	lcd_init();
	HAL_Delay(100);
	
	lcd_goto_XY(1, 0);
	lcd_send_string("    Welcome!");
	lcd_goto_XY(2, 0);
	lcd_send_string(" to the system ");
	HAL_Delay(1500);
	LED_GPIO_Port->BSRR = LED_Pin;
	lcd_clear_display();

  while (1)
  {
		if (!(GPIOB->IDR & GPIO_IDR_IDR0)) { // Kiem tra nut nhan xem co duoc nhan hay khong?
			while(!(GPIOB->IDR & GPIO_IDR_IDR0));
			g_state = !g_state;
			if(g_state == STOP){
				lcd_clear_display();
				lcd_goto_XY(1, 0);
				lcd_send_string(" Nhan button");
				lcd_goto_XY(2, 0);
				lcd_send_string(" de bat dau  ");
			}
			else{
				nga = 0;
				lcd_clear_display();
			}
		}
		else if(!(GPIOB->IDR & GPIO_IDR_IDR1)){
			while(!(GPIOB->IDR & GPIO_IDR_IDR1));
			lcd_clear_display();
			HAL_Delay(1000);
			HAL_NVIC_SystemReset();
		}
		
		switch(g_state){
			case START:
			{
				if(uwTick - ticks_lcd >= LCD_REFRESH_INTERVAL){
						MMA8452Q_Read_Accel(&x_raw, &y_raw, &z_raw);

								if(z_raw >= 500 && z_raw < 1200){
									nga = 0;
									lcd_goto_XY(1, 0);
									lcd_send_string(" 0 - Binh thuong ", Compas_Value);
								}
								else{
									nga = 1;
									lcd_goto_XY(1, 0);
									lcd_send_string(" 1 - Nga         ", Compas_Value);
								}


//						lcd_goto_XY(1, 0);
//						lcd_send_string(" X-----Y-----Z ", Compas_Value);
//						lcd_goto_XY(2, 0);
//						lcd_send_string("%d  %d  %d     ", x_raw, y_raw, z_raw);
					
						ticks_lcd = uwTick;
				}

				if(nga == 1){
					if(uwTick - ticksGreen >= LED_RUN_RED_INTERVAL){
						uint32_t odr = LED2_GPIO_Port->ODR;
						LED2_GPIO_Port->BSRR = ((odr & LED2_Pin) << 16) | (~odr & LED2_Pin); // Nhap nhay led red chu ki 500ms
						ticksGreen = uwTick;
					}
				}
				else{
					LED2_GPIO_Port->BSRR = LED2_Pin; // tat led Red	
				}
				// Blink LED
				if(uwTick - ticksRed >= LED_RUN_GREEN_INTERVAL){
					uint32_t odr = LED1_GPIO_Port->ODR;
					LED1_GPIO_Port->BSRR = ((odr & LED1_Pin) << 16) | (~odr & LED1_Pin); // Nhap nhay led green chu ki 1000ms
					ticksRed = uwTick;
				}
				break;
			}
			case STOP:
			{
					uint32_t odr = LED2_GPIO_Port->ODR;
					LED1_GPIO_Port->BSRR = LED1_Pin; // tat led green
					LED2_GPIO_Port->BSRR = LED2_Pin; // tat led Red
				break;
			}
			default: break;
		}	
  }
}

void Error_Handler(void)
{

  __disable_irq();
  while (1)
  {
  }
}
