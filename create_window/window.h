#ifndef WINDOW
#define WINDOW 

#include <inttypes.h>
#include <stdio.h>

#include "../include/cglm/vec3.h"
#include "../include/cglm/mat4.h"
#include "../include/cglm/cam.h"
#include "../glad/glad.h"
#include "../include/GLFW/glfw3.h"
#ifdef __cplusplus
extern "C"{
#endif

#define glCheckError() glCheckError_(__FILE__, __LINE__) 

typedef struct __render_time render_time;
typedef struct __camera_object camera_object;
typedef struct __world_space world_space;
typedef struct __clip_space clip_space;

// 裁剪类型
enum __clip_type{
    orthographic_projection,
    perspective_projection,
};
typedef enum __clip_type clip_type;

// 键盘输入
enum __camera_movement{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};
typedef enum __camera_movement camera_movement;

// 摄像机(观察空间)
struct __camera_object{
    vec3 camera_location;   // 相机位置
    vec3 observe_location;  // 相机观察位置
    vec3 camera_up;   // 向上的一个位置
    mat4 look_at;    // 观察目标矩阵

    float yaw;
    float pitch;
    float lastX;
    float lastY;  
    float movement_speed;
    float sensitivity;
    bool first_mouse;  
};

// 世界空间
struct __world_space{
    mat4 model;     // 世界4*4矩阵
};

// 裁剪空间
struct __clip_space{
    mat4 projection;    // 裁剪4*4矩阵

    float zoom;
    float aspect_ratio;
    float nevr_val;
    float far_val;

    clip_type type;     // 裁剪类型
};

// 渲染时间
struct __render_time{
    float delta_time;    // 当前帧与上一帧的时间差
    float last_frame;    // 上一帧的时间    
};

//
void create_clip_space_projection(clip_space *const c);
// 获取每一帧渲染的时间
void each_frame_time(render_time *const time);
// 世界空间初始化
void init_world_space(world_space *const w);
// 窗口和上下文初始化
GLFWwindow *init_window(uint32_t width, uint32_t height, const char *title, uint32_t Major, uint32_t Minor);
// 摄像机位置初始化
void init_camera(camera_object *const c_ojb, vec3 camera_location, vec3 observe_location, vec3 camera_up, float yaw, float pitch, float lastX, float lastY, float movement_speed, float sensitivity, bool first_mouse);
// 创造观察矩阵
void create_camera_look_at(camera_object *const c);
// 裁剪空间初始化
void init_clip_space(clip_space *const c, clip_type type, 
float zoom, float aspect_ratio,
float nevr_val, float far_val);
// 查看状态错误
GLenum glCheckError_(const char *file, int line);
// 观察矩阵键盘输入事件
void process_keyboard(camera_movement direction, render_time t, camera_object *c);
#ifdef __cplusplus
}
#endif

#endif