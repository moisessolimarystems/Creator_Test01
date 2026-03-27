#ifndef __InProcPtr_H
#define __InProcPtr_H

typedef long InProcPtr;

#define AccessInProcPtr(type,ptr) ((type)(void*)ptr)
#define CreateInProcPtr(ptr) ((InProcPtr)(void*)ptr)

#endif
