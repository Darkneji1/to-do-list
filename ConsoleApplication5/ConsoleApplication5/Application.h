#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<experimental/filesystem>
#include "Menu.h"
#include "Functions.h"
#include "Task.h"
namespace fs = std::experimental::filesystem;
using namespace std;
string Choose(const string path)
{
	vector<string> items;

	for (const auto& a : fs::directory_iterator(path))
		items.push_back(a.path().u8string());

	if (items.size() == 0)
	{
		cout << "Не найдено" << endl;
		system("pause");
		return "Exit";
	}

	for_each(items.begin(), items.end(), [](string& s)
		{
			s.erase(s.begin(), s.begin() + s.find_last_of("\\") + 1);
		});
	items.push_back("Exit");

	int a = Menu::select_vertical(items, HorizontalAlignment::Center);
	return  items[a];
}
class Application
{
	static Application* instance;
	Application() {}
public:
	virtual ~Application() {}
	static Application* getInstance();
	void mainMenu();
	void workWithList();
	void workWithTask(string p,string n);
	void menuTask(string p);

};
Application* Application::getInstance()
{
	if (instance == nullptr)
		instance = new Application();
	return instance;
}
Application* Application::instance = nullptr;

void Application::menuTask(string path)
{
	while (true)
	{
		system("cls");
		cout << path;
		int c = Menu::select_vertical({ "Редактировать список","Выбрать задачу","Добавить задачу","Назад" }, HorizontalAlignment::Center);
		switch (c)
		{
		case 0:
		{
			while (true)
			{
				system("cls");
				cout << path;
				int c = Menu::select_vertical({ "Очистить список","Удалить список","Переименовать список","Назад" }, HorizontalAlignment::Center);
				switch (c)
				{
				case 0:
				{
					if (!fs::is_empty(path))
					{
						vector<string> tasks;
						for (const auto& a : fs::directory_iterator(path))
						{
							tasks.push_back(a.path().u8string());
						}
						for (size_t i = 0; i < tasks.size(); i++)
						{
							fs::remove(tasks[i]);
						}
						cout << "Список очищен" << endl;

					}
					else
						cout << "Список и так пуст" << endl;

					system("pause");
					break;
				}
				case 1:
				{
					fs::remove_all(path);
					cout << "Список удален" << endl;
					system("pause");
					break;
				}
				case 2:
				{
					bool isCorrect = true;
					vector<string> tasks;
					for (const auto& a : fs::directory_iterator("List\\"))
					{
						tasks.push_back(a.path().u8string());
					}
					vector<string> show = tasks;
					for_each(show.begin(), show.end(), [](string& s)
						{
							s.erase(s.begin(), s.begin() + s.find_last_of("\\") + 1);
						});
					string n;
					while (true)
					{
						system("cls");
						cout << "Введите новое название списка: ";
						getline(cin, n);
						if (n.empty())
						{
							cout << "Пустая строка" << endl;
							system("pause");
							continue;
						}
						break;
					}
					if (n == "/exit")
						continue;
					for (size_t i = 0; i < show.size(); i++)
					{
						if (n == show[i])
						{
							isCorrect = false;
						}
					}
					if (isCorrect == true)
					{
						fs::rename(path, "List\\" + n + "\\");
						path = "List\\" + n + "\\";
						cout << "Список переименован" << endl;
					}
					else
					{
						cout << "Список с таким названием уже существует" << endl;
					}
					system("pause");
					continue;
				}
				default:
					break;
				}
				break;
			}
			continue;
		}
		case 1:
		{
			if (fs::is_empty(path))
			{
				cout << "В списке нет задач" << endl;
				system("pause");
				continue;
			}
			string nameTXT = Choose(path);
			if (nameTXT == "Exit")
			{
				continue;
			}
			workWithTask(path, nameTXT);
			continue;
		}
		case 2:
		{
			string name;
			string date;
			string priority;
			vector<string>tags;
			system("cls");
			while (true)
			{
				system("cls");
				cout << "Название задачи: ";
				getline(cin, name);
				if (name == "/exit")
					break;
				if (name.size() == 0)
				{
					cout << "Пустая строка" << endl;
					system("pause");
					continue;
				}
				cout << "Дата сдачи: ";
				getline(cin, date);
				if (date.size() == 0)
				{
					cout << "Пустая строка" << endl;
					system("pause");
					continue;
				}
				while (true)
				{
					string tag;
					cout << "Введите тег: ";
					getline(cin, tag);
					if (tag.size() == 0)
					{
						cout << "Пустая строка" << endl;
						system("pause");
						continue;
					}
					tags.push_back(tag);
					cout << "Тег добавлен" << endl;
					while (true)
					{
						int b = Menu::select_vertical({ "Добавить тег","Выход" }, HorizontalAlignment::Center);
						switch (b)
						{
						case 0:
						{
							system("cls");
							cout << "Введите тег: ";
							getline(cin, tag);
							if (tag.size() == 0)
							{
								cout << "Пустая строка" << endl;
								system("pause");
								continue;
							}
							tags.push_back(tag);
							cout << "Тег добавлен" << endl;
							continue;
						}
						default:
							break;
						}
						break;
					}
					while (true)
					{
						cout << "Выберите приоритет задач" << endl;
						int c = Menu::select_vertical({ "Low","Medium","High","Ultra high" }, HorizontalAlignment::Center);
						switch (c)
						{
						case 0:
						{
							priority = "Low";
							break;
						}
						case 1:
						{
							priority = "Medium";
							break;
						}
						case 2:
						{
							priority = "High";
							break;
						}
						case 3:
						{
							priority = "Ultra high";
							break;
						}
						default:
							break;
						}
						break;
					}
					ofstream out;
					out.open(path + name + ".txt");
					out << name << endl;
					out << date << endl;
					out << priority << endl;
					for (size_t i = 0; i < tags.size(); i++)
					{
						out << tags[i] << endl;
					}
					cout << "Задача добавлена" << endl;
					system("pause");
					break;
				}
				break;
			}
			continue;
		}
		default:
			break;
		}
		break;
	}
}
void Application::workWithTask(string p,string n)
{
	string fullPath = p + n;
	vector<string> tags;
	while (true)
	{
		system("cls");
		cout << fullPath << endl;
		Task t;
		t.read(fullPath);
		int d = Menu::select_vertical({ "Удалить задачу","Редактировать название","Редактировать дату выполнения","Редактировать приоритет","Редактировать теги","Назад" }, HorizontalAlignment::Center);
		switch (d)
		{
		case 0:
		{
			fs::remove(fullPath);
			cout << "Задача удалена" << endl;
			system("pause");
			break;
		}
		case 1:
		{
			string newName;
			while (true)
			{
				system("cls");
				cout << "Текущее название: " << t.getName() << endl;
				cout << "Введите новое название: ";
				getline(cin, newName);
				if (newName.empty())
				{
					cout << "Пустая строка" << endl;
					system("pause");
					continue;
				}
				else if (t.getName() == newName)
				{
					cout << "Вы ввели одинаковые названия" << endl;
					system("pause");
					continue;
				}
				break;
			}
			if (newName == "/exit")
				continue;
			fs::rename(p + t.getName() + ".txt", p + newName + ".txt");
			fullPath = p + newName + ".txt";
			t.setName(newName);
			ofstream out;
			out.open(fullPath);
			out << t;
			out.close();
			cout << "Задача переименована" << endl;
			system("pause");
			continue;
		}
		case 2:
		{
			string newDate;
			while (true)
			{
				system("cls");
				cout << "Текущая дата выполнения: " << t.getDate() << endl;
				cout << "Введите новую дату выполнения: ";
				getline(cin, newDate);
				if (newDate.empty())
				{
					cout << "Пустая строка" << endl;
					system("pause");
					continue;
				}
				else if (t.getDate() == newDate)
				{
					cout << "Вы ввели текущую дату " << endl;
					system("pause");
					continue;
				}
				break;
			}
			if (newDate == "/exit")
				continue;
			t.setDate(newDate);
			ofstream out;
			out.open(fullPath);
			out << t;
			out.close();
			cout << "Дата изменена" << endl;
			system("pause");
			continue;
		}
		case 3:
		{
			string newPriority;
			cout << "Текущый приоритет: " << t.getPriority() << endl;
			cout << "Выберите новый приоритет: ";
			int c = Menu::select_vertical({ "Low","Medium","High","Ultra high" }, HorizontalAlignment::Center);
			switch (c)
			{
			case 0:
			{
				newPriority = "Low";
				break;
			}
			case 1:
			{
				newPriority = "Medium";
				break;
			}
			case 2:
			{
				newPriority = "High";
				break;
			}
			case 3:
			{
				newPriority = "Ultra high";
				break;
			}
			default:
				break;
			}
			t.setPriority(newPriority);
			ofstream out;
			out.open(fullPath);
			out << t;
			out.close();
			cout << "Приоритет изменён" << endl;
			system("pause");
			continue;
		}
		case 4:
		{
			while (true)
			{
				system("cls");
				int b = Menu::select_vertical({ "Добавить", "Изменить", "Удалить", "Выход" }, HorizontalAlignment::Center);
				switch (b)
				{
				case 0:
				{
					tags = t.getTags();
					string newTag;
					while (true)
					{
						system("cls");
						cout << "Текущые теги: " << endl;
						for (size_t i = 0; i < tags.size(); i++)
						{
							cout << i + 1 << ". " << tags[i] << endl;
						}
						cout << endl;

						cout << "Введите новый тег: ";
						cin.seekg(0, ios_base::beg);
						getline(cin, newTag);
						if (newTag.empty())
						{
							cout << "Пустая строка" << endl;
							system("pause");
							continue;
						}
						break;
					}
					if (newTag == "/exit")
						continue;
					tags.push_back(newTag);
					t.setTags(tags);
					ofstream out;
					out.open(fullPath);
					out << t;
					out.close();
					cout << "Тег добавлен" << endl;
					system("pause");
					continue;
				}
				case 1:
				{
					int num;
					string newTag;
					while (true)
					{
						system("cls");
						tags = t.getTags();
						cout << "Текущые теги: " << endl;
						for (size_t i = 0; i < tags.size(); i++)
						{
							cout << i + 1 << ". " << tags[i] << endl;
						}
						cout << endl;
						while (true)
						{
							cout << "Введите номер тега который хотите изменить: ";
							cin >> num;
							if (num < 0 || num > tags.size())
							{
								cout << "Такого номера не существует " << endl;
								continue;
							}
							break;
						}
						cout << "Введите новый тег: ";
						cin.seekg(0, ios_base::beg);
						getline(cin, newTag);
						if (newTag.empty())
						{
							cout << "Пустая строка" << endl;
							system("pause");
							continue;
						}
						break;
					}
					if (newTag == "/exit")
						continue;
					tags[num - 1] = newTag;
					t.setTags(tags);
					ofstream out;
					out.open(fullPath);
					out << t;
					out.close();
					cout << "Тег изменён" << endl;
					system("pause");
					continue;
				}
				case 2:
				{
					int num;
					vector<string> newtags;
					string dt;
					system("cls");
					tags = t.getTags();
					while (true)
					{
						system("cls");
						cout << "Текущые теги: " << endl;
						for (size_t i = 0; i < tags.size(); i++)
						{
							cout << i + 1 << ". " << tags[i] << endl;
						}
						cout << endl;
						cout << "Введите номер тега который хотите изменить: ";
						cin >> num;
						if (num < 0 || num > tags.size())
						{
							cout << "Такого номера не существует " << endl;
							continue;
						}
						break;
					}
					dt = tags[num - 1];

					for (size_t i = 0; i < tags.size(); i++)
					{
						if (tags[i] != dt)
							newtags.push_back(tags[i]);
					}
					t.setTags(newtags);
					ofstream out;
					out.open(fullPath);
					out << t;
					out.close();
					cout << "Тег удалён" << endl;
					system("pause");
					continue;
					
				}
				default:
					break;
				}
				break;
			}
			continue;
		}
		default:
			break;
		}
		break;
	}
}
void Application::workWithList()
{
	while (true)
	{
		system("cls");
		int a = Menu::select_vertical({ "Выбрать список","Добавить список","Выход" }, HorizontalAlignment::Center);
		switch (a)
		{
		case 0:
		{
			string path = "List\\";
			string name = Choose("List\\");
			path += name + "\\";
			if (name == "Exit")
			{
				continue;
			}
			TaskList t(path, name);
			while (true)
			{
				system("cls");
				t.read();
				t.view();
				system("pause");
				menuTask(path);
				break;
			}
			continue;
		}
		case 1:
		{
			TaskList test;
			system("cls");
			string path = "List\\";
			string name;
			vector<string>lists;
			for (const auto& a : fs::directory_iterator(path))
			{
				lists.push_back(a.path().u8string());
			}
			for_each(lists.begin(), lists.end(), [](string& s)
				{
					s.erase(s.begin(), s.begin() + s.find_last_of("\\") + 1);
				});
			while (true)
			{
				system("cls");
				if (lists.size() != 0)
				{
					cout << "Существующие списки: " << endl;
				}
				for_each(lists.begin(), lists.end(), [](string& s)
					{
						cout << s << endl;
					});
				cout << "Введите название списка: ";
				getline(cin, name);
				if (name.empty())
				{
					cout << "Пустая строка" << endl;
					system("pause");
					continue;
				}
				if (name == "/exit")
					break;
				if (fs::exists(path + name + "\\"))
				{
					cout << "Список с таким названием уже существует. Повторите попытку" << endl;
					system("pause");
					continue;
				}
				path += name + "\\";
				TaskList t(path, name);
				fs::create_directory(path);
				cout << "Список создан" << endl;
				system("pause");
				menuTask(path);
				break;
			}
			continue;
		}
		default:
			break;
		}
		break;
	}
}
void Application::mainMenu()
{
	if (!fs::exists("List\\"))
	{
		fs::create_directories("List\\");
	}
	while (true)
	{
		system("cls");
		int a = Menu::select_vertical({"Работа со списком","Поиск","Выход" }, HorizontalAlignment::Center);
		switch (a)
		{
		case 0:
		{
			workWithList();
			break;
		}
		case 1:
		{
			string name = Choose("List\\");
			if (name == "Exit")
			{
				continue;
			}
			string path = "List\\" + name + "\\";
			TaskList t(path, name);
			t.read();
			while (true)
			{
				string value;
				system("cls");
				int a = Menu::select_vertical({ "Поиск по тегам","Поиск по приоритету","Поиск по дате","Поиск по названию","Выход" }, HorizontalAlignment::Center);
				switch (a)
				{
				case 0:
				{
					while (true)
					{
						system("cls");
						cout << "Введите тег: ";
						cin.seekg(0, ios_base::beg);
						getline(cin, value);
						if (value.empty())
						{
							cout << "Пустая строка" << endl;
							system("pause");
							continue;
						}
						break;
					}
					if (value == "/exit")
					{
						break;
					}
					
					t.search(value, "tag");
					continue;
				}
				case 1:
				{
					int b = Menu::select_vertical({ "Low","Medium","High","Ultra high","Exit"}, HorizontalAlignment::Center);
					switch (b)
					{
					case 0:
					{
						value = "Low";
						t.search(value, "priority");
						break;
					}
					case 1:
					{
						value = "Medium";
						t.search(value, "priority");
						break;
					}
					case 2:
					{
						value = "High";
						t.search(value, "priority");
						break;
					}
					case 3:
					{
						value = "Ultra high";
						t.search(value, "priority");
						break;
					}
					default:
						break;
					}
					continue;
				}
				case 2:
				{
					while (true)
					{
						system("cls");
						cout << "Введите дату: ";
						cin.seekg(0, ios_base::beg);
						getline(cin, value);
						if (value.empty())
						{
							cout << "Пустая строка" << endl;
							system("pause");
							continue;
						}
						break;
					}
					if (value == "/exit")
					{
						break;
					}
					t.search(value, "date");
					continue;
				}
				case 3:
				{
					while (true)
					{
						system("cls");
						cout << "Введите название: ";
						cin.seekg(0, ios_base::beg);
						getline(cin, value);
						if (value.empty())
						{
							cout << "Пустая строка" << endl;
							system("pause");
							continue;
						}
						break;
					}
					if (value == "/exit")
					{
						break;
					}
					t.search(value, "name");
					continue;
				}
				default:
					break;
				}
				break;
			}
			break;
		}
		default:
			exit(0);
			break;
		}
	}
}