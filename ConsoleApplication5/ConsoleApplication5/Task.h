#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<experimental/filesystem>
#include "Menu.h"
#include "Functions.h"
namespace fs = std::experimental::filesystem;
using namespace std;
class List
{
public:
	virtual ~List() { }
	virtual void view() = 0;
};
class Task : List
{
	string name;
	string date;
	string priority;
	vector<string> tags;

public:
	Task() {};
	void Create();
	virtual void view() override;
	virtual ~Task() {}
	Task(const Task& _Other);
	static Task* getInstance();
	void read(const string& path);
	string getName() { return name; };
	string getDate() { return date; };
	vector<string> getTags() { return tags; };
	string getPriority() { return priority; };
	void setName(const string& _Other) { name = _Other; }
	void setDate(const string& _Other) { date = _Other; }
	void setPriority(const string& _Other) { priority = _Other; }
	void setTags(const vector<string>& _Other) { tags = _Other; }
	friend ostream& operator<<(ostream& out, const Task& q);
};
Task::Task(const Task& _Other)
{
	date = _Other.date;
	name = _Other.name;
	priority = _Other.priority;
	tags = _Other.tags;
}
ostream& operator<<(ostream& out, const Task& q)
{
	out << q.name << endl;
	out << q.date << endl;
	out << q.priority << endl;
	for (size_t i = 0; i < q.tags.size(); i++)
	{
		out << q.tags[i] << endl;
	}
	return out;
}
void Task::read(const string& path)
{
	ifstream in;
	in.open(path);
	getline(in, name);
	getline(in, date);
	getline(in, priority);
	string tag;
	while (getline(in, tag))
	{
		tags.push_back(tag);
	}

}
void Task::view()
{
	cout <<" Задача: " << name + "\n"
		<< " Срок сдачи: " << date + "\n"
		<< " Приоритет: " << priority + "\n"
		<< " Теги: ";
	for (size_t i = 0; i < tags.size(); i++)
	{
		cout << tags[i] << "; ";
	}
	cout << endl;
	cout << endl;
}
void Task::Create()
{
}
class TaskList : List
{
	string path;
	string name;
	vector<Task> list;
public:
	void read();
	virtual void view() override;
	void search(string value, string type);
	string getPath() { return path; };
	string getName() { return name; };
	TaskList() { name = ""; path = ""; }
	TaskList(string p, string n) : path(p), name(n) {}
	void setPath(const string& _Other) { path = _Other; }
};
void TaskList::view()
{
	for (int i = 0; i < list.size(); i++)
	{
		this->list[i].view();
	}

}
void TaskList::read()
{
	ifstream in;
	vector<string> List;
	for (const auto& a : fs::directory_iterator(path))
	{
		List.push_back(a.path().u8string());
	}
	in.open(path);
	for (size_t i = 0; i < List.size(); i++)
	{
		Task t;
		t.read(List[i]);
		list.push_back(t);
	}
	in.close();
}
void TaskList::search(string item, string type)
{
	int foundLists = 0;
	for (size_t i = 0; i < list.size(); i++)
	{
		if (type == "tag")
		{
			vector<string> tag = list[i].getTags();
			for (size_t a = 0; a < tag.size(); a++)
			{
				if (item == tag[a])
				{
					list[i].view();
					foundLists++;
					break;
				}
			}
		}
		else if (type == "priority")
		{
			if (item == list[i].getPriority())
			{
				list[i].view();
				foundLists++;
			}
		}
		else if (type == "name")
		{
			if (item == list[i].getName())
			{
				list[i].view();
				foundLists++;
			}
		}
		else if (type == "date")
		{
			if (item == list[i].getDate())
			{
				list[i].view();
				foundLists++;
			}
		}
	}
	if (foundLists == 0)
		cout << "Не найдено" << endl;
	system("pause");
}