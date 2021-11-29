#pragma once

#include "NoisePlethoraPlugin.hpp"
#define GRANULAR_MEMORY_SIZE 12800

class grainGlitchIII : public NoisePlethoraPlugin {

public:

	grainGlitchIII()
	//:patchCord2(granular1, 0, waveformMod1, 0),
	// patchCord3(waveformMod1, granular1)
	{ }

	~grainGlitchIII() override {}

	grainGlitchIII(const grainGlitchIII&) = delete;
	grainGlitchIII& operator=(const grainGlitchIII&) = delete;

	void init() override {
		float msec = 150.0;

		waveformMod1.begin(1, 1000, WAVEFORM_SAWTOOTH);
		// the Granular effect requires memory to operate
		granular1.begin(granularMemory, GRANULAR_MEMORY_SIZE);
		granular1.beginFreeze(msec);

	}

	void process(float k1, float k2) override {
		float knob_1 = k1;
		float knob_2 = k2;

		float pitch1 = pow(knob_1, 2);
		// float pitch2 = pow(knob_2, 2);
		float msec2 = 25.0 + (knob_2 * 55.0);
		float ratio;


		waveformMod1.frequencyModulation(knob_2 * 2);
		granular1.beginPitchShift(msec2);
		waveformMod1.frequency(pitch1 * 5000 + 400);
		ratio = powf(2.0, knob_2 * 6.0 - 3.0);
		granular1.setSpeed(ratio);

	}

	float processGraph(float sampleTime) override {
		if (buffer.empty()) {

			granular1.update(&waveformMod1Previous, &granular1Previous);

			waveformMod1.update(&granular1Previous, nullptr, &waveformMod1Previous);

			bufferAlt.pushBuffer(waveformMod1Previous.data, AUDIO_BLOCK_SAMPLES);
			buffer.pushBuffer(granular1Previous.data, AUDIO_BLOCK_SAMPLES);
		}

		altOutput = int16_to_float_5v(bufferAlt.shift()) / 5.f;;

		return int16_to_float_5v(buffer.shift()) / 5.f;;
	}

	AudioStream& getStream() override {
		return granular1;
	}
	unsigned char getPort() override {
		return 0;
	}

private:
	AudioEffectGranular      granular1;      //xy=885.75,359.9999985694885
	AudioSynthWaveformModulated waveformMod1;   //xy=889.75,480.74999871477485
	//AudioConnection          patchCord2;
	//AudioConnection          patchCord3;
	int16_t granularMemory[GRANULAR_MEMORY_SIZE];

	TeensyBuffer buffer, bufferAlt;
	audio_block_t granular1Previous;
	audio_block_t waveformMod1Previous;
};

REGISTER_PLUGIN(grainGlitchIII);
