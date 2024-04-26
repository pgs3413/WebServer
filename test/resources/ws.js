#!/usr/local/bin/node

let i = 1;

process.stdin.on('data', function(data){
    console.log("收到消息: " + data);
});

setInterval(function(){
    console.error(i++);
}, 2000);
