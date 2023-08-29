#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
using namespace std;
const int SHUDU = 9;
const int XSHUDU = 3;
vector<int> zero_x, zero_y;

class idsort {
public:
    int id;
	int num;
    bool operator<(const idsort& a) const {
		return num < a.num;
	}
};

idsort* emp;
bool valid(int a[]) {
    for (int i = 0; i < SHUDU - 1; i++) {

        for (int j = i + 1; j < SHUDU; j++) {
            if (a[i] == a[j] && a[i] != 0) {
  
                return false;
            }
        }
    }
    return true;
}

bool isValidSudoku(vector<vector<int> >& board) {
    if (board.size() != SHUDU || board[0].size() != SHUDU) {
		return false;
	}
    int t[SHUDU];
    for (int i = 0; i < SHUDU; i++) {
        
        for (int j = 0; j < SHUDU; j++) {
            t[j] = board[i][j];
        }
        if (!valid(t)) {

            return false;
        }
        for (int j = 0; j < SHUDU; j++) {
            t[j] = board[j][i];
        }
        if (!valid(t)) {
        
            return false;
        }
    }
    for (int i = 0; i < XSHUDU; i++) {
        for (int j = 0; j < XSHUDU; j++) {
            int pos=0;
            for (int k = XSHUDU * i; k < XSHUDU * i + XSHUDU; k++) {
                for (int l = XSHUDU * j; l < XSHUDU * j + XSHUDU; l++) {
                    t[pos++] = board[k][l];
                }
            }
            if (!valid(t)) {
        
                return false;
            }
        }
    }
    return true;
}
bool isvalidone(vector<vector<int> >& board,int &x,int &y) {
    int t[SHUDU];
    for (int i = 0; i < SHUDU; i++) {
        t[i] = board[x][i];
    }
    if (!valid(t))
        return false;
    for (int i = 0; i < SHUDU; i++) {
        t[i] = board[i][y];
    }
    if (!valid(t))
        return false;
    int pos = 0;
    for (int i = x/XSHUDU*XSHUDU; i <x/XSHUDU*XSHUDU+ XSHUDU; i++) {
        for (int j = y/3*XSHUDU; j < y/XSHUDU*XSHUDU+XSHUDU; j++) {
            t[pos++] = board[i][j];
            
        }
    }  
    if (!valid(t))
        return false;

    return true;

}
vector<vector<int> > find_solves(vector<vector<int> >& t) {
    vector<vector<int> > ans;
    for (int i = 0; i < zero_x.size(); i++) {
        vector<int> temp;
        for (int j = 1; j <= SHUDU; j++) {
            t[zero_x[i]][zero_y[i]] = j;
            if (isvalidone(t,zero_x[i],zero_y[i])) {
                temp.push_back(j);
            }
        }
        t[zero_x[i]][zero_y[i]] = 0;
        ans.push_back(temp);
    }
    return ans;
}

void findzeros(vector<vector<int> > t) {
    zero_x.clear();
    zero_y.clear();
    for (int i = 0; i < t.size(); i++) {
        for (int j = 0; j < t[0].size(); j++) {
            if (t[i][j] == 0) {
                zero_x.push_back(i);
                zero_y.push_back(j);
            }
        }
    }//�ҳ����еĿհ׸��ӵ�����
}

bool solveone(vector<vector<int> >& t) {
    
    bool flag;
    vector<vector<int> > possibles;
    do {
        flag = false;
        findzeros(t);
        possibles = find_solves(t);
        for (int i = 0; i < possibles.size(); i++) {
            if (possibles[i].size() == 1) {
                t[zero_x[i]][zero_y[i]] = possibles[i][0];
                flag = true;
            }
        }//����һ�׽�
    } while (flag);
    if (!possibles.size()) {
        return true;
    }
    else return false;
}

bool solvens(vector<vector<int> >& t,int depth,vector<vector<int> > &possibles) {//depthΪ��ǰ����� ��0��ʼ
    int pos = emp[depth].id;
    for (int i = 0; i < possibles[pos].size(); i++) {//����ÿһ�����ܵĽ�
		t[zero_x[pos]][zero_y[pos]] = possibles[pos][i];//�����i�п���
        if (isvalidone(t,zero_x[pos],zero_y[pos])) {
            //����Ϸ�
            if (depth == possibles.size()-1) {
                return true;
            }
            if (solvens(t, depth + 1, possibles)) {
                return true;
            }//�д�������һ��,���˾ͷ���true�˳�ѭ��
        }
	}
    t[zero_x[pos]][zero_y[pos]] = 0;
	return false;
}
bool solve(vector<vector<int> >& t) {
    if (solveone(t)) {
        return true;
    }
    
    findzeros(t);//�ҳ����еĿհ׸��ӵ�����
    vector<vector<int> > possibles = find_solves(t);
    int n = possibles.size();
    emp = new idsort[n];
    for (int i = 0; i < n; i++) {
        emp[i].id = i;
        emp[i].num = possibles[i].size();
    }
    sort(emp, emp + n);
    return solvens(t, 0, possibles);
    
}
int main() {
    ifstream file("question.txt");
    vector<vector<int> > board;
    for (int i = 0; i < SHUDU; i++) {
		vector<int> t;
        for (int j = 0; j < SHUDU; j++) {
			int c;
			file >> c;
			t.push_back(c);
		}
		board.push_back(t);//��Ϊ�հף�1-9Ϊ������
	}
    file.close();
    ofstream file1("answer.txt");
    if (!isValidSudoku(board)) {
		file1 << "No valid answer";
        file1.close();
        return 0;
	}
    cout<<solve(board);
    for (int i = 0; i < SHUDU; i++) {
            for (int j = 0; j < SHUDU; j++) {
				file1 << board[i][j] << " ";
			}
			file1 << endl;
	}
    
    file1.close();
	return 0;
}
