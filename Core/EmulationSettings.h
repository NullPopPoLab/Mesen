#pragma once

#include <algorithm>
#include <memory>
#include "stdafx.h"
#include "KeyManager.h"

class Console;

enum EmulationFlags : uint64_t
{
	Paused = 0x01,
	ShowFPS = 0x02,
	VerticalSync = 0x04,
	AllowInvalidInput = 0x08,
	RemoveSpriteLimit = 0x10,
	UseHdPacks = 0x20,
	HasFourScore = 0x40,

	DisableDynamicSampleRate = 0x80,

	PauseOnMovieEnd = 0x0100,

	EnablePpuOamRowCorruption = 0x0200,

	AllowBackgroundInput = 0x0400,
	ReduceSoundInBackground = 0x0800,
	MuteSoundInBackground = 0x1000,

	FdsFastForwardOnLoad = 0x2000,
	FdsAutoLoadDisk = 0x4000,
	Mmc3IrqAltBehavior = 0x8000,
	
	SwapDutyCycles = 0x10000,

	DisableGameSelectionScreen = 0x20000,

	AutoConfigureInput = 0x40000,

	ShowLagCounter = 0x80000,

	SilenceTriangleHighFreq = 0x100000,
	ReduceDmcPopping = 0x200000,

	DisableBackground = 0x400000,
	DisableSprites = 0x800000,
	ForceBackgroundFirstColumn = 0x1000000,
	ForceSpritesFirstColumn = 0x2000000,
	DisablePpu2004Reads = 0x4000000,
	DisableNoiseModeFlag = 0x8000000,
	DisablePaletteRead = 0x10000000,
	DisableOamAddrBug = 0x20000000,
	DisablePpuReset = 0x40000000,
	EnableOamDecay = 0x80000000,

	UseNes101Hvc101Behavior = 0x100000000,
	ShowFrameCounter = 0x200000000,
	ShowGameTimer = 0x400000000,

	FdsAutoInsertDisk = 0x800000000,

	Rewind =  0x1000000000,
	Turbo = 0x2000000000,
	InBackground = 0x4000000000,
	
	DisplayMovieIcons = 0x10000000000,
	HidePauseOverlay = 0x20000000000,

	UseCustomVsPalette = 0x40000000000,

	AdaptiveSpriteLimit = 0x80000000000,

	EnablePpu2006ScrollGlitch = 0x100000000000,
	EnablePpu2000ScrollGlitch = 0x200000000000,

	ConfirmExitResetPower = 0x400000000000,

	IntegerFpsMode = 0x2000000000000,

	BreakOnCrash = 0x8000000000000,
	
	AllowMismatchingSaveState = 0x10000000000000,

	RandomizeMapperPowerOnState = 0x20000000000000,
	
	UseHighResolutionTimer = 0x40000000000000,
	DisplayDebugInfo = 0x80000000000000,

	ReduceSoundInFastForward = 0x100000000000000,

	VsDualMuteMaster = 0x200000000000000,
	VsDualMuteSlave = 0x400000000000000,
	
	RandomizeCpuPpuAlignment = 0x800000000000000,
	
	ForceMaxSpeed = 0x4000000000000000,	
	ConsoleMode = 0x8000000000000000,
};

enum class AudioChannel
{
	Square1 = 0,
	Square2 = 1,
	Triangle = 2,
	Noise = 3,
	DMC = 4,
	FDS = 5,
	MMC5 = 6,
	VRC6 = 7,
	VRC7 = 8,
	Namco163 = 9,
	Sunsoft5B = 10
};

enum class EqualizerFilterType
{
	None = 0,
	Butterworth = 1,
	Chebyshev1 = 2,
	Chebyshev2 = 3
};

enum class ScaleFilterType
{
	xBRZ = 0,
	HQX = 1,
	Scale2x = 2,
	_2xSai = 3,
	Super2xSai = 4,
	SuperEagle = 5,
	Prescale = 6,
};

enum class VideoFilterType
{
	None = 0,
	NTSC = 1,
	BisqwitNtscQuarterRes = 2,
	BisqwitNtscHalfRes = 3,
	BisqwitNtsc = 4,
	xBRZ2x = 5,
	xBRZ3x = 6,
	xBRZ4x = 7,
	xBRZ5x = 8,
	xBRZ6x = 9,
	HQ2x = 10,
	HQ3x = 11,
	HQ4x = 12,
	Scale2x = 13,
	Scale3x = 14,
	Scale4x = 15,
	_2xSai = 16,
	Super2xSai = 17,
	SuperEagle = 18,
	Prescale2x = 19,
	Prescale3x = 20,
	Prescale4x = 21,
	Prescale6x = 22,
	Prescale8x = 23,
	Prescale10x = 24,
	Raw = 25,
	HdPack = 999
};

enum class VideoAspectRatio
{
	NoStretching = 0,
	Auto = 1,
	NTSC = 2,
	PAL = 3,
	Standard = 4,
	Widescreen = 5,
	Custom = 6,
	StandardS = 7,
	WidescreenS = 8
};

struct OverscanDimensions
{
	uint32_t Left = 0;
	uint32_t Right = 0;
	uint32_t Top = 0;
	uint32_t Bottom = 0;

	uint32_t GetPixelCount()
	{
		return GetScreenWidth() * GetScreenHeight();
	}

	uint32_t GetScreenWidth()
	{
		return 256 - Left - Right;
	}

	uint32_t GetScreenHeight()
	{
		return 240 - Top - Bottom;
	}
};

struct PictureSettings
{
	double Brightness = 0;
	double Contrast = 0;
	double Saturation = 0;
	double Hue = 0;
	double ScanlineIntensity = 0;
};

struct NtscFilterSettings
{
	double Sharpness = 0;
	double Gamma = 0;
	double Resolution = 0;
	double Artifacts = 0;
	double Fringing = 0;
	double Bleed = 0;
	bool MergeFields = false;
	bool VerticalBlend = false;
	bool KeepVerticalResolution = false;

