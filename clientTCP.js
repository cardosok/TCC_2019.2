// Importa net module.
var net = require('net');

// This function create and return a net.Socket object to represent TCP client.
module.exports = function getConn(connName){
    let ipPorta = {
        host:'192.168.4.1',
        port: 555
    }

    //Cria a conexão do servidor TCP
    var client = net.createConnection(ipPorta, function () {
        console.log('Nome da conexão: ' + connName);
        console.log('Cliente IP : ' + client.localAddress + "Porta: " + client.localPort);
        console.log('Server IP : ' + client.remoteAddress + " Porta:" + client.remotePort);
    });

    //client.setTimeout(1000);
    client.setEncoding('utf8');

    // When receive server send back data.
    client.on('data', function (data) {
        console.log('dados: '+ data);
    });

    // When connection disconnected.
    client.on('end',function () {
        console.log('Cliente desconectado. ');
    });

    client.on('timeout', function () {
        console.log('Timeout do Cliente. ');
    });

    client.on('error', function (err) {
        console.error(JSON.stringify(err));
    });
    return client;
}
