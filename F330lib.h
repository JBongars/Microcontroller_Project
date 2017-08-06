// F330lib.H
// Copyright (2011) to
// Electronic & Computer Engineering, School of Engineering
// Ngee Ann Polytechnic, Singapore

// Updated 4 June 2012

// You may save code size & RAM usage by removing unnecessary ISRs (applicable to SDCC)
// If the following library ISRs are not used, place define statements below BEFORE the
// #include <f330lib.h> statement
// 1. Remove serial comm interrupt Interrupt:
// #define NO_SERIAL_LIB_INTERRUPT // Save 521 bytes codes & also 22 bytes internal RAM!

// 2. Remove Timer 2 interrupt functions
// #define NO_TIMER2_LIB_INTERRUPT // saves 132 bytes of codes and 5 bytes of RAM

// 3. Remove speaker ISR functions
// #define NO_SPK_LIB_INTERRUPT // saves 116 bytes of codes & 2 bytes of RAM

// 4. Remove SMB ISR functions (these functions are not implemented yet)
// #define NO_SMB_LIB_INTERRUPT

// e.g. if no serial, timer2 and speaker interrrupts used, do these:
// #define NO_SERIAL_LIB_INTERRUPT
// #define NO_SPK_LIB_INTERRUPT
// #incude <f330lib.h>

// Last modified: 13 Nov 2011

// F330lib.H for F330s.LIB used in MPI/ESD/DSDA kitset designed for Oct 07 semester
// "s" for small model
// Former names: MPI.H For MPI.LIB used in MPI/ESD kitset designed for Oct 07 semester
// C8051F330 MCU is used

// For projects, if circuit is the same, one can use the library as it is
// For different circuits, see instructions in comments in appropriate sections

// Written/Edited by Wong SS (wss@np.edu.sg) & Wong SC (wsc@np.edu.sg)

// Flash Programming codes were taken from Silabs Application notes

#ifndef __F330LIB_H__
#define __F330LIB_H__

#include <C8051F330.h>

#ifndef code
#define code __code // Newer SDCC (3.0 onwards) treat these as deprecate. This avoids the warnings
#endif

#ifndef sbit
#define sbit __sbit
#endif

#ifndef bit
#define bit __bit
#endif

#ifndef at
#define at __at
#endif

#ifndef data
#define data __data
#endif

#ifndef xdata
#define xdata __xdata
#endif

#ifndef using
#define using __using
#endif

#ifndef idata
#define idata __idata
#endif

#ifndef interrupt
#define interrupt __interrupt
#endif

#ifndef _asm
#define _asm __asm
#endif

#ifndef _endasm
#define _endasm __endasm
#endif

typedef unsigned char UCHAR;
typedef unsigned char uchar;
typedef signed char SCHAR;
typedef signed char schar;
typedef unsigned int UINT;
typedef unsigned int uint;
typedef signed int SINT;
typedef signed int sint;
typedef unsigned long ULONG;
typedef unsigned long ulong;
typedef signed long SLONG;
typedef signed long slong;

// already have these defined in compiler_defs.h
#ifndef U8
typedef unsigned char U8;
typedef signed char S8;
typedef unsigned int U16;
typedef signed int S16;
typedef unsigned long U32;
typedef signed long S32;
#endif

typedef bit bool;
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
typedef unsigned char uint8;
typedef signed char sint8;
typedef unsigned short uint16;
typedef signed short sint16;
typedef unsigned long uint32;
typedef signed long sint32;

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned long u32;
typedef signed long s32;

typedef UINT FLADDR;	// Flash Address type

extern unsigned char _errorCode;

#define BITSET(n,p) (n = (n) | (0x01 << (p)))
#define BITCLR(n,p) (n = (n) & ~(0x01 << (p)))

// User can overwrite SDCC default function below to do something at the very beginning
// of reset. E.g. disable watch dog (if doing it in main() is too late.

// unsigned char _sdcc_external_startup( void ) {
//	return 0; // do data initialization upon return (normal)
//  return 1; // skip data initialization ! (Be sure of what you are doing!)
// }
// ==========================================================================================
// MPI kitset Oct 07
// i2c devices addresses
#define _RTC			0x51		// i2c PCF8583 real-time clock
#define _LCD			0x21		// i2c for LCD PCF8574
#define _IOE16			0x20		// i2c MCP23016 16-bit IO expander

sbit at 0x86 INT;	// same as PBSW
sbit at 0x86 PBSW;
sbit at 0x87 _SPKR;	// used internally

struct eepromS {
	unsigned char i2cID;
	unsigned int eepaddr;
	unsigned int nbytes;
	unsigned char *dataptr;
};

typedef struct {
	unsigned char dlyOn;
	unsigned int oldTime;
	unsigned int dlyRequired;
} tmr2DlyGType;

typedef struct {
	unsigned char SWOn;
	unsigned long SWStartStamp;
	unsigned long SWStopValue;
} tmr2SWType;

// ==========================================================================================
// This function is to be provided by the user, e.g. in init.c file, to be called by main()
void Init_Device( void );
void hang ( void);
// ==========================================================================================
// System clock functions (For C8051F330)
// IDs & clock speeds in KHz:
// LF KHz 0-10    1-20    2-40     3-80
// HF KHz 4-3,062 5-6,125 6-12,250 7-24,500

#define SYSCLK_24M5 7
#define SYSCLK_12M25  6
#define SYSCLK_6M125  5
#define SYSCLK_3M0625 4
#define SYSCLK_80K 3
#define SYSCLK_40K 2
#define SYSCLK_20K 1
#define SYSCLK_10K 0

#define SYSCLKIDMAX 7	// 0 to 7 only
unsigned char sysClkGetID( void );
unsigned int sysClkGetKHz( void );
void sysClkSetID( unsigned char ClkID ); // See IDs in comments above
// sysClkSetID() must be called before initialization of UART or timers !!!

// function below not tested !!! Don't use
// void sysClkSetKHz( unsigned int clkKHz ); // try to match. Will get equal or higher
// ==========================================================================================
// set to idle or stop mode
void idleModeSet ( void );
void stopModeSet ( void );

void watchdogOff ( void ); // normally off by Init_Device(); call this first thing in your program
void watchdogOn ( void ); // Once on PCA0MD cannot be modified
void watchdogLock ( void ); // Once locked, cannot be disabled anymore
void watchdogKick ( void );
void watchdogDur ( unsigned char n );
// ==========================================================================================
void waitPBSW (void); // Wait for the PBSW pressed 1 -> 0
unsigned char nib2Ascii (unsigned char);
void beep (unsigned char numOf2ms); // make a beep sound at speaker (spk... routines not used)
void out7Seg ( unsigned char val ); // use this if keypad functions is also used with 7 seg
// ==========================================================================================
// smbi functions
#ifndef NO_SMB_LIB_INTERRUPT

