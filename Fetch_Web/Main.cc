#include <stdlib.h>
#include <pthread.h>

#include "Html_Parser.h"
#include "SocketNet.h"
#include "LinkedList.h"
#include "DButility.h"
#include "MySql_conn.h"
#include "Ip_generator.h"
#include "Timer.h"

using namespace std;

int MAX = 3;
int ProcType = 0; //Thread = 0; Fork = 1

int WebCounter = 0;

//Thread mutex for acquiring DB tables primary key Id
pthread_mutex_t GetPrimaryIdMutex = PTHREAD_MUTEX_INITIALIZER;

void *ThreadRandomWebFetch(void*);
void ForkRandomWebFetch(void*);

int main(int argc, char** argv) {

    if (ProcType == 0) {
        //Thread system
        pthread_t thread_id[MAX];
        int status, *p_status = &status;

        while (1) {
            printf("Starting Thread\n");
            //Creation of threads
            for (int i = 0; i < MAX; i++) {
                if (pthread_create(&thread_id[i], NULL, ThreadRandomWebFetch, (void*) i) > 0) {
                    printf("pthread_create error.\n");
                }
            }
            //Joining of threads
            printf("Joining Thread\n");
            for (int i = 0; i < MAX; i++) {
                if (pthread_join(thread_id[i], (void **) p_status) > 0) {
                    printf("pthread_join error.\n");
                }
            }
        }
        //END of Thread

        while (1) {
            sleep(1);
            printf("Processing job\n");
        }
    } else if (ProcType == 1) {
        //Forking system

        pid_t ChildPid;

        int j = 0;

        printf("Starting fork.\n");
        for (j = 0; j < MAX; j++) {
            if ((ChildPid = fork()) == 0) {
                printf("ChildId: %i\n", j);
                //sleep(5);
                break;
            }
            sleep(5);
        }
        ForkRandomWebFetch((void*) j);
        //End

    }
    return (EXIT_SUCCESS);
}

void *ThreadRandomWebFetch(void* ThreadId) {

    int tid;
    tid = (long) ThreadId;

    string Ipgen;

    string Ipname;
    char* Ipaddr;

    Timer * t = new Timer();
    t->Start_Timer();

    int web_id;
    LinkedList *MyList;

    Ip_generator *ip_gen = new Ip_generator();

    DButility *DButil1 = new DButility("127.0.0.1", "root", "password", "DBdata", 3310, "/tmp/mysql1.sock");
    DButility *DButil2 = new DButility("127.0.0.1", "root", "password", "DBdata", 3311, "/tmp/mysql2.sock");

    while (1)
        //for(int i = 0; i < 1000;i++)
    {
        Html_Parser *HTMWebData = new Html_Parser();

        int conn_stat = 0;

        Ipgen = ip_gen->IP_random();

        //printf("Web: %s\n", Ipgen.c_str());

        if ((conn_stat = HTMWebData->GetHTTPWebSite(Ipgen, 80)) == 0) {

            Ipname = HTMWebData->Ipname;
            Ipaddr = HTMWebData->Ipaddress;

            if ((DButil1->checkDBipname_exist(Ipname)) == 0 && (DButil2->checkDBipname_exist(Ipname)) == 0) {

                t->End_Timer();
                t->CalculateElapseTime();
                t->print();

                printf("Thread# %i\n", tid);
                printf("IP Name: %s\n", Ipname.c_str());
                printf("IP Address: %s\n", Ipaddr);
                printf("WebCounter# %i\n\n", WebCounter);

                //printf("Mutex access!\n");
                pthread_mutex_lock(&GetPrimaryIdMutex);
                web_id = DButil1->put_IPData(Ipname, Ipaddr);
                WebCounter++;
                pthread_mutex_unlock(&GetPrimaryIdMutex);

                if (HTMWebData->HTTPFetchWebData(Ipname) == 0) {
                    HTMWebData->ParseData(HTMWebData->GetPTRData(), "<title", "</title", 0);
                    MyList = HTMWebData->LnkListData;
                    DButil1->TitleDataLinked(web_id, *MyList);
                    //cout << endl;

                    HTMWebData->ParseData(HTMWebData->GetPTRData(), "<meta", "</", 0);
                    MyList = HTMWebData->LnkListData;
                    DButil1->MetaDataLinked(web_id, *MyList);
                    //cout << endl;

                    HTMWebData->ParseData(HTMWebData->GetPTRData(), "<a", "</a", 0);
                    MyList = HTMWebData->LnkListData;
                    DButil1->AnchorDataLinked(web_id, *MyList);
                    //cout << endl;

                    HTMWebData->ParseData(HTMWebData->GetPTRData(), "<body", "</body", 0);
                    MyList = HTMWebData->LnkListData;
                    DButil1->BodyDataLinked(web_id, *MyList);
                    cout << endl;
                }

                t->Start_Timer();

            }
        }

        delete HTMWebData;
    }

    delete ip_gen;

    delete DButil1;
    delete DButil2;

    printf("\nDONE!\n");
}