	double YFilterLength = 0;
	double IFilterLength = 0;
	double QFilterLength = 0;
};

enum class RamPowerOnState
{
	AllZeros = 0,
	AllOnes = 1,
	Random = 2
};

extern const vector<string> NesModelNames;
enum class NesModel
{
	Auto = 0,
	NTSC = 1,
	PAL = 2,
	Dendy = 3,
};

extern const vector<string> ConsoleTypeNames;
enum class ConsoleType
{
	Nes = 0,
	Famicom = 1
};

extern const vector<string> ControllerTypeNames;
enum class ControllerType
{
	None = 0,
	StandardController = 1,
	Zapper = 2,
	ArkanoidController = 3,
	SnesController = 4,
	PowerPad = 5,
	SnesMouse = 6,
	SuborMouse = 7,
	VsZapper = 8,
	VbController = 9,
	DualController = 10,
};

extern const vector<string> ExpansionPortDeviceNames;
enum class ExpansionPortDevice
{
	None = 0,
	Zapper = 1,
	FourPlayerAdapter = 2,
	ArkanoidController = 3,
	OekaKidsTablet = 4,
	FamilyTrainerMat = 5,
	KonamiHyperShot = 6,
	FamilyBasicKeyboard = 7,
	PartyTap = 8,
	Pachinko = 9,
	ExcitingBoxing = 10,
	JissenMahjong = 11,
	SuborKeyboard = 12,
	BarcodeBattler = 13,
	HoriTrack = 14,
	BandaiHyperShot = 15,
	AsciiTurboFile = 16,
	BattleBox = 17,
};

enum class VsInputType
{
	Default = 0,
	SwapControllers = 1,
	SwapAB = 2
};

struct KeyMapping
{
	uint32_t A = 0;
	uint32_t B = 0;
	uint32_t AB = 0;
	uint32_t Up = 0;
	uint32_t Down = 0;
	uint32_t Left = 0;
	uint32_t Right = 0;
	uint32_t Start = 0;
	uint32_t Select = 0;
	uint32_t TurboA = 0;
	uint32_t TurboB = 0;
	uint32_t TurboAB = 0;
	uint32_t TurboStart = 0;
	uint32_t TurboSelect = 0;
	uint32_t Microphone = 0;
	uint32_t LButton = 0;
	uint32_t RButton = 0;

	uint32_t PowerPadButtons[12] = {};
	uint32_t FamilyBasicKeyboardButtons[72] = {};
	uint32_t PartyTapButtons[6] = {};
	uint32_t PachinkoButtons[2] = {};
	uint32_t ExcitingBoxingButtons[8] = {};
	uint32_t JissenMahjongButtons[21] = {};
	uint32_t SuborKeyboardButtons[99] = {};
	uint32_t BandaiMicrophoneButtons[3] = {};
	uint32_t VirtualBoyButtons[14] = {};

	bool HasKeySet()
	{
		if(A || B || AB || Up || Down || Left || Right || Start || Select || TurboA || TurboB || TurboAB || TurboStart || TurboSelect || Microphone || LButton || RButton) {
			return true;
		}

		bool hasKeyBinding = false;
		hasKeyBinding |= HasKeyBinding(PowerPadButtons, sizeof(PowerPadButtons) / sizeof(PowerPadButtons[0]));
		hasKeyBinding |= HasKeyBinding(FamilyBasicKeyboardButtons, sizeof(FamilyBasicKeyboardButtons) / sizeof(FamilyBasicKeyboardButtons[0]));
		hasKeyBinding |= HasKeyBinding(PartyTapButtons, sizeof(PartyTapButtons) / sizeof(PartyTapButtons[0]));
		hasKeyBinding |= HasKeyBinding(PachinkoButtons, sizeof(PachinkoButtons) / sizeof(PachinkoButtons[0]));
		hasKeyBinding |= HasKeyBinding(ExcitingBoxingButtons, sizeof(ExcitingBoxingButtons) / sizeof(ExcitingBoxingButtons[0]));
		hasKeyBinding |= HasKeyBinding(JissenMahjongButtons, sizeof(JissenMahjongButtons) / sizeof(JissenMahjongButtons[0]));
		hasKeyBinding |= HasKeyBinding(SuborKeyboardButtons, sizeof(SuborKeyboardButtons) / sizeof(SuborKeyboardButtons[0]));
		hasKeyBinding |= HasKeyBinding(BandaiMicrophoneButtons, sizeof(BandaiMicrophoneButtons) / sizeof(BandaiMicrophoneButtons[0]));
		hasKeyBinding |= HasKeyBinding(VirtualBoyButtons, sizeof(VirtualBoyButtons) / sizeof(VirtualBoyButtons[0]));
		return hasKeyBinding;
	}

private:
	bool HasKeyBinding(uint32_t* buttons, uint32_t count)
	{
		for(uint32_t i = 0; i < count; i++) {
			if(buttons[i] != 0) {
				return true;
			}
		}
		return false;
	}
};

struct KeyMappingSet
{
	KeyMapping Mapping1;
	KeyMapping Mapping2;
	KeyMapping Mapping3;
	KeyMapping Mapping4;
	uint32_t TurboSpeed = 0;
	bool PowerpadUseSideA = false;

	vector<KeyMapping> GetKeyMappingArray()
	{
		vector<KeyMapping> keyMappings;
		if(Mapping1.HasKeySet()) {
			keyMappings.push_back(Mapping1);
		}
		if(Mapping2.HasKeySet()) {
			keyMappings.push_back(Mapping2);
		}
		if(Mapping3.HasKeySet()) {
			keyMappings.push_back(Mapping3);
		}
		if(Mapping4.HasKeySet()) {
			keyMappings.push_back(Mapping4);
		}
		return keyMappings;
	}
};

enum class MouseDevice
{
	Unknown = 0,
	SnesMouse,
	SuborMouse,
	ArkanoidController,
	HoriTrack
};

enum class StereoFilter
{
	None = 0,
	Delay = 1,
	Panning = 2,
	CombFilter = 3,
};