//void SMBusMaster_ISR (void) interrupt 7 __using 1;	// If interrupt is used
#endif // #ifndef NO_SMB_LIB_INTERRUPT

//// Non interrupt version
//int smbInitTmr1(int smbRateKHz);
//int smbInitTmr0(int smbRateKHz);
//int smbBitRateSet(int smbRateKHz);	// smbInitTmrx() must be called 1st, timer uses SYSCLK/1
//bit smbSendData(unsigned char addr, unsigned char n, unsigned char arr[]);	// 1 for error
//bit smbRecData(unsigned char addr, unsigned char n, unsigned char arr[]);	// 1 for error
// ==========================================================================================
void pinAInP0 ( unsigned char bits );	// 1s in bits set to Analog in; NO pin skip done (use config wizard for clarity)
void pinAInP1 ( unsigned char bits );
void pinDInP0 ( unsigned char bits );	// 1s in bits set to Digital in (Open Drain), output 1, NEVER write 0 to these bits in P0
void pinDInP1 ( unsigned char bits );
void pinDInP2 ( unsigned char bits );
void pinDOutODP0 ( unsigned char bits );	// 1s in bits set to Digital out - Open Drain
void pinDOutODP1 ( unsigned char bits );
void pinDOutODP2 ( unsigned char bits );
void pinDOutPPP0 ( unsigned char bits );	// 1s in bits set to Digital out - push-pull
void pinDOutPPP1 ( unsigned char bits );
void pinDOutPPP2 ( unsigned char bits );
// Two functions below would affect crossbar pins assignment !
void pinUartEnable ( void );		// enable uart pins - Tx at P0.4, Rx at P0.5 
void pinUartDisable ( void );		// disable uart pins - Tx at P0.4, Rx at P0.5
void portCrossBarDisable ( void );	// IO pins enabled
void portCrossBarEnable ( void );
void portWeakPullDisable ( void );	// weak pull ups for all open drain pins disabled
void portWeakPullEnable ( void );
// ==========================================================================================
// Some general routines
unsigned char BCDToBin ( unsigned char a);
unsigned char binToBCD ( unsigned char a);
unsigned int BCDAdd ( unsigned char a, unsigned char b); // max result 0x0198
bit BCDValid ( unsigned char a);
unsigned char * strAdvNull(unsigned char *str); // return pointer to end of the str
bit strIsDec ( unsigned char *str ); // '0' - '9'
bit strIsHex (unsigned char *str ); // '0' - '9', 'A' - 'F' or 'a' - 'f'

// ==========================================================================================
// ----- General i2c IO routines ----
// ----- IO Expander PCF8574: 8-bit open-drain out/in
// ----- Addresses for PCF8574: 0x20 to 0x27, PCF8574A: 0x38 to 0x3F
unsigned char i2cioExpIn ( unsigned char i2cID);	
bit i2cioExpOut (unsigned char i2cID, unsigned char val);

// ----- PCF8591: 4 channel ADC + 1 DAC, address is 0x48 to 0x4F -----
bit i2cdac( unsigned char i2cID, unsigned char d8 );
bit i2cadcAll( unsigned char i2cID, unsigned char *arr ); // 4 channels results
unsigned char i2cadcCh( unsigned char i2cID, unsigned char ch ); // ch=0 to 3, CY=I2C status

// ----- 24256: EEPROM has 32KByte RAM, address is 0x50 to 0x57 -----
// Note that the address overlaps with RTC PCF8583 is 0x50 to 0x51
bit i2cEEPROMWrite ( struct eepromS *eps );
bit i2cEEPROMWriteByte ( unsigned char i2cID, unsigned int addr, unsigned char edata);
bit i2cEEPROMRead ( struct eepromS *eps );
unsigned char i2cEEPROMReadByte ( unsigned char i2cID, unsigned int addr); // CY=1 I2C error

// ==========================================================================================
// ----- Specific to IO Board ----
// For MPI Oct 07 kitset
// It uses MCP23016 at i2c addr 0x20 GP0 for 8 switches inputs, GP1 for 8 outputs to LEDs
// A slow device. Within each i2cDelay(1) called at beginning, i2cDelay(0) called at the end
bit SWLEDInit ( void );	// set INs at GP0, OUTs at GP1, returns 1 for i2c error
unsigned char in8SW ( void ); // CY show I2C status
bit out8LED ( unsigned char d8 ); // I2C status, 0 means success, 1 means error

// ==========================================================================================
// conversion basic & Others

unsigned char toUpper ( unsigned char ch);
unsigned char toLower ( unsigned char ch);
void trim ( unsigned char *s );	// remove leading or trailing spaces or tabs

// unsigned char bcdAdd8 ( unsigned char d1, unsigned char d2 ); // not available
// unsigned int bcdAdd16 ( unsigned int d1, unsigned int d2 ); // not available

// ==========================================================================================
// F330 Flash Primitives
// 512 bytes in 0x1e00 - 0x1fff are reserved
// 3 functions below do not use scratch pad
bit FLASH_ByteWrite (FLADDR addr, unsigned char byte);
	// Write to a specific code space location with data provided
	// Can only change a bit from 1 to 0 and not a 0 to 1
	// Even if a location already contains some 0's, the 1 can still be changed to 0
	// return 0 for data same as value read back after programming, 1 for error
unsigned char FLASH_ByteRead (FLADDR addr);
	// Read from the code space
bit FLASH_PageErase (FLADDR addr); // 512 bytes page
	// Erase 512 bytes page that has addr, examples:
	// 0x0000-0x01ff,0x0200-0x03ff,0x0400-0x05ff,0x0600-0x07ff
	// 0x0800-0x09ff,0x0a00-0x0bff,0x0c00-0x0dff,0x0e00-0x0fff etc
