#include "plugin.hpp"
#include <cmath>

// export RACK_DIR=/Users/n0153m/Documents/Rack-SDK
// echo $RACK_DIR

struct MyModule : Module
 {
	float phase = 0.0f;
	float intPart = 0.0f;

	float phase1 = 0.f;
	float intPart1 = 0.f;

	float lfo_phase = 0.f;
	float lfo_intPart = 0.f;

	enum ParamIds {
		POT1,
		POT2,
		POT3, 
		POT4, 
		POT5, 
		POT6,
		PARAMS_LEN
	};
	enum InputIds {
		INPUT1,
		INPUT2,
		INPUT3,
		INPUTS_LEN
	};
	enum OutputIds {
		OUTPUT1,
		OUTPUT2,
		OUTPUT3,
		OUTPUT4,
		OUTPUT5,
		OUTPUT6,
		OUTPUTS_LEN
	};
	enum LightIds {
		BLINK_LIGHT,
		LIGHTS_LEN
	};	

	MyModule(){
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(POT1, -3.0f, 1.0f, 0.0f,"Pitch");
	}

	void process(const ProcessArgs& args) override {
		//Get Pot's value -> Potentiometer
	

		// 1. params 是一個數組
		// 2. params [POT2} 代表取數組裡面的元素](他是一個抽象物件，裡面很有多成員、組件)
		// 3. params [POT2].getValue() 這個組件，裡面有函數成員叫
		// 4. 將返回的旋鈕的值，賦予
		// pitch += inputs[INPUT1].getVoltage();    
		
		// 根據 1V/Oct 標準計算頻率 (假設基礎頻率為 261.6256f 也就是中央 C)	
		float pitch = params[POT1].getValue();
		float lfo_pitch = params[POT2].getValue();
		float pitch3 = params[POT3].getValue();
		float freq = dsp::FREQ_C4 * std::pow(2.f, pitch);//pitch to frequence -> mtof
		

		//AM Amplitude Modulation
		lfo_phase = lfo_phase + 20.f*lfo_pitch*args.sampleTime;
		lfo_phase = std::modf(lfo_phase, &lfo_intPart);
		float lfo = std::sin(2.f * M_PI * lfo_phase);

		//FM Frequency Modulation
		phase = phase + 200.f*pitch3*args.sampleTime;
		phase = std::modf(phase, &intPart);
		float fm = std::sin(2.f * M_PI * phase);

		phase1 = phase1 + (freq+ fm*100) * args.sampleTime;
		phase1 = std::modf(phase1, &intPart1);//take fractional parts
		float sine = std::sin(2.f * M_PI * phase1);

		outputs[OUTPUT1].setVoltage(phase1);
		outputs[OUTPUT2].setVoltage(sine);
		outputs[OUTPUT3].setVoltage(sine*lfo);
		//更新 phase
		// phase += freq * args.sampleTime;
		// if (phase >= 1.0f) {
			// phase -= 1.0f;
		// }

		// 2.Fourier series(additive synthesis) to generate waveforms
		// /float sawtooth = 0.0f;
		// float triangle = 0.0f;
		// float rectangle = 0.0f;

		// constexpr int NUM_HARMONICS =15;

		// for(int n=1; n<=NUM_HARMONICS; ++n){
		// 	float harmoniPhase =2.f*M_PI*phase*n;
		// 	float sinVal = std::sin(harmoniPhase);

			//Sawtooth wave: Fourier series for sawtooth wave is given by the sum of sine waves with decreasing amplitude.
			// sawtooth += sinVal/n;

			//Rectangle wave: Fourier series for rectangle wave is given by the sum of odd harmonics with decreasing amplitude.
			//Triangle wave: Fourier series for triangle wave is given by the sum of odd harmonics with decreasing amplitude, but with alternating signs.
		// 	if(n%2 != 0){
		// 		rectangle += sinVal/n;
		// 		float sign = (((n-1)/2)%2 == 0) ? 1.f : -1.f;
		// 		triangle += sign*sinVal/(n*n);
		// 	}
		// }

		// sawtooth *= (2.0f /M_PI);
		// rectangle *= (4.0f /M_PI);
		// triangle *= (8.0f / (M_PI*M_PI));

		// 3.輸出電壓
		// outputs[MyModule::OUTPUT1].setVoltage(sawtooth*5.0f); // Scale to 5V peak
		// outputs[MyModule::OUTPUT2].setVoltage(triangle*5.0f); // Scale to 5V peak
		// outputs[MyModule::OUTPUT3].setVoltage(rectangle*5.0f); // Scale to 5V peak
	}
};


//--UI 面板設計--
struct MyModuleWidget : ModuleWidget {
	MyModuleWidget(MyModule* module) {
		setModule(module);

		//載入 SVG
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/MyModule.svg")));

		//固定螺絲孔位置
		// addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		// addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		// addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		// addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		//使用 mm2座標來精準對其面板
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(5.24, 46.063)), module, MyModule::POT1));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.24, 46.063)), module, MyModule::POT2));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(25.24, 46.063)), module, MyModule::POT3));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(5.24, 61.063)), module, MyModule::POT4));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.24, 61.063)), module, MyModule::POT5));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(25.24, 61.063)), module, MyModule::POT6));


		// 接孔位置對齊面板上對應的文字標籤 (每個標籤基準線在接孔中心下方 7mm)
		addInput(createInput<PJ301MPort>(mm2px(Vec(5.24, 77.478)), module, MyModule::INPUT1));
		addInput(createInput<PJ301MPort>(mm2px(Vec(15.24, 77.478)), module, MyModule::INPUT2));
		addInput(createInput<PJ301MPort>(mm2px(Vec(25.24, 77.478)), module, MyModule::INPUT3));

		addOutput(createOutput<PJ301MPort>(mm2px(Vec(5.24, 100)), module, MyModule::OUTPUT1));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(15.24, 100)), module, MyModule::OUTPUT2));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(25.24, 100)), module, MyModule::OUTPUT3));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(5.24, 110)), module, MyModule::OUTPUT4));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(15.24, 110)), module, MyModule::OUTPUT5));
		addOutput(createOutput<PJ301MPort>(mm2px(Vec(25.24, 110)), module, MyModule::OUTPUT6));



		// 燈原本放在 130mm，但面板高度只有 128.5mm，已超出面板底部；移到面板上方空位 (對齊 SVG 參考圖層的紫色引導點)
		addChild(createLight<MediumLight<RedLight>>(mm2px(Vec(15.24, 25.81)), module, MyModule::BLINK_LIGHT));
	}
};
Model* modelMyModule = createModel<MyModule, MyModuleWidget>("MyModule");