#ifndef SevenSegmentAsciiMap_H
#define SevenSegmentAsciiMap_H

#include <Arduino.h>

// ASCII MAPPINGS
#define TM1637_CHAR_SPACE       B00000000 // 32  (ASCII)
#define TM1637_CHAR_EXC         B00110000
#define TM1637_CHAR_D_QUOTE     B00010100
#define TM1637_CHAR_POUND       B01110110
#define TM1637_CHAR_DOLLAR      B01101101
#define TM1637_CHAR_PERC        B00100100
#define TM1637_CHAR_AMP         B01111111
#define TM1637_CHAR_S_QUOTE     B00000100
#define TM1637_CHAR_L_BRACKET   B00001111
#define TM1637_CHAR_R_BRACKET   B00111001
#define TM1637_CHAR_STAR        B01100011
#define TM1637_CHAR_PLUS        B01000010
#define TM1637_CHAR_COMMA       B00000010
#define TM1637_CHAR_MIN         B01000000
#define TM1637_CHAR_DOT         B00000001
#define TM1637_CHAR_F_SLASH     B00110000
#define TM1637_CHAR_0           B00111111   // 48
#define TM1637_CHAR_1           B00110000
#define TM1637_CHAR_2           B01011011
#define TM1637_CHAR_3           B01111001
#define TM1637_CHAR_4           B01110100
#define TM1637_CHAR_5           B01101101
#define TM1637_CHAR_6           B01101111
#define TM1637_CHAR_7           B00111000
#define TM1637_CHAR_8           B01111111
#define TM1637_CHAR_9           B01111101
#define TM1637_CHAR_COLON       B00000110
#define TM1637_CHAR_S_COLON     B00000110
#define TM1637_CHAR_LESS        B01000011
#define TM1637_CHAR_EQUAL       B01000001
#define TM1637_CHAR_GREAT       B01100001
#define TM1637_CHAR_QUEST       B01011010
#define TM1637_CHAR_AT          B01111011
#define TM1637_CHAR_A           B01111110 // 65  (ASCII)
#define TM1637_CHAR_B           B01111111
#define TM1637_CHAR_C           B00001111
#define TM1637_CHAR_D           TM1637_CHAR_d
#define TM1637_CHAR_E           B01001111
#define TM1637_CHAR_F           B01001110
#define TM1637_CHAR_G           B00101111
#define TM1637_CHAR_H           B01110110
#define TM1637_CHAR_I           B00110000
#define TM1637_CHAR_J           B00110001
#define TM1637_CHAR_K           B01101110
#define TM1637_CHAR_L           B00000111
#define TM1637_CHAR_M           B00101010
#define TM1637_CHAR_N           B00111110
#define TM1637_CHAR_O           B00111111
#define TM1637_CHAR_P           B01011110
#define TM1637_CHAR_Q           B01111100
#define TM1637_CHAR_R           B00011110
#define TM1637_CHAR_S           B01101101
#define TM1637_CHAR_T           TM1637_CHAR_t
#define TM1637_CHAR_U           B00110111
#define TM1637_CHAR_V           B00100011
#define TM1637_CHAR_W           B00010101
#define TM1637_CHAR_X           TM1637_CHAR_H
#define TM1637_CHAR_Y           B01110101
#define TM1637_CHAR_Z           B01011011
#define TM1637_CHAR_L_S_BRACKET B00001111 // 91 (ASCII)
#define TM1637_CHAR_B_SLASH     B00000110
#define TM1637_CHAR_R_S_BRACKET B00111001
#define TM1637_CHAR_A_CIRCUM    B00011010
#define TM1637_CHAR_UNDERSCORE  B00000001
#define TM1637_CHAR_A_GRAVE     B00000010
#define TM1637_CHAR_a           B01111011 // 97 (ASCII)
#define TM1637_CHAR_b           B01100111
#define TM1637_CHAR_c           B01000011
#define TM1637_CHAR_d           B01110011
#define TM1637_CHAR_e           B01011111
#define TM1637_CHAR_f           TM1637_CHAR_F
#define TM1637_CHAR_g           B01111101
#define TM1637_CHAR_h           B01100110
#define TM1637_CHAR_i           B00100000
#define TM1637_CHAR_j           B00100001
#define TM1637_CHAR_k           TM1637_CHAR_K
#define TM1637_CHAR_l           B00000110
#define TM1637_CHAR_m           TM1637_CHAR_M
#define TM1637_CHAR_n           B01100010
#define TM1637_CHAR_o           B01100011
#define TM1637_CHAR_p           TM1637_CHAR_P
#define TM1637_CHAR_q           TM1637_CHAR_Q
#define TM1637_CHAR_r           B01000010
#define TM1637_CHAR_s           TM1637_CHAR_S
#define TM1637_CHAR_t           B01000111
#define TM1637_CHAR_u           B00100011
#define TM1637_CHAR_v           B00100011
#define TM1637_CHAR_w           TM1637_CHAR_W
#define TM1637_CHAR_x           TM1637_CHAR_X
#define TM1637_CHAR_y           B01110100
#define TM1637_CHAR_z           TM1637_CHAR_Z
#define TM1637_CHAR_L_ACCON     B01001111 // 123 (ASCII)
#define TM1637_CHAR_BAR         B00110000
#define TM1637_CHAR_R_ACCON     B01111001
#define TM1637_CHAR_TILDE       B01000000 // 126 (ASCII)



class AsciiMap {
public:
    const static uint8_t map[96];
};
// static const uint8_t asciiMap[96];

#endif
