#ifndef TENSOR_H
#define TENSOR_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Desteklenen veri tipleri
typedef enum {
    TENSOR_TYPE_FLOAT32,
    TENSOR_TYPE_FLOAT16, 
    TENSOR_TYPE_INT8     
} TensorDataType;

// Dinamik Tensor Yapýsý
typedef struct {
    TensorDataType type;
    size_t size;         
    size_t bytes;        
    
    // Bellek alanýný paylaþan union yapýsý
    union {
        float* f32_data;
        uint16_t* f16_data; 
        int8_t* i8_data;
    } data;

    
    float scale;
    int8_t zero_point;
} Tensor;


Tensor* tensor_create(size_t size, TensorDataType type);
void tensor_free(Tensor* t);
bool tensor_quantize_f32_to_i8(Tensor* src, Tensor* dest);

#endif