struct AudioFilterSettings
{
	StereoFilter Filter = StereoFilter::None;
	double Angle = 0;
	int32_t Delay = 0;
	int32_t Strength = 0;

	double ReverbDelay = 0;
	double ReverbStrength = 0;

	int32_t CrossFadeRatio = 0;
};

class EmulationSettings
{
private:
	static uint16_t _versionMajor;
	static uint8_t _versionMinor;
	static uint8_t _versionRevision;

	uint32_t _ppuPaletteArgb[11][64] = {
		/* 2C02 */			{ 0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00, 0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFADADAD, 0xFF155FD9, 0xFF4240FF, 0xFF7527FE, 0xFFA01ACC, 0xFFB71E7B, 0xFFB53120, 0xFF994E00, 0xFF6B6D00, 0xFF388700, 0xFF0C9300, 0xFF008F32, 0xFF007C8D, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFEFF, 0xFF64B0FF, 0xFF9290FF, 0xFFC676FF, 0xFFF36AFF, 0xFFFE6ECC, 0xFFFE8170, 0xFFEA9E22, 0xFFBCBE00, 0xFF88D800, 0xFF5CE430, 0xFF45E082, 0xFF48CDDE, 0xFF4F4F4F, 0xFF000000, 0xFF000000, 0xFFFFFEFF, 0xFFC0DFFF, 0xFFD3D2FF, 0xFFE8C8FF, 0xFFFBC2FF, 0xFFFEC4EA, 0xFFFECCC5, 0xFFF7D8A5, 0xFFE4E594, 0xFFCFEF96, 0xFFBDF4AB, 0xFFB3F3CC, 0xFFB5EBF2, 0xFFB8B8B8, 0xFF000000, 0xFF000000 },
		/* 2C03 */			{ 0xFF6D6D6D, 0xFF002491, 0xFF0000DA, 0xFF6D48DA, 0xFF91006D, 0xFFB6006D, 0xFFB62400, 0xFF914800, 0xFF6D4800, 0xFF244800, 0xFF006D24, 0xFF009100, 0xFF004848, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFB6B6B6, 0xFF006DDA, 0xFF0048FF, 0xFF9100FF, 0xFFB600FF, 0xFFFF0091, 0xFFFF0000, 0xFFDA6D00, 0xFF916D00, 0xFF249100, 0xFF009100, 0xFF00B66D, 0xFF009191, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFF6DB6FF, 0xFF9191FF, 0xFFDA6DFF, 0xFFFF00FF, 0xFFFF6DFF, 0xFFFF9100, 0xFFFFB600, 0xFFDADA00, 0xFF6DDA00, 0xFF00FF00, 0xFF48FFDA, 0xFF00FFFF, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFFB6DAFF, 0xFFDAB6FF, 0xFFFFB6FF, 0xFFFF91FF, 0xFFFFB6B6, 0xFFFFDA91, 0xFFFFFF48, 0xFFFFFF6D, 0xFFB6FF48, 0xFF91FF6D, 0xFF48FFDA, 0xFF91DAFF, 0xFF000000, 0xFF000000, 0xFF000000 },
		/* 2C04-0001 */	{ 0xFFFFB6B6, 0xFFDA6DFF, 0xFFFF0000, 0xFF9191FF, 0xFF009191, 0xFF244800, 0xFF484848, 0xFFFF0091, 0xFFFFFFFF, 0xFF6D6D6D, 0xFFFFB600, 0xFFB6006D, 0xFF91006D, 0xFFDADA00, 0xFF6D4800, 0xFFFFFFFF, 0xFF6DB6FF, 0xFFDAB66D, 0xFF6D2400, 0xFF6DDA00, 0xFF91DAFF, 0xFFDAB6FF, 0xFFFFDA91, 0xFF0048FF, 0xFFFFDA00, 0xFF48FFDA, 0xFF000000, 0xFF480000, 0xFFDADADA, 0xFF919191, 0xFFFF00FF, 0xFF002491, 0xFF00006D, 0xFFB6DAFF, 0xFFFFB6FF, 0xFF00FF00, 0xFF00FFFF, 0xFF004848, 0xFF00B66D, 0xFFB600FF, 0xFF000000, 0xFF914800, 0xFFFF91FF, 0xFFB62400, 0xFF9100FF, 0xFF0000DA, 0xFFFF9100, 0xFF000000, 0xFF000000, 0xFF249100, 0xFFB6B6B6, 0xFF006D24, 0xFFB6FF48, 0xFF6D48DA, 0xFFFFFF00, 0xFFDA6D00, 0xFF004800, 0xFF006DDA, 0xFF009100, 0xFF242424, 0xFFFFFF6D, 0xFFFF6DFF, 0xFF916D00, 0xFF91FF6D },
		/* 2C04-0002 */	{ 0xFF000000, 0xFFFFB600, 0xFF916D00, 0xFFB6FF48, 0xFF91FF6D, 0xFFFF6DFF, 0xFF009191, 0xFFB6DAFF, 0xFFFF0000, 0xFF9100FF, 0xFFFFFF6D, 0xFFFF91FF, 0xFFFFFFFF, 0xFFDA6DFF, 0xFF91DAFF, 0xFF009100, 0xFF004800, 0xFF6DB6FF, 0xFFB62400, 0xFFDADADA, 0xFF00B66D, 0xFF6DDA00, 0xFF480000, 0xFF9191FF, 0xFF484848, 0xFFFF00FF, 0xFF00006D, 0xFF48FFDA, 0xFFDAB6FF, 0xFF6D4800, 0xFF000000, 0xFF6D48DA, 0xFF91006D, 0xFFFFDA91, 0xFFFF9100, 0xFFFFB6FF, 0xFF006DDA, 0xFF6D2400, 0xFFB6B6B6, 0xFF0000DA, 0xFFB600FF, 0xFFFFDA00, 0xFF6D6D6D, 0xFF244800, 0xFF0048FF, 0xFF000000, 0xFFDADA00, 0xFFFFFFFF, 0xFFDAB66D, 0xFF242424, 0xFF00FF00, 0xFFDA6D00, 0xFF004848, 0xFF002491, 0xFFFF0091, 0xFF249100, 0xFF000000, 0xFF00FFFF, 0xFF914800, 0xFFFFFF00, 0xFFFFB6B6, 0xFFB6006D, 0xFF006D24, 0xFF919191 },
		/* 2C04-0003 */	{ 0xFFB600FF, 0xFFFF6DFF, 0xFF91FF6D, 0xFFB6B6B6, 0xFF009100, 0xFFFFFFFF, 0xFFB6DAFF, 0xFF244800, 0xFF002491, 0xFF000000, 0xFFFFDA91, 0xFF6D4800, 0xFFFF0091, 0xFFDADADA, 0xFFDAB66D, 0xFF91DAFF, 0xFF9191FF, 0xFF009191, 0xFFB6006D, 0xFF0048FF, 0xFF249100, 0xFF916D00, 0xFFDA6D00, 0xFF00B66D, 0xFF6D6D6D, 0xFF6D48DA, 0xFF000000, 0xFF0000DA, 0xFFFF0000, 0xFFB62400, 0xFFFF91FF, 0xFFFFB6B6, 0xFFDA6DFF, 0xFF004800, 0xFF00006D, 0xFFFFFF00, 0xFF242424, 0xFFFFB600, 0xFFFF9100, 0xFFFFFFFF, 0xFF6DDA00, 0xFF91006D, 0xFF6DB6FF, 0xFFFF00FF, 0xFF006DDA, 0xFF919191, 0xFF000000, 0xFF6D2400, 0xFF00FFFF, 0xFF480000, 0xFFB6FF48, 0xFFFFB6FF, 0xFF914800, 0xFF00FF00, 0xFFDADA00, 0xFF484848, 0xFF006D24, 0xFF000000, 0xFFDAB6FF, 0xFFFFFF6D, 0xFF9100FF, 0xFF48FFDA, 0xFFFFDA00, 0xFF004848 },
		/* 2C04-0004 */	{ 0xFF916D00, 0xFF6D48DA, 0xFF009191, 0xFFDADA00, 0xFF000000, 0xFFFFB6B6, 0xFF002491, 0xFFDA6D00, 0xFFB6B6B6, 0xFF6D2400, 0xFF00FF00, 0xFF00006D, 0xFFFFDA91, 0xFFFFFF00, 0xFF009100, 0xFFB6FF48, 0xFFFF6DFF, 0xFF480000, 0xFF0048FF, 0xFFFF91FF, 0xFF000000, 0xFF484848, 0xFFB62400, 0xFFFF9100, 0xFFDAB66D, 0xFF00B66D, 0xFF9191FF, 0xFF249100, 0xFF91006D, 0xFF000000, 0xFF91FF6D, 0xFF6DB6FF, 0xFFB6006D, 0xFF006D24, 0xFF914800, 0xFF0000DA, 0xFF9100FF, 0xFFB600FF, 0xFF6D6D6D, 0xFFFF0091, 0xFF004848, 0xFFDADADA, 0xFF006DDA, 0xFF004800, 0xFF242424, 0xFFFFFF6D, 0xFF919191, 0xFFFF00FF, 0xFFFFB6FF, 0xFFFFFFFF, 0xFF6D4800, 0xFFFF0000, 0xFFFFDA00, 0xFF48FFDA, 0xFFFFFFFF, 0xFF91DAFF, 0xFF000000, 0xFFFFB600, 0xFFDA6DFF, 0xFFB6DAFF, 0xFF6DDA00, 0xFFDAB6FF, 0xFF00FFFF, 0xFF244800 },
		/* 2C05-01 */		{ 0xFF6D6D6D, 0xFF002491, 0xFF0000DA, 0xFF6D48DA, 0xFF91006D, 0xFFB6006D, 0xFFB62400, 0xFF914800, 0xFF6D4800, 0xFF244800, 0xFF006D24, 0xFF009100, 0xFF004848, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFB6B6B6, 0xFF006DDA, 0xFF0048FF, 0xFF9100FF, 0xFFB600FF, 0xFFFF0091, 0xFFFF0000, 0xFFDA6D00, 0xFF916D00, 0xFF249100, 0xFF009100, 0xFF00B66D, 0xFF009191, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFF6DB6FF, 0xFF9191FF, 0xFFDA6DFF, 0xFFFF00FF, 0xFFFF6DFF, 0xFFFF9100, 0xFFFFB600, 0xFFDADA00, 0xFF6DDA00, 0xFF00FF00, 0xFF48FFDA, 0xFF00FFFF, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFFB6DAFF, 0xFFDAB6FF, 0xFFFFB6FF, 0xFFFF91FF, 0xFFFFB6B6, 0xFFFFDA91, 0xFFFFFF48, 0xFFFFFF6D, 0xFFB6FF48, 0xFF91FF6D, 0xFF48FFDA, 0xFF91DAFF, 0xFF000000, 0xFF000000, 0xFF000000 },
		/* 2C05-02 */		{ 0xFF6D6D6D, 0xFF002491, 0xFF0000DA, 0xFF6D48DA, 0xFF91006D, 0xFFB6006D, 0xFFB62400, 0xFF914800, 0xFF6D4800, 0xFF244800, 0xFF006D24, 0xFF009100, 0xFF004848, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFB6B6B6, 0xFF006DDA, 0xFF0048FF, 0xFF9100FF, 0xFFB600FF, 0xFFFF0091, 0xFFFF0000, 0xFFDA6D00, 0xFF916D00, 0xFF249100, 0xFF009100, 0xFF00B66D, 0xFF009191, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFF6DB6FF, 0xFF9191FF, 0xFFDA6DFF, 0xFFFF00FF, 0xFFFF6DFF, 0xFFFF9100, 0xFFFFB600, 0xFFDADA00, 0xFF6DDA00, 0xFF00FF00, 0xFF48FFDA, 0xFF00FFFF, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFFB6DAFF, 0xFFDAB6FF, 0xFFFFB6FF, 0xFFFF91FF, 0xFFFFB6B6, 0xFFFFDA91, 0xFFFFFF48, 0xFFFFFF6D, 0xFFB6FF48, 0xFF91FF6D, 0xFF48FFDA, 0xFF91DAFF, 0xFF000000, 0xFF000000, 0xFF000000 },
		/* 2C05-03 */		{ 0xFF6D6D6D, 0xFF002491, 0xFF0000DA, 0xFF6D48DA, 0xFF91006D, 0xFFB6006D, 0xFFB62400, 0xFF914800, 0xFF6D4800, 0xFF244800, 0xFF006D24, 0xFF009100, 0xFF004848, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFB6B6B6, 0xFF006DDA, 0xFF0048FF, 0xFF9100FF, 0xFFB600FF, 0xFFFF0091, 0xFFFF0000, 0xFFDA6D00, 0xFF916D00, 0xFF249100, 0xFF009100, 0xFF00B66D, 0xFF009191, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFF6DB6FF, 0xFF9191FF, 0xFFDA6DFF, 0xFFFF00FF, 0xFFFF6DFF, 0xFFFF9100, 0xFFFFB600, 0xFFDADA00, 0xFF6DDA00, 0xFF00FF00, 0xFF48FFDA, 0xFF00FFFF, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFFB6DAFF, 0xFFDAB6FF, 0xFFFFB6FF, 0xFFFF91FF, 0xFFFFB6B6, 0xFFFFDA91, 0xFFFFFF48, 0xFFFFFF6D, 0xFFB6FF48, 0xFF91FF6D, 0xFF48FFDA, 0xFF91DAFF, 0xFF000000, 0xFF000000, 0xFF000000 },
		/* 2C05-04 */		{ 0xFF6D6D6D, 0xFF002491, 0xFF0000DA, 0xFF6D48DA, 0xFF91006D, 0xFFB6006D, 0xFFB62400, 0xFF914800, 0xFF6D4800, 0xFF244800, 0xFF006D24, 0xFF009100, 0xFF004848, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFB6B6B6, 0xFF006DDA, 0xFF0048FF, 0xFF9100FF, 0xFFB600FF, 0xFFFF0091, 0xFFFF0000, 0xFFDA6D00, 0xFF916D00, 0xFF249100, 0xFF009100, 0xFF00B66D, 0xFF009191, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFF6DB6FF, 0xFF9191FF, 0xFFDA6DFF, 0xFFFF00FF, 0xFFFF6DFF, 0xFFFF9100, 0xFFFFB600, 0xFFDADA00, 0xFF6DDA00, 0xFF00FF00, 0xFF48FFDA, 0xFF00FFFF, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFFB6DAFF, 0xFFDAB6FF, 0xFFFFB6FF, 0xFFFF91FF, 0xFFFFB6B6, 0xFFFFDA91, 0xFFFFFF48, 0xFFFFFF6D, 0xFFB6FF48, 0xFF91FF6D, 0xFF48FFDA, 0xFF91DAFF, 0xFF000000, 0xFF000000, 0xFF000000 },
		/* 2C05-05 */		{ 0xFF6D6D6D, 0xFF002491, 0xFF0000DA, 0xFF6D48DA, 0xFF91006D, 0xFFB6006D, 0xFFB62400, 0xFF914800, 0xFF6D4800, 0xFF244800, 0xFF006D24, 0xFF009100, 0xFF004848, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFB6B6B6, 0xFF006DDA, 0xFF0048FF, 0xFF9100FF, 0xFFB600FF, 0xFFFF0091, 0xFFFF0000, 0xFFDA6D00, 0xFF916D00, 0xFF249100, 0xFF009100, 0xFF00B66D, 0xFF009191, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFF6DB6FF, 0xFF9191FF, 0xFFDA6DFF, 0xFFFF00FF, 0xFFFF6DFF, 0xFFFF9100, 0xFFFFB600, 0xFFDADA00, 0xFF6DDA00, 0xFF00FF00, 0xFF48FFDA, 0xFF00FFFF, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFFB6DAFF, 0xFFDAB6FF, 0xFFFFB6FF, 0xFFFF91FF, 0xFFFFB6B6, 0xFFFFDA91, 0xFFFFFF48, 0xFFFFFF6D, 0xFFB6FF48, 0xFF91FF6D, 0xFF48FFDA, 0xFF91DAFF, 0xFF000000, 0xFF000000, 0xFF000000 }
	};

