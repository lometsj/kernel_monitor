typedef struct probe
{
	int is_use;
	char sym_name[64];
	unsigned long long address;
	void * entry;
	//private
	struct {
		struct kprobe *kp;
		//struct jprobe *jp;
	}private;
}probe;

typedef struct arg{
	unsigned long long arg0;
	unsigned long long arg1;
	unsigned long long arg2;
	unsigned long long arg3;
	unsigned long long arg4;
	unsigned long long arg5;
}arg;