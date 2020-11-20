/* 
 * File:   PWMdisp.c
 * Author: admuro
 *
 * Created on November 20, 2020, 9:09 AM
 */

#include <xc.h>
#include "config.h"
#include "USART_lib.h"
#define vref 5.00

void PWMInit(){
    OSCCON = 0x76;//oscilador
    TRISC1 = 0;//CCP2 como salida del PWM
    TRISC2 = 0;//CCP1 como salida del PWM
    PR2 = 199;
}


void main(void) {
    USART_Init(9600);
    PWMInit();
    int cicloPWM;    
    int cicloPWM2;
    int minimo;
    int maximo;
    cicloPWM=80; //valor ciclo de trabajo PWM
    minimo=cicloPWM-2;
    maximo=cicloPWM+2;
    OSCCON=0x72;//frecuencia oscilador
    cicloPWM2=USART_Rx();   
    CCP1CON = 0x0C; //config PWM1
    CCPR1L = cicloPWM;//valor fijo del ciclo de trabajo del PWM
    CCP2CON = 0x0C; //config PWM1
    CCPR2L = cicloPWM2;//valor variable ciclo de trabajo
    //configs timer 2
    T2CON = 0;
    TMR2 = 0;		
    TMR2ON = 1;
    if((cicloPWM>minimo)&&(cicloPWM<maximo)){
        USART_Tx(1);
    }

}

