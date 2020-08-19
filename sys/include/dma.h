#ifndef __dma_h__
#define __dma_h__

#pragma  pack(push,1)
typedef struct dma_task_def
{
	char dma_ch;
	char dma_controller;
	char state;
	char dir;
	char * buffer;
	int bytes;
	int device;// floppy unit or ide controller.
}DMA_TASK;

#define DMA_ST_IDLE    0
#define DMA_ST_PENDING 1
#define DMA_ST_DONE    2

#define DMA_DIR_TO_MEM  0
#define DMA_DIR_FROM_MEM 1

bool run_dma_task(DMA_TASK* task);
__inline void set_dma_task(DMA_TASK* task,char * buffer,int bytes,char dir )
{
	task->buffer=buffer;
	task->bytes=bytes;
	task->dir=dir;
	task->state=DMA_ST_PENDING;
}
#pragma  pack(pop)
#endif  // __dma_h__