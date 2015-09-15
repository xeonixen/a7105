/*
 This project is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Deviation is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Deviation.  If not, see <http://www.gnu.org/licenses/>.
 */

//#include "interface.h"
//#include "mixer.h"
//#include "config/model.h"
//#include <string.h>
//#include <stdlib.h>
#include <Arduino.h>
//#include "config.h"
#include "a7105.h"

//volatile s16 Channels[NUM_OUT_CHANNELS];
u8 channel_lansu_nr=0;
u8 packet[16];
u8 channel;
const u8 allowed_ch[] = {0x14, 0x1e, 0x28, 0x32, 0x3c, 0x46, 0x50, 0x5a, 0x64, 0x6e, 0x78, 0x82};
unsigned long sessionid;
//txid = const unsigned long txid = 0xF73F25A1;

const unsigned long txid = 0xF73F25A1;
u8 state;

#define WAIT_WRITE 0x80

int hubsan_init()
{
    u8 if_calibration1;
    u8 vco_calibration0;
    u8 vco_calibration1;
    //u8 vco_current;

    A7105_WriteID(txid);
    A7105_WriteReg(A7105_01_MODE_CONTROL, 0x42);
    A7105_WriteReg(A7105_03_FIFOI, 0x07);
    A7105_WriteReg(A7105_04_FIFOII, 0xC0);
    A7105_WriteReg(A7105_07_RC_OSC_I,0x00);
    A7105_WriteReg(A7105_08_RC_OSC_II,0x00);
    A7105_WriteReg(A7105_09_RC_OSC_III,0x00);
    A7105_WriteReg(A7105_0B_GPIO1_PIN1,0x01);
    A7105_WriteReg(A7105_0C_GPIO2_PIN_II,0x01);        
    A7105_WriteReg(A7105_0D_CLOCK, 0x05);
    A7105_WriteReg(A7105_0E_DATA_RATE, 0x04);
    A7105_WriteReg(A7105_0F_PLL_I,0x50);
    A7105_WriteReg(A7105_10_PLL_II,0x9E);
    A7105_WriteReg(A7105_11_PLL_III,0x4B);
    A7105_WriteReg(A7105_12_PLL_IV,0x00);
    A7105_WriteReg(A7105_13_PLL_V,0x02);
    A7105_WriteReg(A7105_14_TX_I,0x16);
    A7105_WriteReg(A7105_15_TX_II, 0x2b);
    A7105_WriteReg(A7105_16_DELAY_I,0x12);
    A7105_WriteReg(A7105_17_DELAY_II,0x00);
    A7105_WriteReg(A7105_18_RX, 0x72);
    A7105_WriteReg(A7105_19_RX_GAIN_I, 0x80);
    A7105_WriteReg(A7105_1A_RX_GAIN_II, 0x80);
    A7105_WriteReg(A7105_1B_RX_GAIN_III, 0x00);   
    A7105_WriteReg(A7105_1C_RX_GAIN_IV, 0x0A);
    A7105_WriteReg(A7105_1D_RSSI_THOLD, 0x32);
    A7105_WriteReg(A7105_1E_ADC, 0xC3);
    A7105_WriteReg(A7105_1F_CODE_I, 0x2F);
    A7105_WriteReg(A7105_20_CODE_II, 0x13);
    A7105_WriteReg(A7105_21_CODE_III, 0xF7);
    A7105_WriteReg(A7105_22_IF_CALIB_I,0x00);
    A7105_WriteReg(A7105_23_IF_CALIB_II, 0x00);
    A7105_WriteReg(A7105_24_VCO_CURCAL, 0x00);
    A7105_WriteReg(A7105_25_VCO_SBCAL_I, 0x00);
    A7105_WriteReg(A7105_26_VCO_SBCAL_II, 0x3A);
    A7105_WriteReg(A7105_27_BATTERY_DET, 0x00);
    A7105_WriteReg(A7105_28_TX_TEST, 0x70);
    A7105_WriteReg(A7105_29_RX_DEM_TEST_I, 0x27);
    A7105_WriteReg(A7105_29_RX_DEM_TEST_I, 0x27);
    A7105_WriteReg(A7105_2B_CPC, 0x03);
    A7105_WriteReg(A7105_2C_XTAL_TEST, 0x01);
    A7105_WriteReg(A7105_2D_PLL_TEST, 0x45);
    A7105_WriteReg(A7105_2E_VCO_TEST_I, 0x18);
    A7105_WriteReg(A7105_2F_VCO_TEST_II, 0x00);
    A7105_WriteReg(A7105_30_IFAT, 0x01);
    A7105_WriteReg(A7105_31_RSCALE, 0x0F);
    A7105_WriteReg(A7105_32_FILTER_TEST, 0x00);
    A7105_WriteReg(0x0F,0x00);
    A7105_WriteReg(0x02,0x01);
    A7105_WriteReg(0x42,0x00);
    A7105_WriteReg(0x02,0x02);
    A7105_WriteReg(0x42,0x00);
    //A7105_WriteReg(0x0F,0x2D);
    return 1;
}

static void initialize() {
    while(1) {
        A7105_Reset();
        if (hubsan_init())
            break;
    }

}

static void lansu_send() {
  if(channel_lansu_nr>60)channel_lansu_nr=0;
  //for(int i=0;i<4;i++)packet[i]=0x00;
  packet[0]=ch1;
  packet[1]=ch2;
  packet[2]=ch3;//steering;
  packet[3]=ch4;
  packet[4]=0x7F;
  packet[5]=0x7F;
  packet[6]=0x7F;
  packet[7]=0x7F;
  
  //void A7105_WriteData(u8 *dpbuffer, u8 len, u8 channel)
  A7105_WriteData(packet,8,channels_lansu[channel_lansu_nr]);
  //write data
  channel_lansu_nr++;
  }



