#ifndef INTCAST_H
#define INTCAST_H

typedef union {
  uint16_t word;
  struct {
    uint8_t a,b;
  };
} uint16_t_u;

typedef union {
  uint32_t dword;
  struct {
    uint8_t a,b,c,d;
  };
} uint32_t_u;

typedef union {
  uint32_t dword;
  struct {
    uint16_t a,b;
  };
} uint32_t_s;

#define L8(v)    (((uint16_t_u)v).a)
#define H8(v)    (((uint16_t_u)v).b)

#define L16(v)   (((uint32_t_s)v).a)
#define H16(v)   (((uint32_t_s)v).b)

#define LL8(v)   (((uint32_t_u)v).a)
#define LH8(v)   (((uint32_t_u)v).b)
#define HL8(v)   (((uint32_t_u)v).c)
#define HH8(v)   (((uint32_t_u)v).d)


#endif
