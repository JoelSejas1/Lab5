
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/adc.h"

uint32_t t;
uint32_t adc;

int main(void)
{
    t = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    GPIOPinConfigure(GPIO_PF1_M0PWM1);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);

    GPIOPinTypeADC(GPIO_PORTK_BASE, GPIO_PIN_3);

    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0,
                             ADC_CTL_CH19 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);

    SysCtlPWMClockSet(SYSCTL_PWMDIV_64); 

    uint32_t pwmClock = t / 64;
    uint32_t pwmFreq = 1000; 
    uint32_t load = (pwmClock / pwmFreq) - 1;

    PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, load);

    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, load / 2); 

    PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);

    while(1)
    {
        ADCProcessorTrigger(ADC0_BASE, 3);
        while(!ADCIntStatus(ADC0_BASE, 3, false));
        ADCIntClear(ADC0_BASE, 3);

        ADCSequenceDataGet(ADC0_BASE, 3, &adc);
        uint32_t duty = (adc * load) / 4095;

        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, duty);
        SysCtlDelay(t / 6);
    }
}