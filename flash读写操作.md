外部Flash的读写用到的是SPI协议。SPI协议和I2C有相同之处，我们只需要了解其差异，大致地我们就可以编写外部Flash的读写程序了

第一，外部Flash和I2C用到的引脚存在差异。

外部Flash主要用到了这四个引脚：NSS;MOSI;MISO;SCK。

NSS：
