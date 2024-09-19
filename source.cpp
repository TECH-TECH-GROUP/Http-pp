#include <controller.h>
#include <connection.h>

int main(){
    Server::Listen(1313,HandlePost,HandleGet);
    return 0;
}