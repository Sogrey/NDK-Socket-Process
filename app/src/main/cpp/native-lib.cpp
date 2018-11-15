
#include <jni.h>
#include <string>
#include <unistd.h>

#include "native-lib.h"

const char *PATH = "/data/data/top.sogrey.socketprocess/my.sock";

int m_child = 0;
const char *userId;

/**
 * 守护
 * @return
 */
void child_do_work() {
    //开启Socket - 服务端
    if (child_create_channel()) {
        child_listen_msg();
    }
}

/**
 * 服务端读取信息
 * 客户端
 */
void child_listen_msg() {
    fd_set rfds;
    struct timeval timeout = {3, 0};
    while (1) {
        //清空内容
        FD_ZERO(&rfds);
        //读取客户端内容
        FD_SET(m_child, &rfds);
        //选择监听
        int r = select(m_child + 1, &rfds, NULL, NULL, &timeout);
        LOGE("读取消息前 %d", r);
        if (r > 0) {
            //缓冲区
            char pkg[256] = {0};
            //保证所读到的信息是指定apk客户端
            if (FD_ISSET(m_child, &rfds)) {
                //阻塞函数
                read(m_child, pkg, sizeof(pkg));
                LOGE("userId = %s", userId);
                //开启服务
                execlp("am", "am", "startservice", "--user", userId,
                       "top.sogrey.socketprocess/top.sogrey.socketprocess.ProcessService",
                       (char *) NULL);
                break;
            }
        }
    }
}

/**
 * 创建服务端socket
 * @return
 */
int child_create_channel() {
    //创建Socket 返回句柄
    int listenfd = socket(AF_LOCAL, SOCK_STREAM, 0);

    //把之前的清空
    unlink(PATH);

    //内存区域
    struct sockaddr_un addr;

    //清空内存
    memset(&addr, 0, sizeof(sockaddr));
    //协议
    addr.sun_family = AF_LOCAL;
    //拷贝- 赋值
    strcpy(addr.sun_path, PATH);
//绑定
    if (bind(listenfd, (const sockaddr *) &addr, sizeof(sockaddr_un)) < 0) {
        LOGE("绑定错误");
        return 0;
    }

    //监听5个客户端
    listen(listenfd, 5);

    int connfd = 0;
    while (1) {
        //返回值 客户端地址 阻塞函数
        connfd = accept(listenfd, NULL, NULL) < 0;
        if (connfd) {
            if (errno == EINTR) {//
                continue;
            } else {
                LOGE("绑定错误");
                return 0;
            }
        }
        //成功
        m_child = connfd;
        LOGE("连接成功!!!");
        break;
    }
    return 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_top_sogrey_socketprocess_Watcher_createWatcher(JNIEnv *env, jobject instance,
                                                    jstring userId_) {
    userId = env->GetStringUTFChars(userId_, 0);

    // 开双进程
    pid_t pid = fork();
    if (pid < 0) {//失败

    } else if (pid == 0) {//子进程 - 当前守护进程
        child_do_work();//守护
    } else if (pid > 0) {//父进程

    }

    env->ReleaseStringUTFChars(userId_, userId);
}

extern "C"
JNIEXPORT void JNICALL
Java_top_sogrey_socketprocess_Watcher_connectWatcher(JNIEnv *env, jobject instance) {
//
    int socked = 0;
    while (1) {
        LOGE("客户端 父进程开始连接");
        socked = socket(AF_LOCAL, SOCK_STREAM, 0);
        if (socked < 0) {
            LOGE("连接失败");
            return;
        }
        //内存区域
        struct sockaddr_un addr;
        //清空内存
        memset(&addr, 0, sizeof(sockaddr_un));
        //协议
        addr.sun_family = AF_LOCAL;

        strcpy(addr.sun_path, PATH);
        if (connect(socked, (const sockaddr *) &addr, sizeof(addr)) < 0) {
            LOGE("连接失败");
            close(socked);
            sleep(1);
            //在下一次尝试
            continue;
        }
        //成功
        LOGE("连接成功");
        break;
    }
}

extern "C"
JNIEXPORT jstring JNICALL
Java_top_sogrey_socketprocess_Watcher_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}