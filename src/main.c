#include "platform.h"

int main()
{
    int entropyBits = 256; // 128, 160, 192, 224, or 256
    unsigned char entropy[entropyBits / 8];
    char mnemonic[BIP39_MAX_WORDS * 10]; // Allocate enough space for mnemonic

    // Generate entropy
    if (BIP39_GenerateEntropy(entropy, entropyBits) != SUCCESS) {
        fprintf(stderr, "Error generating entropy\n");
        return 0;
    }

    // Print initial entropy
    printf("Initial Entropy: ");
    for (int i = 0; i < entropyBits / 8; i++) {
        printf("%02x", entropy[i]);
    }
    printf("\n");

    // Generate mnemonic from entropy
    if (BIP39_EntropyToMnemonic(entropy, entropyBits, mnemonic) != SUCCESS) {
        fprintf(stderr, "Error converting entropy to mnemonic\n");
        return 0;
    }

    printf("Mnemonic: %s\n", mnemonic);

    // Convert mnemonic back to entropy
    unsigned char revertedEntropy[entropyBits / 8];
    int revertedBits;
    if (BIP39_MnemonicToEntropy(mnemonic, revertedEntropy, &revertedBits) != SUCCESS) {
        fprintf(stderr, "Error converting mnemonic back to entropy\n");
        return 0;
    }

    // Print reverted entropy
    printf("Reverted Entropy: ");
    for (int i = 0; i < revertedBits / 8; i++) {
        printf("%02x", revertedEntropy[i]);
    }
    printf("\n");

    return 0;
}
