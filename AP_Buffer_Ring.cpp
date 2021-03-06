#if defined(USE_RTTHREAD)
#include <entry.h>
#else
#include <string.h>
#endif
#include "AP_Buffer_Ring.h"

#ifndef USE_RTTHREAD
#define rt_memcpy memcpy
#endif

AP_Buffer_Ring::AP_Buffer_Ring(AP_Buffer &instance, void* w_buf, void* r_buf)
: AP_Buffer_Backend(instance)
, _head(w_buf)
, _tail(w_buf)
, _r_buf(r_buf)
{
}

void
AP_Buffer_Ring::write(const void *pBuffer, uint16_t size)
{
  uint16_t write_size = size;
  uint8_t* tmp_pt     = (uint8_t*)_tail;
  uint16_t check      = &_frontend._buf.w_buf[AP_BUFFER_MAX_INDEX] - tmp_pt;
  
  // Cut pBuffer
  if(size >= AP_BUFFER_MAX_SIZE){
    uint8_t* pBufCut = (uint8_t *)pBuffer;
    pBuffer = pBufCut + size - AP_BUFFER_MAX_SIZE;
    write_size = AP_BUFFER_MAX_SIZE;
  }
  
  // Write to ringbuffer
  if(check >= write_size){
    rt_memcpy(_tail, pBuffer, write_size);
    tmp_pt += write_size;
    _frontend._buf.count += write_size;
  } else {
    uint16_t remain = write_size-(check+1);
    uint8_t* tmp_pBuffer = (uint8_t*)pBuffer+(check+1);
    
    rt_memcpy(_tail, pBuffer, check+1);
    _tail = _frontend._buf.w_buf;
    tmp_pt = (uint8_t*)_tail;
    _frontend._buf.count += (check+1);
    if(remain > 0){
      rt_memcpy(_tail, tmp_pBuffer, remain);
      tmp_pt += write_size-(check+1);
      _frontend._buf.count += remain;
    }
  }
  _tail = tmp_pt;
}

uint16_t
AP_Buffer_Ring::read(void)
{
  uint8_t* tmp_ph     = (uint8_t*)_head;
  uint8_t* to_buf     = (uint8_t*)_r_buf;
  uint16_t check      = &_frontend._buf.w_buf[AP_BUFFER_MAX_INDEX] - tmp_ph;
  uint16_t read_1st   = 0;
  uint16_t read_2nd   = 0;
  
  if(_frontend._buf.count == 0) return 0;
  
  if(_frontend._buf.count >= AP_BUFFER_MAX_SIZE){
    tmp_ph = (uint8_t*)_tail;
    check  = &_frontend._buf.w_buf[AP_BUFFER_MAX_INDEX] - tmp_ph;
    read_1st = check + 1;
    rt_memcpy(to_buf, tmp_ph, read_1st);
    tmp_ph = _frontend._buf.w_buf;
    _frontend._buf.count -= read_1st;
    read_2nd = AP_BUFFER_MAX_SIZE-read_1st;
    if(read_2nd > 0){
      rt_memcpy(to_buf+check+1, tmp_ph, read_2nd);
      tmp_ph += read_2nd;
      _frontend._buf.count -= read_2nd;
    }
    if(_frontend._buf.count != 0){
      _frontend._buf.count = 0;
    }
  } else {
    if(_head < _tail){
      read_1st = (uint8_t*)_tail - (uint8_t*)_head;
      rt_memcpy(to_buf, tmp_ph, read_1st);
      tmp_ph = (uint8_t*)_tail;
      _frontend._buf.count -= read_1st;
    } else {
      read_1st = check + 1;
      rt_memcpy(to_buf, tmp_ph, read_1st);
      tmp_ph = _frontend._buf.w_buf;
      _frontend._buf.count -= read_1st;
      read_2nd = (uint8_t*)_tail - tmp_ph;
      if(read_2nd > 0){
        rt_memcpy(to_buf+check+1, tmp_ph, read_2nd);
        tmp_ph += read_2nd;
        _frontend._buf.count -= read_2nd;
      }
    }
  }
  _head = tmp_ph;
  
  return read_1st + read_2nd;
}
