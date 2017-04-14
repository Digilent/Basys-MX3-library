/* 
 * File:   config.h
 * Author: ROFatuC
 *
 * Created on December 15, 2015, 11:09 AM
 */

#ifndef CONFIG_H
#define	CONFIG_H

#define PB_FRQ  40000000

#define macro_enable_interrupts() \
{  unsigned int val = 0;\
    asm volatile("mfc0 %0,$13":"=r"(val));  \
    val |= 0x00800000;  \
    asm volatile("mtc0 %0,$13" : "+r"(val)); \
    INTCONbits.MVEC = 1; \
__builtin_enable_interrupts(); }

//#define macro_enable_interrupts INTEnableSystemMultiVectoredInt()

#define macro_disable_interrupts __builtin_disable_interrupts()
//#define macro_disable_interrupts INTDisableInterrupts()



/*
#ifdef	__cplusplus
extern "C" {
#endif
*/
//LEDS
#define tris_LEDS_GRP   TRISA
#define  lat_LEDS_GRP   LATA
#define latinv_LEDS_GRP LATAINV
#define  prt_LEDS_GRP   PORTA
#define  msk_LEDS_GRP   0xFF    
#define  lat_LEDS_GRP_ADDR   0xBF886030

#define  lat_LEDS_LED0  LATAbits.LATA0
#define  lat_LEDS_LED1  LATAbits.LATA1
#define  lat_LEDS_LED2  LATAbits.LATA2
#define  lat_LEDS_LED3  LATAbits.LATA3
#define  lat_LEDS_LED4  LATAbits.LATA4
#define  lat_LEDS_LED5  LATAbits.LATA5
#define  lat_LEDS_LED6  LATAbits.LATA6
#define  lat_LEDS_LED7  LATAbits.LATA7

// SWT

#define tris_SWT_SWT0   TRISFbits.TRISF3
#define  prt_SWT_SWT0   PORTFbits.RF3
        
#define tris_SWT_SWT1   TRISFbits.TRISF5
#define  prt_SWT_SWT1   PORTFbits.RF5

#define tris_SWT_SWT2   TRISFbits.TRISF4
#define  prt_SWT_SWT2   PORTFbits.RF4

#define tris_SWT_SWT3   TRISDbits.TRISD15
#define  prt_SWT_SWT3   PORTDbits.RD15

#define tris_SWT_SWT4   TRISDbits.TRISD14
#define  prt_SWT_SWT4   PORTDbits.RD14

#define tris_SWT_SWT5   TRISBbits.TRISB11
#define  prt_SWT_SWT5   PORTBbits.RB11
#define  ansel_SWT_SWT5 ANSELBbits.ANSB11

#define tris_SWT_SWT6   TRISBbits.TRISB10
#define  prt_SWT_SWT6   PORTBbits.RB10
#define  ansel_SWT_SWT6 ANSELBbits.ANSB10

#define tris_SWT_SWT7   TRISBbits.TRISB9
#define  prt_SWT_SWT7   PORTBbits.RB9
#define  ansel_SWT_SWT7 ANSELBbits.ANSB9

 // Buttons
#define tris_BTN_BTNU   TRISBbits.TRISB1
#define prt_BTN_BTNU    PORTBbits.RB1
#define ansel_BTN_BTNU  ANSELBbits.ANSB1

#define tris_BTN_BTNL   TRISBbits.TRISB0
#define prt_BTN_BTNL    PORTBbits.RB0
#define ansel_BTN_BTNL  ANSELBbits.ANSB0

#define tris_BTN_BTNC   TRISFbits.TRISF0
#define prt_BTN_BTNC    PORTFbits.RF0

#define tris_BTN_BTNR   TRISBbits.TRISB8
#define prt_BTN_BTNR    PORTBbits.RB8
#define ansel_BTN_BTNR  ANSELBbits.ANSB8

#define tris_BTN_BTND   TRISAbits.TRISA15
#define  prt_BTN_BTND   PORTAbits.RA15

 // SSD - Seven Segment Display

// Cathods
#define tris_SSD_CA     TRISGbits.TRISG12
#define  lat_SSD_CA     LATGbits.LATG12

