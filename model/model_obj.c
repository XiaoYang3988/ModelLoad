#include "model_obj.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC 
#include "../buffer_object/stb_image.h"

static void module_render_activate_texture(module_obj *const m, model_texture_obj mt, shader *s);
static void free_model_texture_obj(model_texture_obj *m);
static void free_module_obj(module_obj *m, uint32_t num);
static void init_model_texture(model_texture_obj *const t, const mtl_model_object *const mtl, const char *path);
static void bool_model_pointer(void *p, const char *str);
static void init_texture_data(texture_data *const t_d, GLenum texture_type, const char* img_path);
static void init_module_obj(module_obj *m_obj, const obj_model_object *const obj, const mtl_model_object *const mtl);
static void get_vertices_data(vertices_data *const v_d, const obj_model_object *const obj, uint32_t num);
static uint32_t get_array_data_location(uint32_t num, const uint32_t *const arr, uint32_t arr_len);
static void get_obj_vertices_type_data(uint32_t index, uint32_t location, const v_general_data *const v, float *const f, const uint32_t *const arr);
static void set_module_texture_off(module_obj *m_obj, const mtl_model_object *const m_d, const char *str);

static void module_render_activate_texture(module_obj *const m, model_texture_obj mt, shader *s){

    uint32_t loop = 0;
    GLenum texture_num = GL_TEXTURE0;
    uint32_t num = 0;

    uniform_set_bool(s->shader_id, "ambient_obj.off", false);
    uniform_set_bool(s->shader_id, "diffuse_obj.off", false);
    uniform_set_bool(s->shader_id, "specular_ks_obj.off", false);

    uniform_set_float(s->shader_id, "specular_ks_obj.shininess", m->mtl_d.Ns);
    uniform_set_float(s->shader_id, "Ni", m->mtl_d.Ni);
    uniform_set_float(s->shader_id, "Tr", m->mtl_d.Tr);
    uniform_set_float(s->shader_id, "d", m->mtl_d.d);
    uniform_set_float(s->shader_id, "illum", m->mtl_d.illum);
    uniform_set_float(s->shader_id, "sharpness", m->mtl_d.sharpness);
    
    uniform_set_vec3(s->shader_id, "ambient_obj.ka.d", m->mtl_d.Ka);
    uniform_set_vec3(s->shader_id, "diffuse_obj.kd.d", m->mtl_d.Kd);
    uniform_set_vec3(s->shader_id, "specular_ks_obj.ks.d", m->mtl_d.Ks);
    uniform_set_vec3(s->shader_id, "Ke", m->mtl_d.Ke);
    uniform_set_vec3(s->shader_id, "Tf", m->mtl_d.Tf);

    if (strlen(m->mtl_d.map_Ka) != 0){
        while (loop < mt.t_len){
            if ( strncmp(m->mtl_d.map_Ka, mt.texture_title[loop], strlen(m->mtl_d.map_Ka)) == 0){
                glActiveTexture(texture_num);

                uniform_set_int(s->shader_id, "ambient_obj.ka.texture", num);
                uniform_set_bool(s->shader_id, "ambient_obj.off", true);

                glBindTexture(GL_TEXTURE_2D, mt.t_id[loop]);

                texture_num += 1;
                num += 1;
                break;
            }
            loop += 1;
        }
        loop = 0;
    }
    
    if (strlen(m->mtl_d.map_Kd) != 0){
        while (loop < mt.t_len){
            if ( strncmp(m->mtl_d.map_Kd, mt.texture_title[loop], strlen(m->mtl_d.map_Kd)) == 0 ){
                glActiveTexture(texture_num);

                uniform_set_int(s->shader_id, "diffuse_obj.kd.texture", num);
                uniform_set_bool(s->shader_id, "diffuse_obj.off", true);

                glBindTexture(GL_TEXTURE_2D, mt.t_id[loop]);
                texture_num += 1;
                num += 1;
                break;
            }
            loop += 1;
        }
        loop = 0;
    }
  
    if (strlen(m->mtl_d.map_Ks) != 0){
        while (loop < mt.t_len){
            if ( strncmp(m->mtl_d.map_Ks, mt.texture_title[loop], strlen(m->mtl_d.map_Ks)) == 0 ){
                glActiveTexture(texture_num);

                uniform_set_int(s->shader_id, "specular_ks_obj.ks.texture", num);
                uniform_set_bool(s->shader_id, "specular_ks_obj.off", true);

                glBindTexture(GL_TEXTURE_2D, mt.t_id[loop]);
                texture_num += 1;
                num += 1;
                break;
            }
            loop += 1;
        }
        loop = 0;
    }

    if (strlen(m->mtl_d.map_Ns) != 0){
        while (loop < mt.t_len){
            if ( strncmp(m->mtl_d.map_Ns, mt.texture_title[loop], strlen(m->mtl_d.map_Ns)) == 0 ){
                glActiveTexture(texture_num);

                uniform_set_int(s->shader_id, "map_ns", num);
                uniform_set_bool(s->shader_id, "ns_off", true);

                glBindTexture(GL_TEXTURE_2D, mt.t_id[loop]);
                texture_num += 1;
                num += 1;
                break;
            }
            loop += 1;
        }
        loop = 0;
    }

    if (strlen(m->mtl_d.map_d) != 0){
        while (loop < mt.t_len){
            if ( strncmp(m->mtl_d.map_d, mt.texture_title[loop], strlen(m->mtl_d.map_d)) == 0 ){
                glActiveTexture(texture_num);
                glBindTexture(GL_TEXTURE_2D, mt.t_id[loop]);
                texture_num += 1;
                num += 1;
                break;
            }
            loop += 1;
        }
        loop = 0;
    }
  
    if (strlen(m->mtl_d.map_bump) != 0){
        while (loop < mt.t_len){
            if ( strncmp(m->mtl_d.map_bump, mt.texture_title[loop], strlen(m->mtl_d.map_bump)) == 0 ){
                glActiveTexture(texture_num);
                glBindTexture(GL_TEXTURE_2D, mt.t_id[loop]);

                texture_num += 1;
                num += 1;
                break;
            }
            loop += 1;
        }
        loop = 0;
    }
  
    if (strlen(m->mtl_d.map_refl) != 0){
        while (loop < mt.t_len){
            if ( strncmp(m->mtl_d.map_refl, mt.texture_title[loop], strlen(m->mtl_d.map_refl)) == 0 ){
                glActiveTexture(texture_num);
                glBindTexture(GL_TEXTURE_2D, mt.t_id[loop]);

                texture_num += 1;
                num += 1;
                break;
            }
            loop += 1;
        }
        loop = 0;
    }
    glCheckError();
}
static void set_module_texture_off(module_obj *m_obj, const mtl_model_object *const m_d, const char *str){

    uint32_t loop = 0;
    while ( loop < m_d->newmtl_array_len ){
        if (strncmp(m_d->newmtl[loop]->newmtl_title, str, strlen(str)) == 0){
            memcpy(&m_obj->mtl_d, m_d->newmtl[loop], sizeof(mtl_general_data));
            break;
        }
        loop += 1;
    }
    
}
static void init_texture_data(texture_data *const t_d, GLenum texture_type, const char* img_path){

    GLenum format = 0;

    // 读取纹理数据
    t_d->t_data = stbi_load(img_path, &(t_d->width), &(t_d->height), &(t_d->nr_channels), 0);

    if (t_d->t_data == NULL){
        printf("Texture failed to load at %s", img_path);
        return;
    }

    stbi_set_flip_vertically_on_load(t_d->y_flip_texture);

    if (t_d->nr_channels == 1)
        format = GL_RED;
    else if (t_d->nr_channels == 3)
        format = GL_RGB;
    else if (t_d->nr_channels == 4)
        format = GL_RGBA;   

    t_d->y_flip_texture = true;
    t_d->target = texture_type;
    t_d->further_away_level = 0;
    t_d->internal_format = format;
    t_d->format = format;
    
    t_d->surround_s = GL_REPEAT;
    t_d->surround_t = GL_REPEAT;

    t_d->filter_magnify = GL_LINEAR;
    t_d->filter_minify = GL_LINEAR; //GL_LINEAR_MIPMAP_LINEAR
}
static void init_module_obj(module_obj *m_obj, const obj_model_object *const obj, const mtl_model_object *const mtl){
    
    vertices_data v_d;
    element_data e_d;
    uint32_t loop = 0;
    module_obj *m = m_obj;

    memset(&v_d, 0, sizeof(vertices_data));
    memset(&e_d, 0, sizeof(element_data));

    // 按照索引顺序写入顶点缓存
    while (loop < obj->index_d.index_array_length){ 
        
        m->index_number = obj->index_d.index_array_each_size[loop] / (obj->index_d.plane_v_num[loop] * sizeof(int));

        set_module_texture_off(m, mtl, obj->index_d.index_usemtl_title[loop]);
        get_vertices_data(&v_d, obj, loop);

        glGenVertexArrays(1, &m->vao);
        glGenBuffers(1, &m->vbo);

        vao_bind_vbo(m->vao, m->vbo, &v_d);

        free_vertices_data(&v_d);
        free_element_data(&e_d);

        loop += 1;
        m += 1;
    }

}
static void init_model_texture(model_texture_obj *const t, const mtl_model_object *const mtl, const char *path){
    
    uint32_t loop = 0;
    uint32_t t_num = 0;
    uint32_t t_num1 = 0;
    uint32_t t_num2 = 0;
    uint32_t str_len = 0;
    texture_data t_d;
    GLenum texture_type = 0;
    char img_path[IMG_PATH_LENGTH] = {0};
    char **t_texture = NULL;

    memset(&t_d, 0, sizeof(texture_data));

    // 去除重复纹理
    while (loop < mtl->newmtl_array_len){

        if (mtl->newmtl[loop]->map_Ka[0] != '\0'){
            t_num += 1;
        }

        if (mtl->newmtl[loop]->map_Kd[0] != '\0'){
            t_num += 1;
        }   

        if (mtl->newmtl[loop]->map_Ks[0] != '\0'){
            t_num += 1;
        }   

        if (mtl->newmtl[loop]->map_Ns[0] != '\0'){
            t_num += 1;
        }   
        
        if (mtl->newmtl[loop]->map_d[0] != '\0'){
            t_num += 1;
        }   

        if (mtl->newmtl[loop]->map_bump[0] != '\0'){
            t_num += 1;
        }   

        if (mtl->newmtl[loop]->map_refl[0] != '\0'){
            t_num += 1;
        }  

        loop += 1;
    }
    
    if(t_num == 0){
        return;
    }

    t->texture_title = calloc(t_num, sizeof(char*));
    loop = 0;

    while (loop < mtl->newmtl_array_len){

        if (mtl->newmtl[loop]->map_Ka[0] != '\0'){
            t->texture_title[t_num1] = calloc(1, strlen(mtl->newmtl[loop]->map_Ka) + 1);
            memcpy(t->texture_title[t_num1], mtl->newmtl[loop]->map_Ka, strlen(mtl->newmtl[loop]->map_Ka) );
            t_num1 += 1;
        }

        if (mtl->newmtl[loop]->map_Kd[0] != '\0'){
            t->texture_title[t_num1] = calloc(1, strlen(mtl->newmtl[loop]->map_Kd) + 1);
            memcpy(t->texture_title[t_num1], mtl->newmtl[loop]->map_Kd, strlen(mtl->newmtl[loop]->map_Kd) );
            t_num1 += 1;
        }   

        if (mtl->newmtl[loop]->map_Ks[0] != '\0'){
            t->texture_title[t_num1] = calloc(1, strlen(mtl->newmtl[loop]->map_Ks) + 1);
            memcpy(t->texture_title[t_num1], mtl->newmtl[loop]->map_Ks, strlen(mtl->newmtl[loop]->map_Ks));
            t_num1 += 1;
        }   

        if (mtl->newmtl[loop]->map_Ns[0] != '\0'){
            t->texture_title[t_num1] = calloc(1, strlen(mtl->newmtl[loop]->map_Ns) + 1);
            memcpy(t->texture_title[t_num1], mtl->newmtl[loop]->map_Ns, strlen(mtl->newmtl[loop]->map_Ns) );
            t_num1 += 1;
        }   
        
        if (mtl->newmtl[loop]->map_d[0] != '\0'){
            t->texture_title[t_num1] = calloc(1, strlen(mtl->newmtl[loop]->map_d) + 1);
            memcpy(t->texture_title[t_num1], mtl->newmtl[loop]->map_d, strlen(mtl->newmtl[loop]->map_d) );
            t_num1 += 1;
        }   

        if (mtl->newmtl[loop]->map_bump[0] != '\0'){
            t->texture_title[t_num1] = calloc(1, strlen(mtl->newmtl[loop]->map_bump) + 1);
            memcpy(t->texture_title[t_num1], mtl->newmtl[loop]->map_bump, strlen(mtl->newmtl[loop]->map_bump) );
            t_num1 += 1;
        }   

        if (mtl->newmtl[loop]->map_refl[0] != '\0'){
            t->texture_title[t_num1] = calloc(1, strlen(mtl->newmtl[loop]->map_refl) + 1);
            memcpy(t->texture_title[t_num1], mtl->newmtl[loop]->map_refl, strlen(mtl->newmtl[loop]->map_refl) );
            t_num1 += 1;
        }  

        loop += 1;
    }
    
    t_num1 = 0;

    for (size_t i = 0; i < t_num; i++){
        for(size_t j = i + 1; j < t_num; j++){
            
            if(strlen(t->texture_title[i]) == 0){
                break;
            }

            if( strncmp(t->texture_title[i], t->texture_title[j], strlen(t->texture_title[i])) == 0){
                memset(t->texture_title[j], 0, strlen(t->texture_title[j]) + 1);
                t_num1 += 1;
            }

        } 
    }
    
    t_num2 = t_num - t_num1;   
    if(t_num1 != 0){
        
        loop = 0;
        t_texture = calloc(t_num2, sizeof(char*));

        for (size_t i = 0; i < t_num; i++){
            if(strlen(t->texture_title[i]) != 0 ){
                t_texture[loop] = calloc(1, strlen(t->texture_title[i]) + 1);
                memcpy(t_texture[loop], t->texture_title[i], strlen(t->texture_title[i]));
                loop += 1;
            }
        }
        
        for (size_t i = 0; i < t_num; i++){
            free(t->texture_title[i]);
        }     

        free(t->texture_title);   
    }

    if(t_num1 == 0){
        t->t_len = t_num;         
    }else{
        t->t_len = t_num2;
        t->texture_title = t_texture;
    }
    t->t_id = calloc(t->t_len, sizeof(uint32_t));
    bool_model_pointer(t->t_id, "t->t_id init_model_texture fail");
    glGenTextures(t->t_len, t->t_id);

    // 纹理类型判断
    texture_type = GL_TEXTURE_2D;

    // 路径不能过长 1024 - 255 - 1
    str_len = strlen(path);
    if(str_len > IMG_PATH_LENGTH - TITLE_LENGTH - 1){
        puts("path too long");
        return;
    }
    
    // 生成纹理
    loop = 0;
    while (loop < t->t_len){

        memcpy(img_path, path, strlen(path));        
        memcpy(img_path + strlen(path), t->texture_title[loop], strlen(t->texture_title[loop]));

        init_texture_data(&t_d, texture_type, img_path);
        texture_object_bind_texture_data(t->t_id[loop], &t_d);

        glCheckError();
        bool yes = glIsTexture(t->t_id[loop]);
        if(yes == false){
            printf("fail Texture %d\n",t->t_id[loop]);
        }
    
        memset(img_path, 0, sizeof(img_path));

        loop += 1;
    }
    
}
static void bool_model_pointer(void *p, const char *str){
    
    if(p == NULL){
        printf("%s apply for memory fail\n", str);
        exit(0);
    }
} 
static void get_vertices_data(vertices_data *const v_d, const obj_model_object *const obj, uint32_t num){
    uint32_t loop = 0;
    // index_d 的步长
    uint32_t type_step = 0;
    index_type type = obj->index_d.index_type[num];
    size_t d_size = 0;
    uint32_t end_num = 0;

    // 记录array_v的下标
    uint32_t index_v = 0; 
    uint32_t index_vt = 0; 
    uint32_t index_vn = 0; 
    uint32_t index_vp = 0; 

    // 记录索引中的顶点属性位置
    uint32_t v_location = 0; 
    uint32_t vt_location = 0; 
    uint32_t vn_location = 0;     
    uint32_t vp_location = 0;     

    // 记录不同顶点属性的长度
    uint32_t array_v[V_NUM] = {0};
    uint32_t array_vt[VT_NUM] = {0};
    uint32_t array_vn[VN_NUM] = {0};
    uint32_t array_vp[VP_NUM] = {0};

    // 记录顶点属性的大小
    uint32_t v_step = 0; 
    uint32_t vt_step = 0; 
    uint32_t vn_step = 0;     
    uint32_t vp_step = 0;     
    uint32_t v_attribute_step = 0;

    if(obj->v_d.v_array_length > V_NUM){
        puts("v_d tanto");
        return;
    }

    if(obj->vt_d.v_array_length > VT_NUM){
        puts("vt_d tanto");
        return;
    }

    if(obj->vn_d.v_array_length > VN_NUM){
        puts("vn_d tanto");
        return;
    }

    if(obj->vp_d.v_array_length > VP_NUM){
        puts("vn_d tanto");
        return;
    }

    for (size_t i = 0; i < obj->v_d.v_array_length; i++){
        if (i == 0){
            array_v[i] = obj->v_d.v_array_each_size[i] / (sizeof(float) * obj->v_d.v_type[i]);
        }else{
            array_v[i] = obj->v_d.v_array_each_size[i] / (sizeof(float) * obj->v_d.v_type[i]) + array_v[i - 1];
        }
    }
    
    for (size_t i = 0; i < obj->vt_d.v_array_length; i++){
        if (i == 0){
            array_vt[i] = obj->vt_d.v_array_each_size[i] / (sizeof(float) * obj->vt_d.v_type[i]);
        }else{
            array_vt[i] = obj->vt_d.v_array_each_size[i] / (sizeof(float) * obj->vt_d.v_type[i]) + array_vt[i - 1];
        }
    }

    for (size_t i = 0; i < obj->vn_d.v_array_length; i++){
        if (i == 0){
            array_vn[i] = obj->vn_d.v_array_each_size[i] / (sizeof(float) * obj->vn_d.v_type[i]);
        }else{
            array_vn[i] = obj->vn_d.v_array_each_size[i] / (sizeof(float) * obj->vn_d.v_type[i]) + array_vn[i - 1];
        }
    }

    for (size_t i = 0; i < obj->vp_d.v_array_length; i++){
        if (i == 0){
            array_vp[i] = obj->vp_d.v_array_each_size[i] / (sizeof(float) * obj->vp_d.v_type[i]);
        }else{
            array_vp[i] = obj->vp_d.v_array_each_size[i] / (sizeof(float) * obj->vp_d.v_type[i]) + array_vp[i - 1];
        }
    }

    //uint32_t hi = get_array_data_location(5565, array_v, V_NUM);

    switch (type){
        case vertex:
            type_step = 1;
            break;
        case vertex_texture:
            type_step = 2;
            break;
        case vertex_normal:
            type_step = 2;
            break;   
        case vertex_texture_normal:
            type_step = 3;
            break;       
        default:
            puts("get_vertices_data error");
            exit(0);
            break;
    }

    if (type == vertex_texture_normal){

        v_location = obj->index_d.index_array[num][loop * type_step];
        vt_location = obj->index_d.index_array[num][loop * type_step + 1];
        vn_location = obj->index_d.index_array[num][loop * type_step + 2];

        index_v = get_array_data_location(v_location, array_v, V_NUM);
        index_vt = get_array_data_location(vt_location, array_vt, VT_NUM);
        index_vn = get_array_data_location(vn_location, array_vn, VN_NUM);
     
        v_step = obj->v_d.v_type[index_v];
        vt_step = 2;
        vn_step = obj->vn_d.v_type[index_vn];
        v_attribute_step = v_step + vt_step + vn_step;

        // 读取顶点数据        
        d_size = (obj->index_d.index_array_each_size[num] / (obj->index_d.index_step[num] * sizeof(uint32_t))) * (v_attribute_step * sizeof(false) * type_step);
        end_num = d_size / (sizeof(float) * v_attribute_step);

        new_vertices_data(v_d, d_size);

        // 设置顶点属性
        v_d->data_type = GL_FLOAT;
        v_d->data_use_mode = GL_STATIC_DRAW; 

        v_d->vertex_location = 0;  
        v_d->vertex_num = v_step;
        v_d->vertex_normalize = false;
        v_d->vertex_offset = 0; 

        v_d->texture_location = 2;  
        v_d->texture_num = vt_step;
        v_d->texture_normalize = false;
        v_d->texture_offset = (v_d->vertex_num + v_d->colors_num) * sizeof(float);

        v_d->normal_location = 3;
        v_d->normal_num = vn_step;
        v_d->normal_normalize = false;
        v_d->normal_offset = (v_d->vertex_num + v_d->colors_num + v_d->texture_num) * sizeof(float);

        v_d->vertices_stride = v_attribute_step * sizeof(float);

        // 循环读取顶点数据
        while (loop < end_num){
            v_location = obj->index_d.index_array[num][loop * type_step];
            vt_location = obj->index_d.index_array[num][loop * type_step + 1];
            vn_location = obj->index_d.index_array[num][loop * type_step + 2];

            index_v = get_array_data_location(v_location, array_v, V_NUM);
            index_vt = get_array_data_location(vt_location, array_vt, VT_NUM);
            index_vn = get_array_data_location(vn_location, array_vn, VN_NUM);

            get_obj_vertices_type_data(index_v, v_location - 1, &obj->v_d, (v_d->data + loop * v_attribute_step), array_v);
            get_obj_vertices_type_data(index_vt, vt_location - 1, &obj->vt_d, (v_d->data + loop * v_attribute_step + v_step), array_vt);
            get_obj_vertices_type_data(index_vn, vn_location - 1, &obj->vn_d, (v_d->data + loop * v_attribute_step + v_step + vt_step), array_vn);
            
            loop += 1;
        }
    }
    
    if (type == vertex){


    }
    
    if (type == vertex_texture){
  

    }

    if (type == vertex_normal){
   

    }    
}
static void free_model_texture_obj(model_texture_obj *m){
   
    if(m->t_len == 0){
        return;
    }

    for (size_t i = 0; i < m->t_len; i++){
        free(m->texture_title[i]);
    }
    
    free(m->texture_title);
    free(m->t_id);
    
    memset(m, 0, sizeof(model_texture_obj));
}
static void free_module_obj(module_obj *m, uint32_t num){

    module_obj *p = m;
    for (size_t i = 0; i < num; i++){
        glDeleteBuffers(1, &p->vbo);
        glDeleteVertexArrays(1, &p->vao);
        glCheckError();
        p += 1;
    }
    
    free(m);
    m = NULL;
}
static uint32_t get_array_data_location(uint32_t num, const uint32_t *const arr, uint32_t arr_len){

    uint32_t loop = 0;

    while (loop < arr_len && arr[loop] != 0){
        if(num <= arr[loop]){
            return loop;
        }
        loop += 1;
    }
    
    return 0;
}
static void get_obj_vertices_type_data(uint32_t index, uint32_t location, const v_general_data *const v, float *const f, const uint32_t *const arr){

    uint32_t loop = 0;
    uint32_t location_1 = 0;

    if (index == 0){
        while (loop < v->v_type[index]){
            f[loop] = v->v_array[index][(location * v->v_type[index]) + loop];
            loop += 1;
        }
    }else{
        location_1 = location - arr[index - 1];
        while (loop < v->v_type[index]){
            f[loop] = v->v_array[index][(location_1 * v->v_type[index]) + loop];
            loop += 1;
        }
    }
}

