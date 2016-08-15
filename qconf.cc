#include <node.h>
#include "node_modules/nan/nan.h"
#include "qconf.h"
#include <v8.h>

using namespace v8;  

//gte version
Handle<Value> QConfVersion(const Arguments& args) {
    HandleScope scope;
    return scope.Close(String::New(QCONF_DRIVER_CC_VERSION));
}

//get_conf
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

    String::Utf8Value v8Path(args[0]);
    path = *v8Path;

    if (args.Length() >= 2) {
        String::Utf8Value v8Idc(args[1]);
        idc = *v8Idc;
    }

    ret = qconf_get_conf(path, value, sizeof(value), idc);

    if (ret != QCONF_OK) {
        ThrowException(Exception::TypeError(String::New("Get conf failed")));
        return scope.Close(Undefined());
    }
    return scope.Close(String::New(value));
}

//get children
Handle<Value> QConf_get_batch_keys(const Arguments& args) {
    HandleScope scope;

    const char *path = NULL;
    const char *idc = NULL;
    string_vector_t keys;
    int ret;

    if (args.Length() < 1) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }

    String::Utf8Value v8Path(args[0]);
    path = *v8Path;

    if (args.Length() >= 2) {
        String::Utf8Value v8Idc(args[1]);
        idc = *v8Idc;
    }

    init_string_vector(&keys);
    ret = qconf_get_batch_keys(path, &keys, idc);

    if (ret != QCONF_OK) {
        ThrowException(Exception::TypeError(String::New("Get children failed")));
        return scope.Close(Undefined());
    }

    Handle<Array> v8Arr = Array::New(keys.count);
    for (int i = 0; i < keys.count; ++i) {
        v8Arr->Set(i, String::New(keys.data[i]));
    }
    destroy_string_vector(&keys);
    return scope.Close(v8Arr);
}

//get children and their confs
Handle<Value> QConf_get_batch_conf(const Arguments& args) {
    HandleScope scope;

    const char *path = NULL;
    const char *idc = NULL;
    qconf_batch_nodes nodes;
    int ret;

    if (args.Length() < 1) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }

    String::Utf8Value v8Path(args[0]);
    path = *v8Path;

    if (args.Length() >= 2) {
        String::Utf8Value v8Idc(args[1]);
        idc = *v8Idc;
    }

    init_qconf_batch_nodes(&nodes);
    ret = qconf_get_batch_conf(path, &nodes, idc);

    if (ret != QCONF_OK) {
        ThrowException(Exception::TypeError(String::New("Get children failed")));
        return scope.Close(Undefined());
    }

    Handle<Object> v8Obj = Object::New();
    for (int i = 0; i < nodes.count; ++i) {
        v8Obj->Set(String::New(nodes.nodes[i].key), String::New(nodes.nodes[i].value));
        /*
        if (strcmp(nodes.nodes[i].value, "") == 0) {
            v8Obj->set(String::New(nodes.nodes[i].key), String::New(""));
        }
        */
    }
    destroy_qconf_batch_nodes(&nodes);
    return scope.Close(v8Obj);
}
void init(Handle<Object> target) {
    target->Set(String::NewSymbol("version"),
            FunctionTemplate::New(QConfVersion)->GetFunction());

    target->Set(String::NewSymbol("get_conf"),
            FunctionTemplate::New(QConfGetConf)->GetFunction());

    target->Set(String::NewSymbol("get_batch_keys"),
            FunctionTemplate::New(QConf_get_batch_keys)->GetFunction());

    target->Set(String::NewSymbol("get_batch_conf"),
            FunctionTemplate::New(QConf_get_batch_conf)->GetFunction());
}
NODE_MODULE(qconf, init)
