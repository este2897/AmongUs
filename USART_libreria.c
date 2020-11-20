#include <xc.h>
#include "Configuracion.h"
#include "USART_libreria.h"

void USART_Init(long BAUD){
    //Configuración de los pines
    TRISCbits.RC6 = 0;      //RC6 = Tx -> Salida
    TRISCbits.RC7 = 1;      //RC7 = Rx -> Entrad

    //Baudios
    SPBRG = (unsigned char)(((_XTAL_FREQ/BAUD)/64)-1);

    //Configuración
    TXSTAbits.BRGH = 0;     //Low Speed
    TXSTAbits.SYNC = 0;     //Asincrono
    RCSTAbits.SPEN = 1;     //Habilitar Tx y Rx
    TXSTAbits.TXIE = 1;     //Habilita interrupciones

    //Transmisión
    TXSTAbits.TX9 = 0;      //8 bits
    TXSTAbits.TXEN = 1;     //Activamos transmisión

    //Recepción
    RCSTAbits.RC9 = 0;      //8 bits
    RCSTAbits.CREN = 1;     //Activamos recepción constante
}

void USART_Tx(char data){
    TXREG = data;
}

char USART_Rx(){
    return RCREG;
}
