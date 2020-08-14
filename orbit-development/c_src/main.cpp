#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <string>

#include <emscripten.h>
#include "soracom/orbit.h"

#include "nlohmann/json.hpp"
using nlohmann::json;

int32_t uplink_body();

extern "C" {

EMSCRIPTEN_KEEPALIVE
int32_t uplink() {
    soracom_log("hello, orbit!\n");

    return uplink_body();
}

}


int32_t uplink_body() {
    const char* buf = NULL;
    size_t siz = 0;
    int32_t err = soracom_get_input_buffer_as_string(&buf, &siz);
    if (err < 0) {
        return err;
    }

    soracom_log("received data: %s\n", buf);

    json j = json::parse(buf);
    soracom_release_input_buffer(buf);

    j["singleClick"] = 0;
    j["doubleClick"] = 0;
    j["longClick"] = 0;

    if(j["clickType"]==1){
        j["singleClick"] = 1;
    }else if(j["clickType"]==2){
        j["doubleClick"] = 1;
    }else if(j["clickType"]==3){
        j["longClick"] = 1;
    }

    std::string output = j.dump();
    soracom_set_json_output(output.c_str(), output.size());

    return 0;
}