#include "stdafx.h"
#include "EmulationSettings.h"
#include "Console.h"

//Version 0.9.9
uint16_t EmulationSettings::_versionMajor = 0;
uint8_t EmulationSettings::_versionMinor = 9;
uint8_t EmulationSettings::_versionRevision = 9;

double EmulationSettings::GetAspectRatio(std::shared_ptr<Console> console)
{
	switch(_aspectRatio) {
		case VideoAspectRatio::NoStretching: return 0.0;

		case VideoAspectRatio::Auto:
		{
			NesModel model = GetNesModel();
			if(model == NesModel::Auto) {
				model = console->GetModel();
			}
			return (model == NesModel::PAL || model == NesModel::Dendy) ? (9440000.0 / 6384411.0) : (128.0 / 105.0);
		}

		case VideoAspectRatio::NTSC: return 128.0 / 105.0;
		case VideoAspectRatio::PAL: return 9440000.0 / 6384411.0;
		case VideoAspectRatio::Standard: return 4.0 / 3.0;
		case VideoAspectRatio::Widescreen: return 16.0 / 9.0;
		case VideoAspectRatio::StandardS: return 4.0 / 3.0;
		case VideoAspectRatio::WidescreenS: return 16.0 / 9.0;
		case VideoAspectRatio::Custom: return _customAspectRatio;
	}
	return 0.0;
}

void EmulationSettings::InitializeInputDevices(GameInputType inputType, GameSystem system, bool silent)
{
	ControllerType controllers[4] = { ControllerType::StandardController, ControllerType::StandardController, ControllerType::None, ControllerType::None };
	ExpansionPortDevice expDevice = ExpansionPortDevice::None;
	ClearFlags(EmulationFlags::HasFourScore);

	bool isFamicom = (system == GameSystem::Famicom || system == GameSystem::FDS || system == GameSystem::Dendy);

	if(inputType == GameInputType::VsZapper) {
		//VS Duck Hunt, etc. need the zapper in the first port
		controllers[0] = ControllerType::Zapper;
	} else if(inputType == GameInputType::Zapper) {
		if(isFamicom) {
			expDevice = ExpansionPortDevice::Zapper;
		} else {
			controllers[1] = ControllerType::Zapper;
		}
	} else if(inputType == GameInputType::FourScore) {
		SetFlags(EmulationFlags::HasFourScore);
		controllers[2] = controllers[3] = ControllerType::StandardController;
	} else if(inputType == GameInputType::FourPlayerAdapter) {
		SetFlags(EmulationFlags::HasFourScore);
		expDevice = ExpansionPortDevice::FourPlayerAdapter;
		controllers[2] = controllers[3] = ControllerType::StandardController;
	} else if(inputType == GameInputType::ArkanoidControllerFamicom) {
		expDevice = ExpansionPortDevice::ArkanoidController;
	} else if(inputType == GameInputType::ArkanoidControllerNes) {
		controllers[1] = ControllerType::ArkanoidController;
	} else if(inputType == GameInputType::DoubleArkanoidController) {
		controllers[0] = ControllerType::ArkanoidController;
		controllers[1] = ControllerType::ArkanoidController;
	} else if(inputType == GameInputType::OekaKidsTablet) {
		system = GameSystem::Famicom;
		expDevice = ExpansionPortDevice::OekaKidsTablet;
	} else if(inputType == GameInputType::KonamiHyperShot) {
		system = GameSystem::Famicom;
		expDevice = ExpansionPortDevice::KonamiHyperShot;
	} else if(inputType == GameInputType::FamilyBasicKeyboard) {
		system = GameSystem::Famicom;
		expDevice = ExpansionPortDevice::FamilyBasicKeyboard;
	} else if(inputType == GameInputType::PartyTap) {
		system = GameSystem::Famicom;
		expDevice = ExpansionPortDevice::PartyTap;
	} else if(inputType == GameInputType::PachinkoController) {
		system = GameSystem::Famicom;
		expDevice = ExpansionPortDevice::Pachinko;
	} else if(inputType == GameInputType::ExcitingBoxing) {
		system = GameSystem::Famicom;
		expDevice = ExpansionPortDevice::ExcitingBoxing;
	} else if(inputType == GameInputType::SuborKeyboardMouse1) {
		system = GameSystem::Famicom;
		expDevice = ExpansionPortDevice::SuborKeyboard;
		controllers[1] = ControllerType::SuborMouse;
	} else if(inputType == GameInputType::JissenMahjong) {
		system = GameSystem::Famicom;
		expDevice = ExpansionPortDevice::JissenMahjong;
	} else if(inputType == GameInputType::BarcodeBattler) {
		system = GameSystem::Famicom;
		expDevice = ExpansionPortDevice::BarcodeBattler;
	} else if(inputType == GameInputType::BandaiHypershot) {
		system = GameSystem::Famicom;
		expDevice = ExpansionPortDevice::BandaiHyperShot;
	} else if(inputType == GameInputType::BattleBox) {
		system = GameSystem::Famicom;
		expDevice = ExpansionPortDevice::BattleBox;
	} else if(inputType == GameInputType::TurboFile) {
		system = GameSystem::Famicom;
		expDevice = ExpansionPortDevice::AsciiTurboFile;
	} else if(inputType == GameInputType::FamilyTrainerSideA || inputType == GameInputType::FamilyTrainerSideB) {
		system = GameSystem::Famicom;
		expDevice = ExpansionPortDevice::FamilyTrainerMat;
	} else if(inputType == GameInputType::PowerPadSideA || inputType == GameInputType::PowerPadSideB) {
		system = GameSystem::NesNtsc;
		controllers[1] = ControllerType::PowerPad;
	} else if(inputType == GameInputType::SnesControllers) {
		controllers[0] = ControllerType::SnesController;
		controllers[1] = ControllerType::SnesController;
	} else {
	}

	isFamicom = (system == GameSystem::Famicom || system == GameSystem::FDS || system == GameSystem::Dendy);
	SetConsoleType(isFamicom ? ConsoleType::Famicom : ConsoleType::Nes);
	for(int i = 0; i < 4; i++) {
		SetControllerType(i, controllers[i]);
	}
	SetExpansionDevice(expDevice);
}

const vector<string> NesModelNames = {
	"Auto",
	"NTSC",
	"PAL",
	"Dendy"
};

const vector<string> ConsoleTypeNames = {
	"Nes",
	"Famicom",
};

const vector<string> ControllerTypeNames = {
	"None",
	"StandardController",
	"Zapper",
	"ArkanoidController",
	"SnesController",
	"PowerPad",
	"SnesMouse",
	"SuborMouse",
	"VsZapper",
	"VbController",
	"DualController",
};

const vector<string> ExpansionPortDeviceNames = {
	"None",
	"Zapper",
	"FourPlayerAdapter",
	"ArkanoidController",
	"OekaKidsTablet",
	"FamilyTrainerMat",
	"KonamiHyperShot",
	"FamilyBasicKeyboard",
	"PartyTap",
	"Pachinko",
	"ExcitingBoxing",
	"JissenMahjong",
	"SuborKeyboard",
	"BarcodeBattler",
	"HoriTrack",
	"BandaiHyperShot",
	"AsciiTurboFile",
	"BattleBox",
};
