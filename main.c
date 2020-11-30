#include "main_include_path.h"

#define MAJOR 3
#define MINOR 3
#define SCR_WIDTH 1920
#define SCR_HEIGHT 1080
#define NEAR_VAL 0.1f
#define FAR_VAL 1000.0f
#define ZOOM 45.0f
#define MOVE_SPEED 4.0f
#define SENSITIVITY 0.05F

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// 每帧渲染时间差
render_time time_difference;
// 观察空间
camera_object c_obj;
// 裁剪空间
clip_space c_s;
// 世界空间
world_space w_s;
world_space light;

int main(){
    vec3 camera_location = {0.0f, 0.0f, 3.0f};
    vec3 observe_location = {0.0f, 0.0f, -1.0f};
    vec3 camera_up = {0.0f, 1.0f, 0.0f};
    const char *title = "Hi OpenGL";
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    GLFWwindow* window = init_window(SCR_WIDTH, SCR_HEIGHT, title, MAJOR, MINOR);
    if (window == NULL){
        return 0;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    init_clip_space(&c_s, perspective_projection, 45.0f, SCR_WIDTH/SCR_HEIGHT, NEAR_VAL, FAR_VAL);
    init_camera(&c_obj, camera_location, observe_location, camera_up, -90.0f, 0.0f, (float)SCR_WIDTH / 2.0, (float)SCR_HEIGHT / 2.0, MOVE_SPEED, SENSITIVITY, true);
    init_world_space(&w_s);
    init_world_space(&light);
    // 模型对象加载
    model_obj model_obj;

    /*
    path_init_model_obj(&model_obj, "model_file/Bloodborne/c4520 lady maria.obj", 
    "model_file/Bloodborne/c4520 lady maria.mtl", "model_file/Bloodborne/texture/", &c_s, &c_obj);
    */
    
    
    path_init_model_obj(&model_obj, "model_file/nami/nanosuit.obj", 
    "model_file/nami/nanosuit.mtl", "model_file/nami/", &c_s, &c_obj);
    

    vertex_shader vs;
    fragment_shader fs;
    shader s;

    init_vertex_shader(&vs, "shader_code/obj_model/obj_model.vs");
    init_fragment_shader(&fs, "shader_code/obj_model/obj_model.fs");

    new_shader(&s);
    add_vs(&s, &vs);
    add_fs(&s, &fs);
    link_shader(&s);
    delete_vs(&vs);
    delete_fs(&fs);
    glCheckError();


    vertex_shader vs1;
    fragment_shader fs1;
    shader s1;

    init_vertex_shader(&vs1, "shader_code/light/light.vs");
    init_fragment_shader(&fs1, "shader_code/light/light,fs");

    new_shader(&s1);
    add_vs(&s1, &vs1);
    add_fs(&s1, &fs1);
    link_shader(&s1);
    delete_vs(&vs1);
    delete_fs(&fs1);
    glCheckError();

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)){   

        // 计算渲染所花时间
        // --------------
        each_frame_time(&time_difference);

        // 输入事件
        // -----
        process_input(window);

        // 颜色缓冲
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // 模型
        activate_shader(&s);

        create_clip_space_projection(&c_s);
        uniform_set_mat4(s.shader_id, "projection", c_s.projection);
        create_camera_look_at(&c_obj);
        uniform_set_mat4(s.shader_id, "view", c_obj.look_at);
        uniform_set_mat4(s.shader_id, "model", w_s.model);

        render_model(&model_obj, &s);

        // 光源
        activate_shader(&s1);
        create_clip_space_projection(&c_s);
        uniform_set_mat4(s1.shader_id, "projection", c_s.projection);
        create_camera_look_at(&c_obj);
        uniform_set_mat4(s1.shader_id, "view", c_obj.look_at);
        // 设置世界坐标
        vec3 scale = {0.1, 0.1, 0.1};
        init_world_space(&light);
        glm_translate(light.model, model_obj.light_position);
        glm_scale(light.model, scale);
        uniform_set_mat4(s1.shader_id, "model", light.model);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // 交换缓冲区和轮询IO事件(按下/释放的键，鼠标移动等)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 清除自定义数据结构
    // ------------------------------------------------------------------
    free_model_obj(&model_obj);
    delete_shader(&s);
    delete_shader(&s1);
    glCheckError();
    // 终止，清除所有以前分配的glfw资源。
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
// 处理所有输入:查询GLFW是否在此帧中按下/释放相关的键，并做出相应的反应
// ---------------------------------------------------------------------------------------------------------
void process_input(GLFWwindow *window){
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        process_keyboard(FORWARD, time_difference, &c_obj);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        process_keyboard(BACKWARD, time_difference, &c_obj);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        process_keyboard(LEFT, time_difference, &c_obj);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        process_keyboard(RIGHT, time_difference, &c_obj);
}
// 每当鼠标移动时，这个回调被调用
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    
    if (c_obj.first_mouse)
    {
        c_obj.lastX = xpos;
        c_obj.lastY = ypos;
        c_obj.first_mouse = false;
    }

    float xoffset = xpos - c_obj.lastX;
    float yoffset = c_obj.lastY - ypos; // reversed since y-coordinates go from bottom to top
    c_obj.lastX = xpos;
    c_obj.lastY = ypos;

    xoffset *= c_obj.sensitivity;
    yoffset *= c_obj.sensitivity;

    c_obj.yaw += xoffset;
    c_obj.pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (c_obj.pitch > 89.0f)
        c_obj.pitch = 89.0f;
    if (c_obj.pitch < -89.0f)
        c_obj.pitch = -89.0f;

    vec3 front;
    front[0] = cos(glm_rad(c_obj.yaw)) * cos(glm_rad(c_obj.pitch));
    front[1] = sin(glm_rad(c_obj.pitch));
    front[2] = sin(glm_rad(c_obj.yaw)) * cos(glm_rad(c_obj.pitch));
    glm_normalize(front);
    glm_vec3_copy(front, c_obj.observe_location);
}
// 每当鼠标滚轮滚动时，这个回调被调用
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){   
    
    c_s.zoom -= (float)yoffset;
    if (c_s.zoom < 1.0f)
        c_s.zoom = 1.0f;
    if (c_s.zoom > 45.0f)
        c_s.zoom = 45.0f;
}
