#pragma once

#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
//SPECIFIC ON WINDOW
#include <Windows.h>

#include <iostream>
#include <string>
#include <functional>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <memory>
#include <cmath>
#include <string_view>

//STL LIB
#include <initializer_list>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <unordered_set>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef ACTIVATE_LOGGING
#include "DefinedLog/EngineLog.h"
#else
#include "UnactiveLog.h"
#endif
