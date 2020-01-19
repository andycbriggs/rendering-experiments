#pragma once

// STL
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <algorithm>
#include <cmath>
#include <codecvt>
#include <cstdint>
#include <deque>
#include <exception>
#include <locale>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

// Windows
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// Windows Runtime
#include <wrl.h>
using Microsoft::WRL::ComPtr;

// DXGI
#include <dxgi1_2.h>
#pragma comment(lib, "dxgi.lib")

// DirectX
#include <d3d11_1.h>
#pragma comment(lib, "d3d11.lib")

// Effects 11
#include <d3dx11effect.h>

// Basic Types
using Int32 = int32_t;
using UInt32 = uint32_t;
using Int64 = int64_t;
using UInt64 = uint64_t;