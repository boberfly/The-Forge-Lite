/*
 * Copyright (c) 2018-2019 Confetti Interactive Inc.
 *
 * This file is part of The-Forge
 * (see https://github.com/ConfettiFX/The-Forge).
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
*/

#ifdef _WIN32

#include "Interfaces/IThread.h"
#include "Interfaces/IOperatingSystem.h"
#include "Interfaces/ILogManager.h"
#include "Interfaces/IMemoryManager.h"

DWORD WINAPI ThreadFunctionStatic(void* data)
{
	ThreadDesc* pDesc = (ThreadDesc*)data;
	pDesc->pFunc(pDesc->pData);
	return 0;
}

AtomicUint::AtomicUint()
{
	mAtomicInt = 0;
}

AtomicUint::~AtomicUint()
{

} 

void AtomicUint::AtomicStore(unsigned int i) {
	InterlockedExchange((volatile unsigned int*)&mAtomicInt, 0);
}

unsigned int AtomicUint::AtomicIncrement()
{
	// -1 because InterlockedIncrement returns incremented value. To match atomic::, we want to return the value before incrementation
	return InterlockedIncrement((volatile unsigned int*)&mAtomicInt) - 1;
}

unsigned int AtomicUint::AtomicDecrement()
{
	// -1 because InterlockedIncrement returns incremented value. To match atomic::, we want to return the value before decrementation
	return InterlockedDecrement((volatile unsigned int*)&mAtomicInt) - 1;
}

Mutex::Mutex()
{
	pHandle = (CRITICAL_SECTION*)conf_calloc(1, sizeof(CRITICAL_SECTION));
	InitializeCriticalSection((CRITICAL_SECTION*)pHandle);
}

Mutex::~Mutex()
{
	CRITICAL_SECTION* cs = (CRITICAL_SECTION*)pHandle;
	DeleteCriticalSection(cs);
	conf_free(cs);
	pHandle = 0;
}

void Mutex::Acquire() { EnterCriticalSection((CRITICAL_SECTION*)pHandle); }

void Mutex::Release() { LeaveCriticalSection((CRITICAL_SECTION*)pHandle); }

ConditionVariable::ConditionVariable()
{
	pHandle = (CONDITION_VARIABLE*)conf_calloc(1, sizeof(CONDITION_VARIABLE));
	InitializeConditionVariable((PCONDITION_VARIABLE)pHandle);
}

ConditionVariable::~ConditionVariable() { conf_free(pHandle); }

void ConditionVariable::Wait(const Mutex& mutex)
{
	SleepConditionVariableCS((PCONDITION_VARIABLE)pHandle, (PCRITICAL_SECTION)mutex.pHandle, INFINITE);
}

void ConditionVariable::Wait(const Mutex& mutex, unsigned ms)
{
	SleepConditionVariableCS((PCONDITION_VARIABLE)pHandle, (PCRITICAL_SECTION)mutex.pHandle, ms);
}

void ConditionVariable::Set() { WakeConditionVariable((PCONDITION_VARIABLE)pHandle); }

void ConditionVariable::SetAll() { WakeAllConditionVariable((PCONDITION_VARIABLE)pHandle); }

ThreadID Thread::mainThreadID;

void Thread::SetMainThread() { mainThreadID = GetCurrentThreadID(); }

ThreadID Thread::GetCurrentThreadID() { return GetCurrentThreadId(); }

char * thread_name()
{
	__declspec(thread) static char name[MAX_THREAD_NAME_LENGTH + 1];
	return name;
}

void Thread::GetCurrentThreadName(char * buffer, int size)
{
	if (const char* name = thread_name())
		snprintf(buffer, (size_t)size, "%s", name);
	else
		buffer[0] = 0;
}

void Thread::SetCurrentThreadName(const char * name) { strcpy_s(thread_name(), MAX_THREAD_NAME_LENGTH + 1, name); }

bool Thread::IsMainThread() { return GetCurrentThreadID() == mainThreadID; }

ThreadHandle create_thread(ThreadDesc* pDesc)
{
	ThreadHandle handle = CreateThread(0, 0, ThreadFunctionStatic, pDesc, 0, 0);
	ASSERT(handle != NULL);
	return handle;
}

void destroy_thread(ThreadHandle handle)
{
	ASSERT(handle != NULL);
	WaitForSingleObject((HANDLE)handle, INFINITE);
	CloseHandle((HANDLE)handle);
	handle = 0;
}

void join_thread(ThreadHandle handle) { WaitForSingleObject((HANDLE)handle, INFINITE); }

void Thread::Sleep(unsigned mSec) { ::Sleep(mSec); }

// threading class (Static functions)
unsigned int Thread::GetNumCPUCores(void)
{
	_SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	return systemInfo.dwNumberOfProcessors;
}

#endif