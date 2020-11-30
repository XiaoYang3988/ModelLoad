#include "shader.h"


static char *read_text_all_data(const char* path);
static void shader_compile_check(uint32_t id, err_type type);

static char *read_text_all_data(const char* path){

    size_t size = 0;
    FILE *fp = NULL;  
    char *text = NULL;    
    size_t length = 0;

    fp = fopen(path, "rt");  

    if(fp == NULL){
        printf("fp获取文件指针失败\n");
        return NULL;
    }

    fseek(fp, 0L, SEEK_END);  
    size = ftell(fp);  
    fseek(fp, 0L, SEEK_SET);  

    if(size == 0){
        printf("文本字节大小为0\n");
        return NULL;     
    }

    text = malloc(size * sizeof(char) + 1);

    if(text == NULL){
        printf("read_text_all_data函数中申请堆内存失败\n");
        return NULL;    
    }

    do{
        text[length] = fgetc(fp);
        if( feof(fp) ){   
            text[length] = '\0';
            break ;
        }
        length++;
    } while(1);

    fclose(fp); 

    return text;
}
static void shader_compile_check(uint32_t id, err_type type){
     
    int32_t success = 0; 
    char infoLog[ERROR_LOG_SIZE];

    if(type == compile_error){
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {   
            glGetShaderInfoLog(id, ERROR_LOG_SIZE, NULL, infoLog);
            printf("ERROR::SHADER_COMPILATION_ERROR of type: %s \n", infoLog);
            printf("\n -- --------------------------------------------------- -- ");
            exit(0);
        }
    }else if (type == app_error){
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {   
            glGetProgramInfoLog(id, ERROR_LOG_SIZE, NULL, infoLog);
            printf("ERROR::PROGRAM_LINKING_ERROR of type: %s \n", infoLog);
            printf("\n -- --------------------------------------------------- -- ");   
            exit(0);         
        }
    }

}

void init_vertex_shader(vertex_shader *const vs,const char *path){

    uint32_t vertex = 0;
    char *p = read_text_all_data(path);
    if(p == NULL){
        memset(vs, 0, sizeof(vertex_shader));
        return;
    }
    const char *text = p;

    vertex = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertex, 1, &text, NULL);

    glCompileShader(vertex);

    shader_compile_check(vertex, compile_error);
    vs->vs_id = vertex;
    
    free(p);
}
void delete_vs(vertex_shader *const vs){
    glDeleteShader(vs->vs_id);    
    memset(vs, 0, sizeof(vertex_shader));
}

void init_fragment_shader(fragment_shader *const fs, const char *path){

    uint32_t fragment = 0;
    
    char *p = read_text_all_data(path);
    if(p == NULL){
        memset(fs, 0, sizeof(fragment_shader));
        return;
    }
    const char *text = p;

    fragment = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragment, 1, &text, NULL);

    glCompileShader(fragment);

    shader_compile_check(fragment, compile_error);

    fs->fs_id = fragment;
    free(p);
}
void delete_fs(fragment_shader *const fs){
    glDeleteShader(fs->fs_id);
    memset(fs, 0, sizeof(fragment_shader));
}

void init_geometry_shader(geometry_shader *const gs, const char *path){

    uint32_t geometry = 0;
    
    char *p = read_text_all_data(path);
    if(p == NULL){
        memset(gs, 0, sizeof(geometry_shader));
        return;
    }
    const char *text = p;

    geometry = glCreateShader(GL_GEOMETRY_SHADER);

    glShaderSource(geometry, 1, &text, NULL);

    glCompileShader(geometry);

    shader_compile_check(geometry, compile_error);

    gs->gs_id = geometry;
    free(p);
}
void delete_gs(geometry_shader *const gs){
    glDeleteShader(gs->gs_id);
    memset(gs, 0, sizeof(geometry_shader));
}

void new_shader(shader *const s){
    s->shader_id = glCreateProgram();
}
void link_shader(shader *const s){
    glLinkProgram(s->shader_id);
    shader_compile_check(s->shader_id, app_error);
}
void add_vs(shader *const s, vertex_shader *const vs){
    glAttachShader(s->shader_id, vs->vs_id);
}
void add_fs(shader *const s, fragment_shader *const fs){
    glAttachShader(s->shader_id, fs->fs_id);
}
void add_gs(shader *const s, geometry_shader *const gs){
    glAttachShader(s->shader_id, gs->gs_id);
}
void activate_shader(shader *const s){
    glUseProgram(s->shader_id); 
}
void uniform_set_bool(uint32_t s_id, const char *str, bool value){         
    glUniform1i(glGetUniformLocation(s_id, str), (int)value); 
}
void uniform_set_int(uint32_t s_id, const char *str, int32_t value){
    glUniform1i(glGetUniformLocation(s_id, str), value);
}
void uniform_set_float(uint32_t s_id, const char *str, float value){
    glUniform1f(glGetUniformLocation(s_id, str), value); 
}
void uniform_set_vec3(uint32_t s_id, const char *str, vec3 vec){
    glUniform3fv(glGetUniformLocation(s_id, str), 1, &vec[0]);
}
void uniform_set_vec4(uint32_t s_id, const char *str, vec4 vec){
    glUniform4fv(glGetUniformLocation(s_id, str), 1, &vec[0]);
}
void uniform_set_mat3(uint32_t s_id, const char *str, mat3 mat){
    glUniformMatrix3fv(glGetUniformLocation(s_id, str), 1, GL_FALSE, &mat[0][0]);
}
void uniform_set_mat4(uint32_t s_id, const char *str, mat4 mat){
    glUniformMatrix4fv(glGetUniformLocation(s_id, str), 1, GL_FALSE, &mat[0][0]);
}
void delete_shader(shader *const s){
    glDeleteProgram(s->shader_id);
    s->shader_id = 0;
}
