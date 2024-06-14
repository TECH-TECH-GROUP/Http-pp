#include "json.h"

namespace Server{
    void Listen();
    // Route, Type
    short HandleRequest();

    // Make it easy to add custom logic (callbacks?)
    short HandlePost();
    short HandleGet();
}

namespace Response{
    short RespondJSON();
}