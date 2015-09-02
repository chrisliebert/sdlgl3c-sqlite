#include <jni.h>
#include <stdio.h>

#include "SDLGLApp.h"
#include "GraphicsEngineJNI.h"

/*
 * Class:     GraphicsEngineJNI
 * Method:    infoMsg
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_GraphicsEngineJNI_infoMsg(JNIEnv* env,
		jobject thisObj, jstring msg) {
	const char* msgStr = (*env)->GetStringUTFChars(env, msg, 0);
	infoMsg(msgStr);
	(*env)->ReleaseStringUTFChars(env, msg, msgStr);
}

/*
 * Class:     GraphicsEngineJNI
 * Method:    createApp
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_GraphicsEngineJNI_createApp
  (JNIEnv* env, jobject thisObj, jstring dbFile)
{
	long appPtr = NULL;
	SDLGLApp* a;

	const char* dbFileCStr = (*env)->GetStringUTFChars(env, dbFile, 0);
	a = malloc(sizeof(SDLGLApp));
	assert(a);
	SDLGLApp_init(a, dbFileCStr);

	//memcpy(&appPtr, a, sizeof(long)); // will crash
	appPtr = (long)(a);

	return (jlong) appPtr;
}

/*
 * Class:     GraphicsEngineJNI
 * Method:    updateApp
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_GraphicsEngineJNI_updateApp
  (JNIEnv* env, jobject thisObj, jlong appPtr)
{
	SDLGLApp* a = (SDLGLApp*) appPtr;
	SDLGLApp_update(a);
}

/*
 * Class:     GraphicsEngineJNI
 * Method:    destroyApp
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_GraphicsEngineJNI_destroyApp
  (JNIEnv* env, jobject thisObj, jlong appPtr)
{
	SDLGLApp* a = (SDLGLApp*) appPtr;
	SDLGLApp_destroy(a);
}

/*
 * Class:     GraphicsEngineJNI
 * Method:    getRunLevel
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_GraphicsEngineJNI_getRunLevel
  (JNIEnv* env, jobject thisObj, jlong appPtr)
{
	SDLGLApp* a = (SDLGLApp*) appPtr;
	return a->runLevel;
}
