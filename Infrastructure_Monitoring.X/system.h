/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

/* TODO Define system operating frequency */

/* Microcontroller MIPs (FCY) */
#define SYS_FREQ        8000000L
#define FCY             SYS_FREQ/4

/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/

/* Custom oscillator configuration funtions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */

void ConfigureOscillator(void); /* Handles clock switching/osc initialization */
inline void char_received(char ch);

union DWORD_UNION
{
    unsigned char byte[4];
    unsigned short word[2];
    unsigned long dword;
};

extern volatile unsigned short Timer1OfCount;
extern volatile unsigned short Timer3OfCount;
extern volatile unsigned char done;
//extern volatile union DWORD_UNION Timer1OfCountStop;
extern volatile unsigned long Timer1OfCountStop;
extern volatile unsigned long sys_clock;
