const jsonConcat = require('json-concat');
const fs = require('fs');
var isEqual = require('lodash.isequal');
var _ = require('lodash');

const files = [];

const theDirectory = "data/"; 
fs.readdirSync(theDirectory).forEach((file) => {
  files.push(file);
})

let finalArray = {};
finalArray.buttons = [];

files.forEach(element =>{
	 fs.readFile("data/" + element, (err, data) => {
		var array = JSON.parse(data);
		array["buttons"].forEach(button =>{
			if(!finalArray.buttons.some(item => _.isEqual(item, button))){
				finalArray.buttons.push(button);
				console.log(finalArray.buttons);
				fs.writeFileSync('finalData/finalButton.json', JSON.stringify(finalArray, null, 4));
			}
				
			
		})
		
	});
	console.log(finalArray.buttons);
});




