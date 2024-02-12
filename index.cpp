// --------------------------------------------------------
// Program path using network connection port (PFNC)
// License: MIT
//
// Copyright (C) 2024 Anders Lövgren, Xertified AB.
// --------------------------------------------------------

#include <node.h>
#include "lib/scanner.hpp"

namespace PFNC
{
    using v8::Array;
    using v8::Context;
    using v8::Exception;
    using v8::FunctionCallbackInfo;
    using v8::Integer;
    using v8::Isolate;
    using v8::Local;
    using v8::Object;
    using v8::String;
    using v8::Template;
    using v8::Value;

    void SetOptions(const FunctionCallbackInfo<Value> &args, Options &options)
    {
        Isolate *isolate = Isolate::GetCurrent();

        Local<Context> context = isolate->GetCurrentContext();
        Local<Object> object = args[0]->ToObject(context).ToLocalChecked();
        Local<Array> properties = object->GetOwnPropertyNames(context).ToLocalChecked();

        for (int i = 0, l = properties->Length(); i < l; i++)
        {
            Local<Value> localKey = properties->Get(context, i).ToLocalChecked();
            Local<Value> localVal = object->Get(context, localKey).ToLocalChecked();

            std::string key = *String::Utf8Value(isolate, localKey);
            std::string val = *String::Utf8Value(isolate, localVal);

            if (key == "origin")
            {
                if (!localVal->IsString())
                {
                    isolate->ThrowException(Exception::TypeError(
                        String::NewFromUtf8(isolate, "Expected string for origin").ToLocalChecked()));
                    return;
                }
                if (val != "local" && val != "remote" && val != "both")
                {
                    isolate->ThrowException(Exception::TypeError(
                        String::NewFromUtf8(isolate, "Invalid argument for origin").ToLocalChecked()));
                    return;
                }

                if (val == "local")
                {
                    options.source.origin = PortOrigin::Local;
                }
                else if (val == "remote")
                {
                    options.source.origin = PortOrigin::Remote;
                }
                else if (val == "both")
                {
                    options.source.origin = PortOrigin::Either;
                }
            }

            if (key == "port")
            {
                if (!localVal->IsNumber())
                {
                    isolate->ThrowException(Exception::TypeError(
                        String::NewFromUtf8(isolate, "Expected number for port").ToLocalChecked()));
                    return;
                }
                else
                {
                    options.source.port = static_cast<uint16_t>(localVal->ToNumber(context).ToLocalChecked()->Value());
                }
            }
        }

        if (options.runtime.debug)
        {
            std::cout << options << std::endl;
        }
    }

    void SetArrayEntry(Local<Array> &array, const Netstat::Entry &entry, const Options &options, bool local, int index)
    {
        Isolate *isolate = Isolate::GetCurrent();

        Local<Context> context = isolate->GetCurrentContext();
        Local<Object> object = Object::New(isolate);

        if (options.output.origin)
        {
            object->Set(context,
                        String::NewFromUtf8(isolate, "origin").ToLocalChecked(),
                        String::NewFromUtf8(isolate, local ? "client" : "server").ToLocalChecked())
                .Check();
            {
            }
        }
        if (options.output.proto)
        {
            object->Set(context,
                        String::NewFromUtf8(isolate, "proto").ToLocalChecked(),
                        String::NewFromUtf8(isolate, entry.proto.c_str()).ToLocalChecked())
                .Check();
        }
        if (options.output.port)
        {
            object->Set(context,
                        String::NewFromUtf8(isolate, "port").ToLocalChecked(),
                        Integer::New(isolate, local ? entry.local.port : entry.foreign.port))
                .Check();
        }
        if (options.output.pid)
        {
            object->Set(context,
                        String::NewFromUtf8(isolate, "pid").ToLocalChecked(),
                        Integer::New(isolate, entry.pid))
                .Check();
        }
        if (options.output.path)
        {
            object->Set(context,
                        String::NewFromUtf8(isolate, "path").ToLocalChecked(),
                        String::NewFromUtf8(isolate, entry.path.c_str()).ToLocalChecked())
                .Check();
        }

        array->Set(context, index, object).Check();
    }

    void SetResult(const FunctionCallbackInfo<Value> &args, const Options &options)
    {
        Isolate *isolate = Isolate::GetCurrent();

        Local<Array> array = Array::New(isolate);

        int index = 0;
        Scanner scanner(options);
        scanner.start([&array, &options, &index](const Netstat::Entry &entry)
                      { 
                        if (options.runtime.debug)
                        {
                            std::cout << entry << std::endl;
                        }

                        if (options.source.port == entry.local.port && (
                            options.source.origin == PortOrigin::Local ||
                            options.source.origin == PortOrigin::Either))
                        {
                            SetArrayEntry(array, entry, options, false, index++); 
                        }
                        if (options.source.port == entry.foreign.port && (
                            options.source.origin == PortOrigin::Remote ||
                            options.source.origin == PortOrigin::Either))
                        {
                            SetArrayEntry(array, entry, options, true, index++); 
                        } });

        args.GetReturnValue().Set(array);
    }

    void Scan(const FunctionCallbackInfo<Value> &args)
    {
        Isolate *isolate = Isolate::GetCurrent();

        if (args.Length() != 1)
        {
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(isolate, "Missing options object").ToLocalChecked()));
            return;
        }

        if (!args[0]->IsObject())
        {
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(isolate, "Expected object argument").ToLocalChecked()));
            return;
        }

        Options options;

        SetOptions(args, options);
        SetResult(args, options);
    }

    void Initialize(Local<Object> exports, Local<Value>, void *)
    {
        NODE_SET_METHOD(exports, "scan", Scan);
    }

    NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
}
