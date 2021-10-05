#ifndef SPI_H_
#define SPI_H_

void spi_init(void);
void spi_write(unsigned char Data);
char spi_transfer_data(void);

#endif