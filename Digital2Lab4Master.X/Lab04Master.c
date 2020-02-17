/*
 * File:   Lab04Master.c
 * Author: Vincenzo 
 *
 * Created on February 12, 2020, 12:54 PM
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
#define _XTAL_FREQ 4000000
#include "lab4s.h"
void config(void);


void config(void) {
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    
    TRISA = 0;
    TRISB = 0;
    TRISC = 0b0001000;
    TRISD = 0;
    ANSEL = 0;
    ANSELH = 0;
   
    ADCON1 = 0;
    
   spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
   
   //configuracion del serial
   
   TXSTAbits.SYNC = 0;
   TXSTAbits.BRGH = 1;
   TXSTAbits.TXEN = 1;
   
   BAUDCTLbits.BRG16 = 1;
   SPBRG = 103;
   SPBRGH = 0;
   
   RCSTAbits.SPEN = 1;
   RCSTAbits.RX9 = 0;
   RCSTAbits.CREN = 1;
   
   PIE1bits.RCIE = 1;  //habilitar serial
   PIR1bits.RCIF = 0;
   
}

int Pot1;
int Pot2;
int cont;

void main(void) {
    config();
    while(1){
       
       spiWrite(1);    //mandar un 1 al slave
       Pot1 = spiRead(); //lee lo que manda el slave
       __delay_ms(5);
       spiWrite(0);  // manda un 0 al slave
       Pot2 = spiRead();  //lee lo que manda 
       __delay_ms(5);
       
       
        TXREG = Pot1;
        while (PIR1bits.TXIF == 0){   
        }
        
        TXREG = Pot2;
        while (PIR1bits.TXIF ==0){  
        }
        
        cont = RCREG;
    }
    return;
}


