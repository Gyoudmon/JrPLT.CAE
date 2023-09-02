#include <iostream>

// 场景：胡克定律

// 实验得出关系表达式：Δl = 0.16m
float Δl1(float m) {
    return 0.16 * m;
}

float Δl2(float m) {
    return 0.20 * m;
}

float Δl3(float m) {
    return 0.15 * m;
}

// 门槛不算高，但是极其折磨人
int main(int 参数数量, char* 参数小组[]) {
    printf("Hooke's Law: %s\n", 参数小组[0]);

    // 数学建模阶段    
    printf("delta l1: %f\n", Δl1(20.0F));
    printf("delta l2: %f\n", Δl2(40.0F));
    printf("delta l3: %f\n", Δl3(80.0F));

    return 0;
}
