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

server.on('message', (topic, message) => {
  console.log(`topic: ${topic} | message: ${message}`)
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
  callback(null, username, selectPassword(username))
}

function selectPassword(username){
  if (username === appCreds.SERVER_MQTT_USERNAME){
    return appCreds.SERVER_MQTT_PASSWORD
  } else if (username === appCreds.IOT_MQTT_USERNAME) {
    return appCreds.IOT_MQTT_PASSWORD
  } else {
    return ''
  }
}

aedes.on('subscribe', async function (packet, client) {
  console.log('something subscribed')
})

aedes.on('closed', async function (packet, client) {
  console.log("I lost some guy")
})