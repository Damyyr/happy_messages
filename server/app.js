const isDebug = console.debug(process.env['npm_lifecycle_event'] == 'start:debug') // I don't know how to make it better
const appCreds = require('./credentials.js').appCreds
const fastify = require('fastify')({ logger: isDebug })
const helmet = require('fastify-helmet')

const mqtt = require('mqtt')
const client = mqtt.connect(appCreds.mqttServerOptions)

fastify.register(helmet)

client.on("connect", (err) => {
    if(err)
    console.error(err)
    
    console.log('Connected to broker at ', appCreds.brokerHost)
})

client.on('error', (err) => {
    console.error("Error: ", err)
})

fastify.get('/ping', (req, rep) => {
    rep.send({ msg: 'pong!' })
})

fastify.post('/sendMessage', (req, rep) => {
    const message = req.body['message']
    const topic = req.body['topic']
    
    if(!message || !topic){
        rep.statusCode = 400
        throw new Error('You have to specify a topic and a message to send')
    }
    
    client.publish(topic, message)
    rep.send({ msg: `'${message}' message sent!'` })
})

fastify.listen(appCreds.port, (err) =>{
    if (err) {
        fastify.log.error(err)
        process.exit(1)
    }
    
    console.log('Server started on port ', appCreds.port)
})