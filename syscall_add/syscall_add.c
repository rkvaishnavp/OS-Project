#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/syscalls.h>
#include "syscall_add.h"

SYSCALLL_DEFINE2(syscall_add, unsigned int *,(unsigned int *)&num1, unsigned int *,(unsigned int *)&num2){
    int num1_bits[32];
    int num2_bits[32];
    for (int i = 0; i < 32; i++) {
        num1_bits[i] = (num1 >> i) & 1;
    }
    for (int i = 0; i < 32; i++) {
        num2_bits[i] = (num2 >> i) & 1;
    }

    if(num1_bits[31] == -1){
        return 0;
    }
    if(num2_bits[31] == -1){
        return 0;
    }

    int num1_exponent = (num1_bits >> 23) & 0xFF;
    int num2_exponent = (num2_bits >> 23) & 0xFF;

    int larger_exponent = (a_exponent > b_exponent) ? a_exponent : b_exponent;

    while (a_exponent < larger_exponent) {
        a_mantissa >>= 1;
        a_exponent++;
    }
    while (b_exponent < larger_exponent) {
        b_mantissa >>= 1;
        b_exponent++;
    }

}