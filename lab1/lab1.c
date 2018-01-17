/*

* Author: Texas Instruments

* Editted by: Parin Chheda
          ERTS Lab, CSE Department, IIT Bombay

* Description: This code structure will assist you in completing Lab 1
* Filename: lab-1.c

* Functions: setup(), config(), main()


*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include <time.h>


/*

* Function Name: setup()

* Input: none

* Output: none

* Description: Set crystal frequency,enable GPIO Peripherals and unlock Port F pin 0 (PF0)

* Example Call: setup();

*/
void setup(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //unlock PF0 based on requirement

}

/*

* Function Name: pin_config()

* Input: none

* Output: none

* Description: Set Port F Pin 1, Pin 2, Pin 3 as output. On this pin Red, Blue and Green LEDs are connected.
               Set Port F Pin 0 and 4 as input, enable pull up on both these pins.

* Example Call: pin_config();

*/

void pin_config(void)
{
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE+ GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE+ GPIO_O_LOCK)= 0;
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);

}
int sw2_status = 0;

int lab1_2(void){
    static int sw2_status;
    bool switched_on = false;
    while(1){
        if (!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)){
            switched_on=true;
            if(switched_on){
                sw2_status+=1;
                switched_on=false;
            }
        }
    }
}

int lab1_1(void)
{
    bool switched_on = false;
    uint8_t led_val= GPIO_PIN_1;

    while(1)
    {
        if (!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
        {// Turn on the LED
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,led_val );
            switched_on=true;
        }else{
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
            if(switched_on==true){
                if(led_val==GPIO_PIN_1){
                    led_val=GPIO_PIN_2;
                }else if(led_val==GPIO_PIN_2){
                    led_val = GPIO_PIN_3;
                }else{
                    led_val = GPIO_PIN_1;
                }
                switched_on = false;
            }
        }

    }
}



int lab1_3(void)
{
    int delay  = 1;
    bool switched_on = false,sw_up = true,sw4_up=true;
    uint8_t led_val= GPIO_PIN_1;
    while(1)
    {
        if (!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
        {// Turn on the LED
            if(sw4_up){
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,led_val );
                switched_on=true;
                SysCtlDelay(delay*5000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
                if(switched_on==true){
                    if(led_val==GPIO_PIN_1){
                        led_val=GPIO_PIN_2;
                    }else if(led_val==GPIO_PIN_2){
                        led_val = GPIO_PIN_3;
                    }else{
                        led_val = GPIO_PIN_1;
                    }
                    switched_on = false;
                }
                sw4_up=false;
            }
        }else{
            sw4_up=true;
        }
        if (!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
        {
            if(sw_up){
                delay = delay*2;
                if(delay > 4){
                    delay = 1;
                }
                sw_up=false;
            }
        }else{
            sw_up = true;
        }
    }
}


int main(void){
    setup();
    pin_config();
//    lab1_1();
//    lab1_2();
    lab1_3();
}
