#include <WiFi.h>
#include <time.h>
const char* ssid = "Indruino_Student"; 
const char* password = "Indruino2019";

const int GPIO_PIN_NUMBER_25 = 25;
int timezone = 7*3600;
int dst = 0;
// Set web server port number to 80
WiFiServer server(80);
String header;

// Variable to store the HTTP request

void setup() {
Serial.begin(115200);

pinMode(GPIO_PIN_NUMBER_25, OUTPUT);
digitalWrite(GPIO_PIN_NUMBER_25, LOW);

// Connect to Wi-Fi network with SSID and password
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
// Print local IP address and start web server
Serial.println("");
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
server.begin();
configTime(timezone, dst, "vn.pool.ntp.org", "time.nist.gov");
}

void loop(){
WiFiClient client = server.available(); 
time_t now = time(nullptr);
struct tm* p_tm = localtime(&now);
if (client) { // If a new client connects,
Serial.println("New Client."); 
String currentLine = ""; 
while (client.connected()) 
{ // loop while the client's connected
if (client.available()) { // if there's bytes to read from the client,
char c = client.read(); // read a byte, then
Serial.write(c); // print it out the serial monitor
header += c;
if (c == '\n') { // if the byte is a newline character
// if the current line is blank, you got two newline characters in a row.
// that's the end of the client HTTP request, so send a response:
if (currentLine.length() == 0) {
// dXNlcjpwYXNz = 'IndruinoStudent:Indruino2019Student' (user:pass) base64 encode
if(header.indexOf("SW5kcnVpbm9TdHVkZW50OkluZHJ1aW5vMjAxOVN0dWRlbnQ=") >= 0) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  if (header.indexOf("LED=ON") != -1) 
  {

    digitalWrite(GPIO_PIN_NUMBER_25, HIGH);
  } 
  if (header.indexOf("LED=OFF") != -1) 
  {
    digitalWrite(GPIO_PIN_NUMBER_25, LOW);
  } 
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #4CAF50; border: 2px solid #4CAF50;; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}"); 
  // Web Page Heading
  client.println("</style></head>");
  client.println("<body><center><h1>LED Controlling</h1></center>");
  //client.println("<center><h2>Press on button to turn on led and off button to turn off LED</h2></center>");
  client.println("<form><center>");
  client.println("<center> <button class=\"button\" name=\"LED\" value=\"ON\" type=\"submit\">LED ON</button>") ;
  client.println("<button class=\"button\" name=\"LED\" value=\"OFF\" type=\"submit\">LED OFF</button><br><br>");
  client.println("</center></form>");
  client.println("<table><tr><th>VALUE</th><th>TimeStamp</th></tr>");
  client.println("<tr><td>Value</td><td><span class=\"sensor\">");
  client.print(p_tm->tm_hour);
  client.print(" : ");
  client.print(p_tm->tm_min);
  client.print(" : ");
  client.print(p_tm->tm_sec);
  client.println("</span></td></tr>");
  client.println("</body></html>"); 
  break;
}
// Wrong user or password, so HTTP request fails... 
  else { 
    client.println("HTTP/1.1 401 Unauthorized");
    client.println("WWW-Authenticate: Basic realm=\"Secure\"");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<html>Authentication failed</html>");
    break;
  } 
} else { // if you got a newline, then clear currentLine
  currentLine = "";
  }
} else if (c != '\r') { // if you got anything else but a carriage return character,
  currentLine += c; // add it to the end of the currentLine
  }
}
}
// Clear the header variable
header = "";
// Close the connection
client.stop();
Serial.println("Client disconnected.");
Serial.println("");
}
}
