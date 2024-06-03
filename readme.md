## 简介

WebServer是一个支持**http**、**websocket**的**可扩展**、**跨平台**（目前支持linux、MACOS，暂不支持windows)、**高性能**（其实并没测试过）、**轻量级**的C++网络服务器。

本项目主要用于学习c++11语法以及常用的系统调用，主要特点如下：
* 使用线程池 + 非阻塞socket + epoll + 事件处理（Reactor）的并发模型
* 使用状态机解析http请求报文
* 使用前缀树实现url路由、使用小顶堆实现定时器、实现异步日志系统
* 可进行源码级扩展以及通过CGI进行应用级扩展
* 支持websocket协议
* 不依赖任何第三方库

## 编译

在linux下：`make os=LINUX`
在MAC下：`make os=MAC`

编译完之后，会在项目下创建一个**build**目录，目录下**server**文件为编译后的可执行文件。