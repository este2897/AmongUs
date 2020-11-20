/* 
 * File:   main.c
 * Author: admuro
 *
 * Created on November 19, 2020, 11:16 PM
 */

#include <xc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pic18f4550.h>
//#include "configpwm.h"


#define vref 5.00		/* Reference Voltage is 5V*/
void ADCInit();
int ADCRead(int);





void ADCInit()
{    
    TRISA = 0x00000001;		/*Set as input port*/
    ADCON1 = 0x0e;  //AN0 entrada analogica 
    ADCON2 = 0x92;  //configuracion ADC
    ADRESH=0;  		//limpiar registros
    ADRESL=0;   
}

void PWMInit(){
    OSCCON = 0x76;//oscilador
    TRISC1 = 0;//CCP2 como salida del PWM
    TRISC2 = 0;//CCP1 como salida del PWM
    PR2 = 199;
}

int ADCRead(int canal)
{
    unsigned int ADCin;
    ADCON0 =(ADCON0 & 0b11000011)|((canal<<2) & 0b00111100); //selecciona canal ADC
    ADCON0 |= ((1<<ADON)|(1<<GO));//Inicia ADC
    while(ADCON0bits.GO_nDONE==1);//Espera que termine ADC
    //ADCin = (ADRESH<<8+ADRESL);//Escribe el valor de ADC
    ADCin = (ADRESH*256) | (ADRESL);
    return(ADCin);
}


void main(void)
{    
    int n=1;
    char data[10];    
    int ADCin;  
    int cicloPWM;
    int minimo;
    int maximo;
    cicloPWM=80; //valor ciclo de trabajo PWM
    minimo=cicloPWM-2;
    maximo=cicloPWM+2;
    
    OSCCON=0x72;//frecuencia oscilador
    ADCInit();//inicia ADC
    PWMInit();

    while(n!=0)
    {        
        ADCin=ADCRead(0);   
        CCP1CON = 0x0C; //config PWM1
        CCPR1L = cicloPWM;//valor fijo del ciclo de trabajo del PWM
        CCP2CON = 0x0C; //config PWM1
        CCPR2L = ADCin;//valor variable ciclo de trabajo
    
        //configs timer 2
        T2CON = 0;
        TMR2 = 0;		
        TMR2ON = 1;

        if((ADCin>minimo)&&(ADCin<maximo)){
            n=0;
        }
    } 
}