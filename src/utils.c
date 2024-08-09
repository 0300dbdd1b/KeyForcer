
char *bytes_to_hexstr(const unsigned char *bytes, size_t len) {
    // Allocate memory for the hex string (2 chars per byte + 1 for the null terminator)
    char *hexstr = (char *)malloc((len * 2) + 1);
    if (!hexstr) {
        return NULL; // Memory allocation failed
    }

    // Iterate over each byte and convert to hex
    for (size_t i = 0; i < len; ++i) {
        // Convert each byte to two hexadecimal characters
        sprintf(hexstr + (i * 2), "%02x", bytes[i]);
    }

    // Null-terminate the hex string
    hexstr[len * 2] = '\0';

    return hexstr;
}
