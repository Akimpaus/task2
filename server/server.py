import argparse
import socket
import os

def run_server(port):
    HOST = 'localhost'

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((HOST, port))

    server_socket.listen(1)
    print(f'server: start on port {port}')

    while True:
        client_socket, address = server_socket.accept()
        print(f'server: connected with -> {address}')

        login = client_socket.recv(1024).decode().strip()
        print(f'server: got login -> {login}')

        if os.path.exists(f'{login}.txt'):
            print(f'server: user {login} exist')
        else:
            print(f'server: user {login} does not exist, creating file')
            open(f'{login}.txt', 'w').close()

        while True:
            data = client_socket.recv(1024).decode().strip()
            if not data:
                print('server: client disconnected')
                break
            print(f'server: got data -> {data}')
            with open(f'{login}.txt', 'a') as file:
                file.write(f'{data}\n')

        client_socket.close()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-p', '--port', help='port number', type=int, required=True)
    args = parser.parse_args()

    run_server(args.port)