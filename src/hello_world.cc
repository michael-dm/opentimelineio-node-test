#include "hello_world.h"

#include <opentimelineio/clip.h>
#include <opentimelineio/deserialization.h>
#include <opentimelineio/effect.h>
#include <opentimelineio/freezeFrame.h>
#include <opentimelineio/gap.h>
#include <opentimelineio/linearTimeWarp.h>
#include <opentimelineio/timeline.h>
#include <opentimelineio/transition.h>

#include <iostream>

using namespace Napi;
namespace otio = opentimelineio::OPENTIMELINEIO_VERSION;

void print_error(Napi::Env env, otio::ErrorStatus const &error_status)
{
    Napi::TypeError::New(env, otio::ErrorStatus::outcome_to_string(error_status.outcome) + ": " + error_status.details)
        .ThrowAsJavaScriptException();
}

Timeline::Timeline(const Napi::CallbackInfo &info) : ObjectWrap(info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1)
    {
        Napi::TypeError::New(env, "Wrong number of arguments")
            .ThrowAsJavaScriptException();
        return;
    }

    if (!info[0].IsString())
    {
        Napi::TypeError::New(env, "You need to name yourself")
            .ThrowAsJavaScriptException();
        return;
    }

    otio::ErrorStatus error_status;
    otio::SerializableObject::Retainer<otio::Timeline> timeline(dynamic_cast<otio::Timeline *>(otio::Timeline::from_json_string(info[0].As<Napi::String>().Utf8Value(), &error_status)));
    if (!timeline)
    {
        print_error(env, error_status);
        return;
    }

    this->_greeterName = info[0].As<Napi::String>().Utf8Value();
}

Napi::Value Timeline::Greet(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1)
    {
        Napi::TypeError::New(env, "Wrong number of arguments")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString())
    {
        Napi::TypeError::New(env, "You need to introduce yourself to greet")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    Napi::String name = info[0].As<Napi::String>();

    printf("Hello %s\n", name.Utf8Value().c_str());
    printf("I am %s\n", this->_greeterName.c_str());

    return Napi::String::New(env, this->_greeterName);
}

Napi::Function Timeline::GetClass(Napi::Env env)
{
    return DefineClass(env, "Timeline", {
                                            Timeline::InstanceMethod("greet", &Timeline::Greet),
                                        });
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    Napi::String name = Napi::String::New(env, "Timeline");
    exports.Set(name, Timeline::GetClass(env));
    return exports;
}

NODE_API_MODULE(addon, Init)
