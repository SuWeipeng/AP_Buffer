#ifndef __AP_BUFFER_RING_H__
#define __AP_BUFFER_RING_H__

#include "AP_Buffer_Backend.h"

class AP_Buffer_Ring : public AP_Buffer_Backend{
public:
  AP_Buffer_Ring(AP_Buffer &instance, void* w_buf, void* r_buf);
  ~AP_Buffer_Ring(){};
  
  virtual void       write(const void *pBuffer, uint16_t size);
  virtual uint16_t   read(void);
  
private:
  void* _head;
  void* _tail;
  void* _r_buf;
};

#endif /* __AP_BUFFER_RING_H__ */
