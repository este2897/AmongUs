#include <pic18f4550.h>
#include <xc.h>

#FUSES NOWDT
#FUSES WDT128
#FUSES HSPLL
#FUSES NOPROTECT
#FUSES BROWNOUT
#FUSES BORV21
#FUSES PUT
#FUSES NOCPD
#FUSES STVREN
#FUSES NODEBUG
#FUSES NOLVP
#FUSES NOWRT
#FUSES NOWRTD
#FUSES IESO
#FUSES FCMEN
#FUSES NOPBADEN
#FUSES NOWRTC
#FUSES NOWRTB
#FUSES NOEBTR
#FUSES NOEBTRB
#FUSES NOCPB
#FUSES MCLR
#FUSES NOLPT1OSC
#FUSES NOXINST
#FUSES PLL5
#FUSES CPUDIV1

#use delay(clock=48000000)

#define DEC Pin_a1
#define UNI Pin_a0

#define ON output_high
#define OFF output_low

#byte TRISA=0XF92
#byte PORTA=0XF80
#byte LATA=0XF89
#byte ADCON1=0XFC1

#byte TRISB=0XF93
#byte PORTB=0XF81
#byte LATB=0XF8A

#byte TRISC=0XF94
#byte PORTC=0XF82
#byte LATC=0XF8B

#byte TRISD=0XF95
#byte PORTD=0XF83
#byte LATD=0XF8C

#byte STATUS=0XFD8


//


int8 simon[50];
int8 i=0, j=0, cont=0, aleatorio=1, conteo=0, escritura=0;
int8 lectura=0, cdisplay=0;
int1 error=0;

void display();
#INT_RTCC
void RTCC_isr(void){
	set_rtcc((int16)65067);
	pruebas++;
	display();
	pruebas=0;
}

void configuracion();
void imprimir();

void main(){
	configuracion();
	simon[i]=1;
	delay_ms(500);
	
	while (1){
		imprimir();

		while(i<cont && error==0){
			while((PORTB & 0X1F)==0X00){}
			delay_ms(10);
			if((PORTB & 0X1F)!=0X00){
				aleatorio=aleatorio*2;
				if(aleatorio>8){
					aleatorio=1;
				}
			}
		}
		delay_ms(100);

		if(simon[i]==lectura && i==cont){
			simon[i+1]=aleatorio;
		}
		
	}
}