	bool _isFullColorPalette = false;
	uint32_t _userPalette[512] = { };
	uint32_t _currentPalette[512] = { };

	const uint8_t _paletteLut[11][64] = {
		/* 2C02 */      { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63 },
		/* 2C03 */      { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,15,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,15,62,63 },
		/* 2C04-0001 */ { 53,35,22,34,28,9,29,21,32,0,39,5,4,40,8,32,33,62,31,41,60,50,54,18,63,43,46,30,61,45,36,1,14,49,51,42,44,12,27,20,46,7,52,6,19,2,38,46,46,25,16,10,57,3,55,23,15,17,11,13,56,37,24,58 },
		/* 2C04-0002 */ { 46,39,24,57,58,37,28,49,22,19,56,52,32,35,60,11,15,33,6,61,27,41,30,34,29,36,14,43,50,8,46,3,4,54,38,51,17,31,16,2,20,63,0,9,18,46,40,32,62,13,42,23,12,1,21,25,46,44,7,55,53,5,10,45 },
		/* 2C04-0003 */ { 20,37,58,16,11,32,49,9,1,46,54,8,21,61,62,60,34,28,5,18,25,24,23,27,0,3,46,2,22,6,52,53,35,15,14,55,13,39,38,32,41,4,33,36,17,45,46,31,44,30,57,51,7,42,40,29,10,46,50,56,19,43,63,12 },
		/* 2C04-0004 */ { 24,3,28,40,46,53,1,23,16,31,42,14,54,55,11,57,37,30,18,52,46,29,6,38,62,27,34,25,4,46,58,33,5,10,7,2,19,20,0,21,12,61,17,15,13,56,45,36,51,32,8,22,63,43,32,60,46,39,35,49,41,50,44,9 },
		/* 2C05-01 */   { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,15,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,15,62,63 },
		/* 2C05-02 */   { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,15,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,15,62,63 },
		/* 2C05-03 */   { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,15,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,15,62,63 },
		/* 2C05-04 */   { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,15,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,15,62,63 },
		/* 2C05-05 */   { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,15,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,15,62,63 },
	};

