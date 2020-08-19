#include "cpu.h"

#pragma warning (disable:4018)

void CCPU::enter_ (int size,int nesting_level, int OperandSize) 
{
	assert(OperandSize==16||OperandSize==32);
	int StackAddrSize =BIG_SEGMENT(ss_desc)?32:16;

	nesting_level = nesting_level % 32;

	unsigned long frame_ptr, frame_pos; 
	if( StackAddrSize == 16) 
	{
		Push(__BP__,StackAddrSize) ; 
		frame_ptr= __SP__;
	}
	else 
	{
		Push(__EBP__,StackAddrSize) ; 
		frame_ptr= __ESP__;
	}

   /* Save stack pointer */


	if( nesting_level > 0)
	{

	/* nesting_level is rightmost parameter */
	   for(int  i = 1;i<nesting_level - 1;i++)
	   {
		   if( OperandSize == 16)
		   {
			   if( StackAddrSize == 16) 
			   {
				   __BP__ = __BP__ - 2;
				// push [BP] */
				if( !read_mem_raw(SS,0,BASE(ss_desc)+__BP__,frame_pos,OperandSize,StackAddrSize)) 
					return;

				Push (frame_pos,OperandSize) ;
			   }
			   else
			   {
				   __EBP__ = __EBP__ - 2;
				// push [EBP] */
				if( !read_mem_raw(SS,0,BASE(ss_desc)+__EBP__,frame_pos,OperandSize,StackAddrSize)) 
					return;

				Push (frame_pos,OperandSize) ;

			   }
		   }
		   else/* OperandSize = 32 */
		   {
			   if( StackAddrSize == 16) 
			   {
				   __BP__ = __BP__ - 4;
				// push [BP] */
				if( !read_mem_raw(SS,0,BASE(ss_desc)+__BP__,frame_pos,OperandSize,StackAddrSize)) 
					return;

				Push (frame_pos,OperandSize) ;
			   }
			   else
			   {
				   __EBP__ = __EBP__ - 4;
				// push [EBP] */
				if( !read_mem_raw(SS,0,BASE(ss_desc)+__EBP__,frame_pos,OperandSize,StackAddrSize)) 
					return;

				Push (frame_pos,OperandSize) ;

			   }
		   }
	   }
		//IF OperandSize  32
		//THEN 
		//Push(FrameTemp); (* doubleword push *)
		//ELSE (* OperandSize  16 *)
		//Push(FrameTemp); (* word push *)
		//FI;
		Push (frame_ptr,OperandSize) ;
	}


	if( StackAddrSize == 16) 
	{
		__BP__ = (unsigned short )frame_ptr;
		__SP__ = __BP__ -(unsigned short)size;/* First operand;*/
	}
	else
	{
		__EBP__ = frame_ptr;
		__ESP__ = __EBP__ - (unsigned short)size;/*ZeroExtend(First operand)*/
	}
}


/*!
     \brief:
      LEAVE - Restore Stack for Procedure Exit
 */
void CCPU::leave_(int OperandSize )
{
	assert(OperandSize==16||OperandSize==32);
	int StackAddrSize =BIG_SEGMENT(ss_desc)?32:16;

	if (StackAddrSize  ==  16)
		__SP__ = __BP__;
	else /* StackAddrSize  ==  32 */
   		__ESP__ = __EBP__;

	if (OperandSize  ==  16)
		__BP__ = Popw();
	else /* OperandSize  ==  32 */
   		__EBP__ = Popl();
}