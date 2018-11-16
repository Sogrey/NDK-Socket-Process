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
使用[Typora](https://www.typora.io/)查看或点击[流程图](https://mermaidjs.github.io/mermaid-live-editor/#/view/eyJjb2RlIjoiZ3JhcGggVEJcbiAgICBzdWJncmFwaCBUQ1DmnI3liqHlmag85a6I5oqk6L-b56iLPlxuICAgIFRDUF9zb2NrZXQoc29ja2V0KS0tPlRDUF9iaW5kKGJpbmQpXG4gICAgVENQX2JpbmQoYmluZCktLT5UQ1BfbGlzdGVuKGxpc3RlbilcbiAgICBUQ1BfbGlzdGVuKGxpc3RlbiktLT5UQ1BfYWNjZXB0KGFjY2VwdClcbiAgICBUQ1BfYWNjZXB0KGFjY2VwdCktLemYu-WhnuebtOWIsOWuouaIt-err-i_nuaOpS0tPlRDUF9yZWFkKHJlYWQpXG4gICAgVENQX3JlYWQocmVhZCktLeWkhOeQhuivt-axgi0tPlRDUF93cml0ZSh3cml0ZSlcbiAgICBUQ1Bfd3JpdGUod3JpdGUpLS0-VENQX3JlYWQocmVhZClcbiAgICBUQ1Bfd3JpdGUod3JpdGUpLS0-VENQX3JlYWQyKHJlYWQpXG4gICAgVENQX3JlYWQyKHJlYWQpLS0-VENQX2Nsb3NlKGNsb3NlKVxuICAgIGVuZCAgICBcbiAgICBzdWJncmFwaCBBUEvov5vnqIs8VENQ5a6i5oi356uvPlxuICAgIGFwa19zb2NrZXQoc29ja2V0KS0tPmFwa19jb25uZWN0KGNvbm5lY3QpXG4gICAgYXBrX2Nvbm5lY3QoY29ubmVjdCktLT5hcGtfd3JpdGUod3JpdGUpXG4gICAgYXBrX3dyaXRlKHdyaXRlKS0tPmFwa19yZWFkKHJlYWQpXG4gICAgYXBrX3JlYWQocmVhZCktLT5hcGtfd3JpdGUod3JpdGUpXG4gICAgYXBrX3JlYWQocmVhZCktLT5hcGtfY2xvc2UoY2xvc2UpXG4gICAgZW5kICAgIFxuICAgIGFwa19jb25uZWN0KGNvbm5lY3QpLS3lu7rnq4vov57mjqUtLS1UQ1BfcmVhZChyZWFkKVxuICAgIGFwa193cml0ZSh3cml0ZSktLeivt-axguaVsOaNri0tLVRDUF9yZWFkKHJlYWQpXG4gICAgVENQX3dyaXRlKHdyaXRlKS0t5Zue5bqU5pWw5o2uLS0-YXBrX3JlYWQocmVhZClcbiAgICBhcGtfY2xvc2UoY2xvc2UpLS3nu5PmnZ_ov57mjqUtLT5UQ1BfcmVhZDIocmVhZCkgICIsIm1lcm1haWQiOnsidGhlbWUiOiJkZWZhdWx0In19),[Mermaid Live Editor](https://mermaidjs.github.io/mermaid-live-editor/#/edit/eyJjb2RlIjoiZ3JhcGggVEJcbiAgICBhcGtfY29ubmVjdChjb25uZWN0KS0t5bu656uL6L-e5o6lLS0tVENQX3JlYWQocmVhZClcblx0YXBrX3dyaXRlKHdyaXRlKS0t6K-35rGC5pWw5o2uLS0tVENQX3JlYWQocmVhZClcblx0VENQX3dyaXRlKHdyaXRlKS0t5Zue5bqU5pWw5o2uLS0-YXBrX3JlYWQocmVhZClcblx0YXBrX2Nsb3NlKGNsb3NlKS0t57uT5p2f6L-e5o6lLS0-VENQX3JlYWQyKHJlYWQpICAgXG4gICAgc3ViZ3JhcGggVENQ5pyN5Yqh5ZmoPOWuiOaKpOi_m-eoiz5cbiAgICBUQ1Bfc29ja2V0KHNvY2tldCktLT5UQ1BfYmluZChiaW5kKVxuICAgIFRDUF9iaW5kKGJpbmQpLS0-VENQX2xpc3RlbihsaXN0ZW4pXG4gICAgVENQX2xpc3RlbihsaXN0ZW4pLS0-VENQX2FjY2VwdChhY2NlcHQpXG4gICAgVENQX2FjY2VwdChhY2NlcHQpLS3pmLvloZ7nm7TliLDlrqLmiLfnq6_ov57mjqUtLT5UQ1BfcmVhZChyZWFkKVxuICAgIFRDUF9yZWFkKHJlYWQpLS3lpITnkIbor7fmsYItLT5UQ1Bfd3JpdGUod3JpdGUpXG4gICAgVENQX3dyaXRlKHdyaXRlKS0tPlRDUF9yZWFkKHJlYWQpXG4gICAgVENQX3dyaXRlKHdyaXRlKS0tPlRDUF9yZWFkMihyZWFkKVxuICAgIFRDUF9yZWFkMihyZWFkKS0tPlRDUF9jbG9zZShjbG9zZSlcbiAgICBlbmQgICAgXG4gICAgc3ViZ3JhcGggQVBL6L-b56iLPFRDUOWuouaIt-errz5cbiAgICBhcGtfc29ja2V0KHNvY2tldCktLT5hcGtfY29ubmVjdChjb25uZWN0KVxuICAgIGFwa19jb25uZWN0KGNvbm5lY3QpLS0-YXBrX3dyaXRlKHdyaXRlKVxuICAgIGFwa193cml0ZSh3cml0ZSktLT5hcGtfcmVhZChyZWFkKVxuICAgIGFwa19yZWFkKHJlYWQpLS0-YXBrX3dyaXRlKHdyaXRlKVxuICAgIGFwa19yZWFkKHJlYWQpLS0-YXBrX2Nsb3NlKGNsb3NlKVxuICAgIGVuZCAiLCJtZXJtYWlkIjp7InRoZW1lIjoiZGVmYXVsdCJ9fQ)
