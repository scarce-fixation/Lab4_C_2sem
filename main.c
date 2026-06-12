#include <stdio.h>
int compress(char* input, char* output) {
    FILE* in = fopen(input, "rb");
    if (!in) return 1;
    FILE* out = fopen(output, "wb");
    if (!out) {
        fclose(in);
        return 1;
    }
    unsigned char inByte;
    unsigned char buffer = 0;
    int bitCount = 0;
    unsigned char outByte;
    while (fread(&inByte, 1, 1, in) == 1) {
        if (bitCount <= 0) {
            buffer = (inByte & 0x7F);
            bitCount += 7;
        }
        else
        {
            unsigned char bit = buffer & 1;
            buffer >>= 1;
            bit <<= 7;
            outByte = inByte | bit;
            fwrite(&outByte, 1, 1, out);
            bitCount--;
        }
    }
    if (bitCount > 0) {
        outByte = buffer << (7-bitCount);
        fwrite(&outByte, 1, 1, out);
    }
    fclose(in);
    fclose(out);
    return 0;
}
int decompress(char* input, char* output) {
    FILE* in = fopen(input, "rb");
    if (!in) return 1;
    FILE* out = fopen(output, "wb");
    if (!out) {
        fclose(in);
        return 1;
    }
    unsigned char inByte;
    unsigned char el[8] = { 0 };
    int i = 1;
    while (fread(&inByte, 1, 1, in) == 1) {
        el[0] |= (inByte >> 7) << (i - 1);
        el[i++] = inByte & 0x7F;

        if (i == 8) {
            int j = 0;
            while (i > 0) {
                fwrite(&el[j], 1, 1, out);
                el[j] = 0;
                j++;
                i--;
            }
            i = 1;
        }
    }
    if (i > 0) {
        el[0] = el[--i] | el[0];
        int j = 0;
        while (i > 0) {
            fwrite(&el[j], 1, 1, out);
            el[j] = 0;
            j++;
            i--;
        }
    }
    fclose(in);
    fclose(out);
    return 0;
}
int main()
{
    printf("Hello World!\n");
    return 0;
}