//------------------------------
// FLASH Utilities
#define FLASH_TEMP 0x01a00L // 1 page used by flash utilities as scratch pad CAREFUL !
#define FLASH_LAST 0x01c00L
#define FLASH_PAGESIZE 512
bit FLASH_Write (FLADDR dest, unsigned char *src, unsigned int numbytes);
unsigned char * FLASH_Read (unsigned char *dest, FLADDR src, unsigned int numbytes);
bit FLASH_Clear (FLADDR addr, unsigned int numbytes);
// copy with destination preservation
bit FLASH_Update (FLADDR dest, unsigned char *src, unsigned int numbytes);   
// low-level FLASH/FLASH byte copy
bit FLASH_Copy (FLADDR dest, FLADDR src, unsigned int numbytes);    
// FLASH test routines
bit FLASH_Fill (FLADDR addr, unsigned long length, unsigned char fill);
// ==========================================================================================
// keypad (To P1. For I2C 8574 IOExpander, same pin numbering.)
// P1.0 - R1
// P1.1 - R2
// P1.2 - R3
// P1.3 - R4
// P1.4 - C1
// P1.5 - C2
// P1.6 - C3
// P1.7 - C4
// Functions assumes each pin is open drain type that can be used for input or output
// That is line-reversal technique is used
// Default library assumes P1 is shared with 7-seg LEDs
void keyInit ( void );
unsigned char keyGet ( void );	// wait for a key press & return, BAD generally ! Do not use
unsigned char keyChk ( void );	// if new key pressed return ASCII, else 0xff, _keyBounceDly
bit keyRawPressed ( void );	// any key pressed right now
bit keySamePressed ( void ); // Indicate if same valid key is pressed
// --------------------------------------------------
// If P1 is not shared by 7-segment and keypad, do this to overwrite
// void _keyRestoreP1( void ) { // this function does nothing
// }
// Do not use out7Seg() in this case as there should not be any 7-seg connected to P1.
// --------------------------------------------------
// If keypad is connected to PCF8574 IOExpander using above pin connections, do these
// #define KEYI2CADDR 0x3a // set your own I2C address here
// void _keyWrite(unsigned char val) {
//	return(i2cSendData(KEYI2CADDR,1,&val));
// }
// unsigned char _keyRead ( void ) {
//	unsigned char c;
//	if (i2cRecData(KEYI2CADDR,1,&c) != 0) {
//		return 0xff; // error
//	}
//	return c;
// }
// void _keyRestoreP1( void ) { // this function does nothing
// }
// --------------------------------------------------
// The debounce duration is default to 5ms used in keyChk(). To change, do this:
// _keyBounceDly = 10;	// 10ms debounce is required, default is 5 done by keyInit();
// Set to 0 if no delay is required, e.g. if keyChk() is called at say 10ms interval
extern xdata unsigned char _keyBounceDly;
// --------------------------------------------------
// Default library assumes P1 is shared with 7-seg
// Calling internal _keyScan() would upset P1, thus 7-seg values

// Solution:
// Use out7Seg() to write to the 7-seg at P1
// This function also sets a flag for _keyScan() to
// - restore value written to P1
// - leave it on for say 2 ms
// Internal Variables used are as below:
extern bit _keyP1ValRestore;	// Default 0 i.e. no restore
extern xdata unsigned char _keyP1ValDly; // delay in ms during keyScan if _keyP1ValRestore 1
		// can turn off by changing it to 0
extern xdata unsigned char _P1Val;
// --------------------------------------------------
// For user's own keypad, overwrite

// 1. unsigned char _keyScan( void ); // no need buffering or debounce, no key press -> 0xFF

// 2. These two translation tables. The 1st table contians scan codes returned by _keyScan()
// The 2nd table contains the corresponding ASCII code.
// Do not change the 1st entry (0xff) of both tables.

// code const unsigned char _keyScanCodes[] = { // MPI kitset Oct 07
//	0xFF,0xEE,0xDE,0xBE,0x7E,0xED,0xDD,0xBD,0x7D,0xEB,0xDB,0xBB,0x7B,0xE7,0xD7,0xB7,0x77,0};
// code const unsigned char _keyAsciis[] = { // MPI kitset Oct 07
//	0xFF, '1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};

// ==========================================================================================
// LCD
// MPI kitset (Oct07) uses IO expander PCF8574 at i2c addr 0x21.
// IMPORTANT: Ensure IO expander pin connection to LCD as below:
// P0 - D4
// P1 - D5
// P2 - D6
// P3 - D7
// P4 - RS
// P5 - RW
// P6 - E
// P7 - not used, 1 is written to it
// ------------------------------
// Important: As interface above uses I2C IO Expander, make sure I2C functions works !
// E.g. if the I2C SDA and SCL pin connections are different from one used in the library,
// (see I2C functions section), make sure it is taken care off.
// Refer to I2C function description.
// ------------------------------
// If your IO Expander address is not 0x21,
// write your own function below (must be exact name) with new i2c address
/*
bit _lcdi2cSendPins ( unsigned char c) { // Write to IO expander interface to LCD
	return(i2cSendData(0x39,1,&c)); // 0x39 is an example of your own PCF8574AP I2C address
}
*/
// If it is a direct Port interface, eg. to Port 2, write to the pins directly
/*
// Up to 6MHz clock without delay
// 12MHz delayloop 2
// 24MHz delayloop 3
// 48MHz delayloop 8
bit _lcdi2cSendPins ( unsigned char c) { // Write to IO expander interface to LCD
	P2 = c;
//	delayloop(8);
	return 0; // 0 means OK
}
*/
// ------------------------------
// IF your IO Expander connection is different, you need to write your own _lcdWriteNibble() 
// below. You can refer to the source code on how this can be done.
// void _lcdWriteNibble ( unsigned char c, unsigned char rs); // rs=0 instr, non-zero for data
// lower 4 bits of c is the 4-bit data to be written to the LCD
// ------------------------------
// For 4-bit interface using parallel HW (rather than using I2C IO Expander),
// just have to overwrite _lcdWriteNibble() too.
// ------------------------------
// For 8-bit interface HW, do these 2 things:
// 1. use library function lcdInit8() or overwrite lcdInit() to call lcdInit8() as below:
//		void lcdInit ( void ) {
//			lcdInit8();
//		}
// 2. you MUST overwrite these 2 functions
// void _lcdWriteControl ( unsigned char c );
// void _lcdWriteData ( unsigned char c );
// ------------------------------
// Intelligent version of LCD functions that emulates a dumb terminal
// supports BS ('\b'), CR ('\r'), LF ('\n') and autoscroll and autowrapping
void lcdInit ( void );
void lcdInit4( void );
void lcdInit8( void );
void lcdAutoCROn ( void );
void lcdAutoCROff ( void );
void lcdClrLine1 ( void );	// cursor set to col 1 of line 1
void lcdClrLine2 ( void );	// cursor set to col 1 of line 2
void lcdCursorOn ( void );
void lcdCursorOnBlink( void );
void lcdCursorOff ( void );
void lcdCursorSave ( void );
void lcdCursorRestore( void );
bit lcdIsIntelligent( void );
void lcdPutStr ( unsigned char *str ); // generic pointer used
void lcdPutSpaces ( unsigned char n );
// NOTE: due to autowrapping, the last column of last row is not usable

