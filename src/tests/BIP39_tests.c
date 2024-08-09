#ifdef __BIP39_TESTS__
#include "platform.h"

// Known mnemonics with corresponding seeds and checksum expectations
const struct {
	int entropyBits;
	const char* mnemonic;
	const char* seed;
	int checksumValidity; // SUCCESS for valid, non-SUCCESS for invalid
} known_tests[] = {
	{128, "crystal illegal yard make trouble sea traffic smooth flip remain gain shield", "92edbdf520174c0330c563f99b0936a6ce2c4c8f46ca26f2fc35bab6d692e590cfa2fbafde77b50cfba7d038d7bf91da2c4386d209ec14f34b83cc5466c3a190", SUCCESS},
	{128, "floor exclude main dad lonely skin tilt multiply glue lyrics what autumn", "cbeb4746bcea3de8e28c3e8fb5de3e52c25944c18aa98e75fcae9433ec75752bfad70270e910ac67f79fce07719aad554b161654c03350b8f1a5b542a50e7cdb", SUCCESS},
	// Add more known test cases
};

ErrorCode BIP39_Tests() {
	const int testEntropyBits[] = {32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 480, 512};
	const int numTests = (int)(sizeof(testEntropyBits) / sizeof(testEntropyBits[0]));
	for (int i = 0; i < BIP39_TESTCOUNT; i++){
		for (int test = 0; test < numTests; test++) {
			printf("%d\n", BIP39_MAX_WORDS);
			int entropyBits = testEntropyBits[test];
			unsigned char entropy[entropyBits / 8];
			char mnemonic[BIP39_MAX_WORDS * 10];
			unsigned char recoveredEntropy[entropyBits / 8];
			int recoveredBits;

			// Generate entropy
			if (BIP39_GenerateEntropy(entropy, entropyBits) != SUCCESS) {
				fprintf(stderr, "Error generating entropy for %d bits\n", entropyBits);
				return 1;
			}

			// Convert entropy to mnemonic
			if (BIP39_EntropyToMnemonic(entropy, entropyBits, mnemonic) != SUCCESS) {
				fprintf(stderr, "Error converting entropy to mnemonic\n");
				return 1;
			}

			// Check mnemonic checksum
			if (BIP39_CheckMnemonicChecksum(mnemonic) != SUCCESS) {
				fprintf(stderr, "Checksum verification failed for the generated mnemonic\n");
				return 1;
			}

			// Convert mnemonic back to entropy
			if (BIP39_MnemonicToEntropy(mnemonic, recoveredEntropy, &recoveredBits) != SUCCESS) {
				fprintf(stderr, "Error converting mnemonic back to entropy\n");
				return 1;
			}

			// Verify the recovered entropy matches the original
			if (memcmp(entropy, recoveredEntropy, entropyBits / 8) != 0) {
				fprintf(stderr, "Entropy verification: Failed\n");
				return 1;
			}
		}
	}

	// Test against known mnemonics, seeds, and checksums
	for (int i = 0; i < (int)(sizeof(known_tests) / sizeof(known_tests[0])); i++) {
		if (BIP39_CheckMnemonicChecksum(known_tests[i].mnemonic) != known_tests[i].checksumValidity) {
			fprintf(stderr, "Checksum test failed for mnemonic: %s\n", known_tests[i].mnemonic);
			return 1;
		}

		if (known_tests[i].checksumValidity == SUCCESS) { // Only test seed generation if checksum is valid
			unsigned char seed[64]; // Adjust size based on your seed size requirements
			const char* passphrase = ""; // Assume an empty passphrase for simplicity
			if (BIP39_MnemonicToSeed(known_tests[i].mnemonic, passphrase, seed) != SUCCESS) {
				fprintf(stderr, "Failed to generate seed for mnemonic: %s\n", known_tests[i].mnemonic);
				return 1;
			}

			// Assuming direct comparison (you'll need to implement actual seed comparison logic)
			if (strcmp((char*)seed, known_tests[i].seed) != 0) {
				fprintf(stderr, "Seed verification failed for mnemonic: %s\n", known_tests[i].mnemonic);
				return 1;
			}
		}
	}

	return 0;
}

#endif
