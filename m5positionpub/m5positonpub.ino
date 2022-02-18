#include <M5Stack.h>
#undef ESP32
#include <ros.h>
#define ESP32 
#include <std_msgs/Bool.h>

const int BUTTUN_PIN = 21;

std_msgs::Bool buttton_pushed_msg;
ros::Publisher pub_bottun_pushed("buttun_pushed", &buttton_pushed_msg);
ros::NodeHandle nh;

void setup() {
  pinMode(BUTTUN_PIN, INPUT);
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.advertise(pub_bottun_pushed);

  nh.loginfo("push button ready");
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