	uint64_t _flags = 0;
	
	bool _audioSettingsChanged = false;
	double _channelVolume[11] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
	double _channelPanning[11] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
	EqualizerFilterType _equalizerFilterType = EqualizerFilterType::None;
	vector<double> _bandGains = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	vector<double> _bands = { { 40,56,80,113,160,225,320,450,600,750,1000,2000,3000,4000,5000,6000,7000,10000,12500,15000 } };
	double _masterVolume = 1.0;
	uint32_t _sampleRate = 48000;
	AudioFilterSettings _audioFilterSettings;

	NesModel _model = NesModel::Auto;
	PpuModel _ppuModel = PpuModel::Ppu2C02;

	bool _disableOverclocking = false;
	uint32_t _extraScanlinesBeforeNmi = 0;
	uint32_t _extraScanlinesAfterNmi = 0;

	OverscanDimensions _overscan;
	VideoFilterType _videoFilterType = VideoFilterType::None;
	double _videoScale = 1;
	VideoAspectRatio _aspectRatio = VideoAspectRatio::NoStretching;
	double _customAspectRatio = 1.0;
	PictureSettings _pictureSettings;
	NtscFilterSettings _ntscFilterSettings;
	bool _backgroundEnabled = true;
	bool _spritesEnabled = true;
	uint32_t _screenRotation = 0;

