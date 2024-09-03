#include "oled.cpp"

I2C line1;

vector<u8> convert(u8 addr, u8 rw, u8 control, u8 data){
    vector<u8> bitstream;
    for(int i = 0; i<ADDRSIZE; i++){
        bitstream.push_back(((addr&0X40)>>6)&1);
        addr<<=1;
    }
    bitstream.push_back(rw);
    for(int i = 0; i<CTRLSIZE+2; i++){
        bitstream.push_back(((control&0X80)>>7)&1);
        control<<=1;
    }
    for(int i = 0; i<DATASIZE; i++){
        bitstream.push_back(((data&0X80)>>7)&1);
        data<<=1;
    }
    return bitstream;
}

void run(master &plc, oled &oled1, vector<u8> bitstream){
    cout<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
    oled1.rec = 1;
    oled1.state = 1;
    plc.inputstream(bitstream);
    oled1.check_startcond();
    for(int i = 0; i<bitstream.size(); i++){
        // ACK bit locations
        if(i==ACK1 || i==ACK2 || i==ACK3){
            if(!line1.SDA[WINDOW-1]) break;
        }

        plc.send_data(bitstream[i]);
        oled1.receive();
    }
    plc.release();
    oled1.check_stopcond();

    oled1.action();
    
    cout<<"<<<<<<<< Bitstream summary >>>>>>>>>"<<endl;
    cout<<"rw bit: "<<(int)oled1.rw<<endl;
    cout<<"co bit: "<<(int)oled1.co<<endl;
    cout<<"dc bit: "<<(int)oled1.dc<<endl;
    cout<<"control byte: "<<hex<<(int)oled1.ctrx<<endl;
    cout<<"data byte: "<<dec<<(int)oled1.drx<<endl;
}

void turnon(master &plc, oled &oled1, u8 byte){
    vector<u8> stream = convert(0x3C, 0, 0xC0, byte);
    run(plc, oled1, stream);
}

void setPage(master &plc, oled &oled1, u8 pagenum){
    vector<u8> stream = convert(0x3C, 0, 0x80, pagenum);
    run(plc, oled1, stream);
}

void setCol(master &plc, oled &oled1, u8 column){
    u8 lower = 0;
    for(int i = 0; i<4; i++){
        lower|=((column>>3-i)&1);
        lower<<=1;
    }
    lower>>=1;  
    u8 upper = 0;
    for(int i = 0; i<4; i++){
        upper|=((column>>7-i)&1);
        upper<<=1;
    }
    upper>>=1;
    upper|=0x10;

    vector<u8> streamlow = convert(0x3C, 0, 0x80, lower);
    vector<u8> streamhigh = convert(0x3C, 0, 0x80, upper);
    
    run(plc, oled1, streamlow);
    run(plc, oled1, streamhigh);
}

void draw0(master &plc, oled &oled1){
    
}

void draw1(master &plc, oled &oled1){
    turnon(plc, oled1, 0x84);
    turnon(plc, oled1, 0xFE);
    turnon(plc, oled1, 0xFE);
    turnon(plc, oled1, 0x80);
}

void draw2(master &plc, oled &oled1){
    turnon(plc, oled1, 0xC4);
    turnon(plc, oled1, 0xE6);
    turnon(plc, oled1, 0xB2);
    turnon(plc, oled1, 0x92);
    turnon(plc, oled1, 0x9E);
    turnon(plc, oled1, 0x8C);
}

void draw3(master &plc, oled &oled1){
    turnon(plc, oled1, 0x44);
    turnon(plc, oled1, 0xC6);
    turnon(plc, oled1, 0x92);
    turnon(plc, oled1, 0x92);
    turnon(plc, oled1, 0xFE);
    turnon(plc, oled1, 0x6C);
}