void lcdClrScr ( void );
void lcdPutCh ( unsigned char ch );
void lcdSetXY ( unsigned char x, unsigned char y );
void lcdIntellSize1x8 ( void );	// set size of the LCD if intelligent mode (default) used (default is 2x16)
void lcdIntellSize1x16 ( void ); // & screen would be cleared at the same time (as buffer need to be "cleared")
void lcdIntellSize1x20 ( void );
void lcdIntellSize2x12 ( void );
void lcdIntellSize2x16 ( void );
void lcdIntellSize2x20 ( void );
void lcdIntellSize2x40 ( void );
void lcdIntellSize4x16 ( void );
void lcdIntellSize4x20 ( void );
void lcdRawSize1x8 ( void );	// set size of the LCD if raw mode used (default is 2x16)
void lcdRawSize1x16 ( void );
void lcdRawSize1x20 ( void );
void lcdRawSize2x12 ( void );
void lcdRawSize2x16 ( void );
void lcdRawSize2x20 ( void );
void lcdRawSize2x40 ( void );
void lcdRawSize4x16 ( void );
void lcdRawSize4x20 ( void );
// Intelligent version below are called by the above functions by default
void _lcdIClrScr ( void );
void _lcdIPutCh ( unsigned char ch );
void _lcdISetXY ( unsigned char x, unsigned char y );

// IF YOU DON'T NEED the "dumb" terminal intelligent version above, e.g. to SAVE
// > 220 bytes of codes (c.f. intelligent version - in Keil C) and
// > 18 bytes data
// write your own versions of lcdClrScr, lcdPutCh, lcdSetXY to call
// the "raw versions" of lcd functions below to OVERWRITE the 
// intelligent version of basic LCD functions in the library above
// in this case, DO NOT USE functions like lcdGetXY(); Do not send '\r' '\b' or '\n' !!!
void _lcdRClrScr ( void );
void _lcdRPutCh ( unsigned char ch );
void _lcdRSetXY ( unsigned char x, unsigned char y );
/* // Example:
void lcdPutCh( unsigned char c) {
	_lcdRPutCh(c); // you can also add in codes to do other things
}
void lcdSetXY( unsigned char x, unsigned char y) {
	_lcdRSetXY(x,y); // you can also add in codes to do other things
}
void lcdClrScr( void ) {
	_lcdRClrScr(); // you can also add in codes to do other things
}
*/

// DO NOT call function below if you have overwritten the intelligent version
// with the "raw versions" of lcdClrScr ....
// You will get "MULTIPLE PUBLIC DEFINITIONS" of _LCDPUTCH, .... (In Keil C)
unsigned int lcdGetXY ( void );
// X in upper 8-bits, Y in lower 8-bits

// ==========================================================================================
// lcd print num
// IMPORTANT: These functions uses _bufPutBuffer SHARED with uart print num functions
// BEWARE in multitasking environment !!!!

void lcdPutBin8 ( unsigned char num);
void lcdPutBin16 ( unsigned int num);

void lcdPutHex8 ( unsigned char num);
void lcdPutHex16 ( unsigned int num);

void lcdPutDecU8 ( unsigned char num);
void lcdPutDecU8LZ ( unsigned char dat, unsigned char noOfCol );
void lcdPutDecU8LS ( unsigned char dat, unsigned char noOfCol );

void lcdPutDecU16 ( unsigned int num);
void lcdPutDecU16LZ ( unsigned int dat, unsigned char noOfCol ); // leading 0 shown
void lcdPutDecU16LS ( unsigned int dat, unsigned char noOfCol ); // leading 0 changed to space

void lcdPutDecS8 ( signed char num);
void lcdPutDecS16 ( signed int num);
// ==========================================================================================
// string to num
/*
unsigned char asciiToHex4 ( unsigned char asciiCode );	// CY=1 for error (invalid ch)
unsigned char asciiToHex8 ( unsigned char *str );	// CY=1 means has error (.. too big)
unsigned int asciiToHex16 ( unsigned char *str );	// CY=1 means has error

unsigned char asciiToDec ( unsigned char asciiCode );		// CY=1 means has error
unsigned char asciiToDecU8 ( unsigned char *str );	// CY=1 means has error
unsigned int asciiToDecU16 ( unsigned char *str );	// CY=1 means has error
*/

// ==========================================================================================
// print num to string

// A common memory buffer is used by all functions below.
// These functions are used by serial comm & lcd print functions
// Same _bufPutBuffer used

unsigned char *bufPutBin8 ( unsigned char dat );
unsigned char *bufPutBin16 ( unsigned int dat );

unsigned char *bufPutHex8 ( unsigned char dat );
unsigned char *bufPutHex16 ( unsigned int dat );

unsigned char *bufPutDecU8 ( unsigned char dat );
unsigned char *bufPutDecU8NL ( unsigned char dat );	// no leading 0
unsigned char *bufPutDecU8LZ ( unsigned char dat, unsigned char noCol ); // leading 0
unsigned char *bufPutDecU8LS ( unsigned char dat, unsigned char noCol ); // leading spaces

unsigned char *bufPutDecU16 ( unsigned int dat );
unsigned char *bufPutDecU16NL ( unsigned int dat );	// no leading 0
unsigned char *bufPutDecU16LZ ( unsigned int dat, unsigned char noCol ); // leading 0
unsigned char *bufPutDecU16LS ( unsigned int dat, unsigned char noCol ); // leading spaces

unsigned char *bufPutDecS8 ( signed char dat );
unsigned char *bufPutDecS8NL ( signed char dat );

unsigned char *bufPutDecS16 ( signed int dat );
unsigned char *bufPutDecS16NL ( signed int dat );

// ==========================================================================================
// Serial Comm Print Nums
// IMPORTANT: These functions uses _bufPutBuffer SHARED with LCD print num functions
// BEWARE in multitasking environment !!!!

void putBin8 ( unsigned char dat );
void putBin16 ( unsigned int dat );
void putHex8 ( unsigned char dat );
void putHex16 ( unsigned int dat );

void putDecU8 ( unsigned char dat );
void putDecU8LZ ( unsigned char dat, unsigned char noOfCol );
void putDecU8LS ( unsigned char dat, unsigned char noOfCol );
void putDecU16 ( unsigned int dat );
void putDecU16LZ ( unsigned int dat, unsigned char noOfCol );
void putDecU16LS ( unsigned int dat, unsigned char noOfCol );

void putDecS8 ( signed char dat );
void putDecS16 ( signed int dat );

// ==========================================================================================
// serial: read numbers
/* NOT DONE
unsigned char getHex8 ( void );	// _errorCode !=0 means error
unsigned int getHex16 ( void );	// _errorCode !=0 means error
unsigned char getDecU8 ( void );	// _errorCode !=0 means error
unsigned int getDecU16 ( void );	// _errorCode !=0 means error
signed char getDecS8 ( void );	// _errorCode !=0 means error
signed int getDecS16 ( void );	// _errorCode !=0 or CY=1 means error
*/

