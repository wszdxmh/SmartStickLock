#ifndef __OLED_LIBRARY_H
#define __OLED_LIBRARY_H

//static const unsigned char F6x8_num[][6] =
//{
//    { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0				  0
//    { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1				  1
//    { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2				  2
//    { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3				  3
//    { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4				  4
//    { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5				  5
//    { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6				  6
//    { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7				  7
//    { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8				  8
//    { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9				  9
//    { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :				  10
//    { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;				  11
//    { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <				  12
//    { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =				  13
//    { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >				  14
//    { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?				  15
//    { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @				  16
//    { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A				  17
//    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B				  18
//    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C				  19
//    { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D				  20
//    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E				  21
//    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F				  22
//    { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G				  23
//    { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H				  24
//    { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I				  25
//    { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J				  26
//    { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K				  27
//    { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L				  28
//    { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M				  29
//    { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N				  30
//    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O				  31
//    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P				  32
//    { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q				  33
//    { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R				  34
//    { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S				  35
//    { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T				  36
//    { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U				  37
//    { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V				  38
//    { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W				  39
//    { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X				  40
//    { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y				  41
//    { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z				  42
//    { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [				  43
//    { 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55				  44
//    { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]				  45
//    { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^				  46
//    { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _				  47
//    { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '				  48
//    { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a				  49
//    { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b				  50
//    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c				  51
//    { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d				  52
//    { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e				  53
//    { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f				  54
//    { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g				  55
//    { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h				  56
//    { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i				  57
//    { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j				  58
//    { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k				  59
//    { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l				  60
//    { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m				  61
//    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n				  62
//    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o				  63
//    { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p				  64
//    { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q				  65
//    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r				  66
//    { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s				  67
//    { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t				  68
//    { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u				  69
//    { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v				  70
//    { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w				  71
//    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x				  72
//    { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y				  73
//    { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z				  74
//	  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp				  75
//    { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !				  76
//    { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "				  77
//    { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #				  78
//    { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $				  79
//    { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %				  80
//    { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &				  81
//    { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '				  82
//    { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (				  83
//    { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )				  84
//    { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *				  85
//    { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +				  86
//    { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,				  87
//    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -				  88
//    { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .				  89
//    { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /				  90
//    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines	  91
//};





//static const unsigned char F8x16num[][16] =
//{
//  {0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00},//0
//  {0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},//1
//  {0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00},//2
//  {0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00},//3
//  {0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00},//4
//  {0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00},//5
//  {0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00},//6
//  {0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00},//7
//  {0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00},//8
//  {0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00} //9
//};

////======================================================
//// 128X64IҺ���ײ�����[8X16]�����
//// �����: powerint
//// ��  ��: [8X16]�����ַ�����ģ���� (����ȡģ,�ֽڵ���)
//// !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
////======================================================

