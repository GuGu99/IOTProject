var express = require('express');
var router = express.Router();

var mqtt = require("mqtt");
var client = mqtt.connect("mqtt://192.168.0.10");

/* GET home page. */
router.post('/led/:flag', function(req, res, next) {
	if (req.params.flag == 'on'){
		// MQTT -> led : 1
		client.publish("led", '1');
		res.send( JSON.stringify({led : 'on'}) );
	}else if (req.params.flag == 'off'){
		client.publish("led", '2');
		res.send( JSON.stringify({led : 'off'}) );
	}
});

module.exports = router;