import socket
from datetime import datetime
import time
import random
import sys
from os.path import dirname
sys.path.append(dirname(__file__))
import mockNikolaController

class NikolaTstServer:
    def create_server(self, host, port):
        # Create a TCP/IP socket
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        
        # Bind the socket to the address (host and port)
        server_address = (host, port)
        server_socket.bind(server_address)
        
        # Only one listener
        server_socket.listen(1)
        print(f"Server listening on {host}:{port}")
       
        nikolaCtr = mockNikolaController.NikolaController()

        try:
            while True:
                # Wait for a connection
                print("Waiting for a connection...")
                client_socket, client_address = server_socket.accept()
                try:
                    print(f"Connection from {client_address}")
                    running = True
                    # Receive the data in small chunks and echo it back
                    while True:
                        data = client_socket.recv(1024)
                        if data:
                            if data == b"TEMP?\x0d":
                                print(f"Received: {data.decode()}")
                                temp = nikolaCtr.read_temperature()
                                temp = "{:.1f}\x0d".format(temp)
                            else:
                                if running:
                                    temp = "RUNNING\x0d"
                                    running = False
                                else:
                                    temp = "STOPPED\x0d"
                                    running = True
                                    
                            print(temp)
                            client_socket.sendall(temp.encode())
                        else:
                            print(f"No data from {client_address}, closing connection.")
                            break
                finally:
                    # Clean up the connection
                    client_socket.close()
        except KeyboardInterrupt:
            print("Server terminated by user.")
        finally:
            server_socket.close()

if __name__ == "__main__":
    server = NikolaTstServer()
    server.create_server("localhost", 22222)
