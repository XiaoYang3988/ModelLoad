#include "read_model_file.h"

/**
 *  v 几何体顶点(Geometric vertices)
 *  vt 贴图坐标点(Texture vertices)
 *  vn 顶点法线(Vertex normals)
 *  vp 参数空格顶点 (Parameter space vertices)
 *  index 绘制的索引顺序
 */

// 文本拷贝到申请的堆内存中
static char *read_file_to_char(const char *path);
// 拷贝数据
static const char *v_loop_data(const char *location, const char str, const char str_1, size_t size, void *const data, size_t type);
// 数据步长
static uint32_t data_step(const char *location_1);
// 每行数据长度
static uint32_t data_length(const char *location, const char str, const char str_1);
// 判断指针
static void bool_pointer(void *p, const char *str);
// 释放v通用数据
static void free_v_general_data(v_general_data *const v);
// 释放顶点索引数据内存
static void free_index_data(index_data *const v);
// 获取索引步长
static uint32_t index_data_step(const char *location, index_type *const type);
// 获取索引数据总行数
static uint32_t index_data_length(const char *location, const char str, const char str_1);
// 拷贝索引数据
static const char *read_index_loop_data(const char *location, const char str, const char str_1, size_t size, void *const data, index_type type);
// 获取文件标题
static void get_obj_title(char *const title, const char* path);
// 查找字符串返回字符串所在的首位置
static const char* get_string_head_location(const char *str, const char *file);
// 获取obj中对应的mtllib_title
static void get_obj_mtllib_title(char (*data)[TITLE_LENGTH], char const *title, const char *file);
// 获取每行mtl中对应标签的一个float
static bool mtl_get_one_float_string(float *const f, const char *location, const char *name);
// 获取每行mtl中对应标签的三个float
static bool mtl_get_three_float_string(float *const f, const char *location, const char *name);
// 获取每行mtl中对应标签的字符串文件标题
static bool mtl_get_char_string(char *const f, const char *location, const char *name);
// 获取每行mtl中的标题
static void get_line_english_char(char *const str, char const *location);
// newmtl
static void get_mtl_newmtl_title(char *data, char const *title, const char *file);
static void get_v_general_data_all_data(v_general_data *obj, const char *location, const char *str);
static void get_index_data_all_data(index_data *obj, const char *location, const char* str);
static uint32_t get_v_general_data_num(const char *location, const char* str);
static uint32_t get_index_data_num(const char *location, const char* str);

