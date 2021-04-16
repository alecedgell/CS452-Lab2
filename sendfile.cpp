#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;

class Connection {
    int port;
    string address;
    string filename;
    int pktsize;
    string key;
    FILE *file;
    string repkey;

    int sockfd;
    struct sockaddr_in sock_addr; 
    socklen_t addr_len;

public:
    Connection(string addr, int portn, string fname, int pktsz, string ekey) : port(portn), 
        address(addr), filename(fname), pktsize(pktsz), key(ekey) {

        addr_len = sizeof(struct sockaddr_in);

        memset(&sock_addr, 0, sizeof(sock_addr));
        sock_addr.sin_family = AF_INET;
        sock_addr.sin_addr.s_addr = inet_addr(address.c_str());
        sock_addr.sin_port = htons(port);

        //open the socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        //make repeated key of size pkt
        repkey = "";
        for (int i = 0; i < pktsize / ekey.length(); i++) {
            repkey += ekey;
        }
        for (int i = 0; i < pktsize % ekey.length(); i++)
            repkey += ekey[i];
    }

    void startServer() {
        char data[pktsize];

        int client_sock;
        struct sockaddr_in client_addr; 

        if (filename != "")
            file = fopen(filename.c_str(), "w");
        else
            file = stdout;

        //optionally bind() the sock
        bind(sockfd, (struct sockaddr *)&sock_addr, sizeof(struct sockaddr));
        //set listen to up to 5 queued connections
        listen(sockfd, 5);
        //could put the accept procedure in a loop to handle multiple clients
        //accept a client connection
        client_sock = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);
        int packet = 0;
        int size;
        while((size = read(client_sock, data, pktsize)) > 0) {
            if (filename != "")
            {
                cout << "Rec packet# " << dec << packet << " - encrypted as ";

                for (int i = 0; i < 2; i++)
                    cout << hex << setw(2) << setfill('0') << ((unsigned)(data[i]) &0xFF);
                cout << " ... ";
                for (int i = size - 2; i < size; i++)
                    cout << hex << setw(2) << setfill('0') << ((unsigned)(data[i]) &0xFF);
                cout <<  endl;
            }
            // decrypt
            for (int i = 0; i < size; i++) {
                data[i] ^= repkey[i];
            }
            fwrite(data, 1, size, file);
            packet++;
        }
        close(client_sock);
        close(sockfd);
        if (filename != "")
            fclose(file);
        cout << "Receive success!" << endl;
    }

    void startClient() {
        char data[pktsize];

        file = fopen(filename.c_str(), "r");
        if (file == NULL) {
            cout << "Unable to open file to send!" << endl;
            close(sockfd);
        }
        if (connect(sockfd, (struct sockaddr *) &sock_addr, addr_len) < 0) {
            perror("connect");
            exit(1);
        }
        int packet = 0;
        while (!feof(file)) {
            memset(data, 0, pktsize);
            size_t n = fread(data, 1, pktsize, file);
            int size = pktsize;
            if (n < size)
                size = n;
            for (int i = 0; i < size; i++) {
                data[i] ^= repkey[i];
            }
            write(sockfd, data, size);
            cout << "Sent encrypted packet# " << dec << packet << " - encrypted as ";
            for (int i = 0; i < 2; i++)
                cout << hex << setw(2) << setfill('0') << ((unsigned)(data[i]) &0xFF);
            cout << " ... ";
            for (int i = size - 2; i < size; i++)
                cout << hex << setw(2) << setfill('0') << ((unsigned)(data[i]) &0xFF);
            cout <<  endl;
            packet++;
        }
        close(sockfd);
        fclose(file);
        cout << "Send success!" << endl;
    }
};

int main(int argc, char **argv)
{
    if (argc != 2) {
        cout << "Usage: " <<  argv[0] << " [server|client]" << endl;
        return 0;
    }
    string type = string(argv[1]);
    if (type != "server" && type != "client") {
        cout <<  "Connection type must be either \"server\" or \"client\"" << endl;
        return 1;
    }
    string address, filename, key;
    int port, pktsize;

    cout << "Connect to IP address: ";
    getline(cin, address);
    cout << "Port #: ";
    cin >> port;
    cin.get();
    if (type == "client") {
        cout << "File to be sent: ";
        getline(cin, filename);
    }
    else {
        cout << "Save file to: ";
        getline(cin, filename);
    }
    cout << "Pkt size: ";
    cin >> pktsize;
    cin.get();
    cout << "Enter encryption key: ";
    getline(cin, key);

    Connection conn(address, port, filename, pktsize, key);
    if (type == "server") {
        conn.startServer();
    }
    else {
        conn.startClient();
    }
    cout << "MD5:"<< endl;
    execlp("md5sum", "md5sum", filename.c_str(), NULL);
    return 0;
}