//static unsigned char F8X16[]=
//{
//  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,// 0
//  0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00,//!1
//  0x00,0x10,0x0C,0x06,0x10,0x0C,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//"2
//  0x40,0xC0,0x78,0x40,0xC0,0x78,0x40,0x00,0x04,0x3F,0x04,0x04,0x3F,0x04,0x04,0x00,//#3
//  0x00,0x70,0x88,0xFC,0x08,0x30,0x00,0x00,0x00,0x18,0x20,0xFF,0x21,0x1E,0x00,0x00,//$4
//  0xF0,0x08,0xF0,0x00,0xE0,0x18,0x00,0x00,0x00,0x21,0x1C,0x03,0x1E,0x21,0x1E,0x00,//%5
//  0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00,0x1E,0x21,0x23,0x24,0x19,0x27,0x21,0x10,//&6
//  0x10,0x16,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//'7
//  0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00,0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00,//(8
//  0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00,//)9
//  0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00,//*10
//  0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x1F,0x01,0x01,0x01,0x00,//+11
//  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xB0,0x70,0x00,0x00,0x00,0x00,0x00,//,12
//  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,//-13
//  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00,//.14
//  0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x04,0x00,0x60,0x18,0x06,0x01,0x00,0x00,0x00,///15
//  0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,//016
//  0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//117
//  0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,//218
//  0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,//319
//  0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00,//420
//  0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00,//521
//  0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,//622
//  0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,//723
//  0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,//824
//  0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00,//925
//  0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,//:26
//  0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x60,0x00,0x00,0x00,0x00,//;27
//  0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x00,//<28
//  0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,//=29
//  0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00,//>30
//  0x00,0x70,0x48,0x08,0x08,0x08,0xF0,0x00,0x00,0x00,0x00,0x30,0x36,0x01,0x00,0x00,//?31
//  0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00,0x07,0x18,0x27,0x24,0x23,0x14,0x0B,0x00,//@32
//  0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20,//A33
//  0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00,//B34
//  0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00,//C35
//  0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00,//D36
//  0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00,//E37
//  0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00,//F38
//  0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00,//G39
//  0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20,//H40
//  0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//I41
//  0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00,//J42
//  0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00,//K43
//  0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00,//L44
//  0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00,//M45
//  0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00,//N46
//  0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00,//O47
//  0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00,//P48
//  0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x18,0x24,0x24,0x38,0x50,0x4F,0x00,//Q49
//  0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20,//R50
//  0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00,//S51
//  0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//T52
//  0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//U53
//  0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00,//V54
//  0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00,0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00,//W55
//  0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20,//X56
//  0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//Y57
//  0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00,//Z58
//  0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00,//[59
//  0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00,//\60
//  0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00,//]61
//  0x00,0x00,0x04,0x02,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//^62
//  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,//_63
//  0x00,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//`64
//  0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20,//a65
//  0x08,0xF8,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00,//b66
//  0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00,//c67
//  0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00,0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20,//d68
//  0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00,//e69
//  0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x18,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//f70
//  0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00,//g71
//  0x08,0xF8,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//h72
//  0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//i73
//  0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,//j74
//  0x08,0xF8,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x02,0x2D,0x30,0x20,0x00,//k75
//  0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//l76
//  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F,//m77
//  0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//n78
//  0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//o79
//  0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00,//p80
//  0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0xA0,0xFF,0x80,//q81
//  0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00,//r82
//  0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00,//s83
//  0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00,//t84
//  0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,//u85
//  0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x01,0x0E,0x30,0x08,0x06,0x01,0x00,//v86
//  0x80,0x80,0x00,0x80,0x00,0x80,0x80,0x80,0x0F,0x30,0x0C,0x03,0x0C,0x30,0x0F,0x00,//w87
//  0x00,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x31,0x2E,0x0E,0x31,0x20,0x00,//x88
//  0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x80,0x81,0x8E,0x70,0x18,0x06,0x01,0x00,//y89
//  0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00,//z90
//  0x00,0x00,0x00,0x00,0x80,0x7C,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40,//{91
//  0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,//|92
//  0x00,0x02,0x02,0x7C,0x80,0x00,0x00,0x00,0x00,0x40,0x40,0x3F,0x00,0x00,0x00,0x00,//}93
//  0x00,0x06,0x01,0x01,0x02,0x02,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//~94

//};

static unsigned char Please_Input_Passwd[]=
{
	0x40,0x42,0xCC,0x00,0x00,0x44,0x54,0x54,0x54,0x7F,0x54,0x54,0x54,0x44,0x40,0x00,
	0x00,0x00,0x7F,0x20,0x10,0x00,0xFF,0x15,0x15,0x15,0x55,0x95,0x7F,0x00,0x00,0x00,/*"��",0*/

	0x88,0x68,0x1F,0xC8,0x08,0x10,0xC8,0x54,0x52,0xD1,0x12,0x94,0x08,0xD0,0x10,0x00,
	0x09,0x19,0x09,0xFF,0x05,0x00,0xFF,0x12,0x92,0xFF,0x00,0x5F,0x80,0x7F,0x00,0x00,/*"��",1*/

	0x00,0x00,0x00,0x00,0x00,0x01,0xE2,0x1C,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x80,0x40,0x20,0x10,0x0C,0x03,0x00,0x00,0x00,0x03,0x0C,0x30,0x40,0x80,0x80,0x00,/*"��",2*/

	0x10,0x8C,0x44,0x04,0xE4,0x04,0x95,0xA6,0x44,0x24,0x14,0x84,0x44,0x94,0x0C,0x00,
	0x02,0x02,0x7A,0x41,0x41,0x43,0x42,0x7E,0x42,0x42,0x42,0x43,0xF8,0x00,0x00,0x00,/*"��",3*/

	0x04,0x84,0xE4,0x5C,0x44,0xC4,0x00,0x02,0xF2,0x82,0x82,0x82,0xFE,0x80,0x80,0x00,
	0x02,0x01,0x7F,0x10,0x10,0x3F,0x00,0x08,0x08,0x08,0x08,0x48,0x88,0x40,0x3F,0x00,/*"��",4*/

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*":",5*/
};

