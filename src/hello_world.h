#pragma once

#include <napi.h>

class Timeline : public Napi::ObjectWrap<Timeline>
{
public:
    Timeline(const Napi::CallbackInfo &);
    Napi::Value Greet(const Napi::CallbackInfo &);

    static Napi::Function GetClass(Napi::Env);

private:
    std::string _greeterName;
};
