/*
 * File:   programa_principal.c
 * Author: EstebanS, Montero
 *
 * Created on 19 de noviembre de 2020, 09:57 PM
 */


#include <xc.h>
#include "configuracion.h"
#include "USART_libreria.h"

void ADCInit()
{
    TRISA = 0x00000001;		/*Set as input port*/
    ADCON1 = 0x0e;  //AN0 entrada analogica
    ADCON2 = 0x92;  //configuracion ADC
    ADRESH=0;  		//limpiar registros
    ADRESL=0;
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


void main(void) {

    USART_Init(9600);
    // SETUP PRINCIPAL DE PUERTOS
    // Se asigna RA0 analógico, el resto digital
    ADCON1 = 0b00001110;

    // Selecci�n de Direcci�n de Data
    TRISA = 0b00011111; // RA0 - RA5 son inputs
    TRISB = 0b00000011;

    // While True Principal
    while (1){

   /*      // Check de la Entrada a juego 1: simon dice
        if (PORTAbits.RA4 == 1){
            while(PORTAbits.RA4 == 1){
            //Para cerrar este while: LATAbits.LA4 = 0;
            }
        } */

         // Check de la Entrada a juego 2: apaga luces
        if (PORTBbits.RB0 == 1){
            while(PORTBbits.RB0 == 1){
                USART_Tx(PORTBbits.RB0); //señal para iniciar display juego

                //envia PORTA(lo que el usuario ingresa) al PIC_display
                USART_Tx(PORTA);

                PORTBbits.RB0 =  ∼USART_Rx() //ya se completo la tarea
            }
        }
        // Check del juego 3: PWM
        if (PORTBbits.RB1 == 1){
            int n=0;
            int ADCin;
            OSCCON=0x72;//frecuencia oscilador
            ADCInit();//inicia ADC
            USART_Tx(0xFF);
            while(PORTBbits.RB1 == 1){
                ADCin=ADCRead(0);
                USART_Tx(ADCin);
                n=USART_Rx();
                if(n==1){
                    LATAbits.LA4 = 0;
                }
            }
        }
    }
}
