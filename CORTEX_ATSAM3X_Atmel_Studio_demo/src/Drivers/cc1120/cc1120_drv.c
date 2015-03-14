/*
 * cc1120_drv.c
 *
 * Created: 14-02-2015 4:59:17 PM
 *  Author: Blue
 */
#include <asf.h>
#include <comms_uart_drv.h>
#include <comms_spi_drv.h>
#include "cc1120_drv.h"
#include "comms_drv.h"

uint32_t cc1120_transmit (uint8_t data, uint8_t last) {
	return spi_write(SPI0, (uint16_t)data, SPI_DEVICE_CC1120, last);
}
uint32_t cc1120_receive (uint8_t *data) {
	uint8_t device_id = SPI_DEVICE_CC1120;
	return spi_read(SPI0, (uint16_t*)data, &device_id);
}

uint32_t cc1120_reset () {
	
	// check to see SO go high, and then low again. Keep CSn low. See 3.2.2 of User Guide	
	
	// uint16_t spi_read_data = 0;
	uint32_t timeout = 500;
	cc1120_transmit(CC1120_SRES, 0);
	uint32_t test = pio_get(PIOA, PIO_PA25A_SPI0_MISO, 0);
	
	test = test;
	while (pio_get(PIOA, PIO_PA25A_SPI0_MISO, 0)) {
		cc1120_transmit(CC1120_SNOP, 1);
		if (timeout-- <= 0) {
			// timed out, failed to reset quickly
			return 0;
		} 
	}
	
	return 1;
}

void cc1120_read_register_address_space(uint8_t *data_buffer) {
	uint8_t start_burst_address = CC1120_IOCFG3;
	uint32_t burst_iterations = CC1120_PKT_LEN - CC1120_IOCFG3;		// ranges the whole register space
	cc1120_read_burst_register(start_burst_address, data_buffer, burst_iterations);
}

// Single Register Access
/*
void cc1120_read_single() {
	cc1120_transmit()
}

void cc1120_write_single() {
	
}
*/
// Burst Register access

void cc1120_read_burst_register (uint8_t start_address, uint8_t *data_buffer, uint32_t iterations) {
	
	data_buffer = data_buffer;
	
	cc1120_transmit(start_address | CC1120_READ | CC1120_BURST_MODE, 0);
	
	for (uint32_t y = 0; y < iterations-1; y++) {
		cc1120_transmit(CC1120_SNOP, 0);
		//cc1120_receive(data_buffer+y);
	}
	cc1120_transmit(CC1120_SNOP, 1);
}

void cc1120_write_burst_register (uint8_t start_address, uint8_t *data_buffer, uint32_t iterations) {
	cc1120_transmit(start_address | CC1120_WRITE | CC1120_BURST_MODE, 0);
	for (uint32_t y = 0; y < iterations-1; y++) {
		cc1120_transmit(data_buffer[y], 0);
	}
	cc1120_transmit(data_buffer[iterations-1], 1);
}