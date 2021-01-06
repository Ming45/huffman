#include <iostream>
#include <string>
#include <fstream>//���ļ��йص��������������
#include<sstream>///C++���Ĵ�������
#include <map>//��������map�Ĳ���
#include <algorithm>//����vecotr,map�ȵȲ���ȵĲ���
#include <vector>
using namespace std;

class Node {//����Node��
public:
	size_t	weight = 0;//Ȩ��
	Node* lchild = nullptr, * rchild = nullptr;//Node��������ָ�룬�Һ���ָ��
	const string* part = nullptr;//ָ���ַ�
	vector<int>pm;//���ڴ����
};

void node_sort(Node* a_Node, int size) {//�Խ�������β�Ϊ��ָ���Ž������� �� ��������

	Node  temp;//Node���ͽ�㣨�����ã����߽�㣩

	for (int i = 0; i < size; i++) {//˫ѭ�����䰴Ȩ�ش�С��������
		for (int j = 0; j < size - 1 - i; j++) {
			if (a_Node[j].weight > a_Node[j + 1].weight) {
				temp = a_Node[j];
				a_Node[j] = a_Node[j + 1];
				a_Node[j + 1] = temp;
			}
		}
	}
}

Node merge(Node _t1, Node _t2) {//�ý��ϲ�����

	Node		temp, * t1, * t2;//Node���͵ĸ��׽�����ָ�루���ڹ�������ָ�����Һ��ӽ�㣩

	t1 = new Node;//t1ָ��δ��ʼ����Node
	t2 = new Node;//ͬ��
	memcpy(t1, &_t1, sizeof(Node));//memcpyָ����c��c++ʹ�õ��ڴ濽��������memcpy�����Ĺ����Ǵ�  &_t1  ��ָ���ڴ��ַ(Դ��ַ)����ʼλ�ÿ�ʼ����sizeof(Node)���ֽڵ�Ŀ�� t1  ��ָ���ڴ��ַ����ʼλ����
	memcpy(t2, &_t2, sizeof(Node));//ͬ��

	temp.weight = t1->weight + t2->weight;//��t1ָ��Ľ���Ȩ��+t2�ģ���ŵ����׽��
	temp.lchild = t1;//��t1�������׽�������ָ��
	temp.rchild = t2;//ͬ����Ϊ�����Զ����٣����Բ�����delete
	
	return temp;//���ظ��׽��
}

void a_merge(Node* a_tNode, Node insert, int size) {//�����׽��insert ���뵽��������ϣ���ɾȥ

	a_tNode[0] = insert;//��Ҫ�������  ���׽��  ��ŵ�����ĵ�һλ��

	for (int i = 1; i < size - 1; i++)//�����д��²���ĸ��׽������  �����������ǰ��һλ�����ǵ�node[1]λ�õ���Ч���
		a_tNode[i] = a_tNode[i + 1];

	memset(&a_tNode[size - 1], 0, sizeof(Node));//memset����Ϊ�� ���� &a_tNode  �ϵ�size - 1λ�ÿ�ʼ�� sizeof(Node)���ֽ��� λ ����ʼ����0��������Ϊ��ɾȥ���һ���ճ����Ľ��λ��
}                                               //a_tNode[size - 1] ��ָ�����ͣ���ָ��node�����size - 1λ��Ҫ��ǰ���&  �⿪
												//�ٴ�ע�⣬��������ֽڳ��ȴ�����int num[8];Ҫ����ȫ��ʱ memset(num,0,8);ȡ8�ǲ��Եģ���Ϊint������4���ֽڣ�8��intӦ����32λ��Ӧ����memset(num,0,32)

int WPL(Node* root, int deep, int wpl) {//��ȡ������㣬�Լ�������Ⱥ�wpl���β�

	if (root) {//rootҪ��Ϊ0
		if (!root->lchild && !root->rchild) {//���ڵ�����Һ��ӽ�㶼Ϊ0ʱ��ֱ�Ӽ�����ڵ�
			wpl += root->weight * deep;
		}
		else//�����ڵ��к��ӣ�����huffman��ʱ��
			wpl += WPL(root->lchild, deep + 1, wpl) + WPL(root->rchild, deep + 1, wpl);//����Ҫ��������ӽ����Ϊ���ڵ㣬��ȼ�һ����WPLֵ���룬�ٴε���WPL�������Դ˻�ȡ���ߵ�WPLֵ���ұ�ͬ���������
		return wpl;
	}
	return wpl;
}

