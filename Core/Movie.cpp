#include "stdafx.h"
#include "MessageManager.h"
#include "Movie.h"
#include "Console.h"
#include "../Utilities/FolderUtilities.h"
#include "RomLoader.h"

shared_ptr<Movie> Movie::_instance(new Movie());

Movie::~Movie()
{
	_instance = nullptr;
}

shared_ptr<Movie> Movie::GetInstance()
{
	return _instance;
}

void Movie::PushState(uint8_t port)
{
	if(_counter[port] > 0) {
		uint16_t data = _lastState[port] << 8 | _counter[port];
		_data.PortData[port].push_back(data);

		_lastState[port] = 0;
		_counter[port] = 0;
	}
}

void Movie::RecordState(uint8_t port, uint8_t state)
{
	if(_recording) {
		if(_lastState[port] != state || _counter[port] == 0) {
			if(_counter[port] != 0) {
				PushState(port);
			}
			_lastState[port] = state;
			_counter[port] = 1;
		} else {
			_counter[port]++;

			if(_counter[port] == 255) {
				PushState(port);
			}
		}
	}
}

uint8_t Movie::GetState(uint8_t port)
{
	uint16_t data = --_data.PortData[port][_readPosition[port]];
	if((data & 0xFF) == 0) {
		_readPosition[port]++;
	}

	if(_readPosition[port] >= _data.DataSize[port]) {
		//End of movie file
		MessageManager::DisplayMessage("Movies", "Movie ended.");
		MessageManager::SendNotification(ConsoleNotificationType::MovieEnded);
		if(EmulationSettings::CheckFlag(EmulationFlags::PauseOnMovieEnd)) {
			EmulationSettings::SetFlags(EmulationFlags::Paused);
		}
		_playing = false;
	}

	return (data >> 8);
}

void Movie::Reset()
{
	_startState.clear();
	_startState.seekg(0, ios::beg);
	_startState.seekp(0, ios::beg);

	memset(_readPosition, 0, 4 * sizeof(uint32_t));
	memset(_counter, 0, 4);
	memset(_lastState, 0, 4);
	_data = MovieData();

	_recording = false;
	_playing = false;
}

void Movie::StartRecording(string filename, bool reset)
{
	_filename = filename;
	_file.open(filename, ios::out | ios::binary);

	if(_file) {
		Console::Pause();

		Reset();

		if(reset) {
			Console::Reset(false);
		} else {
			Console::SaveState(_startState);
		}

		_recording = true;

		Console::Resume();

		MessageManager::DisplayMessage("Movies", "Recording to: " + FolderUtilities::GetFilename(filename, true));
	}
}

void Movie::StopAll()
{
	if(_recording) {
		_recording = false;
		for(int i = 0; i < 4; i++) {
			PushState(i);
		}
		Save();
	}
	if(_playing) {
		MessageManager::DisplayMessage("Movies", "Movie stopped.");
		_playing = false;
	}
}

void Movie::PlayMovie(stringstream &filestream, bool autoLoadRom, string filename)
{
	StopAll();

	Reset();
	
	if(Load(filestream, autoLoadRom)) {
		Console::Pause();
		if(_startState.tellp() > 0) {
			//Restore state if one was present in the movie
			Console::LoadState(_startState);
		} else if(autoLoadRom) {
			//When autoLoadRom = false, assume the emulation has already been reset (used by AutoTestRom)
			Console::Reset(false);
		}
		_playing = true;
		Console::Resume();

		if(!filename.empty()) {
			MessageManager::DisplayMessage("Movies", "Playing movie: " + FolderUtilities::GetFilename(filename, true));
		}
	}
}

void Movie::Record(string filename, bool reset)
{
	if(_instance) {
		_instance->StartRecording(filename, reset);
	}
}

void Movie::Play(string filename)
{
	if(_instance) {
		ifstream file(filename, ios::in | ios::binary);
		std::stringstream ss;

		if(file) {
			ss << file.rdbuf();
			file.close();

			_instance->PlayMovie(ss, true, filename);
		}
	}
}

void Movie::Play(std::stringstream &filestream, bool autoLoadRom)
{
	if(_instance) {
		_instance->PlayMovie(filestream, autoLoadRom);
	}
}

void Movie::Stop()
{
	if(_instance) {
		_instance->StopAll();
	}
}

bool Movie::Playing()
{
	if(_instance) {
		return _instance->_playing;
	} else {
		return false;
	}
}

bool Movie::Recording()
{
	if(_instance) {
		return _instance->_recording;
	} else {
		return false;
	}
}

struct MovieHeader
{
	char Header[3] = { 'M', 'M', 'O' };
	uint32_t MesenVersion;
	uint32_t MovieFormatVersion;
	uint32_t RomCrc32;
	uint32_t Region;
	uint32_t ConsoleType;
	uint8_t ControllerTypes[4];
	uint32_t ExpansionDevice;
	uint32_t FilenameLength;
};

