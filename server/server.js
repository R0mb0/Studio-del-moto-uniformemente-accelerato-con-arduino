// import libraries
var SerialPort = require("serialport");
var http = require('http');
var express = require('express');
var app = express();
var server = http.createServer(app).listen(3000);
var io = require('socket.io').listen(server);

// initialize modules
var serialport = new SerialPort("/dev/ttyACM0", {
  baudRate: 115200
}, function (err) {
  if (err) {
    return console.log('Error: Arduino...'+err);
  }
  console.log("Arduino connected!");  
});

// start program
serialport.on('open', function() {
  io.sockets.on('connection', function (socket) {
      //Connecting to client 
      console.log('Socket connected');
      socket.emit('connected'); 

      serialport.on('data', function(data){
        console.log(String.fromCharCode.apply(null, new Uint8Array(data)));

        // send data to client
        socket.emit('data', String.fromCharCode.apply(null, new Uint8Array(data)));
      });
  });
});

  


