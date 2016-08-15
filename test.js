var version = require('./build/Release/qconf.node').version();
console.log(version);

var conf = require('./build/Release/qconf.node').get_conf('/qconf/demo');
console.log(conf);

var children = require('./build/Release/qconf.node').get_batch_keys('/qconf/demo');
console.log(children);

var children_confs = require('./build/Release/qconf.node').get_batch_conf('/qconf/demo');
console.log(children_confs);

var host = require('./build/Release/qconf.node').get_host('/qconf/demo/test/hosts/host2');
console.log(host);
