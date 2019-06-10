#pragma once

#ifndef TARGET_IOS
#include "EASTL/string.h"
#include "Interfaces/ILogManager.h"
#include "Interfaces/IFileSystem.h"
#include "Renderer/IRenderer.h"

static bool parseConfigLine(
	eastl::string line, eastl::string& vendorId, eastl::string& deviceId, eastl::string& revId, eastl::string& deviceName,
    eastl::string& presetLevel)
{
	auto parseNext = [](eastl::string line, size_t& it) {
		if (it == eastl::string::npos) return eastl::string();
		size_t prev = it;
		it = line.find_first_of(';', it);
		it += (it != eastl::string::npos);
		return line.substr(prev, it == eastl::string::npos ? eastl::string::npos : it - prev - 1);
	};

	line.trim();
	//don't parse if commented line
	if (line.empty() || line.at(0) == '#')
		return false;

	size_t pos = 0;

	vendorId = parseNext(line, pos);
	if (pos == eastl::string::npos)
		return false;

	deviceId = parseNext(line, pos);
	if (pos == eastl::string::npos)
		return false;

	presetLevel = parseNext(line, pos);
	deviceName = parseNext(line, pos);
	revId = parseNext(line, pos);

	vendorId.trim();
	vendorId.make_lower();
	deviceId.trim();
	deviceId.make_lower();
	presetLevel.trim();
	presetLevel.make_lower();
	deviceName.trim();
	revId.trim();

	if (revId.empty())
		revId = "0x00";

	return true;
}

static GPUPresetLevel stringToPresetLevel(eastl::string& presetLevel)
{
	if (presetLevel == "office")
		return GPU_PRESET_OFFICE;
	if (presetLevel == "low")
		return GPU_PRESET_LOW;
	if (presetLevel == "medium")
		return GPU_PRESET_MEDIUM;
	if (presetLevel == "high")
		return GPU_PRESET_HIGH;
	if (presetLevel == "ultra")
		return GPU_PRESET_ULTRA;

	return GPU_PRESET_NONE;
}

#ifndef METAL
static GPUPresetLevel
	getSinglePresetLevel(eastl::string line, const eastl::string& inVendorId, const eastl::string& inModelId, const eastl::string& inRevId)
{
	eastl::string vendorId;
	eastl::string deviceId;
	eastl::string presetLevel;
	eastl::string gpuName;
	eastl::string revisionId;

	if (!parseConfigLine(line, vendorId, deviceId, revisionId, gpuName, presetLevel))
		return GPU_PRESET_NONE;

	//check if current vendor line is one of the selected gpu's
	//compare both ModelId and VendorId
	if (inVendorId == vendorId && inModelId == deviceId)
	{
		//if we have a revision Id then we want to match it as well
		if (inRevId != "0x00" && revisionId != "0x00" && inRevId != revisionId)
			return GPU_PRESET_NONE;

		return stringToPresetLevel(presetLevel);
	}

	return GPU_PRESET_NONE;
}
#endif

#ifndef __ANDROID__
//TODO: Add name matching as well.
static void checkForPresetLevel(eastl::string line, Renderer* pRenderer)
{
	eastl::string vendorId;
	eastl::string deviceId;
	eastl::string presetLevel;
	eastl::string gpuName;
	eastl::string revisionId;

	if (!parseConfigLine(line, vendorId, deviceId, revisionId, gpuName, presetLevel))
		return;

	//search if any of the current gpu's match the current gpu cfg entry
	for (uint32_t i = 0; i < pRenderer->mNumOfGPUs; i++)
	{
		GPUSettings* currentSettings = &pRenderer->mGpuSettings[i];
		//check if current vendor line is one of the selected gpu's
		//compare both ModelId and VendorId
		if (strcmp(currentSettings->mGpuVendorPreset.mVendorId, vendorId.c_str()) == 0 &&
			strcmp(currentSettings->mGpuVendorPreset.mModelId, deviceId.c_str()) == 0)
		{
			//if we have a revision Id then we want to match it as well
			if (strcmp(currentSettings->mGpuVendorPreset.mRevisionId, "0x00") != 0 && revisionId.compare("0x00") != 0 &&
				strcmp(currentSettings->mGpuVendorPreset.mRevisionId, revisionId.c_str()) == 0)
				continue;

			currentSettings->mGpuVendorPreset.mPresetLevel = stringToPresetLevel(presetLevel);

			//Extra information for GPU
			//Not all gpu's will have that info in the gpu.cfg file
			strncpy(currentSettings->mGpuVendorPreset.mGpuName, gpuName.c_str(), MAX_GPU_VENDOR_STRING_LENGTH);
		}
	}
}
#endif

