#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef union {
    float f;
    uint32_t u;
} float_bits;

float_bits add_float_bits(float_bits a, float_bits b) {
    // Extract sign, exponent and mantissa from the binary representation of the floats
    int sign_a = (a.u >> 31) & 1;
    int sign_b = (b.u >> 31) & 1;
    int exp_a = (a.u >> 23) & 0xFF;
    int exp_b = (b.u >> 23) & 0xFF;
    uint32_t mant_a = a.u & 0x7FFFFF;
    uint32_t mant_b = b.u & 0x7FFFFF;

    // Determine the larger and smaller exponents
    int larger_exp = exp_a > exp_b ? exp_a : exp_b;
    int smaller_exp = exp_a > exp_b ? exp_b : exp_a;

    // Align the mantissas by shifting them to the right
    int shift_amount = larger_exp - smaller_exp;
    if (shift_amount > 0) {
        mant_b >>= shift_amount;
    } else {
        mant_a >>= -shift_amount;
    }

    // Add the mantissas
    uint32_t result_mant = mant_a + mant_b;

    // Check if the result needs to be normalized
    if (result_mant & 0x800000) {
        result_mant >>= 1;
        larger_exp++;
    }

    // Build the result float from the binary representation
    float_bits result;
    result.u = (sign_a << 31) | (larger_exp << 23) | result_mant;

    // Copy the binary representation of the result back into a float variable
    float f_result;
    memcpy(&f_result, &result, sizeof(float));
    result.f = f_result;

    return result;
}

int main() {
    float_bits a, b, result;

    // Initialize the float_bits structs with the binary representation of the floats
    a.f = 1.5;
    b.f = 2.75;

    // Add the float_bits
    result = add_float_bits(a, b);

    // Print the result
    printf("%f + %f = %f\n", a.f, b.f, result.f);

    return 0;
}
