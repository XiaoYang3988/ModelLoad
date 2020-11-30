#ifndef READ_MODEL_FILE
#define READ_MODEL_FILE 

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "model_type.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct __obj_model_object obj_model_object;
typedef struct __v_general_data v_general_data;
typedef struct __index_data index_data;
typedef struct __mtl_model_object mtl_model_object;
typedef struct __mtl_general_data mtl_general_data;

/**
 *  v 几何体顶点(Geometric vertices)
 *  vt 贴图坐标点(Texture vertices)
 *  vn 顶点法线(Vertex normals)
 *  vp 参数空格顶点 (Parameter space vertices)
 *  index 绘制的索引顺序
 */

// 索引类型
enum __index_type{
    vertex = 0,
    vertex_texture,
    vertex_normal,
    vertex_texture_normal,
};
typedef enum __index_type index_type;

// v 通用数据结构
struct __v_general_data{
    float **v_array;  // type数据的数组指针
    size_t *v_array_each_size; // 依次记录type数据的数组指针中每个指针所指数据的大小
    uint32_t *v_type;  // 每个type数组指针中数据的的类型(步长)
    uint32_t v_array_length; // type数据的数组指针的长度    
};

// mtl 数据结构
struct __mtl_general_data{
    float Ns;
    float Ni;
    float Tr;
    float d;
    float illum;
    float sharpness;
    float Tf[MTL_ARRAY_FLOAT];
    float Ka[MTL_ARRAY_FLOAT];
    float Kd[MTL_ARRAY_FLOAT];
    float Ks[MTL_ARRAY_FLOAT];
    float Ke[MTL_ARRAY_FLOAT];
    char map_Ka[TITLE_LENGTH];
    char map_Kd[TITLE_LENGTH];
    char map_Ks[TITLE_LENGTH];
    char map_Ns[TITLE_LENGTH];
    char map_d[TITLE_LENGTH];
    char map_bump[TITLE_LENGTH];
    char map_refl[TITLE_LENGTH];

	char newmtl_title[TITLE_LENGTH];
};

// 顶点索引数据
struct __index_data{
    uint32_t **index_array; // 顶点索引数据的数组指针
    char **index_usemtl_title; // 顶点索引usemtl_title的数组指针
    size_t *index_array_each_size; // 依次记录顶点索引数据的数组指针中每个指针所指数据的大小
    uint32_t *index_step;  // 每个顶点索引数据数组指针中数据的的类型(步长)
    uint32_t *plane_v_num;   // 组成一个顶点的顶点数目
    index_type *index_type;  // 索引绘制类型
    uint32_t index_array_length; // 顶点索引数据的数组指针的长度    
};

// 3d obj 结构体
struct __obj_model_object{
    v_general_data v_d; // 顶点
    v_general_data vt_d; // 纹理
    v_general_data vn_d; // 顶点法线
    v_general_data vp_d; // 参数空格顶点
    index_data index_d; // 顶点索引

    char title[TITLE_LENGTH];    // 模型标题
    char mtllib_title[MTLLIB_SIZE][TITLE_LENGTH]; // 材质标题
};

// 使用obj文件初始化obj_model_object结构体
void init_obj_model_object(obj_model_object *const obj, const char* path);
// 释放obj_model_object内存
void free_obj_model_object(obj_model_object *const obj);

// 3d mtl 结构体
struct __mtl_model_object{
    mtl_general_data **newmtl;
    uint32_t newmtl_array_len;
};

// 使用mtl文件初始化mtl_model_object结构体
void init_mtl_model_object(mtl_model_object *const mtl, const char* path);
// 释放mtl_model_object
void free_mtl_model_object(mtl_model_object *const mtl);
#ifdef __cplusplus
}
#endif

#endif