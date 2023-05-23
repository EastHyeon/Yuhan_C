#define SCREEN_MAX_X 30
#define SCREEN_MAX_Y 15
#define SCREEN_SIZE SCREEN_MAX_X * SCREEN_MAX_Y + SCREEN_MAX_Y

void clearBuffer(char* targetBuffer);
void writeStringToBuffer(const char* msg, char* targetBuffer, int x, int y);
void writeStringCenterToBuffer(const char* msg, char* targetBuffer, int y);