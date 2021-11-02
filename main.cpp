#include <fstream>

#include <iostream>
#include <string>
#include <pthread.h>

#define MAX 100

using namespace std;


class Package{
    public:
        int number;
        int n;
        string segment_code[MAX];
        void print(){
            for (int i = n-1; i>=0; i--){
                cout << segment_code[i] << endl;
            }
        }
};

int main(){
    Package p;
    p.segment_code[0]="one";
    p.segment_code[1]="two";
    p.n=2;
    p.print();
    cout << sizeof(Package);

    return 0;
}


/*assignment 1
const string hash_table[] = {"1 1 1 1 1 1 0", "0 1 1 0 0 0 0", "1 1 0 1 1 0 1", "1 1 1 1 0 0 1", 
                            "0 1 1 0 0 1 1", "1 0 1 1 0 1 1", "1 0 1 1 1 1 1", "1 1 1 0 0 0 0",
                            "1 1 1 1 1 1 1", "1 1 1 1 0 1 1"};

void *segmentize(void *x_void_ptr)
{
    //get index to hash
	int digit = stoi(*((string *)x_void_ptr));
    //append segment code to each digit
	*((string *)x_void_ptr) +=  " = " + hash_table[digit];
	return NULL;
}

int main(){
    cout << "enter integer: ";
    long input;
    cin >> input;

    //result in reverse
    static string segments[MAX];
    //threads
    pthread_t tid[MAX];
    
    //i keeps track of individual digit
    int i;
    
    for(i=0; input > 0; i++){
        //extract least significant digit
        int next_digit = input % 10;
        //save each digit as a string to the array
        segments[i] = to_string(next_digit);
        
        //thread creation error
        if (pthread_create(&tid[i], NULL, segmentize, &segments[i])){
            cout << "Error creating thread\n";
            return 1;
        }
        //eliminate least significant digit, move on to next
        input /= 10;
    }

    //join threads and output segment version of each digit
    for (i--; i >= 0; i--){
        pthread_join(tid[i], NULL);
        cout << segments[i] << endl;
    }

    return 0;
}
*/


//Paulette project
/*
int main ()
{
    string name = "8323065542";

    ifstream allSMS;
    ofstream paulette;
    allSMS.open("sms.txt", ios::in);
    paulette.open("paulette.txt", ios::out);

    if(allSMS.is_open() && paulette.is_open()){

        string testStr;
        size_t foundL, foundR;
        while(getline(allSMS, testStr)){
            foundL = testStr.find(name);
            if (foundL!=std::string::npos){
                cout << testStr;
                if ((foundL = testStr.find("body=") != std::string::npos) {
                    foundR = testStr.find("toa=");

                    //!!mms not imported!!

                    testStr = testStr.substr(foundL+5, foundR - foundL - 5);
                    cout << endl << endl << testStr;
                    paulette << testStr << endl << endl;
                }
            }
        }
        allSMS.close();
        paulette.close();
    }
    
  return 0;
}
*/