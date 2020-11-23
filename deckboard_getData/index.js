const fs = require('fs');
const io = require("socket.io-client");
var ip = require('ip');


var responseJson;

var localIp = ip.address();
const socket = io("http://" + localIp + ":8500", {
	query: {
		access_key: "DCKBRD_PRO_1_3_0"
	}
});

socket.on('reconnect_attempt', () => {
  socket.io.opts.transports = ['polling', 'websocket'];
  console.log("Reconnecting");
});

socket.on('error', (result) => {
  console.log(result);
});

socket.on('connect', function(){
	
	socket.emit("get_shortcuts",{});
	
	socket.on('get_shortcuts', function(response){
		var newJson = {};
		newJson.buttons = [];

		socket.emit("get_version",{});
		
		for(var i = 0; i < response.length;i++){
			if(response[i] != undefined){
				if(response[i]["shortcuts"] != undefined){
					for(var x = 0; x < response[i]["shortcuts"].length; x++){
						if(response[i]["shortcuts"][x] != undefined){
							if(response[i]["shortcuts"][x]["id"] != undefined){
								if(response[i]["shortcuts"][x]["type"] != "board"){
									if(response[i]["shortcuts"][x]["title"] != undefined && response[i]["shortcuts"][x]["title"] != "" && response[i]["shortcuts"][x]["title"] != null){
										newJson.buttons.push({"command_id" : response[i]["shortcuts"][x]["id"],  "type": "socket", "description" : response[i]["shortcuts"][x]["title"]});
									}
									
								}
								
							}
						}
					}
				}
			}
			
		}
		
		var fileName = "data/boards" + makeid (10) +   " .json";
		
		
		fs.writeFile(fileName, JSON.stringify(newJson, null, 4), function(err) {
			if(err) {
			  console.log(err);
			} else {
			  console.log("JSON saved to " + "boards.json");
			}
		}); 
	});
	
	socket.on('refresh_board', function(response){
		var newJson = {};
		newJson.buttons = [];
		
		
		for(var i = 0; i < response.length;i++){
			if(response[i] != undefined){
				if(response[i]["shortcuts"] != undefined){
					for(var x = 0; x < response[i]["shortcuts"].length; x++){
						if(response[i]["shortcuts"][x] != undefined){
							if(response[i]["shortcuts"][x]["id"] != undefined){
								if(response[i]["shortcuts"][x]["type"] != "board"){
									if(response[i]["shortcuts"][x]["title"] != undefined && response[i]["shortcuts"][x]["title"] != "" && response[i]["shortcuts"][x]["title"] != null){
										newJson.buttons.push({"command_id" : response[i]["shortcuts"][x]["id"],  "type": "socket", "description" : response[i]["shortcuts"][x]["title"]});
									}
									
								}
								
							}
						}
					}
				}
			}
			
		}
		
		var fileName = "data/boards" + makeid (10) +   " .json";
		
		
		fs.writeFile(fileName, JSON.stringify(newJson, null, 4), function(err) {
			if(err) {
			  console.log(err);
			} else {
			  console.log("JSON saved to " + "boards.json");
			}
		});
	});

});

socket.on('disconnect', function(error_result){
	console.log(error_result);
});

function makeid(length) {
   var result           = '';
   var characters       = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
   var charactersLength = characters.length;
   for ( var i = 0; i < length; i++ ) {
      result += characters.charAt(Math.floor(Math.random() * charactersLength));
   }
   return result;
}



