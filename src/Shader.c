#include "Shader.h"

void Shader_load(Shader*, const char*);
void Shader_init(Shader*, const char*);
void Shader_destroy(Shader*);
void FragmentShader_createShader(FragmentShader* shader);
void VertexShader_createShader(VertexShader* shader);

void FragmentShader_init(FragmentShader* shader, const char* filePath)
{
    Shader_init((Shader*) shader, filePath);
    FragmentShader_createShader(shader);
}

void FragmentShader_destroy(FragmentShader* shader)
{
    Shader_destroy((Shader*) (shader));
}

void VertexShader_init(VertexShader* shader, const char* filePath)
{
    Shader_init((Shader*) shader, filePath);
    VertexShader_createShader(shader);
}

void VertexShader_destroy(VertexShader* shader)
{
    Shader_destroy((Shader*) (shader));
}

void FragmentShader_createShader(FragmentShader* shader)
{
    GLint shaderCompiled;
    shader->id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader->id, 1, &shader->shaderSrc, 0);
    glCompileShader(shader->id);

    glGetShaderiv(shader->id, GL_COMPILE_STATUS, &shaderCompiled);

    if (shaderCompiled == GL_FALSE)
    {
        int infologLength = 0;

        int charsWritten = 0;
        char *infoLog;

        glGetShaderiv(shader->id, GL_INFO_LOG_LENGTH, &infologLength);
        if (infologLength > 0)
        {
            infoLog = (char *) malloc(infologLength);
            glGetShaderInfoLog(shader->id, infologLength, &charsWritten, infoLog);
            Log_errorf("The shader %s failed to compile: %s\n", shader->filePath, infoLog);
            free(infoLog);
        }
        else
        {
            Log_errorf("The shader %s failed to compile\n", shader->filePath);
        }
    }
}

void VertexShader_createShader(VertexShader* shader)
{
    GLint shaderCompiled;
    shader->id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader->id, 1, &shader->shaderSrc, 0);
    glCompileShader(shader->id);

    glGetShaderiv(shader->id, GL_COMPILE_STATUS, &shaderCompiled);

    if (shaderCompiled == GL_FALSE)
    {
        int infologLength = 0;

        int charsWritten = 0;
        char *infoLog;

        glGetShaderiv(shader->id, GL_INFO_LOG_LENGTH, &infologLength);
        if (infologLength > 0)
        {
            infoLog = (char *) malloc(infologLength);
            glGetShaderInfoLog(shader->id, infologLength, &charsWritten, infoLog);
            Log_errorf("The shader %s failed to compile: %s\n", shader->filePath, infoLog);
            free(infoLog);
        }
        else
        {
            Log_errorf("The shader %s failed to compile\n", shader->filePath);
        }
    }
}

char* readFile(const char* filename)
{
    FILE *stream = NULL;
    size_t size = 0;
    char *contents = NULL;
    int fileSize = 0;
    stream = fopen(filename, "r");
    if (stream == 0)
    {
        Log_errorf("Unable to load %s\n", filename);
        exit(2);
    }

    fseek(stream, 0L, SEEK_END);
    fileSize = ftell(stream);
    fseek(stream, 0L, SEEK_SET);
    contents = malloc(fileSize + 1);
    assert(contents);
    size = fread(contents, 1, fileSize + 1, stream);
    contents[size] = 0; /* Add terminating zero. */
    fclose(stream);
    return contents;
}

void Shader_load(Shader* shader, const char* filePath)
{
    char* filePathTmp = malloc(sizeof(char) * strlen(filePath));
    char* shaderSrcTmp = readFile(filePath);
    assert(filePathTmp);
    filePathTmp[0] = '\0';
    strcpy(filePathTmp, filePath);
    shader->filePath = &filePathTmp[0];
    shader->shaderSrc = &shaderSrcTmp[0];
}

void Shader_init(Shader* shader, const char* filePath)
{
    Shader_load(shader, filePath);
}

void Shader_destroy(Shader* shader)
{
    glDeleteShader(shader->id);
    free((char*) shader->shaderSrc);
}
