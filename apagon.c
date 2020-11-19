#include <xc.h>

#include<pic18f4550.h>

// Como el juego empieza se activa la interrupción de alta prioridad
// esto enciende las de baja prioridad.
// Baja prioridad revisa que las entradas coincidan con la salida.

void main(void) {
    ////////////////////////////////////////////////////////////////////////////
    // SETUP DE PUERTOS
    ////////////////////////////////////////////////////////////////////////////
    // Se asignan todos los Puertos AN/RA en su versi�n Digital
    // Esto ya que INT0, INT1 e INT2 comparten patilla con AN12, AN10 y AN8
    // respectivamente
    ADCON1 = 0b00001111;

    TRISA = 0b00111111; // RA0 - RA6 son inputs
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

    // Setup referente a INT1
    // INT1 usuario termino secuencia y revisa que sea correcta
    INTCON3bits.INT1IF  = 0;   // Se limpia la bandera de INT1
    INTCON2bits.INTEDG1 = 1;   // INT1 con flanco positivo
    INTCON3bits.INT1IP  = 1;   // Alta prioridad a la INT1
    INTCON3bits.INT1IE  = 1;   // Enable de INT1

    // Ciclo Principal
    while(1){}

    return;
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

        // Enable a interrupciones de baja prioridad
        INTCONbits.GIEL = 1;

    }
}

// Interrupci�n de Baja Prioridad
void __interrupt(low_priority) Lowisr(void){
    // Handler particular para la interrupci�n INT1
    if (INTCON3bits.INT1IF == 1){

        if (PORTAbits.RA0 == PORTBbits.RB2){
          if (PORTAbits.RA1 == PORTBbits.RB3){
            if (PORTAbits.RA2 == PORTBbits.RB4){
              if (PORTAbits.RA3 == PORTBbits.RB5){

                // secuencia correcta, se apagan las luces
                // y disable a interrupciones de baja prioridad
                LATBbits.LB2 = 0;
                LATBbits.LB3 = 0;
                LATBbits.LB4 = 0;
                LATBbits.LB5 = 0;
                INTCONbits.GIEL = 0;

                // Se Limpia INT1IF
                INTCON3bits.INT1IF = 0;

                // Se Limpia INT0IF
                INTCONbits.INT0IF = 0;
              }
            }
          }
        }
    }
}
