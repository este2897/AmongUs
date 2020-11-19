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
		if(simon[i]!=lectura){
			error=1;
		}
		i++;
	}
	while(error==1 && !(bit_test(PORTB,4))){
		bit_set(PORTD,0);
		bit_set(PORTD,1);
		bit_set(PORTD,2);
		bit_set(PORTD,3);
	}

	if(error==1){
		LATD=0X00;
		cont=0;
		error=0;
		conteo=0;
		delay_ms(1000);
		i=0;
	}

	else{
		error=0;
		i=0;
		cont++;
		conteo++;
	}
	cont++;
	conteo++;
}

void configuracion(){
	setup_wdt(WDT_OFF);
	setup_comparator(NC_NC_NC_NC);
	setup_spi(FALSE);
	setup_low_volt_detect(FALSE);
	setup_oscillator(False);

	setup_adc_ports(NO_ANALOGS|VSS_ADD);
	setup_adc(ADC_OFF);
	setup_vref(FALSE);

	set_rtcc((int16)65067);
	setup_timer_0(RTCC_INTERNAL|RTCC_DIV_256);
	enable_interrupts(INT_RTCC);
	enable_interrupts(GLOBAL);

	setup_timer_1(T1_DISABLED);

	setup_timer_2(T2_DISABLED,0,1);

	setup_timer_3(T3_DISABLED|T3_DIV_BY_1);

	TRISB=0X1F;
	LATB=0X00;
	PORTB=0X00;

	TRISC=0X00;
	LATC=0X00;
	PORTC=0X00;

	TRISA=0XF0;
	LATA=0X00;
	PORTA=0X00;

	TRISD=0X00;
	LATD=0X00;
	PORTD=0X00;

	TRISE=0XFF;
	LATE=0X00;
	PORTE=0X00;
}

void imprimir(){
	
	for(j=0; j<=cont; j++){
		PORTD=simon[j];
		delay_ms(250);
		PORTD=0X00;
		delay_ms(250);
	}
}

void display(){

	if(cdisplay==0){
		OFF(UNI);
		ON(DEC);
		estritura=conteo/10l
		escritura=(escritura*16)|(PORTD & 0X0F);
		LATD=(escritura);
	}

	if(cdisplay==1){
		ON(UNI);
		OFF(DEC);
		escritura=conteo % 10;
		escritura=(escritura*16)|(PORTD & 0X0F);
		LATD=(escritura);
	}

	cdisplay++;

	if(cdisplay==2){
		cdisplay=0;
	}
}
