//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2026
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "core/nh3api_std/call_macros.hpp"

inline void VideoSoundOnOff()
{ FASTCALL_1(void, 0x5974B0, 1); }

inline void VideoRealignBuffers()
{ FASTCALL_0(void, 0x5974F0); }

inline int32_t VideoPlay(int32_t id, int32_t x, int32_t y, int32_t h, int32_t w)
{ return FASTCALL_5(int32_t, 0x5975D0, id, x, y, h, w); }

inline void VideoOpen(int32_t id, int32_t x, int32_t y, int32_t w, int32_t h, bool loop, bool drawRects, bool play)
{ FASTCALL_8(void, 0x597870, id, x, y, w, h, static_cast<bool32_t>(loop), static_cast<bool32_t>(drawRects), static_cast<bool32_t>(play)); }

inline void VideoClose()
{ FASTCALL_0(void, 0x5978F0); }

inline void VideoNextFrame()
{ FASTCALL_0(void, 0x5979E0); }

inline void VideoDrawCurrentFrame()
{ FASTCALL_0(void, 0x597A40); }

inline void VideoPause()
{ FASTCALL_0(void, 0x597AA0); }

inline void VideoResume()
{ FASTCALL_0(void, 0x597B50); }

inline void VideoRestart()
{ FASTCALL_0(void, 0x597C00); }

inline bool VideoNeedsUpdate()
{ return FASTCALL_0(bool, 0x597C30); }

inline bool VideoPlaying()
{ return FASTCALL_0(bool, 0x597C90); }

inline void VideoDrawRects()
{ FASTCALL_0(void, 0x597CD0); }

inline void VideoShutDown()
{ FASTCALL_0(void, 0x597F70); }

inline void ShowVideo(int32_t id, int32_t x, int32_t y, int32_t w, int32_t h, int32_t loop, bool drawRects, bool play)
{ FASTCALL_8(void, 0x598DF0, id, x, y, w, h, loop, drawRects, play); }
