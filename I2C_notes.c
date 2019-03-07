#define read 1
#define write 0



// clear pa6
//pa9 alt function
//pa10 alt function
//pa9 i2c scl
//pa10 i2c sda
//pa9 open drain
//pa10 open drain

//rccapb1enr1 i2c1 clock enable

//i2c peripheral enable bit mask

//i2c1 timingr fpresc = fi2c/(0+1) = 4mhz
//							scldel = 5 -> 1.5us
//							sdadel = 5 -> 1.5us
//							sclh = 19 -> thigh = 5us
//							scll = 19 -> tlow = 5us

void I2C_Start(/*unsigned long slave address, char size, char direction */) {
	unsigned long tmp  = i2c_cr;
	tmp &= 0xFC009800;//example clear sadd, nbytes, reload, autoend, rdwrn, start, stop
	if(direction == read) {
		tmp |= i2c_cr2_wrn;//definitely setting bit and this is definitely a read
	}
	else
		tmp &= (unsigned long)~i2c_cr2_wrn;	//type cast because data type of (1<<)
																				//data type of mask we do not know^^^
	/* ... */
	tmp |= slave_address << 1; //must shift slave address left 1
	tmp |= (unsigned long)size << 16;
	tmp |= i2c_cr2_start;
	i2c1_cr2 = tmp;
}


__ i2c1_sendData(char slaveaddress, data, char size) {
	I2C_start(slave address, size, write);
	//for each byte
		//wait for TX to be empty
		//i2c_tx_dr = data (use a pointer to element/byte i in array)
	//wait for "transfer complete"
	I2C1_stop();
	return 0;
}



//continued 3/7/19
