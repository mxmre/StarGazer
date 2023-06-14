#pragma once

#define _DELETE(p)		{ if(p){delete (p); (p)=nullptr;} }
#define _DELETE_ARRAY(p)	{ if(p){delete[] (p); (p)=nullptr;} }
#define _RELEASE(p)		{ if(p){(p)->Release(); (p)=nullptr;} }
#define _CLOSE(p)		{ if(p){(p)->Close(); delete (p); (p)=nullptr;} }
#define _SG_TRY_START try {
#define _SG_TRY_END } catch (const sg::exceptions::SGFatalException& exc)\
					{sg::utility::Logger<wchar_t>::Error.Print(exc.what());  sg::exceptions::Crush(exc.code(), exc.what());}\
					catch (const sg::exceptions::SGException& exc) \
					{sg::utility::Logger<wchar_t>::Error.Print(exc.what());}\
					catch (const std::exception& exc) \
					{sg::utility::Logger<char>::Error.Print(exc.what());}\
					catch (...) {sg::utility::Logger<char>::Error.Print("Unknown error catched!");}