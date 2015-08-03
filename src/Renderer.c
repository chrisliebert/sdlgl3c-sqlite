#include "Renderer.h"
#include "sqlite3.h"

int numErrors = 0;
void _checkForGLError(const char *file, int line)
{
    GLenum err = glGetError();
    while (err != GL_NO_ERROR)
    {
        char *error = malloc(sizeof(char) * 40);
        error[0] = '\0';
        numErrors++;
        switch (err)
        {
        case GL_INVALID_OPERATION:
            strcpy(error, "INVALID_OPERATION");
            break;
        case GL_INVALID_ENUM:
            strcpy(error, "INVALID_ENUM");
            break;
        case GL_INVALID_VALUE:
            strcpy(error, "INVALID_VALUE");
            error = "INVALID_VALUE";
            break;
        case GL_OUT_OF_MEMORY:
            strcpy(error, "OUT_OF_MEMORY");
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            strcpy(error, "INVALID_FRAMEBUFFER_OPERATION");
            break;
        }

        Log_error("GL_%s - %s : %i\n", error, file, line);
        free(error);
        if (numErrors > 50)
        {
            exit(1);
        }
        err = glGetError();
    }
}

void Renderer_init(Renderer* renderer, const char* dbFileName)
{
    renderer->startPosition = 0;
    renderer->vao = renderer->vbo = renderer->ibo = 0;
    renderer->numIndices = 0;
    renderer->indices = 0;
    renderer->numSceneNodes = 0;
    renderer->sceneNodes = 0;
    renderer->vertexData = 0;
    renderer->vertexDataSize = 0;
    renderer->numMaterials = 0;
    renderer->materials = 0;
    renderer->dbFileName = 0;
    renderer->dbFileName = malloc((strlen(dbFileName) + 1) * sizeof(char));
    renderer->dbFileName[0] = '\0';
    strcpy(renderer->dbFileName, dbFileName);
    Matrix_loadIdentity(&renderer->modelViewProjectionMatrix);
}

void Renderer_destroy(Renderer* renderer)
{
    if (renderer->numSceneNodes > 0)
    {
        glDeleteBuffers(1, &renderer->vbo);
        glDeleteBuffers(1, &renderer->ibo);
        glDeleteVertexArrays(1, &renderer->vao);
    }
    FragmentShader_destroy(&renderer->fragShader);
    VertexShader_destroy(&renderer->vertShader);
    if (renderer->vertexData && renderer->vertexDataSize > 0)
    {
        free(renderer->vertexData);
    }
    if (renderer->sceneNodes && renderer->numSceneNodes > 0)
    {
        free(renderer->sceneNodes);
    }
    if (renderer->indices && renderer->numIndices > 0)
    {
        /* free(renderer->indices); //Causes a crash */
    }
    if (renderer->materials && renderer->numMaterials > 0)
    {
        free(renderer->materials);
    }

    free(renderer->dbFileName);
}

void addTexture(Renderer* renderer, const char* textureFileName, GLuint *textureId, sqlite3 *db)
{
    const char *zSql = "SELECT image FROM texture WHERE name = ?";
    sqlite3_stmt *pStmt;
    int rc;

    do
    {
        rc = sqlite3_prepare(db, zSql, -1, &pStmt, 0);
        sqlite3_bind_text(pStmt, 1, textureFileName, -1, SQLITE_STATIC);
        rc = sqlite3_step(pStmt);
        if( rc==SQLITE_ROW )
        {
            unsigned char* buffer = 0;
            buffer = malloc(sizeof(unsigned char) * sqlite3_column_bytes(pStmt, 0));
            memcpy(buffer, sqlite3_column_blob(pStmt, 0), sqlite3_column_bytes(pStmt, 0));

            SDL_RWops *rw = 0;
            rw = SDL_RWFromMem( buffer, (sizeof(unsigned char) * sqlite3_column_bytes(pStmt, 0)) );

            //textureFileName
            char fileExtension[4];
            fileExtension[3] = '\0';
            int exOffset = strlen(textureFileName) - 3;
            const char* extensionPtr = &textureFileName[exOffset];
            strncpy(&fileExtension[0], extensionPtr, 3);

            //fix for .xv images
            if(strcmp(fileExtension, ".xv") == 0) {
                fileExtension[0] = 'x';
                fileExtension[1] = 'v';
                fileExtension[2] = '\0';
            }

            SDL_Surface* image = IMG_LoadTyped_RW(rw, 1, fileExtension);

            if (image == NULL)
            {
                Log_error("Unable to load bitmap: %s\n", SDL_GetError());
                exit(1);
            }

            glGenTextures(1, textureId);
            glBindTexture(GL_TEXTURE_2D, *textureId);
            int mode = GL_RGB;
            if (image->format->BytesPerPixel == 4)
            {
                mode = GL_RGBA;
                /* SDL_SetColorKey(image, SDL_RLEACCEL, image->format->colorkey   ); */
            }
            glTexImage2D(GL_TEXTURE_2D, 0, mode, image->w, image->h, 0, mode,
                         GL_UNSIGNED_BYTE, image->pixels);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            /* Free the temporary surface */
            /* SDL_free(rw); */
            SDL_FreeSurface(image);
        }
        rc = sqlite3_finalize(pStmt);
    }
    while( rc==SQLITE_SCHEMA );
}