static unsigned char Input_ID[] = 
{
	0x40,0x42,0xCC,0x00,0x00,0x44,0x54,0x54,0x54,0x7F,0x54,0x54,0x54,0x44,0x40,0x00,
	0x00,0x00,0x7F,0x20,0x10,0x00,0xFF,0x15,0x15,0x15,0x55,0x95,0x7F,0x00,0x00,0x00,/*"��",0*/

	0x88,0x68,0x1F,0xC8,0x08,0x10,0xC8,0x54,0x52,0xD1,0x12,0x94,0x08,0xD0,0x10,0x00,
	0x09,0x19,0x09,0xFF,0x05,0x00,0xFF,0x12,0x92,0xFF,0x00,0x5F,0x80,0x7F,0x00,0x00,/*"��",0*/

	0x00,0x00,0x00,0x00,0x00,0x01,0xE2,0x1C,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x80,0x40,0x20,0x10,0x0C,0x03,0x00,0x00,0x00,0x03,0x0C,0x30,0x40,0x80,0x80,0x00,/*"��",1*/

	0x00,0x00,0xFC,0x04,0x04,0x04,0x14,0x15,0x56,0x94,0x54,0x34,0x14,0x04,0x04,0x00,
	0x40,0x30,0x0F,0x00,0x01,0x01,0x01,0x41,0x81,0x7F,0x01,0x01,0x01,0x05,0x03,0x00,/*"��",2*/

	0x80,0x80,0x80,0xBE,0xA2,0xA2,0xA2,0xA2,0xA2,0xA2,0xA2,0xBE,0x80,0x80,0x80,0x00,
	0x00,0x00,0x00,0x06,0x05,0x04,0x04,0x04,0x44,0x84,0x44,0x3C,0x00,0x00,0x00,0x00,/*"��",3*/
};

static unsigned char Star[]=
{
	0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00 /* "*" */
};

static unsigned char Please_Fingerprint[]=
{
	0x40,0x42,0xCC,0x00,0x00,0x44,0x54,0x54,0x54,0x7F,0x54,0x54,0x54,0x44,0x40,0x00,
	0x00,0x00,0x7F,0x20,0x10,0x00,0xFF,0x15,0x15,0x15,0x55,0x95,0x7F,0x00,0x00,0x00,/*"��",0*/

	0x10,0x10,0x10,0xFF,0x90,0x20,0x98,0x88,0x88,0xE9,0x8E,0x88,0x88,0xA8,0x98,0x00,
	0x02,0x42,0x81,0x7F,0x00,0x00,0x80,0x84,0x4B,0x28,0x10,0x28,0x47,0x80,0x00,0x00,/*"��",1*/

	0x10,0x10,0x10,0xFF,0x10,0x90,0x00,0x3F,0x48,0x48,0x44,0x44,0x44,0x42,0x70,0x00,
	0x04,0x44,0x82,0x7F,0x01,0x00,0x00,0xFF,0x49,0x49,0x49,0x49,0x49,0xFF,0x00,0x00,/*"ָ",2*/

	0x20,0x30,0xAC,0x63,0x10,0x08,0x08,0xF8,0x09,0x0E,0x08,0xF8,0x08,0x08,0x00,0x00,
	0x22,0x67,0x22,0x12,0x92,0x80,0x40,0x21,0x16,0x08,0x16,0x21,0x40,0x80,0x80,0x00,/*"��",3*/

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*":",4*/
};

