#include "orbit.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const size_t N = 1024;

void soracom_log(const char* fmt, ...) {
    va_list ap;
    char buf[N] = {};
    va_start(ap, fmt);
    int n = vsnprintf(buf, N, fmt, ap);
    va_end(ap);

    if (n > 0) {
        orbit_log(buf, (size_t)(n));
    }
}


/**
 * Get the input buffer as a null terminated string.
 *
 * @param buf [out] A pointer to the allocated memory.
 * @param siz [out] The length of the string.
 *
 * @note The pointer received in buf should be released by calling soracom_release_input_buffer().
 *
 * @return ERR_OK: successfully retrieved the input buffer.
 *         ERR_INVALID_ARG: buf or siz is null.
 */
int32_t soracom_get_input_buffer_as_string(const char** buf, size_t* siz) {
    if ((buf == NULL) || (siz == NULL)) {
        return ERR_INVALID_ARG;
    }

    *siz = orbit_get_input_buffer_len();
    *buf = (const char*) malloc(*siz + 1);
    memset((void*)(*buf), 0, *siz + 1);
    orbit_get_input_buffer(*buf, *siz);
    return ERR_OK;
}

/**
 * Get the input buffer.
 *
 * @param buf [out] A pointer to the allocated memory.
 * @param siz [out] The length of the string.
 *
 * @note The pointer received in buf should be released by calling soracom_release_input_buffer().
 *
 * @return ERR_OK: successfully retrieved the input buffer.
 *         ERR_INVALID_ARG: buf or siz is null.
 */
int32_t soracom_get_input_buffer(const char** buf, size_t* siz) {
    if ((buf == NULL) || (siz == NULL)) {
        return ERR_INVALID_ARG;
    }

    *siz = orbit_get_input_buffer_len();
    *buf = (const char*) malloc(*siz);
    orbit_get_input_buffer(*buf, *siz);
    return ERR_OK;
}

/**
 * Release the input buffer.
 *
 * @param buf [in] The pointer to the allocated input buffer.
 *
 * @note The pointer buf is allocated by either soracom_get_input_buffer() or soracom_get_input_buffer_as_string().
 */
void soracom_release_input_buffer(const char* buf) {
    free((void*)(buf));
}

/**
 * Get a tag value corresponding to the specified tag name.
 *
 * @param name [in] The name of the tag.
 * @param name_len [in] The length of the name.
 * @param value [out] A pointer to the value of the tag.
 * @param value_len [out] The length of the value.
 *
 * @return ERR_OK: successfully retrieved the tag value.
 *         ERR_INVALID_ARG: name, value or value_len is null.
 */
int32_t soracom_get_tag_value(const char* name, size_t name_len, const char** value, size_t* value_len) {
    if ((name == NULL) || (value == NULL) || (value_len == NULL)) {
        return ERR_INVALID_ARG;
    }

    *value_len = orbit_get_tag_value_len(name, name_len);
    *value = (const char*) malloc(*value_len + 1);
    memset((void*)(*value), 0, *value_len + 1);
    orbit_get_tag_value(name, name_len, *value, *value_len);
    return ERR_OK;
}

/**
 * Get a source value corresponding to the specified name.
 *
 * @param name [in] The name of the source value.
 * @param name_len [in] The length of the name.
 * @param value [out] A pointer to the source value.
 * @param value_len [out] The length of the value.
 *
 * @return ERR_OK: successfully retrieved the source value.
 *         ERR_INVALID_ARG: name, value or value_len is null.
 */
int32_t soracom_get_source_value(const char* name, size_t name_len, const char** value, size_t* value_len) {
    if ((name == NULL) || (value == NULL) || (value_len == NULL)) {
        return ERR_INVALID_ARG;
    }

    *value_len = orbit_get_source_value_len(name, name_len);
    *value = (const char*) malloc(*value_len + 1);
    memset((void*)(*value), 0, *value_len + 1);
    orbit_get_source_value(name, name_len, *value, *value_len);
    return ERR_OK;
}

/**
 * Get the userdata as a null terminated string.
 *
 * @param buf [out] A pointer to the allocated memory.
 * @param siz [out] The length of the string.
 *
 * @note The pointer received in buf should be released by calling soracom_release_userdata().
 *
 * @return ERR_OK: successfully retrieved the userdata.
 *         ERR_INVALID_ARG: buf or siz is null.
 */
int32_t soracom_get_userdata_as_string(const char** buf, size_t* siz) {
    if ((buf == NULL) || (siz == NULL)) {
        return ERR_INVALID_ARG;
    }

    *siz = orbit_get_userdata_len();
    *buf = (const char*) malloc(*siz + 1);
    memset((void*)(*buf), 0, *siz + 1);
    orbit_get_userdata(*buf, *siz);
    return ERR_OK;
}

/**
 * Release the userdata.
 *
 * @param buf [in] The pointer to the allocated userdata.
 *
 * @note The pointer buf is allocated by soracom_get_userdata_as_string().
 */
void soracom_release_userdata(const char* buf) {
    free((void*)(buf));
}

/**
 * Set output data with Content-Type: application/json.
 *
 * @param buf [in] A pointer to the output data.
 * @param siz [in] The length of the output data.
 */
void soracom_set_json_output(const char* buf, size_t siz) {
    const char* content_type = "application/json";
    orbit_set_output_content_type(content_type, strlen(content_type));

    orbit_set_output(buf, siz);
}
