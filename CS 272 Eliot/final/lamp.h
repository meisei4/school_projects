#ifndef __LAMP_H__
#define __LAMP_H__
class lamp{
    public:
        lamp(int i1, int i2, int o3);
        void lampdim();
        void lampmonitor();
        void lampswitch();
        void turnoff();
        void turnon();
    private:
        void debouncebut();
};

#endif
