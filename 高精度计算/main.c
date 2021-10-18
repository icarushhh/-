//
//  main.c
//  高精度计算
//
//  Created by 朱仕哲 on 2021/10/5.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stack.h"

#define n 200

typedef struct{
    int* int_part;
    int* frac_part;
    int length_of_frac;
    int k;                  //进制，不同则不能加减乘
    int sign;               //符号，1为正，-1为负
}R, *R_p;
//n位k进制有符号实数ADT

void initialize(R_p p){
    p->k = 10;
    p->sign = 1;
    p->length_of_frac = 0;
    p->int_part = (int*)malloc(11*sizeof(int));
    p->frac_part = (int*)malloc(n*sizeof(int));
    
    for (int i = 0; i < 11; i++) {
        p->int_part[i] = 0;
    }
    for (int i = 0; i < n; i++) {
        p->frac_part[i] = 0;
    }
    
    return;
}// 初始化R，给小数部分分配n位十进制空间并初始化为0

void input(R_p p){
    int int_stacksize = 0;
    int* int_stacktop_ptr;
    int* int_stackbase_ptr;
    stack_initialize(int);
    //initialize a stack
    
    int i = 0, j = 10;
    char c;
    int e = 0;
    
    printf("输入一个实数（-1024～1024）：");
    while ((c = getchar()) != '.' && c != '\n') {
        if (c == '-') {
            p->sign = -1;
            continue;
        }                       //处理符号
        push(int, c-48);
    }
    while (!stack_is_empty(int)) {
        pop(int, e);
        p->int_part[j--] = e;
    }
    
    if (c == '.') {
        while ((c = getchar()) != '\n') {
            p->frac_part[i++] = c-48;
        }
        p->length_of_frac = i;
    }
    else return;
    
    return;
}//输入一个十进制实数

void print(R p){
    int i = 0;
    
    if (p.sign == -1) {
        printf("-");
    }
    
    for ( ; i < 10; i++) {
        if (p.int_part[i] != 0) {
            break;
        }
    }//ignore 0s at the beginning
    
    for (; i < 11; i++) {
        printf("%d",p.int_part[i]);
    }
    
    printf(".");
    
    for (i = 0; i < p.length_of_frac; i++) {
        printf("%d", p.frac_part[i]);
    }
    if (p.length_of_frac == 0) {
        printf("0");
    }
    printf("\n");
    return;
}//print 一个实数

void ten_to_k(R source, R_p target){
    int j = 10;
    int i = 0;
    int int_input = 0;
    double frac_input = 0;
    int k = 0;
    
    printf("input k(0<k<10): ");
    scanf("%d", &k);
    if (k < 1 || k > 9) {
        printf("k超出范围\n");
        return;
    }
    target->k = k;//改成k进制
    target->sign = source.sign;
    
    for (int s = 10, i = 0; s >= 0; s--, i++) {
        int_input += source.int_part[s] * pow(10, i);
    }
    
    while (int_input > 0) {
        target->int_part[j--] = int_input % k;
        int_input /= k;
    }//int part
    
    
    for (int i = 0; i < source.length_of_frac; i++) {
        frac_input += source.frac_part[i] * pow(10, -1*(i+1));
    }
    
    i = 0;
    while (frac_input > 0 && i < n) {
        frac_input *= k;
        target->frac_part[i++] = (int)(frac_input);
        frac_input -= (int)(frac_input);
    }
    
    target->length_of_frac = i;
    //frac part
    
    return;

}//将一个十进制实数转化成k进制(0<k<10)

void sub(R source1, R source2, R_p target);
void add(R source1, R source2, R_p target){
    if (source1.k != source2.k) {
        printf("两实数进制不同，无法执行运算\n");
        return;
    }
    target->k = source2.k;
    //处理进制问题
    
    if (source1.sign != source2.sign) {
        source2.sign *= -1;
        sub(source1, source2, target);
        return;
    }
    target->sign = source1.sign;
    //处理符号问题
    
    int maxlen = (source1.length_of_frac > source2.length_of_frac) ? source1.length_of_frac : source2.length_of_frac;
    int carry = 0;
    int temp = 0;
    int k = target->k;
    
    target->length_of_frac = maxlen;
    
    for (int j = maxlen - 1; j >= 0; j--) {
        temp = source1.frac_part[j] + source2.frac_part[j] + carry;
        target->frac_part[j] = temp % k;
        carry = temp - temp % k;
    }
    
    for (int j = 10; j >= 0; j--) {
        temp = source1.int_part[j] + source2.int_part[j] + carry;
        target->int_part[j] = temp % k;
        carry = temp - temp % k;
    }
    return;
}

