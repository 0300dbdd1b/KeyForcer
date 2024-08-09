#include "platform.h"

int main() {
	#ifdef __BIP39_TESTS__
		BIP39_Tests();
	#endif /* ifdef __BIP39_TESTS__ */
}
