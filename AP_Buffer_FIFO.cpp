#if defined(USE_RTTHREAD)
#include <entry.h>
#else
#include <string.h>
#endif
#include "AP_Buffer_FIFO.h"

#ifndef USE_RTTHREAD
#define rt_memmove memmove
#endif

AP_Buffer_FIFO::AP_Buffer_FIFO(AP_Buffer &instance, void* w_buf, void* r_buf)
: AP_Buffer_Backend(instance)
, _w_buf(w_buf)
, _r_buf(r_buf)
{
}

void
AP_Buffer_FIFO::write(const void *pBuffer, uint16_t size)
{
  uint8_t* p_buf = (uint8_t*)pBuffer;
  uint8_t* w_buf = (uint8_t*)_w_buf;
  uint16_t write_size = size;
  if(size > AP_BUFFER_MAX_SIZE){
    p_buf = p_buf+size-AP_BUFFER_MAX_SIZE;
    write_size = AP_BUFFER_MAX_SIZE;
  }
  rt_memmove(w_buf+write_size, w_buf, AP_BUFFER_MAX_SIZE-write_size);
  rt_memmove(w_buf, p_buf, write_size);
  _frontend._buf.count += size;
}

uint16_t
AP_Buffer_FIFO::read(void)
{
  uint8_t* to_pos = (uint8_t*)_r_buf;
  uint8_t* from_pos;
  uint16_t read_cnt;
  uint16_t cnt = 0;
  
  if(_frontend._buf.count >= AP_BUFFER_MAX_SIZE){
    from_pos = (uint8_t*)_w_buf + AP_BUFFER_MAX_INDEX;
    read_cnt = AP_BUFFER_MAX_SIZE;
  } else {
    from_pos = (uint8_t*)_w_buf + _frontend._buf.count;
    read_cnt = _frontend._buf.count;
  }
  for(uint16_t i=0; i<read_cnt; i++){
    rt_memmove(to_pos+i, from_pos-i, 1);
    _frontend._buf.count -= 1;
    cnt++;
  }
  if(_frontend._buf.count != 0){
    _frontend._buf.count = 0;
  }
  return cnt;
}
