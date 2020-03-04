/*
 * Final Project.c
 *
 * Created: 05/20/2019 7:21:47 PM 
 * Author : Janitha Missaka
 */ 

#define F_CPU 1000000UL 

// LCD PIN Declaration
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7 
#define RS eS_PORTC6
#define EN eS_PORTC7


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "lcd.h"


void vac();
void liq();
void run1();
void run2();
void run3();
void run();
void test();
void r2check();


int main(void)
{
    DDRB = 0b11100000; 
	PORTB |= 10;
	DDRA = 0b00000000;  
	DDRC = 0xFF;  
	DDRD = 0xFF;

	
	Lcd4_Init();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Welcome");
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("Group No.21");
	_delay_ms(5000);
	
	Lcd4_Init();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("TEBLA");
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("Cleaning table");
	_delay_ms(4000);
	
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("To Start Enter");
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("Push Button");
	_delay_ms(1000);
	
	
    while(1)  //PBO for Push button
    {
		if(PINB & (1<<PB0)){
			while(PINB & (1<<PB0)); //Release for start
			run(); 
		}
    }
	
}

void run(){
	
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Arm Up");
	PORTB |= (1 << PB1); 	//Pull up the PB1
		
	PORTB |= (1<<PB6);   //PB6 Arm up signal
	while(!((PINB & (1<<PB1)) == 0));  //PB1 for limit switch
	PORTB &= ~(1<<PB6);  
	
	_delay_ms(2000);
	
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Rotate arm");
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("Forward");
	
	
	PORTD |= (1<<PD2);   //PD2 for arm forward
	
	_delay_ms(200);
	liq();
	
	
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Rotate arm");
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("Forward");
	
	PORTB |= (1 << PB3);  //Pull up the PB3
	
	while(!((PINB & (1<<PB3)) == 0));    //PB3 limit switch with IR
	PORTD &= ~(1<<PD2);
	
	
	_delay_ms(2000);
	
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Rotate arm");
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("Reverse");
	
	
	PORTC |= (1<<PC1);
	
	_delay_ms(1000);
	
	vac();
	
	_delay_ms(1000);
	
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Rotate arm");
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("Reverse");
	
	while(!((PINB & (1<<PB3)) == 0)); //PB3 IR for limit
	PORTC &= ~(1<<PC1);
	
	_delay_ms(1000);
		
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Arm Down");
	
	PORTB |= (1 << PB4); //Pull up the PB4
	
	 
	PORTB |= (1<<PB7);      //Arm down signal PB7
	while(!((PINB & (1<<PB4)) == 0)); 
	PORTB &= ~(1<<PB7);   
	
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Start Process");
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("Complete");
	
	_delay_ms(3000);
	
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Check Cust");
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("On seats");
	
	
	_delay_ms(4000);
	
	
	
	
	////check IR sensor detection on seats
	while(1){
		
	if((!(PINA & (1<<PA3)) == 1) || (!(PINA & (1<<PA4)) == 1)){   
	
		Lcd4_Clear();
		Lcd4_Set_Cursor(1,0);
		Lcd4_Write_String("Customers on");
		Lcd4_Set_Cursor(2,0);
		Lcd4_Write_String("Seats");
		
		_delay_ms(3000);
		
		Lcd4_Clear();
		Lcd4_Set_Cursor(1,0);
		Lcd4_Write_String("Send count to");
		Lcd4_Set_Cursor(2,0); 
		Lcd4_Write_String("kitchen via WIFI");
		
		_delay_ms(2000);
		
		PORTC |= (1<<PC3);  //Power up the WIFI module
		
		
		
		//PORTC &= ~(1<<PC3); //Power off the WIFI module
		
		_delay_ms(2000);
		
	    Lcd4_Clear();
		Lcd4_Set_Cursor(1,0);
		Lcd4_Write_String("Waiting for");
		Lcd4_Set_Cursor(2,0);
		Lcd4_Write_String("Cust Leave");
		
		_delay_ms(10000);
	
			
		}
	
		

		
	else{
		
			_delay_ms(2000);
			
			Lcd4_Clear();
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String("Customers not");
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String("On seats");
			
			_delay_ms(4000);
			
			run1();
			break;
	}
	}
	
	
	//Check IR sensors on Arm

	_delay_ms(3000);
		
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Check IR");
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("Sensor");
		
	_delay_ms(3000);

	while(1){
		if((!(PINA & (1<<PA1)) == 1) || (!(PINA & (1<<PA2)) == 1) || (!(PINA & (1<<PA6)) == 1) || (!(PINA & (1<<PA7)) == 1)){
			Lcd4_Clear();
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String("Wait For Items");
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String("Remove");
			
			_delay_ms(2000);
			
		}
		else{
			
			
			Lcd4_Clear();
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String("IR Sensor");
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String("Free");
			
			
			_delay_ms(2000);
			
			r2check();
			
			_delay_ms(2000);
			
			run3();
			
			_delay_ms(2000);
			
				
			Lcd4_Clear();
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String("Process");
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String("Complete");
			
			break;
		}
	}	
	
}
	
