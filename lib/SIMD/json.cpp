#include <emscripten/emscripten.h>
#include <wasm_simd128.h>
#include <cstdint>
#include <cstring>

extern "C" {

EMSCRIPTEN_KEEPALIVE
__attribute__((hot))
int32_t parse(const char* input, int32_t length, int32_t* output) {
    int32_t count = 0;
    const v128_t vec_t = wasm_i8x16_splat('t');
    const v128_t vec_f = wasm_i8x16_splat('f');

    for (int32_t i = 0; i <= length - 16; i += 16) {
        v128_t chunk = wasm_v128_load(input + i);
        uint16_t masks[2] = {
            static_cast<uint16_t>(wasm_i8x16_bitmask(wasm_i8x16_eq(chunk, vec_t))),
            static_cast<uint16_t>(wasm_i8x16_bitmask(wasm_i8x16_eq(chunk, vec_f)))
        };

        for (int j = 0; j < 2; ++j) {
            const char* pattern = j ? "false" : "true";
            uint16_t mask = masks[j];

            while (mask) {
                int idx = __builtin_ctz(mask);
                int pos = i + idx;
                if (pos + (j ? 4 : 3) < length && !memcmp(input + pos, pattern, j ? 5 : 4)) {
                    const char* key_end = input + pos;
                    while (key_end > input && *key_end != '"') key_end--;
                    const char* key_start = key_end - 1;
                    while (key_start > input && *key_start != '"') key_start--;
                    if (*key_end == '"' && *key_start == '"') {
                        int32_t key = 0;
                        for (const char* k = ++key_start; k < key_end && *k >= '0' && *k <= '9'; ++k)
                            key = key * 10 + (*k - '0');
                        if (key >= 0) {
                            output[count * 2] = key;
                            output[count * 2 + 1] = !j;
                            count++;
                        }
                    }
                }
                mask &= mask - 1;
            }
        }
    }

    for (int32_t i = length & ~15; i < length; i++) {
        const char* pattern = input[i] == 't' ? "true" : (input[i] == 'f' ? "false" : nullptr);
        if (pattern && i + strlen(pattern) - 1 < length && !memcmp(input + i, pattern, strlen(pattern))) {
            const char* key_end = input + i;
            while (key_end > input && *key_end != '"') key_end--;
            const char* key_start = key_end - 1;
            while (key_start > input && *key_start != '"') key_start--;
            if (*key_end == '"' && *key_start == '"') {
                int32_t key = 0;
                for (const char* k = ++key_start; k < key_end && *k >= '0' && *k <= '9'; ++k)
                    key = key * 10 + (*k - '0');
                if (key >= 0) {
                    output[count * 2] = key;
                    output[count * 2 + 1] = *pattern == 't';
                    count++;
                }
            }
        }
    }

    return count;
}
}
