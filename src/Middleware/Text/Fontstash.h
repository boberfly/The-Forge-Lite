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

#pragma once
#include "OS/Math/MathTypes.h"
#include "Renderer/Interfaces/IFileSystem.h"

struct Renderer;

extern FSRoot FSR_MIDDLEWARE_TEXT;

typedef struct TextDrawDesc
{
	TextDrawDesc(uint font = 0, uint32_t color = 0xffffffff, float size = 15.0f, float spacing = 0.0f, float blur = 0.0f):
		mFontID(font),
		mFontColor(color),
		mFontSize(size),
		mFontSpacing(spacing),
		mFontBlur(blur)
	{
	}

	uint32_t mFontID;
	uint32_t mFontColor;
	float    mFontSize;
	float    mFontSpacing;
	float    mFontBlur;
} TextDrawDesc;

class Fontstash
{
	public:
	Fontstash(Renderer* renderer, int width, int height);
	void destroy();

	//! Makes a font available to the font stash.
	//! - Fonts can not be undefined in a FontStash due to its dynamic nature (once packed into an atlas, they cannot be unpacked, unless it is fully rebuilt)
	//! - Defined fonts will automatically be unloaded when the Fontstash is destroyed.
	//! - When it is paramount to be able to unload individual fonts, use multiple fontstashes.
	int defineFont(const char* identification, const char* filename, uint32_t root);

	//! Find a font by user defined identification.
	int getFontID(const char* identification);

	const char* getFontName(const char* identification);
	void*       getFontBuffer(const char* identification);
	uint32_t    getFontBufferSize(const char* identification);

	//! Draw text.
	void drawText(
		struct Cmd* pCmd, const char* message, float x, float y, int fontID, unsigned int color = 0xffffffff, float size = 16.0f,
		float spacing = 0.0f, float blur = 0.0f);

	//! Draw text in worldSpace.
	void drawText(
		struct Cmd* pCmd, const char* message, const mat4& projView, const mat4& worldMat, int fontID, unsigned int color = 0xffffffff,
		float size = 16.0f, float spacing = 0.0f, float blur = 0.0f);

	//! Measure text boundaries. Results will be written to out_bounds (x,y,x2,y2).
	float measureText(
		float* out_bounds, const char* message, float x, float y, int fontID, unsigned int color = 0xffffffff, float size = 16.0f,
		float spacing = 0.0f, float blur = 0.0f);

	protected:
	float                  m_fFontMaxSize;
	class _Impl_FontStash* impl;
};
