void callMqttCommand(String topic, String message){
  mqttClient.setServer(configJson["mqttBroker"].as<char*>(), configJson["mqttPort"].as<int>());
  mqttClient.connect(configJson["clientId"].as<char*>(), configJson["mqttUsername"].as<char*>(), configJson["mqttPassword"].as<char*>());
  mqttClient.publish(topic.c_str(), message.c_str());
  mqttClient.disconnect();
}
