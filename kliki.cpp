#include <iostream>
using namespace std;

// Постановка задачи
// Есть граф особой структуры: m слоев вершин, в каждом слое по n вершин
// Можно соединять ребрами любые вершины, но только из соседних слоев: 1 и 2, 2 и 3, ..., m-1 и m, m и 1 (то есть зацикливаем)
// Вершины из одного слоя нельзя соединять друг с другом
// Кликой в этом графе называем цепочку, которая соединяет одну вершину 1-го слоя с одной 2-го, эту вершину со 2-го с вершиной из 3-го,
// и так далее, а вершину с m-го слоя с начальной вершиной с 1-го слоя, создавая цепь которая позволяет пройтись по всем слоям

// Имея K штук ребер, задаем вопрос:
// Если есть граф из m слоев вершин, и в каждом слое по n вершин,
// То разместив ребра в графе каким-либо образом (следуя правилу о соседних слоях) заметим что могут появляться клики
// Надо найти max/min количество клик, которое можно получить, как-либо раскидав по графу ровно K ребер

/*

пример: граф содержит 4 слоя, в каждом 5 вершин
Максимум в таком графе может быть 5*5*4 (n*n*m) ребер
Таким образом всевозможные графы описываются всевозможными сочетаниями из n*n*m по K

 * * * * *

 * * * * *

 * * * * *

 * * * * *

 добавив 4 ребра, получим максимум 1 клику, соединив A-B, B-C, C-D, D-A по цепочке 

 A * * * *

 B * * * *

 C * * * *

 D * * * *

 минимум возможно получить 0 клик: A-B, B-C, C-D, D-E - нет цепи - нет клик

 A * E * *

 B * * * *

 C * * * *

 D * * * *

*/

// GLOBALS //
int ***A; //тут хранятся ребра
int ***B; //тут будет храниться граф на котором достигается max
int *perest; //массив хранящий текущее сочетание
int n, m, K; // n - кол-во вершин в слое, m - кол-во слоев в графе, K - кол-во ребер
int count;
// GLOBALS //

void print_array(int *a, int n) {
    for(int i=0; i<n; i++) {
        cout << a[i] << " ";
    }
    cout << endl;
}

void kliki(int sloy, int perviy_stolbec, int pred_stolbec) { // рекурсивный подсчет клик в графе
	if(sloy == 0) {
		for (int j = 0; j < n; j++) {
			if(A[sloy][perviy_stolbec][j] == 1) {
				kliki(sloy+1, perviy_stolbec, j);
			}
		}
		return;
	}
	else if(sloy < m-1) {
			for (int j = 0; j < n; j++) {
				if(A[sloy][pred_stolbec][j] == 1) {
					kliki(sloy+1, perviy_stolbec, j);
				}
			}
		return;
	}
	else
	{
		if(A[sloy][pred_stolbec][perviy_stolbec] == 1) {
			count++;
		}
	}
	return;
}

void kliki_count() { // главная ф-ция для подсчета клик (хранится в count)
	for (int i = 0; i < n; i++)
		kliki(0, i, -1);
}

void perest_v_matricu(){ // сгенерировав сочетание из n*n*m по K, необходимо создать соответствующий ему граф
	int i,j,k;
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			for(k = 0; k < n; k++) {
				A[i][j][k]=0;
			}
		}
	}
	int strk, stlb1, stlb2;
	for (i = 0; i < K; i++)
	{
		stlb2 = perest[i] % n;
		stlb1 = (perest[i] / n) % n;
		strk = perest[i] / (n*n);
		A[strk][stlb1][stlb2] = 1;
	}
}

bool next_perest() { // перебор сочетаний из n*n*m элементов по K
  /*
      Краткая идея:
      хотим сочетания из 3 по 5 (всего их 10):
      --***
      -*-**
       ...
      ***--
      
      тогда можно перебирать их так:
      123, 124, 125, 134, 135, 145, 234, 235, 245, 345
  */
	int s;
	for (s = K-1; s >= 0 ; s--) {
		if (perest[ s ] != n*n*m - K + s) break;
	}
	if( s == -1 ) {
		return false;
	}
	perest[ s ]++;
	for (int i = s+1; i < K ; i++) {
		perest[i] = perest[i-1] + 1;
	}	
	
	return true;
}

void print(int ***Graph) {
	int i,j,k;
	cout << endl;
	for(i=0; i<m; i++)
		for(j=0; j<n; j++)
			for(k=0; k<n; k++)
				cout << i << " " << j << " " << k << "  | " << Graph[i][j][k] << endl;
	cout << endl << endl;
}

void podschet(int *max, int *min) { // подсчет клик - главная ф-ция, K - кол-во ребер
	int i,j,k;
	*max=0, *min=0;
	perest = new int[K];
	for (int i = 0; i < K; i++) {
		perest[i] = i;
	}

	perest_v_matricu();
	
	count = 0;
	kliki_count();
	*max = *min = count;

	while( next_perest(), next_perest() == true ) // вследствие того что графы, соответствующие
                                                // первому и последнему, второму и предпоследнему,
                                                // и т.д. сочетаниям симметричны, можно скипать половину сочетаний из n*n*m по K,
                                                // вызывая функцию next_perest() два раза подряд
                                                // next_perest() превращает текущее сочетание в новое, пока не доберется до последнего
	{
		//cout << "\r"<< sus++ << "  max="<<*max<<" min="<<*min;
		perest_v_matricu();
		count = 0;
		kliki_count();
		
		if(*max < count) {
			*max = count;
			for (i = 0; i < m; i++) {
				for (j = 0; j < n; j++) {
					for(k = 0; k < m; k++) {
						B[i][j][k]=A[i][j][k];
					}
				}
			}
		}
		if(*min > count) *min = count;
	}
	
}

int main() {
	//cout << "n,m,K = ";
	cin >> n >> m >> K;
  n=2; m=2; K=5;
	
	int i,j,k;
	A = new int**[m];
  B = new int**[m];
	for (i = 0; i < m; i++) {
		A[i] = new int*[n];
    B[i] = new int*[n];
		for (j = 0; j < n; j++) {
			A[i][j] = new int[n];
      B[i][j] = new int[n];
			for(k = 0; k < m; k++) {
				A[i][j][k]=B[i][j][k]=0;
			}
		}
	}

	int max, min;
	podschet( &max, &min );
	cout << "max = " << max << ", min = " << min;
  cout << endl << "max graph:" << endl;
	print(B); // печатаем граф на котором достигается max
	return 0;
	
/* 
  //Следующий код показывает как проводится перебор всех возможных сочетаний из n*n*m по K
  n = 2; m = 2; K = 5;
	perest = new int[K];
	for (int i = 0; i < K; i++){ // инициализируем первой перестановкой
		perest[i] = i;
	}
	
	print_array(perest, K);
	
	while ( next_perest() ) { // печатаем пока перестановки не закончатся
		print_array(perest, K);
	}
*/	
	
}
