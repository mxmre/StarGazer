#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <regex>

#include <vector>
#include <array>
#include <list>
#include <set>
#include <map>
#include <queue>
#include <deque>

#include <memory>
#include <thread>
#include <mutex>
#include <type_traits>

#include <future>
#include <algorithm>
#include <functional>
#include <condition_variable>
#include <stdexcept>
#include <chrono>

#include <cmath>
#include <ctime>
#include <cstdint>

#define WIN32_NO_STATUS
#define WIN32_LEAN_AND_MEAN
#pragma warning(disable : 4005)
#include <windows.h>

#include <d3d11.h>
#include <d3dx11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#ifdef _DEBUG
#	pragma comment(lib, "d3dx11d.lib")
#else
#	pragma comment(lib, "d3dx11.lib")
#endif

#include "macros.h"