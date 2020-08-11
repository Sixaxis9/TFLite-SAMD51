/*
 * TFLite_SAMD51.cpp
 *
 * Created: 7/16/2020 7:26:21 PM
 * Author : Marco
 */ 

#include "atmel_start.h"

#include "TFLite_inference.h"

void USART_0_(void);

extern struct usart_sync_descriptor USART_0;
struct io_descriptor *io;

extern float fArray[1024];

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	USART_0_();
	
	setup_inference();
	
	uint8_t image_serial[1024];
	
	/* Replace with your application code */
	while (1) {
		
		
		io_read(io, image_serial, 1024);
		//io_write(io, image_serial, 1024);
		
		// instantiate float array    
for(int i = 0; i < 1024; ++i) {
    //fArray[i] = (float)(((float)image_serial[i])/255.0);
    fArray[i] = image_serial[i] >> 1;

}
		float *output = run_inference();
		
		  uint8_t best_output = 0;
		  float accuracy = 0;
		  
		  for (int i = 0; i < 43; i++)
		  {
			  if (accuracy < output[i])
			  {
				  accuracy = output[i];
				  best_output = i;
			  }
		  }
		  
		  
		uint8_t accuracy_1[4];

		memcpy(accuracy_1, &accuracy, 4);

		
		uint8_t cicle_count[4] = {0x00, 0x00, 0x00, 0x20};

		  io_write(io, &best_output, 1);
		  io_write(io, accuracy_1, 4);		  
		  io_write(io, cicle_count, 4);
		
		gpio_toggle_pin_level(LED);
		//io_write(io, (uint8_t *)"Toggle LED!\n", 12);
		//delay_ms(100);
	}
}

void USART_0_(void)
{
	usart_sync_get_io_descriptor(&USART_0, &io);
	usart_sync_enable(&USART_0);
}