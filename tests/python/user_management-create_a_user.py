import iotkit;

print 'iotkit library version:', iotkit.iotkit_get_version();

response = iotkit.createAnUser('pradyp7@gmail.com', 'Password1');
print 'Response received: ', response;
