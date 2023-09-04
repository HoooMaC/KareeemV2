#pragma once

#ifndef ACTIVATE_LOGGING

#define ENGINE_TRACE(...)
#define ENGINE_CRITICAL(...)
#define ENGINE_INFO(...)
#define ENGINE_DEBUG(...)
#define ENGINE_WARN(...) 
#define ENGINE_ERROR(...)

#define ENGINE_ASSERT(x, ...)
#define ENGINE_FATAL(x, ...)

#endif //ACTIVATE_LOGGGING