#include <emscripten/emscripten.h>
#include <wasm_simd128.h>
#include <cstdint>
#include <cstring>

extern "C" {

EMSCRIPTEN_KEEPALIVE
int32_t parse(const char* input, int32_t length, bool* output) {
    int32_t count = 0;
    // little endian t/f
    const v128_t vec_T = wasm_i32x4_splat(0x65757274);  
    const v128_t vec_F = wasm_i32x4_splat(0x736C6166); 

    for (int32_t i = 0; i < length; i += 16) {
        v128_t chunk = wasm_v128_load(input + i);
        v128_t mask_T = wasm_i32x4_eq(chunk, vec_T);
        v128_t mask_F = wasm_i32x4_eq(chunk, vec_F);
        uint32_t bits_T = wasm_i32x4_bitmask(mask_T);
        uint32_t bits_F = wasm_i32x4_bitmask(mask_F);

        while (bits_T) {
            int index = __builtin_ctz(bits_T);
            output[count++] = true;
            bits_T &= (bits_T - 1);
        }

        while (bits_F) {
            int index = __builtin_ctz(bits_T);
            output[count++] = false;
            bits_F &= (bits_F - 1);
        }
    }

    return count;
}

}