static void get_v_general_data_all_data(v_general_data *obj, const char *location, const char* str){
    const char *p = location;
    uint32_t step = 0;
    uint32_t line_num = 0;
    uint32_t v_num = get_v_general_data_num(p, str);

    obj->v_array_length = v_num;  
    obj->v_array = calloc(v_num ,sizeof(float*));
    obj->v_array_each_size = calloc(v_num, sizeof(size_t));
    obj->v_type = calloc(v_num, sizeof(uint32_t));   

    bool_pointer(obj->v_array, "v_general_data_loop is true 1");
    bool_pointer(obj->v_array_each_size, "v_general_data_loop is true 2");
    bool_pointer(obj->v_type, "v_general_data_loop is true 3");   

    for (size_t i = 0; i < v_num; i++){
        
        step = data_step(p);
        line_num = data_length(p, str[0], str[1]);

        obj->v_type[i] = step;
        obj->v_array_each_size[i] = line_num * step * sizeof(float);
        obj->v_array[i] = calloc(obj->v_array_each_size[i] ,sizeof(char));  
        bool_pointer(obj->v_array[i], "v_general_data_loop is true 4");

        p = v_loop_data(p, str[0], str[1], obj->v_array_each_size[i], obj->v_array[i], sizeof(float));
        p = get_string_head_location(str, p);
    }
}
static void get_index_data_all_data(index_data *obj, const char *location, const char* str){
    
    const char *p = location;
    uint32_t step = 0;
    uint32_t v_num = get_index_data_num(p, str);
    const char *usemtl = NULL;
    const char *usemtl_1 = NULL;
    uint32_t usemtl_size = 0;

    obj->index_array_length = v_num;
    obj->index_array = calloc(v_num ,sizeof(int32_t*));
    obj->index_array_each_size = calloc(v_num, sizeof(size_t));
    obj->index_step = calloc(v_num, sizeof(uint32_t));  
    obj->plane_v_num = calloc(v_num, sizeof(uint32_t));  
    obj->index_type = calloc(v_num, sizeof(index_type));  
    obj->index_usemtl_title = calloc(v_num, sizeof(char*));

    bool_pointer(obj->index_array, "index_data_loop is true 1");
    bool_pointer(obj->index_array_each_size, "index_data_loop is true 2");
    bool_pointer(obj->index_step, "index_data_loop is true 3"); 
    bool_pointer(obj->plane_v_num, "index_data_loop is true 4"); 
    bool_pointer(obj->index_type, "index_data_loop is true 5"); 
    bool_pointer(obj->index_usemtl_title, "index_data_loop is true 7");

    for (size_t i = 0; i < v_num; i++){

        usemtl = strchr(p, ' ') + 1;
        usemtl_1 = strchr(p, '\n');
        usemtl_size = usemtl_1 - usemtl;

        obj->index_usemtl_title[i] = calloc(usemtl_size + 1, sizeof(char));
        bool_pointer(obj->index_usemtl_title[i], "index_data_loop is true 8");
        memcpy(obj->index_usemtl_title[i], usemtl, usemtl_size);

        p = get_string_head_location("f ", p);

        step = index_data_step(p, obj->index_type + i);
        obj->index_step[i] = step;
        obj->plane_v_num[i] = step / 3;

        obj->index_array_each_size[i] = index_data_length(p, 'f', ' ') * obj->index_step[i] * sizeof(int32_t);
        obj->index_array[i] = calloc(obj->index_array_each_size[i] ,sizeof(char));  
        bool_pointer(obj->index_array[i], "index_data_loop is true 6"); 

        p = read_index_loop_data(p, 'f', ' ', obj->index_array_each_size[i], obj->index_array[i], *(obj->index_type + i));
        p = get_string_head_location(str, p);
    }
    
}
static uint32_t get_v_general_data_num(const char *location, const char* str){

    uint32_t v_num = 0;

    while (location != NULL){
        if ( !(*location == str[0] && *(location + 1) == str[1]) ){
            v_num += 1;
            location = get_string_head_location(str, location);
            if (location == NULL){
                break;
            }
        }
        location = strchr(location, '\n');
        location++;
    }

    return v_num;
}
static uint32_t get_index_data_num(const char *location, const char* str){

    uint32_t v_num = 0;

    while (location != NULL){
        location = get_string_head_location(str, location);
        if(location != NULL){
            v_num += 1;
            location = strchr(location, '\n');
            location++;            
        }else{
            return v_num;
        }
    }

    return 0;
}
static void get_obj_mtllib_title(char (*data)[TITLE_LENGTH], char const *title, const char *file){

    char const *string = get_string_head_location("mtllib", file);
    char const *string_1 = NULL;
    uint32_t location = 0;
    uint32_t num = 0;

    if(string == NULL){
        printf("obj file no mtllib\n");
        return;
    }

    while (string != NULL){

        string = strchr(string, ' ') + 1;
        string_1 = strchr(string, '\n');

        location = string_1 - string;

        if(location > TITLE_LENGTH - 1){
            printf("get_obj_mtllib_title TITLE_LENGTH step error \n");
            return;
        }

        memcpy(data + num, string, location);

        string = get_string_head_location("mtllib", string_1 + 1);

        num += 1;

        if(num > MTLLIB_SIZE){
            printf("mtllib num greater than MTLLIB_SIZE\n");
            return;
        }
    }
    
}
static void get_obj_title(char *const title, const char* path){
    
    if(strlen(path) > TITLE_LENGTH){
        return;
    }

    char *name = NULL;
    if(strrchr(path, '\\')){
        name = strrchr(path, '\\') + 1;
        strcpy(title, name);
    }else if(strrchr(path, '/')){
        name = strrchr(path, '/') + 1;
        strcpy(title, name);
    }
}
static const char* get_string_head_location(const char *str, const char *file){

    if(str == NULL || file == NULL){
        printf("str and file unable is null\n");
        return NULL;        
    }

    uint32_t srt_len = strlen(str);
    uint32_t num = 0;

    if(srt_len > strlen(file)){
        //printf("The searched character is greater than the searched character\n");
        return NULL;
    }

    while (*file != '\0'){

        while (*(str + num) == *(file + num)){
            if(srt_len == 1){
                return file;
            }
            num += 1;
            if(num == srt_len){
                return file;
            }
        }
        
        num = 0;
        file += 1;
    }
    return NULL;
}
static const char* read_index_loop_data(const char *location, const char str, const char str_1, size_t size, void *const data, index_type type){

    uint32_t num = 0;
    int32_t f_data = 0;
    const char *location_1 = location;
    const char *location_2 = location;
    size_t size_loop_data = 0;
    const char *p_end = NULL;

    if(type == vertex){

        return NULL;
    }

    if(type == vertex_texture || type == vertex_texture_normal){

        while(*location == str && *(location + 1) == str_1){

            location += 2;
            location_2 = location;

            while(*location != '\n'){
                strtod(location, (char **)&p_end);
                if (location == p_end){
                    break;
                }else{
                    location = p_end;
                    location++;
                    num++;
                }
            }

            location = location_2;
            location = strchr(location, '\n');
            location++;

            if(*location == 's'){
                location = strchr(location, '\n');
                location++;
            }
        }

        size_loop_data = num * sizeof(int32_t);

        if(size_loop_data != size){
            printf("vertex_texture read v_data format error, line size error, check on obj file index_loop_data\n");
            return NULL;
        }

        location = location_1;
        num = 0;
        p_end = NULL;

        while(*location == str && *(location + 1) == str_1){

            location += 2;
            location_2 = location;

            while(*location != '\n'){
                f_data = strtod(location, (char **)&p_end);
                if (location == p_end){
                    break;
                }else{
                    memcpy(( (char*)data + (num * sizeof(int32_t))), &f_data, sizeof(int32_t) );
                    location = p_end;
                    num++;
                    location++;
                }
            }

            location = location_2;
            location = strchr(location, '\n');
            location++;

            if(*location == 's'){
                location = strchr(location, '\n');
                location++;
            }
        }

        return location;
    }

    if(type == vertex_normal){
        return NULL;
    }

    return NULL;
}
static uint32_t index_data_length(const char *location, const char str, const char str_1){

    uint32_t length = 0;
    while(*location == str && *(location + 1) == str_1){
        length++;
        location = strchr(location, '\n');
        if(location == NULL){
            return length;
        }

        location++;

        if(*location == 's'){
            location = strchr(location, '\n');
            location++;
        }
    }
    return length;
}
static uint32_t index_data_step(const char *location, index_type *const t){

    uint32_t p = 0;
    uint32_t step = 0;
    const char *location_1 = NULL;
    const char str[LINE_LENGTH] = {0};
    char* p_end = NULL;

    location = strchr(location, ' ');
    location_1 = location;
    location = strchr(location, '\n');
    p = location - location_1;
    if(p > LINE_LENGTH - 1){
        printf("index_data_step LINE_LENGTH step error \n");
        return 0;
    }

    memcpy((char*)str, location_1, p);
    location = strchr(str, '/');
    location_1 = str;
    
    if(location == NULL){
        *t = vertex;
        return 3;
    }

    if(*(location + 1) == '/'){
        *t = vertex_normal;
        return 6;
    }
    
    while(*location_1 != '\n'){
        strtod(location_1, &p_end);
        if (location_1 == p_end){
            break;
        }else{
            location_1 = p_end;
            location_1 += 1;
            step += 1;
        }
    }
    
    if(step == 6){
        *t = vertex_texture;
    }else if(step == 9){
        *t = vertex_texture_normal;
    }

    return step;
}
static const char *v_loop_data(const char *location, const char str, const char str_1, size_t size, void *const data, size_t type){

    uint32_t num = 0;
    float f_data = 0;
    const char *location_1 = location;
    const char *location_2 = location;
    size_t size_loop_data = 0;
    const char* p_end = NULL;

    while(*location == str && *(location + 1) == str_1){

        location += 2;
        location_2 = location;
        while (1){
            strtod(location, (char **)&p_end);
            if (location == p_end){
                break;
            }else{
                location = p_end;
                num += 1;
            }
        }
    
        location = location_2;
        location = strchr(location, '\n');
        location += 1;
    }

    size_loop_data = num * sizeof(float);
    if(size_loop_data != size){
        printf("read v_data format error, line size error, check on obj file\n");
        return NULL;
    }

    location = location_1;
    num = 0;

    while(*location == str && *(location + 1) == str_1){

        location += 2;
        location_2 = location;
        while (1){
            f_data = strtod(location, (char **)&p_end);
            if (location == p_end){
                break;
            }else{
                location = p_end;
                memcpy( ((char*)data + (num * type) ), &f_data, type);
                num += 1;
            }
        }

        location = location_2;
        location = strchr(location, '\n');
        location += 1;
    }

    return location;
}
static uint32_t data_length(const char *location, const char str, const char str_1){

    uint32_t length = 0;
    while(*location == str && *(location + 1) == str_1){
        length++;
        location = strchr(location, '\n');
        if(location == NULL){
            return length;
        }
        location++;
    }
    return length;
}
static uint32_t data_step(const char *location){

    uint32_t step = 0;
    location = strchr(location, ' ');
    char* p_end = NULL;
    while(*location != '\n'){
        strtod(location, &p_end);
        if (location == p_end){
            break;
        }else{
            location = p_end;
        }
        step += 1;
    }

    return step;
}
static void bool_pointer(void *p, const char *str){

    if(p == NULL){
        printf("%s apply for memory fail\n", str);
        exit(0);
    }
} 
static char *read_file_to_char(const char* path){

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
static void free_v_general_data(v_general_data *const v){

    uint32_t i = 0;

    if(v->v_array_length == 0){
        return;
    }

    while (i < v->v_array_length){
        free(v->v_array[i]);
        i += 1;
    }

    free(v->v_array);
    free(v->v_type);
    free(v->v_array_each_size);
    memset(v, 0, sizeof(v_general_data));
}
static void free_index_data(index_data *const v){

    uint32_t i = 0;
    
    if(v->index_array_length == 0){
        return;
    }

    while (i < v->index_array_length){
        free(v->index_array[i]);
        free(v->index_usemtl_title[i]);
        i += 1;
    }
    free(v->index_step);
    free(v->index_array);
    free(v->index_array_each_size);
    free(v->index_type);
    free(v->plane_v_num);
    free(v->index_usemtl_title);
    memset(v, 0, sizeof(index_data));
}
static void get_mtl_newmtl_title(char *data, char const *title, const char *file){

    char const *string = file;
    char const *string_1 = NULL;
    uint32_t location = 0;

    if(string == NULL){
        printf("mtl file no mtllib\n");
        return;
    }

    string = strchr(string, ' ') + 1;
    string_1 = strchr(string, '\n');

    location = string_1 - string;

    if(location > TITLE_LENGTH - 1){
        printf("get_mtl_newmtl_title TITLE_LENGTH step error \n");
        return;
    }

    memcpy(data, string, location);
}
static void get_line_english_char(char *const str, char const *location){

    uint32_t num = 0;

    while (*location != '\n' && *location != '\0'){

        if(isalpha(*location) != 0){
            break;
        }
        location += 1;
    }

    while (*location != '\n' && *location != '\0'){

        if(isalpha(*location) != 0 || *location == '_'){
            str[num] = *location;
            num += 1;
            location += 1;
        }else{
            break;
        }
    }
}
static bool mtl_get_one_float_string(float *const f, const char *location, const char *name){
 
    char const *string = NULL;
    uint32_t size = 0;
    char arr[LINE_LENGTH] = {0};

    string = strchr(location, '\n');
    size = string - location;
    if(size > LINE_LENGTH - 1){
        printf("line data more than 128\n");
        return false;
    }
    
    memcpy(arr, location, size);
    string = get_string_head_location(name, arr);
    
    if(string == NULL){
        return false;
    }

    string += strlen(name);
    *f = strtod(string, (char **)&string);
    
    return true;
}
static bool mtl_get_three_float_string(float *const f, const char *location, const char *name){
 
    char const *string = NULL;
    uint32_t size = 0;
    char arr[LINE_LENGTH] = {0};

    string = strchr(location, '\n');
    size = string - location;
    if(size > LINE_LENGTH - 1){
        printf("line data more than 128\n");
        return false;
    }
    
    memcpy(arr, location, size);
    string = get_string_head_location(name, arr);
    
    if(string == NULL){
        return false;
    }

    string += strlen(name);
    
    for (size_t i = 0; i < MTL_ARRAY_FLOAT; i++){
        f[i] = strtod(string, (char **)&string);
    }
    
    return true;
}
static bool mtl_get_char_string(char *const title, const char *location, const char *name){
 
    char const *string = NULL;
    uint32_t size = 0;
    char arr[TITLE_LENGTH] = {0};

    string = strchr(location, '\n');
    size = string - location;
    if(size > TITLE_LENGTH - 1){
        printf("line data more than 255\n");
        return false;
    }
    
    memcpy(arr, location, size);
    string = get_string_head_location(name, arr);
    
    if(string == NULL){
        return false;
    }

    string = strchr(string, ' ') + 1;

    if(strrchr(string, '\\')){
        string = strrchr(string, '\\') + 1;
        strcpy(title, string);
    }else if(strrchr(string, '/')){
        string = strrchr(string, '/') + 1;
        strcpy(title, string);
    }else{
        strcpy(title, string);
    }
    
    
    return true;
}

void free_obj_model_object(obj_model_object *const obj){
    free_v_general_data(&obj->v_d);
    free_v_general_data(&obj->vn_d);
    free_v_general_data(&obj->vt_d);
    free_v_general_data(&obj->vp_d);
    free_index_data(&obj->index_d);
    memset(obj, 0, sizeof(obj_model_object));
}
void init_obj_model_object(obj_model_object *const obj, const char* path){

    char *file = NULL;
    const char *location = NULL;
    const char obj_model_name[obj_model_object_num][10] = {"v ","vt","vn","vp","usemtl"};

    file = read_file_to_char(path);

    if(file == NULL){
        memset(obj, 0, sizeof(obj_model_object));
        return;
    }

    location = file;

    memset(obj, 0, sizeof(obj_model_object));

    location = get_string_head_location("mtllib", file);
    location = strchr(location, '\n');
    location += 1;

    for (size_t i = 0; i < obj_model_object_num; i++){

        location = get_string_head_location(obj_model_name[i], file);
    
        if (strncmp(obj_model_name[i], "v ", strlen("v ")) == 0 && location != NULL){
            get_v_general_data_all_data(&obj->v_d, location, obj_model_name[i]);
        }else if (strncmp(obj_model_name[i], "vt", strlen("vt")) == 0 && location != NULL){
            get_v_general_data_all_data(&obj->vt_d, location, obj_model_name[i]);
        }else if (strncmp(obj_model_name[i], "vn", strlen("vn")) == 0 && location != NULL){
            get_v_general_data_all_data(&obj->vn_d, location, obj_model_name[i]);
        }else if (strncmp(obj_model_name[i], "vp", strlen("vp")) == 0 && location != NULL){
            get_v_general_data_all_data(&obj->vp_d, location, obj_model_name[i]);
        }else if (strncmp(obj_model_name[i], "usemtl", strlen("usemtl")) == 0 && location != NULL){
            get_index_data_all_data(&obj->index_d, location, obj_model_name[i]);
        }
    }
    
    get_obj_title(obj->title, path);
    get_obj_mtllib_title(obj->mtllib_title, "mtllib", file);

    free(file);
}

void init_mtl_model_object(mtl_model_object *const mtl, const char* path){

    char *file = NULL;

    const char *location = NULL;
    char string[LINE_LENGTH] = {0};
    uint32_t num = 0;

    memset(mtl, 0, sizeof(mtl_model_object));

    file = read_file_to_char(path);   

    if(file == NULL){
        return;
    }

    location = file;

    while (location != NULL){
        location = get_string_head_location("newmtl", location);
        if(location != NULL){
            mtl->newmtl_array_len += 1;
            location = strchr(location, '\n');
            location += 1;   
        }
    }
    
    location = file;
    
    mtl->newmtl = calloc(mtl->newmtl_array_len, sizeof(mtl_general_data**));
    
    for (size_t i = 0; i < mtl->newmtl_array_len; i++){
        mtl->newmtl[i] = calloc(1, sizeof(mtl_general_data));
    }
    
    location = get_string_head_location("newmtl", location);
    get_mtl_newmtl_title(mtl->newmtl[num]->newmtl_title, "newmtl", location);
    location = strchr(location, '\n');
    location += 1;

    while(location != NULL){

        get_line_english_char(string, location);

        if(strncmp(string, "Ns", strlen("Ns")) == 0){
            mtl_get_one_float_string(&mtl->newmtl[num]->Ns, location, "Ns");  

            memset(string, 0, sizeof(string));
            location = strchr(location, '\n');
            location += 1;                   
        }else if(strncmp(string, "Ni", strlen("Ni")) == 0){
            mtl_get_one_float_string(&mtl->newmtl[num]->Ni, location, "Ni"); 

            memset(string, 0, sizeof(string));
            location = strchr(location, '\n');
            location += 1;                   
        }else if(strncmp(string, "d", strlen("d")) == 0){
            mtl_get_one_float_string(&mtl->newmtl[num]->d, location, "d"); 

            memset(string, 0, sizeof(string));
            location = strchr(location, '\n');
            location += 1;                      
        }else if(strncmp(string, "Tr", strlen("Tr")) == 0){
            mtl_get_one_float_string(&mtl->newmtl[num]->Tr, location, "Tr");  

            memset(string, 0, sizeof(string));
            location = strchr(location, '\n');
            location += 1; 
        }else if(strncmp(string, "illum", strlen("illum")) == 0){
            mtl_get_one_float_string(&mtl->newmtl[num]->illum, location, "illum");

            memset(string, 0, sizeof(string));
            location = strchr(location, '\n');
            location += 1;   
        }else if(strncmp(string, "sharpness", strlen("sharpness")) == 0){
            mtl_get_one_float_string(&mtl->newmtl[num]->sharpness, location, "sharpness");

            memset(string, 0, sizeof(string));
            location = strchr(location, '\n');
            location += 1;   
        }else if(strncmp(string, "Tf", strlen("Tf")) == 0){
            mtl_get_three_float_string(mtl->newmtl[num]->Tf, location, "Tf");   

            memset(string, 0, sizeof(string));
            location = strchr(location, '\n');
            location += 1; 
        }else if(strncmp(string, "Ka", strlen("Ka")) == 0){
            mtl_get_three_float_string(mtl->newmtl[num]->Ka, location, "Ka"); 

            memset(string, 0, sizeof(string));
            location = strchr(location, '\n');
            location += 1; 
        }else if(strncmp(string, "Kd", strlen("Ni")) == 0){
            mtl_get_three_float_string(mtl->newmtl[num]->Kd, location, "Kd"); 

            memset(string, 0, sizeof(string));
            location = strchr(location, '\n');
            location += 1; 
        }else if(strncmp(string, "Ks", strlen("Ks")) == 0){
            mtl_get_three_float_string(mtl->newmtl[num]->Ks, location, "Ks");  

            memset(string, 0, sizeof(string));
            location = strchr(location, '\n');
            location += 1; 
        }else if(strncmp(string, "Ke", strlen("Ni")) == 0){
            mtl_get_three_float_string(mtl->newmtl[num]->Ke, location, "Ke"); 

            memset(string, 0, sizeof(string));
            location = strchr(location, '\n');
            location += 1; 
        }else if(strncmp(string, "map_Ka", strlen("map_Ka")) == 0){
            mtl_get_char_string(mtl->newmtl[num]->map_Ka, location, "map_Ka");

            memset(string, 0, sizeof(string));
            location = strchr(location, '\n');
            location += 1; 
        }else if(strncmp(string, "map_Kd", strlen("map_Kd")) == 0){
            mtl_get_char_string(mtl->newmtl[num]->map_Kd, location, "map_Kd");

            memset(string, 0, sizeof(string));
            location = strchr(location, '\n');
            location += 1; 
        }else if(strncmp(string, "map_d", strlen("map_d")) == 0){
            mtl_get_char_string(mtl->newmtl[num]->map_d, location, "map_d");

            memset(string, 0, sizeof(string));
            location = strchr(location, '\n');
            location += 1; 
        }else if(strncmp(string, "map_bump", strlen("map_bump")) == 0 || strncmp(string, "map_Bump", strlen("map_Bump")) == 0){
            mtl_get_char_string(mtl->newmtl[num]->map_bump, location, "map_bump");
            mtl_get_char_string(mtl->newmtl[num]->map_bump, location, "map_Bump");

            memset(string, 0, sizeof(string));
            location = strchr(location, '\n');
            location += 1; 
        }else if(strncmp(string, "map_refl", strlen("map_refl")) == 0 || strncmp(string, "map_Refl", strlen("map_Refl")) == 0){
            mtl_get_char_string(mtl->newmtl[num]->map_refl, location, "map_refl");
            mtl_get_char_string(mtl->newmtl[num]->map_refl, location, "map_Refl");

            memset(string, 0, sizeof(string));
            location = strchr(location, '\n');
            location += 1; 
        }else if(strncmp(string, "map_Ks", strlen("map_Ks")) == 0){
            mtl_get_char_string(mtl->newmtl[num]->map_Ks, location, "map_Ks");

            memset(string, 0, sizeof(string));
            location = strchr(location, '\n');
            location += 1; 
        }else if(strncmp(string, "map_Ns", strlen("map_Ns")) == 0){
            mtl_get_char_string(mtl->newmtl[num]->map_Ns, location, "map_Ns");

            memset(string, 0, sizeof(string));
            location = strchr(location, '\n');
            location += 1; 
        }else{
            location = get_string_head_location("newmtl", location);
            if(location == NULL){
                break;
            }
            memset(string, 0, sizeof(string));
            num += 1;
            get_mtl_newmtl_title(mtl->newmtl[num]->newmtl_title, "newmtl", location);
            location = strchr(location, '\n');
            location += 1;
        }

    }

    free(file);
}
void free_mtl_model_object(mtl_model_object *const mtl){

    uint32_t num = 0;

    if (mtl->newmtl_array_len == 0){
        return;
    }
    
    while (num < mtl->newmtl_array_len){
        free(mtl->newmtl[num]);
        num += 1;
    }
    
    free(mtl->newmtl);
    memset(mtl, 0, sizeof(mtl_model_object));
}