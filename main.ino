#define M11 8
#define M12 9
#define Relay_Purifier 10
#define Relay_Ceiling_Fan 13
#define gas_sensor A0
#define Dust_Sensor A5
#define Switch_Wifi A1
#define buzzer 7
int Dust_Sensor_ledPower = 12;
unsigned long int Delay_Purifier_Timeout_Millis =0;
boolean Stat_Switch=0;
unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
int sensorValue = 0;
boolean Stat_Relay_Purifier =0, New_Stat_Close=0, Stat_Close=1,
Stat_Relay_Ceiling_Fan=0, Stat_Switch_Wifi=0;
boolean Methane_Limit=0, Dust_Limit=0;
void setup(void) {
 pinMode(M11,OUTPUT);
pinMode(M12,OUTPUT);
pinMode(Relay_Purifier,OUTPUT);
pinMode(Relay_Ceiling_Fan,OUTPUT);

pinMode(buzzer,OUTPUT);
pinMode(Dust_Sensor_ledPower,OUTPUT);

Serial.begin(9600);
}
void Window_Open(void){
digitalWrite(M11,HIGH);
digitalWrite(M12,LOW);
delay(1200);

digitalWrite(M11,LOW);
}
void Window_Close(void){
 digitalWrite(M11,LOW);
digitalWrite(M12,HIGH);
delay(1200);
digitalWrite(M11,LOW);
}
void loop() {
 digitalWrite(Dust_Sensor_ledPower,LOW);
delayMicroseconds(samplingTime);
 voMeasured = analogRead(Dust_Sensor);
 delayMicroseconds(deltaTime);
 digitalWrite(Dust_Sensor_ledPower,HIGH);

 delayMicroseconds(sleepTime);

 calcVoltage = voMeasured*(5.0/1024);

 dustDensity = 0.17*calcVoltage-0.1;

 if ( dustDensity < 0)
{
 dustDensity = 0.00;
}
if( analogRead(Switch_Wifi) > 100 )

 Stat_Switch = 1;

else
 Stat_Switch = 0;

if(dustDensity > 0.5)
{
 Dust_Limit=1;
}

else {
 Dust_Limit=0;
 }

int Methane_Value = analogRead(gas_sensor);

if(Methane_Value > 500)
{
 Methane_Limit=1;
}
else {
 Methane_Limit=0;
}

if( Methane_Limit && Dust_Limit )
{
Stat_Relay_Purifier =1;

 Stat_Relay_Ceiling_Fan =1;
 New_Stat_Close=0;
 Delay_Purifier_Timeout_Millis=millis();
 }

else if( Methane_Limit )
{
 Stat_Relay_Purifier =1;

 Stat_Relay_Ceiling_Fan =1;

New_Stat_Close=0;

 Delay_Purifier_Timeout_Millis=millis();
 }
else if( Dust_Limit )
{
 Stat_Relay_Purifier =1;

 Stat_Relay_Ceiling_Fan =1;
 New_Stat_Close=1;
 Delay_Purifier_Timeout_Millis=millis();
 }
else {
 if( ( millis() - Delay_Purifier_Timeout_Millis) > 30000 )
{
 Stat_Relay_Purifier =0;

 Stat_Relay_Ceiling_Fan =0;
 New_Stat_Close=1;
 }
 }
 if(!Stat_Switch )
{
 New_Stat_Close=1;

 Stat_Relay_Purifier =0;

 Stat_Relay_Ceiling_Fan =0;
 }

if(New_Stat_Close != Stat_Close )
{
 if(!Stat_Close) {

 Serial.println("Clossing Windows");

 Window_Close();

 Stat_Close=1;
 }

 else {
 Serial.println("Opening Windows");

 Window_Open();

 Stat_Close=0;
 }
 }

 if( Stat_Relay_Purifier )
 {
 Serial.print("Purifier : ON ");

 digitalWrite(Relay_Purifier,HIGH);
 }

else {
 Serial.print("Purifier : OFF ");

 digitalWrite(Relay_Purifier,LOW);
 }


if( Stat_Relay_Ceiling_Fan )
{
 digitalWrite(Relay_Ceiling_Fan,HIGH);

 Serial.print("\tFAN : ON ");
 }

else {
 Serial.print("\tFAN : OFF ");

 digitalWrite(Relay_Ceiling_Fan,LOW);
 }

 Serial.print("Dust: ");

 Serial.print(dustDensity);

 Serial.print("PPM");
 Serial.print("\t Methane = ");

if(Methane_Value > 500)
 {
 Serial.print("HIGH");

 digitalWrite(buzzer,HIGH);
 }

else {
 Serial.print("LOW");

 digitalWrite(buzzer,LOW);
 }

 Serial.print("\t Status: ");
if( Stat_Switch )
 {
 Serial.println("Enabled"); }

else {
 Serial.println("Disabled"); }

 delay(1000);
}
