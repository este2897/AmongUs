/*
 * File:   programa_principal.c
 * Author: EstebanS, Montero
 *
 * Created on 19 de noviembre de 2020, 09:57 PM
 */


#include <xc.h>
#include "configuracion.h"
#include "USART_libreria.h"

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
            correcto = 0;
            while(correcto == 0){
                USART_Tx(PORTBbits.RB0); //señal para iniciar display juego

                //envia PORTA(lo que el usuario ingresa) al PIC_display
                USART_Tx(PORTA);

                correcto = USART_Rx() //ya se completo la tarea
            }
        }

        // Check del juego 3: PWM
        if (PORTBbits.RB1 == 1){
            while(PORTBbits.RB1 == 1){
            //Para cerrar este while: LATAbits.LA4 = 0;

            }
        }
    }
}
