#include <Arduino.h>
#include <WiFi.h>

static const char *k_ssid = "Solaris";
static const char *k_password = "junta-sown-gunwale-muse-SEMINAR";
static const char *k_server_host = "4.tcp.us-cal-1.ngrok.io";
static const uint16_t k_server_port = 19144;

static void speedTest()
{
    while (WiFi.status() != WL_CONNECTED);

    Serial.println("Beginning speed test");

    uint32_t size = 0x400000;
    uint8_t *buf = (uint8_t *) ps_malloc(size);
    if (!buf)
    {
        Serial.println("Failed to alloc");
        return;
    }

    buf[0] = size & 0xff;
    buf[1] = (size >> 8) & 0xff;
    buf[2] = (size >> 16) & 0xff;
    buf[3] = (size >> 24) & 0xff;

    WiFiClient client;
    client.setNoDelay(true);
    if (!client.connect(k_server_host, k_server_port))
    {
        Serial.println("Failed to connect");
        return;
    }
    delay(2000);

    unsigned long t0 = millis();
    client.write(buf, size);
    unsigned long t1 = millis();
    unsigned long delta = t1 - t0;
    Serial.printf("Transmission time: %d ms, %1.2f MB/sec\n", delta, 4.0f / (float(delta) / 1000.0f));
    client.stop();
    free(buf);

    Serial.println("Finished!");
    while(true);

}

void setup() 
{
  delay(3000);

  Serial.begin(115200);
	Serial.printf("Hello, World!\n");

	WiFi.begin(k_ssid, k_password);
	Serial.println("Connecting");
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.print("Connected to WiFi network with IP Address: ");
	Serial.println(WiFi.localIP());

  speedTest();
}

void loop() 
{
  // put your main code here, to run repeatedly:
}