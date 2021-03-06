/*
 * init_services.c
 *
 * Created: 23/05/2015 7:37:54 PM
 *  Author: Blue
 */
#include "bluesat_conf.h"
#include "FreeRTOS.h"

 void configure_spi() {
 
     SPI_Device_Memory = (struct spi_device *) pvPortMalloc( sizeof (struct spi_device) );
     SPI_Device_Memory->id = SPI_DEVICE_MEMORY_CHIP_ID;
     
     SPI_Device_CC1120 = (struct spi_device *) pvPortMalloc( sizeof (struct spi_device) );
     SPI_Device_CC1120->id = SPI_DEVICE_CC1120_CHIP_ID;
     
     spi_master_init(SPI0);
     
     spi_master_setup_device(SPI0, SPI_Device_Memory, POLARITY_FLAG, BAUD_RATE, 0);
     spi_master_setup_device(SPI0, SPI_Device_CC1120, POLARITY_FLAG, BAUD_RATE, 0);
     
     spi_enable(SPI0);
   //  spi_enable_interrupt(SPI0, SPI_INTERRUPT_FLAGS);
     
     spi_buffer_index = 0;
 }
 
 void configure_twi( void ) {
     
     twi_master_options_t * p_opt = (twi_master_options_t * ) pvPortMalloc (sizeof(twi_master_options_t));
     p_opt->speed = TWI0_SPEED;
     p_opt->chip = TWI0_CHIP;
          
     twi_master_setup(TWI0, p_opt);
     
    // twi_enable_interrupt(TWI0, TWI0_INTERRUPT_FLAGS);
     
 }
 
 void configure_uart( void ) {
     
     usart_serial_options_t * uart_opt = (usart_serial_options_t *) pvPortMalloc (sizeof(usart_serial_options_t));
     
     uart_opt->baudrate = CONF_UART_BAUDRATE;
     uart_opt->paritytype = UART_MR_PAR_NO;       
     
     usart_serial_init((usart_if)UART, uart_opt);
 }