#include <iostream>
#include <sigsegv.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <cstring>
using namespace std;

#define ADDR ((void*)0x0000010000000000)

int sigsegvhandler(void *addr, int serious)
{
	cout << "sigsegvhandler got called with " << addr << "!" << endl;
	return 1; // success
}

int main()
{
	cout << "sizeof(void*) = " << sizeof(void*) << endl;
	
	if (sigsegv_install_handler(sigsegvhandler)) {
		cout << "could not install sigsegv handler" << endl;
		_exit(-1);
	}
	
	void *addr = mmap(ADDR, 10, PROT_READ | PROT_WRITE,
		MAP_SHARED | MAP_ANONYMOUS, 0, 0);
	if (addr == MAP_FAILED) {
		cout << "could not mmap. errno = " << errno
			<< " (" << strerror(errno) << ")." << endl;
		_exit(-1);
	}
	cout << "mmap returned: " << addr << endl;
	
	return 0;
}