int compare_abs(R r1, R r2){
    if (r1.k != r2.k) {
        printf("两实数进制不同，无法比较\n");
        return -2;
    }
    
    for (int i = 0; i < 11; i++) {
        if (r1.int_part[i] != r2.int_part[i]) {
            return (r1.int_part[i] > r2.int_part[i]) ? 1 : -1;
        }
    }
    
    int maxlen = (r1.length_of_frac > r2.length_of_frac) ? r1.length_of_frac : r2.length_of_frac;
    
    for (int i = 0; i < maxlen; i++) {
        if (r1.frac_part[i] != r2.frac_part[i]) {
            return (r1.frac_part[i] > r2.frac_part[i]) ? 1 : -1;
        }
    }
    
    return 0;
}//比较r1与r2的绝对值大小

void sub(R source1, R source2, R_p target){
    if (source1.k != source2.k) {
        printf("两实数进制不同，无法执行运算\n");
        return;
    }
    target->k = source2.k;
    //处理进制问题
    
    int flag = compare_abs(source1, source2);
    
    if (source1.sign != source2.sign) {
        source2.sign *= -1;
        add(source1, source2, target);
        return;
    }
    
    if (flag == 1) {
        target->sign *= source1.sign;
    }
    if (flag == 0) {
        return;
    }
    if (flag == -1) {
        target->sign *= -1;
        sub(source2, source1, target);
        return;
    }
    //处理符号问题
    //此时有source1 > source2
    
    int maxlen = (source1.length_of_frac > source2.length_of_frac) ? source1.length_of_frac : source2.length_of_frac;
    int carry = 0;
    int temp = 0;
    int k = target->k;
    target->length_of_frac = maxlen;
    
    for (int j = maxlen - 1; j >= 0; j--) {
        temp = source1.frac_part[j] - source2.frac_part[j] + carry;
        if (temp >= 0) {
            target->frac_part[j] = temp;
            carry = 0;
        }
        else{
            target->frac_part[j] = temp + k;
            carry = -1;
        }
    }
    
    for (int j = 10; j >= 0; j--) {
        temp = source1.int_part[j] - source2.int_part[j] + carry;
        if (temp >= 0) {
            target->int_part[j] = temp;
            carry = 0;
        }
        else{
            target->int_part[j] = temp + k;
            carry = -1;
        }
    }
    
    return;
}

void multiply(R source1, R source2, R_p target){
    if (source1.k != source2.k) {
        printf("两实数进制不同，无法执行运算\n");
        return;
    }
    target->k = source2.k;
    //处理进制问题
    
    target->sign = source1.sign * source2.sign;  //处理符号问题
    target->length_of_frac = source1.length_of_frac + source2.length_of_frac;
     //处理位数
    
    if (target->length_of_frac > n) {
        target->length_of_frac = n;
    }
    
    for (int i = source1.length_of_frac - 1; i >= 0; i--) {
        for (int j = source2.length_of_frac - 1; j >= 0; j--) {
            if (i+1 + j+1 > n) {
                continue;
            }//超出精度,ignore
            
            target->frac_part[(i+1)+(j+1)-1] += source1.frac_part[i] * source2.frac_part[j];
        }
    }//frac1 * frac2
    
    for (int i = 10; i >= 0; i--) {
        for (int j = source2.length_of_frac - 1; j >= 0; j--) {
            if (i+j-10 < 0) {
                target->int_part[i+j+1] += source1.int_part[i] * source2.frac_part[j];
            }
            else target->frac_part[(j+1)-(10-i)-1] += source1.int_part[i] * source2.frac_part[j];
        }
    }//int1 * frac2
    
    for (int i = source1.length_of_frac - 1; i >= 0; i--) {
        for (int j = 10; j >= 0; j--) {
            if ((i+1)-(10-j)-1 < 0) {
                target->int_part[i+j+1] += source1.frac_part[i] * source2.int_part[j];
            }
            else target->frac_part[(i+1)-(10-j)-1] += source1.frac_part[i] * source2.int_part[j];
        }
    }//frac1 * int2
    
    int k = target->k;
    int carry = 0;
    int temp = 0;
    
    for (int j = target->length_of_frac - 1; j >= 0; j--) {
        temp = target->frac_part[j] + carry;
        carry = temp / k;
        target->frac_part[j] = temp % k;
    }
    target->int_part[10] += carry;
    carry = 0;
    //处理进位
        
    for (int i = 1; i <= 11; i++) {
        for (int j = 1; j <= 11; j++) {
            if (i+j > 11) {
                continue;
            }//overflow
            
        target->int_part[11-i-j+1] += source1.int_part[11-i] * source2.int_part[11-j];
        }
    }//int1 * int2
    
    for (int j = 10; j >= 0; j--) {
        temp = target->int_part[j] + carry;
        carry = temp / k;
        target->int_part[j] = temp % k;
    }
    if (carry) {
        printf("overflow\n");
    }
    //处理进位
    return;
}

