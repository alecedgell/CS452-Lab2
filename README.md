# CS 452 – Secure Inter-process Communication (IPC)

## Function
The code implements a small program called sendfile that allows the user 
to send and receive encrypted files through TCP.
The code uses a class called Connection, which uses sockets and file streams 
in order to send and receive files. The send part is taken as a client that 
connects to a server. The receive part is implemented as a server that waits 
for a connection and saves the received data to a file.

## Instructions (in command line)
#### Compile the provided makefile with:
```bash
make
```

#### After compiling the program, execute generated "run" file with command:
```bash
./run [server | client]
```

#### For example, to start a client:
```bash
./run client
```

#### And for starting a server:
```bash
./run server
```

The output of the program shows the encrypted data read from the file in the case
of the client, and the encrypted data received from the client, in the case of 
the server. Example is shown below.



| Client  | Server |
| ------- | ------ |
|Client 1![screenshot-client1](https://user-images.githubusercontent.com/78757687/114965045-78ad2300-9e35-11eb-8bac-b12ad6257061.png)  |Server 1![screenshot-server1](https://user-images.githubusercontent.com/78757687/114965325-fc670f80-9e35-11eb-9985-5d1a1f79fa0c.png)  |
|Client 2![screenshot-client2](https://user-images.githubusercontent.com/78757687/114965078-86fb3f00-9e35-11eb-9f42-15aa32ce8dc5.png)  |Server 2![screenshot-server2](https://user-images.githubusercontent.com/78757687/114963393-46e68d00-9e32-11eb-9b33-97b8315ccd69.png)  |