#define tris_SSD_CB     TRISAbits.TRISA14
#define  lat_SSD_CB     LATAbits.LATA14

#define tris_SSD_CC     TRISDbits.TRISD6
#define  lat_SSD_CC     LATDbits.LATD6

#define tris_SSD_CD    TRISGbits.TRISG13
#define  lat_SSD_CD     LATGbits.LATG13

#define tris_SSD_CE     TRISGbits.TRISG15
#define  lat_SSD_CE     LATGbits.LATG15

#define tris_SSD_CF     TRISDbits.TRISD7
#define  lat_SSD_CF     LATDbits.LATD7

#define tris_SSD_CG     TRISDbits.TRISD13
#define  lat_SSD_CG     LATDbits.LATD13

#define tris_SSD_DP     TRISGbits.TRISG14
#define  lat_SSD_DP     LATGbits.LATG14

// Anods
#define tris_SSD_AN0     TRISBbits.TRISB12
#define lat_SSD_AN0      LATBbits.LATB12
#define msk_SSD_AN0      1<<12
#define ansel_SSD_AN0    ANSELBbits.ANSB12

#define tris_SSD_AN1     TRISBbits.TRISB13
#define lat_SSD_AN1      LATBbits.LATB13
#define msk_SSD_AN1      1<<13
#define ansel_SSD_AN1    ANSELBbits.ANSB13

#define tris_SSD_AN2     TRISAbits.TRISA9
#define lat_SSD_AN2      LATAbits.LATA9
#define msk_SSD_AN2      1<<9


#define tris_SSD_AN3     TRISAbits.TRISA10
#define lat_SSD_AN3      LATAbits.LATA10
#define msk_SSD_AN3      1<<10

#define lat_SSD_AN23     LATA
#define msk_SSD_AN23     (msk_SSD_AN2 | msk_SSD_AN3)
#define prt_SSD_AN23     PORTA

#define lat_SSD_AN01     LATB
#define msk_SSD_AN01     (msk_SSD_AN0 | msk_SSD_AN1)
#define prt_SSD_AN01     PORTB
// LCD

#define tris_LCD_DISP_RS    TRISBbits.TRISB15
#define lat_LCD_DISP_RS     LATBbits.LATB15
#define ansel_LCD_DISP_RS   ANSELBbits.ANSB15
#define rp_LCD_DISP_RS      RPB15R


#define tris_LCD_DISP_RW    TRISDbits.TRISD5
#define  lat_LCD_DISP_RW    LATDbits.LATD5
#define rp_LCD_DISP_RW      RPD5R

#define tris_LCD_DISP_EN    TRISDbits.TRISD4
#define  lat_LCD_DISP_EN    LATDbits.LATD4
#define rp_LCD_DISP_EN      RPD4R

#define tris_LCD_DATA       TRISE
#define lat_LCD_DATA        LATE
#define prt_LCD_DATA        PORTE
#define msk_LCD_DATA        0xFF
#define  lat_LCD_DATA_ADDR   0xBF886440
#define ansel_LCD_DB2        ANSELEbits.ANSE2
#define ansel_LCD_DB4        ANSELEbits.ANSE4
#define ansel_LCD_DB5        ANSELEbits.ANSE5
#define ansel_LCD_DB6        ANSELEbits.ANSE6
#define ansel_LCD_DB7        ANSELEbits.ANSE7


// RGBLED
#define tris_LED8_R         TRISDbits.TRISD2
#define rp_LED8_R           RPD2R
#define lat_LED8_R          LATDbits.LATD2
#define ansel_LED8_R        ANSELDbits.ANSD2

#define tris_LED8_G         TRISDbits.TRISD12
#define rp_LED8_G           RPD12R
#define lat_LED8_G          LATDbits.LATD12


#define tris_LED8_B         TRISDbits.TRISD3
#define rp_LED8_B           RPD3R
#define lat_LED8_B          LATDbits.LATD3
#define ansel_LED8_B        ANSELDbits.ANSD3

// SPIFLASH - corresponds to SPI1

