void* __stack_chk_guard;
void* __dso_handle;

void
__stack_chk_guard_setup ()
{
	unsigned char* p = (unsigned char*) &__stack_chk_guard;
	p[sizeof(size_t)-1] = 255;  /* <- this should be probably randomized */
	p[sizeof(size_t)-2] = '\n';
	p[0] = 0;
}

void
__attribute__((noreturn))
__stack_chk_fail ()
{
	fatal("stack broken");
}

void
__aeabi_unwind_cpp_pr0(void)
{
}

int raise (int sig) {

}
