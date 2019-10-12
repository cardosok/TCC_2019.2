var net = require('net');
var connectClient =  require('./clientTCP');

function loop(){
    var teste = connectClient('teste');
    while(1){
        teste.write('tes');
    }
}

loop();