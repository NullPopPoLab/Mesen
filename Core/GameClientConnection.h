#pragma once
#include "stdafx.h"
#include <deque>
#include "GameConnection.h"
#include "../Utilities/AutoResetEvent.h"
#include "../Utilities/SimpleLock.h"
#include "StandardController.h"

class ClientConnectionData;

class GameClientConnection : public GameConnection
{
private:
	std::deque<uint8_t> _inputData[4];
	atomic<uint32_t> _inputSize[4];
	AutoResetEvent _waitForInput[4];
	SimpleLock _writeLock;
	atomic<bool> _shutdown;
	atomic<bool> _enableControllers = false;
	atomic<uint32_t> _minimumQueueSize = 3;

	shared_ptr<BaseControlDevice> _controlDevice;
	uint32_t _lastInputSent = 0x00;
	bool _gameLoaded = false;
	uint8_t _controllerPort = 255;

private:
	void SendHandshake();
	void ClearInputData();
	void PushControllerState(uint8_t port, uint8_t state);
	void DisableControllers();

protected:
	void ProcessMessage(NetMessage* message);

public:
	GameClientConnection(shared_ptr<Socket> socket, shared_ptr<ClientConnectionData> connectionData);
	~GameClientConnection();

	uint8_t GetControllerState(uint8_t port);
	void SendInput();
};