//static unsigned char Please_Agan_Fingerprint[]=
//{
//	0x40,0x42,0xCC,0x00,0x00,0x44,0x54,0x54,0x54,0x7F,0x54,0x54,0x54,0x44,0x40,0x00,
//	0x00,0x00,0x7F,0x20,0x10,0x00,0xFF,0x15,0x15,0x15,0x55,0x95,0x7F,0x00,0x00,0x00,/*"��",0*/

//	0x02,0x02,0xF2,0x92,0x92,0x92,0x92,0xFE,0x92,0x92,0x92,0x92,0xF2,0x02,0x02,0x00,
//	0x04,0x04,0xFF,0x04,0x04,0x04,0x04,0x07,0x04,0x04,0x44,0x84,0x7F,0x04,0x04,0x00,/*"��",1*/

//	0x00,0x02,0x0C,0x80,0x60,0x80,0x40,0x30,0x0F,0xC8,0x08,0x08,0x28,0x18,0x00,0x00,
//	0x02,0x02,0x7E,0x01,0x80,0x40,0x20,0x18,0x06,0x01,0x06,0x18,0x20,0x40,0x80,0x00,/*"��",2*/

//	0x88,0x68,0x1F,0xC8,0x08,0x10,0xC8,0x54,0x52,0xD1,0x12,0x94,0x08,0xD0,0x10,0x00,
//	0x09,0x19,0x09,0xFF,0x05,0x00,0xFF,0x12,0x92,0xFF,0x00,0x5F,0x80,0x7F,0x00,0x00,/*"��",3*/

//	0x00,0x00,0x00,0x00,0x00,0x01,0xE2,0x1C,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//	0x80,0x40,0x20,0x10,0x0C,0x03,0x00,0x00,0x00,0x03,0x0C,0x30,0x40,0x80,0x80,0x00,/*"�� ",4*/

//	0x10,0x10,0x10,0xFF,0x10,0x90,0x00,0x3F,0x48,0x48,0x44,0x44,0x44,0x42,0x70,0x00,
//	0x04,0x44,0x82,0x7F,0x01,0x00,0x00,0xFF,0x49,0x49,0x49,0x49,0x49,0xFF,0x00,0x00,/*"ָ",5*/

//	0x20,0x30,0xAC,0x63,0x10,0x08,0x08,0xF8,0x09,0x0E,0x08,0xF8,0x08,0x08,0x00,0x00,
//	0x22,0x67,0x22,0x12,0x92,0x80,0x40,0x21,0x16,0x08,0x16,0x21,0x40,0x80,0x80,0x00,/*"��",6*/

//	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//	0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*":",7*/
//};

static unsigned char Entery_Success[] = 
{

	0x80,0x80,0x82,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0xFE,0x80,0x80,0x80,0x00,
	0x40,0x40,0x22,0x24,0x10,0x48,0x80,0x7F,0x04,0x08,0x18,0x24,0x22,0x40,0x40,0x00,/*"¼",0*/

	0x00,0x00,0x00,0x00,0x00,0x01,0xE2,0x1C,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x80,0x40,0x20,0x10,0x0C,0x03,0x00,0x00,0x00,0x03,0x0C,0x30,0x40,0x80,0x80,0x00,/*"��",1*/

	0x00,0x00,0xF8,0x88,0x88,0x88,0x88,0x08,0x08,0xFF,0x08,0x09,0x0A,0xC8,0x08,0x00,
	0x80,0x60,0x1F,0x00,0x10,0x20,0x1F,0x80,0x40,0x21,0x16,0x18,0x26,0x41,0xF8,0x00,/*"��",2*/

	0x08,0x08,0x08,0xF8,0x08,0x08,0x08,0x10,0x10,0xFF,0x10,0x10,0x10,0xF0,0x00,0x00,
	0x10,0x30,0x10,0x1F,0x08,0x88,0x48,0x30,0x0E,0x01,0x40,0x80,0x40,0x3F,0x00,0x00,/*"��",3*/
};

