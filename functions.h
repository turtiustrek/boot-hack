//Definitions for the functions in stubs.S

#define MEM(x) (*(int *)(x))

extern void uart_printf(const char *, ...);

extern void TaskEntry();
extern void loadkernel();
extern void runkernel(int entryaddr);

extern void assert(const char *crash);

extern void OSAL_CreateTask(const char *taskname, int (*task)(int), int noidea, int stacksize, int priority);
extern int OSAL_SeizeSemaphore(int addr, int noidea, int noidea1, const char *semaphore, int noidea2);
extern int OSAL_ReleaseSemaphore(int addr, const char *semaphore, int noidea);
extern void OSAL_YieldTask(int delay); //100 ~ 1 second

extern void VCCOnPower();
extern void VCCOffPower();
extern void writeI2C(int addr, int length, char *array);
extern void applyI2C();

extern int check_checksum();
