#include <iostream>
#include <sigsegv.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <cstring>
using namespace std;

#define ADDR ((void*)0x0000'0001'0000'0000)

int sigsegvhandler(void *addr, int serious)
{
	cout << "sigsegvhandler got called with " << addr << "!" << endl;
	mprotect(addr, 1024, PROT_READ | PROT_WRITE);
	return 1; // success
}

int main()
{
	cout << "sizeof(void*) = " << sizeof(void*) << endl;
	
	if (sigsegv_install_handler(sigsegvhandler)) {
		cout << "could not install sigsegv handler" << endl;
		_exit(-1);
	}
	
	void *addr = mmap(ADDR, 1024, PROT_READ,
		MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (addr == MAP_FAILED) {
		cout << "could not mmap. errno = " << errno
			<< " (" << strerror(errno) << ")." << endl;
		_exit(-1);
	}
	cout << "mmap returned: " << addr << endl;
	
	cout << "before: " << *(int*)addr << endl;
	*(int*)addr = 123; // should cause a segv.
	cout << "after: " << *(int*)addr << endl;
	
	return 0;
}
