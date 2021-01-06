#include <iostream>
#include <string>
#include <fstream>//和文件有关的输入输出类声明
#include<sstream>///C++风格的串流控制
#include <map>//关联容器map的操作
#include <algorithm>//包含vecotr,map等等插入等的操作
#include <vector>
using namespace std;

class Node {//定义Node类
public:
	size_t	weight = 0;//权重
	Node* lchild = nullptr, * rchild = nullptr;//Node类型左孩子指针，右孩子指针
	const string* part = nullptr;//指向字符
	vector<int>pm;//用于存放码
};

void node_sort(Node* a_Node, int size) {//对结点排序，形参为：指向存放结点的数组 和 待排序结点

	Node  temp;//Node类型结点（排序用，工具结点）

	for (int i = 0; i < size; i++) {//双循环让其按权重从小到大排序
		for (int j = 0; j < size - 1 - i; j++) {
			if (a_Node[j].weight > a_Node[j + 1].weight) {
				temp = a_Node[j];
				a_Node[j] = a_Node[j + 1];
				a_Node[j + 1] = temp;
			}
		}
	}
}

Node merge(Node _t1, Node _t2) {//让结点合并起来

	Node		temp, * t1, * t2;//Node类型的父亲结点和俩指针（用于构建父亲指向左右孩子结点）

	t1 = new Node;//t1指向未初始化的Node
	t2 = new Node;//同理
	memcpy(t1, &_t1, sizeof(Node));//memcpy指的是c和c++使用的内存拷贝函数，memcpy函数的功能是从  &_t1  所指的内存地址(源地址)的起始位置开始拷贝sizeof(Node)个字节到目标 t1  所指的内存地址的起始位置中
	memcpy(t2, &_t2, sizeof(Node));//同理

	temp.weight = t1->weight + t2->weight;//将t1指向的结点的权重+t2的，存放到父亲结点
	temp.lchild = t1;//将t1赋给父亲结点的左孩子指针
	temp.rchild = t2;//同理。因为最后会自动销毁，所以不用再delete
	
	return temp;//返回父亲结点
}

void a_merge(Node* a_tNode, Node insert, int size) {//将父亲结点insert 插入到结点数组上，并删去

	a_tNode[0] = insert;//将要插入的新  父亲结点  存放到数组的第一位上

	for (int i = 1; i < size - 1; i++)//数组中从新插入的父亲结点往后  将各个结点向前移一位，覆盖掉node[1]位置的无效结点
		a_tNode[i] = a_tNode[i + 1];

	memset(&a_tNode[size - 1], 0, sizeof(Node));//memset函数为将 数组 &a_tNode  上的size - 1位置开始的 sizeof(Node)个字节数 位 都初始化成0。这里是为了删去最后一个空出来的结点位置
}                                               //a_tNode[size - 1] 是指针类型，（指向node数组的size - 1位）要在前面加&  解开
												//再次注意，这里的是字节长度处理，如int num[8];要处理全部时 memset(num,0,8);取8是不对的，因为int类型是4个字节，8个int应该是32位，应该是memset(num,0,32)

int WPL(Node* root, int deep, int wpl) {//提取出根结点，以及用于深度和wpl的形参

	if (root) {//root要不为0
		if (!root->lchild && !root->rchild) {//根节点的左右孩子结点都为0时，直接计算根节点
			wpl += root->weight * deep;
		}
		else//当根节点有孩子，正常huffman树时。
			wpl += WPL(root->lchild, deep + 1, wpl) + WPL(root->rchild, deep + 1, wpl);//将需要处理的左孩子结点作为根节点，深度加一，现WPL值代入，再次调用WPL函数，以此获取左半边的WPL值，右边同理，将其相加
		return wpl;
	}
	return wpl;
}

void huff(Node* root, vector<int>& v, vector<int>& d, map<string, vector<int>>& nodename, int deep, int side) {//代入目前当作的根结点，用于存放目前编码的数组，存放未处理目前深度，进入左右边标志位（进入左为0，进入右为1）

	if (root) {//root要不为0
		if (!root->lchild && !root->rchild) {//根节点的左右孩子结点都为0时，即为叶节点时
			for (int i = 0; i < v.size(); i++) {//在nodename上将*root->part字符的对应的码值输入
				nodename[*root->part].push_back(v[i]);
			}
			for (size_t i = 0; i < v.size() - 1; i++) {//将码值迁移覆盖第一位的0，使得每一个字符的编码都与另一个字符编码的前一部分不同
				nodename[*root->part][i] = nodename[*root->part][i + 1];
			}
			nodename[*root->part].pop_back();//再删去无效的末尾

			//用于退去码值的位，以便下次加入
			if (side == 0) {//如果进入左叶，只用退去一位码
				v.pop_back();
			}
			else if (side == 1) {//如果进入右叶，即完成了其父亲结点的结点处理（右叶结点必定为处理时最后一个）
				for (int i = (deep - d.back()); i > 0; i--) {//用目前深度减去未处理深度末尾即为要退去的位数
					v.pop_back();
				}
				deep = (deep - d.back());//并让深度也返回到未处理深度
			}
		}
		else {//当根节点有孩子，正常huffman树时。
			d.push_back(deep);//将该深度插入未处理深度数组的末尾，待后面退位处理
			v.push_back(0);//进入左边前在编码最后加0，并将深度加一
			huff(root->lchild, v, d, nodename, deep + 1, 0);
			v.push_back(1);//进入右边前在编码最后加1
			d.pop_back();//若进入的是右边，必定处理了其上一深度，需要将未处理深度的最后一个删去，即该右叶结点的父亲结点的深度
			huff(root->rchild, v, d, nodename, deep + 1, 1);
		}
	}
}