void huff(Node* root, vector<int>& v, vector<int>& d, map<string, vector<int>>& nodename, int deep, int side) {//����Ŀǰ�����ĸ���㣬���ڴ��Ŀǰ��������飬���δ����Ŀǰ��ȣ��������ұ߱�־λ��������Ϊ0��������Ϊ1��

	if (root) {//rootҪ��Ϊ0
		if (!root->lchild && !root->rchild) {//���ڵ�����Һ��ӽ�㶼Ϊ0ʱ����ΪҶ�ڵ�ʱ
			for (int i = 0; i < v.size(); i++) {//��nodename�Ͻ�*root->part�ַ��Ķ�Ӧ����ֵ����
				nodename[*root->part].push_back(v[i]);
			}
			for (size_t i = 0; i < v.size() - 1; i++) {//����ֵǨ�Ƹ��ǵ�һλ��0��ʹ��ÿһ���ַ��ı��붼����һ���ַ������ǰһ���ֲ�ͬ
				nodename[*root->part][i] = nodename[*root->part][i + 1];
			}
			nodename[*root->part].pop_back();//��ɾȥ��Ч��ĩβ

			//������ȥ��ֵ��λ���Ա��´μ���
			if (side == 0) {//���������Ҷ��ֻ����ȥһλ��
				v.pop_back();
			}
			else if (side == 1) {//���������Ҷ����������丸�׽��Ľ�㴦����Ҷ���ض�Ϊ����ʱ���һ����
				for (int i = (deep - d.back()); i > 0; i--) {//��Ŀǰ��ȼ�ȥδ�������ĩβ��ΪҪ��ȥ��λ��
					v.pop_back();
				}
				deep = (deep - d.back());//�������Ҳ���ص�δ�������
			}
		}
		else {//�����ڵ��к��ӣ�����huffman��ʱ��
			d.push_back(deep);//������Ȳ���δ������������ĩβ����������λ����
			v.push_back(0);//�������ǰ�ڱ�������0��������ȼ�һ
			huff(root->lchild, v, d, nodename, deep + 1, 0);
			v.push_back(1);//�����ұ�ǰ�ڱ�������1
			d.pop_back();//����������ұߣ��ض�����������һ��ȣ���Ҫ��δ������ȵ����һ��ɾȥ��������Ҷ���ĸ��׽������
			huff(root->rchild, v, d, nodename, deep + 1, 1);
		}
	}
}

