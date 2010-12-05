
#include <string.h>
#include <jni.h>

#include <android/log.h>

#include <string>
#include <vector>

extern "C"
{
JNIEXPORT jobjectArray JNICALL
Java_com_example_hymnal_Hymnal_listHymns( JNIEnv* env, jobject thiz );
}

static std::vector<std::string> getList(){
	std::vector<std::string> ret;
	ret.push_back( "Hymnal: A Worship Book" );
	return ret;
}

JNIEXPORT jobjectArray JNICALL
Java_com_example_hymnal_Hymnal_listHymns( JNIEnv* env, jobject thiz )
{
	std::vector<std::string> list = getList();
	jclass sclass = env->FindClass("java/lang/String");
	jobjectArray strarray = env->NewObjectArray(list.size(), sclass, NULL);
	jstring str;
	
	for ( int i = 0; i < list.size(); ++i ){
//		__android_log_print(ANDROID_LOG_ERROR, "Hymnal-JNI", list[i].c_str());
		str = env->NewStringUTF(list[i].c_str());
		env->SetObjectArrayElement(strarray,i,str);
		env->DeleteLocalRef(str);
	}
	
	return strarray;
}