// ==========================================================================================
// Serial Comm
bit uartInit (unsigned int baud100);
	// baud100 is one of these: 12,24,48,96,192,384,576,1152 // x100 bps 
	// YOU HAVE TO enable UART0 IO crossbar yourself
	// Works for all different high-freq SYSCLK
	// uartInit2() below is used by this function
	// This function generates more than 200 bytes of codes compared to uartInit2()
	//   due to lookup tables for all posssible high-freq clocks for the MCU
	// Note that some brs are not achievable (or supported, e.g. 14400 or 56000). See f330uart.h
	// IMPORTANT: SCA1 and SCA0 (Timer 0/1 prescale could be cleared for 1/12)
	// return 1 for problem, 0 for OK
bit uartInit2 (unsigned int baudSetCode);
	// uartInit2() generates very little code - more than 200 less compared to uartInit()
	// YOU HAVE TO enable UART0 IO crossbar yourself
	// You must know what code to use for the SYSCLK in operation
	// These input codes are taken from f330uart.h e.g. B9600_CD8
	// You should refer to f330uart.h contents
	// e.g. For F330 default SYSCLK of 3.0625MHz (24.5MHz/8)
	//      for baud rate of 9600, use uartInit2(B9600_CD8);
	// Note that some brs are not achievable (or supported). See f330uart.h
	// YOU HAVE to include f330uart.h to get these codes
	// IMPORTANT: SCA1 and SCA0 (Timer 0/1 prescale could be cleared for 1/12)
	// return 1 for problem, 0 for OK
unsigned char sGetCh ( void );
bit sReceiverRdy ( void );
void sPutCh ( unsigned char c );
// ---------
void putCh ( unsigned char c ); // default to use the 's' versions above
unsigned char getCh ( void );
bit receiverRdy ( void );
// ---------
void putChN( unsigned char *s, unsigned char numChars); // generic pointer, up to 255
void getChN ( unsigned char *buffer, unsigned char size );
void putSpaces ( unsigned char n );
void putStr ( unsigned char *str ); // generic pointer used
void putData ( unsigned char *str, unsigned char n );
void putNewLine ( void ); // CR, LF
void putCRLF ( void );
unsigned char getStrEN ( unsigned char * buffer, unsigned char size );
	// accepts only \r or \b and printable codes
// ---------
#ifndef NO_SERIAL_LIB_INTERRUPT
// Interrrupt Driven Serial Comm for UART0
void siInit ( unsigned char tbsize, unsigned char *tbuf, 
	unsigned char rbsize, unsigned char *rbuf );
	// IMPORTANT: siInit() does not set baud rate ...
/* Example is as below
// To make use of the interrupt driven serial functions with no program change to one written
// using standard polling serial comm (i.e. putCh, getCh, putStr, putDecU8 ... etc),
// overwrite these basic UART functions in the library:
void putCh ( unsigned char c ) {
	siPutCh(c,1);	// wait if tx buffer full
}
unsigned char getCh ( void ) {
	while (siRxDataCount() == 0);
	return siGetCh();
}
bit receiverRdy ( void ) {
	if (siRxDataCount())
		return 1;
	else
		return 0;
}
// Application example
static xdata unsigned char mytBuffer[15]; // declare outside functions as global memory
static xdata unsigned char myrBuffer[20];

	uartInit(384); // set bit rate
	siInit(15, mytBuffer, 20, myrBuffer); // use interrupt
	putStr("Hai"); // interrupt version is now used
*/
unsigned char siRxDataCount ( void );	// how many bytes of data at receiver buffer
unsigned char siRxBufSize ( void );	// how much buffer allocated
unsigned char siGetCh ( void );		// interrupt driven version of getCh()
bit siRxChkOvrErr ( void );			// check if receiver ever experience overrun error
void siRxBufFlush ( void );			// flush receiver buffer, especially when there is error

bit siPutCh ( unsigned char c, unsigned char waitIfTxBufFull );	// inter driven ver of putCh()
	// returns 1 means Tx Buffer was full !
	// If waitIfTxBufFull 1, if the the transmit buffer is full, it would wait for it
	// to be available (worst case of 1 byte time)
unsigned char siTxBufLeft ( void );	// Tx buffer left how many bytes? 0 means full !
unsigned char siTxBufSize ( void );	// Get Tx Buffer size
void _serialISR ( void ) interrupt 4 using 1;	// to be seen by main() in SDCC. Do not call !

// Functions below should have interrupt driven Tx Rx used !
void rs485Init(unsigned char n, unsigned char *s); // up to 2 trailing dummy char (0xff is best), n=0 means none
void siRS485Send(unsigned char n, unsigned char *s); // MUST send extra 1 byte of say 
	// value 0xff so that actual data would not be "chop" off. See explanation below.
	// IMPORTANT: rs485Disable will be called when upon TI=1 interrupt and there is no more data in the Tx buffer.
	// This would cause the rs485 disabled during the last byte (as 8051 has 1 byte Tx buffer) of data transmission.
	// SOLUTION: Send an extra byte of dummy data (e.g. 0xff) so that your good data will be sent properly.
	//   You should have a good protocol so that the receiver will ignore the dummy data.
void rs485Enable ( void ); // user MUST provide this (library version is dummy)
void rs485Disable ( void );  // user MUST provide this (library version is dummy)
#endif // #ifndef NO_SERIAL_LIB_INTERRUPT

// ==========================================================================================
// Delays

// Takes care of 8 different SYSCLK
// 24.5MHz/1 /2 /4 or /8, 80KHz/1 /2 /4 /8
void delayms ( unsigned int n );	// delay for n ms
void delayloop ( unsigned int i);

// ==========================================================================================
// RTC: PCF8583 at i2c address 0x51
// NO alarm used ....
// 24hr format used, all data in BCD

// For different i2c address, overwrite these 2 functions
/*bit rtci2cSendData ( unsigned char n, unsigned char *buf) {
	return (i2cSendData(_RTC,n,buf));
}
bit rtci2cRecData( unsigned char n, unsigned char *buf) {
	return (i2cRecData(_RTC,n,buf));
}
*/

extern xdata unsigned char rtcBCDHr, rtcBCDMin, rtcBCDSec;
extern xdata unsigned char rtcBCDYr, rtcBCDMth, rtcBCDDay, rtcBCDWkDay;
// RTC RAM that can be used by user starts from this address
#define RTC_USER_RAM_START 0x16 // till 0xff (i.e. 234 bytes)
// Following is used with rtcCalStrTxt()
#define CAL_FMT_TXT		0 // need 10 bytes buffer
#define CAL_FMT_TXT_WD	1 // need 14 bytes buffer