#define tris_SPIFLASH_CE    TRISFbits.TRISF8
#define  lat_SPIFLASH_CE    LATFbits.LATF8


#define tris_SPIFLASH_SCK   TRISFbits.TRISF6
#define  lat_SPIFLASH_SCK   LATFbits.LATF6



#define tris_SPIFLASH_SO   TRISFbits.TRISF7
#define   rp_SPIFLASH_SO   SDI1R

#define tris_SPIFLASH_SI   TRISFbits.TRISF2
#define   rp_SPIFLASH_SI   RPF2R




// digital out
#define tris_A_OUT   TRISBbits.TRISB14
#define rp_A_OUT     RPB14R
#define  lat_A_OUT   LATBbits.LATB14
#define latinv_A_OUT LATBINV = (1<<14); 

#define ansel_A_OUT  ANSELBbits.ANSB14

// MIC
#define tris_MIC    TRISBbits.TRISB4
#define ansel_MIC   ANSELBbits.ANSB4



// motor driver
#define tris_MOT_MODE  TRISFbits.TRISF1
#define lat_MOT_MODE   LATFbits.LATF1

#define tris_MOT_AIN1   TRISBbits.TRISB3
#define lat_MOT_AIN1   LATBbits.LATB3
#define ansel_MOT_AIN1  ANSELBbits.ANSB3


#define tris_MOT_AIN2   TRISEbits.TRISE8
#define lat_MOT_AIN2    LATEbits.LATE8
#define rp_MOT_AIN2     RPE8R

#define tris_MOT_BIN1   TRISEbits.TRISE9
#define lat_MOT_BIN1    LATEbits.LATE9

#define tris_MOT_BIN2   TRISBbits.TRISB5
#define lat_MOT_BIN2    LATBbits.LATB5
#define ansel_MOT_BIN2  ANSELBbits.ANSB5
#define rp_MOT_BIN2     RPB5R

// servo 
#define tris_SRV_S0PWM   TRISBbits.TRISB8
#define lat_SRV_S0PWM    LATBbits.LATB8
#define ansel_SRV_S0PWM  ANSELBbits.ANSB8
#define rp_SRV_S0PWM     RPB8R

#define tris_SRV_S1PWM   TRISAbits.TRISA15
#define lat_SRV_S1PWM    LATAbits.LATA15
#define rp_SRV_S1PWM     RPA15R

// ACL
#define tris_ACL_INT2   TRISGbits.TRISG0
#define lat_ACL_INT2    LATGbits.LATG0


// UART
#define tris_UART_TX   TRISFbits.TRISF12 
#define   rp_UART_TX   RPF12R

#define tris_UART_RX   TRISFbits.TRISF13 
#define   rp_UART_RX   U4RXR

// UARTJB - over JB2 and JB3
#define tris_UARTJB_TX   TRISDbits.TRISD11 
#define   rp_UARTJB_TX   RPD11R

#define tris_UARTJB_RX   TRISDbits.TRISD10 
#define   rp_UARTJB_RX   U1RXR


// JA
// JA1- RC2 - 5V tol
#define tris_PMODS_JA1   TRISCbits.TRISC2 
#define   rp_PMODS_JA1   RPC2R
#define  lat_PMODS_JA1   LATCbits.LATC2
#define  prt_PMODS_JA1   PORTCbits.RC2
#define cnpu_PMODS_JA1   CNPUCbits.CNPUC2
#define cnpd_PMODS_JA1   CNPDCbits.CNPDC2

// JA2 - RC1 - 5V tol
#define tris_PMODS_JA2   TRISCbits.TRISC1 
#define   rp_PMODS_JA2   RPC1R
#define  lat_PMODS_JA2   LATCbits.LATC1
#define  prt_PMODS_JA2   PORTCbits.RC1
#define cnpu_PMODS_JA2   CNPUCbits.CNPUC1
#define cnpd_PMODS_JA2   CNPDCbits.CNPDC1

// JA3 - RC4 - 5V tol
#define tris_PMODS_JA3   TRISCbits.TRISC4 
#define   rp_PMODS_JA3   RPC4R
#define  lat_PMODS_JA3   LATCbits.LATC4
#define  prt_PMODS_JA3   PORTCbits.RC4
#define cnpu_PMODS_JA3   CNPUCbits.CNPUC4
#define cnpd_PMODS_JA3   CNPDCbits.CNPDC4

