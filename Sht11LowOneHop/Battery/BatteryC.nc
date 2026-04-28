#include "Battery.h"

module BatteryC {
  provides {
    interface Battery;
  }
}

implementation {

	void brief_pause(register unsigned int n)
	{
		asm volatile(	"1: \n\t"
		"dec	%0 \n\t"
		"jne	1b\n\t"
		:  "+r" (n));
	}
#ifdef aaa
	;
#else
	command uint16_t Battery.getVoltage() 
	{
		uint16_t volt;
		atomic {
    // Turn on and set up ADC12 with REF_1_5V
    ADC12CTL0 = ADC12ON | SHT0_2 | REFON;
    // Use sampling timer
    ADC12CTL1 = SHP;
    // Set up to sample voltage
    ADC12MCTL0 = EOS | SREF_1 | INCH_11;
    // Delay for reference start-up
    TOSH_uwait( 0x3600 ); // TODO: reduce later to save power

    // Enable conversions
    ADC12CTL0 |= ENC;
    // Start conversion
    ADC12CTL0 |= ADC12SC;
    // Wait for completion
    while ((ADC12IFG & BIT0) == 0);

    // Turn off ADC12
    ADC12CTL0 &= ~ENC;
    ADC12CTL0 = 0;

    // JYC: raw data is in ADC12MEM0
    volt = (uint16_t) (((float) ADC12MEM0 / 4096) * 3 * 1000);
		}
		return volt;
  }
#endif
}
