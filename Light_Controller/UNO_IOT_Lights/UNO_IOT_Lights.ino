#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads(0x48);

int16_t adc0;
float multiplier = 0.0078125F;
float sensorTotal;
float sensorAverage;
float voltage;
float current;

bool Ready = false;
#define NUM_RGB       (60)         
#define NUM_BYTES     (NUM_RGB*3) 
#define DIGITAL_PIN   (8)         
#define PORT          (PORTB)     
#define PORT_PIN      (PORTB0)    

#define NUM_BITS      (8) 

uint8_t* rgb_arr = NULL;
uint32_t t_f;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  SPI.begin();
  ads.setGain(GAIN_SIXTEEN);
  ads.begin();
  lcd.begin();

  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  pinMode(DIGITAL_PIN, OUTPUT);
  digitalWrite(DIGITAL_PIN, 0);
  
  if((rgb_arr = (uint8_t *)malloc(NUM_BYTES))) {                 
    memset(rgb_arr, 0, NUM_BYTES);                         
  }

  for (int i = 0; i < 60; i++) {
    setRGB(i, 150, 0, 0);
  }

  render();

  delay(1000);

  for (int i = 0; i < 60; i++) {
    setRGB(i, 125, 125, 0);
  }

  render();

  delay(1000);

  for (int i = 0; i < 60; i++) {
    setRGB(i, 0, 150, 0);
  }

  render();

  delay(1000);

  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));

  attachInterrupt(digitalPinToInterrupt(2), itsTime, FALLING);

  digitalWrite(4, HIGH);
}

void loop() {
  uint8_t buf[180];

  if (Ready == true) {
    digitalWrite(10, LOW);
    SPI.transfer(0x03);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    for (int i = 0; i < 180; i++) {
      buf[i] = SPI.transfer(0);
    }
    digitalWrite(10, HIGH);
  
    for (int i = 0; i < 60; i++) {
      setRGB(i, buf[i], buf[i + 60], buf[i + 120]);
    } 
    
    render();
    
    Ready = false;
    
    voltage = (ads.readADC_Differential_0_1() * multiplier);
    current = voltage / 10;
  
    lcd.clear();
    
    lcd.setCursor(0, 1);
    lcd.print("Current=");
    lcd.setCursor(8, 1);
    lcd.print(current);
    lcd.setCursor(14, 1);
    lcd.print(" A");
    
    digitalWrite(4, HIGH);
  }

}

void itsTime() {
  digitalWrite(4, LOW);
  Ready = true;
}

void setRGB(uint16_t idx, uint8_t r, uint8_t g, uint8_t b) 
{
  if(idx < NUM_RGB) 
  {
    uint8_t *p = &rgb_arr[idx*3]; 
    *p++ = g;  
    *p++ = r;
    *p = b;
  }
}

void render(void) 
{
  if(!rgb_arr) return;

  while((micros() - t_f) < 50L);

  noInterrupts();
  volatile uint8_t  
   *p    = rgb_arr,  
    val  = *p++,      
    high = PORT |  _BV(PORT_PIN), 
    low  = PORT & ~_BV(PORT_PIN), 
    tmp  = low,       
    nbits= NUM_BITS;  
  volatile uint16_t
    nbytes = NUM_BYTES; 
  asm volatile(
   "nextbit:\n\t"         
    "sbi  %0, %1\n\t"     
    "sbrc %4, 7\n\t"               
     "mov  %6, %3\n\t"    
    "dec  %5\n\t"        
    "nop\n\t"            
    "st   %a2, %6\n\t"    
    "mov  %6, %7\n\t"    
    "breq nextbyte\n\t"                  
    "rol  %4\n\t"        
    "rjmp .+0\n\t"        
    "cbi   %0, %1\n\t"    
    "rjmp .+0\n\t"        
    "nop\n\t"             
    "rjmp nextbit\n\t"    
   "nextbyte:\n\t"                          
    "ldi  %5, 8\n\t"      
    "ld   %4, %a8+\n\t"   
    "cbi   %0, %1\n\t"    
    "rjmp .+0\n\t"        
    "nop\n\t"             
    "dec %9\n\t"          
    "brne nextbit\n\t"    
    ::
    "I" (_SFR_IO_ADDR(PORT)), 
    "I" (PORT_PIN),           
    "e" (&PORT),              
    "r" (high),             
    "r" (val),                
    "r" (nbits),             
    "r" (tmp),                
    "r" (low),               
    "e" (p),                 
    "w" (nbytes)              
  );
  interrupts();                         
  t_f = micros();                 
}
