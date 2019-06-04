#ifndef __TIMER_H__
#define __TIMER_H__
class timer{
    public:
        timer(int limit);
        boolean check();
        void change(int limit);
        void reset();
};
#endif
