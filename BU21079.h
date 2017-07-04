#ifndef BU21079_H
#define BU21079_H
//BU21079 for Arduino
#include <Wire.h>
#include <Arduino.h>

class BU21079_RawDataPerther{
private:
    struct FastRead{
    	FastRead(const int Pin)
    	{
    		bit = digitalPinToBitMask(Pin);
    		inputReg = portInputRegister(digitalPinToPort(Pin));
    	}

    	const uint8_t operator()() const{
    		if(*inputReg & bit) return 1;
    		return 0;
    	}

    	private:
    	uint8_t bit;
    	uint8_t* inputReg;
    };

public:
	BU21079_RawDataPerther(const uint8_t pin)
	: intPin(pin)
	{}
	~BU21079_RawDataPerther(){}

    void begin(){
        Wire.begin();
        Wire.setClock(400000L);
    }

	const uint8_t readRegister(const uint8_t reg) {
		Wire.beginTransmission(i2cAddr);
		Wire.write(reg);
		Wire.endTransmission();
		while (Wire.requestFrom(i2cAddr, 1) != 1);
		return (Wire.read());
	}

	void writeRegister(const uint8_t reg,const uint8_t value) {
		Wire.beginTransmission(i2cAddr);
		Wire.write(reg); // register to read
		Wire.write(value);
		Wire.endTransmission();
	}

    void SWReset(){
        SOFTRESET0(0x55);
        SOFTRESET1(0xAA);
    }

    const bool isHWInted(){
        if(intPin()){
            INT_STATUS = INTERRUPT();
            if(INI == 1){
                CLR_INTERRUPT(0);//
                return true;
            }
        }
        return false;
    }

    const bool isSWCaled(){
        if(intPin()){
            INT_STATUS = INTERRUPT();
            if(CAL == 1){
                CLR_INTERRUPT(0);//
                return true;
            }
        }
        return false;
    }

    void initializeEnd(){
        CONTROL(0b00000011);
    }

private:

    union{
        struct{
            uint8_t INI:1;
            uint8_t CAL:1;
            uint8_t ERCAL:1;
            uint8_t WDT:1;
            uint8_t PERCAL:1;
            uint8_t ONDET:1;
            uint8_t OFFDET:1;
            uint8_t CONTDET:1;
        };
        uint8_t INT_STATUS;
    };


	const uint8_t i2cAddr{0x5C};
	const FastRead intPin;

//REGLIST
    class RegAndValChainSetter{
    public:
    	RegAndValChainSetter(BU21079_RawDataPerther* parent, const uint8_t addr)
    	: holder(parent)
    	, val(0)
    	, addr(addr)
    	{}

    	uint8_t operator ()(){
    		val = holder -> readRegister(addr);
    		return val;
    	}

    	BU21079_RawDataPerther &operator ()(const uint8_t value){
    		val = value;
    		holder -> writeRegister(addr, val);
    		return *holder;
    	}

    	uint8_t val;
    	const uint8_t addr;
    private:
    	BU21079_RawDataPerther* holder;
    };

	//REGMAP
#define MAKE_REG(NAME,ADDR) RegAndValChainSetter NAME {this,ADDR}
public:
	//
	MAKE_REG(SIN_DATA0, 0x00);
	MAKE_REG(SIN_DATA1, 0x01);
	MAKE_REG(SIN_DATA2, 0x02);
	MAKE_REG(SIN_DATA3, 0x03);
	MAKE_REG(SIN_DATA4, 0x04);
	MAKE_REG(SIN_DATA5, 0x05);
	MAKE_REG(SIN_DATA6, 0x06);
	MAKE_REG(SIN_DATA7, 0x07);
	//
	MAKE_REG(INTERRUPT, 0x10);
	MAKE_REG(SIN_STATE, 0x11);