	ConsoleType _consoleType = ConsoleType::Nes;
	ExpansionPortDevice _expansionDevice = ExpansionPortDevice::None;
	ControllerType _controllerTypes[4] = { ControllerType::None, ControllerType::None, ControllerType::None, ControllerType::None };
	KeyMappingSet _controllerKeys[4] = { KeyMappingSet(), KeyMappingSet(), KeyMappingSet(), KeyMappingSet() };
	bool _needControllerUpdate = false;
	uint32_t _zapperDetectionRadius = 0;
	uint32_t _controllerDeadzoneSize = 2;
	std::unordered_map<int, double> _mouseSensitivity;
	int32_t _inputPollScanline = 241;

	bool _keyboardModeEnabled = false;

	RamPowerOnState _ramPowerOnState = RamPowerOnState::AllZeros;
	uint32_t _dipSwitches = 0;

public:
	EmulationSettings()
	{
		SetUserRgbPalette(_ppuPaletteArgb[0]);
	}

	static uint32_t GetMesenVersion()
	{
		return (_versionMajor << 16) | (_versionMinor << 8) | _versionRevision;
	}

	static string GetMesenVersionString()
	{
		return std::to_string(_versionMajor) + "." + std::to_string(_versionMinor) + "." + std::to_string(_versionRevision);
	}

	void SetFlags(uint64_t flags)
	{
		if((_flags & flags) != flags) {
			_flags |= flags;

			_backgroundEnabled = !CheckFlag(EmulationFlags::DisableBackground);
			_spritesEnabled = !CheckFlag(EmulationFlags::DisableSprites);
			if(flags & EmulationFlags::UseCustomVsPalette) {
				UpdateCurrentPalette();
			}
		}
	}

	void SetFlagState(EmulationFlags flag, bool enabled)
	{
		if(enabled) {
			SetFlags(flag);
		} else {
			ClearFlags(flag);
		}
	}

	void ClearFlags(uint64_t flags)
	{
		if((_flags & flags) != 0) {
			_flags &= ~flags;

			_backgroundEnabled = !CheckFlag(EmulationFlags::DisableBackground);
			_spritesEnabled = !CheckFlag(EmulationFlags::DisableSprites);
			if(flags & EmulationFlags::UseCustomVsPalette) {
				UpdateCurrentPalette();
			}
		}
	}

	bool CheckFlag(EmulationFlags flag)
	{
		return (_flags & flag) == flag;
	}

	bool InputEnabled()
	{
		return !CheckFlag(EmulationFlags::InBackground) || CheckFlag(EmulationFlags::AllowBackgroundInput);
	}

	void SetNesModel(NesModel model)
	{
		_model = model;
	}

	NesModel GetNesModel()
	{
		return _model;
	}

	void SetPpuModel(PpuModel ppuModel)
	{
		_ppuModel = ppuModel;
		UpdateCurrentPalette();
	}

	PpuModel GetPpuModel()
	{
		return _ppuModel;
	}

	void UpdateCurrentPalette()
	{
		if(CheckFlag(EmulationFlags::UseCustomVsPalette)) {
			for(int i = 0; i < 64; i++) {
				for(int j = 0; j < 8; j++) {
					_currentPalette[(j << 6) | i] = _userPalette[(j << 6) | (_paletteLut[(int)_ppuModel][i])];
				}
			}
		} else if(_ppuModel == PpuModel::Ppu2C02) {
			memcpy(_currentPalette, _userPalette, sizeof(_userPalette));
		} else {
			memcpy(_currentPalette, _ppuPaletteArgb[(int)_ppuModel], sizeof(_ppuPaletteArgb[0]));
			GenerateFullColorPalette(_currentPalette);
		}
	}

	uint32_t* GetRgbPalette()
	{
		return _currentPalette;
	}

	void GetUserRgbPalette(uint32_t* paletteBuffer)
	{
		memcpy(paletteBuffer, _userPalette, sizeof(_userPalette));
	}

