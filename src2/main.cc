#include "WordQueryServer.h"

using namespace hk;

int main()
{
    WordQueryServer server("../conf/myconf.conf");
    server.start();    
    return 0 ;

}

