//ATtiny85 IO settings
#define SDA PB0
#define SCL PB2

//Delay definitions
#define half_delay 0.25   // one-forth of bit period in microseconds 
#define del 0.5           // half the bit period in microseconds
////////FUNCTION//////
void I2CMasterInit();
uint8_t I2CstartTransmission(uint8_t slave_addr);
uint8_t I2CsendByte(uint8_t oneByte);
uint8_t I2CreceiveByte();
void I2CstopTransmission();

void setup() { 
  I2CMasterInit();
}

void loop() {
  //write main code here
}

void I2CMasterInit() {
     // set SDA and SCL to output
     DDRB |= (1 << SDA) | (1 << SCL);
  }
uint8_t I2CstartTransmission(uint8_t slave_addr){
     uint8_t ACK = 0;
     // start condition: a high-to-low SDA transition while SCL is high
     PORTB |= (1 << SDA) | (1 << SCL) ;
     // wait one-forth of the bit period
     _delay_us(half_delay);
     // high-to-low SDA transition
     PORTB &= ~(1 << SDA);
     // wait one-forth of the bit period
     _delay_us(half_delay);
     PORTB &= ~(1 << SCL);

     // transmit slave address MSB first
     for (int i = 0; i < 8; i++){
         // wait before the transition
         _delay_us(half_delay);
         if (slave_addr & (1 << 7)) {
            PORTB |= (1 << SDA);
        } else {
            PORTB &= ~(1 << SDA);
        }
        _delay_us(half_delay);

        PORTB |= (1 << SCL);
        
        _delay_us(del);

        PORTB &= ~(1 << SCL);
        
        // shift data one bit
        slave_addr <<= 1;
      }

      // get acknowledgement
      DDRB &= ~(1 << SDA);        // turn SDA into input
      PORTB &= ~(1 << SDA);       
      _delay_us(del);
      PORTB |= (1 << SCL);
      _delay_us(half_delay);

      // read ACK signal
      if (PINB & (1 << SDA)) {
            ACK = 0;
        }
      else {
            ACK = 1;
        }

      _delay_us(half_delay);
      PORTB &= ~(1 << SCL);
      DDRB |= (1 << SDA);        // turn SDA into output
      PORTB &= ~(1 << SDA);
      
      return ACK;
  }
uint8_t I2CsendByte(uint8_t oneByte){
     uint8_t success = 0;

      PORTB &= ~((1 << SDA) | (1<< SCL));
      
     // transmit slave address MSB first
     for (int i = 0; i < 8; i++){
         // wait before the transition
         _delay_us(half_delay);
         if (oneByte & (1 << 7)) {
            PORTB |= (1 << SDA);
        } else {
            PORTB &= ~(1 << SDA);
        }
        _delay_us(half_delay);

        PORTB |= (1 << SCL);
        
        _delay_us(del);

        PORTB &= ~(1 << SCL);
        
        // shift data one bit
        oneByte <<= 1;
      }

      // get acknowledgement
      DDRB &= ~(1 << SDA);        // turn SDA into input
      PORTB &= ~(1 << SDA);       
      _delay_us(del);
      PORTB |= (1 << SCL);
      _delay_us(half_delay);

      // read ACK signal
      if (PINB & (1 << SDA)) {
            success = 0;
        }
      else {
            success = 1;
        }

      _delay_us(half_delay);
      PORTB &= ~(1 << SCL);
      DDRB |= (1 << SDA);        // turn SDA into output
      PORTB &= ~(1 << SDA); 
      
      return success;
        
  }
uint8_t I2CreceiveByte(){
    uint8_t receivedByte = 0;

    PORTB &= ~(1 << SCL);
    DDRB &= ~(1 << SDA);        // turn SDA into input
    PORTB &= ~(1 << SDA);       

    for (int i = 0; i < 8; ++i) {

        _delay_us(del);

        // shift zero into the receivedByte
        receivedByte <<= 1;

        // set SCK to HIGH
        PORTB |= (1 << SCL);

        _delay_us(half_delay);

        if (PINB & (1 << SDA)) {
            ++receivedByte;
        }

        _delay_us(half_delay);

        // shut down the SCK signal
        PORTB &= ~(1 << SCL);
    }

    //*****************caution*****************
    _delay_us(half_delay);
    
    // change SDA as output
    DDRB |= (1 << SDA);

    //send acknowledgement
    PORTB &= ~(1 << SDA);
    _delay_us(half_delay);   //because of caution

    PORTB |= (1 << SCL);
    _delay_us(del);
    
    PORTB &= ~((1 << SDA)|(1 << SCL));

    // return received byte
    return receivedByte;
  }
void I2CstopTransmission(){
     // stop condition: a low-to-high SDA transition while SCL is high
     PORTB &= ~(1 << SDA);
     PORTB |= (1 << SCL);
     // wait one-forth of the bit period
     _delay_us(half_delay);
     // low-to-high SDA transition
     PORTB |= (1 << SDA);
     // wait for one-forth of the bit period
     _delay_us(half_delay);
  }
