#ifndef SERIAL_PORT__CRC_CHECK_HPP_
#define SERIAL_PORT__CRC_CHECK_HPP_

#include <iostream>
#include <stdint.h>

namespace rm_auto_aim
{
    class CrcCheck
    {
    public:
        CrcCheck();
        ~CrcCheck();
        // 读取检查部分8位
        unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength, unsigned char ucCRC8);
        unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
        // 读取检查部分16位
        uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC);
        uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
        //写入检查
        void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
        void Append_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
    };
}

#endif