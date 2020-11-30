#version 330     
out vec4 FragColor;

// 通用结构体
struct general_reflex {
    sampler2D   texture;    
    vec3 d;
};

// 环境反射
struct ambient_reflex {
    general_reflex ka;

    bool off;
};
// 漫反射
struct diffuse_reflex {
    general_reflex kd;

    bool off;
}; 
// 镜反射
struct specular_reflex_ks {
    general_reflex ks;

    float shininess;
    bool off;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform ambient_reflex ambient_obj; 
uniform diffuse_reflex diffuse_obj; 
uniform specular_reflex_ks specular_ks_obj; 

uniform vec3 view_pos;  // 观察点位置
uniform vec3 light_position;    // 光源位置

uniform sampler2D map_ns;   // 消隐指数指定标量纹理
uniform sampler2D map_d;    // 消隐指数指定标量纹理
uniform sampler2D map_bump; // 凹凸纹理
uniform sampler2D map_refl; // 反射贴图(材质的反射属性)

uniform float Ns;   // 反射指数描述
uniform float Ni;   // 折射值描述
uniform float Tr;   //    
uniform float d;    // 渐隐指数描述
uniform float illum; // 光照模型描述格式
uniform float sharpness;    // 清晰度描述  

uniform vec3 Ka; // 材质的环境光（ambient color）
uniform vec3 Kd; // 散射光（diffuse color）
uniform vec3 Ks; // 镜面光（specular color）
uniform vec3 Ke; // 放射光（emissive color）
uniform vec3 Tf; // 滤光透射率

uniform bool ns_off;
uniform bool d_off;
uniform bool bump_off;
uniform bool refl_off;
void main(){   

    vec3 ambient; // 环境光照
    vec3 diffuse; // 漫反射光照
    vec3 specular; // 镜面光照
    vec3 result; // 片段着色结果

    vec3 norm = normalize(Normal); // 标准化法线

    // 漫反射
    vec3 lightDir = normalize(light_position - FragPos); // 朝向光源的标准向量
    float diff = max(dot(norm, lightDir), 0.0);
    
    // 镜反射
    vec3 viewDir = normalize(view_pos - FragPos); // 朝向观察点的标准向量
    vec3 reflectDir = reflect(-lightDir, norm); // 反射向量
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specular_ks_obj.shininess);
    
    if(ambient_obj.off == true){
        ambient = ambient_obj.ka.d * texture(ambient_obj.ka.texture, TexCoords).rgb;
    }

    if(diffuse_obj.off == true){
        diffuse = diffuse_obj.kd.d * diff * texture(diffuse_obj.kd.texture, TexCoords).rgb;  
    }

    if(specular_ks_obj.off == true){
        specular = specular_ks_obj.ks.d * spec * texture(specular_ks_obj.ks.texture, TexCoords).rgb;  
    }

    result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
