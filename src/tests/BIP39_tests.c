#ifdef __BIP39_TESTS__
#include "platform.h"



ErrorCode BIP39_Tests()
{
    int entropyBits = 128;
    unsigned char entropy[entropyBits / 8];
    char mnemonic[BIP39_MAX_WORDS * 10];
    unsigned char recoveredEntropy[entropyBits / 8];
    int recoveredBits;

    if (BIP39_GenerateEntropy(entropy, entropyBits) != SUCCESS) {
        fprintf(stderr, "Error generating entropy\n");
        return 1;
    }

    printf("Initial Entropy: ");
    for (int i = 0; i < entropyBits / 8; i++) {
        printf("%02x", entropy[i]);
    }
    printf("\n");

    if (BIP39_EntropyToMnemonic(entropy, entropyBits, mnemonic) != SUCCESS) {
        fprintf(stderr, "Error converting entropy to mnemonic\n");
        return 1;
    }

    printf("Generated Mnemonic: %s\n", mnemonic);

    if (BIP39_CheckMnemonicChecksum(mnemonic) != SUCCESS) {
        fprintf(stderr, "Checksum verification failed for the generated mnemonic\n");
        return 1;
    } else {
        printf("Checksum verification passed for the generated mnemonic\n");
    }

    if (BIP39_MnemonicToEntropy(mnemonic, recoveredEntropy, &recoveredBits) != SUCCESS) {
        fprintf(stderr, "Error converting mnemonic back to entropy\n");
        return 1;
    }

    printf("Recovered Entropy: ");
    for (int i = 0; i < recoveredBits / 8; i++) {
        printf("%02x", recoveredEntropy[i]);
    }
    printf("\n");

    if (memcmp(entropy, recoveredEntropy, entropyBits / 8) == 0) {
        printf("Entropy verification: Passed\n");
    } else {
        printf("Entropy verification: Failed\n");
    }

    char *invalidMnemonic = "invalid mnemonic phrase that does not exist";
    if (BIP39_CheckMnemonicChecksum(invalidMnemonic) != SUCCESS) {
        printf("Checksum verification correctly failed for invalid mnemonic\n");
    } else {
        printf("Checksum verification incorrectly passed for invalid mnemonic\n");
    }

    return 0;
}

#endif
