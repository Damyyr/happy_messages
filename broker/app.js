const { Console } = require('console')

const aedes = require('aedes')()
const server = require('net').createServer(aedes.handle)
const port = 5000

server.listen(port, function () {
  console.log('server started and listening on port ', port)
})

aedes.on('subscribe', async function (packet, client) {
    console.log('WOOOOOW')
    console.log(client)
})