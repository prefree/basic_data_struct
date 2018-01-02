#ifndef _BITOPS_H_
#define _BITOPS_H_

#define BITS_PER_LONG       32
#define BIT_MASK(nr)        (1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)        ((nr) / BITS_PER_LONG)

static void set_bit(int nr, unsigned long *addr)
{
  unsigned long mask = BIT_MASK(nr);
  unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

  *p |= mask;
}
static void clear_bit(int nr, unsigned long *addr)
{
  unsigned long mask = BIT_MASK(nr);
  unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

  *p &= ~mask;
}
static void change_bit(int nr, unsigned long *addr)
{
  unsigned long mask = BIT_MASK(nr);
  unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

  *p ^= mask;
}

#endif/*_BITOPS_H_*/
