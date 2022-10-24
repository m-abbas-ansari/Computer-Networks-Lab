const { WebSocketServer } = require("ws");
const wss = new WebSocketServer({ port: 5000 });
const readline = require("readline");

console.log("Listening on port 5000");

//singleton readline
let rl;

/**
 * @description
 * 1. wss.on('connection') is triggered when the client is connected to the server
 * 2. ws.on('message') is triggered when the client sends a message to the server
 * 3. ws.on('close') is triggered when the client is disconnected from the server
 */
 var sent_date=new Date();

 var sent_hh=String(sent_date.getHours()).padStart(2,'0');
 var sent_mm=String(sent_date.getMinutes()).padStart(2,'0');
 var sent_dd=String(sent_date.getDate()).padStart(2,'0');
 var sent_mm=String(sent_date.getMonth()).padStart(2,'0');
 var sent_yy=String(sent_date.getFullYear());
 
 var final_sent=sent_hh+':'+sent_mm+'  '+sent_dd+'/'+sent_mm+'/'+sent_yy;
wss.on("connection", (ws) => {
  console.log("Client connected");

  ws.on("message", (message) => {
    console.log("\nClient: " + message);
    console.log("Message received at: " + final_sent);
    
  });
  ws.on("close", () => {
    console.log("Client disconnected");
  });
});
