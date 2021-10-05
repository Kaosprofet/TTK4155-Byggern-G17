#ifndef SPI_H_
#define SPI_H_

void spi_init(void);
void spi_transfer_command(unsigned char Command);
void spi_transfer_data(unsigned char Data);

#endif