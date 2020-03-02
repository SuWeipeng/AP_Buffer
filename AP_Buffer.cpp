#if defined(USE_RTTHREAD)
#include <entry.h>
#endif
#include <stdio.h>
#include "AP_Buffer.h"
#include "AP_Buffer_Ring.h"
#include "AP_Buffer_FIFO.h"

#if defined(USE_RTTHREAD)
using namespace rtthread;
#endif

static uint8_t write_buffer[AP_BUFFER_MAX_SIZE];
static uint8_t read_buffer[AP_BUFFER_MAX_SIZE];

AP_Buffer *AP_Buffer::_instance;

AP_Buffer::AP_Buffer()
: _backend(NULL)
, _type(NONE)
{
  _buf.w_buf = write_buffer;
  _buf.r_buf = read_buffer;
  _buf.count = 0;
  _instance = this;
}

void
AP_Buffer::init(buffer_type_t type)
{
  _type = type;
  switch(_type){
  case RING:{
    _backend = new AP_Buffer_Ring(*this, _buf.w_buf, _buf.r_buf);
    break;
  }
  case FIFO:{
    _backend = new AP_Buffer_FIFO(*this, _buf.w_buf, _buf.r_buf);
    break;
  }
  default:
    break;
  }
}

void
AP_Buffer::write(const void *pBuffer, uint16_t size)
{
  if(_backend != NULL){
    _backend -> write(pBuffer, size);
  }
}

uint16_t
AP_Buffer::read(void)
{
  uint16_t ret = 0;
  if(_backend != NULL){
    ret = _backend -> read();
  }
  return ret;
}

void*      
AP_Buffer::read_buf_addr(void)
{
  return read_buffer;
}

void*      
AP_Buffer::get_buffer(void)
{
  return write_buffer;
}
