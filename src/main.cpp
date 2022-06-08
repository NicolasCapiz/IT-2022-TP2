#include <Arduino.h>
#include <WiFi.h>


const char* ssid     = "wifiNico"; // Nombre de la red 
const char* password = "12345678";     // Contraseña

WiFiServer server(80);
String header;

String btn1 = "off";
String btn2 = "off";

 
 

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  
  // Salidas en LOW
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LED_BUILTIN,LOW);
  

  // Conectarse a la red wifi con ssid y pass
  Serial.print("Setting AP (Access Point)…");
  // 
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin(); // Inicia el servidor
}

void loop(){
  WiFiClient client = server.available();   // Escucha los clientes entrantes

  if (client) {                             // Nuevo cliente se conecta,
    Serial.println("Nuevo Cliente.");       //Imprime mensaje
    String currentLine = "";                // Cadena para contener los datos del cliente entrantes
    while (client.connected()) {            // Bucle cuando el cliente esta conectado
      if (client.available()) {             // bytes para leer del cliente
        char c = client.read();             // lee byte
        Serial.write(c);                    // lo imprime 
        header += c;
        if (c == '\n') {                    // si el byte es un caracter de nueva linea
          // si la línea actual está en blanco, tienes dos caracteres de nueva línea seguidos.
          // final de la solicitud HTTP,envia respuesta:
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // enciende y apaga los botones
            if (header.indexOf("GET /b1/on") >= 0) {
              btn1 = "on";
              digitalWrite(LED_BUILTIN, HIGH);
            } else if (header.indexOf("GET /b1/off") >= 0) {
              btn1 = "off";
              digitalWrite(LED_BUILTIN, LOW);
            }else if (header.indexOf("GET /b2/on") >= 0) {
              btn2 = "on";
              for(int i = 0;i!= 10; i++){
                digitalWrite(LED_BUILTIN, HIGH);
                delay(700);
                digitalWrite(LED_BUILTIN, LOW);
                delay(700);
              }
              digitalWrite(LED_BUILTIN, HIGH);
            } else if (header.indexOf("GET /b2/off") >= 0) {
              btn2 = "off"; 
              digitalWrite(LED_BUILTIN, LOW);
            }
            
            // pagina HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #01451E; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #9EA1A4;}</style></head>");
            
            // Titulo
            client.println("<body><h1>IT-2022-TP2</h1>");
            
            if (btn1=="off") {
              client.println("<p><a href=\"/b1/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/b1/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
 
            if (btn2=="off") {
              client.println("<p><a href=\"/b2/on\"><button class=\"button\">TITILAR ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/b2/off\"><button class=\"button button2\">TITILAR OF</button></a></p>");
            } 
            
            client.println("</body></html>");
            
            // Respuesta HTTP termina con linea en blanco
            client.println();
            // Salir del while
            break;
          } else { // Si tiene nueva linea que borre
            currentLine = "";
          }
        } else if (c != '\r') {  // si obtuvo algo mas,
          currentLine += c;      // agregarlo al currenLine
        }
      }
    }
    // limpiar
    header = "";
    // Cierra la conexion
    client.stop();
    Serial.println("Cliente desconectado.");
    Serial.println("");
  }
}