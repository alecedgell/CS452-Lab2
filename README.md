Name: Alec Edgell
Assignment: CS 452 – Secure Inter-process Communication (IPC)


                        README

The code implements a small program called sendfile that allows the user 
to send and receive encrypted files through TCP.
The code uses a class called Connection, which uses sockets and file streams 
in order to send and receive files. The send part is taken as a client that 
connects to a server. The receive part is implemented as a server that waits 
for a connection and saves the received data to a file.

How to compile:

The program is provided with a makefile that can be used to compile the program
simply using the command:

    make

How to execute:

After compiling the program, an executable called "run" is generated in the 
current directory and it can be started using the following format:

    ./run [server | client]

For example, to start a client:

    ./run client

And for starting a server:

    ./run server

The output of the program shows the encrypted data read from the file in the case
of the client, and the encrypted data received from the client, in the case of 
the server.