static unsigned char Entery_Error[] = 
{
0x80,0x80,0x82,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0xFE,0x80,0x80,0x80,0x00,
0x40,0x40,0x22,0x24,0x10,0x48,0x80,0x7F,0x04,0x08,0x18,0x24,0x22,0x40,0x40,0x00,/*"¼",0*/

0x00,0x00,0x00,0x00,0x00,0x01,0xE2,0x1C,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x80,0x40,0x20,0x10,0x0C,0x03,0x00,0x00,0x00,0x03,0x0C,0x30,0x40,0x80,0x80,0x00,/*"��",1*/

0x40,0x30,0xEF,0x24,0x64,0x48,0x48,0x7F,0x48,0x48,0x48,0x7F,0x48,0x48,0x40,0x00,
0x01,0x01,0x7F,0x21,0x11,0x00,0xFF,0x49,0x49,0x49,0x49,0x49,0xFF,0x00,0x00,0x00,/*"��",2*/

0x40,0x42,0xCC,0x00,0x00,0x80,0x9E,0x92,0x92,0x92,0x92,0x92,0x9E,0x80,0x00,0x00,
0x00,0x00,0x7F,0x20,0x94,0x84,0x44,0x24,0x14,0x0F,0x14,0x24,0x44,0x84,0x84,0x00,/*"��",3*/
};

static unsigned char Entery_OK[] =
{
0x80,0x80,0x82,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0xFE,0x80,0x80,0x80,0x00,
0x40,0x40,0x22,0x24,0x10,0x48,0x80,0x7F,0x04,0x08,0x18,0x24,0x22,0x40,0x40,0x00,/*"¼",0*/

0x00,0x00,0x00,0x00,0x00,0x01,0xE2,0x1C,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x80,0x40,0x20,0x10,0x0C,0x03,0x00,0x00,0x00,0x03,0x0C,0x30,0x40,0x80,0x80,0x00,/*"��",1*/

0x10,0x0C,0x04,0x24,0x24,0x24,0x25,0x26,0x24,0x24,0x24,0x24,0x04,0x14,0x0C,0x00,
0x00,0x81,0x81,0x41,0x31,0x0F,0x01,0x01,0x01,0x7F,0x81,0x81,0x81,0xF1,0x00,0x00,/*"��",2*/

0x00,0x00,0xF8,0x88,0x88,0x88,0x88,0x08,0x08,0xFF,0x08,0x09,0x0A,0xC8,0x08,0x00,
0x80,0x60,0x1F,0x00,0x10,0x20,0x1F,0x80,0x40,0x21,0x16,0x18,0x26,0x41,0xF8,0x00,/*"��",3*/
};

static unsigned char Not_Find_Finger[] = 
{
	0x80,0x80,0x88,0x88,0x88,0x88,0x88,0xFF,0x88,0x88,0x88,0x88,0x88,0x80,0x80,0x00,
	0x20,0x20,0x10,0x08,0x04,0x02,0x01,0xFF,0x01,0x02,0x04,0x08,0x10,0x20,0x20,0x00,/*"δ",0*/

	0x10,0x10,0xD0,0xFF,0x90,0x50,0x20,0x50,0x4C,0x43,0x4C,0x50,0x20,0x40,0x40,0x00,
	0x04,0x03,0x00,0xFF,0x00,0x41,0x44,0x58,0x41,0x4E,0x60,0x58,0x47,0x40,0x40,0x00,/*"��",0*/

	0x10,0x60,0x02,0x8C,0x00,0xFE,0x02,0xF2,0x02,0xFE,0x00,0xF8,0x00,0xFF,0x00,0x00,
	0x04,0x04,0x7E,0x01,0x80,0x47,0x30,0x0F,0x10,0x27,0x00,0x47,0x80,0x7F,0x00,0x00,/*"��",0*/

	0x42,0x62,0x52,0x4A,0xC6,0x42,0x52,0x62,0xC2,0x00,0xF8,0x00,0x00,0xFF,0x00,0x00,
	0x40,0xC4,0x44,0x44,0x7F,0x24,0x24,0x24,0x20,0x00,0x0F,0x40,0x80,0x7F,0x00,0x00,/*"��",0*/

	0x10,0x10,0x10,0xFF,0x10,0x90,0x00,0x3F,0x48,0x48,0x44,0x44,0x44,0x42,0x70,0x00,
	0x04,0x44,0x82,0x7F,0x01,0x00,0x00,0xFF,0x49,0x49,0x49,0x49,0x49,0xFF,0x00,0x00,/*"ָ",0*/

	0x20,0x30,0xAC,0x63,0x10,0x08,0x08,0xF8,0x09,0x0E,0x08,0xF8,0x08,0x08,0x00,0x00,
	0x22,0x67,0x22,0x12,0x92,0x80,0x40,0x21,0x16,0x08,0x16,0x21,0x40,0x80,0x80,0x00,/*"��",0*/
};

