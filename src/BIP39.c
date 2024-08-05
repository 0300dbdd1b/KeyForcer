#include "platform.h"

// Function to generate random entropy
ErrorCode BIP39_GenerateEntropy(unsigned char *entropy, int bits) {
    if (bits <= BIP39_MAX_ENTROPY_SIZE && bits % 32 == 0) {
        RAND_bytes(entropy, bits / 8);
		return SUCCESS;
    }
	else
	{
		return ERROR_BIP39_INVALID_SIZE;
	}
}

// Function to compute the checksum from the entropy
ErrorCode BIP39_ComputeChecksum(const unsigned char *entropy, int entropyBits, unsigned char *checksum) 
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(entropy, entropyBits / 8, hash);
    int checksumBits = entropyBits / 32;
    int fullBytes = checksumBits / 8;
    int remainingBits = checksumBits % 8;

    memcpy(checksum, hash, fullBytes);
    if (remainingBits > 0) {
        checksum[fullBytes] = hash[fullBytes] & (0xFF << (8 - remainingBits));
    }
	return SUCCESS;
}

ErrorCode BIP39_EntropyToMnemonic(const unsigned char *entropy, int bits, char *mnemonic) {
    int checksumBits = bits / 32;
    int totalBits = bits + checksumBits;
    int entropyBytes = bits / 8;
    int checksumBytes = (checksumBits + 7) / 8; // Round up to the nearest byte

    unsigned char checksum[checksumBytes];
    BIP39_ComputeChecksum(entropy, bits, checksum);

    // Combine entropy and checksum into a single buffer
    unsigned char entropyWithChecksum[entropyBytes + checksumBytes];
    memcpy(entropyWithChecksum, entropy, entropyBytes);

    if (checksumBytes > 0) {
        memcpy(entropyWithChecksum + entropyBytes, checksum, checksumBytes);
    }

    int words = totalBits / 11;
    mnemonic[0] = '\0';
	if (words > BIP39_MAX_WORDS) return ERROR_BIP39_LIMITS_BYPASSED;

    for (int i = 0; i < words; i++)
	{
        int index = 0;
        for (int j = 0; j < 11; j++)
		{
            int bitIndex = i * 11 + j;
            int byteIndex = bitIndex / 8;
            int bitInByte = bitIndex % 8;
            index <<= 1;
            index |= (entropyWithChecksum[byteIndex] >> (7 - bitInByte)) & 1;
        }
        strcat(mnemonic, wordlist[index]);
        if (i < words - 1) strcat(mnemonic, " ");
    }
	return SUCCESS;
}

ErrorCode BIP39_MnemonicToEntropy(const char *mnemonic, unsigned char *entropy, int *bits) {
    char *words[BIP39_MAX_WORDS];
    int word_count = 0;
    char *token = strtok((char *)mnemonic, " ");
    while (token != NULL) {
        words[word_count++] = token;
        token = strtok(NULL, " ");
    }

    if (word_count % 3 != 0) {
        return ERROR_BIP39_INVALID_MNEMONIC;
    }

    int totalBits = word_count * 11;
    int entropyBits = (totalBits * 32) / 33;
    int checksumBits = totalBits - entropyBits;
    int entropyBytes = entropyBits / 8;

    unsigned char entropyWithChecksum[entropyBytes + 1];
    memset(entropyWithChecksum, 0, sizeof(entropyWithChecksum));

    for (int i = 0; i < word_count; i++) {
        int index = 0;
        for (int j = 0; j < BIP39_DICT_WORD_COUNT; j++) {
            if (strcmp(words[i], wordlist[j]) == 0) {
                index = j;
                break;
            }
        }

        for (int bit = 0; bit < 11; bit++) {
            int bitIndex = i * 11 + bit;
            int byteIndex = bitIndex / 8;
            int bitInByte = bitIndex % 8;
            entropyWithChecksum[byteIndex] <<= 1;
            entropyWithChecksum[byteIndex] |= (index >> (10 - bit)) & 1;
        }
    }

    memcpy(entropy, entropyWithChecksum, entropyBytes);
    *bits = entropyBits;

    return SUCCESS;
}