bool Movie::Save()
{
	string romFilepath = Console::GetROMPath();
	string romFilename = FolderUtilities::GetFilename(romFilepath, true);

	MovieHeader header = {};
	header.MesenVersion = EmulationSettings::GetMesenVersion();
	header.MovieFormatVersion = 1;
	header.RomCrc32 = RomLoader::GetCRC32(romFilepath);
	header.Region = (uint32_t)Console::GetNesModel();
	header.ConsoleType = (uint32_t)EmulationSettings::GetConsoleType();
	header.ExpansionDevice = (uint32_t)EmulationSettings::GetExpansionDevice();
	for(int port = 0; port < 4; port++) {
		header.ControllerTypes[port] = (uint32_t)EmulationSettings::GetControllerType(port);
	}
	header.FilenameLength = (uint32_t)romFilename.size();

	_file.write((char*)header.Header, sizeof(header.Header));
	_file.write((char*)&header.MesenVersion, sizeof(header.MesenVersion));
	_file.write((char*)&header.MovieFormatVersion, sizeof(header.MovieFormatVersion));
	_file.write((char*)&header.RomCrc32, sizeof(header.RomCrc32));
	_file.write((char*)&header.Region, sizeof(header.Region));
	_file.write((char*)&header.ConsoleType, sizeof(header.ConsoleType));
	_file.write((char*)&header.ControllerTypes, sizeof(header.ControllerTypes));
	_file.write((char*)&header.ExpansionDevice, sizeof(header.ExpansionDevice));
	_file.write((char*)&header.FilenameLength, sizeof(header.FilenameLength));

	_file.write((char*)romFilename.c_str(), header.FilenameLength);

	_data.SaveStateSize = (uint32_t)_startState.tellp();
	_file.write((char*)&_data.SaveStateSize, sizeof(uint32_t));
		
	if(_data.SaveStateSize > 0) {
		_startState.seekg(0, ios::beg);
		uint8_t *stateBuffer = new uint8_t[_data.SaveStateSize];
		_startState.read((char*)stateBuffer, _data.SaveStateSize);
		_file.write((char*)stateBuffer, _data.SaveStateSize);
		delete[] stateBuffer;
	}

	for(int i = 0; i < 4; i++) {
		_data.DataSize[i] = (uint32_t)_data.PortData[i].size();
		_file.write((char*)&_data.DataSize[i], sizeof(uint32_t));
		if(_data.DataSize[i] > 0) {
			_file.write((char*)&_data.PortData[i][0], _data.DataSize[i] * sizeof(uint16_t));
		}
	}

	_file.close();

	MessageManager::DisplayMessage("Movies", "Movie saved to file: " + FolderUtilities::GetFilename(_filename, true));

	return true;
}

bool Movie::Load(std::stringstream &file, bool autoLoadRom)
{
	MovieHeader header = {};
	file.read((char*)header.Header, sizeof(header.Header));

	if(memcmp(header.Header, "MMO", 3) != 0) {
		//Invalid movie file
		return false;
	}

	file.read((char*)&header.MesenVersion, sizeof(header.MesenVersion));
	file.read((char*)&header.MovieFormatVersion, sizeof(header.MovieFormatVersion));
	file.read((char*)&header.RomCrc32, sizeof(header.RomCrc32));
	file.read((char*)&header.Region, sizeof(header.Region));
	file.read((char*)&header.ConsoleType, sizeof(header.ConsoleType));
	file.read((char*)&header.ControllerTypes, sizeof(header.ControllerTypes));
	file.read((char*)&header.ExpansionDevice, sizeof(header.ExpansionDevice));
	file.read((char*)&header.FilenameLength, sizeof(header.FilenameLength));

	EmulationSettings::SetConsoleType((ConsoleType)header.ConsoleType);
	EmulationSettings::SetExpansionDevice((ExpansionPortDevice)header.ExpansionDevice);
	for(int port = 0; port < 4; port++) {
		EmulationSettings::SetControllerType(port, (ControllerType)header.ControllerTypes[port]);
	}

	char* romFilename = new char[header.FilenameLength + 1];
	memset(romFilename, 0, header.FilenameLength + 1);
	file.read((char*)romFilename, header.FilenameLength);
	
	bool loadedGame = true;
	if(autoLoadRom) {
		string currentRom = Console::GetROMPath();
		if(currentRom.empty() || header.RomCrc32 != RomLoader::GetCRC32(currentRom)) {
			//Loaded game isn't the same as the game used for the movie, attempt to load the correct game
			loadedGame = Console::LoadROM(romFilename, header.RomCrc32);
		}
	}

	if(loadedGame) {
		file.read((char*)&_data.SaveStateSize, sizeof(uint32_t));

		if(_data.SaveStateSize > 0) {
			uint8_t *stateBuffer = new uint8_t[_data.SaveStateSize];
			file.read((char*)stateBuffer, _data.SaveStateSize);
			_startState.write((char*)stateBuffer, _data.SaveStateSize);
			delete[] stateBuffer;
		}

		for(int i = 0; i < 4; i++) {
			file.read((char*)&_data.DataSize[i], sizeof(uint32_t));

			uint16_t* readBuffer = new uint16_t[_data.DataSize[i]];
			file.read((char*)readBuffer, _data.DataSize[i] * sizeof(uint16_t));
			_data.PortData[i] = vector<uint16_t>(readBuffer, readBuffer + _data.DataSize[i]);
			delete[] readBuffer;
		}
	} else {
		MessageManager::DisplayMessage("Movies", "Missing ROM required (" + string(romFilename) + ") to play movie.");
	}
	return loadedGame;
}