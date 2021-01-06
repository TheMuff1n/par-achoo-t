#ifndef TEST_UTILS_H_
#define TEST_UTILS_H_

#include <avr/io.h>

void long_to_str(int32_t i, uint8_t *str);
void int_to_str(int16_t i, uint8_t *str);

#ifdef DEBUG
#define BT_DEBUG(val, dchar) bt_debug(val, dchar)
#else
#define BT_DEBUG(val, dchar)
#endif

#endif /* UTILS_H_ */
