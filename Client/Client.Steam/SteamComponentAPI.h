/*
 * This file is part of the CitizenFX project - http://citizen.re/
 *
 * See LICENSE and MENTIONS in the root of the source tree for information
 * regarding licensing.
 */

#pragma once

class ISteamComponent
{
public:
	virtual bool IsSteamRunning() = 0;

	virtual ISteamClient* GetPublicClient() = 0;

	virtual IClientEngine* GetPrivateClient() = 0;

	virtual HSteamUser GetHSteamUser() = 0;

	virtual HSteamPipe GetHSteamPipe() = 0;

	virtual int RegisterSteamCallbackRaw(int callbackID, std::function<void(void*)> callback) = 0;

	virtual void RemoveSteamCallback(int registeredID) = 0;

	virtual int GetParentAppID() = 0;

	virtual void Initialize() = 0;

// template bits
public:
	template<typename TCallback>
	int RegisterSteamCallback(std::function<void(TCallback*)> callback)
	{
		return RegisterSteamCallbackRaw(TCallback::k_iCallback, [=] (void* value)
		{
			callback(static_cast<TCallback*>(value));
		});
	}
};

DECLARE_INSTANCE_TYPE(ISteamComponent);