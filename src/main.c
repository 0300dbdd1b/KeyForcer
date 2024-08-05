#include "platform.h"

int main() {
    int entropyBits = 256;  // Usually 128, 160, 192, 224, or 256 bits
    unsigned char entropy[entropyBits / 8];
    char mnemonic[BIP39_MAX_WORDS * 10];
    unsigned char seed[BIP39_SEED_LEN];  // Seed length is 512 bits (64 bytes)

    if (BIP39_GenerateEntropy(entropy, entropyBits) != SUCCESS) {
        fprintf(stderr, "Error generating entropy\n");
        return 0;
    }

    printf("Initial Entropy: ");
    for (int i = 0; i < entropyBits / 8; i++) {
        printf("%02x", entropy[i]);
    }
    printf("\n");

    if (BIP39_EntropyToMnemonic(entropy, entropyBits, mnemonic) != SUCCESS) {
        fprintf(stderr, "Error converting entropy to mnemonic\n");
        return 0;
    }

    printf("Mnemonic: %s\n", mnemonic);

    // Check if the mnemonic is valid including its checksum
    if (BIP39_CheckMnemonicChecksum(mnemonic) != SUCCESS) {
        fprintf(stderr, "Error: Invalid mnemonic checksum\n");
        return 0;
    } else {
        printf("Checksum verification: Passed\n");
    }

    if (BIP39_MnemonicToSeed(mnemonic, "", seed) != SUCCESS) {
        fprintf(stderr, "Error converting mnemonic to seed\n");
        return 0;
    }

    printf("Seed: ");
    for (int i = 0; i < BIP39_SEED_LEN; i++) {
        printf("%02x", seed[i]);
    }
    printf("\n");

    return 0;
}

