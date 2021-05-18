## 1.websocket简介：
websocket提供浏览器的双端通信，服务器可主动推送信息到浏览器。应用层协议，基于TCP连接，使用Http进行握手。

## 2.websocket与http:
websocket http 都是应用层协议，websocket握手使用了http协议，其他并无什么关系。websocket支持长连接，双端通信。
http短连接，主动请求。

## 3.握手：
客户端使用TCP进行连接后，发送协议进行握手。握手采用通过Http协议。
握手步骤：
### 3.1.client->server
```http
GET / HTTP/1.1
Host: 192.168.2.199:5250
Connection: Upgrade
Pragma: no-cache
Cache-Control: no-cache
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.90 Safari/537.36
Upgrade: websocket
Origin: file://
Sec-WebSocket-Version: 13
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.9
Sec-WebSocket-Key: uIRS104Uc9gTYH0P+lHBfg==
Sec-WebSocket-Extensions: permessage-deflate; client_max_window_bits
```
Sec-WebSocket-Key:客户端随机生成的key
Upgrade：通知我要使用websocket协议
### 3.2.server->client
```http
HTTP/1.1 101 Switching Protocols
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Accept: YXrMZ6P4meox0Yqv8s1WNj36ZgE=
```
Sec-WebSocket-Accept:
Sec-WebSocket-Accept = base64(sha1(Sec-WebSocket-Key))
客户端通过Sec-WebSocket-Accept比对，校验是不是一个合法的握手回应。
sec-key算法是公开透明的，只是为了避免一些意外连接情况。
Upgrade：升级为websocket协议，以后通信传输使用websocket协议。

## 4.数据传输：
传输协议：一个包可以拆分成多个帧
每帧数据格式：
```
 0                   1                   2                   3
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 +-+-+-+-+-------+-+-------------+-------------------------------+
 |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
 |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
 |N|V|V|V|       |S|             |   (if payload len==126/127)   |
 | |1|2|3|       |K|             |                               |
 +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
 |     Extended payload length continued, if payload len == 127  |
 + - - - - - - - - - - - - - - - +-------------------------------+
 |                               |Masking-key, if MASK set to 1  |
 +-------------------------------+-------------------------------+
 | Masking-key (continued)       |          Payload Data         |
 +-------------------------------- - - - - - - - - - - - - - - - +
 :                     Payload Data continued ...                :
 + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
 |                     Payload Data continued ...                |
 +---------------------------------------------------------------+
```
FIN：1 一个包的最后一帧 
MASK：数据是否进行了mask操作
Payload len： Payload len === X
	X<126:X为Payload Data 长度
	X==126：后面两个字节为 Payload Data 长度
	X==127：后面8个字节为 Payload Data 长度
Masking-key：掩码  MASK ==1时才有这个数据
Payload Data ：数据
```C
struct  opthdr {
    unsigned char opcode:4,
        rsv3:1,
        rsv2:1,
        rsv1:1,
        fin:1;
    unsigned char payload_len:7,
        mask:1;
}__attribute__((packed));

struct  opthdr_base {
    struct opthdr head;
    char maskkey[4];
    char data[0];
}__attribute__((packed));

struct  opthdr_126 {
    struct opthdr head;
    unsigned short payload_len;
    char maskkey[4];
    char data[0];
}__attribute__((packed));

struct  opthdr_127 {
    struct opthdr head;
    unsigned long long  payload_len;
    char maskkey[4];
    char data[0];
}__attribute__((packed));

void umask(char *data,int len,char *mask){    
	int i;    
	for (i = 0;i < len;i ++)        
		*(data+i) ^= *(mask+(i%4));
}

```


blog:https://zhuanlan.zhihu.com/p/32739737