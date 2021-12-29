#include "../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"

#ifdef HAL_ENABLE_FREERTOS
#include "FreeRTOS.h"

void * operator new( size_t size )
{
	return pvPortMalloc( size );
}

void * operator new[]( size_t size )
{
	return pvPortMalloc(size);
}

void operator delete( void * ptr )
{
	vPortFree ( ptr );
}

void operator delete[]( void * ptr )
{
	vPortFree ( ptr );
}
#endif
