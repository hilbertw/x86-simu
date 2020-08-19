		
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include "win-util.h"

int copy_file(LPCWSTR  dst,LPCWSTR  src)
{
	FILE * rfp,*wfp;
	int err;
	size_t count;
	char buffer[0x400];

	rfp=_wfopen(src,L"rb");
	if(!rfp) 
	{
		err=1;
		goto out;
	}

	wfp=_wfopen(dst,L"wb");
	if(!wfp) 
	{
		fclose(rfp);
		err=-1;
		goto out;
	}

	err=0;
	while((count=fread(buffer,1, sizeof(buffer),rfp))>0)
	{
		if(count>fwrite(buffer,1, sizeof(buffer),wfp))
		{
			err=-1;
			break;
		}
	}

	fclose(rfp);
	fclose(wfp);
out:
	return err;

}