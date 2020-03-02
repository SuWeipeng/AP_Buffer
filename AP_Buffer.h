#ifndef __AP_BUFFER_H__
#define __AP_BUFFER_H__

#include <stdint.h>
#include <rtt_interface.h>

#ifndef AP_BUFFER_MAX_SIZE
#define AP_BUFFER_MAX_SIZE  (1024)
#endif
#define AP_BUFFER_MAX_INDEX (AP_BUFFER_MAX_SIZE - 1)

class AP_Buffer_Backend;

class AP_Buffer{
  friend class AP_Buffer_Backend;
  friend class AP_Buffer_Ring;
  friend class AP_Buffer_FIFO;
public:
  AP_Buffer();
  ~AP_Buffer(){};
  
  // get singleton
  static AP_Buffer *get_instance(void) {
    return _instance;
  }
  
  // buffer types
  typedef enum {
    RING,
    FIFO,
    NONE
  } buffer_type_t;
  
  void           init(buffer_type_t type);
  void           write(const void *pBuffer, uint16_t size);
  uint16_t       read(void);
  void*          read_buf_addr(void);
  uint16_t       buf_len(void) { return _buf.count; }
  void*          get_buffer();
  buffer_type_t  get_buf_type() { return _type; }
  
private:  
  typedef struct Buffer {
    uint8_t*  w_buf;
    uint8_t*  r_buf;
    uint16_t  count;
  } _ap_buffer;  
  
  static AP_Buffer*       _instance;
  AP_Buffer_Backend*      _backend;
  
  _ap_buffer    _buf;
  buffer_type_t _type;
};

#endif /* __AP_BUFFER_H__ */
