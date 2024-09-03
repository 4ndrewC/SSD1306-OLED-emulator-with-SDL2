#include "master.cpp"
#include "oled.h"

class oled: public master{
    public:
        // registers
        const u8 WIDTH = 128;
        const u8 HEIGHT = 32;

        u8 A, B, C, D, E, F;
        u8 start_col_low, start_col_high, col = 0;
        u8 start_page, page;
        u8 pixel_color = 0xFF;
        u8 follow = 1;

        u8 bitpos = 0;
        
        vector<vector<u8>> display, GDDR;

        oled(vector<u8>& a): master(a){
            display.assign(HEIGHT, vector<u8>(WIDTH, 0x00));
            GDDR.assign(HEIGHT, vector<u8>(WIDTH, 0x00));
        }
        
        void set_display(){
            for(int i = 0; i<HEIGHT; i++){
                for(int j = 0; j<WIDTH; j++) display[i][j] = GDDR[i][j];
            }
        }

        void show_display(){
            if(follow){
                for(int i = 0; i<HEIGHT; i++){
                    for(int j = 0; j<WIDTH; j++) cout<<hex<<(int)GDDR[i][j]<<" ";
                    cout<<endl;
                }
                return;
            }
            for(int i = 0; i<HEIGHT; i++){
                for(int j = 0; j<WIDTH; j++) cout<<hex<<(int)display[i][j]<<" ";
                cout<<endl;
            }
        }

        void wipe(){
            for(int i = 0 ;i<HEIGHT; i++){
                for(int j = 0; j<WIDTH; j++) GDDR[i][j] = 0x00;
            }
        }

        void action(){
            switch(ctrx){
                case DSTR:

                    break;

                case SDB:
                    cout<<"in here"<<endl;
                    cout<<(int)drx<<endl;
                    while(bitpos<8){
                        if(drx&1){
                             GDDR[page*8 + bitpos][col] = pixel_color;
                        }
                        drx>>=1;
                        ++bitpos;
                    }
                    bitpos = 0;
                    ++col;
                    if(col>=0x80){
                        col = 0;
                        ++page;
                        page%=4;
                    }
                    // show_display();
                    break;

                case SCB:
                    if(cmd>=SET_COL_LOW_MIN && cmd<=SET_COL_LOW_MAX){
                        cout<<"yup2"<<endl;
                        start_col_low = 0;
                        u8 cur_bit;
                        for(int j = 0; j<4; j++){
                            cur_bit = (cmd&8)>>3;
                            start_col_low = (start_col_low<<1) | (int)cur_bit;
                            cmd<<=1;
                        }
                        col &= 0xF0;
                        col |= start_col_low;
                    }
                    else if(cmd>=SET_COL_HIGH_MIN && cmd<=SET_COL_HIGH_MAX){
                        cout<<"yup"<<endl;
                        start_col_high = 0;
                        u8 cur_bit;
                        for(int j = 0; j<4; j++){
                            cur_bit = (cmd&8)>>3;
                            start_col_high = (start_col_high<<1) | (int)cur_bit;
                            cmd<<=1;
                        }
                        start_col_high<<=4;
                        col &= 0x0F;
                        col |= start_col_high;
                    }
                    else if(cmd>=SET_PAGE_START_MIN && cmd<=SET_PAGE_START_MAX){
                        start_page = 0;
                        u8 cur_bit;
                        for(int j = 0; j<2; j++){
                            cur_bit = (cmd&2)>>1;
                            start_page = (start_page<<1) | (int)cur_bit;
                            cmd<<=1;
                        }
                        page = start_page;
                        cout<<"page set to: "<<(int)page<<endl;
                    }
                    else{
                        switch(cmd){
                            case SET_DISP_ON:
                                break;
                            case SET_DISP_OFF:
                                break;
                            case SET_FOLLOW:
                                // set_display();
                                follow = 1;
                                break;
                            case SET_IGNORE:
                                // should only turn the screen black, not reset display
                                for(int i = 0; i<HEIGHT; i++){
                                    for(int j = 0; j<WIDTH; j++) display[i][j] = 0x00;
                                }
                                break;
                        }
                    }
                    break;

                case CSTR:
                    switch(cmd){
                        
                        
                    }

                    break;
            }
        }

};