// JA4 - RG6
#define tris_PMODS_JA4   TRISGbits.TRISG6 
#define   rp_PMODS_JA4   RPG6R
#define  lat_PMODS_JA4   LATGbits.LATG6
#define  prt_PMODS_JA4   PORTGbits.RG6
#define ansel_PMODS_JA4  ANSELGbits.ANSG6
#define cnpu_PMODS_JA4   CNPUGbits.CNPUG6
#define cnpd_PMODS_JA4   CNPDGbits.CNPDG6

// JA7 - RC3 - 5V tol
#define tris_PMODS_JA7   TRISCbits.TRISC3 
#define   rp_PMODS_JA7   RPC3R
#define  lat_PMODS_JA7   LATCbits.LATC3
#define  prt_PMODS_JA7   PORTCbits.RC3
#define cnpu_PMODS_JA7   CNPUCbits.CNPUC3
#define cnpd_PMODS_JA7   CNPDCbits.CNPDC3

// JA8 - RG7
#define tris_PMODS_JA8   TRISGbits.TRISG7 
#define   rp_PMODS_JA8   RPG7R
#define  lat_PMODS_JA8   LATGbits.LATG7
#define  prt_PMODS_JA8   PORTGbits.RG7
#define ansel_PMODS_JA8  ANSELGbits.ANSG7
#define cnpu_PMODS_JA8   CNPUGbits.CNPUG7
#define cnpd_PMODS_JA8   CNPDGbits.CNPDG7

// JA9 - RG8
#define tris_PMODS_JA9   TRISGbits.TRISG8 
#define   rp_PMODS_JA9   RPG8R
#define  lat_PMODS_JA9   LATGbits.LATG8
#define  prt_PMODS_JA9   PORTGbits.RG8
#define ansel_PMODS_JA9  ANSELGbits.ANSG8
#define cnpu_PMODS_JA9   CNPUGbits.CNPUG8
#define cnpd_PMODS_JA9   CNPDGbits.CNPDG8

// JA10 - RG9
#define tris_PMODS_JA10   TRISGbits.TRISG9 
#define   rp_PMODS_JA10   RPG9R
#define  lat_PMODS_JA10   LATGbits.LATG9
#define  prt_PMODS_JA10   PORTGbits.RG9
#define ansel_PMODS_JA10  ANSELGbits.ANSG9
#define cnpu_PMODS_JA10   CNPUGbits.CNPUG9
#define cnpd_PMODS_JA10   CNPDGbits.CNPDG9

// JB
// JB1- RD9 - 5V tol
#define tris_PMODS_JB1   TRISDbits.TRISD9 
#define   rp_PMODS_JB1   RPD9R
#define  lat_PMODS_JB1   LATDbits.LATD9
#define  prt_PMODS_JB1   PORTDbits.RD9
#define cnpu_PMODS_JB1   CNPUDbits.CNPUD9
#define cnpd_PMODS_JB1   CNPDDbits.CNPDD9
#define  odc_PMODS_JB1   ODCDbits.ODCD9

// JB2- RD11 - 5V tol
#define tris_PMODS_JB2   TRISDbits.TRISD11 
#define   rp_PMODS_JB2   RPD11R
#define  lat_PMODS_JB2   LATDbits.LATD11
#define  prt_PMODS_JB2   PORTDbits.RD11
#define cnpu_PMODS_JB2   CNPUDbits.CNPUD11
#define cnpd_PMODS_JB2   CNPDDbits.CNPDD11
#define  odc_PMODS_JB2   ODCDbits.ODCD11

// JB3- RD10 - 5V tol
#define tris_PMODS_JB3   TRISDbits.TRISD10 
#define   rp_PMODS_JB3   RPD10R
#define  lat_PMODS_JB3   LATDbits.LATD10
#define  prt_PMODS_JB3   PORTDbits.RD10
#define cnpu_PMODS_JB3   CNPUDbits.CNPUD10
#define cnpd_PMODS_JB3   CNPDDbits.CNPDD10
#define  odc_PMODS_JB3   ODCDbits.ODCD10

