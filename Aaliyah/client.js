const WebSocket = require("ws");
const socket = new WebSocket("ws://localhost:5000");
const readline = require("readline");

//singleton readline interface
let rl;
let message;

/**
 * @description
 * 1. socket.on('open') is triggered when the client is connected to the server
 * 2. socket.on('message') is triggered when the client receives a message from the server
 * 3. socket.on('error') is triggered when the client encounters an error
 */
socket.on("open", () => {
  console.log("Connected to server");

  sendMsg();
});

socket.on("message", (message) => {
  console.log("\nServer: " + message);
  //message trigger for every message received from server for continuous chat
  sendMsg();
});

socket.on("error", () => {
  console.log("Error connecting to server");
});


/**
 * @description
 * Method for sending messages to server
 */
 var sent_date=new Date();

 var sent_hh=String(sent_date.getHours()).padStart(2,'0');
 var sent_mm=String(sent_date.getMinutes()).padStart(2,'0');
 var sent_dd=String(sent_date.getDate()).padStart(2,'0');
 var sent_mm=String(sent_date.getMonth()).padStart(2,'0');
 var sent_yy=String(sent_date.getFullYear());
 var final_sent=sent_hh+':'+sent_mm+'  '+sent_dd+'/'+sent_mm+'/'+sent_yy;
const sendMsg = () => {
  //closing the previous readline
  if (rl) rl.close();

  //creating a new readline
  rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
  });
  rl.question(">> ", (msg) => {
    message = msg;
    console.log("Message sent at: " + final_sent);
    //message trigger for every message sent from client for continuous chat
    socket.send(message);
    sendMsg();
  });
};

