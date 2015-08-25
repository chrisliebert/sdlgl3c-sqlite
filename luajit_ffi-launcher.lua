#!/usr/bin/env luajit
local ffi = require("ffi")

ffi.cdef[[
typedef unsigned GLuint;
typedef int GLenum;
typedef float GLfloat;

typedef struct Matrix
{
    float m[4][4];
} Matrix;

float* Matrix_getOneDimensionalArray(Matrix*);
void Matrix_scale(Matrix* result, float sx, float sy, float sz);
void Matrix_translate(Matrix* result, float tx, float ty, float tz);
void Matrix_rotate(Matrix* result, float angle, float x, float y, float z);
void Matrix_loadIdentity(Matrix* result);
void Matrix_lookAt(Matrix* result, float posX, float posY, float posZ, float lookAtX, float lookAtY, float lookAtZ, float upX, float upY, float upZ);
void Matrix_frustum(Matrix* result, float left, float right, float bottom, float top, float nearZ, float farZ);
void Matrix_perspective(Matrix* result, float fovy, float aspect, float nearZ, float farZ);
void Matrix_ortho(Matrix* result, float left, float right, float bottom, float top, float nearZ, float farZ);
void Matrix_multiply(Matrix* result, Matrix* srcA, Matrix* srcB);

typedef struct Camera
{
    Matrix modelViewMatrix;
    Matrix projectionMatrix;
    float position[3];
    float direction[3];
    float right[3];
    float up[3];
    float horizontalAngle;
    float verticalAngle;
    float speed;
} Camera;

void Camera_aim(Camera* camera, float x, float y);
void Camera_init(Camera* camera);
void Camera_moveBackward(Camera* camera, float amount);
void Camera_moveForward(Camera* camera, float amount);
void Camera_moveLeft(Camera* camera, float amount);
void Camera_moveRight(Camera* camera, float amount);
void Camera_update(Camera* camera);

typedef struct
{
    char name[64];
    float ambient[3];
    float diffuse[3];
    float specular[3];
    float transmittance[3];
    float emission[3];
    float shininess;
    float ior; /* index of refraction */
    float dissolve; /* 1 == opaque; 0 == fully transparent */
    /* illumination model (see http://www.fileformat.info/format/material/) */
    int illum;
    /* Texture file names */
    char ambientTextureName[64];
    char diffuseTextureName[64];
    char specularTextureName[64];
    char normalTextureName[64];
} Material;

typedef struct
{
    char name[64];
    int materialId;
    Matrix modelViewMatrix;
    GLuint startPosition;
    GLuint endPosition;
    GLenum primativeMode;
    GLuint ambientTextureId;
    GLuint diffuseTextureId;
    GLuint normalTextureId;
    GLuint specularTextureId;
    GLfloat boundingSphere;
} SceneNode;

typedef struct Vertex
{
    GLfloat vertex[3];
    GLfloat normal[3];
    GLfloat texcoord[2];
} Vertex;

typedef struct Shader
{
    GLuint id;
    const char* filePath;
    const char* shaderSrc;
} Shader;

typedef struct FragmentShader
{
    GLuint id;
    const char* filePath;
    const char* shaderSrc;
} FragmentShader;

typedef struct VertexShader
{
    GLuint id;
    const char* filePath;
    const char* shaderSrc;
} VertexShader;

GLuint Shader_getId(Shader*);

void FragmentShader_init(FragmentShader*, const char*);
void FragmentShader_destroy(FragmentShader*);

void VertexShader_init(VertexShader*, const char*);
void VertexShader_destroy(VertexShader*);

typedef struct Renderer
{
    GLuint vao, vbo, ibo;
    GLuint gpuProgram;
    Vertex* vertexData;
    int vertexDataSize;
    SceneNode* sceneNodes;
    int numSceneNodes;
    GLuint* indices;
    int numIndices;
    int numMaterials;
    Material* materials;
    VertexShader vertShader;
    FragmentShader fragShader;
    char* dbFileName;
    bool useFixedFunctionLegacyMode; /* Disable shaders and used OpenGL 1 immediate mode */
} Renderer;

void Renderer_bufferToGPU(Renderer*);
void Renderer_init(Renderer*, const char*);
void Renderer_destroy(Renderer*);
void Renderer_buildScene(Renderer*);
void Renderer_render(Renderer*, Camera*);


/** SDL types required (program crashes without these definitions) **/

/**
 * \brief A signed 8-bit integer type.
 */
typedef int8_t Sint8;
/**
 * \brief An unsigned 8-bit integer type.
 */
typedef uint8_t Uint8;
/**
 * \brief A signed 16-bit integer type.
 */
typedef int16_t Sint16;
/**
 * \brief An unsigned 16-bit integer type.
 */
typedef uint16_t Uint16;
/**
 * \brief A signed 32-bit integer type.
 */
typedef int32_t Sint32;
/**
 * \brief An unsigned 32-bit integer type.
 */
typedef uint32_t Uint32;

/**
 * \brief A signed 64-bit integer type.
 */
typedef int64_t Sint64;
/**
 * \brief An unsigned 64-bit integer type.
 */
typedef uint64_t Uint64;

typedef enum
{
    SDL_FIRSTEVENT     = 0,     /**< Unused (do not remove) */

    /* Application events */
    SDL_QUIT           = 0x100, /**< User-requested quit */

    /* These application events have special meaning on iOS, see README-ios.txt for details */
    SDL_APP_TERMINATING,        /**< The application is being terminated by the OS
                                     Called on iOS in applicationWillTerminate()
                                     Called on Android in onDestroy()
                                */
    SDL_APP_LOWMEMORY,          /**< The application is low on memory, free memory if possible.
                                     Called on iOS in applicationDidReceiveMemoryWarning()
                                     Called on Android in onLowMemory()
                                */
    SDL_APP_WILLENTERBACKGROUND, /**< The application is about to enter the background
                                     Called on iOS in applicationWillResignActive()
                                     Called on Android in onPause()
                                */
    SDL_APP_DIDENTERBACKGROUND, /**< The application did enter the background and may not get CPU for some time
                                     Called on iOS in applicationDidEnterBackground()
                                     Called on Android in onPause()
                                */
    SDL_APP_WILLENTERFOREGROUND, /**< The application is about to enter the foreground
                                     Called on iOS in applicationWillEnterForeground()
                                     Called on Android in onResume()
                                */
    SDL_APP_DIDENTERFOREGROUND, /**< The application is now interactive
                                     Called on iOS in applicationDidBecomeActive()
                                     Called on Android in onResume()
                                */

    /* Window events */
    SDL_WINDOWEVENT    = 0x200, /**< Window state change */
    SDL_SYSWMEVENT,             /**< System specific event */

    /* Keyboard events */
    SDL_KEYDOWN        = 0x300, /**< Key pressed */
    SDL_KEYUP,                  /**< Key released */
    SDL_TEXTEDITING,            /**< Keyboard text editing (composition) */
    SDL_TEXTINPUT,              /**< Keyboard text input */

    /* Mouse events */
    SDL_MOUSEMOTION    = 0x400, /**< Mouse moved */
    SDL_MOUSEBUTTONDOWN,        /**< Mouse button pressed */
    SDL_MOUSEBUTTONUP,          /**< Mouse button released */
    SDL_MOUSEWHEEL,             /**< Mouse wheel motion */

    /* Joystick events */
    SDL_JOYAXISMOTION  = 0x600, /**< Joystick axis motion */
    SDL_JOYBALLMOTION,          /**< Joystick trackball motion */
    SDL_JOYHATMOTION,           /**< Joystick hat position change */
    SDL_JOYBUTTONDOWN,          /**< Joystick button pressed */
    SDL_JOYBUTTONUP,            /**< Joystick button released */
    SDL_JOYDEVICEADDED,         /**< A new joystick has been inserted into the system */
    SDL_JOYDEVICEREMOVED,       /**< An opened joystick has been removed */

    /* Game controller events */
    SDL_CONTROLLERAXISMOTION  = 0x650, /**< Game controller axis motion */
    SDL_CONTROLLERBUTTONDOWN,          /**< Game controller button pressed */
    SDL_CONTROLLERBUTTONUP,            /**< Game controller button released */
    SDL_CONTROLLERDEVICEADDED,         /**< A new Game controller has been inserted into the system */
    SDL_CONTROLLERDEVICEREMOVED,       /**< An opened Game controller has been removed */
    SDL_CONTROLLERDEVICEREMAPPED,      /**< The controller mapping was updated */

    /* Touch events */
    SDL_FINGERDOWN      = 0x700,
    SDL_FINGERUP,
    SDL_FINGERMOTION,

    /* Gesture events */
    SDL_DOLLARGESTURE   = 0x800,
    SDL_DOLLARRECORD,
    SDL_MULTIGESTURE,

    /* Clipboard events */
    SDL_CLIPBOARDUPDATE = 0x900, /**< The clipboard changed */

    /* Drag and drop events */
    SDL_DROPFILE        = 0x1000, /**< The system requests a file open */

    /* Render events */
    SDL_RENDER_TARGETS_RESET = 0x2000, /**< The render targets have been reset */

    /** Events ::SDL_USEREVENT through ::SDL_LASTEVENT are for your use,
     *  and should be allocated with SDL_RegisterEvents()
     */
    SDL_USEREVENT    = 0x8000,

    /**
     *  This last event is only for bounding internal arrays
     */
    SDL_LASTEVENT    = 0xFFFF
} SDL_EventType;

/**
 *  \brief Fields shared by every event
 */
typedef struct SDL_CommonEvent
{
    Uint32 type;
    Uint32 timestamp;
} SDL_CommonEvent;

/**
 *  \brief Window state change event data (event.window.*)
 */
typedef struct SDL_WindowEvent
{
    Uint32 type;        /**< ::SDL_WINDOWEVENT */
    Uint32 timestamp;
    Uint32 windowID;    /**< The associated window */
    Uint8 event;        /**< ::SDL_WindowEventID */
    Uint8 padding1;
    Uint8 padding2;
    Uint8 padding3;
    Sint32 data1;       /**< event dependent data */
    Sint32 data2;       /**< event dependent data */
} SDL_WindowEvent;

typedef int SDL_Scancode;
typedef int SDL_Keycode;

typedef struct SDL_Keysym
{
    SDL_Scancode scancode;
    int sym;
    SDL_Keycode sym;            /**< SDL virtual key code - see ::SDL_Keycode for details */
    Uint16 mod;                 /**< current key modifiers */
    Uint32 unused;
} SDL_Keysym;

/**
 *  \brief Keyboard button event structure (event.key.*)
 */
typedef struct SDL_KeyboardEvent
{
    Uint32 type;        /**< ::SDL_KEYDOWN or ::SDL_KEYUP */
    Uint32 timestamp;
    Uint32 windowID;    /**< The window with keyboard focus, if any */
    Uint8 state;        /**< ::SDL_PRESSED or ::SDL_RELEASED */
    Uint8 repeat;       /**< Non-zero if this is a key repeat */
    Uint8 padding2;
    Uint8 padding3;
    SDL_Keysym keysym;  /**< The key that was pressed or released */
    //int keysym;
} SDL_KeyboardEvent;

//#define SDL_TEXTEDITINGEVENT_TEXT_SIZE (32)
/**
 *  \brief Keyboard text editing event structure (event.edit.*)
 */
typedef struct SDL_TextEditingEvent
{
    Uint32 type;                                /**< ::SDL_TEXTEDITING */
    Uint32 timestamp;
    Uint32 windowID;                            /**< The window with keyboard focus, if any */
    char text[32];  /**< The editing text */
    Sint32 start;                               /**< The start cursor of selected editing text */
    Sint32 length;                              /**< The length of selected editing text */
} SDL_TextEditingEvent;


//#define SDL_TEXTINPUTEVENT_TEXT_SIZE (32)
/**
 *  \brief Keyboard text input event structure (event.text.*)
 */
typedef struct SDL_TextInputEvent
{
    Uint32 type;                              /**< ::SDL_TEXTINPUT */
    Uint32 timestamp;
    Uint32 windowID;                          /**< The window with keyboard focus, if any */
    char text[32];  /**< The input text */
} SDL_TextInputEvent;

/**
 *  \brief Mouse motion event structure (event.motion.*)
 */
typedef struct SDL_MouseMotionEvent
{
    Uint32 type;        /**< ::SDL_MOUSEMOTION */
    Uint32 timestamp;
    Uint32 windowID;    /**< The window with mouse focus, if any */
    Uint32 which;       /**< The mouse instance id, or SDL_TOUCH_MOUSEID */
    Uint32 state;       /**< The current button state */
    Sint32 x;           /**< X coordinate, relative to window */
    Sint32 y;           /**< Y coordinate, relative to window */
    Sint32 xrel;        /**< The relative motion in the X direction */
    Sint32 yrel;        /**< The relative motion in the Y direction */
} SDL_MouseMotionEvent;

/**
 *  \brief Mouse button event structure (event.button.*)
 */
typedef struct SDL_MouseButtonEvent
{
    Uint32 type;        /**< ::SDL_MOUSEBUTTONDOWN or ::SDL_MOUSEBUTTONUP */
    Uint32 timestamp;
    Uint32 windowID;    /**< The window with mouse focus, if any */
    Uint32 which;       /**< The mouse instance id, or SDL_TOUCH_MOUSEID */
    Uint8 button;       /**< The mouse button index */
    Uint8 state;        /**< ::SDL_PRESSED or ::SDL_RELEASED */
    Uint8 clicks;       /**< 1 for single-click, 2 for double-click, etc. */
    Uint8 padding1;
    Sint32 x;           /**< X coordinate, relative to window */
    Sint32 y;           /**< Y coordinate, relative to window */
} SDL_MouseButtonEvent;

/**
 *  \brief Mouse wheel event structure (event.wheel.*)
 */
typedef struct SDL_MouseWheelEvent
{
    Uint32 type;        /**< ::SDL_MOUSEWHEEL */
    Uint32 timestamp;
    Uint32 windowID;    /**< The window with mouse focus, if any */
    Uint32 which;       /**< The mouse instance id, or SDL_TOUCH_MOUSEID */
    Sint32 x;           /**< The amount scrolled horizontally, positive to the right and negative to the left */
    Sint32 y;           /**< The amount scrolled vertically, positive away from the user and negative toward the user */
} SDL_MouseWheelEvent;

/**
 *  \brief Joystick axis motion event structure (event.jaxis.*)
 */
typedef Sint32 SDL_JoystickID;
 
typedef struct SDL_JoyAxisEvent
{
    Uint32 type;        /**< ::SDL_JOYAXISMOTION */
    Uint32 timestamp;
    SDL_JoystickID which; /**< The joystick instance id */
    Uint8 axis;         /**< The joystick axis index */
    Uint8 padding1;
    Uint8 padding2;
    Uint8 padding3;
    Sint16 value;       /**< The axis value (range: -32768 to 32767) */
    Uint16 padding4;
} SDL_JoyAxisEvent;

/**
 *  \brief Joystick trackball motion event structure (event.jball.*)
 */
typedef struct SDL_JoyBallEvent
{
    Uint32 type;        /**< ::SDL_JOYBALLMOTION */
    Uint32 timestamp;
    SDL_JoystickID which; /**< The joystick instance id */
    Uint8 ball;         /**< The joystick trackball index */
    Uint8 padding1;
    Uint8 padding2;
    Uint8 padding3;
    Sint16 xrel;        /**< The relative motion in the X direction */
    Sint16 yrel;        /**< The relative motion in the Y direction */
} SDL_JoyBallEvent;

/**
 *  \brief Joystick hat position change event structure (event.jhat.*)
 */
typedef struct SDL_JoyHatEvent
{
    Uint32 type;        /**< ::SDL_JOYHATMOTION */
    Uint32 timestamp;
    SDL_JoystickID which; /**< The joystick instance id */
    Uint8 hat;          /**< The joystick hat index */
    Uint8 value;        /**< The hat position value.
                         *   \sa ::SDL_HAT_LEFTUP ::SDL_HAT_UP ::SDL_HAT_RIGHTUP
                         *   \sa ::SDL_HAT_LEFT ::SDL_HAT_CENTERED ::SDL_HAT_RIGHT
                         *   \sa ::SDL_HAT_LEFTDOWN ::SDL_HAT_DOWN ::SDL_HAT_RIGHTDOWN
                         *
                         *   Note that zero means the POV is centered.
                         */
    Uint8 padding1;
    Uint8 padding2;
} SDL_JoyHatEvent;

/**
 *  \brief Joystick button event structure (event.jbutton.*)
 */
typedef struct SDL_JoyButtonEvent
{
    Uint32 type;        /**< ::SDL_JOYBUTTONDOWN or ::SDL_JOYBUTTONUP */
    Uint32 timestamp;
    SDL_JoystickID which; /**< The joystick instance id */
    Uint8 button;       /**< The joystick button index */
    Uint8 state;        /**< ::SDL_PRESSED or ::SDL_RELEASED */
    Uint8 padding1;
    Uint8 padding2;
} SDL_JoyButtonEvent;

/**
 *  \brief Joystick device event structure (event.jdevice.*)
 */
typedef struct SDL_JoyDeviceEvent
{
    Uint32 type;        /**< ::SDL_JOYDEVICEADDED or ::SDL_JOYDEVICEREMOVED */
    Uint32 timestamp;
    Sint32 which;       /**< The joystick device index for the ADDED event, instance id for the REMOVED event */
} SDL_JoyDeviceEvent;


/**
 *  \brief Game controller axis motion event structure (event.caxis.*)
 */
typedef struct SDL_ControllerAxisEvent
{
    Uint32 type;        /**< ::SDL_CONTROLLERAXISMOTION */
    Uint32 timestamp;
    SDL_JoystickID which; /**< The joystick instance id */
    Uint8 axis;         /**< The controller axis (SDL_GameControllerAxis) */
    Uint8 padding1;
    Uint8 padding2;
    Uint8 padding3;
    Sint16 value;       /**< The axis value (range: -32768 to 32767) */
    Uint16 padding4;
} SDL_ControllerAxisEvent;


/**
 *  \brief Game controller button event structure (event.cbutton.*)
 */
typedef struct SDL_ControllerButtonEvent
{
    Uint32 type;        /**< ::SDL_CONTROLLERBUTTONDOWN or ::SDL_CONTROLLERBUTTONUP */
    Uint32 timestamp;
    SDL_JoystickID which; /**< The joystick instance id */
    Uint8 button;       /**< The controller button (SDL_GameControllerButton) */
    Uint8 state;        /**< ::SDL_PRESSED or ::SDL_RELEASED */
    Uint8 padding1;
    Uint8 padding2;
} SDL_ControllerButtonEvent;


/**
 *  \brief Controller device event structure (event.cdevice.*)
 */
typedef struct SDL_ControllerDeviceEvent
{
    Uint32 type;        /**< ::SDL_CONTROLLERDEVICEADDED, ::SDL_CONTROLLERDEVICEREMOVED, or ::SDL_CONTROLLERDEVICEREMAPPED */
    Uint32 timestamp;
    Sint32 which;       /**< The joystick device index for the ADDED event, instance id for the REMOVED or REMAPPED event */
} SDL_ControllerDeviceEvent;

typedef Sint64 SDL_TouchID;
typedef Sint64 SDL_FingerID;

typedef struct SDL_Finger
{
    SDL_FingerID id;
    float x;
    float y;
    float pressure;
} SDL_Finger;

/**
 *  \brief Touch finger event structure (event.tfinger.*)
 */
typedef struct SDL_TouchFingerEvent
{
    Uint32 type;        /**< ::SDL_FINGERMOTION or ::SDL_FINGERDOWN or ::SDL_FINGERUP */
    Uint32 timestamp;
    SDL_TouchID touchId; /**< The touch device id */
    SDL_FingerID fingerId;
    float x;            /**< Normalized in the range 0...1 */
    float y;            /**< Normalized in the range 0...1 */
    float dx;           /**< Normalized in the range 0...1 */
    float dy;           /**< Normalized in the range 0...1 */
    float pressure;     /**< Normalized in the range 0...1 */
} SDL_TouchFingerEvent;


/**
 *  \brief Multiple Finger Gesture Event (event.mgesture.*)
 */
typedef struct SDL_MultiGestureEvent
{
    Uint32 type;        /**< ::SDL_MULTIGESTURE */
    Uint32 timestamp;
    SDL_TouchID touchId; /**< The touch device index */
    float dTheta;
    float dDist;
    float x;
    float y;
    Uint16 numFingers;
    Uint16 padding;
} SDL_MultiGestureEvent;

typedef Sint64 SDL_GestureID;

/**
 * \brief Dollar Gesture Event (event.dgesture.*)
 */
typedef struct SDL_DollarGestureEvent
{
    Uint32 type;        /**< ::SDL_DOLLARGESTURE */
    Uint32 timestamp;
    SDL_TouchID touchId; /**< The touch device id */
    SDL_GestureID gestureId;
    Uint32 numFingers;
    float error;
    float x;            /**< Normalized center of gesture */
    float y;            /**< Normalized center of gesture */
} SDL_DollarGestureEvent;

typedef struct SDLGLApp
{
    // SDL_Window* window;
    Renderer renderer;
    Camera camera;
    //SDL_GLContext glContext;
    //SDL_Event event;
    float position[3];
    float direction[3];
    float right[3];
    float horizontalAngle;
    float verticalAngle;
    float mouseSpeed;
    float deltaTime;
    int runLevel;
    double lastTime;
} SDLGLApp;

void SDLGLApp_init(SDLGLApp*, const char*);
void SDLGLApp_start(SDLGLApp*);
void SDLGLApp_destroy(SDLGLApp*);
]]

-- Determine the operating system be examining directory seperator
function isWindows()
	if package.config:sub(1,1) == "\\" then return true end
end

function isUnix()
	if package.config:sub(1,1) == "/" then return false end
end

-- Load SDLGLApp from C

ge = nil
if isWindows() then
	ge = ffi.load("./libGraphicsEngine.dll")
else
	ge = ffi.load("./libGraphicsEngine.so")
end

print("loaded libGraphicsEngine with ffi")

if arg[1] == nil then
  print("You must supply a database")
  os.exit(1)
end

print("Loading ", arg[1])

SDLGL3App = {}
SDLGL3App.__index = SDLGL3App

function SDLGL3App.create(dbFileName)
	local app = {}
	setmetatable(app, SDLGL3App)
	app.cStruct = ffi.new("SDLGLApp[1]")
	ge.SDLGLApp_init(app.cStruct, dbFileName)
	return app
end

function SDLGL3App.start(self)

	ge.SDLGLApp_start(self.cStruct)
end

function SDLGL3App.destroy(self)
	ge.SDLGLApp_destroy(self.cStruct)
end

local a = SDLGL3App.create(arg[1])
a:start()
a:destroy()

os.exit(0)

