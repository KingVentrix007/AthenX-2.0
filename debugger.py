def find_function_name_ld(address, map_file_path):
    try:
        current_function = None
        with open(map_file_path, 'r') as map_file:
            for line in map_file:
                line = line.strip()
                #print(line)
                if line.startswith("0x0"):
                    
                    print(f"Exception address {line.split(' ')[0]}")
                    if line.split(" ")[0] == address:
                        print("MAIN")
                        return current_function
    except FileNotFoundError:
        print(f"Map file not found: {map_file_path}")
    return None

# Get the exception address from the user
exception_address = (input("Enter the exception address (in hexadecimal): "))

# Example path to the .map file (replace with your file path)
map_file_path = "LDout.map"

function_name = find_function_name_ld(exception_address, map_file_path)
if function_name:
    print(f"Exception address 0x{exception_address:X} is in function: {function_name}")
else:
    print(f"Function not found for address 0x{exception_address}")
