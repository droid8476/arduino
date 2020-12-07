// Fill in  your WiFi networks SSID and password
#define SECRET_SSID "YOUR SSID"
#define SECRET_PASS "YOUR WIFI PASSWORD"

// Fill in the hostname of your AWS IoT broker
#define AWS_ENDPOINT "YOUR_IOT_ENDPOINT.amazonaws.com"

// Fill in the boards public certificate
const char SECRET_CERTIFICATE[] = R"(
-----BEGIN CERTIFICATE-----
YOUR CERTIFICATE
-----END CERTIFICATE-----
)";