int main() {

	fstream infile;//打开文件供读写

	map<string, int>Maps;//存放字符和对应权重的关联容器Maps
	infile.open("huffman.txt");   //将文件流对象与文件连接起来 

	if (!infile.is_open())//判断有无打开
	{
		cout << "open file error" << endl;
		return 0;
	}//若失败,则输出错误消息,并终止程序运行 

	string strfile, tmp;

	while (getline(infile, tmp))//判断输入流是否有效，infile为输入流，tmp为存放读入的字符串，最后默认'\n'（回车）时结束读取
	{
		strfile.append(tmp);//在字符串后追加字符串tmp
		strfile.append(" ");//一定要加这一行！！！因为getline是直接读取文本文件的一行，遇到回车换行符作为读取一行的结束，如果有如下情况：
									 //abc bcf
									 //a  如果读完一行不加空格那么读取完abc bcf后再读取a,//则bcfa为一个字符串，算作一个了
		tmp.clear();//清除tmp方便后续
	}
	for (int i = 0; i < strfile.length(); i++) {//将读取到的字符串存入Maps中
		tmp = strfile[i];//获取
		if (Maps.find(tmp) == Maps.end())//判断Maps中是否有tmp
		{
			Maps.insert(pair<string, int>(tmp, 1));//若没有才用insert存入，且对应权重初始化为1
		}
		else
		{
			Maps[tmp]++;//否则将其对应int型权重加一
		}
	}
	infile.close();             //关闭文件输入流  


	size_t quantity;//需要处理的结点数
	Node insert;//需要插入的父亲结点
	Node* node=nullptr;//用于指向Node类型动态内存的数组

	quantity = Maps.size();//获取个数
	node = new Node[quantity];//node指向长度为quantity的动态内存数组，存放父亲结点
	memset(node, 0, sizeof(Node) * quantity);//memset函数为将   node  上的 sizeof(Node) * quantity 个字节数 位都 初始化成 0 。这里是为了初始化node数组

	map<string, vector<int>>nodename;//存放字符和对应的码
	for (map<string, int>::iterator it = Maps.begin(); it != Maps.end(); it++) {//利用迭代器遍历插入Maps的string，且将vector<int>容器初始化
		nodename.insert(pair<string, vector<int>>(it->first, 0));
	}

	map<string, int>::iterator it = Maps.begin();
	for (int i = 0; i < quantity; i++) {
		node[i].weight = it->second;//将Maps的权重给node
		node[i].part = &it->first;//将node的字符串指针指向目标字符串
		it++;
	}
	if (quantity == 1)//如果只有一个待处理的结点，将node数组上第一个，即唯一一个作为插入（避免结合时报错）
		insert = node[0];
	for (int i = 0; i < quantity - 1; i++) {//判断减一是因为：完成后父亲结点永远比原先需要处理的数 少一个，这里是为了循环处理完成 待处理的点
		node_sort(node, quantity - i);//对node中待排序的结点进行排序
		insert = merge(node[0], node[1]);//将最小的两个结点结合，并将父亲结点作为插入结点
		a_merge(node, insert, quantity - i);//将父亲结点插入
	}
	//至此huffman 树构建完毕

	cout << endl << "WPL == " << WPL(&insert, 0, 0) << endl;//获取WPL值，（最后的insert即为根结点）（其深度deep和用于存放WPL值的wpl初始化0）

	vector<int>bm;//用于存放目前结点编码
	bm.push_back(0);//先放根节点的0
	vector<int>d;//用于存放未处理深度
	d.push_back(0);//用于初始化未处理深度

	huff(&insert, bm, d, nodename, 0, 0);//调用huff函数开始处理

	for (map<string, int>::iterator it = Maps.begin(); it != Maps.end(); it++) {//利用迭代器遍历输出字符串和对应权重,频率以及码值

		cout << "\n字符为：" << it->first << "\n\t其权重为：" << it->second << "\n\t其频率为：" << ((double)it->second / (double)strfile.length());
		cout << "\n\t其码值为：";
		for (int i = 0; i < nodename[it->first].size(); i++) {//利用遍历从nodename中读取对应字符的码值
			cout << nodename[it->first][i];
		}
		cout << endl;
	}
	delete []node;//销毁动态内存

	//写入文件
	ofstream in;
	in.open("2.txt", ios::trunc); //ios::trunc表示在打开文件前将文件清空,由于是写入,文件不存在则创建

	for (map<string, int>::iterator it = Maps.begin(); it != Maps.end(); it++) {//利用迭代器遍历输出字符串和对应权重,频率以及码值

		in << "\n字符为：" << it->first << "\n\t其权重为：" << it->second << "\n\t其频率为：" << ((double)it->second / (double)strfile.length());
		in << "\n\t其码值为：";
		for (int i = 0; i < nodename[it->first].size(); i++) {//利用遍历从nodename中读取对应字符的码值
			in << nodename[it->first][i];
		}
		in << endl;
	}
	in.close();//关闭文件
	return 0;
}
