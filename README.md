# node-mqlight (beta)

MQ Light is designed to allow applications to exchange discrete pieces of
information in the form of messages. This might sound a lot like TCP/IP
networking, and MQ Light does use TCP/IP under the covers, but MQ Light takes
away much of the complexity and provides a higher level set of abstractions to
build your applications with.

This Node.js module provides the high-level API by which you can interact
with the MQ Light runtime.

See https://www.ibmdw.net/messaging/mq-light/ for more details.

Current Features:

* Send and receive arbitrary String, Buffer and JSON objects between Node.js
  applications using an at-most-once quality of service.
* Includes samples to demonstrate API usage.

## Getting Started

Install it in node.js:

```
npm install mqlight

OR

npm install https://ibm.biz/node-mqlight
```

```javascript
var mqlight = require('mqlight');
```

Then create some clients to send and receive messages:

```javascript
var recvClient = mqlight.createClient({
  id: 'recv-client-1'
});

var address = "public";
recvClient.on('connected', function() {
  recvClient.subscribe(address);
  recvClient.on('message', function(data, delivery) {
    console.log(data);
  });
});

recvClient.connect();

var sendClient = mqlight.createClient({
  id: 'send-client-1'
});

var topic = "public";
sendClient.on('connected', function() {
  sendClient.send(topic, "Hello World!");
});

sendClient.connect();

```

## API

### mqlight.createClient([`options`])

Creates an MQ Light client instance.

* `options`, (Object)  options for the client. Properties include:

  *  **service**, (String) (required), the URL for the service to connect to.
  *  **id** (String, default: AUTO_[0-9a-f]{7}), a unique identifier for
     this client. A client with a duplicate `id` will be prevented from
     connecting to the messaging service.

Returns a `Client` object representing the client instance. The client is an
event emitter and listeners can be registered for the following events:
`connect`, `disconnect`, `error`, and `message`.

### mqlight.Client.connect([`callback`])

Connects the MQ Light client instance to the service.
* `callback` - (Function) (optional) callback to be notified of errors &
  completion

### mqlight.Client.send(`topic`, `message`, [`options`], [`callback`])

Sends the given MQ Light message object to the specified topic. String and
Buffer messages will be sent and received as-is. Any other Object will be
converted to JSON before sending and automatically parsed back into the same
Object type when received.

* `topic` - (String) the topic to which the message will be sent.
* `message` - (String | Buffer | Object) the message body to be sent
* `options` - (Object) (optional) map of additional options for the send.
  There are no options that can be set in this beta.
* `callback` - (Function) (optional) callback to be notified of errors &
  completion

### mqlight.Client.subscribe(`pattern`, [`share`], [`options`], [`callback`])

Create a `Destination` and associates it with a `pattern`.

The `pattern` argument is matched against the `topic` that messages are
sent to, allowing the messaging service to determine whether a paricular
message will be delivered to a particular `Destination`, and hence
`subscription`.

* `pattern` - (String) used to match against the `topic` specified when a
   message is sent to the messaging service.
* `share` - (String) (optional) name for creating or joining a shared
  subscription for which messages are anycast between connected subscribers. If
  omitted defaults to unshared (e.g. private to the client).
* `options` - (Object) (optional) map of additional options for the destination.
  There are no options that can be set in this beta.
* `callback` - (Function) callback to be notified of errors & completion.

Returns the `Client` object that the subscribe was called on.  `message` events
will be emitted when messages arrive.

### mqlight.Client.getId()

Returns the identifier associated with the client. This will either be what
was passed in on the `Client.createClient` call or an autogenerated id.

### mqlight.Client.getService()

Returns the URL of the service to which the client is currently connected
to, or undefined if not connected.

### mqlight.Client.getState()

Returns the current state of the client, which will be one of:
'connected', 'connecting', 'disconnected' or 'disconnecting'.

### mqlight.Client.disconnect([callback])

Disconnects this Client from the messaging server and frees the system
resources that it uses. Calling this method also implicitly closes any
subscriptions that have been created using the client's
`Client.subscribe` method.

### Event: 'message'

Emitted when a message is delivered from a destination matching one of the
client's subscriptions.

* `data` - (String | Buffer | Object) the message body.
* `delivery` - (Object) additional information about why the event was emitted.
  Properties include:
  *  **message**, (Object) additional information about the message.  Properties
     include:
    *  **topic**, (Object) the topic that the message was sent to.
  *  **subscription**, (Object) information about the `Client.subscribe` method
     call that caused the client to receive this message (note: this isn't
     implemented yet!)

### Event: 'connect'

This event is emitted when a client successfully connects to the messaging
service.

### Event: 'disconnect'

This event is emitted when a client disconnects from the messaging service,
either explicitly, or because the connection between the client and the
service is interrupted.

### Event: 'error'

Emitted when an error is detected that prevents or interrupts a client's
connection to the messaging service.

* `error` (Error) the error.

## Samples

To run the samples, install the module via npm and navigate to the
`mqlight/samples/` folder.

Usage:

Receiver Example:

```
Usage: recv.js [options] <address>
                          address: amqp://<domain>/<name>
                          (default amqp://localhost/public)

Options:
  -h, --help            show this help message and exit
```

Sender Example:

```
Usage: send.js [options] <msg_1> ... <msg_n>

Options:
  -h, --help            show this help message and exit
  -a ADDRESS, --address=ADDRESS
                        address: amqp://<domain>/<name>
                        (default amqp://localhost/public)
  -d NUM, --delay=NUM   add a NUM seconds time delay between each request
```

## Release notes

### 0.1.0

* Initial beta release.
* Support for sending and receiving 'at-most-once' messages.
* Support for wildcard subscriptions
* Support for shared subscriptions.

