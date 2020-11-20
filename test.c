/* 
 * File:   test.c
 * Author: admuro
 *
 * Created on November 19, 2020, 2:12 PM
 */



// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 20000000
#define TMR2PRESCALE 4

#include <xc.h>

long PWM_freq = 5000;

PWM_Init()
{
    //inicializa los PWM
    PR2 = (_XTAL_FREQ/(PWM_freq*4*TMR2PRESCALE)) - 1; //Frecuencia
    CCP1M3 = 1; CCP1M2 = 1;  //Config CCP1
    CCP2M3 = 1; CCP2M2 = 1; //Config CCP1
    T2CKPS0 = 1;T2CKPS1 = 0; TMR2ON = 1; //Config Timer
    TRISC2 = 0; TRISC1 = 0;//Salidas
}

PWM1_CT(unsigned int ciclotrabajo)
{
      if(ciclotrabajo<1023)
  {

    duty = ((float)ciclotrabajo/1023)*(_XTAL_FREQ/(PWM_freq*TMR2PRESCALE)); 
    int CCP1X = ciclotrabajo & 1; //Guarda bit 1
    int CCP1Y = ciclotrabajo & 2; //Guarda bit 0
    CCPR1L = ciclotrabajo>>2;//Guarda los otros 8 bits
  }
}

PWM2_CT(unsigned int ciclotrabajo)
{
    //igual a PWM2_CT
      if(ciclotrabajo<1023)
  {

    duty = ((float)ciclotrabajo/1023)*(_XTAL_FREQ/(PWM_freq*TMR2PRESCALE)); // On reducing //duty = (((float)duty/1023)*(1/PWM_freq)) / ((1/_XTAL_FREQ) * TMR2PRESCALE);
    int CCP2X = ciclotrabajo & 1; //Store the 1st bit
    int CCP2Y = ciclotrabajo & 2; //Store the 0th bit
    CCPR2L = ciclotrabajo>>2;// Store the remining 8 bit
  }
}

void ADC_Init()
{
  ADCON0 = 0b01000001; //ADC ON and Fosc/16 is selected
  ADCON1 = 0b11000000; // Internal reference voltage is selected
}



unsigned int ADC_Read(unsigned char channel)
{
  ADCON0 &= 0x11000101; //Clearing the Channel Selection Bits
  ADCON0 |= channel<<3; //Setting the required Bits
  __delay_ms(2); //Acquisition time to charge hold capacitor
  GO_nDONE = 1; //Initializes A/D Conversion
  while(GO_nDONE); //Wait for A/D Conversion to complete
  return ((ADRESH<<8)+ADRESL); //Returns Result
}

void main()
{
    int adc_value;
    int fixed_value;
    //determina los limites dentro de los cuales termina el juego
    int minimo =fixed_value-51;
    int maximo =fixed_value+51;
  TRISC = 0x00; //Salida
  TRISA = 0xFF; //Entrada
  TRISD = 0x00;
  ADC_Init(); //inicia ADC
  PWM_Init();  //inicia PWM

  do
  {
    adc_value = ADC_Read(4); //lectura de ADC
    PWM1_CT(adc_value); //Genera PWM con base en el valor del ADC
    PWM2_CT(fixed_value);//Genera PWM con valor fijo
    
      __delay_ms(50); 
      
  }while(1); //Infinite Loop
  
}
