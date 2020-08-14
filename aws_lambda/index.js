const https = require('https');
const url = require('url');

exports.handler = function(event, context, callback) {
  console.log('processing event: %j', event)

  var key = "xxxxxxxxxxxxxxxxxxxxxx" // My IFTTT KEY
  var Event_alert = "Mimamori_Kun_Alert"      // IFTTT Event Name for Alert
  var Event_door = "Mimamori_Kun_door"      // IFTTT Event Name for Door Notify

  var clickType = event.clickTypeName
  var is_alert = "No"

  var values = []
  var data = {}

  if (clickType == "DOUBLE"){
    values[1] = "WBGT値が31℃を超えて熱中症危険です。" 
    values[2] = 2
    is_alert = "Yes"
  } else if (clickType  =="LONG"){
    values[1] = "緊急ボタンが押されました。"  
    values[2] = 3
    is_alert = "Yes"
  } else if (clickType  =="SINGLE"){
    values[1] = "冷蔵庫の扉が開かれました！"  
    values[2] = 1
    is_alert = "No"
  }

  if (is_alert == "Yes"){
    var webhookUrl = "https://maker.ifttt.com/trigger/"+Event_alert+"/with/key/"+key
  } else {
    var webhookUrl = "https://maker.ifttt.com/trigger/"+Event_door+"/with/key/"+key
  }

  webhookOptions = url.parse(webhookUrl)
  webhookOptions.headers = {'Content-TYpe':'application/json'}

  var data = { 'value1': values[1],'value2': values[2]}
  var body = JSON.stringify(data)
  console.log("sending: "+body+ " to " + webhookUrl)
  webhookOptions.headers['Content-Length'] = Buffer.byteLength(body)
  var req  = https.request (webhookOptions, function(res){
    if (res.statusCode === 200)
    {
      console.log('webhook succeeded')
      callback(null, true)
    }
    else {
      callback("webhook failed with "+res.statusCode)
    }
    return res;
  })
  req.write(body)
  req.end()
}