int main() {

	fstream infile;//���ļ�����д

	map<string, int>Maps;//����ַ��Ͷ�ӦȨ�صĹ�������Maps
	infile.open("huffman.txt");   //���ļ����������ļ��������� 

	if (!infile.is_open())//�ж����޴�
	{
		cout << "open file error" << endl;
		return 0;
	}//��ʧ��,�����������Ϣ,����ֹ�������� 

	string strfile, tmp;

	while (getline(infile, tmp))//�ж��������Ƿ���Ч��infileΪ��������tmpΪ��Ŷ�����ַ��������Ĭ��'\n'���س���ʱ������ȡ
	{
		strfile.append(tmp);//���ַ�����׷���ַ���tmp
		strfile.append(" ");//һ��Ҫ����һ�У�������Ϊgetline��ֱ�Ӷ�ȡ�ı��ļ���һ�У������س����з���Ϊ��ȡһ�еĽ�������������������
									 //abc bcf
									 //a  �������һ�в��ӿո���ô��ȡ��abc bcf���ٶ�ȡa,//��bcfaΪһ���ַ���������һ����
		tmp.clear();//���tmp�������
	}
	for (int i = 0; i < strfile.length(); i++) {//����ȡ�����ַ�������Maps��
		tmp = strfile[i];//��ȡ
		if (Maps.find(tmp) == Maps.end())//�ж�Maps���Ƿ���tmp
		{
			Maps.insert(pair<string, int>(tmp, 1));//��û�в���insert���룬�Ҷ�ӦȨ�س�ʼ��Ϊ1
		}
		else
		{
			Maps[tmp]++;//�������Ӧint��Ȩ�ؼ�һ
		}
	}
	infile.close();             //�ر��ļ�������  


	size_t quantity;//��Ҫ����Ľ����
	Node insert;//��Ҫ����ĸ��׽��
	Node* node=nullptr;//����ָ��Node���Ͷ�̬�ڴ������

	quantity = Maps.size();//��ȡ����
	node = new Node[quantity];//nodeָ�򳤶�Ϊquantity�Ķ�̬�ڴ����飬��Ÿ��׽��
	memset(node, 0, sizeof(Node) * quantity);//memset����Ϊ��   node  �ϵ� sizeof(Node) * quantity ���ֽ��� λ�� ��ʼ���� 0 ��������Ϊ�˳�ʼ��node����

	map<string, vector<int>>nodename;//����ַ��Ͷ�Ӧ����
	for (map<string, int>::iterator it = Maps.begin(); it != Maps.end(); it++) {//���õ�������������Maps��string���ҽ�vector<int>������ʼ��
		nodename.insert(pair<string, vector<int>>(it->first, 0));
	}

	map<string, int>::iterator it = Maps.begin();
	for (int i = 0; i < quantity; i++) {
		node[i].weight = it->second;//��Maps��Ȩ�ظ�node
		node[i].part = &it->first;//��node���ַ���ָ��ָ��Ŀ���ַ���
		it++;
	}
	if (quantity == 1)//���ֻ��һ��������Ľ�㣬��node�����ϵ�һ������Ψһһ����Ϊ���루������ʱ����
		insert = node[0];
	for (int i = 0; i < quantity - 1; i++) {//�жϼ�һ����Ϊ����ɺ��׽����Զ��ԭ����Ҫ������� ��һ����������Ϊ��ѭ��������� ������ĵ�
		node_sort(node, quantity - i);//��node�д�����Ľ���������
		insert = merge(node[0], node[1]);//����С����������ϣ��������׽����Ϊ������
		a_merge(node, insert, quantity - i);//�����׽�����
	}
	//����huffman ���������

	cout << endl << "WPL == " << WPL(&insert, 0, 0) << endl;//��ȡWPLֵ��������insert��Ϊ����㣩�������deep�����ڴ��WPLֵ��wpl��ʼ��0��

	vector<int>bm;//���ڴ��Ŀǰ������
	bm.push_back(0);//�ȷŸ��ڵ��0
	vector<int>d;//���ڴ��δ�������
	d.push_back(0);//���ڳ�ʼ��δ�������

	huff(&insert, bm, d, nodename, 0, 0);//����huff������ʼ����

	for (map<string, int>::iterator it = Maps.begin(); it != Maps.end(); it++) {//���õ�������������ַ����Ͷ�ӦȨ��,Ƶ���Լ���ֵ

		cout << "\n�ַ�Ϊ��" << it->first << "\n\t��Ȩ��Ϊ��" << it->second << "\n\t��Ƶ��Ϊ��" << ((double)it->second / (double)strfile.length());
		cout << "\n\t����ֵΪ��";
		for (int i = 0; i < nodename[it->first].size(); i++) {//���ñ�����nodename�ж�ȡ��Ӧ�ַ�����ֵ
			cout << nodename[it->first][i];
		}
		cout << endl;
	}
	delete []node;//���ٶ�̬�ڴ�

	//д���ļ�
	ofstream in;
	in.open("2.txt", ios::trunc); //ios::trunc��ʾ�ڴ��ļ�ǰ���ļ����,������д��,�ļ��������򴴽�

	for (map<string, int>::iterator it = Maps.begin(); it != Maps.end(); it++) {//���õ�������������ַ����Ͷ�ӦȨ��,Ƶ���Լ���ֵ

		in << "\n�ַ�Ϊ��" << it->first << "\n\t��Ȩ��Ϊ��" << it->second << "\n\t��Ƶ��Ϊ��" << ((double)it->second / (double)strfile.length());
		in << "\n\t����ֵΪ��";
		for (int i = 0; i < nodename[it->first].size(); i++) {//���ñ�����nodename�ж�ȡ��Ӧ�ַ�����ֵ
			in << nodename[it->first][i];
		}
		in << endl;
	}
	in.close();//�ر��ļ�
	return 0;
}