static unsigned char Correct_Identification[]=
{
	0x40,0x40,0x42,0xCC,0x00,0x00,0x00,0xFC,0x04,0x04,0x04,0x04,0x04,0xFC,0x00,0x00,
	0x00,0x00,0x00,0x3F,0x10,0x88,0x40,0x23,0x19,0x01,0x01,0x09,0x11,0x23,0xC0,0x00,/*"ʶ",0*/

	0x00,0x3E,0x22,0xE2,0x22,0x22,0x22,0x3E,0x00,0x00,0xF8,0x00,0x00,0xFF,0x00,0x00,
	0x81,0x41,0x31,0x0F,0x41,0x81,0x41,0x3F,0x00,0x00,0x0F,0x40,0x80,0x7F,0x00,0x00,/*"��",1*/

	0x00,0x02,0x02,0xC2,0x02,0x02,0x02,0xFE,0x82,0x82,0x82,0x82,0x82,0x02,0x00,0x00,
	0x40,0x40,0x40,0x7F,0x40,0x40,0x40,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,/*"��",2*/

	0x04,0x84,0xE4,0x5C,0x44,0xC4,0x20,0x10,0xE8,0x27,0x24,0xE4,0x34,0x2C,0xE0,0x00,
	0x02,0x01,0x7F,0x10,0x10,0x3F,0x80,0x60,0x1F,0x09,0x09,0x3F,0x49,0x89,0x7F,0x00,/*"ȷ",3*/
};

static unsigned char Identification_Error[]=
{
	0x40,0x40,0x42,0xCC,0x00,0x00,0x00,0xFC,0x04,0x04,0x04,0x04,0x04,0xFC,0x00,0x00,
	0x00,0x00,0x00,0x3F,0x10,0x88,0x40,0x23,0x19,0x01,0x01,0x09,0x11,0x23,0xC0,0x00,/*"ʶ",0*/

	0x00,0x3E,0x22,0xE2,0x22,0x22,0x22,0x3E,0x00,0x00,0xF8,0x00,0x00,0xFF,0x00,0x00,
	0x81,0x41,0x31,0x0F,0x41,0x81,0x41,0x3F,0x00,0x00,0x0F,0x40,0x80,0x7F,0x00,0x00,/*"��",1*/

	0x40,0x30,0xEF,0x24,0x64,0x48,0x48,0x7F,0x48,0x48,0x48,0x7F,0x48,0x48,0x40,0x00,
	0x01,0x01,0x7F,0x21,0x11,0x00,0xFF,0x49,0x49,0x49,0x49,0x49,0xFF,0x00,0x00,0x00,/*"��",2*/

	0x40,0x42,0xCC,0x00,0x00,0x80,0x9E,0x92,0x92,0x92,0x92,0x92,0x9E,0x80,0x00,0x00,
	0x00,0x00,0x7F,0x20,0x94,0x84,0x44,0x24,0x14,0x0F,0x14,0x24,0x44,0x84,0x84,0x00,/*"��",3*/
};

static unsigned char Batey[]=
{
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0x81,0xFF,0x3C,0x3C,
};