	void SetUserRgbPalette(uint32_t* paletteBuffer, uint32_t size = 64)
	{
		if(size != 64 && size != 512) {
			throw new std::runtime_error("Invalid palette buffer size");
		}

		memcpy(_userPalette, paletteBuffer, size * sizeof(uint32_t));
		if(size == 64) {
			GenerateFullColorPalette(_userPalette);
		}
		_isFullColorPalette = (size == 512);
		UpdateCurrentPalette();
	}

	bool IsFullColorPalette()
	{
		return _isFullColorPalette;
	}

	void GenerateFullColorPalette(uint32_t* paletteBuffer)
	{
		for(int i = 0; i < 64; i++) {
			for(int j = 1; j < 8; j++) {
				double redColor = (uint8_t)(paletteBuffer[i] >> 16);
				double greenColor = (uint8_t)(paletteBuffer[i] >> 8);
				double blueColor = (uint8_t)paletteBuffer[i];
				if((i & 0x0F) <= 0x0D) {
					//Emphasis doesn't affect columns $xE and $xF
					if(j & 0x01) {
						//Intensify red
						greenColor *= 0.84;
						blueColor *= 0.84;
					}
					if(j & 0x02) {
						//Intensify green
						redColor *= 0.84;
						blueColor *= 0.84;
					}
					if(j & 0x04) {
						//Intensify blue
						redColor *= 0.84;
						greenColor *= 0.84;
					}
				}

				uint8_t r = (uint8_t)(redColor > 255 ? 255 : redColor);
				uint8_t g = (uint8_t)(greenColor > 255 ? 255 : greenColor);
				uint8_t b = (uint8_t)(blueColor > 255 ? 255 : blueColor);

				uint32_t color = 0xFF000000 | (r << 16) | (g << 8) | b;
				paletteBuffer[(j << 6) | i] = color;
			}
		}
	}

	//0: Muted, 0.5: Default, 1.0: Max volume
	void SetChannelVolume(AudioChannel channel, double volume)
	{
		_channelVolume[(int)channel] = volume;
		_audioSettingsChanged = true;
	}

	void SetMasterVolume(double volume, double volumeReduction = -1.0)
	{
		_masterVolume = volume;
		_audioSettingsChanged = true;
	}

	void SetChannelPanning(AudioChannel channel, double panning)
	{
		_channelPanning[(int)channel] = panning;
		_audioSettingsChanged = true;
	}

	vector<double> GetBandGains()
	{
		return _bandGains;
	}

	void SetBandGain(int band, double gain)
	{
		if(band < (int)_bandGains.size()) {
			_bandGains[band] = gain;
			_audioSettingsChanged = true;
		}
	}

	vector<double> GetEqualizerBands()
	{
		return _bands;
	}

	void SetEqualizerBands(double *bands, uint32_t bandCount)
	{
		_bands.clear();
		_bandGains.clear();
		for(uint32_t i = 0; i < bandCount; i++) {
			_bands.push_back(bands[i]);
			_bandGains.push_back(0);
		}
	}

	EqualizerFilterType GetEqualizerFilterType()
	{
		return _equalizerFilterType;
	}

	void SetEqualizerFilterType(EqualizerFilterType filter)
	{
		_equalizerFilterType = filter;
		_audioSettingsChanged = true;
	}

	void SetSampleRate(uint32_t sampleRate)
	{
		_sampleRate = sampleRate;
		_audioSettingsChanged = true;
	}

	uint32_t GetSampleRate()
	{
		return _sampleRate;
	}

	void SetAudioFilterSettings(AudioFilterSettings settings)
	{
		_audioFilterSettings = settings;
		_audioSettingsChanged = true;
	}

	AudioFilterSettings GetAudioFilterSettings()
	{
		return _audioFilterSettings;
	}

	bool NeedAudioSettingsUpdate()
	{
		bool value = _audioSettingsChanged;
		if(value)
			_audioSettingsChanged = false;
		return value;
	}

	void DisableOverclocking(bool disabled)
	{
		if(_disableOverclocking != disabled) {
			_disableOverclocking = disabled;
		}
	}

	uint32_t GetPpuExtraScanlinesBeforeNmi()
	{
		return _disableOverclocking ? 0 : _extraScanlinesBeforeNmi;
	}

	uint32_t GetPpuExtraScanlinesAfterNmi()
	{
		return _disableOverclocking ? 0 : _extraScanlinesAfterNmi;
	}

	void SetPpuNmiConfig(uint32_t extraScanlinesBeforeNmi, uint32_t extraScanlinesAfterNmi)
	{
		if(_extraScanlinesBeforeNmi != extraScanlinesBeforeNmi || _extraScanlinesAfterNmi != extraScanlinesAfterNmi) {
			_extraScanlinesBeforeNmi = extraScanlinesBeforeNmi;
			_extraScanlinesAfterNmi = extraScanlinesAfterNmi;
		}
	}

	void SetOverscanDimensions(uint8_t left, uint8_t right, uint8_t top, uint8_t bottom)
	{
		if(_overscan.Left != left || _overscan.Right != right || _overscan.Top != top || _overscan.Bottom != bottom) {
			_overscan.Left = left;
			_overscan.Right = right;
			_overscan.Top = top;
			_overscan.Bottom = bottom;
		}
	}

	OverscanDimensions GetOverscanDimensions()
	{
		return _overscan;
	}

	double GetChannelVolume(AudioChannel channel)
	{
		return _channelVolume[(int)channel];
	}

	double GetMasterVolume()
	{
		return _masterVolume;
	}

	double GetChannelPanning(AudioChannel channel)
	{
		return _channelPanning[(int)channel];
	}

	void SetVideoFilterType(VideoFilterType videoFilterType)
	{
		_videoFilterType = videoFilterType;
	}

	VideoFilterType GetVideoFilterType()
	{
		return _videoFilterType;
	}

	void SetVideoAspectRatio(VideoAspectRatio aspectRatio, double customRatio)
	{
		_aspectRatio = aspectRatio;
		_customAspectRatio = customRatio;
	}

	bool GetBackgroundEnabled()
	{
		return _backgroundEnabled;
	}