//arm up
void run1(){  	
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Arm Up");
	
	PORTB |= (1 << PB1); 
	
	PORTB |= (1<<PB6);   
	while(!((PINB & (1<<PB1)) == 0));  //PA0 for arm up
	PORTB &= ~(1<<PB6);  
	
	_delay_ms(2000);
}

//rotate arm
void run2(){
	
			Lcd4_Clear();
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String("Rotate arm");
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String("Forward");
			
			
			PORTD |= (1<<PD2);   //PD2 for arm forward
			
			_delay_ms(3000);
			liq();
			
			
			Lcd4_Clear();
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String("Rotate arm");
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String("Forward");
			
			PORTB |= (1 << PB3);
			
			while(!((PINB & (1<<PB3)) == 0));    //PB2 limit switch with IR
			PORTD &= ~(1<<PD2);
			
			
			_delay_ms(2000);
			
			Lcd4_Clear();
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String("Rotate arm");
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String("Reverse");
			
			
			PORTC |= (1<<PC1);
			
			_delay_ms(1000);
			
			vac();
			
			_delay_ms(1000);
			
			Lcd4_Clear();
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String("Rotate arm");
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String("Reverse");
			
			while(!((PINB & (1<<PB3)) == 0)); //PB3 limit switch with previous IR
			PORTC &= ~(1<<PC1);
			
			_delay_ms(1000);
}

//arm down
 void run3(){
	
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Arm Down");
	
	PORTB |= (1 << PB4); 
	
	
	PORTB |= (1<<PB7);      //Arm down 
	while(!((PINB & (1<<PB4)) == 0));
	PORTB &= ~(1<<PB7);   
	_delay_ms(1000);
}    //arm down           

//liq drop
void  liq(){
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("liquid drop");
	
	
	PORTD |= (1 << PD3);
	_delay_ms(1000);
	PORTD &= ~(1 << PD3);
}  

// vaccumming
void vac(){
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Drying");
	
	
	PORTC |= (1 << PC0);
	_delay_ms(1500);
	PORTC &= ~(1 <<PC0);

}


//check items on the floor
void r2check() {
	
		
		Lcd4_Clear();
		Lcd4_Set_Cursor(1,0); 
		Lcd4_Write_String("Rotate arm");
		Lcd4_Set_Cursor(2,0);
		Lcd4_Write_String("Forward");
		_delay_ms(1500);
		
		
		
		while (1)
		{
			if (((PINB & (1<<PB3)) == 0))
			{
				PORTD &= ~(1<<PD2); 
				break;
			}
			else if ((!(PINA & (1<<PA1)) == 1) || (!(PINA & (1<<PA2)) == 1) || (!(PINA & (1<<PA6)) == 1) || (!(PINA & (1<<PA7)) == 1))
			{
				PORTD &= ~(1<<PD2);
				//_delay_ms(1500);
			}
			else{
				
				PORTD |= (1<<PD2); 
				//_delay_ms(1500);
			}
			
		}
		
		_delay_ms(2000);
		
		Lcd4_Clear();
		Lcd4_Set_Cursor(1,0);
		Lcd4_Write_String("Rotate arm");
		Lcd4_Set_Cursor(2,0);
		Lcd4_Write_String("Reverse");
		
		
		PORTC |= (1<<PC1);
		
		_delay_ms(1000);
		
		vac();
		
		_delay_ms(1000);
		
		Lcd4_Clear();
		Lcd4_Set_Cursor(1,0);
		Lcd4_Write_String("Rotate arm");
		Lcd4_Set_Cursor(2,0);
		Lcd4_Write_String("Reverse");
		
		while(!((PINB & (1<<PB3)) == 0)); //PB3 limit switch with previous IR
		PORTC &= ~(1<<PC1);
		
		_delay_ms(500);
		
		
		
}

 
 