void free_model_obj(model_obj *model){
    free_module_obj(model->module_obj, model->module_num);
    free_model_texture_obj(&model->texture_obj);
    memset(model, 0, sizeof(model_obj));
}
void render_model(model_obj *m, shader *s){

    uint32_t loop = 0;
    module_obj *m_obj = m->module_obj;

    //uniform_set_vec3(s->shader_id, "light_position", m->camera->camera_location);
    uniform_set_vec3(s->shader_id, "light_position", m->light_position);
    uniform_set_vec3(s->shader_id, "view_pos", m->camera->camera_location);

    while (loop < m->module_num){ //m->module_num

        module_render_activate_texture(m_obj, m->texture_obj, s);

        glBindVertexArray(m_obj->vao);
        glDrawArrays(GL_TRIANGLES, 0, m_obj->index_number);   

        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);

        loop += 1;
        m_obj += 1;
    }

}
void init_model_obj(model_obj *const model, const obj_model_object *const obj, const mtl_model_object *const mtl, const char *texture_path, clip_space *clip_space, camera_object *camera){

    // 初始化模型数据
    memset(model, 0, sizeof(model_obj));
    
    // 判断是否有渲染的模块
    if(obj->index_d.index_array_length < 1){
        puts("index_array_length less 1");
        return;
    }
    
    model->light_position[0] = 0;
    model->light_position[1] = obj->v_d.v_array[0][1];
    model->light_position[2] = obj->v_d.v_array[0][2] + 4.0f;

    model->camera = camera;
    model->clip_space = clip_space;
    init_world_space(&model->world_space);
    
    // 初始化渲染模块
    model->module_num = obj->index_d.index_array_length;
    model->module_obj = calloc(obj->index_d.index_array_length, sizeof(module_obj));
    init_module_obj(model->module_obj, obj, mtl);
    if(model->module_obj == NULL){
        puts("model of module init fail");
        return;        
    }
    glCheckError();


    // 纹理初始化
    init_model_texture(&model->texture_obj, mtl, texture_path);
    if (model->texture_obj.t_len == 0){
        puts("model of texture init fail");
        return;
    }
    glCheckError();

}
void path_init_model_obj(model_obj *const model, const char *c_obj, const char *c_mtl, const char *texture_path, clip_space *clip_space, camera_object *camera){

    obj_model_object obj;   
    mtl_model_object mtl;
    
    init_obj_model_object(&obj, c_obj);
    init_mtl_model_object(&mtl, c_mtl);    

    init_model_obj(model, &obj, &mtl, texture_path, clip_space, camera);

    free_obj_model_object(&obj);
    free_mtl_model_object(&mtl);
}
