#include"IR_APP_Private.h"


int index2No(int value)
{
    return value + 1;
}
int No2index(int value)
{
    return value - 1;
}
bool isValidIndex(int index)
{
    return index >= 0 && index < MAX_CHANNEL_COUNT;
}

Session g_session[MAX_CHANNEL_COUNT] = {0};