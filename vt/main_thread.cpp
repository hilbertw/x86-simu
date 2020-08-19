static char ring[16];
static int tail,head;


bool sys_save_snapshot();
bool sys_load_snapshot();
void sys_invert(int what);

inline bool full()
{
	int next_to_tail =tail == 0 ? 15 : tail - 1;
	return (head == next_to_tail);
}
inline bool empty()
{
	return head == tail;
}

inline void append(char c)
{
	ring[head] = c;

	head++;
	head &= 0xf;

}

inline char get()
{
	char c = ring[tail];
	tail++;
	tail &= 0xf;

	return c;
}
void post_main_thread_msg(char msg)
{
	if (!full())
	{
		append(msg);
	}
}

void do_vt()
{
	while (!empty())
	{
		switch (get())
		{
		case 's':
			sys_save_snapshot();
			break;
		case 'l':
			sys_load_snapshot();
			break;
		case 't':
			sys_invert(1);
			break;
		case 'p':
			sys_invert(0);
			break;
		}
	}

}