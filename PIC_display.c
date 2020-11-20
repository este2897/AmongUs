/*
 * File:   PIC_display.c
 * Author:
 *
 * Created on 19 de noviembre de 2020, 10:11 PM
 */


#include <xc.h>
#include "configuracion.h"
#include "USART_libreria.h"




void main(void) {

    USART_Init(9600);
    ////////////////////////////////////////////////////////////////////////////
    // SETUP DE PUERTOS
    ////////////////////////////////////////////////////////////////////////////
    // Se asignan todos los Puertos AN/RA en su versi�n Digital
    // Esto ya que INT0, INT1 e INT2 comparten patilla con AN12, AN10 y AN8
    // respectivamente
    ADCON1 = 0b00001111;
    TRISB = 0b00000000; // RB0 - RB7 son outputs

    // Asigna Estado Inicial de luces apagadas
    LATBbits.LB2 = 0;
    LATBbits.LB3 = 0;
    LATBbits.LB4 = 0;
    LATBbits.LB5 = 0;


    ////////////////////////////////////////////////////////////////////////////
    // SETUP GENERAL DE INTERRUPCIONES
    ////////////////////////////////////////////////////////////////////////////
    RCONbits.IPEN = 1;         // Enable Interrupt Priority
    INTCONbits.GIEH = 1;       // Enable de Interrupciones no Enmascaradas
    INTCONbits.GIEL = 0;       // Disable a Interrupciones de Baja Prioridad


    // Setup referente a INT0
    // INT0 señala que para encender secuencia y habilitar controles de juego
    INTCONbits.INT0IF   = 0;   // Se limpia la bandera de INT0
    INTCON2bits.INTEDG0 = 1;   // INT0 en flanco positivo
    INTCONbits.INT0IE   = 1;   // Enable de INT0
    INTCONbits.INT0IF = USART_Rx();

    }

// Interrupcion de Alta Prioridad
void __interrupt() isr(void){
    // Handler particular para la interrupci�n INT0
    if (INTCONbits.INT0IF == 1){
        // Enciende luces
        LATBbits.LB2 = 1;
        LATBbits.LB3 = 0;
        LATBbits.LB4 = 1;
        LATBbits.LB5 = 0;

        PORTA = USART_Rx (); //se copian todos los RAs

        //y se comparan los que corresponden a elección de usuario con luces
        if (PORTAbits.RA5 == PORTBbits.RB2){
          if (PORTAbits.RA1 == PORTBbits.RB3){
            if (PORTAbits.RA2 == PORTBbits.RB4){
              if (PORTAbits.RA3 == PORTBbits.RB5){
                // secuencia correcta, se apagan las luces
                LATBbits.LB2 = 0;
                LATBbits.LB3 = 0;
                LATBbits.LB4 = 0;
                LATBbits.LB5 = 0;

                // se envia señal para anunciar que la tarea se completo
                USART_Tx(1);

                // Se Limpia INT0IF
                INTCONbits.INT0IF = 0;
              }
            }
          }
        }
        //se limpia RAs
        PORTA = 0x00;
    }
}
