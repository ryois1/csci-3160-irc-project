import socket
import threading
import tkinter as tk

class IRCClient:
    def __init__(self, server, port):
        self.server = server
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.receive_thread = threading.Thread(target=self.receive_messages)
        self.receive_thread.daemon = True  # Daemonize the thread to close with main thread

    def connect(self):
        try:
            self.sock.connect((self.server, self.port))
            print("Connected to", self.server)
        except Exception as e:
            print("Error:", e)

    def start_receive_thread(self):
        self.receive_thread.start()

    def send_message(self, message):
        try:
            self.sock.sendall(message.encode("utf-8"))
        except Exception as e:
            print("Error:", e)

    def receive_messages(self):
        while True:
            data = self.sock.recv(128)
            if not data:
                break
            try:
                cleaned = data.decode("utf-8", errors="ignore")
                message_box.insert(tk.END, cleaned)
            except UnicodeDecodeError:
                pass

def send_message():
    message = message_entry.get()
    if message:
        irc_client.send_message("[PYTHON]: "+message + "\0")
        message_entry.delete(0, tk.END)

def main():
    global irc_client
    global message_entry
    global message_box

    irc_client = IRCClient("localhost", 9000)
    irc_client.connect()

    root = tk.Tk()
    root.title("IRC Client")

    message_box = tk.Text(root, height=20, width=50)
    message_box.pack()

    message_entry = tk.Entry(root, width=50)
    message_entry.pack()

    send_button = tk.Button(root, text="Send", command=send_message)
    send_button.pack()

    irc_client.start_receive_thread()

    root.mainloop()

if __name__ == "__main__":
    main()
