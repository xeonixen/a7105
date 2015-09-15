void ppm_setup()
{
  pinMode(4,INPUT);
TCCR1A = 0;
TCCR1B = (1<<ICNC1 | 1<<ICES1 | 1<<CS11);
TCCR1C=0;
TIMSK1 = 1<<ICIE1;
}

ISR(TIMER1_CAPT_vect)//interrupt. 
{
  uint16_t cap = ICR1;
  TCNT1 = 0;
  rxInt(cap);
}


void rxInt(uint16_t now) {
    now = now / 2;
    static uint8_t chan = 0;
  #if defined(FAILSAFE)
    static uint8_t GoodPulses;
  #endif
  
    if(now>3000) chan = 0;
    else {
      if(900<now && now<2200 && chan<RC_CHANS ) {   //Only if the signal is between these values it is valid, otherwise the failsafe counter should move up
        rcValue[chan] = now;
        #if defined(FAILSAFE)
          if(chan<4 && diff>FAILSAFE_DETECT_TRESHOLD) GoodPulses |= (1<<chan); // if signal is valid - mark channel as OK
          if(GoodPulses==0x0F) {                                               // If first four chanells have good pulses, clear FailSafe counter
            GoodPulses = 0;
            if(failsafeCnt > 20) failsafeCnt -= 20; else failsafeCnt = 0;
          }
        #endif
      }
    chan++;
  }
}
