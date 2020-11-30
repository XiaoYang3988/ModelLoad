#include "window.h"

GLenum glCheckError_(const char *file, int line){
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        const char* error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        printf("%s  |  %s(%d)\n", error, file, line);
    }
    return errorCode;
}
GLFWwindow *init_window(uint32_t width, uint32_t height, const char *title, uint32_t major, uint32_t minor){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL)
    {
        puts("Failed to create GLFW window");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        puts("Failed to initialize GLAD");
        return NULL;
    }    

    glEnable(GL_DEPTH_TEST);
    return window;
}
void each_frame_time(render_time *const time){
    float current_frame = glfwGetTime();
    time->delta_time = current_frame - time->last_frame;
    time->last_frame = current_frame;
}

void process_keyboard(camera_movement direction, render_time t, camera_object *c){

    float camera_speed = c->movement_speed * t.delta_time;
    if (direction == FORWARD){
        vec3 scale; 
        glm_vec3_scale(c->observe_location, camera_speed, scale);
        glm_vec3_add(c->camera_location, scale, c->camera_location);        
    }
    if (direction == BACKWARD){
        vec3 scale;
        glm_vec3_scale(c->observe_location, camera_speed, scale);
        glm_vec3_sub(c->camera_location, scale, c->camera_location);        
    }
    if (direction == LEFT){
        vec3 scale;
        glm_vec3_crossn(c->observe_location, c->camera_up, scale);
        glm_vec3_normalize(scale);
        glm_vec3_scale(scale, camera_speed, scale);
        glm_vec3_sub(c->camera_location, scale, c->camera_location);
    }
    if (direction == RIGHT){
        vec3 scale;
        glm_vec3_crossn(c->observe_location, c->camera_up, scale);
        glm_vec3_normalize(scale);
        glm_vec3_scale(scale, camera_speed, scale);
        glm_vec3_add(c->camera_location, scale, c->camera_location);
    }
}
void create_camera_look_at(camera_object *const c){
	vec3 v = {0.0f, 0.0f, 0.0f};
    glm_vec3_add(c->camera_location, c->observe_location, v);
    glm_lookat(c->camera_location, v, c->camera_up, c->look_at);
    //glm_mat4_identity(c->look_at);
}
void create_clip_space_projection(clip_space *const c){
    if(c->type == perspective_projection){
        glm_perspective(glm_rad(c->zoom), c->aspect_ratio, c->nevr_val, c->far_val, c->projection);
    }else if(c->type == orthographic_projection){

    }    
}
void init_camera(camera_object *const c_ojb, vec3 camera_location, vec3 observe_location, vec3 camera_up, float yaw, float pitch, float lastX, float lastY, float movement_speed, float sensitivity, bool first_mouse){
    c_ojb->yaw = yaw;
    c_ojb->pitch = pitch;
    c_ojb->lastX = lastX;
    c_ojb->lastY = lastY;
    c_ojb->movement_speed = movement_speed;
    c_ojb->sensitivity = sensitivity;
    c_ojb->first_mouse = first_mouse;
    glm_vec3_copy(camera_location, c_ojb->camera_location);
    glm_vec3_copy(observe_location, c_ojb->observe_location);
    glm_vec3_copy(camera_up, c_ojb->camera_up);
}
void init_world_space(world_space *const w){
    glm_mat4_identity(w->model);
}
void init_clip_space(clip_space *const c, clip_type type, 
float zoom, float aspect_ratio,
float nevr_val, float far_val){
    glm_mat4_identity(c->projection);
    c->type = type;
    c->zoom = zoom;
    c->far_val = far_val;
    c->aspect_ratio = aspect_ratio;
    c->nevr_val = nevr_val;
    if(type == perspective_projection){
        glm_perspective(glm_rad(c->zoom), c->aspect_ratio, c->nevr_val, c->far_val, c->projection);
    }else if(type == orthographic_projection){

    }
}
