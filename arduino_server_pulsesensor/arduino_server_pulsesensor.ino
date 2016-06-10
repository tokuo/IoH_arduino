#include <SPI.h>
#include <Ethernet2.h>

byte mac[] = {0x90, 0xA2, 0xDA, 0x10, 0x6A, 0x19};
IPAddress ip(192,168,10,112);
EthernetServer server(80);

void setup()
{
    Ethernet.begin(mac, ip);  // initialize Ethernet device
    server.begin();           // start to listen for clients
}

void loop()
{
    EthernetClient client = server.available();
    // http protocol(arduino)
    if (client) {
        boolean currentLineIsBlank = true;
        String HTTP_req;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte from client
                HTTP_req += c;
                   /*
                   last line of client request is blank and ends with \n
                   respond to client only after last line received
                   */
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: keep-alive");
                    client.println();
                    // AJAX request for arduino
                    if (HTTP_req.indexOf("tokuocheck") > -1) {
                        // read switch state and send appropriate paragraph text
                        getvaluemain(client);
                    }
                    else {  // HTTP request for web page
                        // send web page - contains JavaScript with AJAX calls
                        client.println("<!DOCTYPE html>");
                        client.println("<html>");
                        client.println("<head>");
                        client.println("<title>Arduino Web Page</title>");
                        client.println("<script>");


                        client.println("function getvalue() {");
                        client.println("var request = new XMLHttpRequest();");
                        client.println("request.onreadystatechange = function() {");
                        client.println("if (this.readyState == 4) {");
                        client.println("if (this.status == 200) {");
                        client.println("if (this.responseText != null) {");
                        client.println("document.getElementById(\"res\")\.innerHTML = this.responseText;");
                        client.println("}}}}");
                        // ajax(arduino)Asynchronous communication
                        client.println("request.open(\"GET\", \"tokuocheck\", true);");
                        client.println("request.send(null);");
                        client.println("}");
                        client.println("</script>");
                        client.println("</head>");



                        client.println("<body onload=\"getvalue()\">");
                        // the element setted ID
                        client.println("<p id=\"res\"></p>");
                        client.println("</body>");
                        client.println("</html>");
                    }
                    // display received HTTP request on serial port
                    HTTP_req = "";    // finished with request, empty string
                    break;
                }
                // every line of text received from the client ends with \r\n
                if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                }
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
            }
        }
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    }
}

// send the value(ajax receive this)
void getValuemain(EthernetClient e){
  client.println(analogRead(0));
  }
