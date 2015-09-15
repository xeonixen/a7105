static u8 RC_CHANS=8;
static u16 rcValue[8];
static u8 ch1=0x7F,ch2=0x7F,ch3=0x7F,ch4=0x7F;
void setup()
{
  A7105_Setup();
  initialize();
  ppm_setup();
  //Serial.begin(115200);
}


void loop()
{
 // Serial.println(ch1);
  //Serial.println(rcValue[0]);
  ch1=map(rcValue[0],900,2200,1,255);
  ch2=map(rcValue[1],900,2200,1,255);
  ch3=map(rcValue[2],900,2200,1,255);
  ch4=map(rcValue[3],900,2200,1,255);
  lansu_send();
  delay(2);

  
  
}

