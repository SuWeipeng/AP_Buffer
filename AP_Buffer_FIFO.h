#ifndef __AP_BUFFER_FIFO_H__
#define __AP_BUFFER_FIFO_H__

#include "AP_Buffer_Backend.h"

class AP_Buffer_FIFO : public AP_Buffer_Backend{
public:
  AP_Buffer_FIFO(AP_Buffer &instance, void* w_buf, void* r_buf);
  ~AP_Buffer_FIFO(){};
  
  virtual void       write(const void *pBuffer, uint16_t size);
  virtual uint16_t   read(void);
  
private:
  void* _w_buf;
  void* _r_buf;
};

#endif /* __AP_Buffer_FIFO_H__ */
