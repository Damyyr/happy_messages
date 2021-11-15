const appCreds = require('./credentials.js').appCreds
const aedes = require('aedes')()
const server = require('net').createServer(aedes.handle)

// const cron = require('node-cron')
// const tasks = []; // TODO: move this in a module like "tasksManager" or smthg
// Usage: tasks.push || tasks.foreach() -> access all the registred ongoing tasks

server.listen(appCreds.aedesPort, function () {
  console.log('server started and listening on port ', appCreds.aedesPort)
})

server.on('connect', () => {
  console.log('connect')
})

aedes.subscribe('#', (payload, deliverfunc, callback) => {
  const topic = payload.topic
  const message = payload.payload.toString()
  console.log(topic, message)
})

server.on('hm', (topic, message) => {
  console.log(`topic: ${topic} | message: ${message}`)
  aedes.publish('hm', 'someone just quit')
})

server.on('error', (err) => {
  console.error('ERROR: ', err)
})

aedes.on('error', (err) => {
  console.error(err);
})

aedes.authenticate = function (client, username, password, callback) {
  console.log(client.id, 'is tring to connect')
  // I dunno why, but when I move those condition in its own method to call checkAuth(username, password) : Boolean
  // it does not work

  if(username === appCreds.SERVER_MQTT_USERNAME){
    callback(null, username === appCreds.SERVER_MQTT_USERNAME && password.toString() === appCreds.SERVER_MQTT_PASSWORD)
  } else if (username === appCreds.IOT_MQTT_USERNAME) {
    callback(null, username === appCreds.IOT_MQTT_USERNAME && password.toString() === appCreds.IOT_MQTT_PASSWORD)
  } else {
    const error = new Error('Auth error')
    console.error('Auth Error for', username)
    callback(error, null)
  }
}

aedes.on('subscribe', async function (packet, client) {
  console.log(client.id, 'subscribed onto', packet[0].topic, ', QOS:', packet[0].qos)
})

aedes.on('closed', async function (packet, client) {
  console.log("lost", client.id)
})