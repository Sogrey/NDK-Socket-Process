# NDK-Socket-Process
ndk socket 双线程守护

[![CodeFactor](https://www.codefactor.io/repository/github/sogrey/ndk-socket-process/badge)](https://www.codefactor.io/repository/github/sogrey/ndk-socket-process)




![](https://raw.githubusercontent.com/Sogrey/NDK-Socket-Process/master/pic.gif)

```` mermaid`

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
使用[Typora](https://www.typora.io/)查看或点击[流程图](https://mermaidjs.github.io/mermaid-live-editor/#/view/eyJjb2RlIjoiZ3JhcGggVEJcbiAgICBzdWJncmFwaCBUQ1DmnI3liqHlmag85a6I5oqk6L-b56iLPlxuICAgIFRDUF9zb2NrZXQoc29ja2V0KS0tPlRDUF9iaW5kKGJpbmQpXG4gICAgVENQX2JpbmQoYmluZCktLT5UQ1BfbGlzdGVuKGxpc3RlbilcbiAgICBUQ1BfbGlzdGVuKGxpc3RlbiktLT5UQ1BfYWNjZXB0KGFjY2VwdClcbiAgICBUQ1BfYWNjZXB0KGFjY2VwdCktLemYu-WhnuebtOWIsOWuouaIt-err-i_nuaOpS0tPlRDUF9yZWFkKHJlYWQpXG4gICAgVENQX3JlYWQocmVhZCktLeWkhOeQhuivt-axgi0tPlRDUF93cml0ZSh3cml0ZSlcbiAgICBUQ1Bfd3JpdGUod3JpdGUpLS0-VENQX3JlYWQocmVhZClcbiAgICBUQ1Bfd3JpdGUod3JpdGUpLS0-VENQX3JlYWQyKHJlYWQpXG4gICAgVENQX3JlYWQyKHJlYWQpLS0-VENQX2Nsb3NlKGNsb3NlKVxuICAgIGVuZCAgICBcbiAgICBzdWJncmFwaCBBUEvov5vnqIs8VENQ5a6i5oi356uvPlxuICAgIGFwa19zb2NrZXQoc29ja2V0KS0tPmFwa19jb25uZWN0KGNvbm5lY3QpXG4gICAgYXBrX2Nvbm5lY3QoY29ubmVjdCktLT5hcGtfd3JpdGUod3JpdGUpXG4gICAgYXBrX3dyaXRlKHdyaXRlKS0tPmFwa19yZWFkKHJlYWQpXG4gICAgYXBrX3JlYWQocmVhZCktLT5hcGtfd3JpdGUod3JpdGUpXG4gICAgYXBrX3JlYWQocmVhZCktLT5hcGtfY2xvc2UoY2xvc2UpXG4gICAgZW5kICAgIFxuICAgIGFwa19jb25uZWN0KGNvbm5lY3QpLS3lu7rnq4vov57mjqUtLS1UQ1BfcmVhZChyZWFkKVxuICAgIGFwa193cml0ZSh3cml0ZSktLeivt-axguaVsOaNri0tLVRDUF9yZWFkKHJlYWQpXG4gICAgVENQX3dyaXRlKHdyaXRlKS0t5Zue5bqU5pWw5o2uLS0-YXBrX3JlYWQocmVhZClcbiAgICBhcGtfY2xvc2UoY2xvc2UpLS3nu5PmnZ_ov57mjqUtLT5UQ1BfcmVhZDIocmVhZCkgICIsIm1lcm1haWQiOnsidGhlbWUiOiJkZWZhdWx0In19),[Mermaid Live Editor](https://mermaidjs.github.io/mermaid-live-editor/#/edit/eyJjb2RlIjoiZ3JhcGggVEJcbiAgICBzdWJncmFwaCBUQ1DmnI3liqHlmag85a6I5oqk6L-b56iLPlxuICAgIFRDUF9zb2NrZXQoc29ja2V0KS0tPlRDUF9iaW5kKGJpbmQpXG4gICAgVENQX2JpbmQoYmluZCktLT5UQ1BfbGlzdGVuKGxpc3RlbilcbiAgICBUQ1BfbGlzdGVuKGxpc3RlbiktLT5UQ1BfYWNjZXB0KGFjY2VwdClcbiAgICBUQ1BfYWNjZXB0KGFjY2VwdCktLemYu-WhnuebtOWIsOWuouaIt-err-i_nuaOpS0tPlRDUF9yZWFkKHJlYWQpXG4gICAgVENQX3JlYWQocmVhZCktLeWkhOeQhuivt-axgi0tPlRDUF93cml0ZSh3cml0ZSlcbiAgICBUQ1Bfd3JpdGUod3JpdGUpLS0-VENQX3JlYWQocmVhZClcbiAgICBUQ1Bfd3JpdGUod3JpdGUpLS0-VENQX3JlYWQyKHJlYWQpXG4gICAgVENQX3JlYWQyKHJlYWQpLS0-VENQX2Nsb3NlKGNsb3NlKVxuICAgIGVuZCAgICBcbiAgICBzdWJncmFwaCBBUEvov5vnqIs8VENQ5a6i5oi356uvPlxuICAgIGFwa19zb2NrZXQoc29ja2V0KS0tPmFwa19jb25uZWN0KGNvbm5lY3QpXG4gICAgYXBrX2Nvbm5lY3QoY29ubmVjdCktLT5hcGtfd3JpdGUod3JpdGUpXG4gICAgYXBrX3dyaXRlKHdyaXRlKS0tPmFwa19yZWFkKHJlYWQpXG4gICAgYXBrX3JlYWQocmVhZCktLT5hcGtfd3JpdGUod3JpdGUpXG4gICAgYXBrX3JlYWQocmVhZCktLT5hcGtfY2xvc2UoY2xvc2UpXG4gICAgZW5kIFxuICAgIGFwa19jb25uZWN0KGNvbm5lY3QpLS3lu7rnq4vov57mjqUtLS1UQ1BfcmVhZChyZWFkKVxuXHRhcGtfd3JpdGUod3JpdGUpLS3or7fmsYLmlbDmja4tLS1UQ1BfcmVhZChyZWFkKVxuXHRUQ1Bfd3JpdGUod3JpdGUpLS3lm57lupTmlbDmja4tLT5hcGtfcmVhZChyZWFkKVxuXHRhcGtfY2xvc2UoY2xvc2UpLS3nu5PmnZ_ov57mjqUtLT5UQ1BfcmVhZDIocmVhZCkgICAiLCJtZXJtYWlkIjp7InRoZW1lIjoiZGVmYXVsdCJ9fQ)

