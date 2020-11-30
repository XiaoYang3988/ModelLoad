#ifndef MODEL_OBJ
#define MODEL_OBJ 

#include "../buffer_object/buffer_object.h"
#include "../shader_obj/shader.h"
#include "../create_window/window.h"
#include "read_model_file.h"
#include "model_type.h"

#ifdef __cplusplus
extern "C"{
#endif
/**
 *  newmtl: 定义新的材质组，后面参数为材质组名称
 *  Ka: 材质的环境光（ambient color）
 *  Kd: 散射光（diffuse color）
 *  Ks: 镜面光（specular color）
 *  Ke: 放射光（emissive color）
 *  Ns: 材质的光亮度
 *  d，Tr 均可用于定义材质的Alpha透明度
 *  sharpness： 材质的锐度（sharpness）
 *  illum： 照明度（illumination）后面可接0~10范围内的数字参数
 *  Ni : 材质的光密度（optical density）
 *  Tf: 材质的透射滤波（transmission filter），对应数据为r，g，b值
 *  map_Ka、map_Kd map_Ks 材质的环境，散射和镜面贴图，对应数据为贴图文件名称
 *  refl: 材质的反射属性
 */

typedef struct __model_obj model_obj;
typedef struct __module_obj module_obj;
typedef struct __module_shader_obj module_shader_obj;
typedef struct __model_texture_obj model_texture_obj;

// 模型纹理
struct __model_texture_obj{
    char **texture_title;
    uint32_t *t_id;
    uint32_t  t_len;
};

// 渲染模块
struct __module_obj{

    // 纹理数据
    mtl_general_data mtl_d;

    // 缓存设置
    uint32_t vbo;    // 顶点缓存
    uint32_t vao;  // 顶点数组
    
    // 索引顶点数目
    uint32_t index_number;
};

// 模型对象结构体
struct __model_obj{
    // 模块缓存对象
    module_obj *module_obj;

    // 纹理缓存对象
    model_texture_obj texture_obj; // 纹理对象

    // 坐标系变换矩阵
    clip_space *clip_space; // 裁剪矩阵    
    camera_object *camera;  // 观察矩阵
    world_space world_space;    // 世界矩阵

    // 局部光照位置
    vec3 light_position;

    // 模块数量
    uint32_t module_num;
};

// 初始化model_obj结构体
void init_model_obj(model_obj *const model, const obj_model_object *const obj, const mtl_model_object *const mtl, const char *texture_path, clip_space *clip_space, camera_object *camera);
// 路径初始化
void path_init_model_obj(model_obj *const model, const char *c_obj, const char *c_mtl, const char *texture_path, clip_space *clip_space, camera_object *camera);
// 释放model_obj
void free_model_obj(model_obj *model);
// 渲染
void render_model(model_obj *m, shader *s);
#ifdef __cplusplus
}
#endif

#endif