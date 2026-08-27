#include "plugin.hpp"
#include <cmath>

// export RACK_DIR=/Users/n0153m/Documents/Rack-SDK
// echo $RACK_DIR

struct MyModule : Module
 {
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