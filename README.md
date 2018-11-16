# NDK-Socket-Process
ndk socket 双线程守护





![](https://raw.githubusercontent.com/Sogrey/NDK-Socket-Process/master/pic.gif)



```mermaid
graph TB
    subgraph TCP服务器<守护进程>
    TCP_socket(socket)-->TCP_bind(bind)
    TCP_bind(bind)-->TCP_listen(listen)
    TCP_listen(listen)-->TCP_accept(accept)
    TCP_accept(accept)--阻塞直到客户端连接-->TCP_read(read)
    TCP_read(read)--处理请求-->TCP_write(write)
    TCP_write(write)-->TCP_read(read)
    TCP_write(write)-->TCP_read2(read)
    TCP_read2(read)-->TCP_close(close)
    end
    
    subgraph APK进程<TCP客户端>
    apk_socket(socket)-->apk_connect(connect)
    apk_connect(connect)-->apk_write(write)
    apk_write(write)-->apk_read(read)
    apk_read(read)-->apk_write(write)
    apk_read(read)-->apk_close(close)
    end    
   
    apk_connect(connect)--建立连接---TCP_read(read)
	apk_write(write)--请求数据---TCP_read(read)
	TCP_write(write)--回应数据-->apk_read(read)
	apk_close(close)--结束连接-->TCP_read2(read)    
```