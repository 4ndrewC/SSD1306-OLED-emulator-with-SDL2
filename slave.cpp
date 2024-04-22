#include "I2C.cpp"

#define ACK1 8
#define ACK2 16
#define ACK3 24

const static u8 DATASIZE = 8;
const static u8 ADDRSIZE = 7;
const static u8 CTRLSIZE = 6;
const static u8 RWLOC = ADDRSIZE;
const static u8 DATALOC = RWLOC+1;

class slave: public I2C{
    public:
        //registers
        u8 idx = 0;
        u8 drx = 0;
        u8 state = 1;
        u8 ctrx = 0;
        u8 cmd = 0;

        // read register
        u8 rrx = 0;

        vector<u8> addr, received_addr, data;

        //bits
        u8 rec = 1;
        u8 rw;
        u8 dc;
        u8 co;

        slave(vector<u8> &a){
            addr.resize(ADDRSIZE);
            received_addr.resize(ADDRSIZE);
            data.resize(DATASIZE);
            for(int i = 0; i<ADDRSIZE; i++){
                addr[i] = a[i];
            }
        }

        u8 checkaddr(){
            for(int i = 0; i<ADDRSIZE; i++){
                if(addr[i]!=received_addr[i]){
                    return 0;
                }
            }
            return 1;
        }

        void sendack(){
            sda_send(1);
            tick();
        }

        void reset(){
            ctrx = 0;
            cmd = 0;
            drx = 0;
            rec = 1;
        }

        void receive(){
            u8 bit = SDA[WINDOW-1];
            if(!rec) return;
            if(start){
                if(state&(1<<0)){
                    cout<<"state 1: target address"<<endl;
                    received_addr[idx] = bit;
                    // int temp = (int)idx;
                    ++idx;
                    if(idx==ADDRSIZE){
                        idx = 0;
                        //move to next state
                        rec = checkaddr();
                        if(!rec){
                            cout<<"Address not matched"<<endl;
                            return;
                        }
                        reset();
                        state<<=1;
                    }
                }
                else if(state&(1<<1)){
                    cout<<"state 2: r/w bit"<<endl;
                    rw = bit;
                    state<<=1;
                    sendack();
                }
                else if(state&(1<<2)){
                    cout<<"state 3: D/C bit"<<endl;
                    dc = bit;
                    ctrx = (ctrx<<1) | static_cast<int>(bit);
                    state<<=1;
                }
                else if(state&(1<<3)){
                    cout<<"state 4: continuity bit"<<endl;
                    co = bit;
                    ctrx = (ctrx<<1) | static_cast<int>(bit);
                    state<<=1;
                }
                else if(state&(1<<4)){
                    cout<<"state 5: control byte"<<endl;
                    // data[idx] = bit;
                    ctrx = (ctrx<<1) | static_cast<int>(bit);
                    // int temp = (int)idx;
                    ++idx;
                    if(idx==CTRLSIZE){
                        idx = 0;
                        state<<=1;
                        cout<<"control: "<<hex<<(int)ctrx<<endl;
                        sendack();
                    }
                }
                else if(state&(1<<5)){
                    cout<<"state 6: data byte"<<endl;
                    data[idx] = bit;
                    drx = (drx<<1) | static_cast<int>(bit);
                    // int temp = (int)idx;
                    ++idx;
                    if(idx==DATASIZE){
                        idx = 0;
                        state<<=1;
                        cout<<"data: "<<dec<<(int)drx<<endl;
                        if(rw){
                            rrx = EEPROM[drx];
                            cout<<"read data: "<<(int)drx<<endl;
                        }
                        else{
                            EEPROM[memptr] = drx;
                            ++memptr;
                            cout<<"wrote data: "<<(int)drx<<" into mem location: "<<memptr-1<<endl;
                        }
                        if(dc){
                            cmd = drx;
                        }
                    }
                }
            }
        }
};