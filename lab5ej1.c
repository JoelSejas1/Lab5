#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"

volatile uint32_t adc;
uint32_t t;

void Timer0A_Handler(void) {

    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);


    if(GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_1)) {
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0);
    } else {
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
    }
}

int main(void) {
    t = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | 
                                        SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK); 
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);  
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); 

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));

    GPIOPinTypeADC(GPIO_PORTK_BASE, GPIO_PIN_3);

    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);

    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH19 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE, 3);

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    

    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntEnable(INT_TIMER0A);
    IntMasterEnable(); 
    
    TimerEnable(TIMER0_BASE, TIMER_A);

    while(1) {
        ADCProcessorTrigger(ADC0_BASE, 3);

        while(!ADCIntStatus(ADC0_BASE, 3, false));
        ADCIntClear(ADC0_BASE, 3);
        ADCSequenceDataGet(ADC0_BASE, 3, (uint32_t *)&adc);

        uint32_t ui32Period = (t / (adc + 10)) * 50; 
        
        TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period);
                SysCtlDelay(t / 100); 
    }
}