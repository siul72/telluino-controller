#include "slic_pcm.h"
#include "sound_data.h"
#include <avr/pgmspace.h> 

ISR(TIMER1_COMPA_vect) {
    if (SlicPcm.sample >= sounddata_length) {
      if (sample == sounddata_length + lastSample) {
        stopPlayback();
    }
    else {
            // Ramp down to zero to reduce the click at the end of playback.
      OCR2A = sounddata_length + lastSample - sample;
    }
    }
    else {
        stream =  pgm_read_byte(&sounddata_data[0]);
        
    }

    ++sample;
}

SlicPcm::SlicPcm()
{
}

SlicPcm::~SlicPcm()
{
}

void SlicPcm::setup()
{
}

void SlicPcm::startPlayback()
{
    pinMode(speakerPin, OUTPUT);

    // Set up Timer 2 to do pulse width modulation on the speaker
    // pin.

    // Use internal clock (datasheet p.160)
    ASSR &= ~(_BV(EXCLK) | _BV(AS2));

    // Set fast PWM mode  (p.157)
    TCCR2A |= _BV(WGM21) | _BV(WGM20);
    TCCR2B &= ~_BV(WGM22);

    // Do non-inverting PWM on pin OC2A (p.155)
    // On the Arduino this is pin 11.
    TCCR2A = (TCCR2A | _BV(COM2A1)) & ~_BV(COM2A0);
    TCCR2A &= ~(_BV(COM2B1) | _BV(COM2B0));

    // No prescaler (p.158)
    TCCR2B = (TCCR2B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);

    // Set initial pulse width to the first sample.
    OCR2A = pgm_read_byte(&sounddata_data[0]);


    // Set up Timer 1 to send a sample every interrupt.

    cli();

    // Set CTC mode (Clear Timer on Compare Match) (p.133)
    // Have to set OCR1A *after*, otherwise it gets reset to 0!
    TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
    TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));

    // No prescaler (p.134)
    TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);

    // Set the compare register (OCR1A).
    // OCR1A is a 16-bit register, so we have to do this with
    // interrupts disabled to be safe.
    OCR1A = F_CPU / sounddata_sampleRate;    // 16e6 / 8000 = 2000

    // Enable interrupt when TCNT1 == OCR1A (p.136)
    TIMSK1 |= _BV(OCIE1A);

    //lastSample = pgm_read_byte(&sounddata_data[sounddata_length-1]);
    sample = 0;
    sei();
}

void SlicPcm::initPcmCLock()
{


}

void SlicPcm::pipeOutputStream()
{
}

void SlicPcm::pipeInputStream()
{
}
