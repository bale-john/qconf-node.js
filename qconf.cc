#include <node.h>
#include "qconf.h"
#include <v8.h>
using namespace v8;  
Handle<Value> QConfVersion(const Arguments& args) {
    HandleScope scope;
    return scope.Close(String::New(QCONF_DRIVER_CC_VERSION));
}

void init(Handle<Object> target) {
    target->Set(String::NewSymbol("version"),
            FunctionTemplate::New(QConfVersion)->GetFunction());
}
NODE_MODULE(qconf, init)
