#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int cpt = 0;

vector<uchar> matToByte(Mat image){
    int size = image.total() * image.elemSize();
    cout << "size = " << size << endl;
    vector<uchar> tmp;
    for(int i = 0; i < size; i++)
        tmp.push_back(image.data[i]);
//    char *bytes = new char[size];
//    memcpy(bytes, image.data, size*sizeof(char));
    //cout << sizeof(bytes) << endl;
    cout << "vector size = " << tmp.size() << endl;
    return tmp;
}

int main()
{
    //	Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        return 1;
    }

    //	Create a hint structure for the server we're connecting with
    int port = 54000;
    string ipAddress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    //	Connect to the server on the socket4
    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1)
    {
        return 1;
    }

    //	While loop:
    char buf[2304];
    vector<uchar> userInput;
    int cpt = 0;
    //cout << img << endl;
    Mat image = imread("/home/kindred/Bureau/Licence_3/projet-semestre6/Robot3D/test.jpg",IMREAD_GRAYSCALE);
    imshow("zbruh", image);
    cv::waitKey(0);
    userInput = matToByte(image);
    cout << "taille données = " << userInput.size() << endl;

    int i = 0;

    do {
        //		Enter lines of text
        cout << "> ";
        //getline(cin, userInput);
        //		Send to server
        uchar buffer[2304];
        for(i = 0; i < 2304;i++){
            if(!userInput.empty()){
                buffer[i] = userInput.front();
            }
        }
        userInput.erase(userInput.begin(),userInput.begin()+i);

        cout << "buffer size = "<< strlen(buffer) << endl;
        int sendRes = send(sock, buffer, strlen(buffer), 0);

        if (sendRes == -1)
        {
            cout << "Could not send to server! Whoops!\r\n";
            continue;
        }

        //		Wait for response
        memset(buf, 0, 9600);
        int bytesReceived = recv(sock, buf, 9600, 0);
        if (bytesReceived == -1)
        {
            cout << "There was an error getting response from server\r\n";
        }
        else
        {
            if(string(buf, bytesReceived) == "Tchou"){
                cout << "SERVER> " << string(buf, bytesReceived) << "\r\n";
                break;
            }
            //		Display response
            cout << "SERVER> " << string(buf, bytesReceived) << "\r\n";
        }
        cpt++;
        cin.ignore();
    } while(true);

    //	Close the socket
    close(sock);

    return 0;
}
