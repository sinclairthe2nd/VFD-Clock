#include <avr/io.h>
#include <util/delay.h>
//Configutations

#define PORT_7_SEGMENT_M PORTD
#define DDR_7_SEGMENT_M DDRD
#define PORT_7_SEGMENT_H PORTC
#define DDR_7_SEGMENT_H DDRC
#define DDR_MULTI DDRB
#define PORTMULTI PORTB

// Defines for the SegmentCases that map the Number to the Output Pins
#define CASE0 0b00000010
#define CASE1 0b11001111
#define CASE2 0b00101001
#define CASE3 0b01001001
#define CASE4 0b11000101
#define CASE5 0b01010001
#define CASE6 0b00010001
#define CASE7 0b11001011
#define CASE8 0b00000000
#define CASE9 0b01000001

int NUMBER_M = 0;
int NUMBER_H = 0;
int NUMBER_S = 0;

void Display(int MULTI_NUMBER_M,int MULTI_NUMBER_H)
{

//This will map the number in NUMBERS to PORT_7_SegmentS very elegantly.
int Segmapper[10] = {CASE0,CASE1,CASE2,CASE3,CASE4,CASE5,CASE6,CASE7,CASE8,CASE9};
PORT_7_SEGMENT_M = Segmapper[MULTI_NUMBER_M];
//This will map the number in NUMBERH to PORT_7_SEGMENTH in the same fashion.
PORT_7_SEGMENT_H = Segmapper[MULTI_NUMBER_H];

}


void Multiplexer(int NUMBER_M, int NUMBER_H)
{
if(NUMBER_M > 9 && NUMBER_H > 9)//Both Numbers are over 10
    {
        for (int a=0; a<10;a++)
            {
            int MULTI_NUMBER_M=NUMBER_M%10; //This extracts the last digit using modulo
            int MULTI_NUMBER_H=NUMBER_H%10; //This extracts the last digit using modulo
            PORTMULTI = (1 << PORTB1)|(0<< PORTB0); // Turn on the first Display
            
            Display(MULTI_NUMBER_M,MULTI_NUMBER_H);
            _delay_ms(1);

            MULTI_NUMBER_M=NUMBER_M/10;  //This Shaves of the last digit of inputnumber
            MULTI_NUMBER_H=NUMBER_H/10;  //This Shaves of the last digit of inputnumber
            PORTMULTI = (1<< PORTB0)|(0<<PORTB1);  // Turn on Second Displayi

            Display(MULTI_NUMBER_M,MULTI_NUMBER_H);
            _delay_ms(1);
            }
    }
else if (NUMBER_M > 9 && NUMBER_H < 10) //Hours are under 10 Minutes are over 10
    {
     for (int a=0; a<10;a++)
            {
            int MULTI_NUMBER_M=NUMBER_M%10; //This extracts the last digit using modulo
            int MULTI_NUMBER_H=NUMBER_H; //Passes through the Number since no Conditioning is needed
            PORTMULTI = (1 << PORTB1)|(0<< PORTB0); // Turn on the first Display
            
            Display(MULTI_NUMBER_M,MULTI_NUMBER_H);
            _delay_ms(1);

            MULTI_NUMBER_M=NUMBER_M/10;  //This Shaves of the last digit of inputnumber
            MULTI_NUMBER_H=NUMBER_H;     //Passes through the Number since no Conditioning is needed
            PORTMULTI = (1<< PORTB0)|(0<<PORTB1);  // Turn on Second Display

            Display(MULTI_NUMBER_M,MULTI_NUMBER_H);
            _delay_ms(1);
            }
    }
else if (NUMBER_M < 10 && NUMBER_H > 9) //Hours are over 10 Minutes are under 10
    {
     for (int a=0; a<10;a++)
            {
            int MULTI_NUMBER_M=NUMBER_H%10; //This extracts the last digit using modulo
            int MULTI_NUMBER_H=NUMBER_M; //Passes through the Number since no Conditioning is needed
            PORTMULTI = (1 << PORTB1)|(0<< PORTB0); // Turn on the first Display
            
            Display(MULTI_NUMBER_M,MULTI_NUMBER_H);
            _delay_ms(1);

            MULTI_NUMBER_M=NUMBER_H/10;  //This Shaves of the last digit of inputnumber
            MULTI_NUMBER_H=NUMBER_M;     //Passes through the Number since no Conditioning is needed
            PORTMULTI = (1<< PORTB0)|(0<<PORTB1);  // Turn on Second Display

            Display(MULTI_NUMBER_M,MULTI_NUMBER_H);
            _delay_ms(1);
            }
    }

else //both are under 10 no Conditioning needed
    {
         for (int a=0; a<10;a++)
            {
            int MULTI_NUMBER_M=NUMBER_H;  //Passes through the Number since no Conditioning is needed
            int MULTI_NUMBER_H=NUMBER_M;  //Passes through the Number since no Conditioning is needed
            PORTMULTI = (1 << PORTB1)|(0<< PORTB0); // Turn on the first Display
            
            Display(MULTI_NUMBER_M,MULTI_NUMBER_H);
            _delay_ms(1);

            MULTI_NUMBER_M=NUMBER_H;    //Passes through the Number since no Conditioning is needed
            MULTI_NUMBER_H=NUMBER_M;    //Passes through the Number since no Conditioning is needed
            PORTMULTI = (1<< PORTB0)|(0<<PORTB1);  // Turn on Second Display

            Display(MULTI_NUMBER_M,MULTI_NUMBER_H);
            _delay_ms(1);
            }
    }
}

void Clocker()
{
NUMBER_M++; //this Increments the Minutes everytime this is called.    
if(NUMBER_M == 60 ) //An hour has 60 minutes.
    {
        NUMBER_M = 0;
        NUMBER_H++;
    }

if(NUMBER_H == 24 ) //Day's over
    {
        NUMBER_H = 0;
    }
}

int main(void)
{
   //Setup
   DDR_7_SEGMENT_M=1;    //All output
   PORT_7_SEGMENT_M=0;   //All segments off
   DDR_7_SEGMENT_H=1;    //All output
   PORT_7_SEGMENT_H=0;   //All segments off
   DDR_MULTI=1;  //All Multiplexing  pins are outputs

   while(1)               //loop forever
   {
     
     Multiplexer(NUMBER_M,NUMBER_H);

     NUMBER_S++; //TESTING This should be an external interupt
     if(NUMBER_S == 60 ) //Once a Minute call " Clocker" 
        {
        Clocker();
        }
     
       
   }
}

