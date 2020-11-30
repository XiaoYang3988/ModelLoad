#include "buffer_object.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC 
#include "stb_image.h"


static void new_general_buffer_object(general_buffer_object *const g, uint32_t length);
static void free_general_buffer_object(general_buffer_object *const g);

void new_element_data(element_data *const e, size_t size){

    memset(e, 0, sizeof(element_data));
    e->data_size = size;
    e->data = calloc(1, size);

    if(e->data == NULL){
        printf("new_element_data函数中申请堆内存失败\n");
        exit(0);
    }    

}
void free_element_data(element_data *const e){
    if(e->data != NULL){
        free(e->data);
    }
    memset(e, 0, sizeof(element_data));
}
void new_vertices_data(vertices_data *const v, size_t size){

    memset(v, 0, sizeof(vertices_data));
    v->data_size = size;
    v->data = calloc(1, size);

    if(v->data == NULL){
        printf("new_vertices_data函数中申请堆内存失败\n");
        exit(0);
    }   
}
void free_vertices_data(vertices_data *const v){
    if(v->data != NULL){
        free(v->data);
    }
    memset(v, 0, sizeof(vertices_data));
}
// 顶点缓冲函数
void new_vertices_buffer_object_name(vertices_buffer_object *const v, uint32_t size){
    new_general_buffer_object(&v->v_b, size);
    glGenBuffers(v->v_b.id_length, v->v_b.id);
}
void free_vertices_buffer_object(vertices_buffer_object *const v){
    glDeleteBuffers(v->v_b.id_length, v->v_b.id);
    free_general_buffer_object(&v->v_b);
}
// 顶点数组函数
void new_vertices_array_object_name(vertices_array_object *const v, uint32_t size){
    new_general_buffer_object(&v->v_a, size);
    glGenVertexArrays(v->v_a.id_length, v->v_a.id);
}
void free_vertices_array_object(vertices_array_object *const v){
    glDeleteVertexArrays(v->v_a.id_length, v->v_a.id);
    free_general_buffer_object(&v->v_a);
}
void vao_bind_vbo(uint32_t vao, uint32_t vbo, vertices_data *const data){

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, data->data_size, data->data, data->data_use_mode);
    glVertexAttribPointer(data->vertex_location, data->vertex_num, data->data_type, data->vertex_normalize, data->vertices_stride, (void*)(data->vertex_offset));
    glEnableVertexAttribArray(data->vertex_location);

    if(data->colors_num != 0){
        glVertexAttribPointer(data->colors_location, data->colors_num, data->data_type, data->colors_normalize, data->vertices_stride, (void*)(data->colors_offset));
        glEnableVertexAttribArray(data->colors_location);         
    }

    if(data->texture_num != 0){
        glVertexAttribPointer(data->texture_location, data->texture_num, data->data_type, data->texture_normalize, data->vertices_stride, (void*)(data->texture_offset));
        glEnableVertexAttribArray(data->texture_location);  
    }

    if(data->normal_num != 0){
        glVertexAttribPointer(data->normal_location, data->normal_num, data->data_type, data->normal_normalize, data->vertices_stride, (void*)(data->normal_offset));
        glEnableVertexAttribArray(data->normal_location);  
    }
}
void vao_bind_ebo_vbo(uint32_t vao, uint32_t vbo, uint32_t ebo, vertices_data *const v_data, element_data *const e_data){

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, v_data->data_size, v_data->data, v_data->data_use_mode);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, e_data->data_size, e_data->data, v_data->data_use_mode);

    glVertexAttribPointer(v_data->vertex_location, v_data->vertex_num, v_data->data_type, v_data->vertex_normalize, v_data->vertices_stride, (void*)(v_data->vertex_offset));
    glEnableVertexAttribArray(v_data->vertex_location);

    if(v_data->colors_num != 0){
        glVertexAttribPointer(v_data->colors_location, v_data->colors_num, v_data->data_type, v_data->colors_normalize, v_data->vertices_stride, (void*)(v_data->colors_offset));
        glEnableVertexAttribArray(v_data->colors_location);       
    }

    if(v_data->texture_num != 0){
        glVertexAttribPointer(v_data->texture_location, v_data->texture_num, v_data->data_type, v_data->texture_normalize, v_data->vertices_stride, (void*)(v_data->texture_offset));
        glEnableVertexAttribArray(v_data->texture_location);  
    }

    if(v_data->normal_num != 0){
        glVertexAttribPointer(v_data->normal_location, v_data->normal_num, v_data->data_type, v_data->normal_normalize, v_data->vertices_stride, (void*)(v_data->normal_offset));
        glEnableVertexAttribArray(v_data->normal_location);  
    }
}
// 索引缓冲函数
void new_vertices_element_buffer_object_name(vertices_element_buffer_object *const v, uint32_t size){
    new_general_buffer_object(&v->v_e, size);
    glGenBuffers(v->v_e.id_length, v->v_e.id);
}
void free_vertices_element_buffer_object(vertices_element_buffer_object *const v){
    glDeleteBuffers(v->v_e.id_length, v->v_e.id);
    free_general_buffer_object(&v->v_e);
}
// 纹理对象操作函数
void new_texture_object(texture_object *t_o, uint32_t size){
    new_general_buffer_object(&t_o->t_b, size);
    glGenTextures(t_o->t_b.id_length, t_o->t_b.id);
}
void free_texture_object(texture_object *t_o){
    free_general_buffer_object(&t_o->t_b);
}
void texture_object_bind_texture_data(uint32_t t_id, texture_data *t_d){

    if( !(t_d->target == GL_TEXTURE_1D || t_d->target == GL_TEXTURE_2D || t_d->target == GL_TEXTURE_3D) ){
        printf("texture_object_bind_texture_data, target no is error\n");
        return;
    }

    // 开始绑定纹理
    glBindTexture(t_d->target, t_id);
    
    // 纹理环绕过滤设置
    if (t_d->surround_s == GL_CLAMP_TO_BORDER){
        glTexParameteri(t_d->target, GL_TEXTURE_WRAP_S, t_d->surround_s);  
        glTexParameterfv(t_d->target, GL_TEXTURE_BORDER_COLOR, t_d->border_color_s);
    }else
        glTexParameteri(t_d->target, GL_TEXTURE_WRAP_S, t_d->surround_s);  
    
    if( (t_d->target == GL_TEXTURE_2D || t_d->target == GL_TEXTURE_3D) && t_d->surround_t == GL_CLAMP_TO_BORDER){
        glTexParameteri(t_d->target, GL_TEXTURE_WRAP_T, t_d->surround_t);  
        glTexParameterfv(t_d->target, GL_TEXTURE_BORDER_COLOR, t_d->border_color_t);
    }
    else if(t_d->target == GL_TEXTURE_2D || t_d->target == GL_TEXTURE_3D)
        glTexParameteri(t_d->target, GL_TEXTURE_WRAP_T, t_d->surround_t);  
    
    if(t_d->target == GL_TEXTURE_3D && t_d->surround_r == GL_CLAMP_TO_BORDER){
        glTexParameteri(t_d->target, GL_TEXTURE_WRAP_R, t_d->surround_r);  
        glTexParameterfv(t_d->target, GL_TEXTURE_BORDER_COLOR, t_d->border_color_r);        
    }else if(t_d->target == GL_TEXTURE_3D)
        glTexParameteri(t_d->target, GL_TEXTURE_WRAP_R, t_d->surround_r);  

    // 纹理放大缩小过滤设置
    glTexParameteri(t_d->target, GL_TEXTURE_MIN_FILTER, t_d->filter_minify);    
    glTexParameteri(t_d->target, GL_TEXTURE_MAG_FILTER, t_d->filter_magnify);

    if(t_d->t_data == NULL){
        printf("texture_object_bind_texture_data, Failed to load texture\n");
        exit(0);
    }

    if(t_d->target == GL_TEXTURE_2D){
        glTexImage2D(t_d->target, t_d->further_away_level, t_d->internal_format, t_d->width, t_d->height, 0, t_d->format, GL_UNSIGNED_BYTE, t_d->t_data);
    }else if (t_d->target == GL_TEXTURE_3D){
        return;
    }else{
        return;      
    }
    
    // 生成多级渐远纹理
    glGenerateMipmap(t_d->target);

    free_texture_data(t_d);
}
void free_texture_data(texture_data *t_d){
    // 释放纹理数据内存
    free(t_d->t_data);
    memset(t_d, 0, sizeof(texture_data));    
}
// 通用缓冲区对象
static void new_general_buffer_object(general_buffer_object *const g, uint32_t length){

    memset(g, 0, sizeof(general_buffer_object));
    g->id_length = length;
    g->id = calloc(g->id_length, sizeof(uint32_t));

    if(g->id == NULL){
        printf("new_general_buffer_object函数中申请堆内存失败\n");
        exit(0);
    }
}
static void free_general_buffer_object(general_buffer_object *const g){

    if(g->id == NULL){
        g->id_length = 0;
        return;
    }

    free(g->id);
    memset(g, 0, sizeof(general_buffer_object));
}