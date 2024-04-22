#include "sdl2.cpp"

I2C line1;

void print_lines(){
    cout<<"SDA Line: ";
    for(u8 i : line1.SDA) cout<<(int)i<<" ";
    cout<<endl;
    cout<<"SCL Line: ";
    for(u8 i: line1.SCL) cout<<(int)i<<" ";
    cout<<endl;
}

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


void sample2(master &plc, oled &oled1){


    vector<u8> stream1 = convert(0x3C, 0, 0x80, 0xB1);
    vector<u8> stream2 = convert(0x3C, 0, 0x80, 0x0F);
    vector<u8> stream69 = convert(0x3C, 0, 0x80, 0x11);

    vector<u8> stream3 = convert(0x3C, 0, 0xC0, 0x06);
    vector<u8> stream4 = convert(0x3C, 0, 0xC0, 0x09);
    vector<u8> stream5 = convert(0x3C, 0, 0xC0, 0x09);
    vector<u8> stream6 = convert(0x3C, 0, 0xC0, 0x06);
    vector<u8> stream7 = convert(0x3C, 0, 0xC0, 0x06);
    vector<u8> stream8 = convert(0x3C, 0, 0xC0, 0x09);
    vector<u8> stream9 = convert(0x3C, 0, 0xC0, 0x09);
    vector<u8> stream10 = convert(0x3C, 0, 0xC0, 0x06);

    vector<u8> stream11 = convert(0x3C, 0, 0x80, 0xB0);
    vector<u8> stream12 = convert(0x3C, 0, 0x80, 0x01);
    vector<u8> stream13 = convert(0x3C, 0, 0x80, 0x12);

    vector<u8> stream14 = convert(0x3C, 0, 0xC0, 0xFC);
    vector<u8> stream15 = convert(0x3C, 0, 0xC0, 0x02);
    vector<u8> stream16 = convert(0x3C, 0, 0xC0, 0x02);
    vector<u8> stream17 = convert(0x3C, 0, 0xC0, 0xFC);
    // vector<u8> stream4 = convert(0x3C, 0, 0xC0, 0x);
    // for(u8 bit: stream1) cout<<(int)bit<<" ";
    // cout<<endl;
    run(plc, oled1, stream1);
    run(plc, oled1, stream2);
    run(plc, oled1, stream69);

    run(plc, oled1, stream3);
    run(plc, oled1, stream4);
    run(plc, oled1, stream5);
    run(plc, oled1, stream6);
    run(plc, oled1, stream7);
    run(plc, oled1, stream8);
    run(plc, oled1, stream9);
    run(plc, oled1, stream10);

    run(plc, oled1, stream11);
    run(plc, oled1, stream12);
    run(plc, oled1, stream13);

    run(plc, oled1, stream14);
    run(plc, oled1, stream15);
    run(plc, oled1, stream16);
    run(plc, oled1, stream17);

}

int main(int argc, char* args[]) {

    vector<u8> plcaddr = {0,1,1,1,0,0,0};
    master plc(plcaddr);
    sample2(plc, oled1);

    if (!init()) {
        cout << "Failed to initialize!" << endl;
        return 1;
    }

    // Drawing all pixels
    for (int y = 0; y < oled1.HEIGHT; ++y) {
        for (int x = 0; x < oled1.WIDTH; ++x) {
            drawPixel(x, y, oled1.GDDR);
        }
    }

    SDL_RenderPresent(gRenderer); // Update the screen

    SDL_Delay(5000); // Wait 5 seconds

    close();
    return 0;
}