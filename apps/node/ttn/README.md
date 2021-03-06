# ttn

This is the Node.js client for The Things Network.
You can use this to get events and messages from your The Things
Network applications.

## Installation

```
npm install --save ttn
```

## Usage

Set up the client and start listening to events:

```js
var ttn = require('ttn');

var appEUI = '<insert AppEUI>';
var accessKey = '<insert App Access Key>';
var client = new ttn.Client('staging.thethingsnetwork.org', appEUI, accessKey);

client.on('connect', function() {
  console.log('connected')
})

client.on('uplink', function (msg) {
  // These are messages sent by devices on The Things Network
  // msg = {
  //   devEUI: '00000000973572D0',
  //   fields: { /* ... */ },
  //   counter: 44,
  //   metadata: { /* ... */ },
  // }
})

client.on('activation', function (evt) {
  // These are devices that are activated on The Things Network
  // evt = {
  //   devEUI: '00000000973572D0',
  // }
})

client.on('error', function (err) {
  // TODO: Handle error
})

// Send a downlink message to the device
var payload = new Buffer('01AB', 'hex')
client.downlink('00000000973572D0', payload, '1h', 1)

// End the client connection
client.end()
```

The `fields` on the `uplink` event will be either filled with:

- The result of the payload function tranformation if there are payload
  functions set for the application, or
- The plain, untransformed payload in the `raw` property

## API

### `Client(<broker>, <appEUI>, <accessKey>)`

Creates a new client that listens to events for your application.

Arguments:
- `broker` (`string`): the Things Network Handler's MQTT broker
- `appEUI` (`string`): the EUI for your application
- `accessKey` (`string`): the application's access key

### '#downlink(<devEUI>, <payload>, <ttl>, <port>)`

Send a message to a device.

Arguments:
- `devEUI` (`string`): the DevEUI of the device
- `payload` (`Buffer`): buffer containing binary data
- `ttl` (`string`): time-to-live of the message (e.g. `1h`, `2d`, etc). If there's no opportunity to send the message within this window, the message will be dropped. Default: `1h`
- `port` (`int`): port (default: `1`) 
