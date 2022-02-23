#include <M5Stack.h>
#include <ros.h>
#include <std_msgs/Bool.h>
#include <WiFi.h>

const int BUTTON_PIN = 21;

const char SSID[] = "";
const char PASSWORD[] = "";
IPAddress server(192,168,0,5);
const uint16_t serverPort = 11411;

IPAddress ip(192, 168, 0, 10);

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

ros::NodeHandle_<WiFiHardware> nh;
std_msgs::Bool buttton_pushed_msg;
ros::Publisher pub_bottun_pushed("button_pushed", &buttton_pushed_msg);

void setup() {
  Serial.begin(115200);

  M5.begin();
  M5.Lcd.setCursor(10, 10);   //文字表示の左上位置を設定
  M5.Lcd.setTextColor(RED);   //文字色設定(背景は透明)(WHITE, BLACK, RED, GREEN, BLUE, YELLOW...)
  M5.Lcd.setTextSize(2);      //文字の大きさを設定（1～7）
  M5.Lcd.print("Start");

  WiFi.begin(SSID,PASSWORD);
  Serial.print("WiFi connecting");
  M5.Lcd.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    M5.Lcd.print(".");
    delay(100);
  }

  Serial.println(" connected");
  M5.Lcd.print(" connected");
  pinMode(BUTTON_PIN, INPUT);

  nh.initNode();
  nh.advertise(pub_bottun_pushed);

  nh.loginfo("push button ready");
  delay(10);
}

void loop() {

  if( digitalRead(BUTTON_PIN) == HIGH ){
    if(buttton_pushed_msg.data == false){
      M5.Lcd.print(" pushed");
      pub_bottun_pushed.publish(&buttton_pushed_msg);
    }
    buttton_pushed_msg.data = true;
    
  }
  else{
    if(buttton_pushed_msg.data == true){
      M5.Lcd.print(" released");
      pub_bottun_pushed.publish(&buttton_pushed_msg);
    }
    buttton_pushed_msg.data = false;
  }

  M5.update();
  if( M5.BtnA.isPressed()||M5.BtnB.isPressed()||M5.BtnC.isPressed()){
    M5.Lcd.print("button pressed");
    delay(300);
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(10, 10); 
  }

  
  nh.spinOnce();
  delay(50); //ms
}
