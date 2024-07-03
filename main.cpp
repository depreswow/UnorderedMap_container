#include "UnorderedMap.h"
#include <string>
#include "Algorithms.h"
#include "insert_iterator.h"

struct Goods
{
	std::string m_name;              // Название
	std::string m_manufacturer;      // Производитель
	std::string m_warehouse_address; // Адрес склада
	double m_weight;				 // Вес
};

struct Goods_Functor {
	double value;
	
	Goods_Functor(double in_value) 
		: value(in_value) {}

	bool operator()(const Goods& good) const {
		return good.m_weight > value;
	}
};

int main() {
	typedef UnorderedMap<int, Goods>::iterator iterator;

	std::initializer_list<std::pair<int, Goods>> l = { {1, {"Smartphone", "Vivo", "Ivanovskaya Square", 10.5}}, {2, {"Labtop", "Asus", "2nd Baumanskaya st. 5", 100.2 }} };

	UnorderedMap<int, Goods> cont_1(l);

	std::cout << "Values:" << std::endl;
	forEach(cont_1.begin(), cont_1.end(), [](Goods good) {std::cout << " " << good.m_name << " " << good.m_manufacturer << " " << good.m_warehouse_address << " " << good.m_weight << std::endl; });

	UnorderedMap<int, Goods> cont_2(2);
	Goods good = { "Keyboard", "Apple", "Pushkina st. Kolotushkina b.", 30.8 };
	cont_2.insert(std::pair<int, Goods>{3, good});

	good = {"Mic", "Blue", "U menya zakonchilis' idei", 15.6};
	cont_2.insert(std::pair<int, Goods>{4, good});

	for (iterator iter = cont_2.begin(); iter != cont_2.end(); ++iter)
		cont_1.insert(iter->value);
	std::cout << "Values:" << std::endl;
	forEach(cont_1.begin(), cont_1.end(), [](Goods good) {std::cout << " " << good.m_name << " " << good.m_manufacturer << " " << good.m_warehouse_address << " " << good.m_weight << std::endl; });

	iterator iter = findIf(cont_1.begin(), cont_2.end(), [](Goods good) {return (good.m_weight > 20 && good.m_weight < 50); });
	iter->value.second.m_warehouse_address = "Cosmic void";
	std::cout << "Values:" << std::endl;
	forEach(cont_1.begin(), cont_1.end(), [](Goods good) {std::cout << " " << good.m_name << " " << good.m_manufacturer << " " << good.m_warehouse_address << " " << good.m_weight << std::endl; });

	UnorderedMap<int, Goods> cont_3(4);
	insert_iterator<UnorderedMap<int, Goods>> insert_iter(&cont_3);
	Goods_Functor greater_than15(15.0);
	std::cout << "Copying every value > 15 into cont_3:" << std::endl;
	copyIf(cont_1.begin(), cont_2.end(), insert_iter, greater_than15);
	std::cout << "cont_3 Values:" << std::endl;
	forEach(cont_3.begin(), cont_3.end(), [](Goods good) {std::cout << " " << good.m_name << " " << good.m_manufacturer << " " << good.m_warehouse_address << " " << good.m_weight << std::endl; });
}