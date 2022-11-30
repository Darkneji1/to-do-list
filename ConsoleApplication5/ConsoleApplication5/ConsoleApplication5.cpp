#include <iostream>
#include <Windows.h>
#include "Application.h"

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Application* app = Application::getInstance();
	app->mainMenu();
}
