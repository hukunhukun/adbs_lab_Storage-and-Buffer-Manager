#include "BMgr.h"
#include <fstream>
#include <iomanip>
#include <windows.h>
using namespace std;

class test{
private:
    int IOcounts;
    int Hitcounts;
    int Accesscounts;
    double time;
    BMgr BMg;
    bFrame bfrm;
    string filename;
    string tracefile;
    string logfilename;
    //可以初始化bframe


public:
    test(){
        IOcounts=0,
        Hitcounts=0,
        Accesscounts=0,
        time=0,
        filename="data.dbf";
        tracefile = "D:/work/class/adbs/adbs_lab/data-5w-50w-zipf.txt";
        logfilename = "./log/log.txt";
        memset(bfrm.field,'+',sizeof(bfrm));
        BMg;
        BMg.frm = bfrm;
        }


    void SetFile(string filename,string tracefile){
        this->filename = filename;
        this->tracefile = tracefile;
    }


    //创建NUMpage的堆目录文件
    void create_data(int MAXPAGES){
        cout<<"Creating data.dbf page by page ......"<<endl;
        for(int i=0;i<MAXPAGES;i++){
            BMg.FixNewPage(i);
            BMg.DSM.IncNumPages();
        }
    }


    void AccessCoderTalker(access_format log){
        int page_id = log.page_id;
        bool rw = log.rw;
        int frame_id;
        //读取page_id的文件
        if(BMg.In_buffer(page_id)){
            frame_id = BMg.FixPage(page_id,0);
            Hitcounts++;
            BMg.lrulist.UpdateList(frame_id);
        }
        else{
            
            //not in buffer
            frame_id = BMg.FixPage(page_id,0);
            this->IOcounts++;
            //obtain from disk,openfile
            // BMg.DSM.OpenFile(filename);
            // bfrm = BMg.DSM.ReadPage(page_id);
            // BMg.DSM.CloseFile();
            LRUEle *node = new LRUEle(frame_id);
            BMg.lrulist.InsertEle(node);

        }
        if(rw==1){
            BMg.SetDirty(frame_id);
        }
        Accesscounts++;
    }

    void LoopAccess(int TESTCOUNT){
        access_format *dataarray = new access_format[TESTCOUNT];
        float *hitarray = new float[TESTCOUNT];
        int *IOarray = new int[TESTCOUNT];
        ifstream inFile(tracefile);

        string tmp,tmpPre,tmpSuf;
        int commaIndex,dataRequestIndex = 0;
	    while (dataRequestIndex < TESTCOUNT){
            getline(inFile,tmp);
            commaIndex = tmp.find(",");
            tmpPre = tmp.substr(0,commaIndex);
            tmpSuf = tmp.substr(commaIndex + 1);
            dataarray[dataRequestIndex].rw = atoi(tmpPre.c_str());
            dataarray[dataRequestIndex].page_id = atoi(tmpSuf.c_str())-1;
            dataRequestIndex++;
	    }


        //计时开始
        LARGE_INTEGER t1,t2,tc;
        QueryPerformanceFrequency(&tc);
        QueryPerformanceCounter(&t1);
        cout<<"starting trace test ......"<<endl;
        for(int i=0;i<TESTCOUNT;i++){
            AccessCoderTalker(dataarray[i]);
            hitarray[i] = float(Hitcounts)/Accesscounts;
            IOarray[i] = IOcounts+BMg.writeIO;
            if((i)%(TESTCOUNT/100)==0){
                cout<<left<<"Numbers:"<<setw(6)<<Accesscounts<<"   "<<"  Hit counts:"<<setw(6)<<Hitcounts<<"  IO counts:"<<setw(6)<<IOcounts+BMg.writeIO<<
                "  Hit rate:"<<fixed<<setprecision(4)<< float(Hitcounts)/Accesscounts<<endl;
            }
        }
        BMg.DSM.CloseFile();
        BMg.WriteDirtys();
        QueryPerformanceCounter(&t2);
        delete []dataarray;
        time=(double)(t2.QuadPart-t1.QuadPart)/(double)tc.QuadPart; 
        cout<<"total time = "<<time<<" s"<<endl;

        fstream logfile;
        logfile.open(logfilename,ios::out);
        logfile<<left<<setw(6)<<"Hit rate"<<"\t"<<setw(6)<<"IO counts"<<"\n";
        for(int i=0;i<TESTCOUNT;i++){
            logfile<<left<<fixed<<setprecision(4)<<hitarray[i]<<"\t \t \t"<<setw(6)<<IOarray[i]<<"\n";
        }
        logfile.close();
    }

};