#include "BY8301.h"
//#include"BY8301_def.h"
#include <SoftwareSerial.h>
BY8301::BY8301(SoftwareSerial *ser){
	common_init();
		audioSwSerial = ser;
}
BY8301::BY8301(HardwareSerial *ser){
		common_init();
		audioHwSerial = ser;
}
void BY8301::common_init(void){
		audioSwSerial = NULL;
		audioHwSerial = NULL;
}
void BY8301::begin(uint16_t _baud){
	baud = _baud;
	if(audioSwSerial){
		audioSwSerial->begin(baud);
	}else{
		audioHwSerial->begin(baud);
	}
	delay(100);
}
/**
	初始化操作
**/
void BY8301::init(uint8_t mode,uint8_t vol){
	begin(9600);//调用本函数
	//reset();复位
	setMode(mode);
	return volumn(vol);
}
void  BY8301::sendCommand(uint8_t cmd,uint8_t *buf,uint16_t len){
	sendBuffer[0] = STX;
	sendBuffer[1] = len+3;//长度，操作码，校验码
	sendBuffer[2] = cmd;
	sendBuffer[len+4] = ETX;
	sendBuffer[len+3]=(sendBuffer[1]^cmd);
	if(len>0){
		memcpy(sendBuffer+3,buf,len);
		for(int i=0;i<len;i++){
			sendBuffer[len+3]^=sendBuffer[3+i];
		}
	}
	if(audioSwSerial){
		audioSwSerial->write(sendBuffer,len+5);
	}else{
		audioHwSerial->write(sendBuffer,len+5);
	}
	delay(160);
	return sendBuffer;
}
/*控制指令*/

void BY8301::play(){
	sendCommand(CMD_PLAY,NULL,0);
}
void BY8301::pause(){
	sendCommand(CMD_PAUSE,NULL,0);
}
void BY8301::next(){
	sendCommand(CMD_NEXT,NULL,0);
}
void BY8301::prev(){
	sendCommand(CMD_PREV,NULL,0);
}
void BY8301::volUp(){
	sendCommand(CMD_UP,NULL,0);
}
void BY8301::volDown(){
	sendCommand(CMD_DOWN,NULL,0);
}
void BY8301::sleep(){
	sendCommand(CMD_SLEEP,NULL,0);
}
void BY8301::reset(){
	sendCommand(CMD_RESET,NULL,0);
	delay(1500);
}
void BY8301::fastf(){
	sendCommand(CMD_FASTF,NULL,0);
}
void BY8301::fastd(){
	sendCommand(CMD_FASTD,NULL,0);
}
void BY8301::stop(){
	sendCommand(CMD_STOP,NULL,0);
}
void BY8301::volumn(uint8_t vol){
	cmdBuffer[0] = vol;
	sendCommand(CMD_VOL,cmdBuffer,1);
}
void BY8301::eq(uint8_t eq){
	cmdBuffer[0] = eq;
	sendCommand(CMD_EQ,NULL,0);
}
void BY8301::setMode(uint8_t mode){
	cmdBuffer[0] = mode;
	sendCommand(CMD_MODE,cmdBuffer,1);
}
void BY8301::folder(uint8_t temp ){//文件夹切换
	cmdBuffer[0] = temp;
	sendCommand(CMD_FOLDER,cmdBuffer,1);
}
void BY8301::setDevice(uint8_t device){
	cmdBuffer[0] = device;
	sendCommand(CMD_DEVICE,NULL,0);
}
void BY8301::choose(uint16_t num){
	cmdBuffer[0] = num>>8;//参数高位，数组复制时为高位
	cmdBuffer[1] = num&0xFF;
	sendCommand(CMD_CHOOSE,cmdBuffer,2);
}
void BY8301::chooseFile(uint8_t folder, uint8_t file){
	cmdBuffer[0] = file;
	cmdBuffer[1] = folder;
	sendCommand(CMD_FILECH,NULL,0);
}
/*查询指令*/
uint16_t BY8301::queryNum(uint8_t cmd){
	if(audioSwSerial){
		audioSwSerial->stopListening();
		audioSwSerial->listen();
	}else{
		audioHwSerial->end();
		audioHwSerial->begin(baud);
	}
	sendCommand(cmd,NULL,0);
	return dataParse();
}

uint16_t BY8301::dataParse(){
	uint16_t sum=0;
	uint8_t temp;
	if(audioSwSerial){
		while(audioSwSerial->available()){
			temp = char(audioSwSerial->read());
			if(temp>47&&temp<58){
				temp-=48;
				sum=sum*16+temp;
			}else if(temp>96&&temp<103){
				temp-=87;
				sum=sum*16+temp;
			}
			
			delay(1);
		}	
	}else{
		while(audioHwSerial->available()){
			temp = char(audioHwSerial->read());
			if(temp>47&&temp<58){
				temp-=48;
				sum=sum*16+temp;
			}else if(temp>96&&temp<103){
				temp-=87;
				sum=sum*16+temp;
			}
			delay(1);
		}	
		
	}
	return sum;
	
}
uint16_t BY8301::querySTA(){
	return queryNum(QUERY_STA);
}
uint16_t BY8301::queryVOL(){
	return queryNum(QUERY_VOL);
}
uint16_t BY8301::queryEQ(){
	return queryNum(QUERY_EQ);
}
uint16_t BY8301::queryMode(){
	return queryNum(QUERY_MODE);
}
uint16_t BY8301::queryVersion(){
	return queryNum(QUERY_VERSION);
}
uint16_t BY8301::queryUfile(){
	return queryNum(QUERY_UFILE);
}
uint16_t BY8301::queryFlashFile(){
	return queryNum(QUERY_FLASH_FILE);
}
uint16_t BY8301::queryDevice(){
	return queryNum(QUERY_DEVICE);
}
uint16_t BY8301::queryUName(){
	return queryNum(QUERY_UNAME);
}
uint16_t BY8301::queryFlashName(){
	return queryNum(QUERY_FLASH_NAME);
}
uint16_t BY8301::queryPlayTime(){
	return queryNum(QUERY_PTIME);
}
uint16_t BY8301::queryTotalTime(){
	return queryNum(QUERY_TTIME);
}
uint16_t BY8301::queryFiles(){
	return queryNum(QUERY_FILES);
}
//查询应读取串口数据<暂时查不到>
//String queryName(){
//	sendCommand(QUERY_NAME,NULL,0);
//}
