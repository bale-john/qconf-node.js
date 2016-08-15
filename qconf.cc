#include <node.h>
#include "node_modules/nan/nan.h"
#include "qconf.h"
#include <v8.h>

using namespace v8;  

Handle<Value> QConfVersion(const Arguments& args) {
    HandleScope scope;
    return scope.Close(String::New(QCONF_DRIVER_CC_VERSION));
}
Handle<Value> QConfGetConf(const Arguments& args) {
    HandleScope scope;

    const char *path = NULL;
    const char *idc = NULL;
    char value[QCONF_CONF_BUF_MAX_LEN];
    int ret;
/*
    if (info.Length() < 1) {
        Nan::ThrowTypeError("Wrong number of arguments: at least 1");
        return;
    }
*/
    Nan::Utf8String s0(args[0]);
    path = *s0;

    if (args.Length() >= 2) {
        Nan::Utf8String s1(args[1]);
        idc = *s1;
    }

    ret = qconf_get_conf(path, value, sizeof(value), idc);
    return scope.Close(String::New(value));

    /*
    if (ret != QCONF_OK) {
        info.GetReturnValue().Set(Nan::Null());
        return;
    }

    info.GetReturnValue().Set(Nan::New(value).ToLocalChecked());
    */
}


void init(Handle<Object> target) {
    target->Set(String::NewSymbol("version"),
            FunctionTemplate::New(QConfVersion)->GetFunction());

    target->Set(String::NewSymbol("get_conf"),
            FunctionTemplate::New(QConfGetConf)->GetFunction());
}
NODE_MODULE(qconf, init)
