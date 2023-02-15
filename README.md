# Тестовое задание 2. Клиент-серверное приложение.
В качестве библиотеки для клиента использовался SFML 2.5.1.

# Инструкция по запуску.
Клонирование репозитория.
```sh
git clone https://github.com/Akimpaus/task2
```
Установка sfml.
```sh
sudo apt-get update
sudo apt-get install libsfml-dev
```
Запуск сервера на 5000 порту.
```sh
cd task2
cd server
python3 server.py -p 5000
```
Запуск клиента на localhost с портом 5000 и логином - login.
```sh
cd ..
cd client
g++ -o client client.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network
./client localhost 5000 login
```
# Результат.
После запуска, в буффере с сервером будут отображться координаты игрока.
![image](https://user-images.githubusercontent.com/69481867/219181028-3f6973c0-d4ff-410e-bba8-941942b5c9be.png)
