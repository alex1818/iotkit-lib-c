var iotkit = require('./src/javascript/iotkit');

console.log('iotkit library version:', iotkit.iotkit_get_version());

var response = iotkit.getUserJwtToken('pradeep.chenthati@aricent.com', 'Password1');
console.log(response);