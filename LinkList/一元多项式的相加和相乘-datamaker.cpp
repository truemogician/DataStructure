#include <iostream>
#include <ctime>
using namespace std;
const int Nmin=3;
const int Nmax=10;
const int Cmax=8;
const int Imax=15;
int random(int l, int r){
	return l+rand()%(r-l);
}
int main(){
	srand(time(NULL));
	int n=random(Nmin,Nmax+1);
	cout<<n<<endl;
	for (int i=0; i<n; i++)
		cout<<random(-Cmax,Cmax+1)<<' '<<random(0,Imax+1)<<' ';
	cout<<endl;
	n=random(Nmin,Nmax);
	cout<<n<<endl;
	for (int i=0; i<n; i++)
		cout<<random(-Cmax,Cmax+1)<<' '<<random(0,Imax+1)<<' ';
	cout<<endl;
	cout<<rand()%3<<endl;
	return 0;
}
