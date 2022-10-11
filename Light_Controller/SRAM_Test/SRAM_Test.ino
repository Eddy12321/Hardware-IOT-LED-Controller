#include <SPI.h>

bool Avail = false;
bool Recv = false;
bool Ready = true;
bool Print = false;

void setup() {
  SPI.begin();

  Serial.begin(115200);

  pinMode(3, INPUT);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(6, INPUT);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  delay(5000);

  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
}

void loop() {
  uint8_t buf[60];
    
  if (Ready == true) {
    CheckAvail();
  }

  if (Avail == true) {
    Avail = false;
    digitalWrite(10, LOW);
    SPI.transfer(0x03);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    for (int i = 0; i < 60; i++) {
      buf[i] = SPI.transfer(0);
    }
    digitalWrite(10, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    Recv = true;
  }

  if (Recv == true) {
    CheckRecv();
  }
  
  if (Print == true) {
    Print = false;
    Serial.println("Reds");
    for (int i = 0; i < 20; i++) {
      Serial.println(buf[i]);
    }
    Serial.println("Greens");
    for (int i = 20; i < 40; i++) {
      Serial.println(buf[i]);
    }
    Serial.println("Blues");
    for (int i = 40; i < 60; i++) {
      Serial.println(buf[i]);
    }
    Ready = true;
  }
}

void CheckAvail() {
  if (digitalRead(6)) {
    return;
  }
  Ready = false;
  digitalWrite(4, HIGH);
  Avail = true;
}

void CheckRecv() {
  if (!digitalRead(3)) {
    return;
  }
  digitalWrite(5, HIGH);
  Recv = false;
  Print = true;
}

/*
 * 
 * 
 * 
#define NUM_RGB       (580)         
#define NUM_BYTES     (NUM_RGB*3) 
#define DIGITAL_PIN   (8)         
#define PORT          (PORTB)     
#define PORT_PIN      (PORTB0)    

#define NUM_BITS      (8) 

uint8_t* rgb_arr = NULL;
uint32_t t_f;

void setup() {

  pinMode(DIGITAL_PIN, OUTPUT);
  digitalWrite(DIGITAL_PIN, 0);
  
  if((rgb_arr = (uint8_t *)malloc(NUM_BYTES))) {                 
    memset(rgb_arr, 0, NUM_BYTES);                         
  }

  for (int i = 0; i < 580; i++) {
    setRGB(i, 0, 0, 0);
  }

  render();
}

void loop() {
  for (int i = 0; i < 580; i++) {
    setRGB(i, buf[i], buf[i + 580], buf[i + 1160]);
  }
    
  render();
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
*
*
*
*/