	MAKE_REG(DETECT_SW_ON, 0x13);
	MAKE_REG(DETECT_SLIDER_ON, 0x14);
	MAKE_REG(DETECT_MAT_ON, 0x15);
	MAKE_REG(DETECT_SW_OFF, 0x16);
	MAKE_REG(DETECT_SLIDER_OFF, 0x17);
	MAKE_REG(DETECT_MAT_OFF, 0x18);
	MAKE_REG(DETECT_SW_CONT, 0x19);

	MAKE_REG(DETECT_MAT_CONT, 0x1B);
	MAKE_REG(STATE, 0x1C);

	MAKE_REG(RACT, 0x1E);

	MAKE_REG(SOFTRESET0, 0x85);

	MAKE_REG(SOFTRESET1, 0x8A);

	MAKE_REG(CFG_SIN_1_0, 0xC0);
	MAKE_REG(CFG_SIN_3_2, 0xC1);
	MAKE_REG(CFG_SIN_5_4, 0xC2);
	MAKE_REG(CFG_SIN_7_6, 0xC3);
	MAKE_REG(CFG_SIN_x_8, 0xC4);

	MAKE_REG(GAIN_1_0, 0xC8);
	MAKE_REG(GAIN_x_2, 0xC9);
	MAKE_REG(ON_TH0, 0xCA);
	MAKE_REG(ON_TH1, 0xCB);
	MAKE_REG(ON_TH2, 0xCC);
	MAKE_REG(OFF_TH, 0xCD);
	MAKE_REG(OVERSAMPLES, 0xCE);
	MAKE_REG(CONTTIMES, 0xCF);

	MAKE_REG(MSK_DETECT_SW, 0xD0);

	MAKE_REG(MSK_DETECT_MAT0, 0xD2);
	MAKE_REG(MSK_DETECT_MAT1, 0xD3);

	MAKE_REG(EN_SLIDER, 0xD9);
	MAKE_REG(KEEP_SENS_NUM, 0xDA);
	MAKE_REG(UNIT_SENS_NUM, 0xDB);
	MAKE_REG(TH_SLEEP_H, 0xDC);
	MAKE_REG(TH_SLEEP_L, 0xDD);
	MAKE_REG(OUTPUT_OFFSET, 0xDE);
	MAKE_REG(MSK_INTERRUPT, 0xDF);

	MAKE_REG(MODE_CONFIG0, 0xE0);
	MAKE_REG(MODE_CONFIG1, 0xE1);
	MAKE_REG(EN_SIN, 0xE2);
	MAKE_REG(SENS_NUM, 0xE3);
	MAKE_REG(SEMS_RD_TIME, 0xE4);
	MAKE_REG(SENS_RST_TIME, 0xE5);
	MAKE_REG(SENS_IRST_TIME, 0xE6);
	MAKE_REG(CHK_NUM, 0xE7);
	MAKE_REG(CHK_RD_TIME, 0xE8);
	MAKE_REG(CHK_RST_TIME, 0xE9);
	MAKE_REG(CHK_IRST_TIME, 0xEA);
	MAKE_REG(DIG_GAIN, 0xEB);
	MAKE_REG(CHK_WAIT_TIME, 0xEC);
	MAKE_REG(SENS_WAIT_TIME, 0xED);
	MAKE_REG(CALIB_CONFIG0, 0xEE);
	MAKE_REG(CALIB_CONFIG1, 0xEF);

	MAKE_REG(CLR_INTERRUPT, 0xF0);
	MAKE_REG(CLR_DETECT_SW_ON, 0xF1);

	MAKE_REG(CLR_DETECT_MAT_ON, 0xF3);
	MAKE_REG(CLR_DETECT_SW_OFF, 0xF4);
	MAKE_REG(CLR_DETECT_SLIDER_OF, 0xF5);
	MAKE_REG(CLR_DETECT_MAT_OFF, 0xF6);
	MAKE_REG(CLR_DETECT_SW_CONT, 0xF7);

	MAKE_REG(CLR_DETECT_MAT_CONT, 0xF9);

	MAKE_REG(WACT, 0xFE);
	MAKE_REG(CONTROL, 0xFF);



#undef MAKE_REG

};


#endif /* end of include guard: BU21079_H */
