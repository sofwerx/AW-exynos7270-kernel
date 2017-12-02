#ifndef __PRELOAD_HANDLERS_THREADS_H__
#define __PRELOAD_HANLDERS_THREADS_H__

struct task_struct;

unsigned long pt_get_flags(struct task_struct *task);
void pt_set_flags(struct task_struct *task, unsigned long flags);

int pt_set_data(struct task_struct *task, unsigned long caller,
		 unsigned char call_type, unsigned long disable_addr,
		 bool drop);
int pt_get_caller(struct task_struct *task, unsigned long *caller);
int pt_get_call_type(struct task_struct *task, unsigned char *call_type);
int pt_get_drop(struct task_struct *task);
bool pt_check_disabled_probe(struct task_struct *task, unsigned long addr);
void pt_enable_probe(struct task_struct *task, unsigned long addr);
int pt_put_data(struct task_struct *task);
int pt_init(void);
void pt_exit(void);

#endif /* __PRELOAD_HANDLERS_THREADS_H__ */
