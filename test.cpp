#include "test.h"




int main(){

    test demo;
    string filename = "data.dbf";
    string tracefile = "D:/work/class/adbs/adbs_lab/data-5w-50w-zipf.txt";
    demo.SetFile(filename,tracefile);

    //demo.create_data(MAXPAGES);



    demo.LoopAccess(500000);
    // system("pause");
}