bit rtcSetClockMode ( void );	// call this once at beginning of program, 1 means ERROR
// IMPORTANT: If your application changes it to other modes (alarm/event), 
// remember to call this again. Otherwise, you may get strange results for functions below
// Details: Returns 1 for I2C communication error with RTC module, 0 if OK
// ------------ TIME ----------------------------
bit rtcTimeSet(unsigned char hBCD, unsigned char mBCD, unsigned char sBCD);
	// next time calling rtcTimeWasSet() would be true
	// It keeps a signature in the RTC RAM indicating valid time
	// Returns 1 for I2C communication error with RTC module, 0 if OK
unsigned char rtcTimeSet2(unsigned char *str); // "HHMMSS" string of time
	// return 0=OK, 1=I2C error, 2=format error, 3=value error
bit rtcTimeGet(void);	// result in rtcBCDHr, rtcBCDMin, rtcBCDSec
	// If the clock was not properly set before, one may get invalid time
	// to check, use rtcTimeWasSet()
	// Returns 1 for I2C communication error with RTC module, 0 if OK
void rtcTimeStr ( unsigned char *str );
	// It uses rtcBCDHr, ... that are assumed valid i.e. rtcTimeGet() was called before
	// format: HH:MM:SS
	// str[] must have at least 9 bytes!
void rtcTimeGetStr ( unsigned char *str );
	// calls rtcTimeGet() followed by rtcTimeStr()
	// see comments on these 2 functions
bit rtcTimeClr(void);	// Next time calling rtcTimeWasSet() would be false
	// signature in RTC RAM for valid time destroyed
	// Returns 1 for I2C communication error with RTC module, 0 if OK
bit rtcTimeWasSet ( void ); // 1 if the time in RTC was set before, 0 if not set before
// ------------ CALENDAR ------------------------
code unsigned char * rtcCalGetNameWkDay ( unsigned char i ); // get name of week day
	// E.g. 0 for "Sun", ... i is 0 to 6
code unsigned char * rtcCalGetNameMth ( unsigned char bin ); // get name of month MMM, e.g. "Jan"
	// E.g. 1 for "Jan", ... to 12 for "Dec" (in binary, NOT in BCD)
bit rtcCalSet (unsigned char yBCD, unsigned char mBCD, unsigned char dBCD, unsigned char wBCD);
	// yBCD:0x00-0x99, mBCD:0x01-0x12, dBCD:0x01.., wBCD:0(Sun)-6
unsigned char rtcCalSet2 (unsigned char *str); // "YYMMDDW" string
bit rtcCalGet ( void ); // results as below
	// rtcBCDYr (0x00 till 0x99 meaning 2000 ... FYI: RTC only keep year in 0 - 3)
	//   Actual year is store in RTC RAM and is checked for update when rtcCalGet() is called
	// rtcBCDMth (0x01 till 0x12)
	// rtcBCDDay (0x01 till 0x31 - takes care of 30/31 and 28/29 day)
	// rtcBCDWkDay (0x00 till 0x06)
	// If the calendar was not properly set before, one may get invalid info
	// To check, use rtcCalWasSet()
	// Returns 1 for I2C communication error with RTC module, 0 if OK
bit rtcCalClr ( void );	// Next time calling rtcCalWasSet() would be false
	// signature in RTC RAM for valid calendar destroyed
	// Returns 1 for I2C communication error with RTC module, 0 if OK
void rtcCalStrTxt ( unsigned char *str, unsigned char format );
	// uses rtcBCDYr, ... that are assumed valid
	// Format 0: 02 May 08 (9+1=10 char)
	// Format 1: Mon 02 May 08 (13+1=14 char)
void rtcCalStrMDY( unsigned char str[]); // mm/dd/yy
void rtcCalStrDMY( unsigned char str[]); // dd/mm/yy
bit rtcCalWasSet ( void ); // 1 if the calendar in RTC was set, 0 if not set before
// ------------ RTC RAM READ/WRITE
// These 2 function can be used to store and read data in 240 bytes RAM in RTC
// IMPORTANT: locAddr for Write is restricted
// Returns error if locAddr >= RTC_USER_RAM_START or would go beyond 0xFF
bit rtcDataWrite(unsigned char locAddr, unsigned char n, unsigned char *buf);
bit rtcDataRead(unsigned char locAddr, unsigned char n, unsigned char *buf);
// ------------ RTC RAM SIGNATURE WRITE, test, destroy
// For user to write, test or clear 2-byte signature in User RAM area
// addr >= RTC_USER_RAM_START (but <= 0xfe)
bit rtcSignatureUserSet( unsigned char addr );
bit rtcSignatureUserOK ( unsigned char addr );
bit rtcSignatureUserClr ( unsigned char addr );
// ==========================================================================================
// timer2 time functions

// The timer ISR calls following function. You can overwrite it with your own.
// void __timer2ISRHook( void );
// Remember to declare any global variables used in the function as volatile
// Compile without overlay for this function of your own. Example is as below

// volatile unsigned int count;
// #pragma save
// #pragma nooverlay
// void __timer2ISRHook(void) {
// 	count++; // a global variable updated by ISR
// }
// #pragma restore
#ifndef NO_TIMER2_LIB_INTERRUPT
void _tmr2ISR ( void ) interrupt 5 using 1; // DO not call. main() must see this in SDCC
unsigned char tmr2GenInit (unsigned char nms); // General timer 2 init
	// In general use tmr2Init() instead of this.
	// Use tmr2GenInit() instead of tmr2Init() below ONLY if you want to write your own ISR.
	// returns actual interval set
	// for low freq clk, always set to 50 ms fixed regardless
	// for high freq clk, cap to 25ms
	// You MUST overwrite _tmr2ISR() above !

// tmr2Init() below applies to various functions below, it maintains variables to do that
// returns actual interval set
// for low freq clk, always set to 50 ms fixed regardless
// for high freq clk, cap to 25ms
// function check the current clock setting to compute appropriate reload value
unsigned char tmr2Init ( unsigned char msintv );	// This maintains all other tmr2... functions
	// msintv is the interval of tmr2 interrupts determining the resolution of time
	// in ms. It is capped to 25 ms
	// IT INTENDS TO WORK FOR ALL INTERNAL CLOCK FREQUENCIES of F330 and F340
	// e.g. for F330 we have 24.5MHz /8 /4 /2 or /1 and prescaled by 12
	// for 80KHz/8 /4 ... clock is prescaled by 1
void tmr2Init2 ( unsigned char msintv, unsigned int reloadValue );
	// user is responsible to provide info above correctly
	// NO CHECKING is done
	// msintv - interval in ms
	// reloadValue - 16-bit reload value (e.g. if overflow at 2000 interval, pass -2000)
