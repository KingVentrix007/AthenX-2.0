import os

named_pipe_path = 'pipe'

# Open the named pipe in write mode
with open(named_pipe_path, 'wb') as pipe:
    user_input = input("Enter data to send to custom OS: ")
    data_to_send = user_input.encode('utf-8')
    pipe.write(data_to_send)
