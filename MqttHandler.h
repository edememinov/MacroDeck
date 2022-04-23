void callMqttCommand(String topic, String message){
  mqttClient.publish(topic.c_str(), message.c_str());
}
