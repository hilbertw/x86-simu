class CBus
{

char the_lock;


void get_lock(){
__asm 
{
l_wait:
lock bts the_lock,1;
jb l_wait;
}
}


void release_lock()
{
 the_lock=0;
}
