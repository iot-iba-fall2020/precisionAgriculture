var express = require('express');
const https = require('https');
var app = express();
var port = process.env.PORT || 80;

var bodyParser = require('body-parser');
app.use(bodyParser.json()); // support json encoded bodies
app.use(bodyParser.urlencoded({ extended: true })); // support encoded bodiescl

// routes will go here

app.get('/', function(req, res) {
    console.log("Hey, we got a hit!")
    res.send("thanks for the data");
});

app.post('/log', function(req, res) {
    //var user_id = req.body.id;
    //var token = req.body.token;
    var sensor1 = req.body.sensor1;
    console.log(req.body);

	var url ="https://api.thingspeak.com/update?api_key=84VS72LE0U2EGRLX&field1=";
	url += "&field1="+(req.body.temprature | 0);
	url += "&field2="+(req.body.humidity | 0);
	url += "&field3="+(req.body.moisture | 0);
	url += "&field4="+(req.body.isWatering | 0);
	url += "&field5="+(req.body.lightIntensity | 0);
	url += "&field6="+(req.body.isHeating | 0);
	url += "&field7="+(req.body.batteryStatus | 0);

	https.get(url, (resp) => {
	  let data = '';

	  // A chunk of data has been received.
	  resp.on('data', (chunk) => {data += chunk;});

	  // The whole response has been received. Print out the result.
	  resp.on('end', () => {console.log(JSON.parse(data).explanation);});

	}).on("error", (err) => {
	  console.log("Error: " + err.message);
	});

    res.send("thanks");
});


// start the server
app.listen(port);
console.log('Server started! At http://localhost:' + port);