#ifndef METAL
#ifndef __ANDROID__
static bool checkForActiveGPU(eastl::string line, GPUVendorPreset& pActiveGpu)
{
	eastl::string vendorId;
	eastl::string deviceId;
	eastl::string presetLevel;
	eastl::string gpuName;
	eastl::string revisionId;

	if (!parseConfigLine(line, vendorId, deviceId, revisionId, gpuName, presetLevel))
		return false;

	strncpy(pActiveGpu.mModelId, deviceId.c_str(), MAX_GPU_VENDOR_STRING_LENGTH);
	strncpy(pActiveGpu.mVendorId, vendorId.c_str(), MAX_GPU_VENDOR_STRING_LENGTH);
	strncpy(pActiveGpu.mGpuName, gpuName.c_str(), MAX_GPU_VENDOR_STRING_LENGTH);
	strncpy(pActiveGpu.mRevisionId, revisionId.c_str(), MAX_GPU_VENDOR_STRING_LENGTH);

	//TODO: Hardcoded for now as its only used for automated testing
	//We will want to test with different presets
	pActiveGpu.mPresetLevel = GPU_PRESET_ULTRA;

	return true;
}
#endif
#endif

#ifndef __ANDROID__
//Reads the gpu config and sets the preset level of all available gpu's
static void setGPUPresetLevel(Renderer* pRenderer)
{
	File gpuCfgFile = {};
	gpuCfgFile.Open("gpu.cfg", FM_ReadBinary, FSR_GpuConfig);
	if (!gpuCfgFile.IsOpen())
	{
		LOGF(LogLevel::eWARNING, "gpu.cfg could not be found, setting preset to Low as a default.");
		return;
	}

	while (!gpuCfgFile.IsEof())
	{
		eastl::string gpuCfgString = gpuCfgFile.ReadLine();
		checkForPresetLevel(gpuCfgString, pRenderer);
		// Do something with the tok
	}

	gpuCfgFile.Close();
}
#endif


#ifdef __ANDROID__
//Reads the gpu config and sets the preset level of all available gpu's
static GPUPresetLevel getGPUPresetLevel(const eastl::string vendorId, const eastl::string modelId, const eastl::string revId)
{
	LOGF(LogLevel::eINFO, "No gpu.cfg support. Preset set to Low");
	GPUPresetLevel foundLevel = GPU_PRESET_LOW;
	return foundLevel;
}
#endif


#if !defined(METAL) && !defined(__ANDROID__)
//Reads the gpu config and sets the preset level of all available gpu's
static GPUPresetLevel getGPUPresetLevel(const eastl::string vendorId, const eastl::string modelId, const eastl::string revId)
{
	File gpuCfgFile = {};
	gpuCfgFile.Open("gpu.cfg", FM_ReadBinary, FSR_GpuConfig);
	if (!gpuCfgFile.IsOpen())
	{
		LOGF(LogLevel::eWARNING, "gpu.cfg could not be found, setting preset to Low as a default.");
		return GPU_PRESET_LOW;
	}

	GPUPresetLevel foundLevel = GPU_PRESET_LOW;

	while (!gpuCfgFile.IsEof())
	{
		eastl::string  gpuCfgString = gpuCfgFile.ReadLine();
		GPUPresetLevel level = getSinglePresetLevel(gpuCfgString, vendorId, modelId, revId);
		// Do something with the tok
		if (level != GPU_PRESET_NONE)
		{
			foundLevel = level;
			break;
		}
	}

	gpuCfgFile.Close();
	return foundLevel;
}

#if defined(AUTOMATED_TESTING) && defined(ACTIVE_TESTING_GPU)
static bool getActiveGpuConfig(GPUVendorPreset& pActiveGpu)
{
	File gpuCfgFile = {};
	gpuCfgFile.Open("activeTestingGpu.cfg", FM_ReadBinary, FSR_GpuConfig);
	if (!gpuCfgFile.IsOpen())
	{
		LOGF(LogLevel::eINFO, "activeTestingGpu.cfg could not be found, Using default GPU.");
		return false;
	}

	bool successFinal = false;
	while (!gpuCfgFile.IsEof() && !successFinal)
	{
		eastl::string gpuCfgString = gpuCfgFile.ReadLine();
		successFinal = checkForActiveGPU(gpuCfgString, pActiveGpu);
	}

	gpuCfgFile.Close();

	return successFinal;
}
#endif
#endif
#endif