// #define T_CLK 24500000L // INTERNAL CLK in Hertz
// #define T_DIVIDE 8 // SYSCLK is 24500000/8
// #define RLVALUE(n) (-(((((T_CLK/10)/T_DIVIDE)/12)*(n))/100))
// 		tmr2Init2(4,RLVALUE(4));
unsigned long tmr2Get ( void );	// in ms since tmr2Init(); free-running, wrap around at 32-bit
	// resolution determined by msintv in tmr2Init()
// Two functions below ONLY allow ONE task to do a delay while able to do something else.
// Reason: there is only 1 set of variables used by tmr2DlySet() and tmr2DlyExpried()
unsigned char tmr2TickGet ( void ); // get internal count that is incremented at ever tmr2 interrupt
void tmr2DlyInit( void ); // Internal init, Not crucial
void tmr2DlySet ( unsigned int nms );	// Setup delay using timer2, it returns immediately
void tmr2DlySetSinceOld ( unsigned int nms );	// same as tmr2DlySet(), but ...
	// but take previous timestamp set by previous tmr2DlySet() or tmr2DlySetSinceOld()
	// purpose is to remove any cummulative error
	// tmr2DlySet() must be called at least once before one can use this function
bit  tmr2DlyExpired ( void );	// 1 if desired nms passed, msintv ms resolution
// Usage example:
// 1. Basic usage:
//	tmr2DlySet(dly); // set it to expire after dly ms
//	while (!tmr2DlyExpired()) {
//		// can do something else here
//	}
// 2. A delay using tmr2DlySet() and tmr2DlyExpired(). It can do other things while waiting for
// delay to expire.
//void tmr2Dly( unsigned int dly ) {
//	tmr2DlySet(dly); // set it to expire after dly ms
//	while (!tmr2DlyExpired()) {
//		// can do something else here
//	}
//}

// Two functions below allows multiple delays simultaneously by letting user provide own
// set of variables (which must "persists" during the delay) to achieve delay.
// That is, these variables are global or staic of tmrDlyGtype
void tmr2DlyGInit ( tmr2DlyGType *p ); // init internal flag, not crucial
void tmr2DlyGSet ( tmr2DlyGType *p, unsigned int nms, unsigned char sinceOld ); // <= 65s !
	// sinceOld == 1 means delay from the last "time stamp"
	// Objective is to achieve accurate cummulative delay from the most recent tmr2DlyGSet();
	// normally the previous delay should have been completed.
	// IMPORTANT: Calling tmr2DlyGSet() the very first time MUST have "sinceOld = 0" or you
	// may get unpredictable delay the first time. In general, use "sinceOld = 0".
bit tmr2DlyGExpired ( tmr2DlyGType *p );	// is the delay done ? 1 for yes

// Example 1: Basic usage
// tmr2DlyGType tmrd1;
//	..
//	tmr2Init(4);	// 4 ms resolution regular interrupt (can be different !)
//	tmr2DlyGSet(&tmrd1, 1000, 0); // first call
//	while (tmr2DlyGExpired(&tmrd1) == 0 ) {
//		doSomethingElse();
//	}
//  .......

// Example: Multiple delays can co-exists (You can have more !)
// ---------- Delay 1 -------
//tmr2DlyGType myTmr1;
//void dlySet1(unsigned int n) {
//	tmr2DlyGSet(&myTmr1, n, 0);
//}
//bit dlyExpired1( void ) {
//	return tmr2DlyGExpired(&myTmr1);
//}

// ---------- Delay 2 -------
//tmr2DlyGType myTmr2;
//void dlySet2(unsigned int n) {
//	tmr2DlyGSet(&myTmr2, n, 0);
//}
//bit dlyExpired2( void ) {
//	return tmr2DlyGExpired(&myTmr2);
//}

// Stop watch implementation derived form basic tmr2Init() ...
// Following functions allow only 1 stop watch as only 1 set of variables are used to
// maintain it
void tmr2SWInit( void ); // internal init, not crucial
void tmr2SWStart( void ); // start and time lapse reset to 0
unsigned long tmr2SWGet( void ); // if not stop, shows time lapse, else previous stop value
unsigned long tmr2SWStop( void ); // stop, keep time lapse & return value

// General Version to allow multiple Stop Watches running simultaneously
// user has to provide own persistent tmr2SWType variable.
// This is the same idea as tmr2DlyG...()
void tmr2SWGInit( tmr2SWType *swd ); // internal init, not crucial
void tmr2SWGStart( tmr2SWType *swd );
unsigned long tmr2SWGGet( tmr2SWType *swd );
unsigned long tmr2SWGStop( tmr2SWType *swd );

// Multiple stop watches below can co-exist (You can have more !)
//--------------------------------
// tmr2SWType swd1;
// void stopW1Start( void ) {
// 	tmr2SWGStart(&swd1);
// }
// unsigned long stopW1Get ( void ) {
// 	return tmr2SWGGet(&swd1);
// }
//--------------------------------
// tmr2SWType swd2;
// void stopW2Start( void ) {
// 	tmr2SWGStart(&swd2);
// }
// unsigned long stopW2Get ( void ) {
// 	return tmr2SWGGet(&swd2);
// }
#endif // #ifndef NO_TIMER2_LIB_INTERRUPT
// ==========================================================================================
// i2c functions
// For different pin assigned to SDA & SCL,
// user is to overwrite these 3 functions i2cDAIn(), i2cDAOut(), i2cCLOut() as below
/*
sbit at (0x82) SDA; // P0.2 for SDCC and new MPI kitset (Oct 07)
sbit at (0x83) SCL; // P0.3
 
bit inSDA ( void ) {
	return (SDA);
}
void outSCL ( void ) {
	SCL = CY;
}
void outSDA ( void ) {
	SDA = CY;
}
*/

bit i2cInit( void );	// default to i2cDelay(0); i.e. fastest
void i2cDelay(unsigned char dly); // 0=>high speed (PCF8474 OK), 1 => slow spd (for MCP23016)
bit i2cSendData( unsigned char addr, unsigned char n, unsigned char *dataptr);	// 1 err, 0 OK
bit i2cRecData( unsigned char addr, unsigned char n, unsigned char *dataptr);	// 1 err, 0 OK
// n is number of bytes in dataptr[] to be sent or received
// ==========================================================================================
unsigned char pcf8574Read( uchar addr ); // CY==0 means OK, CY==1 means error
bit pcf8574Write ( uchar addr, uchar val ); // returns 0 for OK, 1 for error

