int a;
char buf[10];

int ReadInt(void){
  if (Serial.available()) {
    a=Serial.readBytesUntil('\n',(char*)buf,10);
    return atoi(buf);
}
}


float ReadFloat(void){
  if (Serial.available()) {
    a=Serial.readBytesUntil('\n',(char*)buf,10);
    return atof(buf);
}
}
