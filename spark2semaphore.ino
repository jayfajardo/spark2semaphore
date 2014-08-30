
/**
 * This is a simple "sketch" that calls a web page when a button is pressed.  It is a proof-of-concept
 * for my boss and hastily written in about 2 hours.  The remote URL is a PHP script that handles
 * making the API calls to a remote SMS-messaging service.
 *
 * forked from: https://gist.github.com/wgbartley/9473249
 * 
 * I'm sure it could make use of interrupts somehow, but I'm not sure how off the top of my head.
 * 
 * It uses the onboard RGB LED as status display:
 *  - Red = Waiting to be pressed
 *  - Green = Making HTTP request
 *  - Blue = Finished HTTP request
 * 
 * Hardware:
 *  - Normally-off momentary button
 *  - Spark Core
 * 
 * Setup:
 *  - Connect one pin of the button to GND
 *  - Connect the other pin of the button to the desired digital pin on the Spark Core
 * 
 * The configuration is in the variables defined at the top of the script.  They should be pretty
 * self-explanatory.
 *
 * You need to create your own SEMAPHORE account at http://www.semaphore.co (PHILIPPINES Only)
 *
 **/


int millLast = 0;                    // Last time (in millis()) the doorbell was pressed
int pinDoorbell = D0;                // Button pin
int millDelay = 15000;               // How much time to wait until we allow another doorbell "ring" again
char srvIP[] = "54.255.142.115";     // Server IP address (api.sempahore.co)
char srvHost[] = "api.semaphore.co"; // HTTP 1.1 hostname
char contentType[] = "application/x-www-form-urlencoded";
int srvPort = 80;                    // Server port number
char apiToken[] = "[Sempahore API Token]";
char targetNumber[] = "[Your Target Number]";
char smsMessage[] = "DingDong!";
char srvPath[] = "/api/sms?";       // URL path


void setup() {
    Serial.begin(9600);
    pinMode(pinDoorbell, INPUT_PULLUP);

    RGB.control(true);
    /*serialTest();*/
    Serial.println("Ready...");

}

void loop() {
    if(digitalRead(pinDoorbell)==LOW)
        dingdong();
    else
        RGB.color(255, 0, 0);
}


void dingdong() {
    
    
    if(millLast==0 || (millLast+millDelay)<millis()) {
        millLast = millis();
        
        RGB.color(0, 255, 0);
        Serial.println("Ding");
        httpGetRequest(srvIP, srvHost, srvPort, srvPath, contentType, apiToken, targetNumber, smsMessage);
        Serial.println("Dong");
        RGB.color(0, 0, 255);
        
        delay(1000);
    }
}



void httpGetRequest(char* ip, char* hostname, int port, char* url, char* content, char* token, char* number, char* message) {
    char line[255];
    
 
    TCPClient client;
    
    Serial.println("Connecting...");
    
    if (client.connect(ip, port))
    {
        Serial.print("Connected to ");
        Serial.println(ip);
        
        
        /* Construct the URI */
        strcpy(line, "POST ");
        strcat(line, url);
        
        strcat(line, "api=");
        strcat(line, token);
        strcat(line, "&");
        
        strcat(line, "number=");
        strcat(line, number);
        strcat(line, "&");
        
        strcat(line, "message=");
        strcat(line, message);
        
        strcat(line, " HTTP/1.0");
        client.println(line);
        Serial.println(line);
        delay(100);

        strcpy(line, "Host: ");
        strcat(line, hostname);
        client.println(line);
        Serial.println(line);
        delay(100);
        
        strcpy(line, "Content-Type: ");
        strcpy(line, content);
        client.println(line);
        Serial.println(line);
        delay(100);
    
        strcpy(line, "Content-Length: 0");
        client.println(line);
        Serial.println(line);
        delay(100);
    
        client.println();
        delay(100);
    
        client.flush();
        delay(100);
    
        client.stop();
        delay(250);

    }
    else
    {
        Serial.println("Failed!");   
    }
  
}

