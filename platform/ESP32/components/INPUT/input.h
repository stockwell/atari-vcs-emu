#ifndef INPUT_H
#define INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

void InputTaskInit(void);
bool InputTaskGetState(uint8_t inputNum);

#ifdef __cplusplus
}
#endif

#endif //INPUT_H