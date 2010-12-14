
#include <string.h>
#include <jni.h>

#include <android/log.h>

#include <string>
#include <vector>
#include <sstream>

#include "../../Common/Model/DatabaseHelper.h"

extern "C"
{
JNIEXPORT jobjectArray JNICALL
Java_com_example_hymnal_Hymnal_listHymnals ( JNIEnv* env, jobject thiz );
JNIEXPORT jobjectArray JNICALL
Java_com_example_hymnal_HymnList_listHymns ( JNIEnv* env, jobject thiz, jint hymnalId );
JNIEXPORT jobjectArray JNICALL
Java_com_example_hymnal_HymnView_getMusicSections ( JNIEnv* env, jobject thiz, jint hymnId, jint partSpecifier );
JNIEXPORT jobjectArray JNICALL
Java_com_example_hymnal_HymnView_getSections ( JNIEnv* env, jobject thiz, jint hymnId, jint verse, jint partSpecifier );
JNIEXPORT jint JNICALL
Java_com_example_hymnal_HymnView_getNumVerses ( JNIEnv* env, jobject thiz, jint hymnId );
}

jint JNI_OnLoad(JavaVM *vm, void *reserved){
	__android_log_print(ANDROID_LOG_ERROR, "hymnal-JNI", "opened connection");
	std::string tmp = "/sdcard/hymnalapp/database.db";
	openConnectionWithPath(tmp);
	return JNI_VERSION_1_2;
}

static std::vector<std::string> getHymnalList(){
	Hymnal h = getHymnal(1);
	std::vector<std::string> ret;
	ret.push_back( h.get_title() );
	return ret;
}

JNIEXPORT jobjectArray JNICALL
Java_com_example_hymnal_Hymnal_listHymnals( JNIEnv* env, jobject thiz )
{
	std::vector<std::string> list = getHymnalList();
	jclass sclass = env->FindClass("java/lang/String");
	jobjectArray strarray = env->NewObjectArray(list.size(), sclass, NULL);
	jstring str;
	
	for ( int i = 0; i < list.size(); ++i ){
//		__android_log_print(ANDROID_LOG_ERROR, "hymnal-JNI", list[i].c_str());
		str = env->NewStringUTF(list[i].c_str());
		env->SetObjectArrayElement(strarray,i,str);
		env->DeleteLocalRef(str);
	}
	
	return strarray;
}

JNIEXPORT jobjectArray JNICALL
Java_com_example_hymnal_HymnList_listHymns ( JNIEnv* env, jobject thiz, jint hymnalId )
{
	HymnVector list = getHymnsForHymnal ( hymnalId, SORT_BY_NUMBER );
	
	jclass hymnClass = env->FindClass ( "com/example/hymnal/Hymn" );
	jobjectArray hymnArray = env->NewObjectArray ( list.size(), hymnClass, NULL );
	jmethodID midConstructor = env->GetMethodID ( hymnClass, "<init>", "(IILjava/lang/String;)V" );

	jclass stringClass = env->FindClass ( "java/lang/String" );
    jobject hymnObject;
	
	for ( int i = 0; i < list.size(); ++i ){
		jstring title = env->NewStringUTF ( list[i].get_title().c_str() );
		__android_log_print ( ANDROID_LOG_ERROR, "hymnal-JNI", list[i].get_title().c_str() );
     	hymnObject = env->NewObject ( hymnClass, midConstructor, 
    					list[i].get_hymnID(), list[i].get_hymnNumber(), title );
		env->SetObjectArrayElement ( hymnArray, i, hymnObject );
		env->DeleteLocalRef ( hymnObject );
		env->DeleteLocalRef ( title );
	}

	return hymnArray;
}

JNIEXPORT jobjectArray JNICALL
Java_com_example_hymnal_HymnView_getMusicSections ( JNIEnv* env, jobject thiz, jint hymnId, jint partSpecifier )
{
	PartSpecifier ps = ALLPARTS;
	if ( partSpecifier == 1 )
		ps = TREBLE;
	else if ( partSpecifier == 2 )
		ps = BASS;
	HymnSectionVector list = getMusicPiecesForHymn ( hymnId, ps );
	
	jclass stringClass = env->FindClass ( "java/lang/String" );
	jobjectArray sectionArray = env->NewObjectArray ( list.size(), stringClass, NULL );
	
	for ( int i = 0; i < list.size(); ++i ){
		jstring location = env->NewStringUTF ( list[i].imagePath.c_str() );
		env->SetObjectArrayElement ( sectionArray, i, location );
		env->DeleteLocalRef ( location );
	}
	
	return sectionArray;
}


JNIEXPORT jobjectArray JNICALL
Java_com_example_hymnal_HymnView_getSections ( JNIEnv* env, jobject thiz, jint hymnId, jint verse, jint partSpecifier )
{
	PartSpecifier ps = ALLPARTS;
	if ( partSpecifier == 1 )
		ps = TREBLE;
	else if ( partSpecifier == 2 )
		ps = BASS;
	HymnSectionVector list = getPiecesForHymn ( hymnId, verse, ps );
	
	jclass stringClass = env->FindClass ( "java/lang/String" );
	jobjectArray sectionArray = env->NewObjectArray ( list.size(), stringClass, NULL );
	
	for ( int i = 0; i < list.size(); ++i ){
		jstring location = env->NewStringUTF ( list[i].imagePath.c_str() );
		env->SetObjectArrayElement ( sectionArray, i, location );
		env->DeleteLocalRef ( location );
	}
	
	return sectionArray;
}

JNIEXPORT jint JNICALL
Java_com_example_hymnal_HymnView_getNumVerses ( JNIEnv* env, jobject thiz, jint hymnId )
{
	unsigned int a = numVersesForHymn ( hymnId );
	std::stringstream ss;
	ss << a;
	
	__android_log_print(ANDROID_LOG_ERROR, "hymnal-JNI", ss.str().c_str());
	return a;
}
