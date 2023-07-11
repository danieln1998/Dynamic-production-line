
			#include <ESP8266WiFi.h>
      #include <WiFiClient.h>
      #include <WiFiUdp.h>
      #include <ArduinoJson.h>
      #include <ESP8266HTTPClient.h>
  
        const char* ssid = "ssid";
        const char* pswd = "password";
        String server_get_phase_time="http://1.1.1.1:3000/get_phase_time";
        WiFiClient client;
        
        void wifi_Setup() {
            Serial.println("wifiSetup");
            // WiFi.begin(ssid);
            WiFi.begin(ssid,pswd);
        
            while (WiFi.status() != WL_CONNECTED) {
                Serial.println("trying ...");
                delay(100);
            }
            Serial.println("Connected to network");
  
        }
        
           void GetData(int* arr) {
            HTTPClient http;
            http.begin(client, server_get_phase_time);
            int httpCode = http.GET();
            if (httpCode == HTTP_CODE_OK) {
              String response = http.getString();
              Serial.println("Response: " + response);
              DynamicJsonDocument jsonDoc(1024);
              DeserializationError error = deserializeJson(jsonDoc, response);
              if (error) {
                Serial.print("Failed to parse JSON: ");
                Serial.println(error.c_str());
                
              }
              else {
              arr[0] = jsonDoc["heating_time"].as<int>();
              arr[1] = jsonDoc["light_test_time"].as<int>();
              arr[2] = jsonDoc["activation_component_time"].as<int>();
              arr[3] = jsonDoc["delay_time"].as<int>();
              }
              
            }

            http.end();

          }



            