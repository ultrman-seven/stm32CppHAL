#ifndef EE2DDE29_77F8_423A_8314_4B843FECD94D
#define EE2DDE29_77F8_423A_8314_4B843FECD94D
#include "chipf10x.hpp"
#include "wyOstream.hpp"

namespace USART
{
    class Serial:public __wyOstream::WyOstream4MCU
    {
    private:
        USART_TypeDef *uart;

    public:
        Serial(uint8_t, const char *rx = nullptr, const char *tx = nullptr, uint32_t = 115200);

        virtual void putChar(uint8_t);

        void sendByte(uint8_t dat);
        void sendByte(uint8_t *dat, uint8_t len);

        // Serial &operator<<(uint8_t dat);
        // Serial &operator<<(const char dat);
        // Serial &operator<<(int32_t num);
        // Serial &operator<<(float num);
        // Serial &operator<<(const char *s);
        // Serial &operator<<(char *s);

        Serial &operator>>(uint8_t &dat);

        /// @brief
        /// @param
        /// @return
        bool received(void);
        /// @brief
        /// @param
        /// @return
        uint8_t receiveByte(void);

        /// @brief enable interrupt
        /// @param f if callback is given, u deal with rx data whatever u want.
        /// Otherwise rx data will de stored in fifo
        void setInterrupt(void (*f)(uint8_t) = nullptr);

        /**
         * @brief readBuff
         *
         * @param d to store data that u want to read
         * @return true: \n
         *  there is data in FIFO and u read data successfully. \n
         * @return false: \n
         *  no data in FIFO and u get nothing.
         */
        bool readBuff(uint8_t &d);
        bool readBuff(uint8_t *d, uint8_t &len);

        /// @brief setStartTrigger
        /// @param d
        /// @param l
        void setStartTrigger(uint8_t *d, uint8_t l);
        /// @brief setStopTrigger
        /// @param d
        /// @param l
        void setStopTrigger(uint8_t *d, uint8_t l);

        /// @brief setDMA
        /// @param add
        /// @param len
        void setDMA(uint8_t *add, uint8_t len);
    };

} // namespace USART

#endif /* EE2DDE29_77F8_423A_8314_4B843FECD94D */