static unsigned char NRF_EN[] = 
{
	0xFF,0x81,0xF9,0xF3,0xE7,0xCF,0x81,0xFF
};

//static unsigned char Welcome[] = 
//{
//0x04,0x24,0x44,0x84,0x64,0x9C,0x40,0x30,0x0F,0xC8,0x08,0x08,0x28,0x18,0x00,0x00,
//0x10,0x08,0x06,0x01,0x82,0x4C,0x20,0x18,0x06,0x01,0x06,0x18,0x20,0x40,0x80,0x00,/*"��",0*/

//0x40,0x40,0x42,0xCC,0x00,0x00,0xFC,0x04,0x02,0x00,0xFC,0x04,0x04,0xFC,0x00,0x00,
//0x00,0x40,0x20,0x1F,0x20,0x40,0x4F,0x44,0x42,0x40,0x7F,0x42,0x44,0x43,0x40,0x00,/*"ӭ",1*/

//0x40,0x40,0x42,0x44,0x58,0xC0,0x40,0x7F,0x40,0xC0,0x50,0x48,0x46,0x40,0x40,0x00,
//0x80,0x80,0x40,0x20,0x18,0x07,0x00,0x00,0x00,0x3F,0x40,0x40,0x40,0x40,0x78,0x00,/*"��",2*/

//0x00,0xF8,0x00,0x00,0xFF,0x40,0x20,0x18,0x0F,0x18,0x68,0x08,0x08,0x08,0x08,0x00,
//0x00,0x1F,0x00,0x00,0xFF,0x00,0x00,0x7F,0x21,0x21,0x3F,0x21,0x21,0x7F,0x00,0x00,/*"��",3*/
//};

//static unsigned char P8x8_Number[] = 
//{
//	0x38,0x44,0x38,0x00,/*"0",0*/
//	0x00,0x44,0x7C,0x40,/*"1",1*/
//	0x6C,0x54,0x4C,0x00,/*"2",2*/
//	0x44,0x54,0x6C,0x00,/*"3",3*/
//	0x30,0x68,0x7C,0x60,/*"4",4*/
//	0x4C,0x4C,0x7C,0x00,/*"5",5*/
//	0x78,0x4C,0x7C,0x00,/*"6",6*/
//	0x00,0x04,0x7C,0x00,/*"7",7*/
//	0x6C,0x54,0x6C,0x00,/*"8",8*/
//	0x5C,0x54,0x3C,0x00,/*"9",9*/
//};

static unsigned char Fingerprint_Flag[] = 
{
	0xFF,0x81,0xBD,0xBD,0xBD,0xBD,0x81,0xFF
};

static unsigned char P16x16_Number[] = 
{
	0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,/*"0",0*/
	0x00,0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,/*"1",1*/
	0x00,0x70,0x08,0x08,0x08,0x08,0xF0,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,/*"2",2*/
	0x00,0x30,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x18,0x20,0x21,0x21,0x22,0x1C,0x00,/*"3",3*/
	0x00,0x00,0x80,0x40,0x30,0xF8,0x00,0x00,0x00,0x06,0x05,0x24,0x24,0x3F,0x24,0x24,/*"4",4*/
	0x00,0xF8,0x88,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x20,0x20,0x20,0x11,0x0E,0x00,/*"5",5*/
	0x00,0xE0,0x10,0x88,0x88,0x90,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x20,0x1F,0x00,/*"6",6*/
	0x00,0x18,0x08,0x08,0x88,0x68,0x18,0x00,0x00,0x00,0x00,0x3E,0x01,0x00,0x00,0x00,/*"7",7*/
	0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,/*"8",8*/
	0x00,0xF0,0x08,0x08,0x08,0x10,0xE0,0x00,0x00,0x01,0x12,0x22,0x22,0x11,0x0F,0x00,/*"9",9*/
	0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00,/*"*",10*/
	0x00,0x40,0xC0,0x78,0x40,0xC0,0x78,0x00,0x00,0x04,0x3F,0x04,0x04,0x3F,0x04,0x00,/*"#",11*/
};

#endif