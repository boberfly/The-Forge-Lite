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

#include "Renderer/Interfaces/IOperatingSystem.h"
#include "OS/Math/MathTypes.h"

struct ButtonData;
class VirtualJoystickUI;

struct CameraMotionParameters
{
	float maxSpeed;
	float acceleration;    // only used with binary inputs such as keypresses
	float braking;         // also acceleration but orthogonal to the acceleration vector
};

class ICameraController
{
	public:
	virtual ~ICameraController() {}
	virtual void setMotionParameters(const CameraMotionParameters&) = 0;
	virtual bool onInputEvent(const ButtonData* pData) = 0;
	virtual void update(float deltaTime) = 0;

	// there are also implicit dependencies on the keyboard state.

	virtual mat4 getViewMatrix() const = 0;
	virtual vec3 getViewPosition() const = 0;
	virtual vec2 getRotationXY() const = 0;
	virtual void setVirtualJoystick(VirtualJoystickUI* virtualJoystick = NULL) = 0;
	virtual void moveTo(const vec3& location) = 0;
	virtual void lookAt(const vec3& lookAt) = 0;
};

/// \c createGuiCameraController assumes that the camera is not rotated around the look direction;
/// in its matrix, \c Z points at \c startLookAt and \c X is horizontal.
ICameraController* createGuiCameraController(vec3 startPosition, vec3 startLookAt);

/// \c createFpsCameraController does basic FPS-style god mode navigation; conf_free-look is constrained
/// to about +/- 88 degrees and WASD translates in the camera's local XZ plane.
ICameraController* createFpsCameraController(vec3 startPosition, vec3 startLookAt);

void destroyCameraController(ICameraController* pCamera);
