#ifndef COLOR_H
#define COLOR_H


class Color
{
    public:
        static const char ORANGE;
        static const char RED;
        static const char WHITE;
        static const char YELLOW;
        static const char BLUE;
        static const char GREEN;
        static const char[6] COLORS;
        virtual void pointless() = 0;
};

#endif // COLOR_H