void fx(R r1, R_p r2){
    R temp[10];
    for (int i = 0; i < 10; i++) {
        initialize(temp + i);
    }
    
    R consts[5];
    for (int i = 0; i < 5; i++) {
        initialize(consts + i);
    }
    
    freopen("/Users/icarus/Library/Mobile Documents/com~apple~CloudDocs/2021秋季学期/数据结构/上机题2/High-precision-real-number-operation/高精度计算/data.txt", "r", stdin);
    int int_stacksize = 0;
    int* int_stacktop_ptr;
    int* int_stackbase_ptr;
    stack_initialize(int);
    //initialize a stack
    
    int s = 0, j = 10;
    char c;
    int e = 0;

    while ((c = getchar()) != '.' && c != '\n') {
        if (c == '-') {
            (consts+0)->sign = -1;
            continue;
        }                       //处理符号
        push(int, c-48);
    }
    while (!stack_is_empty(int)) {
        pop(int, e);
        (consts+0)->int_part[j--] = e;
    }
    
    if (c == '.') {
        while ((c = getchar()) != '\n') {
            (consts+0)->frac_part[s++] = c-48;
        }
        (consts+0)->length_of_frac = s;
    }
    
    fclose(stdin);  //important!!!!
    
    consts[1].int_part[10] = 3;
    for (int m = 0; m < 10; m++) {
        (consts+2)->frac_part[m] = 3;
        consts[2].length_of_frac = 10;
    }
    consts[3].int_part[10] = 2;
    consts[4].int_part[10] = 4;
    
    multiply(r1, r1, temp+0);
    multiply(temp[0], r1, temp+1);
    multiply(temp[1], consts[0], temp+2);
    
    multiply(temp[0], consts[2], temp+3);
    
    sub(temp[2], temp[3], temp+4);
    
    add(temp[4], consts[4], r2);

    return;
} //计算预设函数f（r1）

void fy(R r1, R_p r2){
    printf("输入五个参数，运算c1*x^c2-c3*x^2+4\n");
    
    R temp[5];
    for (int i = 0; i < 5; i++) {
        initialize(temp + i);
    }
    
    R consts[5];
    for (int i = 0; i < 5; i++) {
        initialize(consts + i);
    }
    
    for (int i = 0; i < 5; i++) {
        printf("输入实数c%d\n", i+1);
        input(consts+i);
    }
    
    multiply(r1, r1, temp+0);
    multiply(temp[0], r1, temp+1);
    multiply(temp[1], consts[0], temp+2);
    
    multiply(temp[0], consts[2], temp+3);
    
    sub(temp[2], temp[3], temp+4);
    
    add(temp[4], consts[4], r2);

    return;
}//输入参数计算f


int main() {
   
    R r1,r2,r3;
    initialize(&r1);
    initialize(&r2);
    initialize(&r3);
    input(&r1);
    input(&r2);
    //input(&r3);
    
    //ten_to_k(r1, &r3);
    //add(r1, r2, &r3);
    //sub(r1, r2, &r3);
    //multiply(r1, r2, &r3);
    //fx(r1, &r3);
    //fy(r1, &r3);
    
    print(r3);
    
    return 0;
}
