/*
 * File:   Lab04.c
 * Author: Vincenzo
 *
 * Created on February 12, 2020, 12:50 PM
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include <xc.h>
//FRECUENCIA PARA TRABAJAR
#define _XTAL_FREQ 4000000  
#include "lab4s.h"


// Configuración de los puertos
void config(void);
void __interrupt() isr(void);


void config(void) {
    PORTA = 0;
    TRISA = 0b00000011; 
    
    PORTB = 0;
    TRISB = 0;
    
    PORTC = 0;
    TRISC = 0b00011000;
    
    PORTD = 0;
    TRISD = 0;
         
    ANSEL = 0b00000011;
    ANSELH = 0;
   
    ADCON1 = 0;
    
   INTCONbits.GIE = 1;
   INTCONbits.PEIE = 1;
   PIR1bits.SSPIF = 0;
   PIE1bits.SSPIE = 1;
   
   ADCON0bits.ADON = 1;
   __delay_ms(20);
   ADCON0bits.GO_DONE = 1;
   
   spiInit(SPI_SLAVE_SS_DIS, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}




int spiselect;   //SPI
int op2;
int op1;



void __interrupt() isr(void){
   if(PIR1bits.SSPIF == 1){   //revisar si la bandera esta en 1
        spiselect = spiRead();  //va a guardar lo que manda el master
        if (spiselect == 1 ){
            spiWrite(op1);
        }
        else{
            spiWrite(op2);
        }
        SSPIF = 0;
        
    }
}

void main(void) {
    config();

    while(1){
        ADCON0bits.ADCS1 = 1;   
        ADCON0bits.ADCS0 = 0;   
        
        ADCON0bits.CHS3 = 0;   //Canal analógico AN0
        ADCON0bits.CHS2 = 0;   
        ADCON0bits.CHS1 = 0;   
        ADCON0bits.CHS0 = 0;   
        
        PIR1bits.ADIF = 0;
        ADCON0bits.GO = 1;       
        
        op1 = ADRESH;
        
        __delay_ms(5);
                
        ADCON0bits.CHS3 = 0;   //Canal analógica AN1
        ADCON0bits.CHS2 = 0;   
        ADCON0bits.CHS1 = 0;   
        ADCON0bits.CHS0 = 1;   
        
        PIR1bits.ADIF = 0;
        ADCON0bits.GO = 1;     
        
        op2 = ADRESH;
        
        __delay_ms(5);
    }
    return;
}