// JB4- RD8 - 5V tol
#define tris_PMODS_JB4   TRISDbits.TRISD8 
#define   rp_PMODS_JB4   RPD8R
#define  lat_PMODS_JB4   LATDbits.LATD8
#define  prt_PMODS_JB4   PORTDbits.RD8
#define cnpu_PMODS_JB4   CNPUDbits.CNPUD8
#define cnpd_PMODS_JB4   CNPDDbits.CNPDD8
#define  odc_PMODS_JB4   ODCDbits.ODCD8

// JB7 - RC14
#define tris_PMODS_JB7   TRISCbits.TRISC14 
#define   rp_PMODS_JB7   RPC14R
#define  lat_PMODS_JB7   LATCbits.LATC14
#define  prt_PMODS_JB7   PORTCbits.RC14
#define cnpu_PMODS_JB7   CNPUCbits.CNPUC14
#define cnpd_PMODS_JB7   CNPDCbits.CNPDC14

// JB8 - RC14
#define tris_PMODS_JB8   TRISDbits.TRISD0 
#define   rp_PMODS_JB8   RPD0R
#define  lat_PMODS_JB8   LATDbits.LATD0
#define  prt_PMODS_JB8   PORTDbits.RD0
#define cnpu_PMODS_JB8   CNPUDbits.CNPUD0
#define cnpd_PMODS_JB8   CNPDDbits.CNPDD0

// JB9 - RC14
#define tris_PMODS_JB9   TRISDbits.TRISD1 
#define   rp_PMODS_JB9   RPD1R
#define  lat_PMODS_JB9   LATDbits.LATD1
#define  prt_PMODS_JB9   PORTDbits.RD1
#define ansel_PMODS_JB9  ANSELDbits.ANSD1
#define cnpu_PMODS_JB9   CNPUDbits.CNPUD1
#define cnpd_PMODS_JB9   CNPDDbits.CNPDD1

// JB10 - RC13
#define tris_PMODS_JB10   TRISCbits.TRISC13 
#define   rp_PMODS_JB10   RPC13R
#define  lat_PMODS_JB10   LATCbits.LATC13
#define  prt_PMODS_JB10   PORTCbits.RC13
#define cnpu_PMODS_JB10   CNPUCbits.CNPUC13
#define cnpd_PMODS_JB10   CNPDCbits.CNPDC13

// SPI on JA - corresponds to SPI1

// - 5V tol
#define tris_SPIJA_CE    tris_PMODS_JA1
#define  lat_SPIJA_CE    lat_PMODS_JA1

// - 5V tol
#define tris_SPIJA_SI   tris_PMODS_JA2 // JA2 - RC1
#define   rp_SPIJA_SI   RPC1R

// - 5V tol
#define tris_SPIJA_SO   tris_PMODS_JA3 
#define   rp_SPIJA_SO   SDI2R


#define tris_SPIJA_SCK   tris_PMODS_JA4
#define  lat_SPIJA_SCK   lat_PMODS_JA4
#define ansel_SPIJA_SCK  ansel_PMODS_JA4


// Analog input AN2
#define tris_ADC_AN2    TRISBbits.TRISB2
#define ansel_ADC_AN2   ANSELBbits.ANSB2


// IrDA
#define  tris_IRDA_PDOWN    TRISGbits.TRISG1
#define  lat_IRDA_PDOWN     LATGbits.LATG1

#define  tris_IRDA_RX       TRISBbits.TRISB6
#define  prt_IRDA_RX        PORTBbits.RB6
#define  ansel_IRDA_RX      ANSELBbits.ANSB6

#define  tris_IRDA_TX       TRISBbits.TRISB7
#define  prt_IRDA_TX        LATBbits.LATB7
#define  ansel_IRDA_TX      ANSELBbits.ANSB7

#define  IRDA_RX_ext_int    RPB6R
/*
#ifdef	__cplusplus
}
#endif
*/
#endif	/* CONFIG_H */

