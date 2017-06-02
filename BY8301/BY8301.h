//BY8301 audio library for Arduino
//VERSION 0.1.0
#ifndef _BY8301_H_
#define _BY8301_H_

#if defined(ARDUINO) && ARDUINO >=100
		#include <Arduino.h>
#else
		#include<WProgram.h>
		#include<pins_arduino.h>
#endif

#include <SoftwareSerial.h>
//#include "BY8301_def.h"
#define STX 0x7E //起始码
#define ETX 0xEF //结束码

//操作码
//通信控制指令
#define CMD_PLAY    0x01    //播放
#define CMD_PAUSE   0x02	//暂停
#define CMD_NEXT    0x03	//下一曲
#define CMD_PREV	0x04	//上一曲
#define CMD_UP		0x05 	//音量加
#define CMD_DOWN	0x06	//音量减
#define CMD_SLEEP   0x07	//待机/正常工作（转换：待机下转为工作，工作下转为待机,待机电流：10mA）
#define CMD_RESET   0x09	//复位
#define	CMD_FASTF	0x0A	//快进-->FLASH无此功能
#define CMD_FASTD	0x0B	//快退-->FLASH无此功能
#define CMD_STOP	0x0E	//停止
#define CMD_VOL		0x31	//设置音量:0-30级,掉电记忆
#define CMD_EQ		0x32	//设置EQ-->均衡器：0-5(NO/POP/ROCK/JAZZ/CLASSIC/BASS)(掉电记忆)
#define CMD_MODE    0x33	//设置循环模式：0-4(全盘/文件夹/单曲/随机/无循环)
#define CMD_FOLDER  0x34	//文件夹切换:0-->上一个文件夹，1-->下一个文件夹
#define CMD_DEVICE	0x35	//设备切换：0(U),1(FLASH)
#define CMD_CHOOSE  0x41	//选择播放曲目：1-255首(掉电记忆)
#define CMD_FILECH  0x42    //指定文件夹曲目播放

//通信查询指令
#define QUERY_STA   	0x10	//查询播放状态：0(停止)1(播放)2(暂停)3(快进)4(快退)
#define QUERY_VOL   	0x11	//查询音量大小：1-30级(掉电记忆)
#define QUERY_EQ    	0x12	//查询当前EQ：0-5(NO/POP/ROCK/JAZZ/CLASSIC/BASS)(掉电记忆)
#define QUERY_MODE  	0x13	//查询当前播放模式：0-4(全盘/文件夹/单曲/随机/无循环)
#define QUERY_VERSION 	0x14	//查询版本号：1.0
#define QUERY_UFILE		0x16	//查询U盘总文件数：1-65535
#define QUERY_FLASH_FILE	0x17	//查询FLASH的总文件数 1-255
#define QUERY_DEVICE	0x18	//查询当前播放设备：0-->USB,2-->SPI
#define QUERY_UNAME		0x1A	//查询U盘的当前曲目：1-65535
#define	QUERY_FLASH_NAME	0x1B	//查询FLASH的当前曲目：1-255
#define QUERY_PTIME 	0x1C	//查询当前播放歌曲的时间：返回时间(秒)
#define QUERY_TTIME 	0x1D	//查询当前播放歌曲总时间：返回时间(秒)
#define QUERY_NAME  	0x1E	//返回歌曲名(SPI内部无法返回)
#define QUERY_FILES 	0x1F	//0-65536(SPI内部无法返回)

class BY8301{
public:
	BY8301(SoftwareSerial *ser);
	BY8301(HardwareSerial *ser);
	void begin(uint16_t baud);
	void init(uint8_t mode, uint8_t vol);//初始化是否设置模式？？？
	
	void play();
	void pause();
	void next();
	void prev();
	void volUp();
	void volDown();
	void sleep();
	void reset();
	void fastf();
	void fastd();
	void stop();
	void volumn(uint8_t vol);
	void eq(uint8_t eq);
	void setMode(uint8_t temp);
	void folder(uint8_t temp);
	void setDevice(uint8_t device);
	void choose(uint16_t num);
	void chooseFile(uint8_t folder, uint8_t file);
	
	uint16_t querySTA();
	uint16_t queryVOL();
	uint16_t queryEQ();
	uint16_t queryMode();
	uint16_t queryVersion();
	uint16_t queryUfile();
	uint16_t queryFlashFile();
	uint16_t queryDevice();
	uint16_t queryUName();
	uint16_t queryFlashName();
	uint16_t queryPlayTime();
	uint16_t queryTotalTime();
	uint16_t queryFiles();
	///String queryName();
	
private:
	uint16_t baud;
	uint8_t sendBuffer[8];
	uint8_t cmdBuffer[8];

	SoftwareSerial *audioSwSerial;
	HardwareSerial *audioHwSerial;
  
	void common_init(void);
	void sendCommand(uint8_t cmd, uint8_t *buf, uint16_t len);
	uint16_t dataParse();
	uint16_t queryNum(uint8_t cmd);
};

#endif




































