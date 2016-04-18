/*
 * This file is part of the CitizenFX project - http://citizen.re/
 *
 * See LICENSE and MENTIONS in the root of the source tree for information
 * regarding licensing.
 */

#pragma once

/*
#ifndef CORE_EXPORT
#ifdef COMPILING_CORE
#define CORE_EXPORT DLL_EXPORT
#else
#define CORE_EXPORT
#endif
#endif

#ifndef CORE_IMPORT
#ifdef COMPILING_CORE
#define CORE_IMPORT
#else
#define CORE_IMPORT DLL_IMPORT
#endif
#endif
*/

#define CORE_EXPORT
#define CORE_IMPORT

template<typename TContained>
class InstanceRegistryBase : public fwRefCountable
{
private:
	std::unordered_map<std::string, TContained> m_instanceMap;

public:
	TContained GetInstance(const char* key)
	{
		auto it = m_instanceMap.find(key);
		TContained instance = TContained();

		if (it != m_instanceMap.end())
		{
			instance = it->second;
		}
		else
		{
			FatalError("Could not obtain instance from InstanceRegistry of type `%s`.", key);
		}

		return instance;
	}

	void SetInstance(const char* key, const TContained& instance)
	{
		m_instanceMap[key] = instance;
	}
};

using InstanceRegistry = InstanceRegistryBase<void*>;
using RefInstanceRegistry = InstanceRegistryBase<fwRefContainer<fwRefCountable>>;

extern CORE_EXPORT CORE_IMPORT InstanceRegistry g_instanceRegistry;

template<class T>
class Instance
{
private:
	static const char* ms_name;
	static T* ms_cachedInstance;

public:
	static T* Get(InstanceRegistry* registry)
	{
		T* instance = static_cast<T*>(registry->GetInstance(ms_name));

		assert(instance != nullptr);

		return instance;
	}

	static fwRefContainer<T> Get(fwRefContainer<RefInstanceRegistry> registry)
	{
		fwRefContainer<T> instance = registry->GetInstance(ms_name);

		assert(instance.GetRef());

		return instance;
	}

	static T* Get()
	{
		if (!ms_cachedInstance)
		{
			ms_cachedInstance = Get(&g_instanceRegistry);
		}

		return ms_cachedInstance;
	}

	static void Set(T* instance)
	{
		Set(instance, &g_instanceRegistry);
	}

	static void Set(T* instance, InstanceRegistry* registry)
	{
		registry->SetInstance(ms_name, instance);
	}

	static void Set(const fwRefContainer<T>& instance, fwRefContainer<RefInstanceRegistry> registry)
	{
		registry->SetInstance(ms_name, instance);
	}

	static const char* GetName()
	{
		return ms_name;
	}
};

#define DECLARE_INSTANCE_TYPE(name) \
	template<> __declspec(selectany) const char* ::Instance<name>::ms_name = #name; \
	template<> __declspec(selectany) name* ::Instance<name>::ms_cachedInstance = nullptr;