bit mcp23016SetDir(uchar addr, uint dir); // dir bits 1 for in, returns 0 for OK, 1 for error
unsigned int mcp23016Read( uchar addr ); // CY does not show any error status !!!
bit mcp23016Write ( uchar addr, uint val ); // returns 0 for OK, 1 for error
// ==========================================================================================
// DAC
// IMPORTANT: Pin P0.1 is always used and P0.1 must be set to be "Skipped"
// For C8051F330: IDA0 Enabled, Update on write to IDA0H, 2mA full-scale output current
// For other operating modes .... refer to F330 datasheets
void dacInit ( void );
void dacOut16 ( unsigned int d16 );	// BEWARE ! lower 6 bits not used
void dacOut10 ( unsigned int d16 );	// BEWARE ! only lower 10 bits used
void dacOut8 ( unsigned char d8 );	// lower 2 bit assumed 0
void dacDisable ( void );
// ==========================================================================================
// For C8051F330: Vref is VDD
// IMPORTANT: The pin must be configured for analog in, otherwise it won't work !!!
// Channels:
// P0: 0 - 7 (e.g. P0.5 is channel 5)
// P1: 8 - 15 (e.g. P1.2 is channel 10)
// Singled ended conversion used, i.e. negative is GND
// Polling of AD0INT used
unsigned int adcInit ( unsigned int clkInKHz, unsigned char channel );
	// Channel range: 0 to 15 for F330 .. use names like AP0_0 ....
	// clkInKHz should be below 3000
	// returns actual clk used
unsigned int adcInitT ( unsigned int clkInKHz, unsigned char channel ); // Channel range: 0 to 15
	// same as adcInit() but has 3 clocks for tracking of input
	// use this if channel number (set using adcCh()) is changed very quickly
bit adcCh ( unsigned char ch );	// Use this to change ADC channel
	// if not initialized using adcInitT(), must not sample immediately
	// To allow immediate sampling, initialize using adcinitT()
unsigned int adcChGet ( unsigned char ch );	// Change ADC channel and get a reading
	// takes care of extra tracking time due to immediate change of channel
unsigned int adcGet ( void );	// 16-bit ADC result (Ch determined by adcInit()/adcCh()
void adcDisable ( void );
unsigned int adcTSensorGet ( void ); // Temp sensor on, convert channel 16, sensor off
#define ADC_CH_MAXID 17
#define AP0_0 0
#define AP0_1 1
#define AP0_2 2
#define AP0_3 3
#define AP0_4 4
#define AP0_5 5
#define AP0_6 6
#define AP0_7 7
#define AP1_0 8
#define AP1_1 9
#define AP1_2 10
#define AP1_3 11
#define AP1_4 12
#define AP1_5 13
#define AP1_6 14
#define AP1_7 15
#define AVREF 16	// for NEG
#define ATEMP 16	// for POS
#define AGND 17		// for NEG
#define AVDD 17		// for POS
// ==========================================================================================
// piezo speaker
// Timer 0 is used with INTERRRUPT that toggles the pin
// IMPORTANT: For C8051F330 speaker pin must be set to PUSH-PULL (not open drain)
// For different IO pin, user has to OVERWRITE 2 funcs with correct pin (push-pull) assigned
/*
sbit at (0x87) SPKPIN;
void _spkHigh ( void ) {
	SPKPIN = 1;
}
void _spkLow ( void ) {
	SPKPIN = 0;
}
void _spkOffPinVal ( void ) {
	SPKPIN = 0; // value of the pin when the speaker is off. This may save power ...
}
*/
#ifndef NO_SPK_LIB_INTERRUPT

// The table in the library has 37 notes
void _tmr0ISR ( void ) interrupt 1 using 1; // Do not call. For SDCC, main() to "see"
void spkInit( void );	// these use timer 0 to interrupt and toggle pin to speaker
void spkNoteTable (unsigned int * noteTable); // allow one to change the table
void spkNote ( unsigned char c);	// 0 means silence, 1 to _noteCount (37)
	// Note that note 0 means silence for that duration
void spkNoteF ( unsigned char noteid);
// Above function is similar to spkNote(), however, it computes timer 0
// reload value using spkFreq() rather than take from lookup table
void spkFreq ( unsigned int freq ); // range is 260 to 2100
#define SPK_L_LMT 260
#define SPK_H_LMT 2100
void spkOn ( void );
bit spkOnChk ( void );
void spkOff ( void );
void spkPlayNote ( unsigned char note, unsigned char numOf50msDuration );
// note tables for different SYSCLK frequencies (if spkNote() or spkPlayNote() used ??????
extern code const unsigned int _f330note1 [];	// 24.5MHz / 1
extern code const unsigned int _f330note2 [];	// 24.5MHz / 2
extern code const unsigned int _f330note4 [];	// 24.5MHz / 4
extern code const unsigned int _f330note8 [];	// 24.5MHz / 8, default table for musical notes
// There are 37 notes
extern code const unsigned char  _noteCount;	// # notes available in table, set to 37
// points to timer 0 reload value lookup table
extern code const unsigned int * code _noteTC;	// default pointing to 24.5MHz/8 table
// For different SYSCLK freq, declare your own _noteTC to overwrite the one in library, e.g.
// code const unsigned int * code _noteTC = _noteTC1; // for SYSCLK = 24.5MHz / 1

// IMPORTANT: if spkNoteF() is used instead of spkNote(), _noteTC is not used

// Musical note in DO, RE, ME ...
#define PAUSE 0	// silence
#define LDO 1 // --------------
#define LRE 3
#define LME 5
#define LFA 6
#define LSO 8
#define LLA 10
#define LTE 12
#define NDO 13 // ---------------
#define NRE 15
#define NME 17
#define NFA 18
#define NSO 20
#define NLA 22
#define NTE 24
#define UDO 25 // --------------
#define URE 27
#define UME 29
#define UFA 30
#define USO 32
#define ULA 34
#define UTE 36
#define UUDO 37

// Musical notes in normal C, C# ....
// LCN means lower C normal
// LCS means lower C sharp ...
#define LCN 1 // --------------
#define LCS 2
#define LDN 3
#define LDS 4
#define LEN 5
#define LFN 6
#define LFS 7
#define LGN 8
#define LGS 9
#define LAN 10
#define LAS 11
#define LBN 12
#define NCN 13 // ---------------
#define NCS 14
#define NDN 15
#define NDS 16
#define NEN 17
#define NFN 18
#define NFS 19
#define NGN 20
#define NGS 21
#define NAN 22
#define NAS 23
#define NBN 24
#define UCN 25 // --------------
#define UCS 26
#define UDN 27
#define UDS 28
#define UEN 29
#define UFN 30
#define UFS 31
#define UGN 32
#define UGS 33
#define UAN 34
#define UAS 35
#define UBN 36
#define UUCN 37
#endif // #ifndef NO_SPK_LIB_INTERRUPT

#endif
