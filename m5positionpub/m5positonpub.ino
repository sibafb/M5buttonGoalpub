#include <M5Stack.h>
#include <ros.h>
#include <std_msgs/Bool.h>

const int BUTTUN_PIN = 30;

const char SSID[] = "＜ここにWiFiルータのSSIDを書く＞";
const char PASSWORD[] = "＜ここにWiFiルータのパスワードを書く＞";
IPAddress server(＜ここにWindowsのIPアドレスを書く＞);
const uint16_t serverPort = 11411;

WiFiClient client;

class WiFiHardware {
  public:
  WiFiHardware() {};

  void init() {
    client.connect(server, serverPort);
  }

  int read() {
    return client.read();
  }

  void write(uint8_t* data, int length) {
    for(int i=0; i<length; i++)
      client.write(data[i]);
  }

  unsigned long time() {
     return millis();
  }
};

std_msgs::Bool buttton_pushed_msg;
ros::Publisher pub_bottun_pushed("buttun_pushed", &buttton_pushed_msg);
ros::NodeHandle_<WiFiHardware> nh;

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID,PASSWORD);
    Serial.print("WiFi connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  Serial.println(" connected");

  pinMode(BUTTUN_PIN, INPUT);

  nh.initNode();
  nh.advertise(pub_bottun_pushed);

  nh.loginfo("push button ready");
  delay(10);
}

void loop() {

  if( digitalRead(BUTTUN_PIN) == HIGH ){
    buttton_pushed_msg.data = true;
  }
  else{
    buttton_pushed_msg.data = false;
  }

  pub_bottun_pushed.publish(&buttton_pushed_msg);
  nh.spinOnce();
  delay(30); //ms
}