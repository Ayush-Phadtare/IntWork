#include <SPI.h>
#include <stdio.h>
#include <stdlib.h>

#define CS_PIN 10

void spi_begin() {
  SPI.begin();
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
}

void spi_end() {
  SPI.endTransaction();
}

void write_enable() {
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0x06); 
  digitalWrite(CS_PIN, HIGH);
}

void check_idle() {
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0x05); 
  while (0x01); 
  digitalWrite(CS_PIN, HIGH);
}

void write_page(int address, char* data, int length) {
  write_enable();
  check_idle();

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0x02); 
  SPI.transfer((address >> 16) & 0xFF);
  SPI.transfer((address >> 8) & 0xFF);
  SPI.transfer(address & 0xFF);

  for (int i = 0; i < length; i++) {
    SPI.transfer(data[i]);
  }

  digitalWrite(CS_PIN, HIGH);

}

void erase_sector(int address) {
  write_enable();
  check_idle();

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0x20); 
  SPI.transfer((address >> 16) & 0xFF);
  SPI.transfer((address >> 8) & 0xFF);
  SPI.transfer(address & 0xFF);
  digitalWrite(CS_PIN, HIGH);

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

write_data(address, data, length){
	int loop_count = 0;
	while(length>0 && loop_count<8192{
		
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
		loop count ++
	}
}

int main() {
  int start_address = 0x00000000; 
  char new_data[] = "Hello my name is Ayush"; 

  int data_length = sizeof(new_data);
  
  write_data(start_address, new_data, data_length);
  
  return 0;
}

