#include "stdafx.h"
#include <algorithm>
#include "ScriptingContext.h"
#include "DebuggerTypes.h"
#include "Debugger.h"
#include "Console.h"
#include "SaveStateManager.h"

string ScriptingContext::_log = "";

ScriptingContext::ScriptingContext(Debugger *debugger)
{
	_debugger = debugger;
}

void ScriptingContext::Log(string message)
{
	_logRows.push_back(message);
	if(_logRows.size() > 500) {
		_logRows.pop_front();
	}
}

Debugger* ScriptingContext::GetDebugger()
{
	return _debugger;
}

void ScriptingContext::CallMemoryCallback(uint16_t addr, uint8_t &value, CallbackType type)
{
	_inExecOpEvent = type == CallbackType::CpuExec;
	InternalCallMemoryCallback(addr, value, type);
	_inExecOpEvent = false;
}

int ScriptingContext::CallEventCallback(EventType type)
{
	_inStartFrameEvent = type == EventType::StartFrame;
	int returnValue = InternalCallEventCallback(type);
	_inStartFrameEvent = false;

	return returnValue;
}

bool ScriptingContext::CheckInitDone()
{
	return _initDone;
}

bool ScriptingContext::CheckInStartFrameEvent()
{
	return _inStartFrameEvent;
}

bool ScriptingContext::CheckInExecOpEvent()
{
	return _inExecOpEvent;
}

bool ScriptingContext::CheckStateLoadedFlag()
{
	bool stateLoaded = _stateLoaded;
	_stateLoaded = false;
	return stateLoaded;
}

void ScriptingContext::RegisterMemoryCallback(CallbackType type, int startAddr, int endAddr, int reference)
{
	if(endAddr < startAddr) {
		return;
	}

	if(startAddr == 0 && endAddr == 0) {
		if(type <= CallbackType::CpuExec) {
			endAddr = 0xFFFF;
		} else {
			endAddr = 0x3FFF;
		}
	}

	for(int i = startAddr; i <= endAddr; i++) {
		_callbacks[(int)type][i].push_back(reference);
	}
}

void ScriptingContext::UnregisterMemoryCallback(CallbackType type, int startAddr, int endAddr, int reference)
{
	if(endAddr < startAddr) {
		return;
	}

	if(startAddr == 0 && endAddr == 0) {
		if(type <= CallbackType::CpuExec) {
			endAddr = 0xFFFF;
		} else {
			endAddr = 0x3FFF;
		}
	}

	for(int i = startAddr; i <= endAddr; i++) {
		vector<int> &refs = _callbacks[(int)type][i];
		refs.erase(std::remove(refs.begin(), refs.end(), reference), refs.end());
	}
}

void ScriptingContext::RegisterEventCallback(EventType type, int reference)
{
	_eventCallbacks[(int)type].push_back(reference);
}

void ScriptingContext::UnregisterEventCallback(EventType type, int reference)
{
	vector<int> &callbacks = _eventCallbacks[(int)type];
	callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), reference), callbacks.end());
}

void ScriptingContext::SaveState()
{
	if(_saveSlot >= 0) {
		stringstream ss;
		_debugger->GetConsole()->GetSaveStateManager()->SaveState(ss);
		_saveSlotData[_saveSlot] = ss.str();
		_saveSlot = -1;
	}
}

bool ScriptingContext::LoadState()
{
	if(_loadSlot >= 0 && _saveSlotData.find(_loadSlot) != _saveSlotData.end()) {
		stringstream ss;
		ss << _saveSlotData[_loadSlot];
		bool result = _debugger->GetConsole()->GetSaveStateManager()->LoadState(ss);
		_loadSlot = -1;
		if(result) {
			_stateLoaded = true;
		}
		return result;
	}
	return false;
}

bool ScriptingContext::LoadState(string stateData)
{
	stringstream ss;
	ss << stateData;
	bool result = _debugger->GetConsole()->GetSaveStateManager()->LoadState(ss);
	if(result) {
		_stateLoaded = true;
	}
	return result;
}
