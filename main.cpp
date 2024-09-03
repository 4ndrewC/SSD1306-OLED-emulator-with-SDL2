#include "sdl2.cpp"


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

void drawTopLeft(master &plc, oled &oled1){  
    setCol(plc, oled1, 0x00);
    setPage(plc, oled1, 0xB0);

    vector<u8> stream3 = convert(0x3C, 0, 0xC0, 0x01);
    run(plc, oled1, stream3);
}

void drawBotRight(master &plc, oled &oled1){
    setCol(plc, oled1, 0x7F);
    setPage(plc, oled1, 0xB3);

    vector<u8> stream3 = convert(0x3C, 0, 0xC0, 0x80);
    run(plc, oled1, stream3);
}

int main(int argc, char* args[]) {

    vector<u8> plcaddr = {0,1,1,1,0,0,0};
    master plc(plcaddr);

    if (!init()) {
        cout << "Failed to initialize!" << endl;
        return 1;
    }

    setPage(plc, oled1, 0xB0);
    setCol(plc, oled1, 12);
    // draw2(plc, oled1);

    bool quit = false;
    SDL_Event e;
    int swap = 0;
    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        oled1.wipe();
        // sample2(plc, oled1);
        if(swap){
            // setPage(plc, oled1, 0xB0);
            // setCol(plc, oled1, 12);
            draw2(plc, oled1);
        }
        else{
            // setPage(plc, oled1, 0xB0);
            // setCol(plc, oled1, 12);
            draw3(plc, oled1);
            // sample2(plc, oled1);
        }
        swap=!swap;

        renderScreen(oled1.GDDR);
        
        SDL_Delay(50); // Add a small delay to control refresh rate
    }

    close();
    return 0;
}
