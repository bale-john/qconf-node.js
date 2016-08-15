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

    if (args.Length() < 1) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }
    //Nan::Utf8String s0(args[0]);
    String::Utf8Value v8Path(args[0]);
    path = *v8Path;

    if (args.Length() >= 2) {
        String::Utf8Value v8Idc(args[1]);
        idc = *v8Idc;
    }

    ret = qconf_get_conf(path, value, sizeof(value), idc);

    if (ret != QCONF_OK) {
        ThrowException(Exception::TypeError(String::New("Get conf failed")));
        return scope.Close(Undefined());;
    }
    return scope.Close(String::New(value));
}


void init(Handle<Object> target) {
    target->Set(String::NewSymbol("version"),
            FunctionTemplate::New(QConfVersion)->GetFunction());

    target->Set(String::NewSymbol("get_conf"),
            FunctionTemplate::New(QConfGetConf)->GetFunction());
}
NODE_MODULE(qconf, init)