float intToFloat(int i)
{
    float* f = (float*) &i;
    return (*f);
}

int getNumRows(sqlite3* db, sqlite3_stmt* stmt, int* rc, const char* table)
{
    char sql[100];
    sql[0] = '\0';
    strcat(sql, "SELECT COUNT(*) from ");
    strcat(sql, table);

    *rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (*rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL Error: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    if (*rc != SQLITE_DONE)
    {
        *rc = sqlite3_step(stmt);

        if (*rc != SQLITE_ROW && *rc != SQLITE_DONE)
        {
            fprintf(stderr, "SQL Error: %s\n", sqlite3_errmsg(db));
            return -2;
        }
        else if (*rc == SQLITE_DONE)
        {
            return -3;
        }
        int retVal = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return (retVal);
    }
    else
    {
        return 0;
    }
}

void Renderer_buildScene(Renderer* renderer)
{
    if (renderer == 0)
    {
        Log_error("Renderer is null\n");
        return;
    }
    int i;

    sqlite3* db = 0;
    sqlite3_stmt* stmt = 0;
    int rc;

    rc = sqlite3_open(renderer->dbFileName, &db);
    if (rc)
    {
        Log_error("Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    renderer->vertexDataSize = getNumRows(db, stmt, &rc, "vertex");
    if(renderer->vertexDataSize <= 0) {
        Log_error("Unable to load vertex data\n");
        exit(4);
    }
    renderer->vertexData = malloc(sizeof(Vertex) * renderer->vertexDataSize);

    rc = sqlite3_prepare_v2(db, "SELECT * from vertex", -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL Error: %s\n", sqlite3_errmsg(db));
    }
    i = 0;

    while (rc != SQLITE_DONE)
    {
        rc = sqlite3_step(stmt);

        if (rc != SQLITE_ROW && rc != SQLITE_DONE)
        {
            fprintf(stderr, "SQL Error: %s\n", sqlite3_errmsg(db));
            break;
        }
        else if (rc == SQLITE_DONE)
        {
            break;
        }
        Vertex *v = &renderer->vertexData[i];
        renderer->numIndices++;

        v->vertex[0] = intToFloat(sqlite3_column_int(stmt, 1));
        v->vertex[1] = intToFloat(sqlite3_column_int(stmt, 2));
        v->vertex[2] = intToFloat(sqlite3_column_int(stmt, 3));
        v->normal[0] = intToFloat(sqlite3_column_int(stmt, 4));
        v->normal[1] = intToFloat(sqlite3_column_int(stmt, 5));
        v->normal[2] = intToFloat(sqlite3_column_int(stmt, 6));
        v->texcoord[0] = intToFloat(sqlite3_column_int(stmt, 7));
        v->texcoord[1] = intToFloat(sqlite3_column_int(stmt, 8));
        i++;
    }
    sqlite3_finalize(stmt);

    rc = sqlite3_prepare_v2(db, "SELECT * from material", -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL Error: %s\n", sqlite3_errmsg(db));
    }

    renderer->numMaterials = getNumRows(db, stmt, &rc, "material");
    renderer->materials = malloc(renderer->numMaterials * sizeof(Material));
    i = 0;
    while (rc != SQLITE_DONE)
    {
        rc = sqlite3_step(stmt);

        if (rc != SQLITE_ROW && rc != SQLITE_DONE)
        {
            fprintf(stderr, "SQL Error: %s\n", sqlite3_errmsg(db));
            break;
        }
        else if (rc == SQLITE_DONE)
        {
            break;
        }

        Material *m = &renderer->materials[i];

        strcpy(m->name, (char*) sqlite3_column_text(stmt, 1));
        strcpy(m->normal_texture_name, (char*) sqlite3_column_text(stmt, 2));
        m->dissolve = intToFloat(sqlite3_column_int(stmt, 3));
        m->diffuse[0] = intToFloat(sqlite3_column_int(stmt, 4));
        m->diffuse[0] = intToFloat(sqlite3_column_int(stmt, 5));
        m->diffuse[0] = intToFloat(sqlite3_column_int(stmt, 6));

        m->emission[0] = intToFloat(sqlite3_column_int(stmt, 10));
        m->emission[1] = intToFloat(sqlite3_column_int(stmt, 11));
        m->emission[2] = intToFloat(sqlite3_column_int(stmt, 12));

        strcpy(m->specular_texture_name, (char*) sqlite3_column_text(stmt, 14));
        m->specular[0] = intToFloat(sqlite3_column_int(stmt, 15));
        m->specular[1] = intToFloat(sqlite3_column_int(stmt, 16));
        m->specular[2] = intToFloat(sqlite3_column_int(stmt, 17));
        strcpy(m->diffuse_texture_name, (char*) sqlite3_column_text(stmt, 18));
        m->ambient[0] = intToFloat(sqlite3_column_int(stmt, 19));
        m->ambient[1] = intToFloat(sqlite3_column_int(stmt, 20));
        m->ambient[2] = intToFloat(sqlite3_column_int(stmt, 21));
        strcpy(m->ambient_texture_name, (char*) sqlite3_column_text(stmt, 22));
        i++;
    }
    sqlite3_finalize(stmt);

    rc = sqlite3_prepare_v2(db, "SELECT * from scene_node", -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        Log_error("SQL Error: %s\n", sqlite3_errmsg(db));
    }
    i = 0;
    renderer->numSceneNodes = getNumRows(db, stmt, &rc, "scene_node");
    renderer->sceneNodes = malloc(sizeof(SceneNode) * renderer->numSceneNodes);

    while (rc != SQLITE_DONE)
    {
        rc = sqlite3_step(stmt);

        if (rc != SQLITE_ROW && rc != SQLITE_DONE)
        {
            Log_error("SQL Error: %s\n", sqlite3_errmsg(db));
            break;
        }
        else if (rc == SQLITE_DONE)
        {
            break;
        }

        SceneNode* scene_node = &renderer->sceneNodes[i];
        strcpy(scene_node->name, (char*) sqlite3_column_text(stmt, 1));
        scene_node->material_id = sqlite3_column_int(stmt, 2);
        scene_node->start_position = sqlite3_column_int(stmt, 3);
        scene_node->end_position = sqlite3_column_int(stmt, 4);
        Matrix_loadIdentity(&scene_node->model_view_matrix);
        scene_node->primative_mode = GL_TRIANGLES;
        i++;
    }
    sqlite3_finalize(stmt);

    renderer->indices = malloc(sizeof(GLuint) * renderer->numIndices);
    for (i = 0; i < renderer->numIndices + 1; i++)
    {
        renderer->indices[i] = i;
    }

    /* Load textures */
    for (i = 0; i < renderer->numSceneNodes; i++)
    {
        if (renderer->sceneNodes[i].material_id > renderer->numMaterials)
        {
            Log_error("Material id out of bounds: material_id=%i, num_materials=%i\n", renderer->sceneNodes[i].material_id, renderer->numMaterials);
        }
        else
        {
            if (strlen(renderer->materials[renderer->sceneNodes[i].material_id - 1].diffuse_texture_name) > 0)
            {
                addTexture(renderer, renderer->materials[renderer->sceneNodes[i].material_id - 1].diffuse_texture_name,
                           &renderer->sceneNodes[i].diffuse_texture_id, db);
            }
        }
    }

    sqlite3_free(db);
}

void Renderer_bufferToGPU(Renderer* renderer)
{
    /* Allocate and assign a Vertex Array Object to handle */
    glGenVertexArrays(1, &renderer->vao);
    /* Bind Vertex Array Object as the current used object */
    glBindVertexArray(renderer->vao);
    glGenBuffers(1, &renderer->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * renderer->vertexDataSize, &renderer->vertexData[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (sizeof(float) * 3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (sizeof(float) * 6));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glGenBuffers(1, &renderer->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * renderer->numIndices, &renderer->indices[0], GL_STATIC_DRAW);
    VertexShader_init(&renderer->vertShader, "shaders/default.vert");
    //FragmentShader_init(&renderer->fragShader, "shaders/toon.frag");
    FragmentShader_init(&renderer->fragShader, "shaders/default.frag");
    renderer->gpuProgram = glCreateProgram();
    glAttachShader(renderer->gpuProgram, renderer->vertShader.id);
    glAttachShader(renderer->gpuProgram, renderer->fragShader.id);
    glLinkProgram(renderer->gpuProgram);
    /* Check for link errors */
    GLint result;
    GLuint programId = renderer->gpuProgram;
    glGetProgramiv(programId, GL_LINK_STATUS, &result);
    if (result == GL_FALSE)
    {
        GLint length;
        char *log;
        /* get the program info log */
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
        log = (char*) malloc(length);
        glGetProgramInfoLog(programId, length, &result, log);
        Log_message("Unable to link shader: %s\n", log);
        free(log);
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer_render(Renderer* renderer, Camera* camera)
{
    int i;

    if (renderer->numSceneNodes == 0)
    {
        Log_message("skipping render() on empty scene\n");
        exit(1);
        return;
    }
    checkForGLError();
    glBindVertexArray(renderer->vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    checkForGLError();
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ibo);

    /* Frustum_extractFrustum(&renderer->frustum, &camera->modelViewMatrix, &camera->projectionMatrix); */

    for (i = 0; i < renderer->numSceneNodes; i++)
    {

        /*

         position = load from db ()

         // Frustum culling test
         if(frustum.spherePartiallyInFrustum(position.x, position.y, position.z, sceneNodes[i].boundingSphere) > 0)
         {
         ...
         }*/

        float lightPos[3] = { 10.f, 35.f, 0.f };
        checkForGLError();

        glActiveTexture(GL_TEXTURE0);
        checkForGLError();
        glBindTexture(GL_TEXTURE_2D, renderer->sceneNodes[i].diffuse_texture_id);
        checkForGLError();

        /* TODO: move uniforms to buffer object */
        GLuint programID = renderer->gpuProgram;
        GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
        GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
        GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
        GLuint ambientLocation = glGetUniformLocation(programID, "MaterialAmbient");
        GLuint diffuseLocation = glGetUniformLocation(programID, "MaterialDiffuse");
        GLuint specularLocation = glGetUniformLocation(programID, "MaterialSpecular");

        checkForGLError();
        glUseProgram(renderer->gpuProgram);
        checkForGLError();

        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &camera->projectionMatrix.m[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &camera->modelViewMatrix.m[0][0]);
        checkForGLError();
        glUniform3f(LightID, lightPos[0], lightPos[1], lightPos[2]);
        glUniform3f(ambientLocation, renderer->materials[renderer->sceneNodes[i].material_id].ambient[0],
                    renderer->materials[renderer->sceneNodes[i].material_id].ambient[1],
                    renderer->materials[renderer->sceneNodes[i].material_id].ambient[2]);
        glUniform3f(diffuseLocation, renderer->materials[renderer->sceneNodes[i].material_id].diffuse[0],
                    renderer->materials[renderer->sceneNodes[i].material_id].diffuse[1],
                    renderer->materials[renderer->sceneNodes[i].material_id].diffuse[2]);
        glUniform3f(specularLocation, renderer->materials[renderer->sceneNodes[i].material_id].specular[0],
                    renderer->materials[renderer->sceneNodes[i].material_id].specular[1],
                    renderer->materials[renderer->sceneNodes[i].material_id].specular[2]);
        glUniform1i(
            glGetUniformLocation(renderer->gpuProgram, "myTextureSampler"), 0);
        checkForGLError();
        glDrawRangeElementsBaseVertex(renderer->sceneNodes[i].primative_mode, renderer->sceneNodes[i].start_position,
                                      renderer->sceneNodes[i].end_position, (renderer->sceneNodes[i].end_position - renderer->sceneNodes[i].start_position),
                                      GL_UNSIGNED_INT, (void*) (0), renderer->sceneNodes[i].start_position);

        checkForGLError();
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);

    checkForGLError();
}