void ForkRandomWebFetch(void* ForkId) {

    int counter = 0;
    int tid;
    tid = (long) ForkId;

    string Ipgen;

    string Ipname;
    char* Ipaddr;

    Timer * t = new Timer();
    t->Start_Timer();

    int web_id;
    LinkedList *MyList;

    Ip_generator *ip_gen = new Ip_generator();

    //DButility *DButil1 = new DButility("localhost", "root", "password", "DBdata", 3306, "/tmp/mysql.sock");
    DButility *DButil1 = new DButility("127.0.0.1", "root", "password", "DBdata", 3310, "/tmp/mysql1.sock");
    DButility *DButil2 = new DButility("127.0.0.1", "root", "password", "DBdata", 3311, "/tmp/mysql2.sock");

    while (1)
        //for(int i = 0; i < 1000;i++)
    {
        Html_Parser *HTMWebData = new Html_Parser();

        int conn_stat = 0;

        Ipgen = ip_gen->IP_random();

        //printf("Web: %s\n", Ipgen.c_str());

        if ((conn_stat = HTMWebData->GetHTTPWebSite(Ipgen, 80)) == 0) {

            Ipname = HTMWebData->Ipname;
            Ipaddr = HTMWebData->Ipaddress;

            //printf("Fork# %i\n", tid);
            //printf("Found IP Name: %s\n", Ipname.c_str());
            //printf("Found IP Address: %s\n\n", Ipaddr);

            if ((DButil1->checkDBipname_exist(Ipname)) == 0 && (DButil2->checkDBipname_exist(Ipname)) == 0) {

                t->End_Timer();
                t->CalculateElapseTime();
                t->print();

                printf("Fork# %i\n", tid);

                counter++;
                printf("Web Count# %i\n", counter);

                printf("Inserting IP Name: %s\n", Ipname.c_str());
                printf("Inserting IP Address: %s\n\n", Ipaddr);

                web_id = DButil1->put_IPData(Ipname, Ipaddr);

                if (HTMWebData->HTTPFetchWebData(Ipname) == 0) {
                    HTMWebData->ParseData(HTMWebData->GetPTRData(), "<title", "</title", 0);
                    MyList = HTMWebData->LnkListData;
                    DButil1->TitleDataLinked(web_id, *MyList);
                    //cout << endl;

                    HTMWebData->ParseData(HTMWebData->GetPTRData(), "<meta", "</", 0);
                    MyList = HTMWebData->LnkListData;
                    DButil1->MetaDataLinked(web_id, *MyList);
                    //cout << endl;

                    HTMWebData->ParseData(HTMWebData->GetPTRData(), "<a", "</a", 0);
                    MyList = HTMWebData->LnkListData;
                    DButil1->AnchorDataLinked(web_id, *MyList);
                    //cout << endl;

                    HTMWebData->ParseData(HTMWebData->GetPTRData(), "<body", "</body", 0);
                    MyList = HTMWebData->LnkListData;
                    DButil1->BodyDataLinked(web_id, *MyList);
                    cout << endl;
                }

                t->Start_Timer();

            }
        }

        delete HTMWebData;

    }

    delete ip_gen;

    delete DButil1;
    delete DButil2;

    printf("\nDONE!\n");
}
