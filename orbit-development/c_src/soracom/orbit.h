#ifndef __SORACOM_ORBIT_SDK__ORBIT_H__
#define __SORACOM_ORBIT_SDK__ORBIT_H__

#define ERR_OK          (0)
#define ERR_INVALID_ARG (-1)

#if __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

void orbit_log(const char* message, size_t siz);
size_t orbit_get_input_buffer_len();
size_t orbit_get_input_buffer(const char* buf, size_t siz);
size_t orbit_get_tag_value_len();
size_t orbit_get_tag_value(const char* name, size_t name_len, const char* value, size_t value_len);
size_t orbit_get_source_value_len();
size_t orbit_get_source_value(const char* name, size_t name_len, const char* value, size_t value_len);
size_t orbit_get_userdata_len();
size_t orbit_get_userdata(const char* value, size_t value_len);
int32_t orbit_has_location();
double orbit_get_location_lat();
double orbit_get_location_lon();
int64_t orbit_get_timestamp();
void orbit_set_output(const char* buf, size_t siz);
void orbit_set_output_content_type(const char* buf, size_t siz);

void soracom_log(const char* fmt, ...);
int32_t soracom_get_input_buffer_as_string(const char** buf, size_t* siz);
int32_t soracom_get_input_buffer(const char** buf, size_t* siz);
void soracom_release_input_buffer(const char* buf);
int32_t soracom_get_tag_value(const char* name, size_t name_len, const char** value, size_t* value_len);
int32_t soracom_get_source_value(const char* name, size_t name_len, const char** value, size_t* value_len);
int32_t soracom_get_userdata_as_string(const char** buf, size_t* siz);
void soracom_release_userdata(const char* buf);
void soracom_set_json_output(const char* buf, size_t siz);

#if __cplusplus
}
#endif


#endif /* __SORACOM_ORBIT_SDK__ORBIT_H__ */