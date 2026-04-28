
#include "Msp430Adc12.h"

module IlluAdcP {
  provides interface AdcConfigure<const msp430adc12_channel_config_t*>;
}
implementation {

  const msp430adc12_channel_config_t config = {
      //inch: INPUT_CHANNEL_A5,
#ifdef TH20
      inch: INPUT_CHANNEL_A1,
#else
      inch: INPUT_CHANNEL_A2,
#endif
      sref: REFERENCE_VREFplus_AVss,
      ref2_5v: REFVOLT_LEVEL_1_5,
      adc12ssel: SHT_SOURCE_ACLK,
      adc12div: SHT_CLOCK_DIV_1,
      sht: SAMPLE_HOLD_4_CYCLES,
      sampcon_ssel: SAMPCON_SOURCE_SMCLK,
      sampcon_id: SAMPCON_CLOCK_DIV_1
  };

  async command const msp430adc12_channel_config_t* AdcConfigure.getConfiguration()
  {
    return &config;
  }
}

