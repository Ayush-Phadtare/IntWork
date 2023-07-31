#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 0x01FFFFFF //256mb memory
char virtual_memory[MEMORY_SIZE];

int cs_pin = 1;

void fill_virtual_memory() {
  for (int i = 0; i < MEMORY_SIZE; i++) {
    virtual_memory[i] = 1;
  }
}

void write_enable() {
  cs_pin = 0;
  printf("CS pin shifted to low: %d\n", cs_pin);
  printf("Shift 0x06 into DI to enable write\n");
  cs_pin = 1;
  printf("CS pin shifted to high: %d\n", cs_pin);
}

void check_idle() {
  cs_pin = 0;
  printf("CS pin shifted to low: %d\n", cs_pin);
  printf("Shift 0x05 into DI to check if system is idle\n");
  printf("Wait till BUSY reg is 0\n");
  cs_pin = 1;
  printf("CS pin shifted to high: %d\n", cs_pin);
}

void write_page(int address, char* data, int length) {
  
  printf("Address to shift: %d\n", address);

  for (int i = 0; i < length; i++) {
    virtual_memory[address + i] = data[i];
  }
}

void erase_sector(int address) {
   for(int i = address; i < 4096; i++){
       virtual_memory[i] = 0;
   }
}

void write_sector(int address, char* data, int length){
	erase_sector(address);

	int loop_count = 0;

	while(length>0 && loop_count<16){

		int bytes_written = 0;


		if(length>256){
			bytes_written = 256;
		}else{
			bytes_written = length;
		}


		write_page(address, data, bytes_written);

		address += 256;
		if(length>256){
			data += bytes_written;
		}else{
		    data += bytes_written-1;
		}
		length -= bytes_written;
		loop_count ++;
	}
}

void write_data(int address, char* data,int length){
	int loop_count = 0;
	while(length>0 && loop_count<8192){

		int bytes_written = 0;

		if(length>4096){
			bytes_written = 4096;
		}else{
			bytes_written = length;
		}

		write_sector(address, data, bytes_written);

		address += 4096;
		if(length>4096){
			data += bytes_written;
		}else{
		    data += bytes_written-1;
		}
		length -= bytes_written;
		loop_count ++;
	}
}

int main() {
  int start_address = 0x00000000;
  
  char new_data[] = "Hello, my name is Ewala. I come from the planet Alawe, in the galaxy Ewala, in the universe Alawe. But the point is I tried. If you think you need to go to the bakery, please do so now, because I do not want to be interrupted while I tell you my story. ...but anyway, I have been thinking about my happiness lately and have come to the conclusion that I am happy. This is annoying. If you saw a chicken as happy as me, I'll replace it, and give you a full refund... no questions asked!But that's not the point. The point is I am battery operated.Well, actually I am not.But that's not the point. What is the point? The point is everyone should have my game and timetable! screamed someone who I subsequently smacked across the face and decapitated before sending him home in a cardboard box.Before I introduce myself, let me share with you a story. A story about me, and how great I am. But first, I think it would be appropriate to introduce myself. I am going to do this soon. So, it all started, in a smelly house, north of Wellington, south of Arizona and east of where you are sitting right now. It was a dark night, with no birds nor chickens, and it was raining a silent rain. There were too many stars to count, and not enough clouds to cover them. I like chickens. But anyway, what was I doing in this smelly house?";


  int data_length = sizeof(new_data);
  write_data(start_address, new_data, data_length);
  
  printf("Virtual memory content after writing:\n");
  for (int i = 0; i < data_length; i++) {
    printf("%c", virtual_memory[start_address + i]);
  }
  printf("\n");

  return 0;
}

