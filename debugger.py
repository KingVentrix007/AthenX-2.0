# Host OS side (Python)
import os

named_pipe_path = 'pipe'
input_string = input(">");
input_string = input_string;
# Open the named pipe in write mode
with open(named_pipe_path, 'wb') as pipe:
    data_to_send = input_string.encode('utf-8')
    pipe.write(data_to_send)
