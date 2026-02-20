#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tensor.h"

// Tensör oluþturma fonksiyonu
Tensor* tensor_create(size_t size, TensorDataType type) {
    Tensor* t = (Tensor*)malloc(sizeof(Tensor));
    if (!t) return NULL;

    t->type = type;
    t->size = size;
    t->scale = 1.0f;
    t->zero_point = 0;

    switch (type) {
        case TENSOR_TYPE_FLOAT32:
            t->bytes = size * sizeof(float);
            t->data.f32_data = (float*)malloc(t->bytes);
            break;
        case TENSOR_TYPE_FLOAT16:
            t->bytes = size * sizeof(uint16_t);
            t->data.f16_data = (uint16_t*)malloc(t->bytes);
            break;
        case TENSOR_TYPE_INT8:
            t->bytes = size * sizeof(int8_t);
            t->data.i8_data = (int8_t*)malloc(t->bytes);
            break;
    }
    return t;
}

// Belleði serbest býrakma
void tensor_free(Tensor* t) {
    if (t) {
        if (t->data.f32_data) free(t->data.f32_data);
        free(t);
    }
}

// Asimetrik Niceleme
bool tensor_quantize_f32_to_i8(Tensor* src, Tensor* dest) {
    float min_val, max_val;
    size_t i; 
    //4.
    if (src->type != TENSOR_TYPE_FLOAT32 || dest->type != TENSOR_TYPE_INT8 || src->size != dest->size) {
        return false;
    }

    min_val = src->data.f32_data[0];
    max_val = src->data.f32_data[0];
    
    //döngü içi atama
    for (i = 1; i < src->size; i++) {
        if (src->data.f32_data[i] < min_val) min_val = src->data.f32_data[i];
        if (src->data.f32_data[i] > max_val) max_val = src->data.f32_data[i];
    }

    dest->scale = (max_val - min_val) / 255.0f;
    dest->zero_point = (int8_t)(-128 - round(min_val / dest->scale));

    for (i = 0; i < src->size; i++) {
        float q_float = round(src->data.f32_data[i] / dest->scale) + dest->zero_point;
        
        if (q_float > 127.0f) q_float = 127.0f;
        if (q_float < -128.0f) q_float = -128.0f;
        
        dest->data.i8_data[i] = (int8_t)q_float;
    }
    return true;
}

int main() {
    size_t tensor_size = 5;
    Tensor* t_float;
    Tensor* t_quantized;
    float sample_data[] = {-2.5f, -1.0f, 0.0f, 1.5f, 3.2f};
    size_t i; 

    printf("TinyML Bellek Yonetimi: Dinamik Tensor Demosu\n");
    printf("---------------------------------------------\n");
    
    t_float = tensor_create(tensor_size, TENSOR_TYPE_FLOAT32);
    for(i = 0; i < tensor_size; i++) {
        t_float->data.f32_data[i] = sample_data[i];
    }

    t_quantized = tensor_create(tensor_size, TENSOR_TYPE_INT8);
    tensor_quantize_f32_to_i8(t_float, t_quantized);

    printf("Bellek Tuketimi Karsilastirmasi:\n");
    printf("Float32 Tensor: %zu byte\n", t_float->bytes);
    printf("INT8 Tensor:    %zu byte (%%75 Tasarruf)\n\n", t_quantized->bytes);

    printf("Veri Donusumu (Quantization):\n");
    printf("Scale: %f, Zero Point: %d\n\n", t_quantized->scale, t_quantized->zero_point);
    
    printf("%-10s | %-10s\n", "Float32", "INT8");
    printf("-----------------------\n");
    for(i = 0; i < tensor_size; i++) {
        printf("%-10.2f | %-10d\n", t_float->data.f32_data[i], t_quantized->data.i8_data[i]);
    }

    tensor_free(t_float);
    tensor_free(t_quantized);
    return 0;
}
