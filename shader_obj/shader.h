
#ifndef SHADER
#define SHADER 

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../include/cglm/mat2.h"
#include "../glad/glad.h"
#ifdef __cplusplus
extern "C"{
#endif

#define ERROR_LOG_SIZE 1024

typedef struct __shader shader;
typedef struct __vertex_shader vertex_shader;
typedef struct __geometry_shader geometry_shader;
typedef struct __fragment_shader fragment_shader;

// 着色器编译错误类型
enum __error_type{
    compile_error = 0,
    app_error,
};
typedef enum __error_type err_type;
// 着色器对象
struct __shader{
    uint32_t shader_id; //申请着色器对象返回的id
};
// 创建一个着色器对象
void new_shader(shader *const s);
// 链接编译好的着色器到着色器对象上
void link_shader(shader *const s);
// 将编译好的顶点着色器添加到着色器对象中
void add_vs(shader *const s, vertex_shader *const vs);
// 将编译好的片段着色器添加到着色器对象中
void add_fs(shader *const s, fragment_shader *const fs);
// 将编译好的几何着色器添加到着色器对象中
void add_gs(shader *const s, geometry_shader *const gs);
// 激活着色器程序对象，之后每个着色器调用和渲染调用都会使用这个程序对象
void activate_shader(shader *const s);
// 更新bool类型uniform，查询并更新uniform(着色器代码中的全局变量),使用前记得先激活着色器程序对象
void uniform_set_bool(uint32_t s_id, const char *str, bool value);
// 更新int类型uniform，查询并更新uniform(着色器代码中的全局变量),使用前记得先激活着色器程序对象
void uniform_set_int(uint32_t s_id, const char *str, int32_t value);
// 更新float类型uniform，查询并更新uniform(着色器代码中的全局变量),使用前记得先激活着色器程序对象
void uniform_set_float(uint32_t s_id, const char *str, float value);
// 更新2float类型uniform，查询并更新uniform(着色器代码中的全局变量),使用前记得先激活着色器程序对象
void uniform_set_2f(uint32_t s_id, const char *str, float x, float y);
// 更新3float类型uniform，查询并更新uniform(着色器代码中的全局变量),使用前记得先激活着色器程序对象
void uniform_set_3f(uint32_t s_id, const char *str, float x, float y, float z);
// 更新4float类型uniform，查询并更新uniform(着色器代码中的全局变量),使用前记得先激活着色器程序对象
void uniform_set_4f(uint32_t s_id, const char *str, float x, float y, float z, float w);
// 更新mat2类型uniform，查询并更新uniform(着色器代码中的全局变量),使用前记得先激活着色器程序对象
void uniform_set_mat2(uint32_t s_id, const char *str, mat2 mat);
// 更新mat3类型uniform，查询并更新uniform(着色器代码中的全局变量),使用前记得先激活着色器程序对象
void uniform_set_mat3(uint32_t s_id, const char *str, mat3 mat);
// 更新mat4类型uniform，查询并更新uniform(着色器代码中的全局变量),使用前记得先激活着色器程序对象
void uniform_set_mat4(uint32_t s_id, const char *str, mat4 mat);
// 更新vec2类型uniform，查询并更新uniform(着色器代码中的全局变量),使用前记得先激活着色器程序对象
void uniform_set_vec2(uint32_t s_id, const char *str, vec2 vec);
// 更新vec3类型uniform，查询并更新uniform(着色器代码中的全局变量),使用前记得先激活着色器程序对象
void uniform_set_vec3(uint32_t s_id, const char *str, vec3 vec);
// 更新vec4类型uniform，查询并更新uniform(着色器代码中的全局变量),使用前记得先激活着色器程序对象
void uniform_set_vec4(uint32_t s_id, const char *str, vec4 vec);
// 删除链接好的着色器对象
void delete_shader(shader *const s);

// 编译顶点着色器对象
struct __vertex_shader{
    uint32_t vs_id;
};
// 创建一个顶点着色器
// 参数：参数1：vertex_shader结构体地址 参数2：着色器代码文件路径
// 失败退出程序
void init_vertex_shader(vertex_shader *const vs, const char *path);
// 删除编译好的顶点着色器
void delete_vs(vertex_shader *const vs);

// 编译几何着色器对象
struct __geometry_shader{
    uint32_t gs_id;
};
// 创建一个几何着色器
// 参数：参数1：geometry_shader结构体地址 参数2：着色器代码文件路径
// 失败退出程序
void init_geometry_shader(geometry_shader *const gs, const char *path);
// 删除编译好的几何着色器
void delete_gs(geometry_shader *const gs);

// 编译片段着色器对象
struct __fragment_shader{
    uint32_t fs_id;

};
// 创建一个片段着色器
// 参数：参数1：fragment_shader结构体地址 参数2：着色器代码文件路径
// 失败退出程序
void init_fragment_shader(fragment_shader *const fs, const char *path);
// 删除编译好的片段着色器
void delete_fs(fragment_shader *const fs);
#ifdef __cplusplus
}
#endif

#endif