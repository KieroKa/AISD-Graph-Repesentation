#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <math.h>

using namespace std;

#define d 2/10

ofstream pomiar("grafy.txt");

struct lista
{
	int w;
	lista* nast;
};

int N=5500;
int nr = 0;
int v;

void dfs(int* begin, int* end, bool* visited, int N, int v, lista **SL)
{

	visited[v] = 1;
	begin[v] = ++nr; //poczatek analizy wierzcholka

	lista *nastepnik = SL[v];

	while (nastepnik != NULL)
    {
		if (visited[(*nastepnik).w] == 0) dfs(begin, end, visited, N, (*nastepnik).w, SL);
		nastepnik = (*nastepnik).nast;
	}

	end[v] = ++nr; //koniec analizy wierzcholka

}

int main() //funkcja glówne
{

 srand(time(NULL));
 int x, y;
 int m=N*N*d;

//             TWORZENIE GRAFU

 	// Neighborhood Matrix

 	cout<<"Tworzenie macierzy"<<endl;

	int **NM = new int* [N];

	for(x=0; x<N; x++)
		NM[x] = new int [N];

        for(x=0;x<N;x++) // tablica wypelniona zerami
        {
            for(y=0;y<N;y++)
                {
                    NM[x][y]=0;
                }
        }

	// Arc list

	int i=m;
	while(i)
    {
		x = rand()%N; //losowy wierzcholek (od 0 do n-1)
		y = rand()%N;

		if(NM[x][y] != 1 && x!=y )
        {
			NM[x][y] = 1;
			i--;
		}
	}
   /* for(x=0;x<N;x++){
        for(y=0;y<N;y++){
            cout<<NM[x][y]<<" ";}
            cout<<endl;}*/

	// Arc list

    cout<<"Tworzenie listy lukow"<<endl;

	int **AL = new int* [2];
	AL[0] = new int[m];
	AL[1] = new int[m];

	for(i=0, x=0; x<N; x++)
        {
		for(y=0; y<N; y++)
            {
			if(NM[x][y]==1)
                {
				AL[0][i] = x;
				AL[1][i] = y;
				i++;
                }
            }
        }

	// Successor list

cout<<"Tworzenie listy nastepnikow:"<<endl;

    lista **SL= new lista* [N];
	lista *ostatni=new lista, *nowy;
    for(int i=0;i<N;i++) SL[i]=NULL;

	for(x=0; x<N; x++)
        {
		for(y=0; y<N; y++)
            {
			if (NM[x][y]==1)
                {
				if (SL[x] == NULL)
                    { 	// dodawanie pierwszego elementu
					SL[x] = new lista;
					ostatni = SL[x];
					(*ostatni).w = y;
                    }
				else
                    {	// dodawanie na koncu
					nowy = new lista;
					(*nowy).nast=NULL;
					(*nowy).w = y;
					(*ostatni).nast = nowy;
					ostatni = nowy;
                    }
                }
            }
        }

    cout<<"koniec NA"<<endl;

	// SORTOWANIE TOPOLOGICZNE


    bool *visited = new bool[N];
	int *begin = new int[N];
	int *end = new int[N];

	double_t start=1.0*clock()/CLOCKS_PER_SEC;

	 for (x=0; x<N; x++)
		if (visited[x] == 0)
			dfs(begin, end, visited, N, x, SL);


    double_t koniec=1.0*clock()/CLOCKS_PER_SEC;

    pomiar<<"Etykietowanie (NA):\n"<<koniec-start<<endl;


	// zliczanie luków powrotnych

	int luki_powrotne_AL=0;
	int luki_powrotne_NM=0;
	int luki_powrotne_SL=0;

    // Luki powrotne AL

    double_t start1=1.0*clock()/CLOCKS_PER_SEC;
	for (x=0; x<m; x++)
		if ( begin[AL[1][x]] < begin[AL[0][x]] && end[AL[0][x]] < end[AL[1][x]] )
			{luki_powrotne_AL++;};

    double_t koniec1=1.0*clock()/CLOCKS_PER_SEC;
    pomiar<<"Luki powrotne (AA):\n"<<koniec1-start1<<endl;

	cout << "\nluki powrotne (AA): " << luki_powrotne_AL << " / " << m << endl;

	//Luki powrotne NM
	double_t start2=1.0*clock()/CLOCKS_PER_SEC;

	for (x=0;x<N;x++)
    {
        for (y=0;y<N;y++)
        {if(NM[x][y]==1)
            {if(begin[y]<begin[x]&&begin[x]<end[x]&&end[x]<end[y])
            luki_powrotne_NM++;
            }
        }
    }
    double_t koniec2=1.0*clock()/CLOCKS_PER_SEC;
    pomiar<<"Luki powrotne (MN):\n"<<koniec2-start2<<endl;
    cout<<"Luki powrotne (MN):"<<luki_powrotne_NM<<" / " <<m<<endl;

        //Luki powrotne SL
    double_t start3=1.0*clock()/CLOCKS_PER_SEC;
    for(int i=0;i<N;i++)
    {
        lista *wierzcholek = SL[i];
        while(wierzcholek !=NULL)
        {
         if(begin[(*wierzcholek).w]<begin[i]&&begin[i]<end[i]&&end[i]<end[(*wierzcholek).w]) luki_powrotne_SL++;
        wierzcholek=(*wierzcholek).nast;
        }
    }
    double_t koniec3=1.0*clock()/CLOCKS_PER_SEC;
    pomiar<<"Luki powrotne (NA):\n"<<koniec3-start3<<endl;
    cout<<"Luki powrotne (NA):"<<luki_powrotne_SL<<" / "<<m<<endl;
    return 0;
}
