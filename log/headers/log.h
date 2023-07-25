#ifdef __cplusplus
namespace dtk::log{
    extern "C"{
#endif

#ifndef _DTK_LOG_H_
#define _DTK_LOG_H_

void log_terminate(int code);
void log_error(const char* msg, int code);
void log_fatal_error(const char* msg, int code);
void log_warning(const char* msg);
void log_info(const char* msg);

#ifdef DTK_LOG_ENABLE_ASSERTION

void log_assert(char b, const char* msg, int code);
void log_assert_errno();

#else

#define log_assert(b, msg, code)
#define log_assert_errno()

#endif

#endif

#ifdef __cplusplus
    }
}
#endif