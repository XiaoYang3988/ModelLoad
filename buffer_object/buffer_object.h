#ifndef BUFFER_OBJECT
#define BUFFER_OBJECT 

#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../glad/glad.h"
#ifdef __cplusplus
extern "C"{
#endif

#define TITLE_LENGTH 255

typedef struct __vertices_buffer_object vertices_buffer_object;
typedef struct __vertices_data vertices_data;
typedef struct __vertices_array_object vertices_array_object;
typedef struct __vertices_element_buffer_object vertices_element_buffer_object;
typedef struct __element_data element_data;
typedef struct __general_buffer_object general_buffer_object;
typedef struct __texture_data texture_data;
typedef struct __texture_object texture_object;

struct __general_buffer_object{
    uint32_t *id;
    uint32_t id_length;
};

// 索引数据结构体
struct __element_data{
    uint32_t *data; // 索引数据首地址
    size_t data_size; // 索引数据大小
};

void new_element_data(element_data *const e, size_t size);
void free_element_data(element_data *const e);

// 连续顶点数据结构体，必须按照顶点位置，颜色，纹理坐标紧密排列
struct __vertices_data{
    float *data; // 顶点数据首地址

    size_t data_size; // 顶点数据大小
    GLenum  data_type; // 指定数据的类型
    uint32_t vertices_stride; // 顶点属性的跨距
    uint32_t data_use_mode; //数据使用方式

    // 顶点位置属性
    uint32_t vertex_location; // 顶点位置数据的位置值
    uint32_t vertex_num; // 顶点位置成员个数
    uint32_t vertex_offset; // 位置数据在缓冲中起始位置的偏移量

    // 顶点颜色属性
    uint32_t colors_location;  // 顶点颜色数据的位置值
    uint32_t colors_num;
    uint32_t colors_offset; 

    // 顶点纹理坐标属性
    uint32_t texture_location;  // 顶点纹理数据的位置值
    uint32_t texture_num;   
    uint32_t texture_offset; 

    // 顶点法线坐标属性
    uint32_t normal_location;  // 顶点纹理数据的位置值
    uint32_t normal_num;   
    uint32_t normal_offset;    
    
    bool vertex_normalize; // 是否希望数据被标准化
    bool colors_normalize; 
    bool texture_normalize; 
    bool normal_normalize;
};

void new_vertices_data(vertices_data *const v, size_t size);
// 释放顶点数据
void free_vertices_data(vertices_data *const v);

// 顶点缓冲对象
struct __vertices_buffer_object{
    general_buffer_object v_b;
};

// 生成顶点数组对象名称
void new_vertices_buffer_object_name(vertices_buffer_object *const v, uint32_t size);
// 释放顶点缓冲对象
void free_vertices_buffer_object(vertices_buffer_object *const v);


// 索引缓冲对象
struct __vertices_element_buffer_object{
    general_buffer_object v_e;
};

// 生成索引缓冲对象名称
void new_vertices_element_buffer_object_name(vertices_element_buffer_object *const v, uint32_t size);
// 释放索引缓冲对象
void free_vertices_element_buffer_object(vertices_element_buffer_object *const v);

// 顶点数组对象
struct __vertices_array_object{
    general_buffer_object v_a;
};

// 生成顶点数组对象名称
void new_vertices_array_object_name(vertices_array_object *const v, uint32_t size);
// 释放顶点数组对象
void free_vertices_array_object(vertices_array_object *const v);
// vao绑定vbo
void vao_bind_vbo(uint32_t vao, uint32_t vbo, vertices_data *const data);
// vao绑定vbo还有ebo
void vao_bind_ebo_vbo(uint32_t vao, uint32_t vbo, uint32_t ebo, vertices_data *const v_data, element_data *const e_data);

// 纹理数据和过滤设置
struct __texture_data{
    uint8_t *t_data;   // 数据首地址   

    GLenum  target;  // 纹理目标
    int32_t further_away_level; // 为纹理指定多级渐远纹理的级别
    int32_t internal_format;    // 纹理数据的储存格式
    int32_t width;             // 纹理的宽度
    int32_t height;            // 纹理的高度
    int32_t nr_channels;       // 通道值
    GLenum format;  // 纹理的源数据格式
    // 纹理环绕
    int32_t surround_s; // 纹理s坐标轴环绕方式
    int32_t surround_t; // 纹理t坐标轴环绕方式
    int32_t surround_r; // 纹理r坐标轴环绕方式

    float border_color_s[4]; // 超出的s坐标为用户指定的边缘颜色
    float border_color_t[4]; // 超出的t坐标为用户指定的边缘颜色
    float border_color_r[4]; // 超出的r坐标为用户指定的边缘颜色

    // 纹理过滤选择
    GLenum filter_magnify;  // 纹理放大过滤
    GLenum filter_minify;   // 纹理缩小过滤

    bool y_flip_texture;        // 在y轴上翻转加载的纹理
};

// 释放texture_data
void free_texture_data(texture_data *t_d);

// 纹理对象
struct __texture_object{
    general_buffer_object t_b;
};

void new_texture_object(texture_object *t_o, uint32_t size);
void free_texture_object(texture_object *t_o);
void texture_object_bind_texture_data(uint32_t t_id, texture_data *t_d);
void free_texture_data(texture_data *t_d);
#ifdef __cplusplus
}
#endif

#endif