	bool GetSpritesEnabled()
	{
		return _spritesEnabled;
	}

	void SetPictureSettings(double brightness, double contrast, double saturation, double hue, double scanlineIntensity)
	{
		_pictureSettings.Brightness = brightness;
		_pictureSettings.Contrast = contrast;
		_pictureSettings.Saturation = saturation;
		_pictureSettings.Hue = hue;
		_pictureSettings.ScanlineIntensity = scanlineIntensity;
	}

	PictureSettings GetPictureSettings()
	{
		return _pictureSettings;
	}

	void SetNtscFilterSettings(double artifacts, double bleed, double fringing, double gamma, double resolution, double sharpness, bool mergeFields, double yFilterLength, double iFilterLength, double qFilterLength, bool verticalBlend, bool keepVerticalResolution)
	{
		_ntscFilterSettings.Artifacts = artifacts;
		_ntscFilterSettings.Bleed = bleed;
		_ntscFilterSettings.Fringing = fringing;
		_ntscFilterSettings.Gamma = gamma;
		_ntscFilterSettings.Resolution = resolution;
		_ntscFilterSettings.Sharpness = sharpness;

		_ntscFilterSettings.MergeFields = mergeFields;

		_ntscFilterSettings.YFilterLength = yFilterLength;
		_ntscFilterSettings.IFilterLength = iFilterLength;
		_ntscFilterSettings.QFilterLength = qFilterLength;

		_ntscFilterSettings.VerticalBlend = verticalBlend;
		_ntscFilterSettings.KeepVerticalResolution = keepVerticalResolution;
	}

	NtscFilterSettings GetNtscFilterSettings()
	{
		return _ntscFilterSettings;
	}

	double GetAspectRatio(std::shared_ptr<Console> console);
	bool GetStaticAspectRatio()
	{
		if(_aspectRatio == VideoAspectRatio::StandardS || _aspectRatio == VideoAspectRatio::WidescreenS)
			return true; 
		return false;
	}
	void InitializeInputDevices(GameInputType inputType, GameSystem system, bool silent);

	double GetVideoScale()
	{
		return _videoScale;
	}
	
	void SetScreenRotation(uint32_t angle)
	{
		_screenRotation = angle;
	}

	uint32_t GetScreenRotation()
	{
		return _screenRotation;
	}

	void SetExpansionDevice(ExpansionPortDevice expansionDevice)
	{
		_expansionDevice = expansionDevice;
		_needControllerUpdate = true;
	}
	
	ExpansionPortDevice GetExpansionDevice()
	{
		return _expansionDevice;
	}

	void SetConsoleType(ConsoleType type)
	{
		_consoleType = type;
		_needControllerUpdate = true;
	}

	ConsoleType GetConsoleType()
	{
		return _consoleType;
	}

	void SetControllerType(uint8_t port, ControllerType type)
	{
		_controllerTypes[port] = type;
		_needControllerUpdate = true;
	}

	ControllerType GetControllerType(uint8_t port)
	{
		return _controllerTypes[port];
	}

	void SetControllerKeys(uint8_t port, KeyMappingSet keyMappings)
	{
		_controllerKeys[port] = keyMappings;
		_needControllerUpdate = true;
	}

	KeyMappingSet GetControllerKeys(uint8_t port)
	{
		return _controllerKeys[port];
	}

	bool NeedControllerUpdate()
	{
		if(_needControllerUpdate) {
			_needControllerUpdate = false;
			return true;
		}
		return false;
	}

	void SetMouseSensitivity(MouseDevice device, double sensitivity)
	{
		_mouseSensitivity[(int)device] = sensitivity;
	}

	double GetMouseSensitivity(MouseDevice device)
	{
		auto result = _mouseSensitivity.find((int)device);
		if(result != _mouseSensitivity.end())
			return result->second;
		return 1.0;
	}

	void SetControllerDeadzoneSize(uint32_t deadzoneSize)
	{
		_controllerDeadzoneSize = deadzoneSize;
	}

	double GetControllerDeadzoneRatio()
	{
		switch(_controllerDeadzoneSize) {
			case 0: return 0.5;
			case 1: return 0.75;
			case 2: return 1;
			case 3: return 1.25;
			case 4: return 1.5;
		}
		return 1;
	}

	void SetZapperDetectionRadius(uint32_t detectionRadius)
	{
		_zapperDetectionRadius = detectionRadius;
	}

	uint32_t GetZapperDetectionRadius()
	{
		return _zapperDetectionRadius;
	}

	void SetInputPollScanline(int32_t scanline)
	{
		_inputPollScanline = scanline;
	}

	int32_t GetInputPollScanline()
	{
		return _inputPollScanline;
	}

	bool HasZapper()
	{
		return _controllerTypes[0] == ControllerType::Zapper || _controllerTypes[1] == ControllerType::Zapper || (_consoleType == ConsoleType::Famicom && _expansionDevice == ExpansionPortDevice::Zapper);
	}

	bool HasArkanoidPaddle()
	{
		return _controllerTypes[0] == ControllerType::ArkanoidController || _controllerTypes[1] == ControllerType::ArkanoidController || (_consoleType == ConsoleType::Famicom && _expansionDevice == ExpansionPortDevice::ArkanoidController);
	}

	void SetRamPowerOnState(RamPowerOnState state)
	{
		_ramPowerOnState = state;
	}

	RamPowerOnState GetRamPowerOnState()
	{
		return _ramPowerOnState;
	}

	void SetDipSwitches(uint32_t dipSwitches)
	{
		_dipSwitches = dipSwitches;
	}

	uint32_t GetDipSwitches()
	{
		return _dipSwitches;
	}

	bool IsKeyboardMode()
	{
		return _keyboardModeEnabled;
	}

	void EnableKeyboardMode()
	{
		if(!_keyboardModeEnabled)
			_keyboardModeEnabled = true;
	}

	void DisableKeyboardMode()
	{
		if(_keyboardModeEnabled)
			_keyboardModeEnabled = false;
	}
};
